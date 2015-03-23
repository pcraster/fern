#include <cstddef>
#include "fern/feature/core/raster.h"
#include "pow.h"


int main(
    int /* argc */,
    char** /* argv */)
{
    using Raster = fern::Raster<double, 2>;

    size_t const nr_rows{2};
    size_t const nr_cols{3};
    double west{0.0};
    double north{0.0};
    double cell_size{10.0};
    Raster::Transformation transformation{{west, cell_size, north, cell_size}};
    Raster base(fern::extents[nr_rows][nr_cols], transformation);

    // Assign values to the raster.
    // ...

    double exponent{2};

    Raster result(fern::extents[nr_rows][nr_cols], transformation);

    pow(base, exponent, result);

    return EXIT_SUCCESS;
}
