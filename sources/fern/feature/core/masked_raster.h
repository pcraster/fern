#pragma once
#include <array>
#include "fern/feature/core/masked_array.h"


namespace fern {


//! Class for rasters with masked values, with a cartesian coordinate system.
/*!
  \tparam    .
  \param     .
  \return    .
  \exception .
  \warning   .
  \sa        .
*/
template<
    class T,
    size_t nr_dimensions>
class MaskedRaster:
    public MaskedArray<T, nr_dimensions>
{

public:

    //! Information for transforming cell indices to projection coordinates.
    /*!
        For each dimension, this array contains:
        - Projection coordinate of the border of the first cell.
        - Size of each cell.

        With this information, cell indices can be translated to projection
        coordinates, and vice versa.

        For example, in case of a 2D raster, this array can be used like this:

        \code
        Transformation transformation{{west, cell_width, north, cell_height}};
        \endcode

        In case of a 2D raster, the cell indices of the upper left cell
        are 0, 0. The center of this cell has cell indices 0.5, 0.5.
    */
    using Transformation = std::array<double, 2 * nr_dimensions>;

    template<size_t nr_ranges>
                   MaskedRaster        (gen_type<nr_ranges> const& sizes,
                                        Transformation const& transformation);

    Transformation const&
                   transformation      () const;

private:

    Transformation _transformation;

};


template<
    class T,
    size_t nr_dimensions>
template<
    size_t nr_ranges>
inline MaskedRaster<T, nr_dimensions>::MaskedRaster(
    gen_type<nr_ranges> const& sizes,
    Transformation const& transformation)

    : MaskedArray<T, nr_dimensions>(sizes),
      _transformation(transformation)

{
}


template<
    class T,
    size_t nr_dimensions>
inline typename MaskedRaster<T, nr_dimensions>::Transformation const&
    MaskedRaster<T, nr_dimensions>::transformation() const
{
    return _transformation;
}

} // namespace fern