#include <boost/test/unit_test.hpp>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"

#include <ctime>

using namespace ecore;

BOOST_AUTO_TEST_SUITE( EcoreFactoryTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    BOOST_CHECK( EcoreFactory::eInstance() );
}

BOOST_AUTO_TEST_CASE( Getters )
{
    BOOST_CHECK_EQUAL( EcoreFactory::eInstance()->getEcorePackage(), EcorePackage::eInstance() );
}

#ifdef _WINDOWS

auto timegm = _mkgmtime;

#endif // WINDOWS

BOOST_AUTO_TEST_CASE( CreateDateFromString )
{
    auto dateType = EcorePackage::eInstance()->getEDate();
    auto dateAny = EcoreFactory::eInstance()->createFromString( dateType, "1974-06-20T05:22:10.000" );
    auto t = anyCast<std::time_t>( dateAny );
    std::tm tm = {10, 22, 5, 20, 5, 74, 0, 0, 0};
    BOOST_CHECK_EQUAL( t, timegm( &tm ) );
}

BOOST_AUTO_TEST_CASE( ConvertDateToString )
{
    std::tm tm = {10, 22, 5, 20, 5, 74, 0, 0, 0};
    auto t = timegm( &tm );
    auto dateType = EcorePackage::eInstance()->getEDate();
    auto dateStr = EcoreFactory::eInstance()->convertToString( dateType, t );
    BOOST_CHECK_EQUAL( dateStr, "1974-06-20T05:22:10.000" );
}

BOOST_AUTO_TEST_SUITE_END()
