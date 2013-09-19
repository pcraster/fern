#include "geoneric/io/gdal/feature.h"


namespace geoneric {

bool Feature::has_attribute(
    String const& name)
{
    return find(name) != end();
}

} // namespace geoneric
