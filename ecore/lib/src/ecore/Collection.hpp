// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_COLLECTION_HPP_
#define ECORE_COLLECTION_HPP_

#include "ecore/Assert.hpp"
#include <memory>

namespace ecore
{
    template <typename T>
    class Collection;

    namespace detail
    {
        template <typename CT, typename CQ>
        class ConstDelegateCollection : public CT
        {
            typedef typename CT::ValueType T;
            typedef typename CQ::ValueType Q;
            typedef std::shared_ptr<const CQ> T_Delegate;

        public:
            ConstDelegateCollection( const T_Delegate& delegate )
                : delegate_( delegate )
            {
                _ASSERTE( delegate_ );
            }

            virtual ~ConstDelegateCollection()
            {
            }

            virtual bool add( const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual bool addAll( const Collection<T>& l )
            {
                throw "UnsupportedOperationException";
            }

            virtual T get( std::size_t pos ) const
            {
                return cast<Q, T>::do_cast( delegate_->get( pos ) );
            }

            virtual bool remove( const T& e )
            {
                throw "UnsupportedOperationException";
            }

            virtual void clear()
            {
                throw "UnsupportedOperationException";
            }

            virtual std::size_t size() const
            {
                return delegate_->size();
            }

            virtual bool empty() const
            {
                return delegate_->empty();
            }

            virtual bool contains( const T& e ) const
            {
                return delegate_->contains( cast<T, Q>::do_cast( e ) );
            }

        protected:
            T_Delegate delegate_;

            template <typename A, typename B>
            struct cast
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

        template <typename CT, typename CQ>
        class DelegateCollection : public ConstDelegateCollection<CT, CQ>
        {
            typedef typename CT::ValueType T;
            typedef typename CQ::ValueType Q;
            typedef std::shared_ptr<CQ> T_Delegate;

        public:
            DelegateCollection( const T_Delegate& delegate )
                : ConstDelegateCollection<CT, CQ>( delegate )
                , delegate_( delegate )
            {
                _ASSERTE( delegate_ );
            }

            virtual ~DelegateCollection()
            {
            }

            virtual bool add( const T& e )
            {
                return delegate_->add( cast<T, Q>::do_cast( e ) );
            }

            virtual bool addAll( const Collection<T>& l )
            {
                auto transformed = l.asCollectionOf<Q>();
                return delegate_->addAll( *transformed );
            }

            virtual bool remove( const T& e )
            {
                return delegate_->remove( cast<T, Q>::do_cast( e ) );
            }

            virtual void clear()
            {
                delegate_->clear();
            }


        protected:
            T_Delegate delegate_;
        };

    } // namespace detail
  
    template <typename T>
    class Collection : public std::enable_shared_from_this<Collection<T>>
    {
    public:
        typedef typename T ValueType;

        virtual ~Collection() = default;

        virtual bool add( const T& e ) = 0;

        virtual bool addAll( const Collection<T>& l ) = 0;

        virtual bool remove( const T& e ) = 0;

        virtual T get( std::size_t index ) const = 0;

        virtual std::size_t size() const = 0;

        virtual void clear() = 0;

        virtual bool empty() const = 0;

        virtual bool contains( const T& e ) const = 0;

        /** Iterator interfaces for an Collection<T>. */
        template <typename CollectionType>
        class CollectionIterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = typename CollectionType::ValueType;
            using pointer = typename CollectionType::ValueType*;
            using reference = typename CollectionType::ValueType&;

        public:
            CollectionIterator( CollectionType* c, std::size_t index )
                : c_( c )
                , index_( index )
            {
            }

            value_type operator*() const
            {
                return c_->get( index_ );
            }

            CollectionIterator& operator--()
            {
                --index_;
                return *this;
            }

            CollectionIterator operator--( int )
            {
                CollectionIterator old( *this );
                --( *this );
                return old;
            }

            CollectionIterator& operator++()
            {
                ++index_;
                return *this;
            }

            CollectionIterator operator++( int )
            {
                CollectionIterator old( *this );
                ++( *this );
                return old;
            }

            CollectionIterator& operator+=( difference_type offset )
            {
                index_ += offset;
                return ( *this );
            }

            CollectionIterator& operator-=( difference_type offset )
            {
                return ( *this += -offset );
            }

            CollectionIterator operator+( const difference_type& offset ) const
            {
                CollectionIterator tmp = *this;
                return ( tmp += offset );
            }

            CollectionIterator operator-( const difference_type& rhs ) const
            {
                CollectionIterator tmp = *this;
                return ( tmp -= offset );
            }

            difference_type operator-( const CollectionIterator& rhs ) const
            {
                _Compat( rhs );
                return index_ - rhs.index_;
            }

            bool operator==( const CollectionIterator& rhs ) const
            {
                _Compat( rhs );
                return ( index_ == rhs.index_ );
            }

            bool operator!=( const CollectionIterator& rhs ) const
            {
                return !( *this == rhs );
            }

            bool operator<( const CollectionIterator& rhs )
            {
                _Compat( rhs );
                return index_ < rhs.index_;
            }

            bool operator>( const CollectionIterator& rhs )
            {
                return ( rhs < *this );
            }

            bool operator<=( const CollectionIterator& rhs )
            {
                return ( !( rhs < *this ) );
            }

            bool operator>=( const CollectionIterator& rhs )
            {
                return ( !( *this < rhs ) );
            }

            bool hasNext() const
            {
                return ( (int64_t)index_ < (int64_t)c_->size() - 1 );
            }

            const CollectionType* getEList() const
            {
                return c_;
            }

            std::size_t getIndex() const
            {
                return index_;
            }

        private:
            void _Compat( const CollectionIterator& rhs ) const
            {
#if _ITERATOR_DEBUG_LEVEL == 0
                (void)rhs;
#else
                VERIFY( c_ == rhs.c_, "collection iterators incompatible" );
#endif
            }

        private:
            CollectionType* c_;
            std::size_t index_;
        };

        typedef CollectionIterator<Collection<T>> Iterator;
        typedef CollectionIterator<const Collection<T>> ConstIterator;

        Iterator begin()
        {
            return Iterator( this, 0 );
        }

        ConstIterator begin() const
        {
            return ConstIterator( this, 0 );
        }

        Iterator end()
        {
            return Iterator( this, size() );
        }

        ConstIterator end() const
        {
            return ConstIterator( this, size() );
        }

        ConstIterator cbegin() const
        {
            return begin();
        }

        ConstIterator cend() const
        {
            return end();
        }

        /**
         * Allows treating an EList<T> as an EList<Q> (if T can be casted to Q dynamically)
         */
        template <typename Q>
        inline std::shared_ptr<Collection<Q>> asCollectionOf()
        {
            return std::make_shared<detail::DelegateCollection<Collection<Q>, Collection<T>>>( shared_from_this() );
        }

        template <typename Q>
        inline std::shared_ptr<const Collection<Q>> asCollectionOf() const
        {
            return std::make_shared<detail::ConstDelegateCollection<Collection<Q>, Collection<T>>>( shared_from_this() );
        }
    };

    template <typename T>
    inline typename Collection<T>::Iterator begin( const std::unique_ptr<Collection<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename Collection<T>::ConstIterator begin( const std::unique_ptr<const Collection<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename Collection<T>::Iterator end( const std::unique_ptr<Collection<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename Collection<T>::ConstIterator end( const std::unique_ptr<const Collection<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename Collection<T>::Iterator begin( const std::shared_ptr<Collection<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename Collection<T>::ConstIterator begin( const std::shared_ptr<const Collection<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename Collection<T>::Iterator end( const std::shared_ptr<Collection<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename Collection<T>::ConstIterator end( const std::shared_ptr<const Collection<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline bool operator==( const Collection<T>& lhs, const Collection<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    template <typename T>
    inline bool operator!=( const Collection<T>& lhs, const Collection<T>& rhs )
    {
        return !( lhs == rhs );
    }

} // namespace ecore


#endif /* ECORE_COLLECTION_HPP_ */
