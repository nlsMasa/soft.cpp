// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_ADAPTER_HPP_
#define ECORE_MOCK_ADAPTER_HPP_

#include "ecore/EAdapter.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    MOCK_BASE_CLASS( MockEAdapter , EAdapter )
    {
        MOCK_METHOD( notifyChanged, 1 );
        MOCK_METHOD( getTarget , 0 );
        MOCK_METHOD( setTarget, 1 );
        MOCK_METHOD( unsetTarget, 1 );
    };
}

#endif
