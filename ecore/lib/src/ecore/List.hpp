// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_LIST_HPP_
#define ECORE_LIST_HPP_

#include "ecore/Collection.hpp"

#include <functional>

namespace ecore
{
    template <typename T>
    class List : public Collection<T>
    {
    public:
        typedef typename T ValueType;

        virtual ~List() = default;

        using Collection::add;

        virtual bool add( std::size_t pos, const T& e ) = 0;

        using Collection::addAll;

        virtual bool addAll( std::size_t pos, const Collection<T>& l ) = 0;

        virtual void move( std::size_t newPos, const T& e ) = 0;

        virtual T move( std::size_t newPos, std::size_t oldPos ) = 0;

        virtual T get( std::size_t pos ) const = 0;

        virtual T set( std::size_t pos, const T& e ) = 0;

        using Collection::remove;

        virtual T remove( std::size_t pos ) = 0;

        using Collection::contains;

        virtual std::size_t indexOf( const T& e ) const = 0;

        virtual std::size_t indexOf( std::function<bool( const T& )> ) const = 0;
    };

} // namespace ecore

#endif /* ECORE_LIST_HPP_ */
