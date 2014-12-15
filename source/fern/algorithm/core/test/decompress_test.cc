#define BOOST_TEST_MODULE fern algorithm core decompress
#include <boost/test/unit_test.hpp>
#include "fern/core/constant_traits.h"
#include "fern/core/vector_traits.h"
#include "fern/feature/core/array_traits.h"
#include "fern/algorithm/core/compress.h"
#include "fern/algorithm/core/decompress.h"


namespace fa = fern::algorithm;


BOOST_FIXTURE_TEST_SUITE(decompress, fern::ThreadClient)

void test_array_1d(
    fa::ExecutionPolicy const& execution_policy)
{
    size_t const nr_threads{fern::ThreadClient::hardware_concurrency()};
    size_t const nr_elements{10 * nr_threads};
    std::vector<int> values(nr_elements);
    std::vector<int> compress_result_we_want(nr_elements);
    std::vector<int> compress_result_we_got(nr_elements);
    std::vector<int> decompress_result_we_got(nr_elements);
    int const no_data_value{99};

    // 0, 1, 2, 3, ..., n-1
    // All values dividable by 5 are replaced by a special value.
    std::iota(values.begin(), values.end(), 0);
    std::transform(values.begin(), values.end(), values.begin(),
        [&](int const& value) {
            return value % 5 == 0? no_data_value : value; });

    // Compression without input no-data is the same as copy.
    compress_result_we_want = values;

    size_t count{12345};

    {
        fa::core::compress(execution_policy, values, compress_result_we_got,
            count);
        BOOST_CHECK_EQUAL(count, values.size());
        BOOST_CHECK(compress_result_we_got == compress_result_we_want);

        fa::core::decompress(execution_policy, compress_result_we_got,
            decompress_result_we_got);
        BOOST_CHECK(decompress_result_we_got == values);
    }
}


BOOST_AUTO_TEST_CASE(array_1d_sequential)
{
    test_array_1d(fa::sequential);
}


BOOST_AUTO_TEST_CASE(array_1d_parallel)
{
    test_array_1d(fa::parallel);
}


void test_array_1d_masked(
    fa::ExecutionPolicy const& execution_policy)
{
    size_t const nr_threads{fern::ThreadClient::hardware_concurrency()};
    size_t const nr_elements{10 * nr_threads};
    std::vector<int> values(nr_elements);
    std::vector<int> compress_result_we_want(nr_elements);
    std::vector<int> compress_result_we_got(nr_elements);
    std::vector<int> decompress_result_we_got(nr_elements);
    int const no_data_value{99};

    // 0, 1, 2, 3, ..., n-1
    // All values dividable by 5 are replaced by a special value.
    std::iota(values.begin(), values.end(), 0);
    std::transform(values.begin(), values.end(), values.begin(),
        [&](int const& value) {
            return value % 5 == 0 ? no_data_value : value; });

    using InputNoDataPolicy = fa::DetectNoDataByValue<std::vector<int>>;
    using OutputNoDataPolicy = fa::MarkNoDataByValue<std::vector<int>>;
    InputNoDataPolicy input_no_data_policy(values, no_data_value);

    compress_result_we_want = values;
    compress_result_we_want.erase(std::remove(compress_result_we_want.begin(),
        compress_result_we_want.end(), no_data_value),
        compress_result_we_want.end());

    size_t count{999999};

    {
        fa::core::compress(input_no_data_policy,
            execution_policy, values, compress_result_we_got, count);
        compress_result_we_got.resize(count);
        BOOST_CHECK_EQUAL(count, compress_result_we_want.size());
        BOOST_CHECK(compress_result_we_got == compress_result_we_want);

        OutputNoDataPolicy output_no_data_policy(decompress_result_we_got,
            no_data_value);
        fa::core::decompress(input_no_data_policy, output_no_data_policy,
            execution_policy, compress_result_we_got, decompress_result_we_got);
        BOOST_CHECK(decompress_result_we_got == values);
    }
}


BOOST_AUTO_TEST_CASE(array_1d_masked_sequential)
{
    test_array_1d_masked(fa::sequential);
}


BOOST_AUTO_TEST_CASE(array_1d_masked_parallel)
{
    test_array_1d_masked(fa::parallel);
}


void test_array_2d(
    fa::ExecutionPolicy const& execution_policy)
{
    // Create 2D array of values.
    // Compression should result in the same array.
    size_t const nr_threads{fern::ThreadClient::hardware_concurrency()};
    size_t const nr_rows{30 * nr_threads};
    size_t const nr_cols{20 * nr_threads};
    size_t const nr_elements{nr_rows * nr_cols};

    fern::Array<int, 2> values(fern::extents[nr_rows][nr_cols]);
    std::vector<int> compress_result_we_want(nr_elements);
    std::vector<int> compress_result_we_got(nr_elements);
    fern::Array<int, 2> decompress_result_we_got(
        fern::extents[nr_rows][nr_cols]);

    int const no_data_value{99};

    std::iota(values.data(), values.data() + nr_elements, 0);
    std::transform(values.data(), values.data() + nr_elements, values.data(),
        [&](int const& value) {
            return value % 5 == 0? no_data_value : value; });

    // Compression without input no-data is the same as copy.
    std::copy(values.data(), values.data() + nr_elements,
        compress_result_we_want.data());

    size_t count{12345};

    {
        fa::core::compress(execution_policy, values, compress_result_we_got,
            count);
        BOOST_CHECK_EQUAL(count, nr_elements);
        BOOST_CHECK(compress_result_we_got == compress_result_we_want);

        fa::core::decompress(execution_policy, compress_result_we_got,
            decompress_result_we_got);
        BOOST_CHECK(decompress_result_we_got == values);
    }
}


BOOST_AUTO_TEST_CASE(array_2d_sequential)
{
    test_array_2d(fa::sequential);
}


BOOST_AUTO_TEST_CASE(array_2d_parallel)
{
    test_array_2d(fa::parallel);
}


void test_array_2d_masked(
    fa::ExecutionPolicy const& execution_policy)
{
    size_t const nr_threads{fern::ThreadClient::hardware_concurrency()};
    size_t const nr_rows{30 * nr_threads};
    size_t const nr_cols{20 * nr_threads};
    size_t const nr_elements{nr_rows * nr_cols};

    fern::Array<int, 2> values(fern::extents[nr_rows][nr_cols]);
    std::vector<int> compress_result_we_want(nr_elements);
    std::vector<int> compress_result_we_got(nr_elements);
    fern::Array<int, 2> decompress_result_we_got(
        fern::extents[nr_rows][nr_cols]);

    int const no_data_value{99};

    std::iota(values.data(), values.data() + nr_elements, 0);
    std::transform(values.data(), values.data() + nr_elements, values.data(),
        [&](int const& value) {
            return value % 5 == 0? no_data_value : value; });

    using InputNoDataPolicy = fa::DetectNoDataByValue<fern::Array<int, 2>>;
    using OutputNoDataPolicy = fa::MarkNoDataByValue<fern::Array<int, 2>>;
    InputNoDataPolicy input_no_data_policy(values, no_data_value);

    std::copy(values.data(), values.data() + nr_elements,
        compress_result_we_want.data());
    compress_result_we_want.erase(std::remove(compress_result_we_want.begin(),
        compress_result_we_want.end(), no_data_value),
        compress_result_we_want.end());

    size_t count{999999};

    {
        fa::core::compress(input_no_data_policy, execution_policy, values,
            compress_result_we_got, count);
        compress_result_we_got.resize(count);
        BOOST_CHECK_EQUAL(count, compress_result_we_want.size());
        BOOST_CHECK(compress_result_we_got == compress_result_we_want);

        OutputNoDataPolicy output_no_data_policy(decompress_result_we_got,
            no_data_value);
        fa::core::decompress(input_no_data_policy, output_no_data_policy,
            execution_policy, compress_result_we_got, decompress_result_we_got);
        BOOST_CHECK(decompress_result_we_got == values);
    }
}


BOOST_AUTO_TEST_CASE(array_2d_masked_sequential)
{
    test_array_2d_masked(fa::sequential);
}


BOOST_AUTO_TEST_CASE(array_2d_masked_parallel)
{
    test_array_2d_masked(fa::parallel);
}

BOOST_AUTO_TEST_SUITE_END()