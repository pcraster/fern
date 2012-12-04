#include "ranally/language/subscript_vertex.h"


namespace ranally {

SubscriptVertex::SubscriptVertex(
    ExpressionVertexPtr const& expression,
    ExpressionVertexPtr const& selection)

    : ExpressionVertex("subscript"),
      _expression(expression),
      _selection(selection)

{
}


ExpressionVertexPtr const& SubscriptVertex::expression() const
{
    return _expression;
}


ExpressionVertexPtr const& SubscriptVertex::selection() const
{
    return _selection;
}

} // namespace ranally
