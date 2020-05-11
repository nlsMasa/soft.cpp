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

BOOST_AUTO_TEST_CASE( OperatorSet )
{
    auto mockObject1 = std::make_shared<MockEObject>();
    MOCK_EXPECT( mockObject1->eIsProxy ).returns( true );

    auto mockObject2 = std::make_shared<MockEObject>();
    MOCK_EXPECT( mockObject2->eIsProxy ).returns( true );

    Proxy<std::shared_ptr<EObject>> proxy(mockObject1);
    BOOST_CHECK_EQUAL( proxy.get(), mockObject1 );
    proxy = mockObject2;
    BOOST_CHECK_EQUAL( proxy.get(), mockObject2 );
}

BOOST_AUTO_TEST_CASE( OperatorEqualProxy )
{
    Proxy<std::shared_ptr<EObject>> proxy1;
    Proxy<std::shared_ptr<EObject>> proxy2;
    BOOST_CHECK( proxy1 == proxy2 );
    
    auto mockObject = std::make_shared<MockEObject>();
    MOCK_EXPECT( mockObject->eIsProxy ).returns( true );
    proxy1.set( mockObject );
    proxy2.set( mockObject );
    BOOST_CHECK( proxy1 == proxy2 );
}

BOOST_AUTO_TEST_CASE( OperatorEqualValue )
{
    Proxy<std::shared_ptr<EObject>> proxy;
    BOOST_CHECK( proxy == std::shared_ptr<EObject>() );
    BOOST_CHECK( proxy == nullptr );
}

BOOST_AUTO_TEST_SUITE_END()
