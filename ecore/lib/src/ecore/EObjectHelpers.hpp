// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTHELPERS_HPP_
#define ECORE_EOBJECTHELPERS_HPP_

#include "ecore/Any.hpp"

#include <memory>
#include <type_traits>

namespace ecore
{

    class EObject;

    template <class T>
    struct IsSharedEObject : std::false_type
    {
    };

    template <class T>
    struct IsSharedEObject<std::shared_ptr<T>> : std::is_base_of<EObject, typename T>
    {
    };

    template <class T>
    using IsSharedEObjectOrAny = std::disjunction<std::is_same<T, Any>, IsSharedEObject<T>>;

} // namespace ecore

#endif // ECORE_EOBJECTHELPERS_HPP_
