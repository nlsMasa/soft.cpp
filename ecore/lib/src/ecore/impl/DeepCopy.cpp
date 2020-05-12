#include "ecore/impl/DeepCopy.hpp"
#include "ecore/AnyCast.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/impl/EObjectInternal.hpp"

#include <algorithm>

using namespace ecore;
using namespace ecore::impl;

DeepCopy::DeepCopy( bool resolve, bool originalReferences )
    : resolve_( resolve )
    , originalReferences_( originalReferences )
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
            objects_[eObject] = copyEObject;

            auto eClass = eObject->eClass();
            for( const auto& eAttribute : eClass->getEAttributes() )
            {
                if( eAttribute->isChangeable() && !eAttribute->isDerived() )
                    copyAttribute( eAttribute, eObject, copyEObject );
            }
            for( const auto& eReference : eClass->getEReferences() )
            {
                if( eReference->isChangeable() && !eReference->isDerived() && eReference->isContainment() )
                    copyContainment( eReference, eObject, copyEObject );
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
    std::transform( l->begin(), l->end(), std::back_inserter(v), [&]( const std::shared_ptr<EObject>& o ) { return copy( o ); } );
    return std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>( std::move( v ) );
}

void DeepCopy::copyReferences()
{
    for( const auto& entry : objects_ )
    {
        auto eObject = entry.first;
        auto copyEObject = entry.second;
        for( const auto& eReference : eObject->eClass()->getEReferences() )
        {
            if( eReference->isChangeable() && !eReference->isDerived() && !eReference->isContainment() && !eReference->isContainer() )
                copyReference( eReference, eObject, copyEObject );
        }
    }
}

void ecore::impl::DeepCopy::copyReference( const std::shared_ptr<EReference>& eReference,
                                           const std::shared_ptr<EObject>& eObject,
                                           const std::shared_ptr<EObject>& copyEObject )
{
    if( eObject->eIsSet( eReference ) )
    {
        auto value = eObject->eGet( eReference, resolve_ );
        if( eReference->isMany() )
        {
            auto listSource = anyListCast<std::shared_ptr<EObject>>( value );
            auto listTarget = anyListCast<std::shared_ptr<EObject>>( copyEObject->eGet( eReference, false ) );
            auto source = resolve_ ? listSource : listSource->getUnResolvedList();
            auto target = listTarget->getUnResolvedList();
            if( source->empty() )
                target->clear();
            else
            {
                bool isBidirectional = eReference->getEOpposite() != nullptr;
                std::size_t index = 0;
                for( const auto& referencedObject : source )
                {
                    auto copyReferencedEObject = objects_[referencedObject];
                    if( copyReferencedEObject )
                    {
                        if( isBidirectional )
                        {
                            auto position = target->indexOf( copyReferencedEObject );
                            if( position == -1 )
                                target->add( index, copyReferencedEObject );
                            else if( index != position )
                                target->move( index, copyReferencedEObject );
                        }
                        else
                            target->add( index, copyReferencedEObject );
                        ++index;
                    }
                    else
                    {
                        if( originalReferences_ && !isBidirectional )
                        {
                            target->add( index, referencedObject );
                            index++;
                        }
                    }
                }
            }
        }
        else
        {
            auto object = anyObjectCast<std::shared_ptr<EObject>>( value );
            if( object )
            {
                auto copyReferencedEObject = objects_[object];
                if( copyReferencedEObject )
                    copyEObject->eSet( eReference, copyReferencedEObject );
                else
                {
                    if( originalReferences_ && !eReference->getEOpposite() )
                        copyEObject->eSet( eReference, object );
                }
            }
            else
                copyEObject->eSet( eReference, object );
        }
    }
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
    if( eObject->eIsSet( eReference ) )
    {
        auto value = eObject->eGet( eReference, resolve_ );
        if( eReference->isMany() )
        {
            auto list = anyListCast<std::shared_ptr<EObject>>( value );
            copyEObject->eSet( eReference, copyAll( list ) );
        }
        else
        {
            auto object = anyObjectCast<std::shared_ptr<EObject>>( value );
            copyEObject->eSet( eReference, copy( object ) );
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
