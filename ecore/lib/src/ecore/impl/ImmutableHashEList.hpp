// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMMUTABLE_HASH_ELIST_HPP_
#define ECORE_IMMUTABLE_HASH_ELIST_HPP_

#include "ecore/impl/ImmutableArrayEList.hpp"

#include <unordered_set>

namespace ecore::impl
{

    template <typename T>
    class ImmutableHashEList : public ImmutableArrayEList<T>
    {
    public:
        ImmutableHashEList() = default;

        ImmutableHashEList( const std::vector<T>&& v )
            : ImmutableArrayEList<T>( std::move(v) )
            , s_( std::begin( v ), std::end(v) )
        {
        }

        ImmutableHashEList( std::initializer_list<T> l )
            : ImmutableArrayEList<T>( l )
            , s_( l )
        {
        }

        virtual ~ImmutableHashEList() = default;

        virtual bool contains( const T& e ) const
        {
            return s_.find( e ) != s_.end();
        }

    private:
        std::unordered_set<T> s_;
    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_HASH_ELIST_HPP_ */
