#include <boost/test/unit_test.hpp>

#include "ecore/impl/AbstractResource.hpp"
#include "ecore/impl/ResourceSet.hpp"
#include "ecore/tests/MockEObject.hpp"
#include "ecore/tests/MockEResource.hpp"
#include "ecore/tests/MockEResourceFactory.hpp"
#include "ecore/tests/MockEResourceFactoryRegistry.hpp"

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
} // namespace

BOOST_AUTO_TEST_SUITE( ResourceSetTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
}

BOOST_AUTO_TEST_CASE( Resources_WithMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<MockEResource>();
    auto resources = resourceSet->getResources();
    resources->add( resource );

    BOOST_CHECK( resources->get( 0 ) == resource );
}

BOOST_AUTO_TEST_CASE( Resources_NoMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<Resource>();
    resource->setThisPtr( resource );
    auto resources = resourceSet->getResources();
    resources->add( resource );
    BOOST_CHECK_EQUAL( resource->getResourceSet(), resourceSet );

    resources->remove( resource );
    BOOST_CHECK_EQUAL( resource->getResourceSet(), nullptr );
}

BOOST_AUTO_TEST_CASE( CreateResource )
{
    URI uri( "test://file.t" );
    auto mockResourceFactoryRegistry = std::make_shared<MockEResourceFactoryRegistry>();
    auto mockResourceFactory = std::make_shared<MockEResourceFactory>();
    auto mockResource = std::make_shared<MockEResource>();

    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
    resourceSet->setResourceFactoryRegistry( mockResourceFactoryRegistry );

    MOCK_EXPECT( mockResourceFactoryRegistry->getFactory ).with( uri ).returns( mockResourceFactory );
    MOCK_EXPECT( mockResourceFactory->createResource ).with( uri ).returns( mockResource );
    BOOST_CHECK_EQUAL( resourceSet->createResource( uri ), mockResource );
    BOOST_CHECK( resourceSet->getResources()->contains( mockResource ) );
}

BOOST_AUTO_TEST_CASE( GetResource )
{
    URI uri( "test://file.t" );
    auto mockResourceFactoryRegistry = std::make_shared<MockEResourceFactoryRegistry>();
    auto mockResourceFactory = std::make_shared<MockEResourceFactory>();
    auto mockResource = std::make_shared<MockEResource>();

    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
    resourceSet->setResourceFactoryRegistry( mockResourceFactoryRegistry );

    MOCK_EXPECT( mockResourceFactoryRegistry->getFactory ).with( uri ).returns( mockResourceFactory );
    MOCK_EXPECT( mockResourceFactory->createResource ).with( uri ).returns( mockResource );
    MOCK_EXPECT( mockResource->loadSimple ).once();

    BOOST_CHECK_EQUAL( resourceSet->getResource( uri, true ), mockResource );
}

BOOST_AUTO_TEST_CASE( GetEObject )
{
    URI uri( "test://file.t#//@first/second" );
    auto mockResourceFactoryRegistry = std::make_shared<MockEResourceFactoryRegistry>();
    auto mockResourceFactory = std::make_shared<MockEResourceFactory>();
    auto mockResource = std::make_shared<MockEResource>();
    auto mockObject = std::make_shared<MockEObject>();

    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
    resourceSet->setResourceFactoryRegistry( mockResourceFactoryRegistry );

    MOCK_EXPECT( mockResourceFactoryRegistry->getFactory ).with( uri.trimFragment() ).returns( mockResourceFactory );
    MOCK_EXPECT( mockResourceFactory->createResource ).with( uri.trimFragment() ).returns( mockResource );
    MOCK_EXPECT( mockResource->loadSimple ).once();
    MOCK_EXPECT( mockResource->getEObject ).with( uri.getFragment() ).returns( mockObject );

    BOOST_CHECK_EQUAL( resourceSet->getEObject( uri, true ), mockObject );
}

BOOST_AUTO_TEST_SUITE_END()
