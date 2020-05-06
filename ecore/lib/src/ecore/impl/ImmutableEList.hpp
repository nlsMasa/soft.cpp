// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMMUTABLE_ELIST_HPP_
#define ECORE_IMMUTABLE_ELIST_HPP_

#include "ecore/EList.hpp"

#include <vector>

namespace ecore::impl
{

    template <typename T>
    class ImmutableEList : public EList<T>
    {
    public:
        ImmutableEList() = default;

        ImmutableEList( const std::vector<T>&& v )
            : v_( v )
        {
        }

        ImmutableEList( std::initializer_list<T> l )
            : v_( l )
        {
        }

        virtual ~ImmutableEList()
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

        virtual bool add( std::size_t pos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( std::size_t pos, const Collection<T>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual void move( std::size_t oldPos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual T move( std::size_t oldPos, std::size_t newPos )
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

        virtual bool remove( const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual void clear()
        {
            throw "UnsupportedOperationException";
        }

        virtual T get( std::size_t pos ) const
        {
            return v_.at( pos );
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

        virtual std::shared_ptr<EList<ValueType>> getUnResolvedList()
        {
            return std::static_pointer_cast<EList<ValueType>>( shared_from_this() );
        }

        virtual std::shared_ptr<const EList<ValueType>> getUnResolvedList() const
        {
            return std::static_pointer_cast<const EList<ValueType>>( shared_from_this() );
        }


    private:
        std::vector<T> v_;
    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_ELIST_HPP_ */
