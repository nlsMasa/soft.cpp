// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICEOBJECTLIST_HPP_
#define ECORE_BASICEOBJECTLIST_HPP_

#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/ENotifyingListBase.hpp"
#include "ecore/impl/ArrayEListBase.hpp"
#include "ecore/impl/Proxy.hpp"

namespace ecore::impl
{
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool proxies = false, bool unset = false>
    class BasicEObjectList
        : public ArrayEListBase<ENotifyingListBase<typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type>,
                            typename std::conditional<proxies, Proxy<T>, T>::type>
    {
    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        BasicEObjectList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_( false )
        {
        }

        virtual ~BasicEObjectList() = default;

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return owner_.lock();
        }

        virtual int getFeatureID() const
        {
            return featureID_;
        }

        virtual std::shared_ptr<EStructuralFeature> getFeature() const
        {
            auto owner = owner_.lock();
            return owner ? owner->eClass()->getEStructuralFeature( featureID_ ) : nullptr;
        }

        virtual bool isSet() const
        {
            return isSet_;
        }

        virtual void unset()
        {
            clear();
            bool oldIsSet = isSet_;
            isSet_ = false;
            createAndDispatchNotification( nullptr, ENotification::UNSET, oldIsSet, isSet_, -1 );
        }

    protected:
        std::shared_ptr<EObject> getOwner() const
        {
            return owner_.lock();
        }

        virtual void didChange()
        {
            isSet_ = true;
        }

        virtual std::shared_ptr<AbstractNotification> createNotification( ENotification::EventType eventType,
                                                                          const Any& oldValue,
                                                                          const Any& newValue,
                                                                          std::size_t position ) const
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( owner, eventType, featureID_, oldValue, newValue, position ) : nullptr;
        }

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const T& object,
                                                                const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            if constexpr( inverse )
            {
                if constexpr( opposite )
                    return object->getInternal().eInverseAdd( getOwner(), inverseFeatureID_, notifications );
                else
                    return object->getInternal().eInverseAdd( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
            }
            else
                return notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const T& object,
                                                                   const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            if constexpr( inverse )
            {
                if constexpr( opposite )
                    return object->getInternal().eInverseRemove( getOwner(), inverseFeatureID_, notifications );
                else
                    return object->getInternal().eInverseRemove( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
            }
            else
                return notifications;
        }

        virtual T resolve( std::size_t index, const Proxy<T>& p ) const
        {
            auto oldObject = p.get();
            if( oldObject && oldObject->eIsProxy() )
            {
                auto owner = getOwner();
                if( owner )
                {
                    auto resolved = owner->getInternal().eResolveProxy( oldObject );
                    auto newObject = derived_pointer_cast<typename T::element_type>( resolved );
                    if( newObject && newObject != oldObject )
                    {
                        p.setForced( newObject );
                        std::shared_ptr<ENotificationChain> notifications;
                        if constexpr( containement )
                        {
                            notifications = inverseRemove( oldObject, notifications );
                            notifications = inverseAdd( newObject, notifications );
                        }
                        createAndDispatchNotification( notifications, ENotification::RESOLVE, oldObject, newObject, index );
                        return newObject;
                    }
                }
            }
            return oldObject;
        }

    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        bool isSet_;
    };
} // namespace ecore::impl

#endif /* ECORE_EOBJECTELIST_HPP_ */
