#include <boost/test/unit_test.hpp>

#include "ecore/Any.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotificationChain.hpp"
#include "ecore/Stream.hpp"
#include "ecore/URI.hpp"
#include "ecore/impl/AbstractResource.hpp"
#include "ecore/tests/MockEAdapter.hpp"
#include "ecore/tests/MockEObject.hpp"
#include "ecore/tests/MockEObjectInternal.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class Resource : public AbstractResource
    {
        // Inherited via AbstractResource
        virtual void doLoad( std::istream& is ) override
        {
            throw std::exception( "NotImplementedException " );
        }
        virtual void doSave( std::ostream& os ) override
        {
            throw std::exception( "NotImplementedException " );
        }
    };

    class NotificationsFixture
    {
    public:
        NotificationsFixture()
            : resource( std::make_shared<Resource>() )
            , eAdapter( std::make_unique<MockEAdapter>() )
        {
            BOOST_CHECK( resource );
            resource->setThisPtr( resource );

            MOCK_EXPECT( eAdapter->setTarget ).with( resource ).once();
            resource->eAdapters().add( eAdapter.get() );
        }

        ~NotificationsFixture()
        {
            MOCK_EXPECT( eAdapter->unsetTarget ).with( resource ).once();
            resource->eSetDeliver( false );
            resource->eAdapters().remove( eAdapter.get() );
        }

    protected:
        std::shared_ptr<Resource> resource;
        std::unique_ptr<MockEAdapter> eAdapter;
    };
} // namespace

BOOST_AUTO_TEST_SUITE( ResourceTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto resource = std::make_shared<Resource>();
    resource->setThisPtr( resource );
}

BOOST_AUTO_TEST_CASE( Attribute_URI )
{
    auto resource = std::make_shared<Resource>();
    auto uri = URI( "http://host:10020" );
    resource->setURI( uri );
    BOOST_CHECK_EQUAL( resource->getURI(), uri );
}

BOOST_FIXTURE_TEST_CASE( Attribute_URI_Notifications, NotificationsFixture )
{
    auto uri = URI( "http://host:10020" );

    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == resource && n->getFeatureID() == EResource::RESOURCE__URI && n->getOldValue() == URI()
                   && n->getNewValue() == uri && n->getPosition() == -1;
        } )
        .once();

    resource->setURI( uri );
    BOOST_CHECK_EQUAL( resource->getURI(), uri );
}

BOOST_AUTO_TEST_CASE( Contents )
{
    auto resource = std::make_shared<Resource>();
    resource->setThisPtr( resource );

    auto mockObject = std::make_shared<MockEObject>();
    auto mockInternal = std::make_shared<MockEObjectInternal>();
    MOCK_EXPECT( mockObject->getInternal ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eSetInternalResource )
        .once()
        .with( resource, std::shared_ptr<ENotificationChain>() )
        .returns( std::shared_ptr<ENotificationChain>() );

    auto contents = resource->getContents();
    contents->add( mockObject );
}

BOOST_AUTO_TEST_SUITE_END()