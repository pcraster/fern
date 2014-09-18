#pragma once
#include "fern/core/argument_traits.h"
#include "fern/core/base_class.h"
#include "fern/core/thread_client.h"
#include "fern/algorithm/core/index_ranges.h"
#include "fern/algorithm/policy/execution_policy.h"


namespace fern {
namespace cover {
namespace detail {

template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
static void cover_0d_0d(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& /* output_no_data_policy */,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    if(!input_no_data_policy.is_no_data()) {
        if(!input_no_data_policy.get<0>().is_no_data()) {
            fern::get(result) = fern::get(value1);
        }
        else {
            assert(!input_no_data_policy.get<1>().is_no_data());
            fern::get(result) = fern::get(value2);
        }
    }
}


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
static void cover_2d_0d(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& /* output_no_data_policy */,
    IndexRanges<2> const& index_ranges,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    for(size_t i = index_ranges[0].begin(); i < index_ranges[0].end(); ++i) {
        for(size_t j = index_ranges[1].begin(); j < index_ranges[1].end();
                ++j) {

            if(!input_no_data_policy.is_no_data(i, j)) {

                if(!input_no_data_policy.get<0>().is_no_data(i, j)) {
                    fern::get(result, i, j) = fern::get(value1, i, j);
                }
                else {
                    assert(!input_no_data_policy.get<1>().is_no_data());
                    fern::get(result, i, j) = fern::get(value2);
                }
            }
        }
    }
}


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
static void cover_2d_2d(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& /* output_no_data_policy */,
    IndexRanges<2> const& index_ranges,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    for(size_t i = index_ranges[0].begin(); i < index_ranges[0].end(); ++i) {
        for(size_t j = index_ranges[1].begin(); j < index_ranges[1].end();
                ++j) {

            if(!input_no_data_policy.is_no_data(i, j)) {

                if(!input_no_data_policy.get<0>().is_no_data(i, j)) {
                    fern::get(result, i, j) = fern::get(value1, i, j);
                }
                else {
                    assert(!input_no_data_policy.get<1>().is_no_data(i, j));
                    fern::get(result, i, j) = fern::get(value2, i, j);
                }
            }
        }
    }
}


namespace dispatch {

template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result,
    class ExecutionPolicy,
    class Value1CollectionCategory,
    class Value2CollectionCategory>
struct CoverByArgumentCategory
{
};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result,
    class ExecutionPolicy>
struct CoverByArgumentCategory<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    ExecutionPolicy,
    array_0d_tag,
    array_0d_tag>
{

    // cover(0d, 0d)
    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        ExecutionPolicy const& /* execution_policy */,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        cover_0d_0d(input_no_data_policy, output_no_data_policy,
            value1, value2, result);
    }

};


// TODO: 1d stuff.


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
struct CoverByArgumentCategory<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    SequentialExecutionPolicy,
    array_2d_tag,
    array_0d_tag>
{

    // cover(2d, 0d)
    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        SequentialExecutionPolicy const& /* execution_policy */,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        assert(fern::size(value1, 0) == fern::size(result, 0));
        assert(fern::size(value1, 1) == fern::size(result, 1));

        cover_2d_0d(input_no_data_policy, output_no_data_policy,
            IndexRanges<2>{
                IndexRange(0, fern::size(result, 0)),
                IndexRange(0, fern::size(result, 1)),
            }, value1, value2, result);
    }

};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
struct CoverByArgumentCategory<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    ParallelExecutionPolicy,
    array_2d_tag,
    array_0d_tag>
{

    // cover(2d, 0d)
    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        ParallelExecutionPolicy const& /* execution_policy */,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        assert(fern::size(value1, 0) == fern::size(result, 0));
        assert(fern::size(value1, 1) == fern::size(result, 1));

        ThreadPool& pool(ThreadClient::pool());
        size_t const size1 = fern::size(result, 0);
        size_t const size2 = fern::size(result, 1);
        std::vector<IndexRanges<2>> ranges = index_ranges(pool.size(),
            size1, size2);
        std::vector<std::future<void>> futures;
        futures.reserve(ranges.size());

        for(auto const& block_range: ranges) {
            auto function = std::bind(
                cover_2d_0d,
                std::cref(input_no_data_policy),
                std::ref(output_no_data_policy), std::cref(block_range),
                std::cref(value1), std::cref(value2), std::ref(result));
            futures.emplace_back(pool.submit(function));
        }

        for(auto& future: futures) {
            future.get();
        }
    }

};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
struct CoverByArgumentCategory<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    SequentialExecutionPolicy,
    array_2d_tag,
    array_2d_tag>
{

    // cover(2d, 2d)
    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        SequentialExecutionPolicy const& /* execution_policy */,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        assert(fern::size(value1, 0) == fern::size(result, 0));
        assert(fern::size(value1, 1) == fern::size(result, 1));
        assert(fern::size(value2, 0) == fern::size(result, 0));
        assert(fern::size(value2, 1) == fern::size(result, 1));

        cover_2d_2d(input_no_data_policy, output_no_data_policy,
            IndexRanges<2>{
                IndexRange(0, fern::size(result, 0)),
                IndexRange(0, fern::size(result, 1)),
            }, value1, value2, result);
    }

};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
struct CoverByArgumentCategory<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    ParallelExecutionPolicy,
    array_2d_tag,
    array_2d_tag>
{

    // cover(2d, 2d)
    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        ParallelExecutionPolicy const& /* execution_policy */,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        assert(fern::size(value1, 0) == fern::size(result, 0));
        assert(fern::size(value1, 1) == fern::size(result, 1));
        assert(fern::size(value2, 0) == fern::size(result, 0));
        assert(fern::size(value2, 1) == fern::size(result, 1));

        ThreadPool& pool(ThreadClient::pool());
        size_t const size1 = fern::size(result, 0);
        size_t const size2 = fern::size(result, 1);
        std::vector<IndexRanges<2>> ranges = index_ranges(pool.size(),
            size1, size2);
        std::vector<std::future<void>> futures;
        futures.reserve(ranges.size());

        for(auto const& block_range: ranges) {
            auto function = std::bind(
                cover_2d_2d,
                std::cref(input_no_data_policy),
                std::ref(output_no_data_policy), std::cref(block_range),
                std::cref(value1), std::cref(value2), std::ref(result));
            futures.emplace_back(pool.submit(function));
        }

        for(auto& future: futures) {
            future.get();
        }
    }

};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result,
    class ExecutionPolicy>
struct CoverByExecutionPolicy
{

    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        ExecutionPolicy const& execution_policy,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        CoverByArgumentCategory<
            InputNoDataPolicy,
            OutputNoDataPolicy,
            Value1,
            Value2,
            Result,
            ExecutionPolicy,
            base_class<argument_category<Value1>, array_2d_tag>,
            base_class<argument_category<Value2>, array_2d_tag>>::apply(
                input_no_data_policy, output_no_data_policy, execution_policy,
                value1, value2, result);
    }

};


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class Value1,
    class Value2,
    class Result>
struct CoverByExecutionPolicy<
    InputNoDataPolicy,
    OutputNoDataPolicy,
    Value1,
    Value2,
    Result,
    ExecutionPolicy>
{

    static void apply(
        InputNoDataPolicy const& input_no_data_policy,
        OutputNoDataPolicy& output_no_data_policy,
        ExecutionPolicy const& execution_policy,
        Value1 const& value1,
        Value2 const& value2,
        Result& result)
    {
        switch(execution_policy.which()) {
            case fern::detail::sequential_execution_policy_id: {
                CoverByArgumentCategory<
                    InputNoDataPolicy,
                    OutputNoDataPolicy,
                    Value1,
                    Value2,
                    Result,
                    SequentialExecutionPolicy,
                    base_class<argument_category<Value1>, array_2d_tag>,
                    base_class<argument_category<Value2>, array_2d_tag>>
                        ::apply(
                        input_no_data_policy, output_no_data_policy,
                        fern::detail::get_policy<SequentialExecutionPolicy>(
                            execution_policy),
                        value1, value2, result);
                break;
            }
            case fern::detail::parallel_execution_policy_id: {
                CoverByArgumentCategory<
                    InputNoDataPolicy,
                    OutputNoDataPolicy,
                    Value1,
                    Value2,
                    Result,
                    ParallelExecutionPolicy,
                    base_class<argument_category<Value1>, array_2d_tag>,
                    base_class<argument_category<Value2>, array_2d_tag>>
                        ::apply(
                            input_no_data_policy, output_no_data_policy,
                            fern::detail::get_policy<ParallelExecutionPolicy>(
                                execution_policy),
                            value1, value2, result);
                break;
            }
        }
    }

};

} // namespace dispatch


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result>
static void cover(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    dispatch::CoverByExecutionPolicy<InputNoDataPolicy, OutputNoDataPolicy,
        Value1, Value2, Result, ExecutionPolicy>::apply(
            input_no_data_policy, output_no_data_policy, execution_policy,
            value1, value2, result);
}

} // namespace detail
} // namespace cover
} // namespace fern