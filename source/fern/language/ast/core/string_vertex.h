// -----------------------------------------------------------------------------
// Fern © Geoneric
//
// This file is part of Geoneric Fern which is available under the terms of
// the GNU General Public License (GPL), version 2. If you do not want to
// be bound by the terms of the GPL, you may purchase a proprietary license
// from Geoneric (http://www.geoneric.eu/contact).
// -----------------------------------------------------------------------------
#pragma once
#include "fern/language/ast/core/expression_vertex.h"


namespace fern {
namespace language {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class StringVertex:
    public ExpressionVertex
{

    friend class StringVertexTest;

public:

    LOKI_DEFINE_VISITABLE()

                   StringVertex        (int line_nr,
                                        int col_id,
                                        String const& value);

                   ~StringVertex       ()=default;

                   StringVertex        (StringVertex&&)=delete;

    StringVertex&  operator=           (StringVertex&&)=delete;

                   StringVertex        (StringVertex const&)=delete;

    StringVertex&  operator=           (StringVertex const&)=delete;

    String const&  value               () const;

private:

    String         _value;

};

} // namespace language
} // namespace fern
