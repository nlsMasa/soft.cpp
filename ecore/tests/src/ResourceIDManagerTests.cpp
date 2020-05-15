#include <boost/test/unit_test.hpp>

#include "ecore/impl/ImmutableArrayEList.hpp"
#include "ecore/impl/ResourceIDManager.hpp"
#include "ecore/tests/MockEAttribute.hpp"
#include "ecore/tests/MockEClass.hpp"
#include "ecore/tests/MockEDataType.hpp"
#include "ecore/tests/MockEFactory.hpp"
#include "ecore/tests/MockEObject.hpp"
#include "ecore/tests/MockEPackage.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    std::shared_ptr<MockEObject> createMockEObject( const std::string& id )
    {
        auto mockObject = std::make_shared<MockEObject>();
        auto mockClass = std::make_shared<MockEClass>();
        auto mockAttribute = std::make_shared<MockEAttribute>();

        MOCK_EXPECT( mockObject->eClass ).returns( mockClass );
        MOCK_EXPECT( mockClass->getEIDAttribute ).returns( mockAttribute );
        if( id.empty() )
        {
            MOCK_EXPECT( mockObject->eIsSet ).with( mockAttribute ).returns( false );
        }
        else
        {
            MOCK_EXPECT( mockObject->eIsSet ).with( mockAttribute ).returns( true );
            auto mockDataType = std::make_shared<MockEDataType>();
            auto mockPackage = std::make_shared<MockEPackage>();
            auto mockFactory = std::make_shared<MockEFactory>();
            MOCK_EXPECT( mockObject->eGet_EStructuralFeature ).with( mockAttribute ).returns( Any( id ) );
            MOCK_EXPECT( mockAttribute->getEAttributeType ).returns( mockDataType );
            MOCK_EXPECT( mockDataType->getEPackage ).returns( mockPackage );
            MOCK_EXPECT( mockPackage->getEFactoryInstance ).returns( mockFactory );
            MOCK_EXPECT( mockFactory->convertToString ).with( mockDataType, Any( id ) ).returns( id );
        }
        return mockObject;
    }
} // namespace

BOOST_AUTO_TEST_SUITE( ResourceIDManagerTests )

BOOST_AUTO_TEST_CASE( RegisterNoID )
{
    auto m = std::make_unique<ResourceIDManager>();

    auto mockObject = createMockEObject( "" );
    auto mockChild1 = createMockEObject( "" );
    auto mockChild2 = createMockEObject( "" );
    auto mockChildren = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>(
        std::initializer_list<std::shared_ptr<EObject>>{mockChild1, mockChild2} );
    auto emptyList = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>();
    MOCK_EXPECT( mockObject->eContents ).returns( mockChildren );
    MOCK_EXPECT( mockChild1->eContents ).returns( emptyList );
    MOCK_EXPECT( mockChild2->eContents ).returns( emptyList );

    m->registerObject( mockObject );

    BOOST_CHECK_EQUAL( m->getID( mockObject ), "" );
    BOOST_CHECK_EQUAL( m->getID( mockChild1 ), "" );
    BOOST_CHECK_EQUAL( m->getID( mockChild2 ), "" );
}

BOOST_AUTO_TEST_CASE( RegisterWithID )
{
    auto m = std::make_unique<ResourceIDManager>();

    auto mockObject = createMockEObject( "id" );
    auto mockChild1 = createMockEObject( "id1" );
    auto mockChild2 = createMockEObject( "id2" );
    auto mockChildren = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>(
        std::initializer_list<std::shared_ptr<EObject>>{mockChild1, mockChild2} );
    auto emptyList = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>();
    MOCK_EXPECT( mockObject->eContents ).returns( mockChildren );
    MOCK_EXPECT( mockChild1->eContents ).returns( emptyList );
    MOCK_EXPECT( mockChild2->eContents ).returns( emptyList );

    m->registerObject( mockObject );

    BOOST_CHECK_EQUAL( m->getID( mockObject ), "id" );
    BOOST_CHECK_EQUAL( m->getID( mockChild1 ), "id1" );
    BOOST_CHECK_EQUAL( m->getID( mockChild2 ), "id2" );

    BOOST_CHECK_EQUAL( m->getEObject( "id" ), mockObject );
    BOOST_CHECK_EQUAL( m->getEObject( "id1" ), mockChild1 );
    BOOST_CHECK_EQUAL( m->getEObject( "id2" ), mockChild2 );
}

BOOST_AUTO_TEST_CASE( UnRegisterWithID )
{
    auto m = std::make_unique<ResourceIDManager>();

    auto mockObject = createMockEObject( "id" );
    auto mockChild1 = createMockEObject( "id1" );
    auto mockChild2 = createMockEObject( "id2" );
    auto mockChildren = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>(
        std::initializer_list<std::shared_ptr<EObject>>{mockChild1, mockChild2} );
    auto emptyList = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>();
    MOCK_EXPECT( mockObject->eContents ).returns( mockChildren );
    MOCK_EXPECT( mockChild1->eContents ).returns( emptyList );
    MOCK_EXPECT( mockChild2->eContents ).returns( emptyList );

    m->registerObject( mockObject );

    BOOST_CHECK_EQUAL( m->getID( mockObject ), "id" );
    BOOST_CHECK_EQUAL( m->getID( mockChild1 ), "id1" );
    BOOST_CHECK_EQUAL( m->getID( mockChild2 ), "id2" );

    BOOST_CHECK_EQUAL( m->getEObject( "id" ), mockObject );
    BOOST_CHECK_EQUAL( m->getEObject( "id1" ), mockChild1 );
    BOOST_CHECK_EQUAL( m->getEObject( "id2" ), mockChild2 );

    m->unregisterObject( mockObject );

    BOOST_CHECK_EQUAL( m->getID( mockObject ), "" );
    BOOST_CHECK_EQUAL( m->getID( mockChild1 ), "" );
    BOOST_CHECK_EQUAL( m->getID( mockChild2 ), "" );

    BOOST_CHECK_EQUAL( m->getEObject( "id" ), nullptr );
    BOOST_CHECK_EQUAL( m->getEObject( "id1" ), nullptr );
    BOOST_CHECK_EQUAL( m->getEObject( "id2" ), nullptr );



}

BOOST_AUTO_TEST_SUITE_END()