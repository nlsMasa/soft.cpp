#include "ecore/Stream.hpp"
#include "ecore/impl/BasicEMap.hpp"
#include <boost/test/unit_test.hpp>

#include <Windows.h>
#include <map>

using namespace ecore;
using namespace ecore::impl;

using TestMap = BasicEMap<int, int>;
using TestEntry = EMapEntryImpl<int, int>;

namespace
{

    struct no_debugger
    {
        boost::test_tools::assertion_result operator()( boost::unit_test::test_unit_id )
        {
            return !IsDebuggerPresent();
        }
    };

} // namespace

namespace std
{

    std::ostream& operator<<( std::ostream& out, const EMapEntry& entry )
    {
        return out << entry.getKey() << ": " << entry.getValue();
    }

    template <class Key, class Value>
    std::ostream& operator<<( std::ostream& out, const BasicEMap<Key, Value>& emap )
    {
        return print_container( out, emap );
    }

    template <class T>
    std::ostream& operator<<( std::ostream& out, const std::optional<T>& optional )
    {
        if( optional == std::nullopt )
            return out << "<none>";
        return out << optional.value();
    }

    bool compareSharedPtrs( const std::shared_ptr<EMapEntry>& lhs, const std::shared_ptr<EMapEntry>& rhs )
    {
        return lhs->getKey() == rhs->getKey() && lhs->getValue() == rhs->getValue();
    }

    template <class Key, class Value>
    bool operator==( const BasicEMap<Key, Value>& lhs, const std::vector<std::shared_ptr<EMapEntry>>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin(), compareSharedPtrs );
    }

    template <class Key, class Value>
    bool operator==( const BasicEMap<Key, Value>& lhs, const std::vector<std::shared_ptr<EMapEntryImpl<Key, Value>>>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin(), compareSharedPtrs );
    }

} // namespace std

BOOST_AUTO_TEST_SUITE( BasicEMapTests )

BOOST_AUTO_TEST_CASE( Constructor_Default )
{
    BOOST_CHECK_NO_THROW( TestMap(); );
}

BOOST_AUTO_TEST_CASE( Empty )
{
    {
        TestMap map;
        BOOST_CHECK( map.empty() );
    }
    {
        auto map = TestMap( { std::make_shared<TestEntry>( 1, 1 ) } );
        BOOST_CHECK( !map.empty() );
    }
}

BOOST_AUTO_TEST_CASE( Constructor_Initializer )
{
    auto map = TestMap( { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
    auto expected = std::vector{ std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) };
    BOOST_CHECK_EQUAL( map, expected );
}

BOOST_AUTO_TEST_CASE( Size )
{
    {
        TestMap map;
        BOOST_CHECK_EQUAL( map.size(), 0 );
    }
    {
        auto map = TestMap( { std::make_shared<TestEntry>( 1, 1 ), std::make_shared<TestEntry>( 2, 2 ) } );
        BOOST_CHECK_EQUAL( map.size(), 2 );
    }
}

BOOST_AUTO_TEST_CASE( At )
{
    auto map = TestMap( { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
    BOOST_CHECK_EQUAL( map.at( 1 ), 11 );
    BOOST_CHECK_EQUAL( map.at( 2 ), 22 );
}

BOOST_AUTO_TEST_CASE( Insert )
{
    TestMap map;
    map.insert( 1, 11 );
    map.insert( 2, 22 );
    BOOST_CHECK_EQUAL( map.size(), 2 );
    BOOST_CHECK_EQUAL( map.at( 1 ), 11 );
    map.insert( 1, 12 );
    BOOST_CHECK_EQUAL( map.size(), 2 );
    BOOST_CHECK_EQUAL( map.at( 1 ), 12 );
    BOOST_CHECK_EQUAL( map.at( 2 ), 22 );
    map.insert( 3, 33 );
    BOOST_CHECK_EQUAL( map.size(), 3 );
    BOOST_CHECK_EQUAL( map.at( 3 ), 33 );
}

BOOST_AUTO_TEST_CASE( Add )
{
    TestMap map;
    map.add( std::make_shared<TestEntry>( 1, 11 ) );
    map.add( std::make_shared<TestEntry>( 2, 22 ) );
    BOOST_CHECK_EQUAL( map.at( 1 ), 11 );
    map.add( std::make_shared<TestEntry>( 1, 12 ) );
    BOOST_CHECK_EQUAL( map.size(), 2 );
    BOOST_CHECK_EQUAL( map.at( 1 ), 11 );
    BOOST_CHECK_EQUAL( map.at( 2 ), 22 );
    map.add( std::make_shared<TestEntry>( 3, 33 ) );
    BOOST_CHECK_EQUAL( map.size(), 3 );
    BOOST_CHECK_EQUAL( map.at( 3 ), 33 );
}

BOOST_AUTO_TEST_CASE( Iterators )
{
    TestMap map;
    BOOST_CHECK( map.begin() == map.end() );

    for( int counter = 0; counter < 10; ++counter )
        map.insert( counter, counter * 10 );

    int test = 0;
    for( const auto& entry : map )
    {
        BOOST_CHECK_EQUAL( entry->getKey(), test );
        BOOST_CHECK_EQUAL( entry->getValue(), test * 10 );
        ++test;
    }
}

BOOST_AUTO_TEST_CASE( KeyOf )
{
    TestMap map;
    map.insert( 1, 11 );
    map.insert( 2, 22 );
    BOOST_CHECK_EQUAL( map.keyOf( 11 ), 1 );
    BOOST_CHECK_EQUAL( map.keyOf( 22 ), 2 );
}

BOOST_AUTO_TEST_CASE( IndexOf )
{
    TestMap map;
    map.insert( 1, 11 );
    map.insert( 2, 22 );
    BOOST_CHECK_EQUAL( map.indexOf( []( const auto& entry ) { return entry->getKey() == 1; } ), 0 );
    BOOST_CHECK_EQUAL( map.indexOf( []( const auto& entry ) { return entry->getKey() == 2; } ), 1 );
}

BOOST_AUTO_TEST_CASE( Merge )
{
    {
        TestMap map;
        auto other = ImmutableArrayEList<std::shared_ptr<EMapEntry>>(
            { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
        map.merge( other );
        auto expected
            = std::vector<std::shared_ptr<TestEntry>>{ std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) };
        BOOST_CHECK_EQUAL( map, expected );
    }
    {
        auto map = TestMap( { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
        auto other = ImmutableArrayEList<std::shared_ptr<EMapEntry>>(
            { std::make_shared<TestEntry>( 3, 33 ), std::make_shared<TestEntry>( 4, 44 ) } );
        map.merge( other );
        auto expected = std::vector<std::shared_ptr<TestEntry>>{ std::make_shared<TestEntry>( 1, 11 ),
                                                                 std::make_shared<TestEntry>( 2, 22 ),
                                                                 std::make_shared<TestEntry>( 3, 33 ),
                                                                 std::make_shared<TestEntry>( 4, 44 ) };
        BOOST_CHECK_EQUAL( map, expected );
    }
}

BOOST_AUTO_TEST_CASE( Remove_InvalidKey )
{
    TestMap map;
    BOOST_CHECK_EQUAL( map.remove( 1 ), false );
}

BOOST_AUTO_TEST_CASE( Remove )
{
    auto map = TestMap( { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
    BOOST_CHECK_EQUAL( map.remove( 1 ), true );
    BOOST_CHECK_EQUAL( map.size(), 1 );
}

BOOST_AUTO_TEST_CASE( Clear )
{
    auto map = TestMap( { std::make_shared<TestEntry>( 1, 11 ), std::make_shared<TestEntry>( 2, 22 ) } );
    map.clear();
    BOOST_CHECK( map.empty() );
}

template <class T>
using TestDelegateEntry = EMapEntryImpl<int, std::shared_ptr<T>>;

BOOST_AUTO_TEST_CASE( Delegate_SharedPtr )
{
    class A
    {
    public:
        A() = default;
        virtual ~A() = default;
    };

    class B : public A
    {
    public:
        B() = default;
        virtual ~B() = default;
    };

    auto init = std::initializer_list<std::shared_ptr<TestDelegateEntry<B>>>{
        std::make_shared<TestDelegateEntry<B>>( 1, std::make_shared<B>() ),
        std::make_shared<TestDelegateEntry<B>>( 2, std::make_shared<B>() ) };
    auto map = std::make_shared<BasicEMap<int, std::shared_ptr<B>>>( init );
    auto delegate = map->asEMapOf<std::shared_ptr<A>>();
    auto newB = std::make_shared<B>();

    BOOST_CHECK_EQUAL( delegate->size(), 2 );
    delegate->insert( 3, newB );
    BOOST_CHECK_EQUAL( map->size(), 3 );
    BOOST_CHECK_EQUAL( map->at( 3 ), newB );
}

BOOST_AUTO_TEST_CASE( Delegate_Any )
{
    class A
    {
    public:
        A() = default;
        virtual ~A() = default;
    };

    auto init = std::initializer_list<std::shared_ptr<TestDelegateEntry<A>>>{
        std::make_shared<TestDelegateEntry<A>>( 1, std::make_shared<A>() ),
        std::make_shared<TestDelegateEntry<A>>( 2, std::make_shared<A>() ) };
    auto map = std::make_shared<BasicEMap<int, std::shared_ptr<A>>>( init );
    auto delegate = map->asEMapOf<Any>();
    auto newA = std::make_shared<A>();

    BOOST_CHECK_EQUAL( delegate->size(), 2 );
    delegate->insert( 3, newA );
    BOOST_CHECK_EQUAL( map->size(), 3 );
    BOOST_CHECK_EQUAL( map->at( 3 ), newA );
    BOOST_CHECK_EQUAL( delegate->at( 3 ), newA );
}

BOOST_AUTO_TEST_SUITE_END()
