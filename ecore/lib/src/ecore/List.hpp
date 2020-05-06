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

namespace ecore
{
    template <typename T>
    class List : public Collection<T>
    {
    public:
        typedef typename T ValueType;

        virtual ~List() = default;

        virtual bool add( std::size_t pos, const T& e ) = 0;

        virtual bool addAll( std::size_t pos, const EList<T>& l ) = 0;

        virtual void move( std::size_t newPos, const T& e ) = 0;

        virtual T move( std::size_t newPos, std::size_t oldPos ) = 0;

        virtual T get( std::size_t pos ) const = 0;

        virtual T set( std::size_t pos, const T& e ) = 0;

        virtual T remove( std::size_t pos ) = 0;

        virtual std::size_t indexOf( const T& e ) const = 0;
    };

} // namespace ecore

#endif /* ECORE_LIST_HPP_ */
