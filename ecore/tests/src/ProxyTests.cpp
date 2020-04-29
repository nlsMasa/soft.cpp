#include <boost/test/unit_test.hpp>

#include "ecore\impl\Proxy.hpp"
#include "ecore\tests\MockEObjectInternal.hpp"
#include "ecore\tests\mockEObject.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( ProxyTests )

BOOST_AUTO_TEST_CASE( SetGeT_NoProxy )
{
    Proxy<std::shared_ptr<EObject>> proxy;
    auto mockObject = std::make_shared<MockEObject>();
    MOCK_EXPECT( mockObject->eIsProxy ).returns( false );
    proxy.set( mockObject );
    BOOST_CHECK_EQUAL( proxy.get(), mockObject );
}

BOOST_AUTO_TEST_CASE( SetGeT_WithProxy )
{
    Proxy<std::shared_ptr<EObject>> proxy;
    auto mockObject = std::make_shared<MockEObject>();
    MOCK_EXPECT( mockObject->eIsProxy ).returns( true );
    proxy.set( mockObject );
    BOOST_CHECK_EQUAL( proxy.get(), mockObject );
}

BOOST_AUTO_TEST_SUITE_END()
