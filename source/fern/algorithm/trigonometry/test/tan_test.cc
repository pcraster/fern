#define BOOST_TEST_MODULE fern algorithm trigonometry tan
#include <boost/test/unit_test.hpp>
#include "fern/core/constant_traits.h"
#include "fern/algorithm/trigonometry/tan.h"


namespace fa = fern::algorithm;


BOOST_AUTO_TEST_SUITE(tan_)

template<
    class Value>
using OutOfDomainPolicy = fa::tan::OutOfDomainPolicy<Value>;


BOOST_AUTO_TEST_CASE(out_of_domain_policy)
{
    {
        OutOfDomainPolicy<double> policy;
        BOOST_CHECK(policy.within_domain(5));
        BOOST_CHECK(policy.within_domain(-5));
        BOOST_CHECK(policy.within_domain(0));

        BOOST_CHECK(!policy.within_domain(fern::infinity<double>()));
        BOOST_CHECK(!policy.within_domain(-fern::infinity<double>()));

        BOOST_CHECK(!policy.within_domain(1.0 * fern::half_pi<double>()));
        BOOST_CHECK( policy.within_domain(2.0 * fern::half_pi<double>()));
        BOOST_CHECK(!policy.within_domain(3.0 * fern::half_pi<double>()));

        BOOST_CHECK(!policy.within_domain(-1.0 * fern::half_pi<double>()));
        BOOST_CHECK( policy.within_domain(-2.0 * fern::half_pi<double>()));
        BOOST_CHECK(!policy.within_domain(-3.0 * fern::half_pi<double>()));
    }
}


template<
    class Value,
    class Result>
void verify_zero(
    Value const& value)
{
    Result result_we_want{0};
    Result result_we_get;
    fa::trigonometry::tan(fa::sequential, value, result_we_get);
    BOOST_CHECK_CLOSE(1.0 + result_we_get, 1.0 + result_we_want, 1e-10);
}


template<
    class Value,
    class Result>
void verify_value(
    Value const& value,
    Result const& result_we_want)
{
    Result result_we_get;
    fa::trigonometry::tan(fa::sequential, value, result_we_get);

    // TODO mingw 32 bit / gcc 4.8.2 requires us to use 4e-3. Other compilers
    //      allow the use of 1e-10.
    //      Check compiler and version and restore original epsilon.
    BOOST_CHECK_CLOSE(result_we_get, result_we_want, 4e-3);
}


BOOST_AUTO_TEST_CASE(algorithm)
{
    verify_zero<double, double>(0.0);
    verify_zero<double, double>(-0.0);

    verify_zero<double, double>(1.0 * fern::pi<double>());
    verify_zero<double, double>(2.0 * fern::pi<double>());
    verify_zero<double, double>(-1.0 * fern::pi<double>());
    verify_zero<double, double>(-2.0 * fern::pi<double>());

    verify_value<double, double>(fern::half_pi<double>(),
        std::tan(fern::half_pi<double>()));
    verify_value<double, double>(-fern::half_pi<double>(),
        std::tan(-fern::half_pi<double>()));
}

BOOST_AUTO_TEST_SUITE_END()
