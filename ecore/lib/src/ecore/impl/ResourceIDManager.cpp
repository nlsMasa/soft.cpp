#include "ecore/impl/ResourceIDManager.hpp"
#include "ecore/EcoreUtils.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EList.hpp"

using namespace ecore;
using namespace ecore::impl;

ResourceIDManager::ResourceIDManager()
{
}

ResourceIDManager::~ResourceIDManager()
{
}

void ResourceIDManager::registerObject( const std::shared_ptr<EObject>& eObject )
{
    auto id = EcoreUtils::getID( eObject );
    if( !id.empty() )
    {
        objectToID_[eObject] = id;
        idToObject_[id] = eObject;
    }
    auto contents = eObject->eContents()->getUnResolvedList();
    for( const auto& child : contents )
    {
        registerObject( child );
    }
}

void ResourceIDManager::unregisterObject( const std::shared_ptr<EObject>& eObject )
{
    auto it = objectToID_.find( eObject );
    if( it != objectToID_.end() )
    {
        const auto& id = it->second;
        idToObject_.erase( id );
        objectToID_.erase( it );
    }
    auto contents = eObject->eContents()->getUnResolvedList();
    for( const auto& child : contents )
    {
        unregisterObject( child );
    }
}

std::string ResourceIDManager::getID( const std::shared_ptr<EObject>& eObject ) const
{
    auto it = objectToID_.find( eObject );
    return it != objectToID_.end() ? it->second : "";
}

std::shared_ptr<EObject> ResourceIDManager::getEObject( const std::string& id ) const
{
    auto it = idToObject_.find( id );
    return it != idToObject_.end() ? it->second : nullptr;
}
