// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_DEEPEQUAL_HPP_
#define ECORE_DEEPEQUAL_HPP_

#include "ecore/Exports.hpp"

#include <memory>
#include <unordered_map>

namespace ecore
{
    class EObject;
    class EAttribute;
    class EReference;
    template <typename T>
    class EList;
} // namespace ecore

namespace ecore::impl
{

    class ECORE_API DeepEqual
    {
    public:
        bool equals( const std::shared_ptr<EObject>& lhs, const std::shared_ptr<EObject>& rhs );
        bool equals( const std::shared_ptr<EList<std::shared_ptr<EObject>>>& lhs,
                     const std::shared_ptr<EList<std::shared_ptr<EObject>>>& rhs );

    private:
        bool equals( const std::shared_ptr<EObject>& lhs,
                     const std::shared_ptr<EObject>& rhs,
                     const std::shared_ptr<EAttribute>& eAttribute );

        bool equals( const std::shared_ptr<EObject>& lhs,
                     const std::shared_ptr<EObject>& rhs,
                     const std::shared_ptr<EReference>& eReference );

    private:
        std::unordered_map<std::shared_ptr<EObject>, std::shared_ptr<EObject>> objects_;
    };
} // namespace ecore::impl

#endif