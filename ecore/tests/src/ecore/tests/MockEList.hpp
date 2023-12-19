// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_LIST_HPP_
#define ECORE_MOCK_LIST_HPP_

#include "ecore/EList.hpp"
#include "turtle/mock.hpp"
#include <functional>

namespace ecore::tests
{
    template <typename T>
    MOCK_BASE_CLASS( MockEList, EList<T> )
    {
        MOCK_METHOD_TPL( add, 1, bool( const T& ), add );
        MOCK_METHOD_TPL( add, 2, bool( std::size_t, const T& ), addIndex );
        MOCK_METHOD_TPL( addAll, 1, bool( const Collection<T>& ), addAll );
        MOCK_METHOD_TPL( addAll, 2, bool( std::size_t, const Collection<T>& ), addAllIndex );
        MOCK_METHOD_TPL( move, 2, void( std::size_t, const T& ), move );
        MOCK_METHOD_TPL( move, 2, T( std::size_t, std::size_t ), moveIndex );
        MOCK_CONST_METHOD_TPL( get, 1, T( std::size_t ) );
        MOCK_METHOD_TPL( set, 2, T( std::size_t, const T& ) );
        MOCK_METHOD_TPL( remove, 1, T( std::size_t ), removeIndex );
        MOCK_METHOD_TPL( remove, 1, bool( const T& ), removeObject );
        MOCK_CONST_METHOD_TPL( contains, 1, bool( const T& ) );
        MOCK_CONST_METHOD_TPL( indexOf, 1, std::size_t( const T& ) );
        MOCK_CONST_METHOD_TPL( indexOf, 1, std::size_t( std::function<bool( const T& )> ), indexOfPredicate );
        MOCK_CONST_METHOD_TPL( size, 0, std::size_t() );
        MOCK_METHOD_TPL( clear, 0, void() );
        MOCK_CONST_METHOD_TPL( empty, 0, bool() );
        MOCK_CONST_METHOD_TPL( getUnResolvedList, 0, std::shared_ptr<const EList<T>>(), getUnResolvedListConst );
        MOCK_NON_CONST_METHOD_TPL( getUnResolvedList, 0, std::shared_ptr<EList<T>>(), getUnResolvedList );
    };
} // namespace ecore::tests

#endif
