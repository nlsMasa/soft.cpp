// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECTINTERNAL_HPP
#define ECORE_EOBJECTINTERNAL_HPP

#include "ecore/Exports.hpp"
#include "ecore/Uri.hpp"

namespace ecore
{
    class EObject;
    class EResource;
    class ENotificationChain;
    class EStructuralFeature;
} // namespace ecore

namespace ecore::impl
{
    class ECORE_API EObjectInternal
    {
    public:
        virtual ~EObjectInternal() = default;

        // Resource
        virtual std::shared_ptr<EResource> eInternalResource() const = 0;

        virtual std::shared_ptr<ENotificationChain> eSetInternalResource( const std::shared_ptr<EResource>& resource,
                                                                          const std::shared_ptr<ENotificationChain>& notifications )
            = 0;

        // Container
        virtual std::shared_ptr<EObject> eInternalContainer() const = 0;

        // URI Fragment
        virtual std::shared_ptr<EObject> eObjectForFragmentSegment( const std::string& uriSegment ) const = 0;

        virtual std::string eURIFragmentSegment( const std::shared_ptr<EStructuralFeature>& feature,
                                                 const std::shared_ptr<EObject>& eObject ) const = 0;

        // Inverse
        virtual std::shared_ptr<ENotificationChain> eInverseAdd( const std::shared_ptr<EObject>& otherEnd,
                                                                 int featureID,
                                                                 const std::shared_ptr<ENotificationChain>& notifications )
            = 0;
        virtual std::shared_ptr<ENotificationChain> eInverseRemove( const std::shared_ptr<EObject>& otherEnd,
                                                                    int featureID,
                                                                    const std::shared_ptr<ENotificationChain>& notifications )
            = 0;

        // Proxy
        virtual URI eProxyURI() const = 0;
        virtual void eSetProxyURI( const URI& uri ) = 0;
        virtual std::shared_ptr<EObject> eResolveProxy( const std::shared_ptr<EObject>& proxy ) const = 0;
    };
} // namespace ecore::impl

#endif /* ECORE_EOBJECTINTERNAL_HPP */
