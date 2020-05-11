// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMMUTABLE_ELIST_BASE_HPP_
#define ECORE_IMMUTABLE_ELIST_BASE_HPP_

namespace ecore::impl
{

    template <typename I>
    class ImmutableEListBase : public I
    {
    public:
        typedef typename I::ValueType ValueType;

        virtual ~ImmutableEListBase() = default;
        
        virtual bool add( const ValueType& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( const Collection<ValueType>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool add( std::size_t pos, const ValueType& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( std::size_t pos, const Collection<ValueType>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual void move( std::size_t oldPos, const ValueType& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual ValueType move( std::size_t oldPos, std::size_t newPos )
        {
            throw "UnsupportedOperationException";
        }

        virtual ValueType set( std::size_t pos, const ValueType& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual ValueType remove( std::size_t pos )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool remove( const ValueType& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual void clear()
        {
            throw "UnsupportedOperationException";
        }
        
        virtual std::shared_ptr<EList<ValueType>> getUnResolvedList()
        {
            throw "UnsupportedOperationException";
        }

        virtual std::shared_ptr<const EList<ValueType>> getUnResolvedList() const
        {
            return std::static_pointer_cast<const EList<ValueType>>( shared_from_this() );
        }

    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_ELIST_BASE_HPP_ */
