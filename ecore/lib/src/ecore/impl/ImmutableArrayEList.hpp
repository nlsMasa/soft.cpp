// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMMUTABLE_ARRAY_ELIST_HPP_
#define ECORE_IMMUTABLE_ARRAY_ELIST_HPP_

#include "ecore/EList.hpp"
#include "ecore/impl/ImmutableEListBase.hpp"

#include <vector>

namespace ecore::impl
{

    template <typename T>
    class ImmutableArrayEList : public ImmutableEListBase<EList<T>>
    {
    public:
        ImmutableArrayEList() = default;

        ImmutableArrayEList( const std::vector<T>& v )
            : v_( v )
        {
        }

        ImmutableArrayEList( const std::vector<T>&& v )
            : v_( v )
        {
        }

        ImmutableArrayEList( std::initializer_list<T> l )
            : v_( l )
        {
        }

        virtual ~ImmutableArrayEList() = default;

        T get( std::size_t pos ) const override
        {
            return v_.at( pos );
        }

        std::size_t size() const override
        {
            return v_.size();
        }

        bool empty() const override
        {
            return v_.empty();
        }

        bool contains( const T& e ) const override
        {
            return std::find( v_.begin(), v_.end(), e ) != v_.end();
        }

        std::size_t indexOf( const T& e ) const override
        {
            std::size_t index = std::distance( v_.begin(), std::find( v_.begin(), v_.end(), e ) );
            return index == size() ? -1 : index;
        }

        std::size_t indexOf( std::function<bool( const T& )> predicate ) const override
        {
            std::size_t index = std::distance( v_.begin(), std::find_if( v_.begin(), v_.end(), predicate ) );
            return index == size() ? -1 : index;
        }

    protected:
        std::vector<T> v_;
    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_ELIST_HPP_ */
