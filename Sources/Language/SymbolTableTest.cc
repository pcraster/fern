#include "SymbolTableTest.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "SymbolTable.h"
#include "NameVertex.h"



boost::unit_test::test_suite* SymbolTableTest::suite()
{
  boost::unit_test::test_suite* suite = BOOST_TEST_SUITE(__FILE__);
  boost::shared_ptr<SymbolTableTest> instance(
    new SymbolTableTest());
  suite->add(BOOST_CLASS_TEST_CASE(
    &SymbolTableTest::testScoping, instance));

  return suite;
}



SymbolTableTest::SymbolTableTest()
{
}



void SymbolTableTest::testScoping()
{
  using namespace ranally;
  using namespace ranally::language;

  SymbolTable table;
  UnicodeString name("a");
  BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(0));

  // Add and remove one definition for 'a'.
  {
    table.pushScope();
    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(1));

    boost::scoped_ptr<NameVertex> a(new NameVertex(name));
    table.addDefinition(a.get());
    BOOST_REQUIRE(table.hasDefinition(name));
    BOOST_CHECK_EQUAL(table.scopeLevel(name), table.scopeLevel());

    table.popScope();
    BOOST_REQUIRE(!table.hasDefinition(name));

    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(0));
  }

  // Add and remove two definitions for 'a', not nested.
  {
    table.pushScope();
    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(1));

    boost::scoped_ptr<NameVertex> a1(new NameVertex(name));
    table.addDefinition(a1.get());
    BOOST_REQUIRE(table.hasDefinition(name));
    BOOST_CHECK_EQUAL(table.scopeLevel(name), table.scopeLevel());

    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(1));

    boost::scoped_ptr<NameVertex> a2(new NameVertex(name));
    table.addDefinition(a2.get());
    BOOST_REQUIRE(table.hasDefinition(name));
    BOOST_CHECK_EQUAL(table.scopeLevel(name), table.scopeLevel());

    // Should remove all definitions of 'a' in the current scope.
    table.popScope();
    BOOST_REQUIRE(!table.hasDefinition(name));

    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(0));
  }

  // Add and remove two definitions, nested.
  {
    table.pushScope();
    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(1));

    boost::scoped_ptr<NameVertex> a1(new NameVertex(name));
    table.addDefinition(a1.get());
    BOOST_REQUIRE(table.hasDefinition(name));

    table.pushScope();
    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(2));

    boost::scoped_ptr<NameVertex> a2(new NameVertex(name));
    table.addDefinition(a2.get());
    BOOST_REQUIRE(table.hasDefinition(name));

    // Should reveal the first definition.
    table.popScope();
    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(1));

    BOOST_REQUIRE(table.hasDefinition(name));
    BOOST_CHECK_EQUAL(table.scopeLevel(name), table.scopeLevel());

    table.popScope();
    BOOST_REQUIRE(!table.hasDefinition(name));

    BOOST_CHECK_EQUAL(table.scopeLevel(), SymbolTable::size_type(0));
  }
}
