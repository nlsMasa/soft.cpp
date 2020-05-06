// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ARRAYELIST_HPP_
#define ECORE_ARRAYELIST_HPP_

#include "ecore/impl/AbstractEList.hpp"
#include "ecore/impl/Proxy.hpp"

#include <vector>

namespace ecore
{
    namespace impl
    {
        template <typename Base, typename T = typename Base::ValueType >
        class ArrayEList : public Base
        {
        public:
            ArrayEList()
                : Base()
                , v_()
            {
            }

            ArrayEList( const std::vector<T>&& v )
                : Base()
                , v_( v )
            {
            }

            ArrayEList( const std::initializer_list<T>& init )
                : Base()
                , v_( init )
            {
            }

            ArrayEList( const ArrayEList& o )
                : Base( o )
                , v_( o.v_ )
            {
            }

            virtual ~ArrayEList()
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

            virtual bool contains( const T& e ) const
            {
                return std::find( v_.begin(), v_.end(), e ) != v_.end();
            }

            virtual std::size_t indexOf( const T& e ) const
            {
                std::size_t index = std::distance( v_.begin(), std::find( v_.begin(), v_.end(), e ) );
                return index == size() ? -1 : index;
            }

        protected:
            std::vector<T>& getVector()
            {
                return v_;
            }

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

            virtual bool doAddAll( std::size_t index, const Collection<T>& l )
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

        template <typename Base, typename T>
        class ArrayEList<Base, Proxy<T>> : public Base
        {
        public:
            ArrayEList()
                : Base()
                , v_()
            {
            }

            ArrayEList( const ArrayEList& o )
                : Base( o )
                , v_( o.v_ )
            {
            }

            virtual ~ArrayEList()
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

            virtual bool contains( const T& e ) const
            {
                return indexOf( e ) != -1;
            }

            virtual std::size_t indexOf( const T& e ) const
            {
                for( std::size_t i = 0; i < v_.size(); ++i )
                {
                    if( v_[i] == e || resolve( i, v_[i] ) == e )
                        return i;
                }
                return -1;
            }
            
        protected:
            std::vector<Proxy<T>>& getVector()
            {
                return v_;
            }

            virtual T doGet( std::size_t index ) const
            {
                return resolve( index, v_[index] );
            }

            virtual T doSet( std::size_t index, const T& e )
            {
                auto old = v_[index].get();
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

            virtual bool doAddAll( std::size_t index, const Collection<T>& l )
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
                auto element = it->get();
                v_.erase( it );
                didRemove( index, element );
                didChange();
                return element;
            }

            virtual T doMove( std::size_t newIndex, std::size_t oldIndex )
            {
                auto it = std::next( v_.begin(), oldIndex );
                auto element = it->get();
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
