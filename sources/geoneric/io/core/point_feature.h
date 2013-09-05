#pragma once
#include "geoneric/io/feature.h"
#include "geoneric/io/point_ptr.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class PointFeature:
    public Feature
{

    friend class PointFeatureTest;

public:

                   PointFeature        (String const& name,
                                        PointDomainPtr const& domain);

                   ~PointFeature       ();

    PointDomain const& domain          () const;

    PointAttributes const& attributes  () const;

    bool           exists              (String const& name) const;

    void           add                 (PointAttributePtr const& attribute);

private:

    PointDomainPtr   _domain;

    PointAttributes  _attributes;

};

} // namespace geoneric