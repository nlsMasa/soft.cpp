// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EMAPBASE_HPP_
#define ECORE_EMAPBASE_HPP_

#include "ecore/Any.hpp"
#include "ecore/EMap.hpp"

#include <functional>
#include <unordered_set>
#include <vector>
// clang-format off
#include <fstream>
// clang-format on
namespace ecore::impl
{

    // EMapBase defines all EMap interface methods
    // EMapBase handles delegate methods to corresponding to `do*` methods
    template <class Interface, class EntryType>
    class EMapBase : public Interface
    {
    public:
        using InterfaceType = Interface;
        using KeyType = typename Interface::KeyType;
        using MappedType = typename Interface::MappedType;
        using ValueType = typename Interface::ValueType;

        EMapBase() = default;
        virtual ~EMapBase() = default;

        // From `EMap`

        MappedType at( const KeyType& key ) const override
        {
            const auto found = map_.find( key );
            VERIFY( found != map_.end(), "key not found" );
            return anyCast<MappedType>( doGet( indexOf( [&key]( const auto& entry ) { return entry->getKey() == key; } ) )->getValue() );
        }

        void insert( const KeyType& key, const MappedType& value ) override
        {
            auto entry = std::make_shared<EntryType>( key, value );
            auto index = indexOf( [&entry]( const auto& e ) { return e->getKey() == entry->getKey(); } );
            {
                std::ofstream tmpdbg( "C:\\workspace\\.log\\tmpdbg", std::ios::app );
                tmpdbg << "insert( " << key << ", " << value << " ) @ " << index << "\n";
                tmpdbg.close();
            }
            if( index != std::numeric_limits<std::size_t>::max() )
                doSet( index, entry );
            else
                doAdd( entry );
        }

        void merge( const Collection<ValueType>& collection ) override
        {
            for( const auto& entry : collection )
                insert( anyCast<KeyType>( entry->getKey() ), anyCast<MappedType>( entry->getValue() ) );
        }

        bool remove( const KeyType& key ) override
        {
            auto found = map_.find( key );
            if( found == map_.end() )
                return false;
            doRemove( indexOf( [&key]( const auto& entry ) { return entry->getKey() == key; } ) );
            return true;
        }

        void clear() override
        {
            doClear();
        }

        bool empty() const override
        {
            return map_.empty();
        }

        std::size_t size() const override
        {
            return map_.size();
        }

        bool contains( const KeyType& key ) const override
        {
            return map_.find( key ) != map_.end();
        }

        std::optional<KeyType> keyOf( const MappedType& searched ) const override
        {
            for( const auto& [key, value] : map_ )
                if( value == searched )
                    return key;
            return std::nullopt;
        }

        // From `EList`

        bool add( std::size_t, const ValueType& ) override
        {
            throw "UnsupportedOperationException `add( std::size_t, const ValueType& )`";
        }

        bool addAll( std::size_t, const Collection<ValueType>& ) override
        {
            throw "UnsupportedOperationException `addAll( std::size_t, const Collection<ValueType>& )`";
        }

        void move( std::size_t, const ValueType& ) override
        {
            throw "UnsupportedOperationException `move( std::size_t, const ValueType& )`";
        }

        ValueType move( std::size_t, std::size_t ) override
        {
            throw "UnsupportedOperationException `move( std::size_t, std::size_t )`";
        }

        ValueType set( std::size_t, const ValueType& ) override
        {
            throw "UnsupportedOperationException `set( std::size_t, const ValueType& )`";
        }

        ValueType remove( std::size_t ) override
        {
            throw "UnsupportedOperationException `remove( std::size_t )`";
        }

        // From `Collection`

        bool add( const ValueType& entry ) override
        {
            auto key = anyCast<KeyType>( entry->getKey() );
            if( contains( key ) )
                return false;
            insert( key, anyCast<MappedType>( entry->getValue() ) );
            return true;
        }

        bool addAll( const Collection<ValueType>& entries ) override
        {
            for( const auto& entry : entries )
                add( entry );
            return true;
        }

        bool remove( const ValueType& entry ) override
        {
            return remove( anyCast<KeyType>( entry->getKey() ) );
        }

        ValueType get( std::size_t index ) const override
        {
            return doGet( index );
        }

        bool contains( const ValueType& entry ) const override
        {
            return contains( anyCast<KeyType>( entry->getKey() ) );
        }

    protected:
        virtual ValueType doGet( std::size_t index ) const = 0;
        virtual ValueType doSet( std::size_t index, const ValueType& e ) = 0;
        virtual void doAdd( const ValueType& e ) = 0;
        virtual void doAdd( std::size_t index, const ValueType& e ) = 0;
        virtual bool doAddAll( std::size_t index, const Collection<ValueType>& l ) = 0;
        virtual ValueType doRemove( std::size_t index ) = 0;
        virtual std::shared_ptr<EList<ValueType>> doClear() = 0;

    protected:
        virtual void didSet( std::size_t, const ValueType& entry, const ValueType& )
        {
            didAdd( 0, entry );
        }

        virtual void didAdd( std::size_t, const ValueType& entry )
        {
            map_[anyCast<KeyType>( entry->getKey() )] = anyCast<MappedType>( entry->getValue() );
        }

        virtual void didRemove( std::size_t, const ValueType& entry )
        {
            map_.erase( anyCast<KeyType>( entry->getKey() ) );
        }

        virtual void didClear( const std::vector<ValueType>& entries )
        {
            map_.clear();
        }

        virtual void didMove( std::size_t, const ValueType&, std::size_t )
        {
            // Do nothing.
        }

        virtual void didChange()
        {
            // Do nothing.
        }

    private:
        std::map<KeyType, MappedType> map_;
    };

} // namespace ecore::impl

#endif /* ECORE_EMAPBASE_HPP_ */
