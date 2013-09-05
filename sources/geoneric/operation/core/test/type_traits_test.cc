#define BOOST_TEST_MODULE geoneric operation_core
#include <boost/test/unit_test.hpp>
#include "geoneric/operation/core/type_traits.h"


BOOST_AUTO_TEST_SUITE(result_type)

BOOST_AUTO_TEST_CASE(result_type)
{
    BOOST_CHECK_EQUAL(geoneric::TypeTraits<uint8_t>::value_type,
        geoneric::VT_UINT8);
    BOOST_CHECK_EQUAL(geoneric::TypeTraits<uint8_t>::value_types,
        geoneric::ValueTypes::UINT8);
}

BOOST_AUTO_TEST_SUITE_END()