#include <boost/test/unit_test.hpp>

#include "ecore/EObject.hpp"

using namespace ecore;
using namespace ecore::tests;

namespace
{
    class EObjectDerived : public EObject
    {
    public:
        virtual ~EObjectDerived() = default;
    };

    class NotEObject
    {
    public:
        virtual ~NotEObject() = default;
    };
} // namespace

BOOST_AUTO_TEST_SUITE( EObjectHelpersTests )

BOOST_AUTO_TEST_CASE( IsEObjectOrAnyTest )
{
    BOOST_CHECK( IsSharedEObjectOrAny<Any>::value );
    BOOST_CHECK( IsSharedEObjectOrAny<std::shared_ptr<EObject>>::value );
    BOOST_CHECK( IsSharedEObjectOrAny<std::shared_ptr<EObjectDerived>>::value );
    BOOST_CHECK( !IsSharedEObjectOrAny<int>::value );
    BOOST_CHECK( !IsSharedEObjectOrAny<NotEObject>::value );
}

BOOST_AUTO_TEST_SUITE_END()
