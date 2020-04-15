// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTELIST_HPP_
#define ECORE_EOBJECTELIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/Proxy.hpp"

namespace ecore::impl
{

    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool proxies = false, bool unset = false>
    class EObjectEList : public AbstractENotifyingList<typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type,
                                                       typename std::conditional<proxies, Proxy<T>, T>::type>
    {
    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        EObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : EObjectEList( owner, featureID, inverseFeatureID, std::integral_constant<bool, proxies>() )
        {
        }

        virtual ~EObjectEList()
        {
        }

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
        virtual void didChange()
        {
            isSet_ = true;
        }

        std::shared_ptr<AbstractNotification> createNotification( ENotification::EventType eventType,
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

    private:
        EObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID, std::false_type )
            : AbstractENotifyingList()
            , owner_(owner)
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_(false)
        {

        }

        EObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID, std::true_type )
            : AbstractENotifyingList( [=]( std::size_t index, const Proxy<T>& p ) -> T { return resolve( index, p ); },
                                      [=]( std::size_t index, const T& t ) -> Proxy<T> { return proxify( index, t ); })
            , owner_(owner)
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_( false )
        {
        }

        std::shared_ptr<EObject> getOwner() const
        {
            return owner_.lock();
        }

        T resolve( std::size_t index, const Proxy<T>& p )
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

        Proxy<T> proxify( std::size_t index, const T& t )
        {
            return Proxy<T>( t );
        }

    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        bool isSet_;
    };
} // namespace ecore::impl

#endif /* ECORE_EOBJECTELIST_HPP_ */
