#pragma once
#include "fern/core/assert.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/vector/detail/laplacian.h"


namespace fern {
namespace laplacian {

//! The out-of-range policy for the laplacian operation.
/*!
  The result of the laplacian operation is a floating point. This policy
  verifies whether the result value is finite.
*/
template<
    class Value,
    class Result>
class OutOfRangePolicy
{

    FERN_STATIC_ASSERT(std::is_arithmetic, Result)
    FERN_STATIC_ASSERT(std::is_floating_point, Result)

public:

    inline bool within_range(
        Value const& /* value */,
        Result const& result) const
    {
        return std::isfinite(result);
    }

};

} // namespace laplacian


namespace algebra {

//! Calculate the laplacian of \a value and write the result to \a result.
/*!
    The algorithm implemented is similar to [the one implemented in PCRaster]
    (https://sourceforge.net/p/pcraster/pcrtree2/ci/master/tree/sources/calc/vf.c).

    In short/pseudo code, the algorithm:

    \code
    convolve(value, kernel, result);
    result = (result - (sum(kernel) * value)) / cell_area;
    result = ((convolve(value, kernel) -
        (convolve(defined(value), kernel(1)) * value)) / cell_area;
    \endcode

    Kernel:

    \code
    +---+---+---+
    | 2 | 3 | 2 |
    +---+---+---+
    | 3 | 0 | 3 |
    +---+---+---+
    | 2 | 3 | 2 |
    +---+---+---+
    \endcode

    The value type of \a value and \a result must be floating point and the
    same.

    \ingroup       vector
    \sa            fern::laplacian::OutOfRangePolicy,
                   @ref fern_algorithm_algebra_vector
*/
template<
    template<class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value,
    class Result>
void laplacian(
    InputNoDataPolicy const& input_no_data_policy,
    OutputNoDataPolicy& output_no_data_policy,
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    FERN_STATIC_ASSERT(std::is_arithmetic, value_type<Value>)
    FERN_STATIC_ASSERT(std::is_floating_point, value_type<Result>)
    FERN_STATIC_ASSERT(std::is_same, value_type<Value>, value_type<Result>)

    laplacian::detail::laplacian<OutOfRangePolicy>(input_no_data_policy,
        output_no_data_policy, execution_policy, value, result);
}


/*!
    \ingroup       vector
    \overload
*/
template<
    template<class, class> class OutOfRangePolicy,
    class InputNoDataPolicy,
    class OutputNoDataPolicy,
    class ExecutionPolicy,
    class Value,
    class Result>
void laplacian(
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    OutputNoDataPolicy output_no_data_policy;
    laplacian<OutOfRangePolicy>(InputNoDataPolicy(), output_no_data_policy,
        execution_policy, value, result);
}


/*!
    \ingroup       vector
    \overload
*/
template<
    class ExecutionPolicy,
    class Value,
    class Result>
void laplacian(
    ExecutionPolicy const& execution_policy,
    Value const& value,
    Result& result)
{
    using InputNoDataPolicy = SkipNoData<>;
    using OutputNoDataPolicy = DontMarkNoData;

    OutputNoDataPolicy output_no_data_policy;
    laplacian<unary::DiscardRangeErrors>(InputNoDataPolicy(),
        output_no_data_policy, execution_policy, value, result);
}

} // namespace algebra
} // namespace fern
