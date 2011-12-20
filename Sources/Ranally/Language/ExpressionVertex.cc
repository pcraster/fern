#include "Ranally/Language/ExpressionVertex.h"



namespace ranally {
namespace language {

ExpressionVertex::ExpressionVertex(
  UnicodeString const& name)

  : StatementVertex(),
    _name(name) // ,
    // _dataType(operation::DT_UNKNOWN),
    // _valueType(operation::VT_UNKNOWN)

{
}



ExpressionVertex::ExpressionVertex(
  int lineNr,
  int colId,
  UnicodeString const& name)

  : StatementVertex(lineNr, colId),
    _name(name)

{
}



ExpressionVertex::~ExpressionVertex()
{
}



UnicodeString const& ExpressionVertex::name() const
{
  return _name;
}



// void ExpressionVertex::setDataType(
//   operation::DataType dataType)
// {
//   _dataType = dataType;
// }
// 
// 
// 
// operation::DataType ExpressionVertex::dataType() const
// {
//   return _dataType;
// }
// 
// 
// 
// void ExpressionVertex::setValueType(
//   operation::ValueType valueType)
// {
//   _valueType = valueType;
// }
// 
// 
// 
// operation::ValueType ExpressionVertex::valueType() const
// {
//   return _valueType;
// }



void ExpressionVertex::addResult(
  operation::DataType dataType,
  operation::ValueType valueType)
{
  _resultTypes.push_back(boost::make_tuple(dataType, valueType));
}



operation::DataType ExpressionVertex::dataType(
  size_t index) const
{
  assert(index < _resultTypes.size());
  return _resultTypes[index].get<0>();
}



operation::ValueType ExpressionVertex::valueType(
  size_t index) const
{
  assert(index < _resultTypes.size());
  return _resultTypes[index].get<1>();
}

} // namespace language
} // namespace ranally

