#define BOOST_TEST_MODULE ranally language
#include <boost/test/unit_test.hpp>
#include "ranally/core/string.h"
#include "ranally/script/algebra_parser.h"
#include "ranally/language/script_vertex.h"
#include "ranally/language/xml_parser.h"


class Support
{

public:

    Support()
        : _algebra_parser(),
          _xml_parser()
    {
    }

protected:

    ranally::AlgebraParser _algebra_parser;

    ranally::XmlParser _xml_parser;

};


BOOST_FIXTURE_TEST_SUITE(dot_visitor, Support)

BOOST_AUTO_TEST_CASE(visit_empty_script)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String(""));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "}\n"
    // );
}


BOOST_AUTO_TEST_CASE(visit_name)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("a"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  a;\n"
    //   "}\n"
    // );
}


BOOST_AUTO_TEST_CASE(visit_assignment)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("a = b"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  b -> a;\n"
    //   "}\n"
    // );
}


BOOST_AUTO_TEST_CASE(visit_string)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("\"five\""));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  \"five\";\n"
    //   "}\n"
    // );
}


BOOST_AUTO_TEST_CASE(visit_number)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("5"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  5;\n"
    //   "}\n"
    // );

    // xml = _algebra_parser.parse_string(ranally::String("5L"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  5;\n"
    //   "}\n"
    // );

    // xml = _algebra_parser.parse_string(ranally::String("5.5"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  5.5;\n"
    //   "}\n"
    // );

    // // TODO add tests for all numeric types.
}


BOOST_AUTO_TEST_CASE(visit_function)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("f()"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  f;\n"
    //   "}\n"
    // );

    // xml = _algebra_parser.parse_string(ranally::String("f(1, \"2\", three, four())"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "digraph G {\n"
    //   "  1 -> f;\n"
    //   "  \"2\" -> f;\n"
    //   "  three -> f;\n"
    //   "  four -> f;\n"
    //   "}\n"
    // );
}


BOOST_AUTO_TEST_CASE(visit_operator)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("-a"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) == "-(a)\n");

    // xml = _algebra_parser.parse_string(ranally::String("a + b"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) == "(a) + (b)\n");

    // xml = _algebra_parser.parse_string(ranally::String("-(a + b)"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) == "-((a) + (b))\n");

    // xml = _algebra_parser.parse_string(ranally::String("a + b * c + d"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //   "((a) + ((b) * (c))) + (d)\n");
}


BOOST_AUTO_TEST_CASE(visit_multiple_statements)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String("a\nb"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) == "a\nb\n");
}


BOOST_AUTO_TEST_CASE(visit_if)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String(
    //     "if a:\n"
    //     "  b\n"
    //     "  c"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //     "if a:\n"
    //     "  b\n"
    //     "  c\n");

    // xml = _algebra_parser.parse_string(ranally::String(
    //     "if a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "elif d:\n"
    //     "  e\n"
    //     "  f\n"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //     "if a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "else:\n"
    //     "  if d:\n"
    //     "    e\n"
    //     "    f\n");

    // xml = _algebra_parser.parse_string(ranally::String(
    //     "if a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "elif d:\n"
    //     "  e\n"
    //     "  f\n"
    //     "else:\n"
    //     "  g\n"
    //     "  h\n"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //     "if a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "else:\n"
    //     "  if d:\n"
    //     "    e\n"
    //     "    f\n"
    //     "  else:\n"
    //     "    g\n"
    //     "    h\n");
}


BOOST_AUTO_TEST_CASE(visit_while)
{
    // ranally::String xml;

    // xml = _algebra_parser.parse_string(ranally::String(
    //     "while a:\n"
    //     "  b\n"
    //     "  c"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //     "while a:\n"
    //     "  b\n"
    //     "  c\n");

    // xml = _algebra_parser.parse_string(ranally::String(
    //     "while a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "else:\n"
    //     "  d\n"
    //     "  e"));
    // BOOST_CHECK(_xml_parser.parse(xml)->Accept(_visitor) ==
    //     "while a:\n"
    //     "  b\n"
    //     "  c\n"
    //     "else:\n"
    //     "  d\n"
    //     "  e\n");
}

BOOST_AUTO_TEST_SUITE_END()
