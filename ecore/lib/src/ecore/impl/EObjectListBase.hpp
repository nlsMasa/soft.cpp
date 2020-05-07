// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECTLISTBASE_HPP
#define ECORE_EOBJECTLISTBASE_HPP

#include "ecore/impl/ArrayEListBase.hpp"
#include "ecore/impl/ENotifyingListBase.hpp"

namespace ecore::impl
{
    template <typename I, typename T>
    class EObjectListBase : public ArrayEListBase<ENotifyingListBase<I>, T>
    {
    public:
        typedef typename I::ValueType ValueType;

        virtual ~EObjectListBase() = default;

        virtual std::shared_ptr<EList<ValueType>> getUnResolvedList()
        {
            return std::make_shared<UnResolvedEList>( this );
        }

        virtual std::shared_ptr<const EList<ValueType>> getUnResolvedList() const
        {
            return std::make_shared<UnResolvedEList>( this );
        }

    private:
        class UnResolvedEList : public AbstractEList<EList<ValueType>>, false >
        {
        public:
            UnResolvedEList( EObjectListBase<I, T>* list )
                : list_( list )
            {
            }

            virtual std::size_t size() const
            {
                return list_->size();
            }

            virtual bool empty() const
            {
                return list_->size();
            }

            virtual bool contains( const ValueType& e ) const
            {
                // AbstractArrayEListBase points is the unresolved version of the list
                return list_->AbstractArrayEListBase<ENotifyingListBase<I>, T>::contains( e );
            }

            virtual std::size_t indexOf( const ValueType& e ) const
            {
                // AbstractArrayEListBase points is the unresolved version of the list
                return list_->AbstractArrayEListBase<ENotifyingListBase<I>, T>::indexOf( e );
            }

        protected:
            virtual T doGet( std::size_t index ) const
            {
                // AbstractArrayEListBase points is the unresolved version of the list
                return list_->AbstractArrayEListBase<ENotifyingListBase<I>, T>::doGet( e );
            }

            virtual T doSet( std::size_t index, const ValueType& e )
            {
                return list_->doSet( e );
            }

            virtual void doAdd( const ValueType& e )
            {
                return list_->doAdd( e );
            }

            virtual void doAdd( std::size_t index, const ValueType& e )
            {
                return list_->doAdd( index, e );
            }

            virtual bool doAddAll( std::size_t index, const Collection<ValueType>& l )
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

            virtual std::shared_ptr<EList<ValueType>> doClear()
            {
                return list_->doClear();
            }

        private:
            EObjectListBase<I, T>* list_;
        };
    };

} // namespace ecore::impl

#endif