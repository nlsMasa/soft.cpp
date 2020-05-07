// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_
#define ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_

#include "ecore/Constants.hpp"
#include "ecore/ENotifier.hpp"
#include "ecore/TypeTraits.hpp"
#include "ecore/impl/EListBase.hpp"
#include "ecore/impl/BasicEList.hpp"
#include "ecore/impl/AbstractNotification.hpp"
#include "ecore/impl/NotificationChain.hpp"

#include <algorithm>
#include <memory>

namespace ecore::impl
{
    template <typename I>
    class AbstractENotifyingList : public EListBase<I, true>
    {
    public:
        typedef typename EListBase<I, true> Super;
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;

        AbstractENotifyingList()
            : Super()
        {
        }

        virtual ~AbstractENotifyingList()
        {
        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return nullptr;
        }

        virtual int getFeatureID() const
        {
            return ENotification::NO_FEATURE_ID;
        }

        virtual std::shared_ptr<EStructuralFeature> getFeature() const
        {
            return nullptr;
        }

        virtual bool add( const ValueType& e )
        {
            auto index = size();
            auto result = Super::add( e );
            if( result )
            {
                auto notifications = inverseAdd( e, nullptr );
                createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, toAny( e ), index );
            }
            return result;
        }

        virtual bool add( std::size_t index, const ValueType& e )
        {
            auto result = Super::add( index, e );
            if( result )
            {
                auto notifications = inverseAdd( e, nullptr );
                createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, toAny( e ), index );
            }
            return result;
        }

        virtual std::shared_ptr<ENotificationChain> add( const ValueType& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto index = size();
            auto result = Super::add( e );
            if( result )
            {
                auto n = inverseAdd( e, notifications );
                return createAndAddNotification( n, ENotification::ADD, NO_VALUE, toAny( e ), index );
            }
            return notifications;
        }

        virtual bool addAll( std::size_t index, const Collection<ValueType>& l )
        {
            bool result = Super::addAll( index, l );
            auto notifications = createNotificationChain();
            for( int i = 0; i < l.size(); ++i )
            {
                auto object = get( i + index );
                notifications = inverseAdd( object, notifications );
            }
            createAndDispatchNotification( notifications, [&]() {
                return l.size() == 1 ? createNotification( ENotification::ADD, NO_VALUE, toAny( l.get( 0 ) ), index )
                                     : createNotification( ENotification::ADD_MANY, NO_VALUE, toAny( l ), index );
            } );
            return result;
        }

        virtual ValueType remove( std::size_t index )
        {
            auto oldElement = Super::remove( index );
            auto notifications = inverseRemove( oldElement, nullptr );
            createAndDispatchNotification( notifications, ENotification::REMOVE, toAny( oldElement ), NO_VALUE, index );
            return oldElement;
        }

        virtual std::shared_ptr<ENotificationChain> remove( const ValueType& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            std::size_t index = indexOf( e );
            if( index != -1 )
            {
                auto oldElement = Super::remove( index );
                auto n = inverseRemove( oldElement, notifications );
                return createAndAddNotification( n, ENotification::REMOVE, toAny( oldElement ), NO_VALUE, index );
            }
            return notifications;
        }

        virtual ValueType set( std::size_t index, const ValueType& newElement )
        {
            auto oldElement = Super::set( index, newElement );
            if( newElement != oldElement )
            {
                auto n = inverseRemove( oldElement, nullptr );
                n = inverseAdd( newElement, n );
                createAndDispatchNotification( n, ENotification::SET, toAny( oldElement ), toAny( newElement ), index );
            }
            return oldElement;
        }

        virtual std::shared_ptr<ENotificationChain> set( std::size_t index,
                                                         const ValueType& newElement,
                                                         const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto oldElement = Super::set( index, newElement );
            if( newElement != oldElement )
            {
                auto n = notifications;
                n = inverseRemove( oldElement, n );
                n = inverseAdd( newElement, n );
                return createAndAddNotification( n, ENotification::SET, toAny( oldElement ), toAny( newElement ), index );
            }
            return notifications;
        }

        virtual ValueType move( std::size_t newPos, std::size_t oldPos )
        {
            auto element = Super::move( newPos, oldPos );
            createAndDispatchNotification( nullptr, ENotification::MOVE, oldPos, toAny( element ), newPos );
            return element;
        }

        virtual void clear()
        {
            auto l = doClear();
            if( l )
            {
                if( l->empty() )
                    createAndDispatchNotification( nullptr, ENotification::REMOVE_MANY, toAny(*l), NO_VALUE, -1 );
                else
                {
                    auto notifications = createNotificationChain();

                    for( const auto& e : *l )
                        notifications = inverseRemove( e, notifications );

                    createAndDispatchNotification( notifications, [&]() {
                        return l->size() == 1 ? createNotification( ENotification::REMOVE, toAny( l->get( 0 ) ), NO_VALUE, 0 )
                                              : createNotification( ENotification::ADD_MANY, toAny( *l ), NO_VALUE, -1 );
                    } );
                }
            }
        }

    protected:
        virtual std::shared_ptr<ENotificationChain> inverseAdd( const ValueType& object,
                                                                const std::shared_ptr<ENotificationChain>& notifications ) const = 0;

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const ValueType& object,
                                                                   const std::shared_ptr<ENotificationChain>& notifications ) const = 0;

        virtual std::shared_ptr<AbstractNotification> createNotification( ENotification::EventType eventType,
                                                                          const Any& oldValue,
                                                                          const Any& newValue,
                                                                          std::size_t position ) const
        {
            class Notification : public AbstractNotification
            {
            public:
                Notification( const AbstractENotifyingList& list,
                              ENotification::EventType eventType,
                              const Any& oldValue,
                              const Any& newValue,
                              std::size_t position )
                    : AbstractNotification( eventType, oldValue, newValue, position )
                    , list_( list )
                {
                }

                virtual std::shared_ptr<ENotifier> getNotifier() const
                {
                    return list_.getNotifier();
                }

                virtual std::shared_ptr<EStructuralFeature> getFeature() const
                {
                    return list_.getFeature();
                }

                virtual int getFeatureID() const
                {
                    return list_.getFeatureID();
                }

            private:
                const AbstractENotifyingList& list_;
            };

            return std::make_shared<Notification>( *this, eventType, oldValue, newValue, position );
        }

        virtual std::shared_ptr<ENotificationChain> createNotificationChain() const
        {
            return std::make_shared<NotificationChain>();
        }

        std::shared_ptr<ENotificationChain> createAndAddNotification( const std::shared_ptr<ENotificationChain>& ns,
                                                                      ENotification::EventType eventType,
                                                                      const Any& oldValue,
                                                                      const Any& newValue,
                                                                      std::size_t position ) const
        {
            std::shared_ptr<ENotificationChain> notifications = ns;
            if( isNotificationRequired() )
            {
                auto notification = createNotification( eventType, oldValue, newValue, position );
                if( notifications )
                    notifications->add( notification );
                else
                    notifications = notification;
            }
            return notifications;
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications,
                                            ENotification::EventType eventType,
                                            const Any& oldValue,
                                            const Any& newValue,
                                            std::size_t position ) const
        {
            createAndDispatchNotification( notifications, [&]() { return createNotification( eventType, oldValue, newValue, position ); } );
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications,
                                            std::function<std::shared_ptr<ENotification>()> notificationFactory ) const
        {
            if( isNotificationRequired() )
            {
                auto notification = notificationFactory();
                if( notifications )
                {
                    notifications->add( notification );
                    notifications->dispatch();
                }
                else if( auto notifier = getNotifier() )
                    notifier->eNotify( notification );
            }
            else
            {
                if( notifications )
                    notifications->dispatch();
            }
        }

        bool isNotificationRequired() const
        {
            auto notifier = getNotifier();
            return notifier && notifier->eDeliver() && !notifier->eAdapters().empty();
        }

    private:
        static Any toAny( const ValueType& v )
        {
            if constexpr( is_shared_ptr<ValueType>::value && !std::is_same<ecore::EObject, typename ValueType::element_type>::value
                          && std::is_base_of<ecore::EObject, typename ValueType::element_type>::value )
                return Any( std::static_pointer_cast<EObject>( v ) );
            else
                return Any( v );
        }

        static Any toAny( const Collection<ValueType>& l )
        {
            auto result = std::make_shared<BasicEList<Any>>();
            std::transform( l.begin(), l.end(), result->end(), []( const ValueType& i ) { return toAny( i ); } );
            return std::static_pointer_cast<EList<Any>>(result);
        }
    };
} // namespace ecore::impl

#endif /* ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_ */
