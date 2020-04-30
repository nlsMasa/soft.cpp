// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECT_MockEObjectINTERNAL_HPP
#define ECORE_EOBJECT_MockEObjectINTERNAL_HPP

#include "ecore/impl/EObjectInternal.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{

    MOCK_BASE_CLASS( MockEObjectInternal, impl::EObjectInternal )
    {
        MOCK_METHOD( eInternalResource, 0 );
        MOCK_METHOD( eSetInternalResource, 2 );

        MOCK_METHOD( eInternalContainer, 0 );

        MOCK_METHOD( eObjectForFragmentSegment, 1 );
        MOCK_METHOD( eURIFragmentSegment, 2 );

        MOCK_METHOD( eInverseAdd, 3 );
        MOCK_METHOD( eInverseRemove, 3 );

        MOCK_METHOD( eProxyURI, 0 );
        MOCK_METHOD( eSetProxyURI, 1 );
        MOCK_METHOD( eResolveProxy, 1 );
    };
} // namespace ecore::tests

#endif /* ECORE_EOBJECT_MockEObjectINTERNAL_HPP */
