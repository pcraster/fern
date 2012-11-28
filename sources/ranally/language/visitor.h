#pragma once
#include <loki/Visitor.h>
#include "ranally/language/syntax_vertex.h"


namespace ranally {

class AssignmentVertex;
class FunctionVertex;
class IfVertex;
class NameVertex;
template<typename T>
    class NumberVertex;
class OperationVertex;
class OperatorVertex;
class ScriptVertex;
class StringVertex;
class WhileVertex;


//! Base class for syntax tree visitors.
/*!
  This class offers default implementations for the visit functions for all
  SyntaxVertex specializations.
*/
class Visitor:
    public Loki::BaseVisitor,
    public Loki::Visitor<AssignmentVertex>,
    public Loki::Visitor<FunctionVertex>,
    public Loki::Visitor<IfVertex>,
    public Loki::Visitor<NameVertex>,
    public Loki::Visitor<NumberVertex<int8_t>>,
    public Loki::Visitor<NumberVertex<int16_t>>,
    public Loki::Visitor<NumberVertex<int32_t>>,
    public Loki::Visitor<NumberVertex<int64_t>>,
    public Loki::Visitor<NumberVertex<uint8_t>>,
    public Loki::Visitor<NumberVertex<uint16_t>>,
    public Loki::Visitor<NumberVertex<uint32_t>>,
    public Loki::Visitor<NumberVertex<uint64_t>>,
    public Loki::Visitor<NumberVertex<float>>,
    public Loki::Visitor<NumberVertex<double>>,
    public Loki::Visitor<OperatorVertex>,
    public Loki::Visitor<ScriptVertex>,
    public Loki::Visitor<StringVertex>,
    public Loki::Visitor<WhileVertex>
{

    friend class VisitorTest;

public:

protected:

                   Visitor             ()=default;

    virtual        ~Visitor            ()=default;

                   Visitor             (Visitor&&)=delete;

    Visitor&       operator=           (Visitor&&)=delete;

                   Visitor             (Visitor const&)=delete;

    Visitor&       operator=           (Visitor const&)=delete;

    virtual void   visitStatements     (StatementVertices& statements);

    virtual void   visitExpressions    (ExpressionVertices const& expressions);

    virtual void   Visit               (AssignmentVertex& vertex);

    virtual void   Visit               (IfVertex& vertex);

    virtual void   Visit               (FunctionVertex& vertex);

    virtual void   Visit               (ScriptVertex& vertex);

    virtual void   Visit               (WhileVertex& vertex);

private:

    virtual void   Visit               (ExpressionVertex& vertex);

    virtual void   Visit               (NameVertex& vertex);

    virtual void   Visit               (NumberVertex<int8_t>& vertex);

    virtual void   Visit               (NumberVertex<int16_t>& vertex);

    virtual void   Visit               (NumberVertex<int32_t>& vertex);

    virtual void   Visit               (NumberVertex<int64_t>& vertex);

    virtual void   Visit               (NumberVertex<uint8_t>& vertex);

    virtual void   Visit               (NumberVertex<uint16_t>& vertex);

    virtual void   Visit               (NumberVertex<uint32_t>& vertex);

    virtual void   Visit               (NumberVertex<uint64_t>& vertex);

    virtual void   Visit               (NumberVertex<float>& vertex);

    virtual void   Visit               (NumberVertex<double>& vertex);

    virtual void   Visit               (OperationVertex& vertex);

    virtual void   Visit               (OperatorVertex& vertex);

    virtual void   Visit               (StatementVertex& vertex);

    virtual void   Visit               (StringVertex& vertex);

    virtual void   Visit               (SyntaxVertex& vertex);

};

} // namespace ranally