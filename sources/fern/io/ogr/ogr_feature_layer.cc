#include "fern/io/ogr/ogr_feature_layer.h"
#include "ogrsf_frmts.h"
#include "fern/core/string.h"
#include "fern/io/core/point_domain.h"
#include "fern/io/core/polygon_domain.h"


namespace fern {
namespace {

Domain::Type domain_type(
    OGRwkbGeometryType geometryType)
{
    Domain::Type domain_type;

    switch(geometryType) {
        case wkbPoint: {
            domain_type = Domain::PointDomain;
        }
        case wkbPolygon: {
            domain_type = Domain::PolygonDomain;
        }
#ifndef NDEBUG
        default: {
            assert(false);
        }
#endif
    }

    return domain_type;
}

} // Anonymous namespace.


OGRFeatureLayer::OGRFeatureLayer(
    OGRLayer* const layer)

    : _layer(layer),
      _domain()

{
    switch(domain_type(_layer->GetGeomType())) {
        case Domain::PointDomain: {
            PointsPtr points;
            _domain = std::make_shared<PointDomain>(points);
        }
        case Domain::PolygonDomain: {
            PolygonsPtr polygons;
            _domain = std::make_shared<PolygonDomain>(polygons);
        }
#ifndef NDEBUG
        default: {
            assert(false);
        }
#endif
    }

    assert(_domain);
}


OGRFeatureLayer::~OGRFeatureLayer()
{
}


String OGRFeatureLayer::name() const
{
    return String(_layer->GetName());
}


Domain const& OGRFeatureLayer::domain() const
{
    assert(_domain);
    return *_domain;
}

} // namespace fern
