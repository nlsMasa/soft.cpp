// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ETREEITERATOR_HPP_
#define ECORE_ETREEITERATOR_HPP_

#include "ecore/EList.hpp"

#include <memory>
#include <stack>
#include <type_traits>
#include <functional>

namespace ecore
{

    template <typename T>
    class ETreeIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = typename T;
        using pointer           = typename T*;
        using reference         = typename T&;

    public:
        explicit ETreeIterator()
        {
        }


        explicit ETreeIterator( const std::shared_ptr<const EList<T>>& elements,
                                std::function<std::shared_ptr<const EList<T>>( const T& )> getChildren )
            : getChildren_( getChildren )
        {
            if( elements )
            {
                auto it = elements->begin();
                stack_.push( it );
                current_ = *it;
            }
        }

        ETreeIterator& operator++()
        {
            if( !current_ )
                return *this;

            auto children = getChildren_( current_ );
            auto it = children->begin();
            if( it != children->end() )
            {
                stack_.push( it );
                current_ = *it;
                return *this;
            }

            if( stack_.empty() )
            {
                current_ = nullptr;
                return *this;
            }

            it = stack_.top();
            while( !it.hasNext() )
            {
                stack_.pop();
                if( stack_.empty() )
                {
                    current_ = nullptr;
                    return *this;
                }
                it = stack_.top();
            }

            it = ++stack_.top();
            current_ = *it;
            return *this;
        }

        ETreeIterator operator++( int )
        {
            TreeIterator retval = *this;
            ++( *this );
            return retval;
        }

        bool operator==( const ETreeIterator& other ) const
        {
            return current_ == other.current_;
        }

        bool operator!=( const ETreeIterator& other ) const
        {
            return !( *this == other );
        }

        T operator*() const
        {
            return current_;
        }

        size_t level() const
        {
            return stack_.size();
        }
    
    private:
        std::function< std::shared_ptr<const EList<T>>( const T& )> getChildren_;
        T current_;
        std::stack<typename EList<T>::ConstIterator> stack_;
    };
}

#endif /* ECORE_ETREEITERATOR_HPP_ */
