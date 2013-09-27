#pragma once
#include "geoneric/io/gdal/dataset.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class GDALDataset:
    public Dataset
{

public:

    static bool    can_open            (String const& name);

                   GDALDataset         (String const& name);

                   GDALDataset         (GDALDataset const&)=delete;

    GDALDataset&   operator=           (GDALDataset const&)=delete;

                   GDALDataset         (GDALDataset&&)=delete;

    GDALDataset&   operator=           (GDALDataset&&)=delete;

                   ~GDALDataset        ()=default;

    size_t         nr_features         () const;

    bool           contains_feature    (String const& name) const;

    bool           contains_attribute  (String const& name) const;

    std::shared_ptr<Feature> read_feature(
                                        String const& name) const;

    std::shared_ptr<Attribute> read_attribute(
                                        String const& name) const;

private:

    bool           exists              () const;

};

} // namespace geoneric
