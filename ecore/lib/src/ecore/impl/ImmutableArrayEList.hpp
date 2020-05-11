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

    private:
        std::vector<T> v_;
    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_ELIST_HPP_ */
