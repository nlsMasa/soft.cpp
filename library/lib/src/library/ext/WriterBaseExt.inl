// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef LIBRARY_EXT_WRITERBASEEXT_HPP
#error This file may only be included from WriterBaseExt.hpp
#endif

namespace library::ext
{
    template <typename... I>
    const std::string& WriterBaseExt<I...>::getName() const
    {
        name_ = getFirstName() + "--" + getLastName();
        return name_;
    }

    template <typename... I>
    void WriterBaseExt<I...>::setName( const std::string& newName )
    {
        auto i = newName.find( "--" );
        if( i != std::string::npos )
        {
            setFirstName( newName.substr( 0, i ) );
            setLastName( newName.substr( i + 2 ) );
        }
    }
}