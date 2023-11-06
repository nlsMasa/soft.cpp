// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EUNSETTABLEMAP_HPP_
#define ECORE_EUNSETTABLEMAP_HPP_

#include "ecore/ENotifyingMap.hpp"

namespace ecore
{

    template <class Key, class Value>
    class EUnsettableMap : public ENotifyingMap<Key, Value>
    {
    public:
        virtual ~EUnsettableMap() = default;

        virtual bool isSet() const = 0;

        virtual void unset() = 0;
    };

} // namespace ecore

#endif /* ECORE_EUNSETTABLEMAP_HPP_ */
