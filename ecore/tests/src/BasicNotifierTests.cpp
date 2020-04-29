#include <boost/test/unit_test.hpp>

#include "ecore/ENotifier.hpp"
#include "ecore/impl/BasicNotifier.hpp"
#include "ecore/tests/MockEAdapter.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

using Notifier = BasicNotifier<ENotifier>;

BOOST_AUTO_TEST_SUITE( BasicNotifierTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    Notifier notifier;
}

BOOST_AUTO_TEST_CASE( NoTarget )
{
    auto notifier = std::make_shared<Notifier>();
    auto adapter = std::make_unique<MockEAdapter>();
    MOCK_EXPECT( adapter->setTarget ).with( nullptr ).once();
    notifier->eAdapters().add( adapter.get() );
}

BOOST_AUTO_TEST_CASE( Target )
{
    auto notifier = std::make_shared<Notifier>();
    notifier->setThisPtr( notifier );
    auto adapter = std::make_unique<MockEAdapter>();

    MOCK_EXPECT( adapter->setTarget ).with( notifier ).once();
    notifier->eAdapters().add( adapter.get() );

    MOCK_EXPECT( adapter->notifyChanged )
        .with( [&]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == notifier && n->getEventType() == ENotification::REMOVING_ADAPTER && n->getFeatureID() == -1
                   && n->getOldValue() == static_cast<EAdapter*>( adapter.get() ) && n->getNewValue() == NO_VALUE && n->getPosition() == 0;
        } )
        .once();
    MOCK_EXPECT( adapter->unsetTarget ).with( notifier ).once();
    notifier->eAdapters().remove( adapter.get() );
}

BOOST_AUTO_TEST_CASE( Target_NoDeliver )
{
    auto notifier = std::make_shared<Notifier>();
    notifier->eSetDeliver( false );
    notifier->setThisPtr( notifier );
    auto adapter = std::make_unique<MockEAdapter>();

    MOCK_EXPECT( adapter->setTarget ).with( notifier ).once();
    notifier->eAdapters().add( adapter.get() );

    MOCK_EXPECT( adapter->unsetTarget ).with( notifier ).once();
    notifier->eAdapters().remove( adapter.get() );
}

BOOST_AUTO_TEST_SUITE_END()
