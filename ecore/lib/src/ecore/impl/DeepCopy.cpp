#include "ecore/impl/DeepCopy.hpp"
#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EList.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/AnyCast.hpp"

#include <algorithm>

using namespace ecore;
using namespace ecore::impl;

DeepCopy::DeepCopy( bool resolve, bool originalReferences )
    : resolve_(resolve)
    , originalReferences_(originalReferences)
{
}

DeepCopy::~DeepCopy()
{
}

std::shared_ptr<EObject> DeepCopy::copy( const std::shared_ptr<EObject>& eObject )
{
    if( eObject )
    {
        auto copyEObject = createCopy( eObject ); 
        if( copyEObject )
        {
            objects_[ eObject ] = copyEObject;

            auto eClass = eObject->eClass();
            for( auto eFeature : eClass->getEStructuralFeatures() )
            {
                if( eFeature->isChangeable() && !eFeature->isDerived() )
                {
                    if( auto eAttribute = std::dynamic_pointer_cast<EAttribute>( eFeature ) )
                        copyAttribute( eAttribute, eObject, copyEObject );
                    if( auto eReference = std::dynamic_pointer_cast<EReference>( eFeature ) )
                    {
                        if( eReference->isContainment() )
                            copyContainment( eReference, eObject, copyEObject );
                    }
                }
            }
            copyProxyURI( eObject, copyEObject );
        }
        return copyEObject;
    }
    return nullptr;
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> DeepCopy::copyAll( const std::shared_ptr<EList<std::shared_ptr<EObject>>>& l )
{
    std::vector<std::shared_ptr<EObject>> v;
    std::transform( l->begin(), l->end(), v.end(), [&]( const std::shared_ptr<EObject>& o ) { return copy( o ); } );
    return std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>( std::move( v ) );
}

void DeepCopy::copyReferences()
{
}

std::shared_ptr<EObject> DeepCopy::createCopy( const std::shared_ptr<EObject>& eObject ) const
{
    auto eClass = eObject->eClass();
    auto eFactory = eClass->getEPackage()->getEFactoryInstance();
    return eFactory->create( eClass );    
}

void DeepCopy::copyAttribute( const std::shared_ptr<EAttribute>& eAttribute,
                              const std::shared_ptr<EObject>& eObject,
                              const std::shared_ptr<EObject>& copyEObject ) const
{
    if( eObject->eIsSet( eAttribute ) )
        copyEObject->eSet( eAttribute, eObject->eGet( eAttribute ) );
}

void DeepCopy::copyContainment( const std::shared_ptr<EReference>& eReference,
                                const std::shared_ptr<EObject>& eObject,
                                const std::shared_ptr<EObject>& copyEObject )
{
    if( eObject->eIsSet(eReference ) )
    {
        auto value = eObject->eGet( eReference );
        if( eReference->isMany() )
        {
            auto list = anyListCast<std::shared_ptr<EObject>>( value );
            copyEObject->eSet( eReference, copyAll( list ) );
        }
        else
        {
            auto object = anyObjectCast<std::shared_ptr<EObject>>( value );
            copyEObject->eSet( eReference, copy(object) );
        }
    }
}

void DeepCopy::copyProxyURI( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EObject>& copyEObject ) const
{
    if( eObject->eIsProxy() )
    {
        auto& eObjectInternal = eObject->getInternal();
        auto& eCopyInternal = copyEObject->getInternal();
        eCopyInternal.eSetProxyURI( eObjectInternal.eProxyURI() );
    }
}
