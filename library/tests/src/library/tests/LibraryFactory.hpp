// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef LIBRARY_TESTS_LIBRARYFACTORY_CPP
#define LIBRARY_TESTS_LIBRARYFACTORY_CPP

#include "library/Library.hpp"

namespace library::tests
{
    class LibraryFactory
    {
    public:
        static std::shared_ptr<Library> createLibrary( int nb_employees, int nb_writers, int nb_books, int nb_borrowers );
    };

} // namespace library::tests

#endif // !LIBRARY_TESTS_LIBRARYFACTORY_CPP
