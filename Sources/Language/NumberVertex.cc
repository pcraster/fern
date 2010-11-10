#include "NumberVertex.h"



namespace ranally {

template<typename T>
NumberVertex<T>::NumberVertex(
  T value)

  : ExpressionVertex(),
    _value(value)

{
}



template<typename T>
NumberVertex<T>::NumberVertex(
  int lineNr,
  int colId,
  T value)

  : ExpressionVertex(lineNr, colId),
    _value(value)

{
}



template<typename T>
NumberVertex<T>::~NumberVertex()
{
}



template<typename T>
T NumberVertex<T>::value() const
{
  return _value;
}



template class NumberVertex<int8_t>;
template class NumberVertex<int16_t>;
template class NumberVertex<int32_t>;
template class NumberVertex<int64_t>;
template class NumberVertex<uint8_t>;
template class NumberVertex<uint16_t>;
template class NumberVertex<uint32_t>;
template class NumberVertex<uint64_t>;
template class NumberVertex<float>;
template class NumberVertex<double>;

} // namespace ranally

