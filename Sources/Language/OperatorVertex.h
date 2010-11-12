#ifndef INCLUDED_RANALLY_OPERATORVERTEX
#define INCLUDED_RANALLY_OPERATORVERTEX

#include <vector>
#include <boost/shared_ptr.hpp>

#include "ExpressionVertex.h"



namespace ranally {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class OperatorVertex: public ExpressionVertex
{

  friend class OperatorVertexTest;

public:

  LOKI_DEFINE_VISITABLE()

private:

  ExpressionVertices _expressions;

protected:

public:

                   OperatorVertex      (UnicodeString const& name,
                                        ExpressionVertices const& expressions);

  /* virtual */    ~OperatorVertex     ();

  ExpressionVertices const& expressions() const;

};

} // namespace ranally

#endif
