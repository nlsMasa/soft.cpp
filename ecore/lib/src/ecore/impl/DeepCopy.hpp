// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_DEEPCOPY_HPP_
#define ECORE_DEEPCOPY_HPP_

#include "ecore/Exports.hpp"

#include <memory>
#include <unordered_map>

namespace ecore
{
    class EObject;
    class EReference;
    class EAttribute;
    template <typename T>
    class EList;
}

namespace ecore::impl
{

    class ECORE_API DeepCopy
    {
    public:
        DeepCopy( bool resolve = true , bool originalReferences = true );
        ~DeepCopy();

        std::shared_ptr<EObject> copy( const std::shared_ptr<EObject>& eObject );
        std::shared_ptr<EList<std::shared_ptr<EObject>>> copyAll( const std::shared_ptr<EList<std::shared_ptr<EObject>>>& l );
        void copyReferences();

    private:
        std::shared_ptr<EObject> createCopy( const std::shared_ptr<EObject>& eObject ) const;
        void copyAttribute( const std::shared_ptr<EAttribute>& eAttribute,
                            const std::shared_ptr<EObject>& eObject,
                            const std::shared_ptr<EObject>& copyEObject ) const;
        void copyContainment( const std::shared_ptr<EReference>& eReference,
                              const std::shared_ptr<EObject>& eObject,
                              const std::shared_ptr<EObject>& copyEObject );
        void copyProxyURI( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EObject>& copyEObject ) const;
    private:
        bool resolve_;
        bool originalReferences_;
        std::unordered_map<std::shared_ptr<EObject>, std::shared_ptr<EObject>> objects_;
    };

}

#endif