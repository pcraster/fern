#include "fern/ast/visitor/validate_visitor.h"
#include <sstream>
#include "fern/core/exception.h"
#include "fern/core/string.h"
#include "fern/operation/core/operation.h"
#include "fern/ast/core/vertices.h"


namespace fern {

void ValidateVisitor::Visit(
    NameVertex& vertex)
{
    assert(vertex.expression_types().size() == 1);

    // if(vertex.definitions().empty()) {
    if(!vertex.expression_types()[0].defined()) {
        BOOST_THROW_EXCEPTION(detail::UndefinedIdentifier()
            << detail::ExceptionIdentifier(vertex.name())
            << detail::ExceptionLineNr(vertex.line())
            << detail::ExceptionColNr(vertex.col())
        );
    }
}


void ValidateVisitor::Visit(
    OperationVertex& vertex)
{
    AstVisitor::Visit(vertex);

    // Find out if we know about an operation with the same name as this
    // function's name.
    if(!vertex.operation()) {
        BOOST_THROW_EXCEPTION(detail::UndefinedOperation()
            << detail::ExceptionFunction(vertex.name())
            << detail::ExceptionLineNr(vertex.line())
            << detail::ExceptionColNr(vertex.col())
        );
    }

    fern::Operation const& operation(*vertex.operation());

    // Check if the number of arguments provided equals the required number of
    // arguments.
    if(vertex.expressions().size() != operation.arity()) {
        BOOST_THROW_EXCEPTION(detail::WrongNumberOfArguments()
            << detail::ExceptionFunction(vertex.name())
            << detail::ExceptionRequiredNrArguments(operation.arity())
            << detail::ExceptionProvidedNrArguments(
                vertex.expressions().size())
            << detail::ExceptionLineNr(vertex.line())
            << detail::ExceptionColNr(vertex.col())
        );
    }

    // Check if the result type of each argument expression is compatible
    // with the definition of the operation's parameters.
    for(size_t i = 0; i < operation.arity(); ++i) {
        ExpressionTypes const& parameter_types(
            operation.parameters()[i].expression_types());
        ExpressionTypes const& expression_types(
            vertex.expressions()[i]->expression_types());

        if(!parameter_types.is_satisfied_by(expression_types)) {
            std::ostringstream stream1, stream2;
            stream1 << parameter_types;
            stream2 << expression_types;
            BOOST_THROW_EXCEPTION(detail::WrongTypeOfArgument()
                << detail::ExceptionFunction(vertex.name())
                << detail::ExceptionArgumentId(i + 1)
                << detail::ExceptionRequiredArgumentTypes(stream1.str())
                << detail::ExceptionProvidedArgumentTypes(stream2.str())
                << detail::ExceptionLineNr(vertex.line())
                << detail::ExceptionColNr(vertex.col())
            );
        }
    }
}

} // namespace fern