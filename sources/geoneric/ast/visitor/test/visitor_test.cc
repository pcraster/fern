#define BOOST_TEST_MODULE geoneric ast
#include <boost/test/unit_test.hpp>
#include "geoneric/script/algebra_parser.h"
#include "geoneric/ast/core/module_vertex.h"
#include "geoneric/ast/visitor/visitor.h"
#include "geoneric/ast/xml/xml_parser.h"


class CountVerticesVisitor:
    public geoneric::Visitor
{

public:

    CountVerticesVisitor()
        : geoneric::Visitor(),
          _nr_vertices(0u)
    {
    }

    size_t nr_vertices() const
    {
        return _nr_vertices;
    }

private:

    size_t           _nr_vertices;

    void Visit(
        geoneric::ModuleVertex& vertex)
    {
        _nr_vertices = 0u;
        geoneric::Visitor::Visit(vertex);
    }

    void Visit(
        geoneric::AstVertex& /* vertex */)
    {
        ++_nr_vertices;
    }

};


class Support
{

public:

    Support()
        : _algebra_parser(),
          _xml_parser()
    {
    }

protected:

    geoneric::AlgebraParser _algebra_parser;

    geoneric::XmlParser _xml_parser;

};


BOOST_FIXTURE_TEST_SUITE(visitor, Support)


BOOST_AUTO_TEST_CASE(count_vertices_visitor)
{
    CountVerticesVisitor visitor;
    std::shared_ptr<geoneric::ModuleVertex> tree;

    // Empty script.
    {
        // Script, scope, sentinel.
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 3u);
    }

    // Name.
    {
        // Script, scope, name, sentinel.
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("a")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 4u);
    }

    // Number.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("5")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 4u);
    }

    // String.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("\"five\"")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 4u);
    }

    // Operator.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("a + b")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 6u);
    }

    // Function.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("f(a, b)")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 6u);
    }

    // Assignment.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("c = f(a, b)")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 8u);
    }

    // If.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String(
                "if a > b:\n"
                "    c = d\n"
                "else:\n"
                "    e = f")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 17u);
    }

    // While.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String(
                "while a > b:\n"
                "    c = c + d\n"
                "else:\n"
                "    e = f")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 19u);
    }

    // Slice.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String("a[b]")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 6u);
    }

    // Function definition.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String(u8R"(
def foo():
    return
)")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 7u);
    }

    // Function call.
    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            geoneric::String(u8R"(
bla()
)")));
        assert(tree);
        tree->Accept(visitor);
        BOOST_CHECK_EQUAL(visitor.nr_vertices(), 4u);
    }
}

BOOST_AUTO_TEST_SUITE_END()