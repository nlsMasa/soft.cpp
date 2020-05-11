// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASIC_ELIST_HPP_
#define ECORE_BASIC_ELIST_HPP_

#include "ecore/impl/EListBase.hpp"
#include "ecore/impl/ArrayEListBase.hpp"

namespace ecore::impl
{

    template <typename T, bool unique = false>
    class BasicEList : public ArrayEListBase<EListBase<EList<T>, unique>>
    {
        typedef typename ArrayEListBase<EListBase<EList<T>, unique>> Super;
    public:
        BasicEList()
            : Super()
        {
        }

        BasicEList( const std::initializer_list<T>& init )
            : Super( init )
        {
        }

        BasicEList( const BasicEList& o )
            : Super( o )
        {
        }

        virtual ~BasicEList()
        {
        }
    };

} // namespace ecore::impl

#endif /* ECORE_BASIC_ELIST_HPP_ */
