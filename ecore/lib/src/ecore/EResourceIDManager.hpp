// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCEIDMANAGER_HPP_
#define ECORE_ERESOURCEIDMANAGER_HPP_

#include <memory>
#include <string>

namespace ecore
{
    class EObject;

    class EResourceIDManager
    {
    public:
        virtual ~EResourceIDManager() = default;

        virtual void clear() = 0;

        virtual void registerObject( const std::shared_ptr<EObject>& eObject ) = 0;

        virtual void unregisterObject( const std::shared_ptr<EObject>& eObject ) = 0;

        virtual std::string getID( const std::shared_ptr<EObject>& eObject ) const = 0;

        virtual std::shared_ptr<EObject> getEObject( const std::string& ) const = 0;
    };

}

#endif
