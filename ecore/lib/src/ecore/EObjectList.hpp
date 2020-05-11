// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTLIST_HPP_
#define ECORE_EOBJECTLIST_HPP_

#include "ecore/Any.hpp"
#include "ecore/List.hpp"
#include <memory>
#include <type_traits>

namespace ecore
{
    template <typename L , typename T>
    class EObjectList : public List<T>
    {
    public:
        virtual ~EObjectList() = default;

        virtual std::shared_ptr<L> getUnResolvedList() = 0;

        virtual std::shared_ptr<const L> getUnResolvedList() const = 0;
    };


} // namespace ecore

#endif

