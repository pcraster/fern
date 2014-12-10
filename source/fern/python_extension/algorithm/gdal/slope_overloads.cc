#include <Python.h>
#include "fern/python_extension/algorithm/gdal/slope_overloads.h"
#include "fern/python_extension/algorithm/numpy/util.h"
#include "fern/python_extension/algorithm/gdal/slope.h"
#include "fern/python_extension/algorithm/gdal/util.h"


namespace fern {
namespace python {
namespace gdal {

PyObject* slope_gdal_raster_band(
    PyObject* object)
{
    return python_object(slope(
        gdal_raster_band(object)));
}


UnaryOperationMap<UnaryAlgorithmKey> slope_overloads{
    { UnaryAlgorithmKey(WrappedDataType::gdal_raster_band),
        slope_gdal_raster_band }
};

} // namespace gdal
} // namespace python
} // namespace fern
