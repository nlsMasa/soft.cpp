// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_PROXY_HPP_
#define ECORE_PROXY_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/SmartPtr.hpp"
#include "ecore/TypeTraits.hpp"
#include "ecore/impl/Notification.hpp"

namespace ecore
{
    class EObject;
}

namespace ecore::impl
{
    template <typename T, typename Enable = void>
    class Proxy
    {
    };

    template <typename T>
    class Proxy<T, typename std::enable_if<is_shared_ptr<T>::value>::type>
    {
    public:
        
        Proxy( const Proxy& o )
            : resolved_( o.resolved_ )
            , value_( o.value_ )
        {
        }

        T get() const
        {
            auto resolved = resolved_.lock();
            return resolved ? resolved : value_;
        }

        void set( const T& value )
        {
            if( value )
            {
                if( value->eIsProxy() )
                {
                    resolved_.reset();
                    value_ = std::move( value );
                }
                else
                {
                    resolved_ = value;
                    value_.reset();
                }
            }
            else
            {
                resolved_.reset();
                value_.reset();
            }
        }

        explicit operator bool() const
        {
            return static_cast<bool>( is_uninitialized( resolved_ ) ? value_ : resolved_.lock() );
        }

        typename T::element_type* operator->() const
        {
            return is_uninitialized( resolved_ ) ? value_.operator->() : resolved_.lock().operator->();
        }

        Proxy& operator=( const Proxy& o )
        {
            resolved_ = o.resolved_;
            value_ = o.value_;
            return *this;
        }

        Proxy& operator=( const T& value )
        {
            set( value );
            return *this;
        }

        bool operator==( const Proxy& o ) const
        {
            return equals( resolved_, o.resolved_ ) && value_ == o.value_;
        }

        bool operator!=( const Proxy& o ) const
        {
            return !operator==( o );
        }

        bool operator==( const T& value ) const
        {
            return value_ == value;
        }

        bool operator!=( const T& value ) const
        {
            return !operator==( value );
        }

    private:
        template <typename T, typename U>
        inline static bool equals( const std::weak_ptr<T>& t, const std::weak_ptr<U>& u )
        {
            return ( is_uninitialized( t ) && is_uninitialized( u ) ) || ( !is_uninitialized( t ) && t.lock() == u.lock() );
        }

    private:
        std::weak_ptr<typename T::element_type> resolved_;
        T value_;
    };

    template <typename T>
    bool operator==( std::nullptr_t, const Proxy<T>& right ) noexcept
    { // test if nullptr == shared_ptr
        return ( nullptr == right.value() );
    }

    template <typename T>
    bool operator==( const Proxy<T>& left, nullptr_t ) noexcept
    { // test if nullptr == shared_ptr
        return ( left.value() == nullptr );
    }

    template <typename T>
    bool operator!=( const Proxy<T>& left, std::nullptr_t right ) noexcept
    { // test if shared_ptr != nullptr
        return ( !( left == right ) );
    }

    template <typename T>
    bool operator!=( std::nullptr_t left, const Proxy<T>& right ) noexcept
    { // test if nullptr != shared_ptr
        return ( !( left == right ) );
    }
} // namespace ecore::impl

#endif
