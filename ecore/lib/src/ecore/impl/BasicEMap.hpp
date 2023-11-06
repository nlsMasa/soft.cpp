// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASIC_EMAP_HPP_
#define ECORE_BASIC_EMAP_HPP_

#include "ecore/EMap.hpp"
#include "ecore/impl/ArrayEListBase.hpp"
#include "ecore/impl/BasicEList.hpp"
#include "ecore/impl/EMapBase.hpp"
#include <boost/test/execution_monitor.hpp>

namespace ecore::impl
{

    template <class Key, class Value>
    class EMapEntryImpl : public EMapEntry
    {
    public:
        template <class = std::enable_if_t<std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>>>
        EMapEntryImpl()
            : key_()
            , value_()
        {
        }

        EMapEntryImpl( const Key& key, const Value& value )
            : key_( key )
            , value_( value )
        {
        }

        ~EMapEntryImpl() = default;

        Any getKey() const override
        {
            return key_;
        }

        void setKey( const Any& key ) override
        {
            key_ = anyCast<Key>( key );
        }

        Any getValue() const override
        {
            return value_;
        }

        void setValue( const Any& value ) override
        {
            value_ = anyCast<Value>( value );
        }

    private:
        Key key_;
        Value value_;
    };

    template <class Key, class Value>
    class BasicEMap : public ArrayEListBase<EMapBase<EMap<Key, Value>, EMapEntryImpl<Key, Value>>>
    {
    public:
        using Super = ArrayEListBase<EMapBase<EMap<Key, Value>, EMapEntryImpl<Key, Value>>>;
        using ValueType = typename std::shared_ptr<EMapEntryImpl<Key, Value>>;

        BasicEMap() = default;
        BasicEMap( const BasicEMap& ) = default;
        virtual ~BasicEMap() = default;

        BasicEMap( const std::vector<ValueType>& vector )
            : Super()
        {
            for( const auto& entry : vector )
                insert( anyCast<Key>( entry->getKey() ), anyCast<Value>( entry->getValue() ) );
        }

        BasicEMap( const std::vector<std::pair<Key, Value>>& init )
            : Super()
        {
            for( const auto& pair : init )
                insert( pair.first, pair.second );
        }
    };

} // namespace ecore::impl

#endif /* ECORE_BASIC_EMAP_HPP_ */
