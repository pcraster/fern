#include "geoneric/ast/core/string_vertex.h"


namespace geoneric {

StringVertex::StringVertex(
    int line_nr,
    int col_id,
    String const& value)

    : ExpressionVertex(line_nr, col_id, "\"" + value + "\""),
      _value(value)

{
}


String const& StringVertex::value() const
{
    return _value;
}

} // namespace geoneric
