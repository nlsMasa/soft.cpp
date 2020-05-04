#include <boost/test/unit_test.hpp>

#include "ecore/EAdapter.hpp"
#include "ecore/impl/BasicEObjectList.hpp"
#include "ecore/tests/MockEAdapter.hpp"
#include "ecore/tests/MockEClass.hpp"
#include "ecore/tests/MockEList.hpp"
#include "ecore/tests/MockEObject.hpp"
#include "ecore/tests/MockEObjectInternal.hpp"
#include "ecore/tests/MockEStructuralFeature.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class Fixture
    {
    public:
        Fixture( bool notifications = false )
            : owner( new MockEObject() )
            , object( new MockEObject() )
            , mockInternal( new MockEObjectInternal() )
        {
            MOCK_EXPECT( owner->eDeliver ).returns( notifications );
            MOCK_EXPECT( object->getInternal ).returns( *mockInternal );
        }

        std::shared_ptr<MockEObject> owner;
        std::shared_ptr<MockEObject> object;
        std::shared_ptr<MockEObjectInternal> mockInternal;
    };

    class FixtureNotifications : public Fixture
    {
    public:
        FixtureNotifications()
            : Fixture( true )
            , mockClass( new MockEClass() )
            , mockFeature( new MockEStructuralFeature() )
            , MockEAdapters( new MockEList<EAdapter*>() )

        {
            MOCK_EXPECT( mockClass->getEStructuralFeature_EInt ).with( 1 ).returns( mockFeature );
            MOCK_EXPECT( MockEAdapters->empty ).returns( false );
            MOCK_EXPECT( owner->eClass ).returns( mockClass );
            MOCK_EXPECT( owner->eAdapters ).returns( boost::ref( *MockEAdapters ) );
            MOCK_EXPECT( owner->eNotify )
                .with( [=]( const std::shared_ptr<ENotification>& n ) {
                    return n->getNotifier() == owner && n->getFeature() == mockFeature && n->getOldValue().empty()
                           && anyCast<std::shared_ptr<EObject>>( n->getNewValue() ) == object && n->getPosition() == 0;
                } )
                .once();
        }

        std::shared_ptr<MockEClass> mockClass;
        std::shared_ptr<MockEStructuralFeature> mockFeature;
        std::shared_ptr<MockEList<EAdapter*>> MockEAdapters;
    };

} // namespace

BOOST_AUTO_TEST_SUITE( BasicEObjectListTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto object = std::make_shared<MockEObject>();
    {
        BasicEObjectList<std::shared_ptr<EObject>> list( object, 0, 1 );
    }
    {
        BasicEObjectList<std::shared_ptr<EObject>, false, true, false> list( object, 0, 1 );
    }
    {
        BasicEObjectList<std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
    {
        BasicEObjectList<std::shared_ptr<EObject>, false, false, false, true, false> list( object, 0, 1 );
    }
    {
        BasicEObjectList<std::shared_ptr<EObject>, false, false, false, false, true> list( object, 0, 1 );
    }
    {
        BasicEObjectList<std::shared_ptr<EObject>, true, true, true, true, true> list( object, 0, 1 );
    }
}

BOOST_FIXTURE_TEST_CASE( Add_SimpleNoNotifications, Fixture )
{
    BasicEObjectList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_SimpleNotifications, FixtureNotifications )
{
    BasicEObjectList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseNoNotifications, Fixture )
{
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( owner, -2, nullptr ).once().returns( nullptr );

    BasicEObjectList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseNotifications, FixtureNotifications )
{
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( owner, -2, nullptr ).once().returns( nullptr );
    BasicEObjectList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseOppositeNoNotifications, Fixture )
{
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( owner, 2, nullptr ).once().returns( nullptr );

    BasicEObjectList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseOppositeNotifications, FixtureNotifications )
{
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( owner, 2, nullptr ).once().returns( nullptr );

    BasicEObjectList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Proxies, Fixture )
{
    MOCK_EXPECT( owner->getInternal ).returns( *mockInternal );

    BasicEObjectList<std::shared_ptr<EObject>, false, false, false, true> list( owner, 1, 2 );
    auto proxy = std::make_shared<MockEObject>();
    auto resolved = std::make_shared<MockEObject>();

    MOCK_EXPECT( proxy->eIsProxy ).returns( true );
    MOCK_EXPECT( resolved->eIsProxy ).returns( false );
    BOOST_CHECK( list.add( proxy ) );
    MOCK_EXPECT( mockInternal->eResolveProxy ).once().with( proxy ).returns( proxy );
    BOOST_CHECK_EQUAL( list.get( 0 ), proxy );
    MOCK_EXPECT( mockInternal->eResolveProxy ).once().with( proxy ).returns( resolved );
    BOOST_CHECK_EQUAL( list.get( 0 ), resolved );
}

BOOST_FIXTURE_TEST_CASE( Unset, Fixture )
{
    BasicEObjectList<std::shared_ptr<EObject>, false, false, false, false, true> list( owner, 1, 2 );
    BOOST_CHECK( !list.isSet() );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK( list.isSet() );
    list.unset();
    BOOST_CHECK( !list.isSet() );
}

BOOST_AUTO_TEST_SUITE_END()
