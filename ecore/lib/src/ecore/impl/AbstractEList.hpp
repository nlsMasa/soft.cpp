// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTELIST_HPP_
#define ECORE_ABSTRACTELIST_HPP_

#include "ecore/EList.hpp"
#include "ecore/impl/ImmutableEList.hpp"

#include <unordered_set>
#include <vector>

namespace ecore::impl
{

    template <typename I, bool unique>
    class AbstractEList : public I
    {
    public:
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;

        AbstractEList()
        {
        }

        virtual ~AbstractEList()
        {
        }

        virtual bool add( const ValueType& e )
        {
            if constexpr( unique )
            {
                if( contains( e ) )
                    return false;
            }
            doAdd( e );
            return true;
        }

        virtual bool addAll( const EList<ValueType>& l )
        {
            return addAll( size(), l );
        }

        virtual bool add( std::size_t index, const ValueType& e )
        {
            VERIFY( index <= size(), "out of range" );
            if constexpr( unique )
            {
                VERIFY( !contains( e ), "element already in list : uniqueness constraint is violated" );
            }
            doAdd( index, e );
            return true;
        }

        virtual bool addAll( std::size_t index, const EList<ValueType>& l )
        {
            VERIFY( index <= size(), "out of range" );
            if constexpr( unique )
            {
                auto nonDuplicates = getNonDuplicates( l );
                return doAddAll( index, *nonDuplicates );
            }
            else
                return doAddAll( index, l );
        }

        virtual ValueType move( std::size_t newIndex, std::size_t oldIndex )
        {
            VERIFY( newIndex <= size(), "out of range" );
            VERIFY( oldIndex <= size(), "out of range" );
            return doMove( newIndex, oldIndex );
        }

        virtual void move( std::size_t newIndex, const ValueType& e )
        {
            move( newIndex, indexOf( e ) );
        }

        virtual ValueType get( std::size_t index ) const
        {
            VERIFY( index < size(), "out of range" );
            return doGet( index );
        }

        virtual ValueType set( std::size_t index, const ValueType& e )
        {
            VERIFY( index < size(), "out of range" );
            if constexpr( unique )
            {
                std::size_t currentIndex = indexOf( e );
                VERIFY( currentIndex == -1 || currentIndex == index, "element already in list : uniqueness constraint is violated" );
            }
            return doSet( index, e );
        }

        virtual bool remove( const ValueType& e )
        {
            std::size_t index = indexOf( e );
            if( index >= 0 )
            {
                remove( index );
                return true;
            }
            else
            {
                return false;
            }
        }

        virtual ValueType remove( std::size_t index )
        {
            VERIFY( index < size(), "out of range" );
            return doRemove( index );
        }

        virtual bool empty() const
        {
            return size() == 0;
        }

        virtual void clear()
        {
            doClear();
        }

    protected:
        virtual ValueType doGet( std::size_t index ) const = 0;

        virtual ValueType doSet( std::size_t index, const ValueType& e ) = 0;

        virtual void doAdd( const ValueType& e ) = 0;

        virtual void doAdd( std::size_t index, const ValueType& e ) = 0;

        virtual bool doAddAll( std::size_t index, const EList<ValueType>& l ) = 0;

        virtual ValueType doRemove( std::size_t index ) = 0;

        virtual ValueType doMove( std::size_t newIndex, std::size_t oldIndex ) = 0;

        virtual std::shared_ptr<EList<ValueType>> doClear() = 0;

    protected:
        virtual void didSet( std::size_t index, const ValueType& newObject, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didAdd( std::size_t index, const ValueType& newObject )
        {
            // Do nothing.
        }

        virtual void didRemove( std::size_t index, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didClear( const std::vector<ValueType>& oldObjects )
        {
            for( int i = 0; i < oldObjects.size(); ++i )
                didRemove( i, oldObjects[i] );
        }

        virtual void didMove( std::size_t index, const ValueType& movedObject, std::size_t oldIndex )
        {
            // Do nothing.
        }

        virtual void didChange()
        {
            // Do nothing.
        }

    private:
        std::unique_ptr<EList<ValueType>> getNonDuplicates( const EList<ValueType>& l )
        {
            std::unordered_set<ValueType> s;
            std::vector<ValueType> v;
            for( auto e : l )
            {
                auto i = s.insert( e );
                if( i.second )
                {
                    if( !contains( e ) )
                        v.push_back( e );
                }
            }
            return std::make_unique<ImmutableEList<ValueType>>( std::move( v ) );
        }
    };

} // namespace ecore::impl

#endif /* ECORE_ABSTRACTELIST_HPP_ */
