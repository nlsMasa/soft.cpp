#include <boost/test/unit_test.hpp>

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/tests/MockEList.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( EListTests )

BOOST_AUTO_TEST_CASE( PrimitiveList)
{
    auto list = std::make_shared<MockEList<int>>();
    BOOST_CHECK( list );
}

BOOST_AUTO_TEST_CASE( EObjectList )
{
    auto list = std::make_shared<MockEList<std::shared_ptr<EObject>>>();
    MOCK_EXPECT( list->getUnResolvedList ).returns( list );
    BOOST_CHECK( list->getUnResolvedList() == std::static_pointer_cast<EList<std::shared_ptr<EObject>>>(list) );
}


BOOST_AUTO_TEST_SUITE_END()