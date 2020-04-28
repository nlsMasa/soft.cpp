#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EList.hpp"
#include "ecore/EReference.hpp"
#include "ecore/Stream.hpp"
#include "library/LibraryPackage.hpp"

#include <boost/test/unit_test.hpp>

using namespace library;
using namespace ecore;

namespace std
{
    template <typename T>
    ostream& operator<<( ostream& os, const std::vector<T>& v )
    {
        return print_container( os, v );
    }

    template <typename T>
    bool operator==( const EList<T>& lhs, const std::vector<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }
} // namespace std

BOOST_AUTO_TEST_SUITE( ModelTests )

BOOST_AUTO_TEST_CASE( LendableCopiesContainer )
{
    auto p = LibraryPackage::eInstance();
    auto a = p->getLendable_Copies();
    BOOST_CHECK_EQUAL( std::static_pointer_cast<EObject>( p->getLendable() ), a->eContainer() );
}

BOOST_AUTO_TEST_CASE( LibraryContainments )
{
    auto p = LibraryPackage::eInstance();
    auto c = p->getLibrary();
    auto con = c->getEContainments();
    BOOST_CHECK_EQUAL( *con,
                       std::vector<std::shared_ptr<EReference>>( {p->getLibrary_Writers(),
                                                                  p->getLibrary_Employees(),
                                                                  p->getLibrary_Borrowers(),
                                                                  p->getLibrary_Stock(),
                                                                  p->getLibrary_Books(),
                                                                  p->getLibrary_Branches()} ) );
}

BOOST_AUTO_TEST_SUITE_END()