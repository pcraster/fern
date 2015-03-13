#define BOOST_TEST_MODULE fern algorithm statistic sum
#include <limits>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include "fern/core/data_customization_point/constant.h"
#include "fern/feature/core/data_customization_point/array.h"
#include "fern/feature/core/data_customization_point/masked_array.h"
#include "fern/feature/core/masked_constant_traits.h"
#include "fern/algorithm/core/test/test_utils.h"
#include "fern/algorithm/statistic/sum.h"


namespace f = fern;
namespace fa = f::algorithm;
namespace ft = f::test;


BOOST_AUTO_TEST_SUITE(sum)

template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_0d_0d(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    int result_we_got{-9};
    fa::statistic::sum<>(execution_policy, value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_0d_0d(
    ExecutionPolicy& execution_policy)
{
    {
        int value{5};
        int result_we_want{5};
        verify_0d_0d(execution_policy, value, result_we_want);
    }

    {
        int value{-5};
        int result_we_want{-5};
        verify_0d_0d(execution_policy, value, result_we_want);
    }

    {
        int value{0};
        int result_we_want{0};
        verify_0d_0d(execution_policy, value, result_we_want);
    }
}


template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_0d_0d_masked(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    using InputNoDataPolicy = fa::InputNoDataPolicies<
        fa::DetectNoDataByValue<bool>>;
    using OutputNoDataPolicy = fa::MarkNoDataByValue<bool>;

    f::MaskedConstant<int> result_we_got{-9};

    InputNoDataPolicy input_no_data_policy{{value.mask(), true}};
    OutputNoDataPolicy output_no_data_policy(result_we_got.mask(), true);

    fa::statistic::sum<fa::sum::OutOfRangePolicy>(
        input_no_data_policy, output_no_data_policy, execution_policy,
        value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_0d_0d_masked(
    ExecutionPolicy& execution_policy)
{
    // Regular case.
    {
        f::MaskedConstant<int> value{5};
        f::MaskedConstant<int> result_we_want{5};
        verify_0d_0d_masked(execution_policy, value, result_we_want);
    }

    // Mask a value.
    {
        f::MaskedConstant<int> value{5, true};
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_0d_0d_masked(execution_policy, value, result_we_want);
    }
}


template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_1d_0d(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    int result_we_got{-9};
    fa::statistic::sum<>(execution_policy, value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_1d_0d(
    ExecutionPolicy& execution_policy)
{
    // Regular case.
    {
        f::Array<int, 1> value(ft::nr_elements_1d);
        std::iota(value.data(), value.data() + ft::nr_elements_1d, 0);
        int result_we_want{std::accumulate(value.data(), value.data() +
            ft::nr_elements_1d, 0)};
        verify_1d_0d(execution_policy, value, result_we_want);
    }

    // Out of range.
    {
        f::Array<int, 1> value(ft::nr_elements_1d);
        std::iota(value.data(), value.data() + ft::nr_elements_1d, 0);
        get(value, 5) = std::numeric_limits<int>::max();
        // Overflow, integer wrap.
        int result_we_want = std::accumulate(value.data(), value.data() +
            ft::nr_elements_1d, 0);
        verify_1d_0d(execution_policy, value, result_we_want);
    }

    // Empty.
    {
        f::Array<int, 1> value(0);
        int result_we_want{-9};
        verify_1d_0d(execution_policy, value, result_we_want);
    }
}


template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_1d_0d_masked(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    using InputNoDataPolicy = fa::InputNoDataPolicies<
        fa::DetectNoDataByValue<f::Mask<1>>>;
    using OutputNoDataPolicy = fa::MarkNoDataByValue<bool>;

    f::MaskedConstant<int> result_we_got{-9};

    InputNoDataPolicy input_no_data_policy{{value.mask(), true}};
    OutputNoDataPolicy output_no_data_policy(result_we_got.mask(), true);

    fa::statistic::sum<fa::sum::OutOfRangePolicy>(
        input_no_data_policy, output_no_data_policy, execution_policy,
        value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_1d_0d_masked(
    ExecutionPolicy& execution_policy)
{
    // Regular case.
    {
        f::MaskedArray<int, 1> value(ft::nr_elements_1d);
        std::iota(value.data(), value.data() + ft::nr_elements_1d, 0);
        f::MaskedConstant<int> result_we_want{std::accumulate(
            value.data(), value.data() + ft::nr_elements_1d, 0)};
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }


    // Mask a value.
    {
        f::MaskedArray<int, 1> value(ft::nr_elements_1d);
        std::iota(value.data(), value.data() + ft::nr_elements_1d, 0);
        get(value.mask(), 5) = true;
        f::MaskedConstant<int> result_we_want{std::accumulate(
            value.data(), value.data() + ft::nr_elements_1d, 0)};
        result_we_want -= get(value, 5);
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }


    // Mask all values.
    {
        f::MaskedArray<int, 1> value(ft::nr_elements_1d);
        value.mask().fill(true);
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }


    // Out of range, within a block.
    {
        f::MaskedArray<int, 1> value(ft::nr_elements_1d, 0);
        get(value, 0) = std::numeric_limits<int>::max();
        get(value, 1) = 1;
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }


    // Out of range, when aggregating block results.
    {
        f::MaskedArray<int, 1> value(ft::nr_elements_1d, 0);
        value.fill(0);
        get(value, 0) = std::numeric_limits<int>::max();
        get(value, ft::nr_elements_1d - 1) = 1;
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }

    // Empty.
    {
        f::MaskedArray<int, 1> value(0);
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_1d_0d_masked(execution_policy, value, result_we_want);
    }
}


template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_2d_0d(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    int result_we_got{-9};
    fa::statistic::sum<>(execution_policy, value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_2d_0d(
    ExecutionPolicy& execution_policy)
{
    // Regular case.
    {
        f::Array<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols]);
        std::iota(value.data(), value.data() + ft::nr_elements_2d, 0);
        int result_we_want{std::accumulate(value.data(), value.data() +
            ft::nr_elements_2d, 0)};
        verify_2d_0d(execution_policy, value, result_we_want);
    }

    // Empty.
    {
        f::Array<int, 2> value(f::extents[0][0]);
        int result_we_want{-9};
        verify_2d_0d(execution_policy, value, result_we_want);
    }
}


template<
    typename ExecutionPolicy,
    typename Argument,
    typename Result>
void verify_2d_0d_masked(
    ExecutionPolicy& execution_policy,
    Argument const& value,
    Result const& result_we_want)
{
    using InputNoDataPolicy = fa::InputNoDataPolicies<
        fa::DetectNoDataByValue<f::Mask<2>>>;
    using OutputNoDataPolicy = fa::MarkNoDataByValue<bool>;

    f::MaskedConstant<int> result_we_got{-9};

    InputNoDataPolicy input_no_data_policy{{value.mask(), true}};
    OutputNoDataPolicy output_no_data_policy(result_we_got.mask(), true);

    fa::statistic::sum<fa::sum::OutOfRangePolicy>(
        input_no_data_policy, output_no_data_policy, execution_policy,
        value, result_we_got);
    BOOST_CHECK_EQUAL(result_we_got, result_we_want);
}


template<
    typename ExecutionPolicy>
void test_2d_0d_masked(
    ExecutionPolicy& execution_policy)
{
    // Regular case.
    {
        f::MaskedArray<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols]);
        std::iota(value.data(), value.data() + ft::nr_elements_2d, 0);
        f::MaskedConstant<int> result_we_want{std::accumulate(
            value.data(), value.data() + ft::nr_elements_2d, 0)};
        verify_2d_0d(execution_policy, value, result_we_want);
    }


    // Mask a value.
    {
        f::MaskedArray<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols]);
        std::iota(value.data(), value.data() + ft::nr_elements_2d, 0);
        get(value.mask(), 5) = true;
        f::MaskedConstant<int> result_we_want{std::accumulate(
            value.data(), value.data() + ft::nr_elements_2d, 0)};
        result_we_want -= get(value, 5);
        verify_2d_0d_masked(execution_policy, value, result_we_want);
    }


    // Mask all values.
    {
        f::MaskedArray<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols]);
        value.mask().fill(true);
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_2d_0d_masked(execution_policy, value, result_we_want);
    }


    // Out of range, within a block.
    {
        f::MaskedArray<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols], 0);
        get(value, 0) = std::numeric_limits<int>::max();
        get(value, 1) = 1;
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_2d_0d_masked(execution_policy, value, result_we_want);
    }


    // Out of range, when aggregating block results.
    {
        f::MaskedArray<int, 2> value(f::extents[ft::nr_rows][ft::nr_cols], 0);
        value.fill(0);
        get(value, 0) = std::numeric_limits<int>::max();
        get(value, ft::nr_elements_2d - 1) = 1;
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_2d_0d_masked(execution_policy, value, result_we_want);
    }

    // Empty.
    {
        f::MaskedArray<int, 2> value(f::extents[0][0]);
        f::MaskedConstant<int> result_we_want{-9, true};
        verify_2d_0d_masked(execution_policy, value, result_we_want);
    }
}


FERN_UNARY_AGGREGATE_TEST_CASES()

BOOST_AUTO_TEST_SUITE_END()
