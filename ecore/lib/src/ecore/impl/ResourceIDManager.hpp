// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_RESOURCEIDMANAGER_HPP_
#define ECORE_RESOURCEIDMANAGER_HPP_

#include "ecore/EResourceIDManager.hpp"
#include "ecore/Exports.hpp"

#include <unordered_map>

namespace ecore::impl
{
    class ResourceIDManager : public EResourceIDManager
    {
    public:
        ResourceIDManager();

        virtual ~ResourceIDManager();

        virtual void registerObject( const std::shared_ptr<EObject>& eObject );

        virtual void unregisterObject( const std::shared_ptr<EObject>& eObject );

        virtual std::string getID( const std::shared_ptr<EObject>& eObject ) const;

        virtual std::shared_ptr<EObject> getEObject( const std::string& ) const;

    private:
        std::unordered_map<std::shared_ptr<EObject>, std::string> objectToID_;
        std::unordered_map<std::string, std::shared_ptr<EObject>> idToObject_;
    };

}

#endif