#include "fern/python_extension/algorithm/algebra/elementary/add.h"
#include "fern/python_extension/feature/detail/masked_raster_traits.h"
#include "fern/algorithm/policy/policies.h"
#include "fern/algorithm/algebra/elementary/add.h"
#include "fern/python_extension/core/switch_on_value_type.h"


namespace fa = fern::algorithm;


namespace fern {
namespace python {
namespace {

template<
    typename T1,
    typename T2,
    typename R>
void add(
    fa::ExecutionPolicy& execution_policy,
    detail::MaskedRaster<T1> const& lhs,
    T2 const& rhs,
    detail::MaskedRaster<R>& result)
{
    fa::InputNoDataPolicies<
        fa::DetectNoData<detail::MaskedRaster<T1>>,
        fa::SkipNoData
    > input_no_data_policy{{lhs}, {}};
    fa::MarkNoData<detail::MaskedRaster<R>> output_no_data_policy(result);

    fa::algebra::add<fa::add::OutOfRangePolicy>(input_no_data_policy,
        output_no_data_policy, execution_policy, lhs, rhs, result);
}


template<
    typename T1,
    typename T2>
MaskedRasterHandle add(
    fa::ExecutionPolicy& execution_policy,
    detail::MaskedRaster<T1> const& lhs,
    T2 const& rhs)
{
    // using R = T1; // algorithm::add::result_type<T1, T2>;
    using R = algorithm::add::result_type<T1, T2>;
    auto handle = std::make_shared<detail::MaskedRaster<R>>(
        lhs.sizes(), lhs.origin(), lhs.cell_sizes());
    add(execution_policy, lhs, rhs, *handle);
    return std::make_shared<MaskedRaster>(handle);
}

} // Anonymous namespace

#define CASE(                              \
    value_type_enum1,                      \
    value_type1)                           \
case value_type_enum1: {                   \
    result = add(                          \
        execution_policy,                  \
        raster->raster<value_type1>(),     \
        static_cast<value_type1>(value));  \
    break;                                 \
}

MaskedRasterHandle add(
    fa::ExecutionPolicy& execution_policy,
    MaskedRasterHandle const& raster,
    int64_t value)
{
    MaskedRasterHandle result;
    SWITCH_ON_VALUE_TYPE(raster->value_type(), CASE)
    return result;
}


MaskedRasterHandle add(
    fa::ExecutionPolicy& execution_policy,
    MaskedRasterHandle const& raster,
    double value)
{
    MaskedRasterHandle result;
    SWITCH_ON_VALUE_TYPE(raster->value_type(), CASE)
    return result;
}

#undef CASE

} // namespace python
} // namespace fern
