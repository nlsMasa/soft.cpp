// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef LIBRARY_EXT_WRITERBASEEXT_HPP
#define LIBRARY_EXT_WRITERBASEEXT_HPP

#include "library/impl/WriterBase.hpp"

namespace library::ext
{
    template <typename... I>
    class WriterBaseExt : public library::impl::WriterBase<I...>
    {
    public:
        WriterBaseExt() = default;

        virtual ~WriterBaseExt() = default;

        virtual const std::string& getName() const;

        virtual void setName( const std::string& newName );

    private:
        WriterBaseExt& operator=( WriterBaseExt const& ) = delete;

    private:
        mutable std::string name_;
    };
    


} // namespace library::ext

#include "library/ext/WriterBaseExt.inl"

#endif /* LIBRARY_EXT_WRITERBASEEXT_HPP */
