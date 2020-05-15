// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECOLLECTIONVIEW_HPP_
#define ECORE_ECOLLECTIONVIEW_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/ETreeIterator.hpp"
namespace ecore
{
    template <typename T>
    class ECollectionView
    {
    };

    template <>
    class ECollectionView<std::shared_ptr<EObject>>
    {
    public:
        ECollectionView( const std::shared_ptr<EObject>& eObject, bool resolve = true )
            : elements_( resolve ? eObject->eContents() : eObject->eContents()->getUnResolvedList() )
            , resolve_( resolve )
        {
        }

        ECollectionView( const std::shared_ptr<const EList<std::shared_ptr<EObject>>>& elements, bool resolve = true )
            : elements_( elements )
            , resolve_( resolve )
        {
        }

        ETreeIterator<std::shared_ptr<EObject>> begin() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>( elements_, [&]( const std::shared_ptr<EObject>& eObject ) {
                return resolve_ ? eObject->eContents() : eObject->eContents()->getUnResolvedList();
            } );
        }

        ETreeIterator<std::shared_ptr<EObject>> end() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>();
        }

    private:
        std::shared_ptr<const EList<std::shared_ptr<EObject>>> elements_;
        bool resolve_;
    };
} // namespace ecore

#endif /* ECORE_ECOLLECTIONVIEW_HPP_ */
