// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECTLISTBASE_HPP
#define ECORE_EOBJECTLISTBASE_HPP

#include "ecore/impl/ArrayEListBase.hpp"
#include "ecore/impl/ENotifyingListBase.hpp"

namespace ecore::impl
{
    template <typename I, typename T, bool unique>
    using EObjectListBase = ArrayEListBase<ENotifyingListBase<I,unique>, T>;

} // namespace ecore::impl

#endif