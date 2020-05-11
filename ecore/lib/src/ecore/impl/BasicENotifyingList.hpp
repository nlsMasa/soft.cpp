// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASIC_ENOTIFYING_LIST_HPP_
#define ECORE_BASIC_ENOTIFYING_LIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/impl/ENotifyingListBase.hpp"
#include "ecore/impl/ArrayEListBase.hpp"

namespace ecore::impl
{
    template <typename T>
    using BasicENotifyingList = ArrayEListBase<ENotifyingListBase<ENotifyingList<T>,true>>;
    
} // namespace ecore::impl

#endif /* ECORE_BASIC_ENOTIFYING_LIST_HPP_ */
