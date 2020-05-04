// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#define ECORE_ELIST_HPP_

#include "ecore/Assert.hpp"
#include <memory>

namespace ecore
{
    namespace detail
    {
        template <typename ListType>
        class EListIterator;
    }

    template <typename T>
    class EList : public std::enable_shared_from_this<EList<T>>
    {
    public:
        typedef typename T ValueType;

        virtual ~EList() = default;

        virtual bool add( const T& e ) = 0;

        virtual bool addAll( const EList<T>& l ) = 0;

        virtual bool add( std::size_t pos, const T& e ) = 0;

        virtual bool addAll( std::size_t pos, const EList<T>& l ) = 0;

        virtual void move( std::size_t newPos, const T& e ) = 0;

        virtual T move( std::size_t newPos, std::size_t oldPos ) = 0;

        virtual T get( std::size_t pos ) const = 0;

        virtual T set( std::size_t pos, const T& e ) = 0;

        virtual T remove( std::size_t pos ) = 0;

        virtual bool remove( const T& e ) = 0;

        virtual std::size_t size() const = 0;

        virtual void clear() = 0;

        virtual bool empty() const = 0;

        virtual bool contains( const T& e ) const
        {
            return std::find( begin(), end(), e ) != end();
        }

        std::size_t indexOf( const T& e ) const
        {
            std::size_t index = std::distance( begin(), std::find( begin(), end(), e ) );
            return index == size() ? -1 : index;
        }

        typedef detail::EListIterator<EList<T>> Iterator;
        typedef detail::EListIterator<const EList<T>> ConstIterator;

        Iterator begin()
        {
            return Iterator( this, 0 );
        }

        ConstIterator begin() const
        {
            return ConstIterator( this, 0 );
        }

        Iterator end()
        {
            return Iterator( this, size() );
        }

        ConstIterator end() const
        {
            return ConstIterator( this, size() );
        }

        ConstIterator cbegin() const
        {
            return begin();
        }

        ConstIterator cend() const
        {
            return end();
        }

        /**
         * Allows treating an EList<T> as an EList<Q> (if T can be casted to Q dynamically)
         */
        template <typename Q>
        inline std::shared_ptr<EList<Q>> asEListOf()
        {
            return std::make_shared<detail::DelegateEList<Q, T>>( shared_from_this() );
        }

        template <typename Q>
        inline std::shared_ptr<const EList<Q>> asEListOf() const
        {
            return std::make_shared<detail::ConstDelegateEList<Q, T>>( shared_from_this() );
        }
    };

    template <typename T>
    inline typename EList<T>::Iterator begin( const std::unique_ptr<EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator begin( const std::unique_ptr<const EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::Iterator end( const std::unique_ptr<EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator end( const std::unique_ptr<const EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::Iterator begin( const std::shared_ptr<EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator begin( const std::shared_ptr<const EList<T>>& l )
    {
        return l->begin();
    }

    template <typename T>
    inline typename EList<T>::Iterator end( const std::shared_ptr<EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline typename EList<T>::ConstIterator end( const std::shared_ptr<const EList<T>>& l )
    {
        return l->end();
    }

    template <typename T>
    inline bool operator==( const EList<T>& lhs, const EList<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    template <typename T>
    inline bool operator!=( const EList<T>& lhs, const EList<T>& rhs )
    {
        return !( lhs == rhs );
    }

} // namespace ecore

#include "ecore/EList.inl"

#endif /* ECORE_ELIST_HPP_ */
