#define BOOST_TEST_MODULE fern algorithm core merge_no_data
#include <boost/test/unit_test.hpp>
#include "fern/core/constant_traits.h"
#include "fern/feature/core/array_traits.h"
#include "fern/feature/core/masked_array_traits.h"
#include "fern/feature/core/masked_constant_traits.h"
#include "fern/algorithm/core/merge_no_data.h"
#include "fern/algorithm/core/test/test_utils.h"


namespace fa = fern::algorithm;


BOOST_AUTO_TEST_SUITE(merge_no_data)

template<
    typename ExecutionPolicy>
void test_array_0d(
    ExecutionPolicy& execution_policy)
{
    int value;
    int result_we_want;
    int result_we_got;

    {
        value = 5;

        result_we_want = -9;
        result_we_got = -9;
        fa::core::merge_no_data(execution_policy, value, result_we_got);
        BOOST_CHECK(fern::compare(execution_policy, result_we_got,
            result_we_want));
    }
}


BOOST_AUTO_TEST_CASE(array_0d_sequential)
{
    test_array_0d(fa::sequential);
    fa::ExecutionPolicy execution_policy{fa::sequential};
    test_array_0d(execution_policy);
}


BOOST_AUTO_TEST_CASE(array_0d_parallel)
{
    test_array_0d(fa::parallel);
    fa::ExecutionPolicy execution_policy{fa::parallel};
    test_array_0d(execution_policy);
}


template<
    typename ExecutionPolicy>
void test_array_0d_masked(
    ExecutionPolicy& execution_policy)
{
    fern::MaskedConstant<int> value;
    fern::MaskedConstant<int> result_we_want;
    fern::MaskedConstant<int> result_we_got;

    fa::SkipNoData<
        fa::DetectNoDataByValue<bool>> input_no_data_policy(
            fa::DetectNoDataByValue<bool>(value.mask(), true));
    fa::MarkNoDataByValue<bool> output_no_data_policy(result_we_got.mask(),
        true);

    // merge(false) -> false
    {
        value = 5;
        value.mask() = false;

        result_we_want = fern::MaskedConstant<int>(-9, false);
        result_we_got = -9;
        result_we_got.mask() = false;
        fa::core::merge_no_data(input_no_data_policy, output_no_data_policy,
            execution_policy, value, result_we_got);
        BOOST_CHECK(fern::compare(execution_policy, result_we_got,
            result_we_want));
    }

    // merge(true) -> true
    {
        value = 5;
        value.mask() = true;

        result_we_want = fern::MaskedConstant<int>(-9, true);
        result_we_got = -9;
        result_we_got.mask() = true;
        fa::core::merge_no_data(input_no_data_policy, output_no_data_policy,
            execution_policy, value, result_we_got);
        BOOST_CHECK(fern::compare(execution_policy, result_we_got,
            result_we_want));
    }
}


BOOST_AUTO_TEST_CASE(array_0d_masked_sequential)
{
    test_array_0d_masked(fa::sequential);
    fa::ExecutionPolicy execution_policy{fa::sequential};
    test_array_0d_masked(execution_policy);
}


BOOST_AUTO_TEST_CASE(array_0d_masked_parallel)
{
    test_array_0d_masked(fa::parallel);
    fa::ExecutionPolicy execution_policy{fa::parallel};
    test_array_0d_masked(execution_policy);
}


// TODO 1d


template<
    typename ExecutionPolicy>
void test_array_2d(
    ExecutionPolicy& execution_policy)
{
    size_t const nr_threads{fern::hardware_concurrency()};
    size_t const nr_rows{30 * nr_threads};
    size_t const nr_cols{20 * nr_threads};
    size_t const nr_elements{nr_rows * nr_cols};

    fern::Array<int, 2> value(fern::extents[nr_rows][nr_cols]);
    fern::Array<int, 2> result_we_got(fern::extents[nr_rows][nr_cols]);

    // Fill value: 0, 1, 2, ...
    std::iota(value.data(), value.data() + nr_elements, 0);

    // Fill result_we_want.
    fern::Array<int, 2> result_we_want(fern::extents[nr_rows][nr_cols]);
    result_we_want.fill(-9);

    result_we_got.fill(-9);
    fa::core::merge_no_data(execution_policy, value, result_we_got);
    BOOST_CHECK(fern::compare(execution_policy, result_we_got, result_we_want));
}


BOOST_AUTO_TEST_CASE(array_2d_sequential)
{
    test_array_2d(fa::sequential);
    fa::ExecutionPolicy execution_policy{fa::sequential};
    test_array_2d(execution_policy);
}


BOOST_AUTO_TEST_CASE(array_2d_parallel)
{
    test_array_2d(fa::parallel);
    fa::ExecutionPolicy execution_policy{fa::parallel};
    test_array_2d(execution_policy);
}


template<
    typename ExecutionPolicy>
void test_array_2d_masked(
    ExecutionPolicy& execution_policy)
{
    size_t const nr_threads{fern::hardware_concurrency()};
    size_t const nr_rows{3 * nr_threads};
    size_t const nr_cols{2 * nr_threads};
    size_t const nr_elements{nr_rows * nr_cols};

    fern::MaskedArray<int, 2> value(fern::extents[nr_rows][nr_cols]);
    fern::MaskedArray<int, 2> result_we_got(fern::extents[nr_rows][nr_cols]);

    fa::SkipNoData<fa::DetectNoDataByValue<fern::Mask<2>>>
        input_no_data_policy(fa::DetectNoDataByValue<fern::Mask<2>>(
            value.mask(), true));
    fa::MarkNoDataByValue<fern::Mask<2>> output_no_data_policy(
        result_we_got.mask(), true);

    // Fill value: 0, 1, 2, ...
    // Mask all cells with index % 4.
    std::iota(value.data(), value.data() + nr_elements, 0);
    {
        int n = 0;
        std::generate(value.mask().data(), value.mask().data() +
            nr_elements, [&] () { return n++ % 4 == 0 ? true : false; });
    }

    // Fill result_we_want.
    // Mask all cells for which value is masked.
    fern::MaskedArray<int, 2> result_we_want(fern::extents[nr_rows][nr_cols]);
    result_we_want.fill(-9);
    {
        auto value1_mask_it = value.mask().data();
        std::transform(result_we_want.data(),
            result_we_want.data() + nr_elements, result_we_want.mask().data(),
            [&](bool const& /* value */) {
                ++value1_mask_it;
                return *(value1_mask_it-1); });
    }

    result_we_got.fill(-9);
    fa::core::merge_no_data(input_no_data_policy, output_no_data_policy,
        execution_policy, value, result_we_got);
    BOOST_CHECK(fern::compare(execution_policy, result_we_got, result_we_want));
}


BOOST_AUTO_TEST_CASE(array_2d_masked_sequential)
{
    test_array_2d_masked(fa::sequential);
    fa::ExecutionPolicy execution_policy{fa::sequential};
    test_array_2d_masked(execution_policy);
}


BOOST_AUTO_TEST_CASE(array_2d_masked_parallel)
{
    test_array_2d_masked(fa::parallel);
    fa::ExecutionPolicy execution_policy{fa::parallel};
    test_array_2d_masked(execution_policy);
}

BOOST_AUTO_TEST_SUITE_END()
