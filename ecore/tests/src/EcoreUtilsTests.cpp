#include <boost/test/unit_test.hpp>

#include "ecore/AnyCast.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcoreUtils.hpp"
#include "ecore/Stream.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EList.hpp"
#include "ecore/impl/ImmutableArrayEList.hpp"
#include "ecore/tests/MockEObject.hpp"
#include "ecore/tests/MockEObjectInternal.hpp"
#include "ecore/tests/MockEResource.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( EcoreUtilsTests )

BOOST_AUTO_TEST_CASE( getID )
{
}

BOOST_AUTO_TEST_CASE( getURI )
{
    URI uri( "test://file" );
    auto mockObject = std::make_shared<MockEObject>();
    auto mockInternal = std::make_shared<MockEObjectInternal>();

    MOCK_EXPECT( mockObject->eIsProxy ).returns( true );
    MOCK_EXPECT( mockObject->getInternal ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eProxyURI ).returns( uri );
    BOOST_CHECK_EQUAL( EcoreUtils::getURI( mockObject ), uri );
}

BOOST_AUTO_TEST_CASE( isAssignableFrom )
{
    auto p = EcorePackage::eInstance();
    BOOST_CHECK( EcoreUtils::isAssignableFrom( p->getENamedElement(), p->getEClass() ) );
}

BOOST_AUTO_TEST_CASE( Copy_Attributes )
{
    // create a package with a class and 2 attributes
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto eFactory = ecoreFactory->createEFactory();
    auto eClass = ecoreFactory->createEClass();
    ePackage->getEClassifiers()->add( eClass );
    ePackage->setEFactoryInstance( eFactory );
    auto eAttribute1 = ecoreFactory->createEAttribute();
    eAttribute1->setName( "attribute1" );
    eAttribute1->setEType( ecorePackage->getEString() );
    auto eAttribute2 = ecoreFactory->createEAttribute();
    eAttribute2->setName( "attribute2" );
    eAttribute2->setEType( ecorePackage->getEInt() );
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );

    // create a model from this package
    auto eObject = eFactory->create( eClass );
    eObject->eSet( eAttribute1, std::string( "attribute1" ) );
    eObject->eSet( eAttribute2, 1 );

    // check that deep copy is deep equal
    auto eObjectCopy = EcoreUtils::copy( eObject );
    BOOST_CHECK( EcoreUtils::equals(eObject,eObjectCopy) );

    // modify original object & check that deep equal is not true
    eObject->eSet( eAttribute2, 2 );
    BOOST_CHECK( !EcoreUtils::equals( eObject, eObjectCopy ) );
}

BOOST_AUTO_TEST_CASE( CopyAll_Attributes )
{
    // create a package with a class and 2 attributes
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto eFactory = ecoreFactory->createEFactory();
    auto eClass = ecoreFactory->createEClass();
    ePackage->getEClassifiers()->add( eClass );
    ePackage->setEFactoryInstance( eFactory );
    auto eAttribute1 = ecoreFactory->createEAttribute();
    eAttribute1->setName( "attribute1" );
    eAttribute1->setEType( ecorePackage->getEString() );
    auto eAttribute2 = ecoreFactory->createEAttribute();
    eAttribute2->setName( "attribute2" );
    eAttribute2->setEType( ecorePackage->getEInt() );
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );

    // create a model from this package
    auto eObject1 = eFactory->create( eClass );
    eObject1->eSet( eAttribute1, std::string( "attribute1" ) );
    eObject1->eSet( eAttribute2, 1 );

    auto eObject2 = eFactory->create( eClass );
    eObject2->eSet( eAttribute1, std::string( "attribute2" ) );
    eObject2->eSet( eAttribute2, 2 );

    auto list = std::make_shared<ImmutableArrayEList<std::shared_ptr<EObject>>>( std::initializer_list<std::shared_ptr<EObject>>{eObject1, eObject2} );

    // check that deep copy is deep equal
    auto listCopy = EcoreUtils::copyAll( list );
    BOOST_CHECK( EcoreUtils::equals( list, listCopy ) ); 
}

BOOST_AUTO_TEST_CASE( Copy_Containments )
{
    // create a package with a 2 class with one referenced in the other
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto eFactory = ecoreFactory->createEFactory();
    auto eClass1 = ecoreFactory->createEClass();
    auto eClass2 = ecoreFactory->createEClass();
    ePackage->getEClassifiers()->add( eClass1 );
    ePackage->getEClassifiers()->add( eClass2 );
    ePackage->setEFactoryInstance( eFactory );

    // class2 with 2 attributes
    auto eAttribute1 = ecoreFactory->createEAttribute();
    eAttribute1->setName( "attribute1" );
    eAttribute1->setEType( ecorePackage->getEString() );
    auto eAttribute2 = ecoreFactory->createEAttribute();
    eAttribute2->setName( "attribute2" );
    eAttribute2->setEType( ecorePackage->getEInt() );
    eClass2->getEStructuralFeatures()->add( eAttribute1 );
    eClass2->getEStructuralFeatures()->add( eAttribute2 );

    // class1 with class2 as reference
    auto eReference1 = ecoreFactory->createEReference();
    eReference1->setName( "reference1" );
    eReference1->setContainment( true );
    eReference1->setEType( eClass2 );
    eClass1->getEStructuralFeatures()->add( eReference1 );

    // create a model from this package
    std::shared_ptr<EObject> eObject1 = eFactory->create( eClass1 );
    std::shared_ptr<EObject> eObject2 = eFactory->create( eClass2 );
    eObject2->eSet( eAttribute1, std::string( "attribute1" ) );
    eObject2->eSet( eAttribute2, 1 );
    eObject1->eSet( eReference1, toAny(eObject2) );
    
    // check that deep copy is deep equal
    auto eObject1Copy = EcoreUtils::copy( eObject1 );
    BOOST_CHECK( EcoreUtils::equals( eObject1, eObject1Copy ) );

    // modify original object & check that deep equal is not true
    eObject2->eSet( eAttribute2, 2 );
    BOOST_CHECK( !EcoreUtils::equals( eObject1, eObject1Copy ) );
}

BOOST_AUTO_TEST_CASE( Copy_References )
{
    // create a package with a 2 class with one referenced in the other
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto eFactory = ecoreFactory->createEFactory();
    auto eClass1 = ecoreFactory->createEClass();
    auto eClass2 = ecoreFactory->createEClass();
    ePackage->getEClassifiers()->add( eClass1 );
    ePackage->getEClassifiers()->add( eClass2 );
    ePackage->setEFactoryInstance( eFactory );

    // class2 with 2 attributes
    auto eAttribute1 = ecoreFactory->createEAttribute();
    eAttribute1->setName( "attribute1" );
    eAttribute1->setEType( ecorePackage->getEString() );
    eClass2->getEStructuralFeatures()->add( eAttribute1 );
    
    auto eAttribute2 = ecoreFactory->createEAttribute();
    eAttribute2->setName( "attribute2" );
    eAttribute2->setEType( ecorePackage->getEInt() );
    eClass2->getEStructuralFeatures()->add( eAttribute2 );

    // class1 with class2 as reference
    auto eReference1 = ecoreFactory->createEReference();
    eReference1->setName( "reference1" );
    eReference1->setContainment( true );
    eReference1->setEType( eClass2 );
    eClass1->getEStructuralFeatures()->add( eReference1 );

    auto eReference2 = ecoreFactory->createEReference();
    eReference2->setName( "reference2" );
    eReference2->setContainment( false );
    eReference2->setResolveProxies( true );
    eReference2->setEType( eClass2 );
    eClass1->getEStructuralFeatures()->add( eReference2 );

    // create a model from this package
    std::shared_ptr<EObject> eObject1 = eFactory->create( eClass1 );
    std::shared_ptr<EObject> eObject2 = eFactory->create( eClass2 );
    eObject2->eSet( eAttribute1, std::string( "attribute1" ) );
    eObject2->eSet( eAttribute2, 1 );
    eObject1->eSet( eReference1, toAny( eObject2 ) );
    eObject1->eSet( eReference2, toAny( eObject2 ) );

    // check that deep copy is deep equal
    auto eObject1Copy = EcoreUtils::copy( eObject1 );
    BOOST_CHECK( EcoreUtils::equals( eObject1, eObject1Copy ) );

    // modify original object & check that deep equal is not true
    eObject2->eSet( eAttribute2, 2 );
    BOOST_CHECK( !EcoreUtils::equals( eObject1, eObject1Copy ) );
}


BOOST_AUTO_TEST_SUITE_END()
