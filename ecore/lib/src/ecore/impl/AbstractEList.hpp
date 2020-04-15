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
            if constexpr( unique )
            {
                auto nonDuplicates = getNonDuplicates( l );
                return doAddAll( *nonDuplicates );
            }
            else
            {
                return doAddAll( l );
            }
        }

        virtual bool add( std::size_t pos, const ValueType& e )
        {
            VERIFY( pos <= size(), "out of range" );
            if constexpr( unique )
            {
                VERIFY( !contains( e ), "element already in list : uniqueness constraint is violated" );
            }
            doAdd( pos, e );
            return true;
        }

        virtual bool addAll( std::size_t pos, const EList<ValueType>& l )
        {
            VERIFY( pos <= size(), "out of range" );
            if constexpr( unique )
            {
                auto nonDuplicates = getNonDuplicates( l );
                return doAddAll( pos, *nonDuplicates );
            }
            else
                return doAddAll( pos, l );
        }

        using EList::move;

        virtual void move( std::size_t newPos, const ValueType& e )
        {
            move( newPos, indexOf( e ) );
        }

        virtual ValueType get( std::size_t pos ) const
        {
            VERIFY( pos < size(), "out of range" );
            return doGet( pos );
        }

        virtual ValueType set( std::size_t pos, const ValueType& e )
        {
            VERIFY( pos < size(), "out of range" );
            if constexpr ( unique )
            {
                std::size_t currentIndex = indexOf( e );
                VERIFY( currentIndex == -1 || currentIndex == pos, "element already in list : uniqueness constraint is violated" );
            }
            return doSet( pos, e );
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

        virtual ValueType remove( std::size_t index ) = 0;


        virtual void clear()
        {
            doClear();
        }

        virtual bool empty() const
        {
            return size() == 0;
        }

    protected:

        virtual ValueType doGet( std::size_t pos ) const = 0;

        virtual ValueType doSet( std::size_t pos, const ValueType& e ) = 0;

        virtual void doAdd( const ValueType& e ) = 0;

        virtual void doAdd( std::size_t pos, const ValueType& e ) = 0;

        virtual bool doAddAll( const EList<ValueType>& l ) = 0;

        virtual bool doAddAll( std::size_t pos, const EList<ValueType>& l ) = 0;

        virtual void doClear() = 0;

    protected:
        virtual void didSet( std::size_t pos, const ValueType& newObject, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didAdd( std::size_t pos, const ValueType& newObject )
        {
            // Do nothing.
        }

        virtual void didRemove( std::size_t pos, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didClear( const std::vector<ValueType>& oldObjects )
        {
            for( int i = 0; i < oldObjects.size(); ++i )
                didRemove( i, oldObjects[i] );
        }

        virtual void didMove( std::size_t pos, const ValueType& movedObject, std::size_t oldIndex )
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
