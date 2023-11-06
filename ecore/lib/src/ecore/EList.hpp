// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#define ECORE_ELIST_HPP_

#include "ecore/EObjectHelpers.hpp"
#include "ecore/EObjectList.hpp"
#include "ecore/TypeTraits.hpp"

namespace ecore
{

    template <typename T>
    class EList : public std::conditional_t<IsSharedEObjectOrAny<T>::value, EObjectList<EList<T>, T>, List<T>>
    {
    public:
        typedef typename T ValueType;

        virtual ~EList() = default;

        /**
         * Allows treating an EList<T> as an EList<Q> (if T can be casted to Q dynamically)
         */
        template <typename Q>
        inline std::shared_ptr<EList<Q>> asEListOf()
        {
            auto l = std::static_pointer_cast<EList<T>>( shared_from_this() );
            return std::make_shared<detail::DelegateEList<EList<Q>, EList<T>>>( l );
        }

        template <typename Q>
        inline std::shared_ptr<const EList<Q>> asEListOf() const
        {
            auto l = std::static_pointer_cast<const EList<T>>( shared_from_this() );
            return std::make_shared<detail::ConstDelegateEList<EList<Q>, EList<T>>>( l );
        }
    };

    template <typename T>
    inline typename EList<T>::Iterator begin( const std::unique_ptr<EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator begin( const std::unique_ptr<const EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::Iterator end( const std::unique_ptr<EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator end( const std::unique_ptr<const EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::Iterator begin( const std::shared_ptr<EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator begin( const std::shared_ptr<const EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::Iterator end( const std::shared_ptr<EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator end( const std::shared_ptr<const EList<T>>& l )
    {
        return l->end();
    }

    namespace detail
    {

        template <typename LT, typename LQ>
        class ConstDelegateEList : public ConstDelegateCollection<LT, LQ>
        {
            typedef typename LT::ValueType T;
            typedef typename LQ::ValueType Q;
            typedef std::shared_ptr<const LQ> T_ListDelegate;

        public:
            ConstDelegateEList( const T_ListDelegate& delegate )
                : ConstDelegateCollection<LT, LQ>( delegate )
            {
            }

            virtual ~ConstDelegateEList()
            {
            }

            bool add( std::size_t pos, const T& e ) override
            {
                throw "UnsupportedOperationException";
            }

            bool addAll( std::size_t pos, const Collection<T>& l ) override
            {
                throw "UnsupportedOperationException";
            }

            void move( std::size_t newPos, const T& e ) override
            {
                throw "UnsupportedOperationException";
            }

            T move( std::size_t newPos, std::size_t oldPos ) override
            {
                throw "UnsupportedOperationException";
            }

            T set( std::size_t pos, const T& e ) override
            {
                throw "UnsupportedOperationException";
            }

            T remove( std::size_t pos ) override
            {
                throw "UnsupportedOperationException";
            }

            std::size_t indexOf( const T& e ) const override
            {
                return delegate_->indexOf( cast<T, Q>::do_cast( e ) );
            }

            std::size_t indexOf( std::function<bool( const T& )> predicate ) const override
            {
                return delegate_->indexOf( [&predicate]( const Q& e ) { return predicate( cast<Q, T>::do_cast( e ) ); } );
            }

            std::shared_ptr<LT> getUnResolvedList() override
            {
                throw "UnsupportedOperationException";
            }

            std::shared_ptr<const LT> getUnResolvedList() const override
            {
                if constexpr( IsSharedEObjectOrAny<Q>::value )
                    return delegate_->getUnResolvedList()->asEListOf<T>();
                else
                    return delegate_->asEListOf<T>();
            }
        };

        template <typename LT, typename LQ>
        class DelegateEList : public DelegateCollection<LT, LQ>
        {
            typedef typename LT::ValueType T;
            typedef typename LQ::ValueType Q;
            typedef std::shared_ptr<LQ> T_Delegate;

        public:
            DelegateEList( const T_Delegate& delegate )
                : DelegateCollection<LT, LQ>( delegate )
            {
            }

            virtual ~DelegateEList()
            {
            }

            bool add( std::size_t pos, const T& e ) override
            {
                return delegate_->add( pos, cast<T, Q>::do_cast( e ) );
            }

            bool addAll( std::size_t pos, const Collection<T>& l ) override
            {
                auto transformed = l.asCollectionOf<Q>();
                return delegate_->addAll( pos, *transformed );
            }

            void move( std::size_t newPos, const T& e ) override
            {
                delegate_->move( newPos, cast<T, Q>::do_cast( e ) );
            }

            T move( std::size_t newPos, std::size_t oldPos ) override
            {
                return cast<Q, T>::do_cast( delegate_->move( newPos, oldPos ) );
            }

            T set( std::size_t pos, const T& e ) override
            {
                return cast<Q, T>::do_cast( delegate_->set( pos, cast<T, Q>::do_cast( e ) ) );
            }

            T remove( std::size_t pos ) override
            {
                return cast<Q, T>::do_cast( delegate_->remove( pos ) );
            }

            std::size_t indexOf( const T& e ) const override
            {
                return delegate_->indexOf( cast<T, Q>::do_cast( e ) );
            }

            std::size_t indexOf( std::function<bool( const T& )> predicate ) const override
            {
                std::function<bool( const Q& )> flatmap = [&predicate]( const Q& e ) { return predicate( cast<Q, T>::do_cast( e ) ); };
                return delegate_->indexOf( flatmap );
            }

            virtual std::shared_ptr<LT> getUnResolvedList()
            {
                if constexpr( IsSharedEObjectOrAny<Q>::value )
                    return delegate_->getUnResolvedList()->asEListOf<T>();
                else
                    return delegate_->asEListOf<T>();
            }

            virtual std::shared_ptr<const LT> getUnResolvedList() const
            {
                if constexpr( IsSharedEObjectOrAny<Q>::value )
                    return delegate_->getUnResolvedList()->asEListOf<T>();
                else
                    return delegate_->asEListOf<T>();
            }
        };

    } // namespace detail

} // namespace ecore

#endif /* ECORE_ELIST_HPP_ */
