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
#include "ecore/impl/EObjectListBase.hpp"
#include "ecore/impl/Proxy.hpp"

namespace ecore::impl
{
    // BasicEObjectList defines a list of EObject
    // It handles containment,inverse,opposite,proxies and unset properties.
    // It handles unresolved list definition
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool proxies = false, bool unset = false>
    class BasicEObjectList : public EObjectListBase<std::conditional_t<unset, EUnsettableList<T>, ENotifyingList<T>>,
                                                    std::conditional_t<proxies, Proxy<T>, T>,
                                                    true>
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

        std::shared_ptr<ENotifier> getNotifier() const override
        {
            return owner_.lock();
        }

        int getFeatureID() const override
        {
            return featureID_;
        }

        std::shared_ptr<EStructuralFeature> getFeature() const override
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

        virtual std::shared_ptr<EList<T>> getUnResolvedList()
        {
            const auto thisAsList = static_pointer_cast<BasicEObjectList>( shared_from_this() );
            if constexpr( proxies )
                return std::make_shared<UnResolvedList<BasicEObjectList>>( thisAsList );
            else
                // no need to create a delegate list if we don't have proxies
                // return this
                return thisAsList;
        }

        virtual std::shared_ptr<const EList<T>> getUnResolvedList() const
        {
            const auto thisAsList = static_pointer_cast<const BasicEObjectList>( shared_from_this() );
            if constexpr( proxies )
                return std::make_shared<ConstUnResolvedList<BasicEObjectList>>( thisAsList );
            else
                // no need to create a delegate list if we don't have proxies
                // return this
                return thisAsList;
        }

    protected:
        std::shared_ptr<EObject> getOwner() const
        {
            return owner_.lock();
        }

        void didChange() override
        {
            isSet_ = true;
        }

        std::shared_ptr<AbstractNotification> createNotification( ENotification::EventType eventType,
                                                                  const Any& oldValue,
                                                                  const Any& newValue,
                                                                  std::size_t position ) const override
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( owner, eventType, featureID_, oldValue, newValue, position ) : nullptr;
        }

        std::shared_ptr<ENotificationChain> inverseAdd( const T& object,
                                                        const std::shared_ptr<ENotificationChain>& notifications ) const override
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

        std::shared_ptr<ENotificationChain> inverseRemove( const T& object,
                                                           const std::shared_ptr<ENotificationChain>& notifications ) const override
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
        template <typename C>
        class UnResolvedList : public ENotifyingListBase<typename C::InterfaceType, true>
        {
        public:
            typedef typename C::InterfaceType InterfaceType;
            typedef typename C::ValueType ValueType;

            UnResolvedList( const std::shared_ptr<C>& list )
                : list_( list )
            {
            }

            virtual ~UnResolvedList()
            {
            }

            bool empty() const override
            {
                return list_->empty();
            }

            std::size_t size() const override
            {
                return list_->size();
            }

            bool contains( const ValueType& e ) const override
            {
                return indexOf( e ) != -1;
            }

            std::size_t indexOf( const ValueType& e ) const override
            {
                return list_->unresolvedIndexOf( e );
            }

            std::size_t indexOf( std::function<bool( const ValueType& )> predicate ) const override
            {
                return list_->indexOf( predicate );
            }

            std::shared_ptr<ENotifier> getNotifier() const override
            {
                return list_->getNotifier();
            }

            int getFeatureID() const override
            {
                return list_->getFeatureID();
            }

            std::shared_ptr<EStructuralFeature> getFeature() const override
            {
                return list_->getFeature();
            }

            virtual bool isSet() const
            {
                return list_->isSet();
            }

            virtual void unset()
            {
                list_->unset();
            }

        protected:
            ValueType doGet( std::size_t index ) const override
            {
                return list_->unresolvedGet( index );
            }

            T doSet( std::size_t index, const ValueType& e ) override
            {
                return list_->doSet( index, e );
            }

            void doAdd( const ValueType& e ) override
            {
                return list_->doAdd( e );
            }

            void doAdd( std::size_t index, const ValueType& e ) override
            {
                return list_->doAdd( index, e );
            }

            bool doAddAll( std::size_t index, const Collection<ValueType>& l ) override
            {
                return list_->doAddAll( index, l );
            }

            T doRemove( std::size_t index ) override
            {
                return list_->doRemove( index );
            }

            T doMove( std::size_t newIndex, std::size_t oldIndex ) override
            {
                return list_->doMove( newIndex, oldIndex );
            }

            std::shared_ptr<EList<ValueType>> doClear() override
            {
                return list_->doClear();
            }

            std::shared_ptr<ENotificationChain> inverseAdd( const ValueType& object,
                                                            const std::shared_ptr<ENotificationChain>& notifications ) const override
            {
                return list_->inverseAdd( object, notifications );
            }

            std::shared_ptr<ENotificationChain> inverseRemove( const ValueType& object,
                                                               const std::shared_ptr<ENotificationChain>& notifications ) const override
            {
                return list_->inverseRemove( object, notifications );
            }

        protected:
            std::shared_ptr<C> list_;
        };

        template <typename C>
        class ConstUnResolvedList : public UnResolvedList<C>
        {
        public:
            typedef typename C::InterfaceType InterfaceType;
            typedef typename C::ValueType ValueType;

            ConstUnResolvedList( const std::shared_ptr<const C>& list )
                : UnResolvedList<C>( std::const_pointer_cast<C>( list ) )
            {
            }

            virtual ~ConstUnResolvedList()
            {
            }

            // EList public non const methods
            bool add( const ValueType& e ) override
            {
                throw "UnsupportedOperationException";
            }

            bool addAll( const Collection<ValueType>& l ) override
            {
                throw "UnsupportedOperationException";
            }

            bool add( std::size_t index, const ValueType& e ) override
            {
                throw "UnsupportedOperationException";
            }

            bool addAll( std::size_t index, const Collection<ValueType>& l ) override
            {
                throw "UnsupportedOperationException";
            }

            ValueType move( std::size_t newIndex, std::size_t oldIndex ) override
            {
                throw "UnsupportedOperationException";
            }

            void move( std::size_t newIndex, const ValueType& e ) override
            {
                throw "UnsupportedOperationException";
            }

            ValueType set( std::size_t index, const ValueType& e ) override
            {
                throw "UnsupportedOperationException";
            }

            bool remove( const ValueType& e ) override
            {
                throw "UnsupportedOperationException";
            }

            ValueType remove( std::size_t index ) override
            {
                throw "UnsupportedOperationException";
            }

            void clear() override
            {
                throw "UnsupportedOperationException";
            }

            std::size_t indexOf( std::function<bool( const ValueType& )> ) const override
            {
                throw "UnsupportedOperationException";
            }

            // NotifyingEList public non const methods
            std::shared_ptr<ENotificationChain> add( const ValueType& t, const std::shared_ptr<ENotificationChain>& notifications ) override
            {
                throw "UnsupportedOperationException";
            }

            std::shared_ptr<ENotificationChain> remove( const ValueType& t,
                                                        const std::shared_ptr<ENotificationChain>& notifications ) override
            {
                throw "UnsupportedOperationException";
            }

            std::shared_ptr<ENotificationChain> set( std::size_t index,
                                                     const ValueType& t,
                                                     const std::shared_ptr<ENotificationChain>& notifications ) override
            {
                throw "UnsupportedOperationException";
            }

            // EUnsettableList non const methods
            void unset() override
            {
                throw "UnsupportedOperationException";
            }
        };

    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        bool isSet_;
    };
} // namespace ecore::impl

#endif /* ECORE_EOBJECTELIST_HPP_ */
