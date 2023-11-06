// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECTMAP_HPP_
#define ECORE_EOBJECTMAP_HPP_

#include "ecore/EMap.hpp"
#include <memory>
#include <type_traits>

namespace ecore
{

    template <class Key, class Value>
    class EObjectMap : public EMap<Key, Value>
    {
    public:
        virtual ~EObjectMap() = default;
    };

} // namespace ecore

#endif // ECORE_EOBJECTMAP_HPP_
