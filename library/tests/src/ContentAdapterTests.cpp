#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

#include "ecore/EContentAdapter.hpp"
#include "ecore/ENotification.hpp"
#include "library/Employee.hpp"
#include "library/LibraryFactory.hpp"
#include "library/LibraryPackage.hpp"
#include "library/tests/LibraryFactory.hpp"

using namespace ecore;
using namespace library;
using namespace library::tests;

namespace
{
    class MockContentAdapter : public EContentAdapter
    {
    public:
        virtual void notifyChanged( const std::shared_ptr<ENotification>& notification )
        {
            changed( notification );
            EContentAdapter::notifyChanged( notification );
        }

        MOCK_FUNCTOR( changed, void( const std::shared_ptr<ENotification>& ) );
    };
} // namespace

BOOST_AUTO_TEST_SUITE( ContentAdapterTests )

BOOST_AUTO_TEST_CASE( TopChanged )
{
    auto l = library::tests::LibraryFactory::createLibrary( 2, 5, 20, 5 );
    auto e = library::LibraryFactory::eInstance()->createEmployee();

    MockContentAdapter mockAdapter;
    l->eAdapters().add( &mockAdapter );

    MOCK_EXPECT( mockAdapter.changed ).once().with( [=]( const std::shared_ptr<ENotification>& n ) {
        return n->getNotifier() == l && n->getFeatureID() == LibraryPackage::LIBRARY__EMPLOYEES && n->getOldValue() == NO_VALUE
               && n->getNewValue() == std::static_pointer_cast<EObject>( e ) && n->getPosition() == 2;
    } );
    l->getEmployees()->add( e );
}

BOOST_AUTO_TEST_CASE( DeepChanged )
{
    auto l = library::tests::LibraryFactory::createLibrary( 2, 5, 20, 5 );
    auto e = l->getEmployees()->get( 0 );

    // register adapter on top library
    MockContentAdapter mockAdapter;
    l->eAdapters().add( &mockAdapter );

    // modify a property of an element in library
    // check that event is raised in the top adapter
    MOCK_EXPECT( mockAdapter.changed ).with( [=]( const std::shared_ptr<ENotification>& n ) {
        return n->getNotifier() == e && n->getFeatureID() == LibraryPackage::EMPLOYEE__FIRST_NAME
               && n->getOldValue() == std::string( "First Name 0" ) && n->getNewValue() == std::string( "test" ) && n->getPosition() == -1;
    } );
    e->setFirstName( "test" );

    // remove top adapter - check that it is recursively removed
    MOCK_EXPECT( mockAdapter.changed ).with( [=]( const std::shared_ptr<ENotification>& n ) {
        return n->getEventType() == ENotification::REMOVING_ADAPTER;
    } );
    l->eAdapters().remove( &mockAdapter );

    // check that no event is raised
    e->setFirstName( "test2" );
}

BOOST_AUTO_TEST_SUITE_END()
