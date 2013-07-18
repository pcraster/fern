#include "ranally/ast/core/syntax_vertex.h"
// #include "ranally/ast/copy_visitor.h"


namespace ranally {

SyntaxVertex::SyntaxVertex()

    : Loki::BaseVisitable<>(),
      _line(0),
      _col(0),
      _successors()

{
}


SyntaxVertex::SyntaxVertex(
    int line_nr,
    int col_id)

    : Loki::BaseVisitable<>(),
      _line(line_nr),
      _col(col_id),
      _successors()

{
}


// SyntaxVertex::SyntaxVertex(
//   SyntaxVertex const& other)
// 
//   : Loki::BaseVisitable<>(other),
//     _line(other._line),
//     _col(other._col)
// 
// {
//   // BOOST_FOREACH(SyntaxVertex* vertex, other._successors) {
//   //   CopyVisitor visitor;
//   //   vertex->Accept(visitor);
//   //   _successors.push_back(visitor.vertex());
//   // }
// 
//   CopyVisitor visitor;
//   other.Accept(visitor);
//   _successors = visitor.syntax_vertices();
// }



void SyntaxVertex::set_position(
    int line_nr,
    int col_id)
{
    _line = line_nr;
    _col = col_id;
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


bool SyntaxVertex::has_successor() const
{
    return !_successors.empty();
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
  \sa        set_successor(SyntaxVertex*)

  Prefer set_successor(SyntaxVertex*) over this method.
*/
void SyntaxVertex::add_successor(
    SyntaxVertex* successor)
{
    assert(successor);
    _successors.push_back(successor);
}


//! Adds a successor to the control flow graph of this vertex.
/*!
  \param     successor Successor to set.
  \sa        add_successor(SyntaxVertex*)

  Prefer this method over add_successor(SyntaxVertex*) because it checks
  whether a successor is already set or not. Most of the times, a vertex
  has a single successor.  Only call add_successor for those special
  cases that a vertex can have more than one successor (eg: IfVertex).
*/
void SyntaxVertex::set_successor(
    SyntaxVertex* successor)
{
    assert(_successors.empty());
    add_successor(successor);
}

} // namespace ranally
