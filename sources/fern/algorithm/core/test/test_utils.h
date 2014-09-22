#pragma once
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "fern/feature/core/masked_constant.h"
#include "fern/feature/core/masked_raster.h"
#include "fern/algorithm/algebra/elementary/equal.h"
#include "fern/algorithm/statistic/count.h"


namespace fern {

template<
    class ExecutionPolicy,
    class Value>
bool compare(
    ExecutionPolicy const& execution_policy,
    Value const& value1,
    Value const& value2)
{
    auto equal_result = clone<int>(value1, 0);

    algebra::equal(execution_policy, value1, value2, equal_result);

    size_t nr_equal_values;

    statistic::count(execution_policy, equal_result, 1, nr_equal_values);

    BOOST_CHECK_EQUAL(nr_equal_values, size(value1));

    return nr_equal_values == size(value1);
}


template<
    class ExecutionPolicy,
    class Value,
    size_t nr_dimensions>
bool compare(
    ExecutionPolicy const& execution_policy,
    MaskedArray<Value, nr_dimensions> const& value1,
    MaskedArray<Value, nr_dimensions> const& value2)
{
    bool values_are_the_same = compare(execution_policy,
            dynamic_cast<Array<Value, nr_dimensions> const&>(value1),
            dynamic_cast<Array<Value, nr_dimensions> const&>(value2));
    bool mask_is_the_same = compare(execution_policy,
            value1.mask(),
            value2.mask());

    BOOST_CHECK(values_are_the_same);
    BOOST_CHECK(mask_is_the_same);

    return values_are_the_same && mask_is_the_same;
}


template<
    class ExecutionPolicy,
    class Value,
    size_t nr_dimensions>
bool compare(
    ExecutionPolicy const& execution_policy,
    MaskedRaster<Value, nr_dimensions> const& value1,
    MaskedRaster<Value, nr_dimensions> const& value2)
{
    bool values_are_the_same = compare(execution_policy,
            dynamic_cast<Array<Value, nr_dimensions> const&>(value1),
            dynamic_cast<Array<Value, nr_dimensions> const&>(value2));
    bool mask_is_the_same = compare(execution_policy,
            value1.mask(),
            value2.mask());

    BOOST_CHECK(values_are_the_same);
    BOOST_CHECK(mask_is_the_same);

    return values_are_the_same && mask_is_the_same;
}


template<
    class T>
inline std::ostream& operator<<(
    std::ostream& stream,
    MaskedConstant<T> const& constant)
{
    stream << constant.value() << (constant.mask() ? "(masked)" : "");
    return stream;
}


template<
    class T,
    size_t nr_dimensions>
inline std::ostream& operator<<(
    std::ostream& stream,
    MaskedRaster<T, nr_dimensions> const& raster)
{
    for(size_t r = 0; r < fern::size(raster, 0); ++r) {
        for(size_t c = 0; c < fern::size(raster, 1); ++c) {
            stream
                << (raster.mask()[r][c] ? "(" : " ")
                << std::setw(5) << raster[r][c]
                << (raster.mask()[r][c] ? ")" : " ")
                << " ";
        }
        stream << "\n";
    }

    return stream;
}

}
