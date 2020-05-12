#include <boost/test/unit_test.hpp>

#include "ecore/EcoreUtils.hpp"
#include "library/tests/LibraryFactory.hpp"

using namespace ecore;
using namespace library;
using namespace library::tests;

namespace
{
    constexpr int nb_employees = 10;
    constexpr int nb_writers = 100;
    constexpr int nb_books = 1000;
    constexpr int nb_borrowers = 100;

} // namespace

BOOST_AUTO_TEST_SUITE( DeepUtilsTests )

BOOST_AUTO_TEST_CASE( DeepCopyAndEquals )
{
    auto l = LibraryFactory::createLibrary( nb_employees, nb_writers, nb_books, nb_borrowers );
    auto lbis = EcoreUtils::copy( l );
    BOOST_CHECK( EcoreUtils::equals( l, lbis ) );
}

BOOST_AUTO_TEST_SUITE_END()