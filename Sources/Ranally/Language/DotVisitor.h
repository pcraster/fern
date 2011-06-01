#ifndef INCLUDED_RANALLY_LANGUAGE_DOTVISITOR
#define INCLUDED_RANALLY_LANGUAGE_DOTVISITOR

#include <boost/noncopyable.hpp>
#include <loki/Visitor.h>
#include <unicode/unistr.h>
#include "Ranally/Language/SyntaxVertex.h"



namespace ranally {

namespace language {

class AssignmentVertex;
class FunctionVertex;
class IfVertex;
class NameVertex;
template<typename T>
  class NumberVertex;
class OperatorVertex;
class ScriptVertex;
class StringVertex;
class WhileVertex;

} // namespace language

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  The dot graph is mainly useful for debugging purposes. The graph is handy
  for visualising the syntax-tree.
*/
class DotVisitor: private boost::noncopyable,
  public Loki::BaseVisitor,
  public Loki::Visitor<language::AssignmentVertex>,
  public Loki::Visitor<language::FunctionVertex>,
  public Loki::Visitor<language::IfVertex>,
  public Loki::Visitor<language::NameVertex>,
  public Loki::Visitor<language::NumberVertex<int8_t> >,
  public Loki::Visitor<language::NumberVertex<int16_t> >,
  public Loki::Visitor<language::NumberVertex<int32_t> >,
  public Loki::Visitor<language::NumberVertex<int64_t> >,
  public Loki::Visitor<language::NumberVertex<uint8_t> >,
  public Loki::Visitor<language::NumberVertex<uint16_t> >,
  public Loki::Visitor<language::NumberVertex<uint32_t> >,
  public Loki::Visitor<language::NumberVertex<uint64_t> >,
  public Loki::Visitor<language::NumberVertex<float> >,
  public Loki::Visitor<language::NumberVertex<double> >,
  public Loki::Visitor<language::OperatorVertex>,
  public Loki::Visitor<language::ScriptVertex>,
  public Loki::Visitor<language::StringVertex>,
  public Loki::Visitor<language::WhileVertex>
{

  friend class DotVisitorTest;

public:

  virtual          ~DotVisitor         ();

  UnicodeString const& script          () const;

protected:

                   DotVisitor          ();

  void             setScript           (UnicodeString const& string);

  void             addScript           (UnicodeString const& string);

  void             Visit               (
                                  language::NumberVertex<int8_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<int16_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<int32_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<int64_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<uint8_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<uint16_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<uint32_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<uint64_t>& vertex);

  void             Visit               (
                                  language::NumberVertex<float>& vertex);

  void             Visit               (
                                  language::NumberVertex<double>& vertex);

private:

  UnicodeString    _script;

  /// language::SyntaxVertex const* _definition;

  virtual void     Visit               (language::ScriptVertex& vertex)=0;

  template<typename T>
  void             Visit               (language::NumberVertex<T>& vertex);

};

} // namespace ranally

#endif
