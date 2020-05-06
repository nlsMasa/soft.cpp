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
        template <typename U>
        using IsEObject = std::disjunction<std::is_same<U, Any>,
                                           std::conjunction<is_shared_ptr<U>, std::is_base_of<EObject, typename U::element_type>>>;

        template <typename T, typename = void>
        class EList : public List<T>
        {
        public:
            typedef typename T ValueType;

            virtual ~EList() = default;
        };

        template <typename T>
        class EList<T, typename IsEObject<T>::type> : public List<T>
        {
        public:
            typedef typename T ValueType;

            virtual ~EList() = default;

            virtual std::shared_ptr<EList<T>> getUnResolvedList() = 0;

            virtual std::shared_ptr<const EList<T>> getUnResolvedList() const = 0;
        };

        template <typename LT, typename LQ>
        class ConstDelegateEList : public LT
        {
            typedef typename LT::ValueType T;
            typedef typename LQ::ValueType Q;
            typedef std::shared_ptr<const LQ> T_ListDelegate;

        public:
            ConstDelegateEList( const T_ListDelegate& delegate )
                : delegate_( delegate )
            {
                _ASSERTE( delegate_ );
            }

            virtual ~ConstDelegateEList()
            {
            }

            virtual bool add( const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool add( std::size_t pos, const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool addAll( const Collection<T>& l )
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

            virtual T get( std::size_t pos ) const
            {
                return cast<Q, T>::do_cast( delegate_->get( pos ) );
            }

            virtual T set( std::size_t pos, const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual T remove( std::size_t pos )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool remove( const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual std::size_t size() const
            {
                return delegate_->size();
            }

            virtual void clear()
            {
                throw "UnsupportedOperationException";
            }

            virtual bool empty() const
            {
                return delegate_->empty();
            }

            virtual bool contains( const T& e ) const
            {
                return delegate_->contains( cast<T, Q>::do_cast( e ) );
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
                return delegate_->getUnResolvedList()->asListOf<Q>();
            }

        protected:
            T_ListDelegate delegate_;

            template <typename A, typename B>
            struct cast
            {
                static inline B do_cast( const A& a )
                {
                    return static_cast<B>( a );
                }
            };

            template <typename A, typename B>
            struct cast<std::shared_ptr<A>,std::shared_ptr<B>>
            {
                static inline B do_cast( const A& a )
                {
                    return derived_pointer_cast<typename B::element_type, typename A::element_type>( a );
                }
            };


            template <typename A>
            struct cast<A, A>
            {
                static inline A do_cast( const A& a )
                {
                    return a;
                }
            };

            template <typename A>
            struct cast<A, Any>
            {
                static inline Any do_cast( const A& a )
                {
                    return a;
                }
            };

            template <typename A>
            struct cast<Any, A>
            {
                static inline A do_cast( const Any& a )
                {
                    return anyCast<A>( a );
                }
            };
        };

        template <typename LT, typename LQ>
        class DelegateEList : public ConstDelegateEList<LT, LQ>
        {
            typedef typename LT::ValueType T;
            typedef typename LQ::ValueType Q;
            typedef std::shared_ptr<LQ> T_ListDelegate;

        public:
            DelegateEList( const T_ListDelegate& delegate )
                : ConstDelegateEList<T, Q>( delegate )
                , delegate_( delegate )
            {
                _ASSERTE( delegate_ );
            }

            virtual ~DelegateEList()
            {
            }

            virtual bool add( const T& e )
            {
                return delegate_->add( cast<T, Q>::do_cast( e ) );
            }

            virtual bool add( std::size_t pos, const T& e )
            {
                return delegate_->add( pos, cast<T, Q>::do_cast( e ) );
            }

            virtual bool addAll( const Collection<T>& l )
            {
                auto transformed = const_cast<EList<T>&>( l ).asEListOf<Q>();
                return delegate_->addAll( *transformed );
            }

            virtual bool addAll( std::size_t pos, const Collection<T>& l )
            {
                auto transformed = const_cast<EList<T>&>( l ).asEListOf<Q>();
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

            virtual bool remove( const T& e )
            {
                return delegate_->remove( cast<T, Q>::do_cast( e ) );
            }

            virtual void clear()
            {
                delegate_->clear();
            }

            virtual std::shared_ptr<EList<T>> getUnResolvedList()
            {
                return delegate_->getUnResolvedList()->asListOf<Q>();
            }

        private:
            T_ListDelegate delegate_;
        };

    } // namespace detail

    template <typename T>
    class EList : public detail::EList<T, void>, public std::enable_shared_from_this<EList<T>>
    {
    public:
        virtual ~EList() = default;

        /**
         * Allows treating an EList<T> as an EList<Q> (if T can be casted to Q dynamically)
         */
        template <typename Q>
        inline std::shared_ptr<EList<Q>> asEListOf()
        {
            return std::make_shared<detail::DelegateEList<EList<Q>, EList<T>>>( shared_from_this() );
        }

        template <typename Q>
        inline std::shared_ptr<const EList<Q>> asEListOf() const
        {
            return std::make_shared<detail::ConstDelegateEList<EList<Q>, EList<T>>>( shared_from_this() );
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
