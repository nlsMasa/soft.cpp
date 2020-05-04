// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTLIST_HPP_
#define ECORE_EOBJECTLIST_HPP_

#include "ecore/ENotifyingList.hpp"

namespace ecore
{
    template <typename T>
    class EObjectList : public ENotifyingList<T>
    {
    public:
        virtual ~EObjectList() = default;

        /**
         * Returns the the list of unresolved objects.
         * @return the the list of unresolved objects.
         */
        virtual std::shared_ptr<EList<T>> getUnResolvedList() = 0;
    };
} // namespace ecore

#endif /* ECORE_EOBJECTLIST_HPP_ */
