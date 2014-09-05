#define BOOST_TEST_MODULE fern algorithm algebra unary_disaggregate_operation
#include <boost/test/unit_test.hpp>
#include "fern/core/constant_traits.h"
#include "fern/core/vector_traits.h"
#include "fern/feature/core/array_traits.h"
#include "fern/feature/core/masked_array_traits.h"
#include "fern/feature/core/masked_constant_traits.h"
#include "fern/algorithm/algebra/boolean/defined.h"


using ArgumentValue = int32_t;
using ResultValue = int32_t;

template<
    class Value>
using Algorithm = fern::fill::detail::Algorithm<Value>;


BOOST_AUTO_TEST_SUITE(unary_disaggregate_operation)

BOOST_AUTO_TEST_CASE(array_0d)
{
    using InputNoDataPolicy = fern::SkipNoData;
    using OutputNoDataPolicy = fern::DontMarkNoData;
    using Argument = ArgumentValue;
    using Result = ResultValue;

    OutputNoDataPolicy output_no_data_policy;
    Argument value{5};
    Result result{3};

    fern::unary_disaggregate_operation<Algorithm,
        fern::unary::DiscardDomainErrors,
        fern::unary::DiscardRangeErrors>(
            InputNoDataPolicy(), output_no_data_policy,
            fern::sequential, value, result);

    BOOST_REQUIRE_EQUAL(result, 5);

    result = 3;

    fern::unary_disaggregate_operation<Algorithm,
        fern::unary::DiscardDomainErrors,
        fern::unary::DiscardRangeErrors>(
            InputNoDataPolicy(), output_no_data_policy,
            fern::parallel, value, result);

    BOOST_REQUIRE_EQUAL(result, 5);
}


BOOST_AUTO_TEST_CASE(array_0d_masked)
{
    using InputNoDataPolicy = fern::DetectNoDataByValue<bool>;
    using OutputNoDataPolicy = fern::MarkNoDataByValue<bool>;
    using Argument = fern::MaskedConstant<ArgumentValue>;
    using Result = fern::MaskedConstant<ResultValue>;

    Argument argument;
    Result result;

    // Input is not masked.
    {
        argument.value() = 5;
        argument.mask() = false;
        result.value() = 3;
        result.mask() = argument.mask();

        InputNoDataPolicy input_no_data_policy(result.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask(), false);
        BOOST_REQUIRE_EQUAL(result.value(), 5);
    }

    // Input is masked.
    {
        argument.value() = 5;
        argument.mask() = true;
        result.value() = 3;
        result.mask() = argument.mask();

        InputNoDataPolicy input_no_data_policy(result.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);
        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask(), true);
        BOOST_REQUIRE_EQUAL(result.value(), 3);
    }
}


BOOST_AUTO_TEST_CASE(array_1d_sequential)
{
    using Argument = ArgumentValue;
    using InputNoDataPolicy = fern::SkipNoData;
    using OutputNoDataPolicy = fern::DontMarkNoData;

    Argument argument{5};

    OutputNoDataPolicy output_no_data_policy;

    // vector
    {
        using Result = std::vector<ResultValue>;

        Result result{3, 3, 3};

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result[0], 5);
        BOOST_REQUIRE_EQUAL(result[1], 5);
        BOOST_REQUIRE_EQUAL(result[2], 5);
    }

    // 1d array
    {
        using Result = fern::Array<ResultValue, 1>;

        Result result{3, 3, 3};

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result[0], 5);
        BOOST_REQUIRE_EQUAL(result[1], 5);
        BOOST_REQUIRE_EQUAL(result[2], 5);
    }

    // empty
    {
        using Result = std::vector<ResultValue>;

        Result result;

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_CHECK(result.empty());
    }
}


BOOST_AUTO_TEST_CASE(array_1d_parallel)
{
    using Argument = ArgumentValue;
    using InputNoDataPolicy = fern::SkipNoData;
    using OutputNoDataPolicy = fern::DontMarkNoData;

    Argument argument{5};

    OutputNoDataPolicy output_no_data_policy;

    fern::ThreadClient client(2);

    // vector
    {
        using Result = std::vector<ResultValue>;

        Result result{3, 3, 3};

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::parallel, argument, result);

        BOOST_REQUIRE_EQUAL(result[0], 5);
        BOOST_REQUIRE_EQUAL(result[1], 5);
        BOOST_REQUIRE_EQUAL(result[2], 5);
    }

    // 1d array
    {
        using Result = fern::Array<ResultValue, 1>;

        Result result{3, 3, 3};

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::parallel, argument, result);

        BOOST_REQUIRE_EQUAL(result[0], 5);
        BOOST_REQUIRE_EQUAL(result[1], 5);
        BOOST_REQUIRE_EQUAL(result[2], 5);
    }

    // empty
    {
        using Result = std::vector<ResultValue>;

        Result result;

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                InputNoDataPolicy(), output_no_data_policy,
                fern::parallel, argument, result);

        BOOST_CHECK(result.empty());
    }
}


BOOST_AUTO_TEST_CASE(array_1d_masked)
{
    using Argument = fern::MaskedConstant<ArgumentValue>;
    using Result = fern::MaskedArray<ResultValue, 1>;
    using InputNoDataPolicy = fern::DetectNoDataByValue<bool>;
    using OutputNoDataPolicy = fern::MarkNoDataByValue<fern::Mask<1>>;


    {
        Argument argument(5);
        Result result(3);

        InputNoDataPolicy input_no_data_policy(argument.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);

        result.fill(3);

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask()[0], false);
        BOOST_REQUIRE_EQUAL(result.mask()[1], false);
        BOOST_REQUIRE_EQUAL(result.mask()[2], false);
        BOOST_REQUIRE_EQUAL(result[0], 5);
        BOOST_REQUIRE_EQUAL(result[1], 5);
        BOOST_REQUIRE_EQUAL(result[2], 5);

        argument.mask() = true;
        result.fill(3);
        result.mask().fill(argument.mask());

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask()[0], true);
        BOOST_REQUIRE_EQUAL(result.mask()[1], true);
        BOOST_REQUIRE_EQUAL(result.mask()[2], true);
        BOOST_REQUIRE_EQUAL(result[0], 3);
        BOOST_REQUIRE_EQUAL(result[1], 3);
        BOOST_REQUIRE_EQUAL(result[2], 3);
    }

    // empty
    {
        Argument argument(5);
        Result result;

        InputNoDataPolicy input_no_data_policy(argument.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_CHECK(result.empty());
    }
}


BOOST_AUTO_TEST_CASE(array_2d_sequential)
{
    using InputNoDataPolicy = fern::SkipNoData;
    using OutputNoDataPolicy = fern::DontMarkNoData;
    using Argument = ArgumentValue;
    using Result = fern::Array<ResultValue, 2>;

    OutputNoDataPolicy output_no_data_policy;

    Argument argument{5};
    Result result{
        { 3, 3 },
        { 3, 3 },
        { 3, 3 }
    };

    fern::unary_disaggregate_operation<Algorithm,
        fern::unary::DiscardDomainErrors,
        fern::unary::DiscardRangeErrors>(
            InputNoDataPolicy(), output_no_data_policy,
            fern::sequential, argument, result);

    BOOST_CHECK_EQUAL(result[0][0], 5);
    BOOST_CHECK_EQUAL(result[0][1], 5);
    BOOST_CHECK_EQUAL(result[1][0], 5);
    BOOST_CHECK_EQUAL(result[1][1], 5);
    BOOST_CHECK_EQUAL(result[2][0], 5);
    BOOST_CHECK_EQUAL(result[2][1], 5);
}


BOOST_AUTO_TEST_CASE(array_2d_parallel)
{
    using InputNoDataPolicy = fern::SkipNoData;
    using OutputNoDataPolicy = fern::DontMarkNoData;
    using Argument = ArgumentValue;
    using Result = fern::Array<ResultValue, 2>;

    OutputNoDataPolicy output_no_data_policy;

    fern::ThreadClient client(2);

    Argument argument{5};
    Result result{
        { 3, 3 },
        { 3, 3 },
        { 3, 3 }
    };

    fern::unary_disaggregate_operation<Algorithm,
        fern::unary::DiscardDomainErrors,
        fern::unary::DiscardRangeErrors>(
            InputNoDataPolicy(), output_no_data_policy,
            fern::parallel, argument, result);

    BOOST_CHECK_EQUAL(result[0][0], 5);
    BOOST_CHECK_EQUAL(result[0][1], 5);
    BOOST_CHECK_EQUAL(result[1][0], 5);
    BOOST_CHECK_EQUAL(result[1][1], 5);
    BOOST_CHECK_EQUAL(result[2][0], 5);
    BOOST_CHECK_EQUAL(result[2][1], 5);
}


BOOST_AUTO_TEST_CASE(array_2d_masked)
{
    using Argument = fern::MaskedConstant<ArgumentValue>;
    using Result = fern::MaskedArray<ResultValue, 2>;
    using InputNoDataPolicy = fern::DetectNoDataByValue<bool>;
    using OutputNoDataPolicy = fern::MarkNoDataByValue<fern::Mask<2>>;

    {
        Argument argument{5};
        Result result{
            { 3, 3 },
            { 3, 3 },
            { 3, 3 }
        };

        InputNoDataPolicy input_no_data_policy(argument.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);

        result.fill(3);

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask()[0][0], false);
        BOOST_REQUIRE_EQUAL(result.mask()[0][0], false);
        BOOST_REQUIRE_EQUAL(result.mask()[1][0], false);
        BOOST_REQUIRE_EQUAL(result.mask()[1][1], false);
        BOOST_REQUIRE_EQUAL(result.mask()[2][0], false);
        BOOST_REQUIRE_EQUAL(result.mask()[2][1], false);
        BOOST_CHECK_EQUAL(result[0][0], 5);
        BOOST_CHECK_EQUAL(result[0][1], 5);
        BOOST_CHECK_EQUAL(result[1][0], 5);
        BOOST_CHECK_EQUAL(result[1][1], 5);
        BOOST_CHECK_EQUAL(result[2][0], 5);
        BOOST_CHECK_EQUAL(result[2][1], 5);

        argument.mask() = true;
        result.fill(3);
        result.mask().fill(argument.mask());

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_REQUIRE_EQUAL(result.mask()[0][0], true);
        BOOST_REQUIRE_EQUAL(result.mask()[0][0], true);
        BOOST_REQUIRE_EQUAL(result.mask()[1][0], true);
        BOOST_REQUIRE_EQUAL(result.mask()[1][1], true);
        BOOST_REQUIRE_EQUAL(result.mask()[2][0], true);
        BOOST_REQUIRE_EQUAL(result.mask()[2][1], true);
        BOOST_CHECK_EQUAL(result[0][0], 3);
        BOOST_CHECK_EQUAL(result[0][1], 3);
        BOOST_CHECK_EQUAL(result[1][0], 3);
        BOOST_CHECK_EQUAL(result[1][1], 3);
        BOOST_CHECK_EQUAL(result[2][0], 3);
        BOOST_CHECK_EQUAL(result[2][1], 3);
    }

    // empty
    {
        Argument argument{5};
        Result result;

        InputNoDataPolicy input_no_data_policy(argument.mask(), true);
        OutputNoDataPolicy output_no_data_policy(result.mask(), true);

        fern::unary_disaggregate_operation<Algorithm,
            fern::unary::DiscardDomainErrors,
            fern::unary::DiscardRangeErrors>(
                input_no_data_policy, output_no_data_policy,
                fern::sequential, argument, result);

        BOOST_CHECK(result.empty());
    }
}

BOOST_AUTO_TEST_SUITE_END()