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
            : owner_(owner)
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

        virtual std::shared_ptr<EList<T>> getUnResolvedList()
        {
            if constexpr( proxies )
                return std::make_shared<UnResolvedList<BasicEObjectList>>( this );
            else
                // no need to create a delegate list if we don't have proxies
                // return this
                return std::static_pointer_cast<EList<T>>( shared_from_this() );
        }

        virtual std::shared_ptr<const EList<T>> getUnResolvedList() const
        {
            if constexpr( proxies )
                return std::make_shared<ConstUnResolvedList<BasicEObjectList>>( this );
            else
                // no need to create a delegate list if we don't have proxies
                // return this
                return std::static_pointer_cast<const EList<T>>( shared_from_this() );
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
        

        template <typename C>
        class UnResolvedList : public ENotifyingListBase<typename C::InterfaceType, true>
        {
        public:
            typedef typename C::InterfaceType InterfaceType;
            typedef typename C::ValueType ValueType;

            UnResolvedList( C* list )
                : list_( list )
            {
            }

            virtual ~UnResolvedList()
            {
            }

            virtual bool empty() const
            {
                return list_->empty();
            }

            virtual std::size_t size() const
            {
                return list_->size();
            }

            virtual bool contains( const ValueType& e ) const
            {
                return indexOf( e ) != -1;
            }

            virtual std::size_t indexOf( const ValueType& e ) const
            {
                return list_->unresolvedIndexOf( e );
            }

            virtual std::shared_ptr<ENotifier> getNotifier() const
            {
                return list_->getNotifier();
            }

            virtual int getFeatureID() const
            {
                return list_->getFeatureID();
            }

            virtual std::shared_ptr<EStructuralFeature> getFeature() const
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
            
            virtual ValueType doGet( std::size_t index ) const
            {
                return list_->unresolvedGet( index );
            }

            virtual T doSet( std::size_t index, const ValueType& e )
            {
                return list_->doSet( index, e );
            }

            virtual void doAdd( const ValueType& e )
            {
                return list_->doAdd( e );
            }

            virtual void doAdd( std::size_t index, const ValueType& e )
            {
                return list_->doAdd( index, e );
            }

            virtual bool doAddAll( std::size_t index, const Collection<ValueType>& l )
            {
                return list_->doAddAll( index, l );
            }

            virtual T doRemove( std::size_t index )
            {
                return list_->doRemove( index );
            }

            virtual T doMove( std::size_t newIndex, std::size_t oldIndex )
            {
                return list_->doMove( newIndex, oldIndex );
            }

            virtual std::shared_ptr<EList<ValueType>> doClear()
            {
                return list_->doClear();
            }

            virtual std::shared_ptr<ENotificationChain> inverseAdd( const ValueType& object,
                                                                    const std::shared_ptr<ENotificationChain>& notifications ) const
            {
                return list_->inverseAdd( object, notifications );
            }

            virtual std::shared_ptr<ENotificationChain> inverseRemove( const ValueType& object,
                                                                       const std::shared_ptr<ENotificationChain>& notifications ) const
            {
                return list_->inverseRemove( object, notifications );
            }

        private:
            C* list_;
        };

        template <typename C>
        class ConstUnResolvedList : public UnResolvedList<C>
        {
        public:
            typedef typename C::InterfaceType InterfaceType;
            typedef typename C::ValueType ValueType;

            ConstUnResolvedList( const C* list )
                : UnResolvedList<C>( const_cast<C*>( list ) )
            {
            }

            virtual ~ConstUnResolvedList()
            {
            }

            // EList public non const methods
            virtual bool add( const ValueType& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool addAll( const Collection<ValueType>& l )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool add( std::size_t index, const ValueType& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool addAll( std::size_t index, const Collection<ValueType>& l )
            {
                throw "UnsupportedOperationException";
            }

            virtual ValueType move( std::size_t newIndex, std::size_t oldIndex )
            {
                throw "UnsupportedOperationException";
            }

            virtual void move( std::size_t newIndex, const ValueType& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual ValueType set( std::size_t index, const ValueType& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool remove( const ValueType& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual ValueType remove( std::size_t index )
            {
                throw "UnsupportedOperationException";
            }

            virtual void clear()
            {
                throw "UnsupportedOperationException";
            }

            // NotifyingEList public non const methods
            virtual std::shared_ptr<ENotificationChain> add( const ValueType& t, const std::shared_ptr<ENotificationChain>& notifications )
            {
                throw "UnsupportedOperationException";
            }

            virtual std::shared_ptr<ENotificationChain> remove( const ValueType& t,
                                                                const std::shared_ptr<ENotificationChain>& notifications )
            {
                throw "UnsupportedOperationException";
            }

            virtual std::shared_ptr<ENotificationChain> set( std::size_t index,
                                                             const ValueType& t,
                                                             const std::shared_ptr<ENotificationChain>& notifications )
            {
                throw "UnsupportedOperationException";
            }

            // EUnsettableList non const methods
            virtual void unset()
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
