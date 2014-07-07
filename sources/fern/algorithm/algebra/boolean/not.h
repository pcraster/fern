#pragma once
#include "fern/core/assert.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/boolean/detail/not.h"


namespace fern {
namespace not_ {

} // namespace not


namespace algebra {

template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value,
    class Result
>
void not_(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    FERN_STATIC_ASSERT(std::is_same, value_type<Value>, bool)
    FERN_STATIC_ASSERT(std::is_same, value_type<Result>, bool)

    not_::detail::not_<>(input_no_data_policy,
        output_no_data_policy, execution_policy, value, result);
}


template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value,
    class Result
>
void not_(
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    OutputNoDataPolicy output_no_data_policy;
    not_<>(InputNoDataPolicy(), output_no_data_policy,
        execution_policy, value, result);
}


template<
    class ExecutionPolicy,
    class Value,
    class Result
>
void not_(
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    using InputNoDataPolicy = SkipNoData;
    using OutputNoDataPolicy = DontMarkNoData;

    OutputNoDataPolicy output_no_data_policy;
    not_<>(InputNoDataPolicy(),
        output_no_data_policy, execution_policy, value, result);
}

} // namespace algebra
} // namespace fern