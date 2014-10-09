#pragma once
#include "fern/core/argument_traits.h"
#include "fern/feature/core/point.h"


namespace fern {

template<
    typename T,
    size_t nr_dimensions,
    typename CoordinateSystem>
struct ArgumentTraits<
    Point<T, nr_dimensions, CoordinateSystem>>
{

    using value_type = T;

    static size_t const rank = nr_dimensions;

};

} // namespace fern