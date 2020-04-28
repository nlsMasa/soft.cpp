// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASIC_NOTIFIER_HPP_
#define ECORE_BASIC_NOTIFIER_HPP_

#include "ecore/EAdapter.hpp"
#include "ecore/impl/BasicEList.hpp"
#include <memory>

namespace ecore::impl
{

    template <typename... I>
    class BasicNotifier : public I...
    {
    public:
        virtual ~BasicNotifier() = default;

        virtual EList<EAdapter*>& eAdapters() const
        {
            return *eAdapters_;
        }

        virtual bool eDeliver() const
        {
            return eDeliver_;
        }

        virtual void eSetDeliver( bool deliver )
        {
            eDeliver_ = deliver;
        }

        virtual void eNotify( const std::shared_ptr<ENotification>& notification ) const
        {
            for( auto eAdapter : *eAdapters_ )
                eAdapter->notifyChanged( notification );
        }

        bool eNotificationRequired() const
        {
            return eDeliver_ && eAdapters_->size() > 0;
        }

        inline void setThisPtr( const std::shared_ptr<BasicNotifier>& thisPtr )
        {
            thisPtr_ = thisPtr;
        }

        inline std::shared_ptr<BasicNotifier> getThisPtr() const
        {
            return thisPtr_.lock();
        }

    private:
        class AdapterList : public BasicEList<EAdapter*>
        {
        public:
            AdapterList( BasicNotifier& notifier )
                : notifier_( notifier )
            {
            }

        protected:
            virtual void didAdd( std::size_t pos, const ValueType& adapter ) override
            {
                auto notifier = notifier_.thisPtr_.lock();
                auto eAdapter = const_cast<EAdapter*>( adapter );
                eAdapter->setTarget( notifier );
            }

            virtual void didRemove( std::size_t pos, const ValueType& adapter ) override
            {
                auto notifier = notifier_.thisPtr_.lock();
                auto eAdapter = const_cast<EAdapter*>( adapter );
                
                //  notify removing adapter
                if( notifier_.eDeliver_ )
                    eAdapter->notifyChanged(
                        std::make_shared<Notification>( *this, Notification::REMOVING_ADAPTER, eAdapter, NO_VALUE, pos ) );

                // unset target
                eAdapter->unsetTarget( notifier );
            }

        private:
            class Notification : public AbstractNotification
            {
            public:
                Notification( AdapterList& list, EventType type, const Any& oldValue, const Any& newValue, std::size_t position = NO_INDEX )
                    : AbstractNotification( type, oldValue, newValue, position )
                    , list_( list )
                {
                }
                virtual std::shared_ptr<ENotifier> getNotifier() const
                {
                    auto& notifier = list_.notifier_;
                    return notifier.thisPtr_.lock();
                }

                virtual int getFeatureID() const
                {
                    return -1;
                }

                virtual std::shared_ptr<EStructuralFeature> getFeature() const
                {
                    return nullptr;
                }

            private:
                AdapterList& list_;
            };

        private:
            BasicNotifier& notifier_;
        };

    protected:
        std::weak_ptr<BasicNotifier> thisPtr_;
        std::unique_ptr<EList<EAdapter*>> eAdapters_{new AdapterList( *this )};
        bool eDeliver_{true};
    };
} // namespace ecore::impl

#endif /* ECORE_ABSTRACT_NOTIFIER_HPP_ */
