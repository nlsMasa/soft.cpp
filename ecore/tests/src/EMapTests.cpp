#include <boost/test/unit_test.hpp>

#include "ecore/EMap.hpp"
#include "ecore/EObject.hpp"
#include "ecore/tests/MockEMap.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( EMapTests )

BOOST_AUTO_TEST_CASE( PrimitiveMap )
{
    //auto map = std::make_shared<MockEMap<int, int>>();
    //BOOST_CHECK( map != nullptr );
}

//BOOST_AUTO_TEST_CASE( EObjectMap )
//{
//    auto map = std::make_shared<MockEMap<int, std::shared_ptr<EObject>>>();
//    MOCK_EXPECT( map->getUnResolvedMap ).returns( map );
//
//    // cast to interface & check that getUnResolved is correctly called
//    auto mapAsInterface = std::static_pointer_cast<EMap<int, std::shared_ptr<EObject>>>( map );
//    BOOST_CHECK( mapAsInterface->getUnResolvedMap() == mapAsInterface );
//}
//
//BOOST_AUTO_TEST_CASE( AnyMap )
//{
//    auto map = std::make_shared<MockEMap<int, Any>>();
//    MOCK_EXPECT( map->getUnResolvedMap ).returns( map );
//
//    // cast to interface & check that getUnResolved is correctly called
//    auto mapAsInterface = std::static_pointer_cast<EMap<int, Any>>( map );
//    BOOST_CHECK( mapAsInterface->getUnResolvedMap() == mapAsInterface );
//}

BOOST_AUTO_TEST_SUITE_END()
