#pragma once
#include <cassert>
#include "fern/core/argument_traits.h"
#include "fern/feature/core/array.h"
#include "fern/feature/core/array_reference.h"


namespace fern {
namespace detail {
namespace dispatch {

template<
    class T,
    size_t nr_dimensions>
struct ArrayReferenceCategoryTag
{
};


#define ARRAY_CATEGORY_TAG(                         \
    nr_dimensions)                                  \
template<                                           \
    class T>                                        \
struct ArrayReferenceCategoryTag<T, nr_dimensions>  \
{                                                   \
                                                    \
    using type = array_##nr_dimensions##d_tag;      \
                                                    \
};

ARRAY_CATEGORY_TAG(1)
ARRAY_CATEGORY_TAG(2)
ARRAY_CATEGORY_TAG(3)

#undef ARRAY_CATEGORY_TAG

} // namespace dispatch
} // namespace detail


// template<
//     class T,
//     size_t nr_dimensions>
// struct ArgumentTraits<
//     View<T, nr_dimensions>>
// {
// 
//     using argument_category = typename detail::dispatch::ArrayReferenceCategoryTag<T, nr_dimensions>::type;
// 
//     template<
//         class U>
//     struct Collection
//     {
//         using type = ArrayReference<T, nr_dimensions>;
//     };
// 
//     using value_type = T;
// 
// };


template<
    class T,
    size_t nr_dimensions>
struct ArgumentTraits<
    ArrayReference<T, nr_dimensions>>
{

    using argument_category = typename
        detail::dispatch::ArrayReferenceCategoryTag<T, nr_dimensions>::type;

    /// template<
    ///     class U>
    /// struct Collection
    /// {
    ///     using type = ArrayReference<U, nr_dimensions>;
    /// };

    /// template<
    ///     class U>
    /// struct Clone
    /// {
    ///     using type = Array<U, nr_dimensions>;
    /// };

    using value_type = T;

    using reference = T&;

    using const_reference = T const&;

    static bool const is_masking = false;

    static size_t const rank = nr_dimensions;

};


template<
    class T,
    size_t nr_dimensions>
inline size_t size(
    ArrayReference<T, nr_dimensions> const& array)
{
    return array.num_elements();
}


template<
    class T,
    size_t nr_dimensions>
inline size_t size(
    ArrayReference<T, nr_dimensions> const& array,
    size_t dimension)
{
    assert(dimension < array.num_dimensions());
    return array.shape()[dimension];
}


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 1>>::const_reference get(
    ArrayReference<T, 1> const& array,
    size_t index)
{
    assert(index < array.shape()[0]);
    return array[index];
}


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 1>>::reference get(
    ArrayReference<T, 1>& array,
    size_t index)
{
    assert(index < array.shape()[0]);
    return array[index];
}


/// template<
///     class U,
///     class V>
/// inline Array<U, 1> clone(
///     ArrayReference<V, 1> const& array)
/// {
///     return std::move(Array<U, 1>(extents[array.shape()[0]]));
/// }
/// 
/// 
/// template<
///     class U,
///     class V>
/// inline Array<U, 1> clone(
///     ArrayReference<V, 1> const& array,
///     U const& value)
/// {
///     return std::move(Array<U, 1>(extents[array.shape()[0]], value));
/// }


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 2>>::const_reference get(
    ArrayReference<T, 2> const& array,
    size_t index1,
    size_t index2)
{
    assert(index1 < array.shape()[0]);
    assert(index2 < array.shape()[1]);
    return array[index1][index2];
}


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 2>>::reference get(
    ArrayReference<T, 2>& array,
    size_t index1,
    size_t index2)
{
    assert(index1 < array.shape()[0]);
    assert(index2 < array.shape()[1]);
    return array[index1][index2];
}


/// template<
///     class U,
///     class V>
/// inline Array<U, 2> clone(
///     ArrayReference<V, 2> const& array)
/// {
///     return std::move(Array<U, 2>(extents[array.shape()[0]][array.shape()[1]]));
/// }
/// 
/// 
/// template<
///     class U,
///     class V>
/// inline Array<U, 2> clone(
///     ArrayReference<V, 2> const& array,
///     U const& value)
/// {
///     return std::move(Array<U, 2>(extents[array.shape()[0]][array.shape()[1]],
///         value));
/// }


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 3>>::const_reference get(
    ArrayReference<T, 3> const& array,
    size_t index1,
    size_t index2,
    size_t index3)
{
    assert(index1 < array.shape()[0]);
    assert(index2 < array.shape()[1]);
    assert(index3 < array.shape()[2]);
    return array[index1][index2][index3];
}


template<
    class T>
inline typename ArgumentTraits<ArrayReference<T, 3>>::const_reference get(
    ArrayReference<T, 3>& array,
    size_t index1,
    size_t index2,
    size_t index3)
{
    assert(index1 < array.shape()[0]);
    assert(index2 < array.shape()[1]);
    assert(index3 < array.shape()[2]);
    return array[index1][index2][index3];
}


/// template<
///     class U,
///     class V>
/// inline Array<U, 3> clone(
///     ArrayReference<V, 3> const& array)
/// {
///     return std::move(Array<U, 3>(
///         extents[array.shape()[0]][array.shape()[1]][array.shape()[2]]));
/// }
/// 
/// 
/// template<
///     class U,
///     class V>
/// inline Array<U, 3> clone(
///     ArrayReference<V, 3> const& array,
///     U const& value)
/// {
///     return std::move(Array<U, 3>(
///         extents[array.shape()[0]][array.shape()[1]][array.shape()[2]],
///         value));
/// }

} // namespace fern