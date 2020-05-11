#include "ecore/EContentAdapter.hpp"
#include "ecore/Any.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EReference.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/EObjectInternal.hpp"

#include <vector>

using namespace ecore;
using namespace ecore::impl;

void EContentAdapter::notifyChanged( const std::shared_ptr<ENotification>& notification )
{
    selfAdapt( notification );
}

void EContentAdapter::setTarget( const std::shared_ptr<ENotifier>& newTarget )
{
    AbstractAdapter::setTarget( newTarget );
    if( newTarget )
    {
        auto eObject = std::static_pointer_cast<EObject>( newTarget );
        auto eContents = eObject->eContents();
        for( auto& eContent : eContents )
            addAdapter( eContent );
    }    
}

void EContentAdapter::unsetTarget( const std::shared_ptr<ENotifier>& oldTarget )
{
    AbstractAdapter::unsetTarget( oldTarget );
    if( oldTarget )
    {
        auto eObject = std::static_pointer_cast<EObject>( oldTarget );
        auto eContents = eObject->eContents();
        for( auto& eContent : eContents )
            removeAdapter( eContent );
    }
}

void EContentAdapter::addAdapter( const std::shared_ptr<EObject>& eObject )
{
    if( !eObject )
        return;

    auto& eAdapters = eObject->eAdapters();
    if( !eAdapters.contains( this ) )
        eAdapters.add( this );
}

void EContentAdapter::removeAdapter( const std::shared_ptr<EObject>& eObject, bool checkContainer )
{
    if( !eObject )
        return;

    if( checkContainer )
    {
        auto container = eObject->getInternal().eInternalContainer();
        if( container && container->eAdapters().contains( this ) )
            return;
    }
    eObject->eAdapters().remove( this );
}

void EContentAdapter::selfAdapt( const std::shared_ptr<ENotification>& notification )
{
    auto feature = notification->getFeature();
    auto reference = std::dynamic_pointer_cast<EReference>( feature );
    if( reference && reference->isContainment() )
        handleContainment( notification );
}

void ecore::EContentAdapter::handleContainment( const std::shared_ptr<ENotification>& notification )
{
    switch( notification->getEventType() )
    {
    case ENotification::RESOLVE:
    {
        // We need to be careful that the proxy may be resolved while we are attaching this adapter.
        // We need to avoid attaching the adapter during the resolve
        // and also attaching it again as we walk the eContents() later.
        // Checking here avoids having to check during addAdapter.
        //
        auto oldValue = anyCast<std::shared_ptr<EObject>>( notification->getOldValue() );
        if( oldValue->eAdapters().contains( this ) )
        {
            removeAdapter( oldValue );
            auto newValue = anyCast<std::shared_ptr<EObject>>( notification->getNewValue() );
            addAdapter( newValue );
        }
        break;
    }
    case ENotification::UNSET:
    {
        auto oldValue = notification->getOldValue();
        if( oldValue.type() != typeid( bool ) )
        {
            removeAdapter( anyCast<std::shared_ptr<EObject>>( notification->getOldValue() ) );
            addAdapter( anyCast<std::shared_ptr<EObject>>( notification->getNewValue() ) );
        }
        break;
    }
    case ENotification::SET:
    {
        removeAdapter( anyCast<std::shared_ptr<EObject>>( notification->getOldValue() ) );
        addAdapter( anyCast<std::shared_ptr<EObject>>( notification->getNewValue() ) );
        break;
    }
    case ENotification::ADD:
    {
        addAdapter( anyCast<std::shared_ptr<EObject>>( notification->getNewValue() ) );
        break;
    }
    case ENotification::ADD_MANY:
    {
        std::vector<Any> newValues = anyCast<std::vector<Any>>( notification->getNewValue() );
        for( auto& newValue : newValues )
        {
            auto notifier = anyCast<std::shared_ptr<EObject>>( newValue );
            addAdapter( notifier );
        }
        break;
    }
    case ENotification::REMOVE:
    {
        removeAdapter( anyCast<std::shared_ptr<EObject>>( notification->getOldValue() ) );
        break;
    }
    case ENotification::REMOVE_MANY:
    {
        std::vector<Any> oldValues = anyCast<std::vector<Any>>( notification->getOldValue() );
        for( auto& oldValue : oldValues )
        {
            auto notifier = anyCast<std::shared_ptr<EObject>>( oldValue );
            removeAdapter( notifier );
        }
        break;
    }
    }
}
