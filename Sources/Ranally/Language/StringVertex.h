#ifndef INCLUDED_RANALLY_LANGUAGE_STRINGVERTEX
#define INCLUDED_RANALLY_LANGUAGE_STRINGVERTEX

#include <unicode/unistr.h>
#include "Ranally/Language/ExpressionVertex.h"



namespace ranally {
namespace language {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class StringVertex: public ExpressionVertex
{

  friend class StringVertexTest;

private:

  UnicodeString    _value;

protected:

public:

  LOKI_DEFINE_VISITABLE()

                   StringVertex        (int lineNr,
                                        int colId,
                                        UnicodeString const& value);

  /* virtual */    ~StringVertex       ();

  UnicodeString const& value           () const;

};

} // namespace language
} // namespace ranally

#endif
