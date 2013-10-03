#include "geoneric/operation/std/abs.h"
#include "geoneric/core/value_type_traits.h"
#include "geoneric/feature/core/attributes.h"
#include "geoneric/operation/core/attribute_argument.h"


namespace geoneric {

template<
    typename T>
std::vector<std::shared_ptr<Argument>> abs(
        ConstantAttribute<T> const& attribute)
{
    T result = std::abs(attribute.values().value());

    return std::vector<std::shared_ptr<Argument>>({
        std::shared_ptr<Argument>(new AttributeArgument(
            std::make_shared<ConstantAttribute<T>>(result)))
    });
}


template<
    typename T>
std::vector<std::shared_ptr<Argument>> abs(
        FieldAttribute<T> const& attribute)
{
    FieldAttributePtr<T> result(new FieldAttribute<T>);

    for(auto const& pair: attribute.domain()) {
        FieldValue<T> const& source_array(*attribute.values().value(
            pair.first));
        assert(source_array.size() > 0);
        FieldValuePtr<T> destination_array_ptr(new FieldValue<T>(
            extents[source_array.size()][source_array[0].size()]));
        FieldValue<T>& destination_array(*destination_array_ptr);

        destination_array.mask() = source_array.mask();

        for(size_t row = 0; row < source_array.shape()[0]; ++row) {
            for(size_t col = 0; col < source_array.shape()[1]; ++col) {
                if(!source_array.mask()[row][col]) {
                    destination_array[row][col] = std::abs(
                        source_array[row][col]);
                }
            }
        }

        // TODO perform abs...
        // - abs with a single value.
        // - abs with two arrays, without a mask
        // - ...
        // - ...

        // TODO share or copy domain...

        result->add(pair.second, destination_array_ptr);
    }

    return std::vector<std::shared_ptr<Argument>>({
        std::shared_ptr<Argument>(new AttributeArgument(
            result))
    });
}


Abs::Abs()

    : Operation("abs",
          "Calculate the absolute value of the argument and return the result.",
          {
              Parameter("Input argument",
                  "Argument to calculate absolute value of.",
                  DataTypes::CONSTANT | DataTypes::STATIC_FIELD,
                  ValueTypes::NUMBER)
          },
          {
              Result("Result value",
                  "Absolute value of the input argument.",
                  DataTypes::CONSTANT | DataTypes::STATIC_FIELD,
                  ValueTypes::NUMBER)
          }
      )

{
}


#define CONSTANT_CASE(                                                         \
        value_type)                                                            \
    case value_type: {                                                         \
        typedef ValueTypeTraits<value_type>::type type;                        \
        result = abs<type>(dynamic_cast<ConstantAttribute<type> const&>(       \
            attribute));                                                       \
        break;                                                                 \
    }

#define FIELD_CASE(                                                            \
        value_type)                                                            \
    case value_type: {                                                         \
        typedef ValueTypeTraits<value_type>::type type;                        \
        result = abs<type>(dynamic_cast<FieldAttribute<type> const&>(          \
            attribute));                                                       \
        break;                                                                 \
    }


std::vector<std::shared_ptr<Argument>> Abs::execute(
    std::vector<std::shared_ptr<Argument>> const& arguments) const
{
    assert(arguments.size() == 1);
    assert(arguments[0]->argument_type() == ArgumentType::AT_ATTRIBUTE);

    AttributeArgument const& attribute_argument(
        *std::dynamic_pointer_cast<AttributeArgument>(arguments[0]));
    Attribute const& attribute(*attribute_argument.attribute());
    std::vector<std::shared_ptr<Argument>> result;

    switch(attribute_argument.data_type()) {
        case DataType::DT_CONSTANT: {
            switch(attribute_argument.value_type()) {
                CONSTANT_CASE(ValueType::VT_UINT8)
                CONSTANT_CASE(ValueType::VT_INT8)
                CONSTANT_CASE(ValueType::VT_UINT16)
                CONSTANT_CASE(ValueType::VT_INT16)
                CONSTANT_CASE(ValueType::VT_UINT32)
                CONSTANT_CASE(ValueType::VT_INT32)
                CONSTANT_CASE(ValueType::VT_UINT64)
                CONSTANT_CASE(ValueType::VT_INT64)
                CONSTANT_CASE(ValueType::VT_FLOAT32)
                CONSTANT_CASE(ValueType::VT_FLOAT64)
                case ValueType::VT_STRING: {
                    assert(false);
                    break;
                }
            }
            break;
        }
        case DataType::DT_STATIC_FIELD: {
            switch(attribute_argument.value_type()) {
                FIELD_CASE(ValueType::VT_UINT8)
                FIELD_CASE(ValueType::VT_INT8)
                FIELD_CASE(ValueType::VT_UINT16)
                FIELD_CASE(ValueType::VT_INT16)
                FIELD_CASE(ValueType::VT_UINT32)
                FIELD_CASE(ValueType::VT_INT32)
                FIELD_CASE(ValueType::VT_UINT64)
                FIELD_CASE(ValueType::VT_INT64)
                FIELD_CASE(ValueType::VT_FLOAT32)
                FIELD_CASE(ValueType::VT_FLOAT64)
                case ValueType::VT_STRING: {
                    assert(false);
                    break;
                }
            }
            break;
        }
        // case DataType::DT_POINT: {
        //     assert(false);
        //     break;
        // }
        // case DataType::DT_LINE: {
        //     assert(false);
        //     break;
        // }
        // case DataType::DT_POLYGON: {
        //     assert(false);
        //     break;
        // }
    }

    return result;
}

} // namespace geoneric
