#pragma once
#include "fern/core/assert.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/elementary/detail/less.h"


namespace fern {
namespace less {

} // namespace less


namespace algebra {

template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void less(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value1>, bool)
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value2>, bool)
    FERN_STATIC_ASSERT(std::is_same, value_type<Result>, bool)

    less::detail::less<>(input_no_data_policy, output_no_data_policy,
        execution_policy, value1, value2, result);
}


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void less(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    OutputNoDataPolicy output_no_data_policy;
    less<>(InputNoDataPolicy(),
        output_no_data_policy, execution_policy, value1, value2, result);
}


template<
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void less(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    using InputNoDataPolicy = SkipNoData;
    using OutputNoDataPolicy = DontMarkNoData;

    OutputNoDataPolicy output_no_data_policy;
    less<>(InputNoDataPolicy(), output_no_data_policy, execution_policy,
        value1, value2, result);
}

} // namespace algebra
} // namespace fern