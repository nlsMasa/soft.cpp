// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ARRAYELISTBASE_HPP_
#define ECORE_ARRAYELISTBASE_HPP_

#include "ecore/impl/EListBase.hpp"
#include "ecore/impl/Proxy.hpp"

#include <vector>

namespace ecore::impl
{
    template <typename Base, typename T>
    class AbstractArrayEListBase : public Base
    {
    public:
        typedef typename Base::ValueType ValueType;

        virtual ~AbstractArrayEListBase() = default;

    protected:
        AbstractArrayEListBase()
            : Base()
            , v_()
        {
        }

        AbstractArrayEListBase( const std::vector<T>&& v )
            : Base()
            , v_( v )
        {
        }

        AbstractArrayEListBase( const std::initializer_list<T>& init )
            : Base()
            , v_( init )
        {
        }

        AbstractArrayEListBase( const AbstractArrayEListBase& o )
            : Base( o )
            , v_( o.v_ )
        {
        }

    public:
        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual bool empty() const
        {
            return v_.empty();
        }

protected:
        virtual ValueType doGet( std::size_t index ) const
        {
            return static_cast<ValueType>( v_[index] );
        }

        virtual ValueType doSet( std::size_t index, const ValueType& e )
        {
            auto old = static_cast<ValueType>(v_[index]);
            v_[index] = e;
            didSet( index, e, old );
            didChange();
            return old;
        }

        virtual void doAdd( const ValueType& e )
        {
            auto index = size();
            v_.push_back( e );
            didAdd( index, e );
            didChange();
        }

        virtual void doAdd( std::size_t index, const ValueType& e )
        {
            v_.insert( std::next( v_.begin(), index ), e );
            didAdd( index, e );
            didChange();
        }

        virtual bool doAddAll( std::size_t index, const Collection<ValueType>& l )
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

        virtual ValueType doRemove( std::size_t index )
        {
            auto it = std::next( v_.begin(), index );
            auto element = static_cast<ValueType>(*it);
            v_.erase( it );
            didRemove( index, element );
            didChange();
            return element;
        }

        virtual ValueType doMove( std::size_t newIndex, std::size_t oldIndex )
        {
            auto it = std::next( v_.begin(), oldIndex );
            auto element = static_cast<ValueType>(*it);
            v_.erase( it );
            v_.insert( std::next( v_.begin(), newIndex ), element );
            didMove( newIndex, element, oldIndex );
            didChange();
            return element;
        }

    protected:
        std::vector<T> v_;
    };

    template <typename Base, typename T = typename Base::ValueType>
    class ArrayEListBase : public AbstractArrayEListBase<Base,T>
    {
    public:
        typedef typename Base Base;
        typedef typename Base::ValueType ValueType;

        ArrayEListBase()
            : AbstractArrayEListBase()
        {
        }

        ArrayEListBase( const std::vector<T>&& v )
            : AbstractArrayEListBase(v)
        {
        }

        ArrayEListBase( const std::initializer_list<T>& init )
            : AbstractArrayEListBase(init)
        {
        }

        ArrayEListBase( const ArrayEListBase& o )
            : AbstractArrayEListBase( o )
        {
        }

        virtual ~ArrayEListBase()
        {
        }

        virtual bool contains( const ValueType& e ) const
        {
            return std::find( v_.begin(), v_.end(), e ) != v_.end();
        }

        virtual std::size_t indexOf( const ValueType& e ) const
        {
            std::size_t index = std::distance( v_.begin(), std::find( v_.begin(), v_.end(), e ) );
            return index == size() ? -1 : index;
        }

    protected:    

        inline ValueType unresolvedGet( std::size_t index ) const
        {
            return v_[index];
        }

        inline std::size_t unresolvedIndexOf( const ValueType& e ) const
        {
            std::size_t index = std::distance( v_.begin(), std::find( v_.begin(), v_.end(), e ) );
            return index == size() ? -1 : index;
        }

        virtual std::shared_ptr<EList<T>> doClear()
        {
            auto l = std::make_shared<ImmutableArrayEList<T>>( std::move( v_ ) );
            v_.clear();
            return l;
        }
    };

    template <typename Base, typename T>
    class ArrayEListBase<Base, Proxy<T>> : public AbstractArrayEListBase<Base, Proxy<T> >
    {
    public:
        typedef typename Base Base;
        typedef typename Base::ValueType ValueType;

        ArrayEListBase()
            : AbstractArrayEListBase()
        {
        }

        ArrayEListBase( const ArrayEListBase& o )
            : AbstractArrayEListBase( o )
        {
        }

        virtual ~ArrayEListBase()
        {
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
        
        virtual T doGet( std::size_t index ) const
        {
            return resolve( index, v_[index] );
        }

        inline ValueType unresolvedGet( std::size_t index ) const
        {
            return static_cast<ValueType>(v_[index]);
        }

        inline std::size_t unresolvedIndexOf( const ValueType& e ) const
        {
            for( std::size_t i = 0; i < v_.size(); ++i )
            {
                if( v_[i] == e )
                    return i;
            }
            return -1;
        }

        virtual std::shared_ptr<EList<T>> doClear()
        {
            std::vector<T> result;
            std::transform( v_.begin(), v_.end(), std::back_inserter(result), []( const Proxy<T>& p ) { return p.get(); } );
            auto l = std::make_shared<ImmutableArrayEList<T>>( std::move( result ) );
            v_.clear();
            return l;
        }

    protected:
        virtual T resolve( std::size_t index, const Proxy<T>& e ) const = 0;

    };

} // namespace ecore::impl

#endif /* ECORE_ARRAYELISTBASE_HPP_ */
