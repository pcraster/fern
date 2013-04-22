#include "ranally/ast/core/function_vertex.h"


namespace ranally {

FunctionVertex::FunctionVertex(
    String const& name,
    ExpressionVertices const& expressions)

    : OperationVertex(name, expressions)

{
}

} // namespace ranally