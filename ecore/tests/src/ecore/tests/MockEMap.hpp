// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_EMAP_HPP_
#define ECORE_MOCK_EMAP_HPP_

#include "ecore/EMap.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{

    //MOCK_BASE_CLASS( MockEMapEntry, MapEntry )
    //{
    //    MOCK_METHOD_TPL( getKey, 0, Any(), getKey );
    //    MOCK_METHOD_TPL( setKey, 1, void( const Any& ), setKey );
    //    MOCK_METHOD_TPL( getValue, 0, Any(), getValue );
    //    MOCK_METHOD_TPL( setValue, 1, void( const Any& ), setValue );
    //};

    //template <class Key, class Value>
    //MOCK_BASE_CLASS( MockEMap, EMap<Key, Value> )
    //{
    //    MOCK_METHOD_TPL( add, 1, bool( const MapEntry& e ) );
    //    MOCK_METHOD_TPL( addAll, 1, bool( const Collection<std::shared_ptr<EMapEntry>>& ) );
    //    MOCK_CONST_METHOD_TPL( get, 1, std::shared_ptr<EMapEntry>( std::size_t ) );
    //    MOCK_CONST_METHOD_TPL( at, 1, Value( const Key& ) );
    //    MOCK_METHOD_TPL( insert, 2, void( const Key&, const Value& ) );
    //    MOCK_METHOD_TPL( remove, 1, bool( const MapEntry& ), removeEntry );
    //    MOCK_METHOD_TPL( remove, 1, bool( const Key& ), remove );
    //    MOCK_METHOD_TPL( clear, 0, void() );
    //    MOCK_CONST_METHOD_TPL( empty, 0, bool() );
    //    MOCK_CONST_METHOD_TPL( size, 0, std::size_t() );
    //    MOCK_CONST_METHOD_TPL( contains, 1, bool( const MapEntry& ), containsEntry );
    //    MOCK_CONST_METHOD_TPL( contains, 1, bool( const Key& ), contains );
    //    MOCK_CONST_METHOD_TPL( keyOf, 1, std::optional<Key>( const Value& ) );
    //    MOCK_CONST_METHOD_TPL( indexOf, 1, std::size_t( const MapEntry& ) );
    //};

} // namespace ecore::tests

#endif // ECORE_MOCK_EMAP_HPP_
