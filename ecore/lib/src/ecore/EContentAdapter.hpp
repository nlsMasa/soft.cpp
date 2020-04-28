// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECONTENTADAPTER_HPP_
#define ECORE_ECONTENTADAPTER_HPP_

#include "ecore/Exports.hpp"
#include "ecore/impl/AbstractAdapter.hpp"

#include <memory>

namespace ecore
{
    class EObject;

    /**
     * An adapter that maintains itself as an adapter for all contained objects
     * as they come and go.
     * It can be installed for an {@link EObject}
     */
    class ECORE_API EContentAdapter : public impl::AbstractAdapter
    {
    public:
        EContentAdapter() = default;
        
        virtual ~EContentAdapter() = default;

        virtual void notifyChanged( const std::shared_ptr<ENotification>& notification );

        virtual void setTarget( const std::shared_ptr<ENotifier>& target );
        
        virtual void unsetTarget( const std::shared_ptr<ENotifier>& target );

    private:
        void selfAdapt( const std::shared_ptr<ENotification>& notification );

        void handleContainment( const std::shared_ptr<ENotification>& notification );

        void addAdapter( const std::shared_ptr<EObject>& eObject );

        void removeAdapter( const std::shared_ptr<EObject>& eObject , bool checkContainer = false );
    };

} // namespace ecore

#endif
