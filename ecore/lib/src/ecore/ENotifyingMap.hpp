// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ENOTIFYINGMAP_HPP_
#define ECORE_ENOTIFYINGMAP_HPP_

#include "ecore/EMap.hpp"

#include <memory>

namespace ecore
{

    class ENotifier;
    class ENotificationChain;
    class EStructuralFeature;

    template <class Key, class Value>
    class ENotifyingMap : public EMap<Key, Value>
    {
    public:
        virtual ~ENotifyingMap() = default;

        using EMap::add;
        using EMap::remove;

        virtual std::shared_ptr<ENotifier> getNotifier() const = 0;

        virtual std::shared_ptr<EStructuralFeature> getFeature() const = 0;

        virtual int getFeatureID() const = 0;

        virtual std::shared_ptr<ENotificationChain> add( const Key&, const Value&, const std::shared_ptr<ENotificationChain>& ) = 0;

        virtual std::shared_ptr<ENotificationChain> remove( const Key&, const std::shared_ptr<ENotificationChain>& ) = 0;

        virtual std::shared_ptr<ENotificationChain> set( const Key&, const Value&, const std::shared_ptr<ENotificationChain>& ) = 0;
    };

} // namespace ecore

#endif /* ECORE_ENOTIFYINGMAP_HPP_ */
