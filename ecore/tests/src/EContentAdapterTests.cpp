#include <boost/test/unit_test.hpp>

#include "ecore/EContentAdapter.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include "ecore/tests/MockList.hpp"
#include "ecore/tests/MockNotification.hpp"
#include "ecore/tests/MockObject.hpp"

#include <chrono>
#include <random>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    std::default_random_engine generator;
    constexpr int nb_children = 10;
    constexpr int nb_objects = 2;
} // namespace

BOOST_AUTO_TEST_SUITE( EContentAdapterTests )

BOOST_AUTO_TEST_CASE( SetTarget )
{
    EContentAdapter adapter;

    // create mock children for a mock object
    
    std::uniform_int_distribution<int> d( 0, nb_children );
    auto nb = d( generator );
    std::vector<std::shared_ptr<EObject>> vchildren;
    for( int i = 0; i < nb; ++i )
    {
        auto mockObject = std::make_shared<MockObject>();
        auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
        MOCK_EXPECT( mockObject->eAdapters ).returns( *mockAdapters );
        MOCK_EXPECT( mockAdapters->contains ).once().with( &adapter ).returns( false );
        MOCK_EXPECT( mockAdapters->add ).once().with( &adapter ).returns( true );
        MOCK_EXPECT( mockAdapters->removeObject ).once().with( &adapter ).returns( true );
        vchildren.push_back( mockObject );
    }
    auto mockChildren = std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>( std::move( vchildren ) );

    // create mock object
    auto mockObject = std::make_shared<MockObject>();
    auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
    MOCK_EXPECT( mockObject->eContents ).returns( mockChildren->asEListOf<std::shared_ptr<EObject>>() );
    MOCK_EXPECT( mockObject->eAdapters ).returns( *mockAdapters );

    // set adapter target -> this should recursively register adapter on all object children
    adapter.setTarget( mockObject );

    // unset adapter target -> this should recursively unregister adapter on all object children
    adapter.setTarget( nullptr );
}

BOOST_AUTO_TEST_CASE( NotifyChanged_Resolve )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockOldObject = std::make_shared<MockObject>();
    auto mockOldAdapters = std::make_shared<MockList<EAdapter*>>();

    MOCK_EXPECT( mockOldObject->eAdapters ).returns( *mockOldAdapters );
    MOCK_EXPECT( mockOldAdapters->contains ).with( &adapter ).returns( false );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::RESOLVE );
    MOCK_EXPECT( notification->getOldValue ).returns( Any( std::static_pointer_cast<EObject>( mockOldObject ) ) );

    adapter.notifyChanged( notification );
}

BOOST_AUTO_TEST_CASE( NotifyChanged_Resolve_Contains )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockOldObject = std::make_shared<MockObject>();
    auto mockOldAdapters = std::make_shared<MockList<EAdapter*>>();
    auto mockNewObject = std::make_shared<MockObject>();
    auto mockNewAdapters = std::make_shared<MockList<EAdapter*>>();

    MOCK_EXPECT( mockOldObject->eAdapters ).returns( *mockOldAdapters );
    MOCK_EXPECT( mockOldAdapters->contains ).with( &adapter ).returns( true );
    MOCK_EXPECT( mockOldAdapters->removeObject ).with( &adapter ).returns( true );

    MOCK_EXPECT( mockNewObject->eAdapters ).returns( *mockNewAdapters );
    MOCK_EXPECT( mockNewAdapters->contains ).with( &adapter ).returns( false );
    MOCK_EXPECT( mockNewAdapters->add ).with( &adapter ).returns( true );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::RESOLVE );
    MOCK_EXPECT( notification->getOldValue ).returns( Any( std::static_pointer_cast<EObject>( mockOldObject ) ) );
    MOCK_EXPECT( notification->getNewValue ).returns( Any( std::static_pointer_cast<EObject>( mockNewObject ) ) );

    adapter.notifyChanged( notification );
}


BOOST_AUTO_TEST_CASE( NotifyChanged_UnSet )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockOldObject = std::make_shared<MockObject>();
    auto mockOldAdapters = std::make_shared<MockList<EAdapter*>>();
    auto mockNewObject = std::make_shared<MockObject>();
    auto mockNewAdapters = std::make_shared<MockList<EAdapter*>>();

    MOCK_EXPECT( mockOldObject->eAdapters ).returns( *mockOldAdapters );
    MOCK_EXPECT( mockOldAdapters->contains ).with( &adapter ).returns( true );
    MOCK_EXPECT( mockOldAdapters->removeObject ).with( &adapter ).returns( true );

    MOCK_EXPECT( mockNewObject->eAdapters ).returns( *mockNewAdapters );
    MOCK_EXPECT( mockNewAdapters->contains ).with( &adapter ).returns( false );
    MOCK_EXPECT( mockNewAdapters->add ).with( &adapter ).returns( true );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::SET );
    MOCK_EXPECT( notification->getOldValue ).returns( Any( std::static_pointer_cast<EObject>( mockOldObject ) ) );
    MOCK_EXPECT( notification->getNewValue ).returns( Any( std::static_pointer_cast<EObject>( mockNewObject ) ) );

    adapter.notifyChanged( notification );
}

BOOST_AUTO_TEST_CASE( NotifyChanged_Set )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockOldObject = std::make_shared<MockObject>();
    auto mockOldAdapters = std::make_shared<MockList<EAdapter*>>();
    auto mockNewObject = std::make_shared<MockObject>();
    auto mockNewAdapters = std::make_shared<MockList<EAdapter*>>();

    MOCK_EXPECT( mockOldObject->eAdapters ).returns( *mockOldAdapters );
    MOCK_EXPECT( mockOldAdapters->contains ).with( &adapter ).returns( true );
    MOCK_EXPECT( mockOldAdapters->removeObject ).with( &adapter ).returns( true );

    MOCK_EXPECT( mockNewObject->eAdapters ).returns( *mockNewAdapters );
    MOCK_EXPECT( mockNewAdapters->contains ).with( &adapter ).returns( false );
    MOCK_EXPECT( mockNewAdapters->add ).with( &adapter ).returns( true );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::SET );
    MOCK_EXPECT( notification->getOldValue ).returns( Any( std::static_pointer_cast<EObject>( mockOldObject ) ) );
    MOCK_EXPECT( notification->getNewValue ).returns( Any( std::static_pointer_cast<EObject>( mockNewObject ) ) );

    adapter.notifyChanged( notification );
}


BOOST_AUTO_TEST_CASE( NotifyChanged_Add )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockNewObject = std::make_shared<MockObject>();
    auto mockNewAdapters = std::make_shared<MockList<EAdapter*>>();

    MOCK_EXPECT( mockNewObject->eAdapters ).returns( *mockNewAdapters );
    MOCK_EXPECT( mockNewAdapters->contains ).with( &adapter ).returns( false );
    MOCK_EXPECT( mockNewAdapters->add ).with( &adapter ).returns( true );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::ADD );
    MOCK_EXPECT( notification->getNewValue ).returns( Any( std::static_pointer_cast<EObject>( mockNewObject ) ) );

    adapter.notifyChanged( notification );
}


BOOST_AUTO_TEST_CASE( NotifyChanged_AddMany )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    
    std::vector<Any> mockObjects;
    std::uniform_int_distribution<int> d( 1, nb_objects );
    auto nb = d( generator );
    for( int i = 0; i < nb; ++i )
    {
        auto mockObject = std::make_shared<MockObject>();
        auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
        MOCK_EXPECT( mockObject->eAdapters ).returns( *mockAdapters );
        MOCK_EXPECT( mockAdapters->contains ).with( &adapter ).returns( false );
        MOCK_EXPECT( mockAdapters->add ).with( &adapter ).returns( true );
        mockObjects.push_back( std::static_pointer_cast<EObject>(mockObject) );
    }

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::ADD_MANY );
    MOCK_EXPECT( notification->getNewValue ).returns( Any(mockObjects) );

    adapter.notifyChanged( notification );
}

BOOST_AUTO_TEST_CASE( NotifyChanged_Remove )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();
    auto mockOldObject = std::make_shared<MockObject>();
    auto mockOldAdapters = std::make_shared<MockList<EAdapter*>>();
   
    MOCK_EXPECT( mockOldObject->eAdapters ).returns( *mockOldAdapters );
    MOCK_EXPECT( mockOldAdapters->contains ).with( &adapter ).returns( true );
    MOCK_EXPECT( mockOldAdapters->removeObject ).with( &adapter ).returns( true );

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::REMOVE );
    MOCK_EXPECT( notification->getOldValue ).returns( Any( std::static_pointer_cast<EObject>( mockOldObject ) ) );

    adapter.notifyChanged( notification );
}

BOOST_AUTO_TEST_CASE( NotifyChanged_RemoveMany )
{
    EContentAdapter adapter;
    auto notification = std::make_shared<MockNotification>();

    std::vector<Any> mockObjects;
    std::uniform_int_distribution<int> d( 1, nb_objects );
    auto nb = d( generator );
    for( int i = 0; i < nb; ++i )
    {
        auto mockObject = std::make_shared<MockObject>();
        auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
        MOCK_EXPECT( mockObject->eAdapters ).returns( *mockAdapters );
        MOCK_EXPECT( mockAdapters->contains ).with( &adapter ).returns( false );
        MOCK_EXPECT( mockAdapters->removeObject ).with( &adapter ).returns( true );
        mockObjects.push_back( std::static_pointer_cast<EObject>(mockObject) );
    }

    MOCK_EXPECT( notification->getEventType ).returns( ENotification::REMOVE_MANY );
    MOCK_EXPECT( notification->getOldValue ).returns( Any(mockObjects) );

    adapter.notifyChanged( notification );
}

BOOST_AUTO_TEST_SUITE_END()