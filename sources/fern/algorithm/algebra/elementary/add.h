#pragma once
#include "fern/core/assert.h"
#include "fern/core/base_class.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/elementary/detail/add.h"


namespace fern {
namespace algorithm {
namespace add {

/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @brief      Determine the result type when adding instances of @a Value1
                and @a Value2.
    @sa         fern::algorithm::add::result_value_type,
                fern::algorithm::Result
*/
template<
    class Value1,
    class Value2>
using result_type = typename fern::algorithm::Result<Value1, Value2>::type;


/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @brief      Determine the result value type when adding instances of
                @a Value1 and @a Value2.
    @sa         fern::algorithm::add::result_type
*/
template<
    class Value1,
    class Value2>
using result_value_type = typename fern::algorithm::Result<value_type<Value1>,
    value_type<Value2>>::type;


/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @brief      Out of range policy for fern::algebra::add algorithm.
    @sa         @ref fern_algorithm_policies_out_of_range_policy

    The logic for determining whether add's result is out of range depends
    on the types involved (unsigned integers, signed integers, floating
    points) and their sizes.

    The value types of @a value1 and @a value2 must be arithmetic and not
    `bool`. The value type of @a result must be equal to
    fern::add::result_value_type<Value1, Value2>.
*/
template<
    class Value1,
    class Value2,
    class Result>
class OutOfRangePolicy
{

    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value1>)
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value1>, bool)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value2>)
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value2>, bool)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Result>)
    FERN_STATIC_ASSERT(std::is_same, value_type<Result>,
        add::result_value_type<Value1, Value2>)

public:

    inline static constexpr bool within_range(
        Value1 const& value1,
        Value2 const& value2,
        Result const& result)
    {
        using value1_tag = base_class<number_category<Value1>, integer_tag>;
        using value2_tag = base_class<number_category<Value2>, integer_tag>;

        return detail::dispatch::within_range<Value1, Value2, Result,
            value1_tag, value2_tag>::calculate(value1, value2, result);
    }

};

} // namespace add


namespace algebra {

/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @brief      Add @a value1 to @a value2 and write the result to @a result.
    @sa         fern::algorithm::add::OutOfRangePolicy,
                fern::algorithm::add::result_type,
                fern::algorithm::add::result_value_type,
                fern::algorithm::binary_local_operation

    The value types of @a value1 and @a value2 must be arithmetic and not
    `bool`. The value type of @a result must be equal to
    fern::add::result_value_type<Value1, Value2>.
*/
template<
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void add(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value1>)
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value1>, bool)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value2>)
    FERN_STATIC_ASSERT(!std::is_same, value_type<Value2>, bool)
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Result>)
    FERN_STATIC_ASSERT(std::is_same, value_type<Result>,
        add::result_value_type<Value1, Value2>)

    add::detail::add<OutOfRangePolicy>(input_no_data_policy,
        output_no_data_policy, execution_policy, value1, value2, result);
}


/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @overload
*/
template<
    template<class, class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void add(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    OutputNoDataPolicy output_no_data_policy;
    add<OutOfRangePolicy>(InputNoDataPolicy(), output_no_data_policy,
        execution_policy, value1, value2, result);
}


/*!
    @ingroup    fern_algorithm_algebra_elementary_group
    @overload
*/
template<
    class ExecutionPolicy,
    class Value1,
    class Value2,
    class Result
>
void add(
    ExecutionPolicy const& execution_policy,
    Value1 const& value1,
    Value2 const& value2,
    Result& result)
{
    using InputNoDataPolicy = SkipNoData<>;
    using OutputNoDataPolicy = DontMarkNoData;

    OutputNoDataPolicy output_no_data_policy;
    add<binary::DiscardRangeErrors>(InputNoDataPolicy(),
        output_no_data_policy, execution_policy, value1, value2, result);
}

} // namespace algebra
} // namespace algorithm
} // namespace fern
