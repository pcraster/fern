#pragma once
#include <cstddef>
#include "fern/core/data_traits.h"


namespace fern {

// Declarations of functions that are used in the implementation of operations.
// These are not defined. For each collection type they need to be implemented.
// See also data_traits/vector.h, data_traits/array.h, ...


/*!
    @ingroup    fern_data_customization_point_group
    @brief      Return the number of elements in @a collection.
*/
template<
    class T>
size_t             size                (T const& collection);


/*!
    @ingroup    fern_data_customization_point_group
    @brief      Convert 2D indices to linear index.
*/
template<
    class T>
size_t             index               (T const& collection,
                                        size_t index1,
                                        size_t index2);

/*!
    @ingroup    fern_data_customization_point_group
    @brief      Return the value type instance of the data type instance
                passed in.
*/
template<
    class T>
T const&           get                 (T const&);


/*!
    @ingroup    fern_data_customization_point_group
    @overload
*/
template<
    class T>
T&                 get                 (T&);


/*!
    @ingroup    fern_data_customization_point_group
    @brief      Return the value type instance possitioned at @a index
                in the data type instance passed in.
*/
template<
    class T>
T const&           get                 (T const& collection,
                                        size_t index);

/*!
    @ingroup    fern_data_customization_point_group
    @overload
*/
template<
    class T>
T&                 get                 (T& collection,
                                        size_t index);


//! Clone a collection, using a different value type.
/*!
    @ingroup    fern_data_customization_point_group
    @tparam    ValueType Value type to use for the result.
    @tparam    Value Value whose collection type is used for the result.
    @param     value Instance whose properties are used for the result.
    @return    New collection with value type \a ValueType and the same
               collection properties as \a value.
    @warning   The individual values in \a value are not copied to the result.

    The result of calling this function is useful as an output parameter when
    calling an algorithm.

    For example, result of calling

    @code
    auto result = clone<double>(std::vector<int>{1, 2, 3, 4, 5});
    @endcode

    is a `std::vector<double>` with the same lenght. All values are initialized
    by the `std::vector` constructor used in the implementation of clone for
    `std::vector`.
*/
template<
    class ValueType,
    class Value>
CloneT<Value, ValueType>
                   clone               (Value const& value);


/*!
    @ingroup    fern_data_customization_point_group
    @overload

    The @a initial_value is used to initialize the values in the result.
*/
template<
    class ValueType,
    class Value>
CloneT<Value, ValueType>
                   clone               (Value const& value,
                                        ValueType const& initial_value);

} // namespace fern
