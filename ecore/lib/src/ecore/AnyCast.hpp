// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ANYCAST_HPP_
#define ECORE_ANYCAST_HPP_

#include "ecore/Any.hpp"
#include "ecore/EList.hpp"
#include "ecore/EMap.hpp"
#include "ecore/EObject.hpp"
#include "ecore/SmartPtr.hpp"
#include "ecore/impl/BasicEList.hpp"
#include "ecore/impl/BasicEMap.hpp"

namespace ecore
{
    template <typename T>
    Any toAny( const T& t )
    {
        // Store an EObject
        if constexpr( IsSharedEObject<T>::value )
            return Any( std::static_pointer_cast<EObject>( t ) );
        else
            return Any( t );
    }

    template <typename T>
    Any toAny( const Collection<T>& c )
    {
        if constexpr( IsSharedEObject<T>::value )
        {
            std::shared_ptr<EList<std::shared_ptr<EObject>>> result = std::make_shared<impl::BasicEList<std::shared_ptr<EObject>>>();
            for( const auto& t : c )
                result->add( std::static_pointer_cast<EObject>( t ) );
            return result;
        }
        else
        {
            std::shared_ptr<EList<Any>> result = std::make_shared<impl::BasicEList<Any>>();
            for( const auto& t : c )
                result->add( Any( t ) );
            return result;
        }
    }

    template <typename T>
    Any toAny( const std::shared_ptr<EList<T>>& l )
    {
        if constexpr( IsSharedEObject<T>::value )
        {
            // Store a list of EObject
            if constexpr( std::is_same<EObject, typename T::element_type>::value )
                return l;
            else
                return l->asEListOf<std::shared_ptr<EObject>>();
        }
        else
            // Store a list of Any
            return l->asEListOf<Any>();
    }

    template <typename T>
    T anyObjectCast( const Any& any )
    {
        auto id = &any.type();
        // Any store an EObject
        if( id == &typeid( std::shared_ptr<EObject> ) )
        {
            auto object = anyCast<std::shared_ptr<EObject>>( any );
            return derived_pointer_cast<typename T::element_type>( object );
        }

        // Any store the correct Type
        if( id == &typeid( T ) )
            return anyCast<T>( any );
        return nullptr;
    }

    template <typename T>
    std::shared_ptr<EList<T>> anyListCast( const Any& any )
    {
        auto id = &any.type();

        if constexpr( IsSharedEObject<T>::value )
        {
            // Any store a list of EObject
            if( id == &typeid( std::shared_ptr<EList<std::shared_ptr<EObject>>> ) )
            {
                auto l = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( any );
                if constexpr( std::is_same<EObject, typename T::element_type>::value )
                    return l;
                else
                    return l->asEListOf<T>();
            }

            // Any store a list of EObject with the correct type
            if( id == &typeid( std::shared_ptr<EList<T>> ) )
                return anyCast<std::shared_ptr<EList<T>>>( any );

            // Any store a list of Any
            if( id == &typeid( std::shared_ptr<EList<Any>> ) )
            {
                auto l = anyCast<std::shared_ptr<EList<Any>>>( any );
                auto o = l->asEListOf<std::shared_ptr<EObject>>();
                if constexpr( std::is_same<EObject, typename T::element_type>::value )
                    return o;
                else
                    return o->asEListOf<T>();
            }
        }
        else
        {
            if( id == &typeid( std::shared_ptr<EList<T>> ) )
                return anyCast<std::shared_ptr<EList<T>>>( any );
        }
        return nullptr;
    }

} // namespace ecore

#endif /* ECORE_ANYCAST_HPP_ */
