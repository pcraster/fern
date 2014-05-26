#define BOOST_TEST_MODULE fern algorithm algebra elementary less_equal
#include <boost/test/unit_test.hpp>
#include "fern/core/constant_traits.h"
#include "fern/algorithm/algebra/elementary/less_equal.h"


BOOST_AUTO_TEST_SUITE(less_equal)

BOOST_AUTO_TEST_CASE(traits)
{
    using LessEqual = fern::algebra::LessEqual<float, float, bool>;
    BOOST_CHECK((std::is_same<fern::OperationTraits<LessEqual>::category,
        fern::local_operation_tag>::value));
}


template<
    class Value1,
    class Value2,
    class Result>
void verify_value(
    Value1 const& value1,
    Value2 const& value2,
    Result const& result_we_want)
{
    Result result_we_get;
    fern::algebra::less_equal(value1, value2, result_we_get);
    BOOST_CHECK_EQUAL(result_we_get, result_we_want);
}


BOOST_AUTO_TEST_CASE(algorithm)
{
    verify_value<float, float, bool>(0.0f, 0.0f, true);
    verify_value<float, float, bool>(1.0f, 1.0f, true);
    verify_value<float, float, bool>(-1.0f, -1.0f, true);

    verify_value<int, int, bool>(1, 2, true);
    verify_value<int, int, bool>(2, 1, false);

    verify_value<int, int, bool>(-1, -2, false);
    verify_value<int, int, bool>(-2, -1, true);
}

BOOST_AUTO_TEST_SUITE_END()
