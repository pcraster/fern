#include "Ranally/Language/SyntaxVertex.h"
#include <boost/foreach.hpp>



namespace ranally {
namespace language {

SyntaxVertex::SyntaxVertex()

  : Loki::BaseVisitable<>()

{
}



SyntaxVertex::SyntaxVertex(
  int lineNr,
  int colId)

  : Loki::BaseVisitable<>(),
    _line(lineNr),
    _col(colId)

{
}



SyntaxVertex::SyntaxVertex(
  SyntaxVertex const& other)

  : Loki::BaseVisitable<>(),
    _line(other._line),
    _col(other._col)

{
  assert(false);
  // TODO
  // std::vector<SyntaxVertex*> _successors;

  // TODO Put boost::shared_ptr<SyntaxVertex> in vector.
  // TODO Create CopyVisitor.

  // BOOST_FOREACH(SyntaxVertexPtr const& vertex, _successors) {
  //   // CopyVisitor visitor;
  //   // visitor.accept(*vertex);
  //   // _successors.push_back(visitor.vertex());
  // }
}



SyntaxVertex::~SyntaxVertex()
{
  // The vertices from the control flow graph are for use only.
}



void SyntaxVertex::setPosition(
  int lineNr,
  int colId)
{
  _line = lineNr;
  _col = colId;
}



int SyntaxVertex::line() const
{
  return _line;
}



int SyntaxVertex::col() const
{
  return _col;
}



SyntaxVertex::SyntaxVertices const& SyntaxVertex::successors() const
{
  return _successors;
}



//! Return the successor in the control flow graph of this vertex.
/*!
  \return    Pointer to the successor.
  \warning   It is assumed that this vertex has only one successor.
*/
SyntaxVertex const* SyntaxVertex::successor() const
{
  return successor(0);
}



/*!
  \overload
*/
SyntaxVertex* SyntaxVertex::successor()
{
  return successor(0);
}



//! Return one of the successors in the control flow graph of this vertex.
/*!
  \param     index Index of successor to return.
  \return    Pointer to the successor.
*/
SyntaxVertex const* SyntaxVertex::successor(
  size_type index) const
{
  assert(index < _successors.size());
  return _successors[index];
}



/*!
  \overload
*/
SyntaxVertex* SyntaxVertex::successor(
  size_type index)
{
  assert(index < _successors.size());
  return _successors[index];
}



//! Adds a successor to the control flow graph of this vertex.
/*!
  \param     successor Successor to set.
  \sa        setSuccessor(SyntaxVertex*)
*/
void SyntaxVertex::addSuccessor(
  SyntaxVertex* successor)
{
  assert(successor);
  _successors.push_back(successor);
}

} // namespace language
} // namespace ranally

