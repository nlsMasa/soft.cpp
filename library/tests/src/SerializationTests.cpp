#include <boost/test/unit_test.hpp>

#include "LibraryFactory.hpp"
#include "library/LibraryPackage.hpp"

#include "ecore/EResource.hpp"
#include "ecore/EDiagnostic.hpp"
#include "ecore/EResourceFactory.hpp"
#include "ecore/EResourceFactoryRegistry.hpp"
#include "ecore/EPackageRegistry.hpp"
#include "ecore/URI.hpp"

#include <fstream>

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

BOOST_AUTO_TEST_SUITE( SerializationTests )

BOOST_AUTO_TEST_CASE( GenerateModel, *boost::unit_test::disabled() )
{
    auto l = LibraryFactory::createLibrary( nb_employees, nb_writers, nb_books, nb_borrowers );
    auto fileURI = URI("file:D:/dev/mylib.xml");
    auto resourceFactory = EResourceFactoryRegistry::getInstance()->getFactory( fileURI );
    BOOST_CHECK( resourceFactory );
    auto resource = resourceFactory->createResource( fileURI );
    BOOST_CHECK( resource );
    resource->getContents()->add( l );
    resource->save();
}

namespace
{
    std::string replaceAll( std::string str, const std::string& from, const std::string& to )
    {
        size_t start_pos = 0;
        while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
        {
            str.replace( start_pos, from.length(), to );
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
} // namespace

BOOST_AUTO_TEST_CASE( LoadSave )
{
    EPackageRegistry::getInstance()->registerPackage( LibraryPackage::eInstance() );

    auto fileURI = URI( "data/library.xml" );
    auto resourceFactory = EResourceFactoryRegistry::getInstance()->getFactory( fileURI );
    BOOST_CHECK( resourceFactory );
    auto resource = resourceFactory->createResource( fileURI );
    BOOST_CHECK( resource );
    resource->load();

    BOOST_CHECK( resource->isLoaded() );
    BOOST_CHECK( resource->getWarnings()->empty() );
    BOOST_CHECK( resource->getErrors()->empty() );

    std::ifstream ifs( "data/library.xml" );
    std::string expected( ( std::istreambuf_iterator<char>( ifs ) ), std::istreambuf_iterator<char>() );

    std::stringstream ss;
    resource->save( ss );

    BOOST_CHECK_EQUAL( replaceAll( ss.str(), "\r\n", "\n" ), replaceAll( expected, "\r\n", "\n" ) );
}

BOOST_AUTO_TEST_SUITE_END()