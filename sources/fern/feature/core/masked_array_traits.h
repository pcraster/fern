#pragma once
#include "fern/feature/core/array_traits.h"
#include "fern/feature/core/masked_array.h"


namespace fern {

template<
    class T,
    size_t nr_dimensions>
struct ArgumentTraits<
    MaskedArray<T, nr_dimensions>>
{

    typedef typename detail::dispatch::ArrayCategoryTag<T, nr_dimensions>::type
        argument_category;

    template<
        class U>
    struct Collection
    {
        typedef MaskedArray<U, nr_dimensions> type;
    };

    typedef T value_type;

    static bool const is_masking = false;

};

} // namespace fern