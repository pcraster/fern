#define BOOST_TEST_MODULE geoneric ast
#include <boost/test/unit_test.hpp>
#include "geoneric/script/algebra_parser.h"
#include "geoneric/ast/core/assignment_vertex.h"
#include "geoneric/ast/visitor/identify_visitor.h"
#include "geoneric/ast/core/if_vertex.h"
#include "geoneric/ast/core/function_call_vertex.h"
#include "geoneric/ast/core/module_vertex.h"
#include "geoneric/ast/core/name_vertex.h"
#include "geoneric/ast/core/operator_vertex.h"
#include "geoneric/ast/xml/xml_parser.h"


class Support
{

public:

    Support()
        : _algebra_parser(),
          _xml_parser(),
          _visitor()
    {
    }

protected:

    geoneric::AlgebraParser _algebra_parser;

    geoneric::XmlParser _xml_parser;

    geoneric::IdentifyVisitor _visitor;

};


//! Return a C pointer to the instance pointed to by the shared pointer.
/*!
  \tparam    Vertex Type of vertex pointer to return.
  \tparam    SmartPointerToVertex Type of smart pointer passed in.
  \param     vertex Smart pointer.
  \return    C pointer.
  \warning   A dynamic cast is performed to convert from the type of the
             instance pointed to by \a vertex to the type that needs to be
             returned. This cast must succeed.

  This is a utility function to make the test cases smaller.
*/
template<
    class Vertex,
    class SmartPointerToVertex>
Vertex const* c_ptr(
    SmartPointerToVertex const& vertex)
{
    Vertex const* result = dynamic_cast<Vertex const*>(&(*vertex));
    assert(result);
    return result;
}


BOOST_FIXTURE_TEST_SUITE(identify_visitor, Support)

BOOST_AUTO_TEST_CASE(visit_empty_script)
{
    using namespace geoneric;

    std::shared_ptr<ModuleVertex> tree;

    tree = _xml_parser.parse_string(_algebra_parser.parse_string(String("")));
    tree->Accept(_visitor);
}


BOOST_AUTO_TEST_CASE(visit_name)
{
    using namespace geoneric;

    std::shared_ptr<ModuleVertex> tree;

    tree = _xml_parser.parse_string(_algebra_parser.parse_string(String("a")));

    NameVertex const* a = c_ptr<NameVertex>(tree->scope()->statements()[0]);

    BOOST_CHECK(a->definitions().empty());
    BOOST_CHECK(a->uses().empty());

    tree->Accept(_visitor);

    BOOST_CHECK(a->definitions().empty());
    BOOST_CHECK(a->uses().empty());
}


BOOST_AUTO_TEST_CASE(visit_assignment)
{
    using namespace geoneric;

    std::shared_ptr<ModuleVertex> tree;

    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            String("a = b")));

        AssignmentVertex const* assignment = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[0]);
        NameVertex const* a = c_ptr<NameVertex>(assignment->target());
        NameVertex const* b = c_ptr<NameVertex>(assignment->expression());

        BOOST_CHECK(a->definitions().empty());
        BOOST_CHECK(a->uses().empty());

        BOOST_CHECK(b->definitions().empty());
        BOOST_CHECK(b->uses().empty());

        tree->Accept(_visitor);

        BOOST_REQUIRE_EQUAL(a->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(a->definitions()[0], a);
        BOOST_CHECK(a->uses().empty());

        BOOST_CHECK(b->definitions().empty());
        BOOST_CHECK(b->uses().empty());
    }

    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            String(
                "a = b\n"
                "d = f(a)\n"
        )));

        AssignmentVertex const* assignment_1 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[0]);
        NameVertex const* a1 = c_ptr<NameVertex>(assignment_1->target());
        NameVertex const* b = c_ptr<NameVertex>(assignment_1->expression());

        AssignmentVertex const* assignment_2 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[1]);
        FunctionCallVertex const* function = c_ptr<FunctionCallVertex>(
            assignment_2->expression());
        NameVertex const* vertex_a2 = c_ptr<NameVertex>(
            function->expressions()[0]);
        NameVertex const* vertex_d = c_ptr<NameVertex>(assignment_2->target());

        BOOST_CHECK(a1->definitions().empty());
        BOOST_CHECK(a1->uses().empty());

        BOOST_CHECK(b->definitions().empty());
        BOOST_CHECK(b->uses().empty());

        BOOST_CHECK(vertex_d->definitions().empty());
        BOOST_CHECK(vertex_d->uses().empty());

        BOOST_CHECK(vertex_a2->definitions().empty());
        BOOST_CHECK(vertex_a2->uses().empty());

        tree->Accept(_visitor);

        BOOST_REQUIRE_EQUAL(a1->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(a1->definitions()[0], a1);
        BOOST_REQUIRE_EQUAL(a1->uses().size(), 1u);
        BOOST_CHECK_EQUAL(a1->uses()[0], vertex_a2);

        BOOST_CHECK(b->definitions().empty());
        BOOST_CHECK(b->uses().empty());

        BOOST_REQUIRE_EQUAL(vertex_d->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(vertex_d->definitions()[0], vertex_d);
        BOOST_CHECK(vertex_d->uses().empty());

        BOOST_REQUIRE_EQUAL(vertex_a2->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(vertex_a2->definitions()[0], a1);
        BOOST_CHECK(vertex_a2->uses().empty());
    }
}


BOOST_AUTO_TEST_CASE(visit_if)
{
    using namespace geoneric;

    std::shared_ptr<ModuleVertex> tree;

    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            String(
                "a = b\n"
                "if True:\n"
                "    a = c\n"
                "d = a\n"
        )));

        AssignmentVertex const* assignment_1 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[0]);
        NameVertex const* a1 = c_ptr<NameVertex>(assignment_1->target());

        IfVertex const* if_ = c_ptr<IfVertex>(tree->scope()->statements()[1]);
        AssignmentVertex const* assignment_2 = c_ptr<AssignmentVertex>(
            if_->true_scope()->statements()[0]);
        NameVertex const* vertex_a2 = c_ptr<NameVertex>(assignment_2->target());

        AssignmentVertex const* assignment_3 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[2]);
        NameVertex const* vertex_a3 = c_ptr<NameVertex>(
            assignment_3->expression());

        tree->Accept(_visitor);

        BOOST_REQUIRE_EQUAL(a1->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(a1->definitions()[0], a1);
        BOOST_REQUIRE_EQUAL(a1->uses().size(), 1u);
        BOOST_CHECK_EQUAL(a1->uses()[0], vertex_a3);

        BOOST_REQUIRE_EQUAL(vertex_a2->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(vertex_a2->definitions()[0], vertex_a2);
        // TODO
        // BOOST_REQUIRE_EQUAL(vertex_a2->uses().size(), 1u);
        // BOOST_CHECK_EQUAL(vertex_a2->uses()[0], vertex_a3);

        // BOOST_REQUIRE_EQUAL(vertex_a3->definitions().size(), 2u);
        // BOOST_CHECK_EQUAL(vertex_a3->definitions()[0], a1);
        // BOOST_CHECK_EQUAL(vertex_a3->definitions()[0], vertex_a2);
        // BOOST_CHECK(vertex_a3->uses().empty());
    }
}


BOOST_AUTO_TEST_CASE(visit_reuse_of_identifiers)
{
    using namespace geoneric;

    std::shared_ptr<ModuleVertex> tree;

    {
        tree = _xml_parser.parse_string(_algebra_parser.parse_string(
            String(
                "a = \"MyRaster\"\n"
                "b = abs(a)\n"
                "c = abs(b)\n"
                "b = c + b\n"
        )));

        AssignmentVertex const* assignment_1 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[0]);
        NameVertex const* a1 = c_ptr<NameVertex>(assignment_1->target());
        BOOST_REQUIRE(assignment_1);
        BOOST_REQUIRE(a1);

        AssignmentVertex const* assignment_2 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[1]);
        NameVertex const* vertex_b1 = c_ptr<NameVertex>(assignment_2->target());
        FunctionCallVertex const* vertex_abs1 = c_ptr<FunctionCallVertex>(
            assignment_2->expression());
        NameVertex const* vertex_a2 = c_ptr<NameVertex>(
            vertex_abs1->expressions()[0]);
        BOOST_REQUIRE(assignment_2);
        BOOST_REQUIRE(vertex_b1);
        BOOST_REQUIRE(vertex_abs1);
        BOOST_REQUIRE(vertex_a2);

        AssignmentVertex const* assignment_3 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[2]);
        NameVertex const* c1 = c_ptr<NameVertex>(assignment_3->target());
        FunctionCallVertex const* vertex_abs2 = c_ptr<FunctionCallVertex>(
            assignment_3->expression());
        NameVertex const* b2 = c_ptr<NameVertex>(vertex_abs2->expressions()[0]);
        BOOST_REQUIRE(assignment_3);
        BOOST_REQUIRE(c1);
        BOOST_REQUIRE(vertex_abs2);
        BOOST_REQUIRE(b2);

        AssignmentVertex const* assignment_4 = c_ptr<AssignmentVertex>(
            tree->scope()->statements()[3]);
        NameVertex const* vertex_b4 = c_ptr<NameVertex>(
            assignment_4->target());
        OperatorVertex const* plus_1 = c_ptr<OperatorVertex>(
            assignment_4->expression());
        NameVertex const* c2 = c_ptr<NameVertex>(plus_1->expressions()[0]);
        NameVertex const* vertex_b3 = c_ptr<NameVertex>(
            plus_1->expressions()[1]);
        BOOST_REQUIRE(assignment_4);
        BOOST_REQUIRE(vertex_b4);
        BOOST_REQUIRE(plus_1);
        BOOST_REQUIRE(c2);
        BOOST_REQUIRE(vertex_b3);

        tree->Accept(_visitor);

        BOOST_REQUIRE_EQUAL(a1->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(a1->definitions()[0], a1);
        BOOST_REQUIRE_EQUAL(a1->uses().size(), 1u);
        BOOST_CHECK_EQUAL(a1->uses()[0], vertex_a2);

        BOOST_REQUIRE_EQUAL(vertex_b1->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(vertex_b1->definitions()[0], vertex_b1);
        BOOST_REQUIRE_EQUAL(vertex_b1->uses().size(), 2u);
        BOOST_CHECK_EQUAL(vertex_b1->uses()[0], b2);
        BOOST_CHECK_EQUAL(vertex_b1->uses()[1], vertex_b3);

        BOOST_REQUIRE_EQUAL(c1->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(c1->definitions()[0], c1);
        BOOST_REQUIRE_EQUAL(c1->uses().size(), 1u);
        BOOST_CHECK_EQUAL(c1->uses()[0], c2);

        BOOST_REQUIRE_EQUAL(vertex_b4->definitions().size(), 1u);
        BOOST_CHECK_EQUAL(vertex_b4->definitions()[0], vertex_b4);
        BOOST_REQUIRE_EQUAL(vertex_b4->uses().size(), 0u);
    }
}

BOOST_AUTO_TEST_SUITE_END()