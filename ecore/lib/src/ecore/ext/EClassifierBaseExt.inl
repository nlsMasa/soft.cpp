// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSIFIERBASEEXT_HPP
#error This file may only be included from EClassifierBaseExt.hpp
#endif


#include "ecore/EList.hpp"
#include "ecore/EPackage.hpp"

namespace ecore::ext
{
    template <typename... I>
    EClassifierBaseExt<I...>::EClassifierBaseExt()
    {
    }

    template <typename... I>
    EClassifierBaseExt<I...>::~EClassifierBaseExt()
    {
    }

    template <typename... I>
    bool EClassifierBaseExt<I...>::isInstance(const Any& object) const
    {
        return getInstanceClass() && getInstanceClass()->operator==(object.type());
    }

    template <typename... I>
    int EClassifierBaseExt<I...>::initClassifierID()
    {
        return getEPackage() ? static_cast<int>(getEPackage()->getEClassifiers()->indexOf(getThisPtr())) : -1;
    }

    template <typename... I>
    const std::string& EClassifierBaseExt<I...>::getInstanceTypeName() const
    {
        return instanceTypeName_;
    }

    template <typename... I>
    void EClassifierBaseExt<I...>::setInstanceTypeName( const std::string& newInstanceTypeName )
    {
        auto oldInstanceTypeName = instanceTypeName_;
        instanceTypeName_ = newInstanceTypeName;
        if( eNotificationRequired() )
            eNotify( std::make_shared<ecore::impl::Notification>(getThisAsEObject(), ecore::ENotification::SET, EcorePackage::ECLASSIFIER__INSTANCE_TYPE_NAME, oldInstanceTypeName, newInstanceTypeName));
    }
}
