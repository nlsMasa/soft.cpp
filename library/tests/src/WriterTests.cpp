#include <boost/test/unit_test.hpp>

#include "library/LibraryFactory.hpp"
#include "library/Writer.hpp"
#include "library/ext/WriterExt.hpp"

using namespace library;
using namespace library::ext;

BOOST_AUTO_TEST_SUITE( WriterTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto f = LibraryFactory::eInstance();
    auto w = f->createWriter();
    BOOST_CHECK( std::dynamic_pointer_cast<WriterExt>( w ) != nullptr );
}

BOOST_AUTO_TEST_CASE( Name )
{
    auto f = LibraryFactory::eInstance();
    auto w = f->createWriter();

    w->setName( "first name--last name" );
    BOOST_CHECK_EQUAL( w->getFirstName(), "first name" );
    BOOST_CHECK_EQUAL( w->getLastName(), "last name" );
    BOOST_CHECK_EQUAL( w->getName(), "first name--last name" );
}

BOOST_AUTO_TEST_SUITE_END()

