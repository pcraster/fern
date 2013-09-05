#pragma once
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/const_iterator.hpp>
#include "geoneric/data_traits.h"


namespace geoneric {
namespace algorithm {
namespace detail {

template<
    typename Argument1,
    typename Argument2,
    typename Result>
inline void plus(
    RangeTag /* tag1 */,
    Argument1 const& argument1,
    ScalarTag /* tag2 */,
    Argument2 const& argument2,
    Result& result)
{
    typename boost::range_const_iterator<Argument1>::type argument1_it =
        boost::begin(argument1);
    typename boost::range_const_iterator<Argument1>::type const end1 =
        boost::end(argument1);
    typename boost::range_iterator<Result>::type result_it =
        boost::begin(result);

    for(; argument1_it != end1; ++argument1_it, ++result_it) {
        *result_it = *argument1_it + argument2;
    }
}


template<
    typename Argument1,
    typename Argument2,
    typename Result>
inline void plus(
    ScalarTag tag1,
    Argument1 argument1,
    RangeTag tag2,
    Argument2 const& argument2,
    Result& result)
{
    // Reorder the arguments and call the other algorithm.
    plus(tag2, argument2, tag1, argument1, result);
}


template<
    typename Argument1,
    typename Argument2,
    typename Result>
inline void plus(
    ScalarTag /* tag1 */,
    Argument1 argument1,
    ScalarTag /* tag2 */,
    Argument2 argument2,
    Result& result)
{
    result = argument1 + argument2;
}


template<
    typename Argument1,
    typename Argument2,
    typename Result>
inline void plus(
    RangeTag /* tag1 */,
    Argument1 const& argument1,
    RangeTag /* tag2 */,
    Argument2 const& argument2,
    Result& result)
{
  typename boost::range_const_iterator<Argument1>::type argument1_it =
      boost::begin(argument1);
  typename boost::range_const_iterator<Argument2>::type argument2_it =
      boost::begin(argument2);
  typename boost::range_const_iterator<Argument1>::type const end1 =
      boost::end(argument1);
  typename boost::range_iterator<Result>::type result_it =
      boost::begin(result);

  for(; argument1_it != end1; ++argument1_it, ++argument2_it, ++result_it) {
      *result_it = *argument1_it + *argument2_it;
  }
}

} // namespace detail


template<
    typename Argument1,
    typename Argument2,
    typename Result>
inline void plus(
    Argument1 const& argument1,
    Argument2 const& argument2,
    Result& result)
{
    typedef typename DataTraits<Argument1>::DataCategory category1;
    typedef typename DataTraits<Argument2>::DataCategory category2;
    detail::plus(category1(), argument1, category2(), argument2, result);
}

} // namespace algorithm
} // namespace geoneric