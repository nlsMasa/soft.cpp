// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASIC_ENOTIFYING_LIST_HPP_
#define ECORE_BASIC_ENOTIFYING_LIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/AbstractEListBase.hpp"

namespace ecore::impl
{
    template <typename T>
    using BasicENotifyingList = AbstractEListBase<AbstractENotifyingList<ENotifyingList<T>>>;
    
} // namespace ecore::impl

#endif /* ECORE_BASIC_ENOTIFYING_LIST_HPP_ */
