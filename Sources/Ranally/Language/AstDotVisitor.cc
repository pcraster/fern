#include "Ranally/Language/AstDotVisitor.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "Ranally/Language/Vertices.h"



namespace ranally {

AstDotVisitor::AstDotVisitor(
  int modes)

  : DotVisitor(),
    _mode(Declaring),
    _modes(modes)

{
}



AstDotVisitor::~AstDotVisitor()
{
}



void AstDotVisitor::setMode(
  Mode mode)
{
  _mode = mode;
}



void AstDotVisitor::addAstVertex(
  language::SyntaxVertex const& sourceVertex,
  language::SyntaxVertex const& targetVertex)
{
  assert(_mode == ConnectingAst);
  addScript(
    UnicodeString((boost::format("\"%1%\"") % &sourceVertex).str().c_str()) +
    " -> " +
    (boost::format("\"%1%\"") % &targetVertex).str().c_str() + " ["
    "];\n"
  );
}



void AstDotVisitor::addCfgVertices(
  language::SyntaxVertex const& sourceVertex)
{
  assert(_mode == ConnectingCfg);
  BOOST_FOREACH(language::SyntaxVertex const* successor,
    sourceVertex.successors()) {
    addScript(
      UnicodeString((boost::format("\"%1%\"") % &sourceVertex).str().c_str()) +
      " -> " +
      (boost::format("\"%1%\"") % successor).str().c_str() + " ["
        "color=red, "
        "constraint=false, "
        "style=dashed"
      "];\n"
    );
  }
}



void AstDotVisitor::addUseVertices(
  language::NameVertex const& vertex)
{
  assert(_mode == ConnectingUses);
  BOOST_FOREACH(language::NameVertex const* use, vertex.uses()) {
    addScript(
      UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
      " -> " +
      (boost::format("\"%1%\"") % use).str().c_str() + " ["
        "color=blue, "
        "constraint=false, "
        "style=dotted"
      "];\n"
    );
  }
}






void AstDotVisitor::Visit(
  language::AssignmentVertex& vertex)
{
  language::ExpressionVertices const& targets = vertex.targets();
  language::ExpressionVertices const& expressions = vertex.expressions();

  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"=\"];\n");
      break;
    }
    case ConnectingAst: {
      assert(expressions.size() == targets.size());
      for(size_t i = 0; i < expressions.size(); ++i) {
        addAstVertex(vertex, *vertex.targets()[i]);
        addAstVertex(vertex, *vertex.expressions()[i]);
      }
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }

  BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
    expressionVertex, vertex.expressions()) {
    expressionVertex->Accept(*this);
  }

  BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
    expressionVertex, vertex.targets()) {
    expressionVertex->Accept(*this);
  }
}



void AstDotVisitor::Visit(
  language::OperatorVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      // TODO Implement symbol member.
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"" + vertex.symbol() + "\"];\n"
      );
      break;
    }
    case ConnectingAst: {
      BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
        expressionVertex, vertex.expressions()) {
        addAstVertex(vertex, *expressionVertex);
      }
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }

  BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
    expressionVertex, vertex.expressions()) {
    expressionVertex->Accept(*this);
  }
}



void AstDotVisitor::Visit(
  language::FunctionVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"" + vertex.name() + "\"];\n");
      break;
    }
    case ConnectingAst: {
      BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
        expressionVertex, vertex.expressions()) {
        addAstVertex(vertex, *expressionVertex);
      }
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }

  BOOST_FOREACH(boost::shared_ptr<language::ExpressionVertex>
    expressionVertex, vertex.expressions()) {
    expressionVertex->Accept(*this);
  }
}



void AstDotVisitor::Visit(
  language::IfVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"If\", shape=diamond];\n"
      );
      break;
    }
    case ConnectingAst: {
      addAstVertex(vertex, *vertex.condition());
      BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
        statementVertex, vertex.trueStatements()) {
        addAstVertex(vertex, *statementVertex);
      }
      BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
        statementVertex, vertex.falseStatements()) {
        addAstVertex(vertex, *statementVertex);
      }
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }

  vertex.condition()->Accept(*this);
  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
    statementVertex, vertex.trueStatements()) {
    statementVertex->Accept(*this);
  }
  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
    statementVertex, vertex.falseStatements()) {
    statementVertex->Accept(*this);
  }
}



void AstDotVisitor::Visit(
  language::NameVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"" + vertex.name() + "\"];\n"
      );
      break;
    }
    case ConnectingAst: {
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      addUseVertices(vertex);
      break;
    }
  }
}



void AstDotVisitor::Visit(
  language::ScriptVertex& vertex)
{
  setScript(UnicodeString(
    "digraph G {\n"
    "ordering=out;\n"
    "rankdir=TB;\n"
  ));

  setMode(Declaring);
  // TODO Use script name.
  addScript(
    UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
    " [label=\"Script\"];\n");

  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex> statementVertex,
    vertex.statements()) {
    statementVertex->Accept(*this);
  }

  setMode(ConnectingAst);
  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex> statementVertex,
    vertex.statements()) {
    addAstVertex(vertex, *statementVertex);
    statementVertex->Accept(*this);
  }

  if(_modes & ConnectingCfg) {
    setMode(ConnectingCfg);
    addCfgVertices(vertex);
    BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
      statementVertex, vertex.statements()) {
      statementVertex->Accept(*this);
    }
  }

  if(_modes & ConnectingUses) {
    setMode(ConnectingUses);
    BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
      statementVertex, vertex.statements()) {
      statementVertex->Accept(*this);
    }
  }

  addScript("}\n");
}



void AstDotVisitor::Visit(
  language::StringVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"\\\"" + vertex.value() + "\\\"\", shape=box];\n"
      );
      break;
    }
    case ConnectingAst: {
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }
}



void AstDotVisitor::Visit(
  language::WhileVertex& vertex)
{
  switch(_mode) {
    case Declaring: {
      addScript(
        UnicodeString((boost::format("\"%1%\"") % &vertex).str().c_str()) +
        " [label=\"While\", shape=diamond];\n"
      );
      break;
    }
    case ConnectingAst: {
      addAstVertex(vertex, *vertex.condition());
      BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
        statementVertex, vertex.trueStatements()) {
        addAstVertex(vertex, *statementVertex);
      }
      BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
        statementVertex, vertex.falseStatements()) {
        addAstVertex(vertex, *statementVertex);
      }
      break;
    }
    case ConnectingCfg: {
      addCfgVertices(vertex);
      break;
    }
    case ConnectingUses: {
      break;
    }
  }

  vertex.condition()->Accept(*this);
  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
    statementVertex, vertex.trueStatements()) {
    statementVertex->Accept(*this);
  }
  BOOST_FOREACH(boost::shared_ptr<language::StatementVertex>
    statementVertex, vertex.falseStatements()) {
    statementVertex->Accept(*this);
  }
}

} // namespace ranally

