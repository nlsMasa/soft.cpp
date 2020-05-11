#include "ecore/impl/DeepEqual.hpp"
#include "ecore/AnyCast.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EReference.hpp"
#include "ecore/impl/EObjectInternal.hpp"

using namespace ecore;
using namespace ecore::impl;

bool DeepEqual::equals( const std::shared_ptr<EObject>& eObj1, const std::shared_ptr<EObject>& eObj2 )
{
    // If the first object is null, the second object must be null.
    if( !eObj1 )
        return !eObj2;

    // We know the first object isn't null, so if the second one is, it can't be equal.
    if( !eObj2 )
        return false;

    // Both eObject1 and eObject2 are not null.
    // If eObject1 has been compared already...
    auto eObj1Mapped = objects_[eObj1];
    if( eObj1Mapped )
        // Then eObject2 must be that previous match.
        return eObj1Mapped == eObj2;

    // If eObject2 has been compared already...
    auto eObj2Mapped = objects_[eObj2];
    if( eObj2Mapped )
        // Then eObject1 must be that match.
        return eObj2Mapped == eObj1;

    // Neither eObject1 nor eObject2 have been compared yet.

    // If eObject1 and eObject2 are the same instance...
    if( eObj1 == eObj2 )
    {
        // Match them and return true.
        //
        objects_[eObj1] = eObj2;
        objects_[eObj2] = eObj1;
        return true;
    }

    // If eObject1 is a proxy...
    if( eObj1->eIsProxy() )
    {
        auto& eObj1Internal = eObj1->getInternal();
        auto& eObj2Internal = eObj2->getInternal();
        if( eObj1Internal.eProxyURI() == eObj2Internal.eProxyURI() )
        {
            objects_[eObj1] = eObj2;
            objects_[eObj2] = eObj1;
            return true;
        }
        else
            return false;
    }
    // If eObject1 isn't a proxy but eObject2 is, they can't be equal.
    else if( eObj2->eIsProxy() )
        return false;

    // If they don't have the same class, they can't be equal.
    auto eClass = eObj1->eClass();
    if( eClass != eObj2->eClass() )
        return false;

    // Assume from now on that they match.
    objects_[eObj1] = eObj2;
    objects_[eObj2] = eObj1;

    for( const auto& eAttribute : eClass->getEAttributes() )
    {
        if( !eAttribute->isDerived() && !equals( eObj1, eObj2, eAttribute ) )
        {
            objects_.erase( eObj1 );
            objects_.erase( eObj2 );
            return false;
        }
    }
    for( const auto& eReference : eClass->getEReferences() )
    {
        if( !eReference->isDerived() && !equals( eObj1, eObj2, eReference ) )
        {
            objects_.erase( eObj1 );
            objects_.erase( eObj2 );
            return false;
        }
    }

    // There's no reason they aren't equal, so they are.
    return true;
}

bool DeepEqual::equals( const std::shared_ptr<EList<std::shared_ptr<EObject>>>& l1,
                        const std::shared_ptr<EList<std::shared_ptr<EObject>>>& l2 )
{
    auto size = l1->size();
    if( size != l2->size() )
        return false;

    for( int i = 0; i < size; i++ )
    {
        auto eObj1 = l1->get( i );
        auto eObj2 = l2->get( i );
        if( !equals( eObj1, eObj2 ) )
            return false;
    }
    return true;
}

bool DeepEqual::equals( const std::shared_ptr<EObject>& eObj1,
                        const std::shared_ptr<EObject>& eObj2,
                        const std::shared_ptr<EAttribute>& eAttribute )
{
    auto isSet1 = eObj1->eIsSet( eAttribute );
    auto isSet2 = eObj2->eIsSet( eAttribute );
    if( isSet1 && isSet2 )
    {
        auto value1 = eObj1->eGet( eAttribute );
        auto value2 = eObj2->eGet( eAttribute );
        return value1 == value2;
    }
    return isSet1 == isSet2;
}

bool DeepEqual::equals( const std::shared_ptr<EObject>& eObj1,
                        const std::shared_ptr<EObject>& eObj2,
                        const std::shared_ptr<EReference>& eReference )
{
    auto isSet1 = eObj1->eIsSet( eReference );
    auto isSet2 = eObj2->eIsSet( eReference );
    if( isSet1 && isSet2 )
    {
        auto value1 = eObj1->eGet( eReference );
        auto value2 = eObj2->eGet( eReference );
        return eReference->isMany()
                   ? equals( anyListCast<std::shared_ptr<EObject>>( value1 ), anyListCast<std::shared_ptr<EObject>>( value2 ) )
                   : equals( anyObjectCast<std::shared_ptr<EObject>>( value1 ), anyObjectCast<std::shared_ptr<EObject>>( value2 ) );
    }
    return isSet1 == isSet2;
}
