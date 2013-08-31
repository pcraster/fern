#include "geoneric/operation/core/parameter.h"
#include <cassert>


namespace geoneric {

//!
/*!
  \tparam    .
  \param     .
  \return    .
  \exception .
  \warning   .
  \sa        .

  The \a data_types argument passed in can not be DataTypes::UNKNOWN. The
  \a value_types argument passed in can be ValueTypes::UNKNOWN, in which case
  the value type is taken to be not relevant. This is the case for operations
  that work on feature domains and not on the attributes, like an operation
  that counts the number of feature elements.
*/
Parameter::Parameter(
    String const& name,
    String const& description,
    DataTypes data_types,
    ValueTypes value_types)

    : _name(name),
      _description(description),
      _data_types(data_types),
      _value_types(value_types)

{
    assert(!_name.is_empty());
    assert(!_description.is_empty());
    assert(_data_types != DataTypes::UNKNOWN);
}


Parameter::Parameter(
    Parameter const& other)

    : _name(other._name),
      _description(other._description),
      _data_types(other._data_types),
      _value_types(other._value_types)

{
}


Parameter& Parameter::operator=(
    Parameter const& other)
{
    if(&other != this) {
        _name = other._name;
        _description = other._description;
        _data_types = other._data_types;
        _value_types = other._value_types;
    }

    return *this;
}


Parameter::~Parameter()
{
}


String const& Parameter::name() const
{
    return _name;
}


String const& Parameter::description() const
{
    return _description;
}


DataTypes Parameter::data_types() const
{
    return _data_types;
}


ValueTypes Parameter::value_types() const
{
    return _value_types;
}

} // namespace geoneric
