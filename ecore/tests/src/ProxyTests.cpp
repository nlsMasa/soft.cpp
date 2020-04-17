#include <boost/test/unit_test.hpp>

#include "ecore\tests\MockObject.hpp"
#include "ecore\tests\MockObjectInternal.hpp"
#include "ecore\impl\Proxy.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE(ProxyTests)

BOOST_AUTO_TEST_CASE(SetGeT_NoProxy)
{
    Proxy<std::shared_ptr<EObject>> proxy;
    auto mockObject = std::make_shared<MockObject>();
    MOCK_EXPECT(mockObject->eIsProxy).returns(false);
    proxy.set(mockObject);
    BOOST_CHECK_EQUAL(proxy.get(), mockObject);
}

BOOST_AUTO_TEST_CASE(SetGeT_WithProxy)
{
    Proxy<std::shared_ptr<EObject>> proxy;
    auto mockObject = std::make_shared<MockObject>();
    MOCK_EXPECT( mockObject->eIsProxy ).returns( true );
    proxy.set( mockObject );
    BOOST_CHECK_EQUAL( proxy.get(), mockObject );
}



BOOST_AUTO_TEST_SUITE_END()
