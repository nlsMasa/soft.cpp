// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTARRAYELIST_HPP_
#define ECORE_ABSTRACTARRAYELIST_HPP_

#include "ecore/impl/AbstractEList.hpp"
#include "ecore/impl/Proxy.hpp"

#include <vector>

namespace ecore
{
    namespace impl
    {
        template <typename L, typename T>
        class AbstractArrayEList : public L
        {
        public:
            AbstractArrayEList()
                : L()
                , v_()
            {
            }

            AbstractArrayEList( const std::vector<T>&& v )
                : L()
                , v_( v )
            {
            }

            AbstractArrayEList( const std::initializer_list<T>& init )
                : L()
                , v_( init )
            {
            }

            AbstractArrayEList( const AbstractArrayEList& o )
                : L( o )
                , v_( o.v_ )
            {
            }

            virtual ~AbstractArrayEList()
            {
            }

            virtual std::size_t size() const
            {
                return v_.size();
            }

            virtual bool empty() const
            {
                return v_.empty();
            }

        protected:
            virtual T doGet( std::size_t index ) const
            {
                return v_[index];
            }

            virtual T doSet( std::size_t index, const T& e )
            {
                auto old = v_[index];
                v_[index] = e;
                didSet( index, e, old );
                didChange();
                return old;
            }

            virtual void doAdd( const T& e )
            {
                auto index = size();
                v_.push_back( e );
                didAdd( index, e );
                didChange();
            }

            virtual void doAdd( std::size_t index, const T& e )
            {
                v_.insert( std::next( v_.begin(), index ), e );
                didAdd( index, e );
                didChange();
            }

            virtual bool doAddAll( std::size_t index, const EList<T>& l )
            {
                std::size_t growth = l.size();
                std::size_t oldSize = v_.size();
                v_.resize( oldSize + growth );
                for( int i = (int)oldSize - 1; i >= (int)index; --i )
                    v_[i + growth] = v_[i];
                for( int i = 0; i < growth; ++i )
                {
                    auto v = l.get( i );
                    auto n = i + index;
                    v_[n] = v;
                    didAdd( n, v );
                    didChange();
                }
                return growth != 0;
            }

            virtual T doRemove( std::size_t index )
            {
                auto it = std::next( v_.begin(), index );
                auto element = std::move( *it );
                v_.erase( it );
                didRemove( index, element );
                didChange();
                return element;
            }

            virtual T doMove( std::size_t newIndex, std::size_t oldIndex )
            {
                auto it = std::next( v_.begin(), oldIndex );
                auto element = std::move( *it );
                v_.erase( it );
                v_.insert( std::next( v_.begin(), newIndex ), element );
                didMove( newIndex, element, oldIndex );
                didChange();
                return element;
            }

            virtual std::shared_ptr<EList<T>> doClear()
            {
                auto l = std::make_shared<ImmutableEList<T>>( std::move( v_ ) );
                v_.clear();
                return l;
            }

        private:
            std::vector<T> v_;
        };

        template <typename L, typename T>
        class AbstractArrayEList<L, Proxy<T>> : public L
        {
        public:
            AbstractArrayEList()
                : L()
                , v_()
            {
            }

            AbstractArrayEList( const AbstractArrayEList& o )
                : L( o )
                , v_( o.v_ )
            {
            }

            virtual ~AbstractArrayEList()
            {
            }

            virtual std::size_t size() const
            {
                return v_.size();
            }

            virtual bool empty() const
            {
                return v_.empty();
            }

        protected:
            virtual T doGet( std::size_t index ) const
            {
                return resolve( index, v_[index] );
            }

            virtual T doSet( std::size_t index, const T& e )
            {
                auto old = v_[index].get();
                v_[index].set( e );
                didSet( index, e, old );
                didChange();
                return old;
            }

            virtual void doAdd( const T& e )
            {
                auto index = size();
                v_.push_back( e );
                didAdd( index, e );
                didChange();
            }

            virtual void doAdd( std::size_t index, const T& e )
            {
                v_.insert( std::next( v_.begin(), index ), e );
                didAdd( index, e );
                didChange();
            }

            virtual bool doAddAll( std::size_t index, const EList<T>& l )
            {
                std::size_t growth = l.size();
                std::size_t oldSize = v_.size();
                v_.resize( oldSize + growth );
                for( int i = (int)oldSize - 1; i >= (int)index; --i )
                    v_[i + growth] = v_[i];
                for( int i = 0; i < growth; ++i )
                {
                    auto e = l.get( i );
                    auto n = i + index;
                    v_[n] = e;
                    didAdd( n, e );
                    didChange();
                }
                return growth != 0;
            }

            virtual T doRemove( std::size_t index )
            {
                auto it = std::next( v_.begin(), index );
                auto element = std::move( (*it).get() );
                v_.erase( it );
                didRemove( index, element );
                didChange();
                return element;
            }

            virtual T doMove( std::size_t newIndex, std::size_t oldIndex )
            {
                auto it = std::next( v_.begin(), oldIndex );
                auto element = std::move( (*it).get() );
                v_.erase( it );
                v_.insert( std::next( v_.begin(), newIndex ), element );
                didMove( newIndex, element, oldIndex );
                didChange();
                return element;
            }

            virtual std::shared_ptr<EList<T>> doClear()
            {
                std::vector<T> result;
                std::transform( v_.begin(), v_.end(), result.end(), []( const Proxy<T>& p ) { return p.get(); } );
                auto l = std::make_shared<ImmutableEList<T>>( std::move( result ) );
                v_.clear();
                return l;
            }

        protected:
            virtual T resolve( std::size_t index, const Proxy<T>& e ) const = 0;

        private:
            std::vector<Proxy<T>> v_;
        };

    } // namespace impl

} // namespace ecore

#endif /* ECORE_ABSTRACTARRAYELIST_HPP_ */
