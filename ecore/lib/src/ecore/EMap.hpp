// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2023 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EMAP_HPP_
#define ECORE_EMAP_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObjectHelpers.hpp"
#include "ecore/TypeTraits.hpp"

#include <limits>
#include <map>
#include <memory>
#include <optional>

namespace ecore
{

    class EMapEntry
    {
    public:
        virtual ~EMapEntry() = default;

        virtual Any getKey() const = 0;
        virtual void setKey( const Any& ) = 0;
        virtual Any getValue() const = 0;
        virtual void setValue( const Any& ) = 0;

        bool operator==( const EMapEntry& other ) const
        {
            return getKey() == other.getKey();
        }

        bool operator!=( const EMapEntry& other ) const
        {
            return !( *this == other );
        }
    };

    template <class Key, class Value>
    class EMap : public EList<std::shared_ptr<EMapEntry>>
    {
    public:
        using Super = EList<std::shared_ptr<EMapEntry>>;
        using KeyType = Key;
        using MappedType = Value;
        using ValueType = Super::ValueType;

        virtual ~EMap() = default;

        virtual MappedType at( const KeyType& ) const = 0;

        virtual void insert( const KeyType&, const MappedType& ) = 0;

        virtual void merge( const Collection<ValueType>& ) = 0;

        virtual bool remove( const KeyType& ) = 0;

        virtual void clear() = 0;

        virtual bool empty() const = 0;

        virtual bool contains( const KeyType& ) const = 0;

        virtual std::optional<KeyType> keyOf( const MappedType& ) const = 0;

        virtual bool remove( const ValueType& ) = 0;

        virtual bool contains( const ValueType& ) const = 0;

        /**
         * Allows treating an `EMap<Key, Value>` as an `EMap<Key, OtherValue>` (if `Value` can be dynamically cast to `OtherValue`)
         */
        template <class OtherValue>
        inline std::shared_ptr<EMap<Key, OtherValue>> asEMapOf()
        {
            auto map = std::static_pointer_cast<EMap<Key, Value>>( shared_from_this() );
            return std::make_shared<detail::DelegateEMap<EMap<Key, OtherValue>, EMap<Key, Value>>>( map );
        }

        /**
         * Allows treating an `EMap<Key, Value> const` as an `EMap<Key, OtherValue> const` (if `Value` can be dynamically cast to
         * `OtherValue`)
         */
        template <class OtherValue>
        inline std::shared_ptr<const EMap<Key, OtherValue>> asEMapOf() const
        {
            auto map = std::static_pointer_cast<const EMap<Key, Value>>( shared_from_this() );
            return std::make_shared<detail::ConstDelegateEMap<EMap<Key, OtherValue>, EMap<Key, Value>>>( map );
        }
    };

    template <class Key, class Value>
    inline typename EMap<Key, Value>::Iterator begin( const std::unique_ptr<EMap<Key, Value>>& m )
    {
        return m->begin();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::ConstIterator begin( const std::unique_ptr<const EMap<Key, Value>>& m )
    {
        return m->begin();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::Iterator end( const std::unique_ptr<EMap<Key, Value>>& m )
    {
        return m->end();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::ConstIterator end( const std::unique_ptr<const EMap<Key, Value>>& m )
    {
        return m->end();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::Iterator begin( const std::shared_ptr<EMap<Key, Value>>& m )
    {
        return m->begin();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::ConstIterator begin( const std::shared_ptr<const EMap<Key, Value>>& m )
    {
        return m->begin();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::Iterator end( const std::shared_ptr<EMap<Key, Value>>& m )
    {
        return m->end();
    }

    template <class Key, class Value>
    inline typename EMap<Key, Value>::ConstIterator end( const std::shared_ptr<const EMap<Key, Value>>& m )
    {
        return m->end();
    }

    namespace detail
    {

        template <class MapTo, class MapFrom, class = std::enable_if_t<std::is_same_v<typename MapFrom::KeyType, typename MapTo::KeyType>>>
        class ConstDelegateEMap : public ConstDelegateCollection<MapTo, MapFrom>
        {
        public:
            using KeyType = typename MapFrom::KeyType;
            using ValueFrom = typename MapFrom::ValueType;
            using MappedFrom = typename MapFrom::MappedType;
            using ValueTo = typename MapTo::ValueType;
            using MappedTo = typename MapTo::MappedType;
            using MapDelegate = std::shared_ptr<const MapFrom>;

            ConstDelegateEMap( const MapDelegate& delegate )
                : ConstDelegateCollection<MapTo, MapFrom>( delegate )
            {
            }

            virtual ~ConstDelegateEMap() = default;

            // From `EMap`

            MappedTo at( const KeyType& key ) const override
            {
                return cast<MappedFrom, MappedTo>::do_cast( delegate_->at( key ) );
            }

            void insert( const KeyType&, const MappedTo& ) override
            {
                throw "UnsupportedOperationException";
            }

            void merge( const Collection<ValueTo>& ) override
            {
                throw "UnsupportedOperationException";
            }

            bool remove( const KeyType& ) override
            {
                throw "UnsupportedOperationException";
            }

            void clear() override
            {
                throw "UnsupportedOperationException";
            }

            bool empty() const override
            {
                return delegate_->empty();
            }

            std::size_t size() const override
            {
                return delegate_->size();
            }

            bool contains( const KeyType& key ) const override
            {
                return delegate_->contains( key );
            }

            std::optional<KeyType> keyOf( const MappedTo& value ) const override
            {
                return delegate_->keyOf( cast<MappedTo, MappedFrom>::do_cast( value ) );
            }

            // From `EList`

            std::size_t indexOf( const ValueTo& entry ) const override
            {
                return delegate_->indexOf( cast<ValueTo, ValueFrom>::do_cast( entry ) );
            }

            size_t indexOf( std::function<bool( const ValueTo& )> predicate ) const
            {
                std::function<bool( const ValueFrom& )> flatmap
                    = [&predicate]( const ValueFrom& e ) { return predicate( cast<ValueFrom, ValueTo>::do_cast( e ) ); };
                return delegate_->indexOf( flatmap );
            }

            // From `Collection`

            bool add( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            bool addAll( size_t, const ecore::Collection<ValueTo>& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo move( size_t, size_t )
            {
                throw "UnsupportedOperationException";
            }

            void move( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo set( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo remove( size_t )
            {
                throw "UnsupportedOperationException";
            }

            bool remove( const ValueTo& entry ) override
            {
                throw "UnsupportedOperationException";
            }

            bool contains( const ValueTo& entry ) const override
            {
                throw "UnsupportedOperationException";
            }
        };

        template <class MapTo, class MapFrom, class = std::enable_if_t<std::is_same_v<MapFrom::KeyType, MapTo::KeyType>>>
        class DelegateEMap : public DelegateCollection<MapTo, MapFrom>
        {
        public:
            using KeyType = typename MapFrom::KeyType;
            using ValueFrom = typename MapFrom::ValueType;
            using MappedFrom = typename MapFrom::MappedType;
            using ValueTo = typename MapTo::ValueType;
            using MappedTo = typename MapTo::MappedType;
            using MapDelegate = std::shared_ptr<MapFrom>;

            DelegateEMap( const MapDelegate& delegate )
                : DelegateCollection<MapTo, MapFrom>( delegate )
            {
            }

            virtual ~DelegateEMap() = default;

            // From `EMap`

            MappedTo at( const KeyType& key ) const override
            {
                return cast<MappedFrom, MappedTo>::do_cast( delegate_->at( key ) );
            }

            void insert( const KeyType& key, const MappedTo& value ) override
            {
                delegate_->insert( key, cast<MappedTo, MappedFrom>::do_cast( value ) );
            }

            bool remove( const KeyType& key ) override
            {
                return delegate_->remove( key );
            }

            void merge( const Collection<ValueTo>& collection ) override
            {
                for( const auto& entry : collection )
                    insert( anyCast<KeyType>( entry->getKey() ),
                            cast<MappedTo, MappedFrom>::do_cast( anyCast<MappedTo>( entry->getValue() ) ) );
            }

            void clear() override
            {
                delegate_->clear();
            }

            bool empty() const override
            {
                return delegate_->empty();
            }

            std::size_t size() const override
            {
                return delegate_->size();
            }

            bool contains( const KeyType& key ) const override
            {
                return delegate_->contains( key );
            }

            std::optional<KeyType> keyOf( const MappedTo& value ) const override
            {
                return delegate_->keyOf( cast<MappedTo, MappedFrom>::do_cast( value ) );
            }

            // From `EList`

            std::size_t indexOf( const ValueTo& entry ) const override
            {
                return delegate_->indexOf( cast<ValueTo, ValueFrom>::do_cast( entry ) );
            }

            size_t indexOf( std::function<bool( const ValueTo& )> predicate ) const
            {
                std::function<bool( const ValueFrom& )> flatmap
                    = [&predicate]( const ValueFrom& e ) { return predicate( cast<ValueFrom, ValueTo>::do_cast( e ) ); };
                return delegate_->indexOf( flatmap );
            }

            // From `Collection`

            bool add( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            bool addAll( size_t, const ecore::Collection<ValueTo>& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo move( size_t, size_t )
            {
                throw "UnsupportedOperationException";
            }

            void move( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo set( size_t, const ValueTo& )
            {
                throw "UnsupportedOperationException";
            }

            ValueTo remove( size_t )
            {
                throw "UnsupportedOperationException";
            }

            bool remove( const ValueTo& entry ) override
            {
                throw "UnsupportedOperationException";
            }

            bool contains( const ValueTo& entry ) const override
            {
                throw "UnsupportedOperationException";
            }
        };

    } // namespace detail

} // namespace ecore

#endif /* ECORE_EMAP_HPP_ */
