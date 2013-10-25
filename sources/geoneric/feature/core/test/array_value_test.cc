#define BOOST_TEST_MODULE geoneric feature
#include <boost/test/unit_test.hpp>
#include "geoneric/feature/core/array_value.h"


BOOST_AUTO_TEST_SUITE(value)

BOOST_AUTO_TEST_CASE(array)
{
    size_t const nr_dimensions = 1;
    size_t const nr_cells = 3;
    geoneric::ArrayValue<int, nr_dimensions> array(geoneric::extents[nr_cells]);

    array[0] = 3;
    array[1] = 2;
    array[2] = 1;

    BOOST_CHECK_EQUAL(array.size(), nr_cells);
    BOOST_CHECK_EQUAL(array[0], 3);
    BOOST_CHECK_EQUAL(array[1], 2);
    BOOST_CHECK_EQUAL(array[2], 1);
}


BOOST_AUTO_TEST_CASE(grid)
{
    size_t const nr_dimensions = 2;
    size_t const nr_rows = 3;
    size_t const nr_cols = 2;
    geoneric::ArrayValue<int, nr_dimensions> array(
        geoneric::extents[nr_rows][nr_cols]);

    array[0][0] = -2;
    array[0][1] = -1;
    array[1][0] = 0;
    array[1][1] = 999;
    array[2][0] = 1;
    array[2][1] = 2;

    BOOST_CHECK_EQUAL(array.size(), nr_rows);
    BOOST_CHECK_EQUAL(array[0].size(), nr_cols);
    BOOST_CHECK_EQUAL(array[0][0], -2);
    BOOST_CHECK_EQUAL(array[0][1], -1);
    BOOST_CHECK_EQUAL(array[1][0], 0);
    BOOST_CHECK_EQUAL(array[1][1], 999);
    BOOST_CHECK_EQUAL(array[2][0], 1);
    BOOST_CHECK_EQUAL(array[2][1], 2);
}

BOOST_AUTO_TEST_SUITE_END()