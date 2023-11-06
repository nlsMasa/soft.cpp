// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICENOTIFYINGMAP_HPP_
#define ECORE_BASICENOTIFYINGMAP_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/Map.hpp"
#include "ecore/impl/ArrayEListBase.hpp"
#include "ecore/impl/BasicENotifyingList.hpp"
#include "ecore/impl/ENotifyingListBase.hpp"

namespace ecore::impl
{

    template <class Key, class Value>
    class BasicENotifyingMap : public ArrayEListBase<ENotifyingListBase<ENotifyingList<MapEntry<Key, Value>>, true>>
    {
    public:
        using Super = ArrayEListBase<ENotifyingListBase<ENotifyingList<MapEntry<Key, Value>>, true>>;
        using ValueType = typename Super::ValueType;

        BasicENotifyingMap() = default;
        BasicENotifyingMap( const BasicENotifyingMap& ) = default;
        virtual ~BasicENotifyingMap() = default;

        BasicENotifyingMap( const std::vector<MapEntry<Key, Value>>& vector )
            : Super( vector )
        {
        }

        BasicENotifyingMap( const std::initializer_list<MapEntry<Key, Value>>& init )
            : Super( init )
        {
        }
    };

} // namespace ecore::impl

#endif /* ECORE_BASICENOTIFYINGMAP_HPP_ */
