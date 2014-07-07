#pragma once
#include <utility>
#include "fern/core/argument_categories.h"
#include "fern/core/argument_traits.h"
#include "fern/core/assert.h"
#include "fern/core/collection_traits.h"
#include "fern/algorithm/core/index_ranges.h"


namespace fern {
namespace sum {
namespace detail {
namespace dispatch {

template<class Values, class Result,
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ArgumentCollectionCategory>
class Sum
{
};


template<class Values, class Result,
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy>
class Sum<Values, Result,
        OutOfRangePolicy,
        InputNoDataPolicy,
        OutputNoDataPolicy,
        array_0d_tag>:

    public OutOfRangePolicy<value_type<Values>, value_type<Values>,
        value_type<Result>>,
    public InputNoDataPolicy,
    public OutputNoDataPolicy

{

public:

    // This means that there cannot be overflow/underflow, so we don't need
    // to check for it.
    FERN_STATIC_ASSERT(std::is_same, Values, Result)

    Sum()
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(),
          OutputNoDataPolicy()
    {
    }

    Sum(
        InputNoDataPolicy&& input_no_data_policy,
        OutputNoDataPolicy&& output_no_data_policy)
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(std::forward<InputNoDataPolicy>(
              input_no_data_policy)),
          OutputNoDataPolicy(std::forward<OutputNoDataPolicy>(
              output_no_data_policy))
    {
    }

    // constant
    inline void calculate(
        Values const& values,
        Result& result)
    {
        using INDP = InputNoDataPolicy;
        using ONDP = OutputNoDataPolicy;

        if(!INDP::is_no_data()) {
            fern::get(result) = fern::get(values);
        }
        else {
            ONDP::mark_as_no_data();
        }
    }

private:

};


template<class Values, class Result,
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy>
class Sum<Values, Result,
        OutOfRangePolicy,
        InputNoDataPolicy,
        OutputNoDataPolicy,
        array_1d_tag>:

    public OutOfRangePolicy<value_type<Values>, value_type<Values>,
        value_type<Result>>,
    public InputNoDataPolicy,
    public OutputNoDataPolicy

{

public:

    Sum()
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(),
          OutputNoDataPolicy()
    {
    }

    Sum(
        InputNoDataPolicy&& input_no_data_policy,
        OutputNoDataPolicy&& output_no_data_policy)
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(std::forward<InputNoDataPolicy>(
              input_no_data_policy)),
          OutputNoDataPolicy(std::forward<OutputNoDataPolicy>(
              output_no_data_policy))
    {
    }

    // 1d array
    inline void calculate(
        Values const& values,
        Result& result)
    {
        size_t const size = fern::size(values);

        auto ranges = IndexRanges<1>{
            IndexRange(0, size)
        };

        calculate(ranges, values, result);
    }

    template<
        class Indices>
    inline void calculate(
        Indices const& indices,
        Values const& values,
        Result& result)
    {
        using INDP = InputNoDataPolicy;
        using ONDP = OutputNoDataPolicy;
        using OORP = OutOfRangePolicy<value_type<Values>, value_type<Values>,
            value_type<Result>>;

        size_t const begin = indices[0].begin();
        size_t const end = indices[0].end();
        bool data_seen{false};

        if(begin < end) {
            value_type<Values> value;
            value_type<Result> tmp_result{0};
            fern::get(result) = tmp_result;

            for(size_t i = begin; i < end; ++i) {

                if(!INDP::is_no_data(i)) {

                    value = fern::get(values, i);
                    tmp_result += value;

                    // lhs, rhs, lhs + rhs
                    if(!OORP::within_range(fern::get(result), value,
                            tmp_result)) {
                        ONDP::mark_as_no_data();
                        break;
                    }

                    fern::get(result) = tmp_result;

                    data_seen = true;
                }
            }
        }

        if(!data_seen) {
            ONDP::mark_as_no_data();
        }
    }

private:

};


template<class Values, class Result,
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy>
class Sum<Values, Result,
        OutOfRangePolicy,
        InputNoDataPolicy,
        OutputNoDataPolicy,
        array_2d_tag>:

    public OutOfRangePolicy<value_type<Values>, value_type<Values>,
        value_type<Result>>,
    public InputNoDataPolicy,
    public OutputNoDataPolicy

{

public:

    Sum()
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(),
          OutputNoDataPolicy()
    {
    }

    Sum(
        InputNoDataPolicy&& input_no_data_policy,
        OutputNoDataPolicy&& output_no_data_policy)
        : OutOfRangePolicy<value_type<Values>, value_type<Values>,
              value_type<Result>>(),
          InputNoDataPolicy(std::forward<InputNoDataPolicy>(
              input_no_data_policy)),
          OutputNoDataPolicy(std::forward<OutputNoDataPolicy>(
              output_no_data_policy))
    {
    }

    // 2d array
    inline void calculate(
        Values const& values,
        Result& result)
    {
        size_t const size1 = fern::size(values, 0);
        size_t const size2 = fern::size(values, 1);

        auto ranges = IndexRanges<2>{
            IndexRange(0, size1),
            IndexRange(0, size2)
        };

        calculate(ranges, values, result);
    }

    template<
        class Indices>
    inline void calculate(
        Indices const& indices,
        Values const& values,
        Result& result)
    {

        /// Instead of three nested loops, a single loop is sufficient to run
        /// from index 0 to (width*height*depth).
        ///
        /// long index = 0;
        /// const long indexend = (long)width * (long)height * (long)depth;
        /// while(index != indexend)
        ///     sum += data[index++];

        using INDP = InputNoDataPolicy;
        using ONDP = OutputNoDataPolicy;
        using OORP = OutOfRangePolicy<value_type<Values>, value_type<Values>,
            value_type<Result>>;

        size_t const begin1 = indices[0].begin();
        size_t const end1 = indices[0].end();
        size_t const begin2 = indices[1].begin();
        size_t const end2 = indices[1].end();
        bool data_seen{false};

        if(begin1 < end1 && begin2 < end2) {

            value_type<Values> value;
            value_type<Result> tmp_result{0};
            fern::get(result) = tmp_result;

            for(size_t i = begin1; i < end1; ++i) {
                for(size_t j = begin2; j < end2; ++j) {

                    if(!INDP::is_no_data(i, j)) {

                        value = fern::get(values, i, j);
                        tmp_result += value;

                        // lhs, rhs, lhs + rhs
                        if(!OORP::within_range(fern::get(result),
                                value, tmp_result)) {
                            ONDP::mark_as_no_data();
                            break;
                        }

                        fern::get(result) = tmp_result;

                        data_seen = true;
                    }
                }

                // TODO We need to break again! The inner break breaks only
                //      the inner for loop.
            }
        }

        if(!data_seen) {
            ONDP::mark_as_no_data();
        }
    }

private:

};

} // namespace dispatch
} // namespace detail
} // namespace sum
} // namespace fern