// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#define ECORE_ELIST_HPP_

#include "ecore/Any.hpp"
#include "ecore/List.hpp"
#include "ecore/TypeTraits.hpp"
#include <memory>

namespace ecore
{
    class EObject;

    namespace detail
    {
        template <typename T>
        struct IsSharedEObject : std::false_type
        {
        };

        template <typename T>
        struct IsSharedEObject<std::shared_ptr<T>> : std::is_base_of<EObject, typename T>
        {
        };

        template <typename T>
        using IsEObject = std::disjunction<std::is_same<T, Any>, IsSharedEObject<T>>;

        template <typename L, typename T, typename = void>
        class EList : public List<T>
        {
        public:
            virtual ~EList() = default;
        };

        template <typename L, typename T>
        class EList<L, T, typename std::enable_if<IsEObject<T>::value>::type> : public List<T>
        {
        public:
            virtual ~EList() = default;

            virtual std::shared_ptr<L> getUnResolvedList() = 0;

            virtual std::shared_ptr<const L> getUnResolvedList() const = 0;
        };

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

            virtual bool add( std::size_t pos, const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool addAll( std::size_t pos, const Collection<T>& l )
            {
                throw "UnsupportedOperationException";
            }

            virtual void move( std::size_t newPos, const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual T move( std::size_t newPos, std::size_t oldPos )
            {
                throw "UnsupportedOperationException";
            }

            virtual T set( std::size_t pos, const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual T remove( std::size_t pos )
            {
                throw "UnsupportedOperationException";
            }

            virtual std::size_t indexOf( const T& e ) const
            {
                return delegate_->indexOf( cast<T, Q>::do_cast( e ) );
            }

            virtual std::shared_ptr<LT> getUnResolvedList()
            {
                throw "UnsupportedOperationException";
            }

            virtual std::shared_ptr<const LT> getUnResolvedList() const
            {
                if constexpr( IsEObject<Q>::value )
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

            virtual bool add( std::size_t pos, const T& e )
            {
                return delegate_->add( pos, cast<T, Q>::do_cast( e ) );
            }

            virtual bool addAll( std::size_t pos, const Collection<T>& l )
            {
                auto transformed = l.asCollectionOf<Q>();
                return delegate_->addAll( pos, *transformed );
            }

            virtual void move( std::size_t newPos, const T& e )
            {
                delegate_->move( newPos, cast<T, Q>::do_cast( e ) );
            }

            virtual T move( std::size_t newPos, std::size_t oldPos )
            {
                return cast<Q, T>::do_cast( delegate_->move( newPos, oldPos ) );
            }

            virtual T set( std::size_t pos, const T& e )
            {
                return cast<Q, T>::do_cast( delegate_->set( pos, cast<T, Q>::do_cast( e ) ) );
            }

            virtual T remove( std::size_t pos )
            {
                return cast<Q, T>::do_cast( delegate_->remove( pos ) );
            }

            virtual std::size_t indexOf( const T& e ) const
            {
                return delegate_->indexOf( cast<T, Q>::do_cast( e ) );
            }

            virtual std::shared_ptr<LT> getUnResolvedList()
            {
                if constexpr( IsEObject<Q>::value )
                    return delegate_->getUnResolvedList()->asEListOf<T>();
                else
                    return delegate_->asEListOf<T>();
            }

            virtual std::shared_ptr<const LT> getUnResolvedList() const
            {
                if constexpr( IsEObject<Q>::value )
                    return delegate_->getUnResolvedList()->asEListOf<T>();
                else
                    return delegate_->asEListOf<T>();
            }
        };

    } // namespace detail

    template <typename T>
    class EList : public detail::EList<EList<T>, T>
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

} // namespace ecore

#endif /* ECORE_ELIST_HPP_ */
