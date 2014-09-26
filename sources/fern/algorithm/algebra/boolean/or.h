#pragma once
#include "fern/core/assert.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/boolean/detail/or.h"


namespace fern {
namespace algebra {

//! Determine the boolean or result of \a value1 and \a value2 and write the result to \a result.
/*!
    \ingroup       boolean
    \sa            fern::binary_local_operation,
                   @ref fern_algorithm_algebra_boolean

    The value types of \a value1, \a value2 and \a result must be arithmetic.
*/
template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void or_(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value1>)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value2>)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Result>)

    or_::detail::or_<>(input_no_data_policy, output_no_data_policy,
        execution_policy, value1, value2, result);
}


/*!
    \ingroup       boolean
    \overload
*/
template<
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void or_(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    OutputNoDataPolicy output_no_data_policy;
    or_<>(InputNoDataPolicy(), output_no_data_policy, execution_policy,
        value1, value2, result);
}


/*!
    \ingroup       boolean
    \overload
*/
template<
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void or_(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    using InputNoDataPolicy = SkipNoData<>;
    using OutputNoDataPolicy = DontMarkNoData;

    OutputNoDataPolicy output_no_data_policy;
    or_<>(InputNoDataPolicy(), output_no_data_policy, execution_policy,
        value1, value2, result);
}

} // namespace algebra
} // namespace fern
