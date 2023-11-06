// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICEOBJECTMAP_HPP_
#define ECORE_BASICEOBJECTMAP_HPP_

#include "ecore/EMap.hpp"
#include "ecore/impl/BasicEObjectList.hpp"

#include <type_traits>

namespace ecore::impl
{

    template <bool containement = false, bool inverse = false, bool opposite = false, bool proxies = false>
    using BasicEObjectMap = BasicEObjectList<EMapEntry, containement, inverse, opposite, proxies, false>;

} // namespace ecore::impl

#endif // ECORE_BASICEOBJECTMAP_HPP_
