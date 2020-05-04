// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTEOBJECTLIST_HPP_
#define ECORE_ABSTRACTEOBJECTLIST_HPP_

#include "ecore/EObjectList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/Proxy.hpp"

namespace ecore::impl
{
    template <typename Base, typename T = typename Base::ValueType>
    class AbstractEObjectList : public Base
    {
    public:
        virtual ~AbstractEObjectList() = default;

        virtual std::shared_ptr<EList<T>> getUnResolvedList()
        {
            return shared_from_this();
        }
    };

    template <typename Base, typename T>
    class AbstractEObjectList<Base, Proxy<T>> : public Base
    {
    public:
        virtual ~AbstractEObjectList() = default;

        virtual std::shared_ptr<EList<T>> getUnResolvedList()
        {
            class UnResolvedList : public AbstractEList<EList<T>, false>
            {
            public:
                UnResolvedList( AbstractEObjectList* list )
                    : list_( list )
                {
                }

                virtual ~UnResolvedList() = default;

                virtual std::size_t size() const
                {
                    return list_->size();
                }

                virtual bool empty() const
                {
                    return list_->empty();
                }

                virtual bool contains( const T& e ) const
                {
                    return indexOf( e ) != -1;
                }

                virtual std::size_t indexOf( const T& e ) const
                {
                    const auto& v = list_->getVector();
                    for( std::size_t i = 0; i < size(); ++i )
                    {
                        if( v[i].get() == e )
                            return i;
                    }
                    return -1;
                }

            protected:
                virtual T doGet( std::size_t index ) const
                {
                    const auto& v = list_->getVector();
                    return v[index].get();
                }

                virtual T doSet( std::size_t index, const T& e )
                {
                    return list_->doSet( index, e );
                }

                virtual void doAdd( const T& e )
                {
                    return list_->doAdd( e );
                }

                virtual void doAdd( std::size_t index, const T& e )
                {
                    return list_->doAdd( index, e );
                }

                virtual bool doAddAll( std::size_t index, const EList<T>& l )
                {
                    return list_->doAddAll( index, l );
                }

                virtual T doRemove( std::size_t index )
                {
                    return list_->doRemove( index );
                }

                virtual T doMove( std::size_t newIndex, std::size_t oldIndex )
                {
                    return list_->doMove( newIndex, oldIndex );
                }

                virtual std::shared_ptr<EList<T>> doClear()
                {
                    return list_->doClear();
                }

            private:
                AbstractEObjectList* list_;
            };
            return std::make_shared<UnResolvedList>( this );
        }
    };


} // namespace ecore::impl

#endif /* ECORE_EOBJECTELIST_HPP_ */
