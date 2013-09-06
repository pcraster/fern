#pragma once
#include "geoneric/io/core/dataset_driver.h"
#include "geoneric/io/ogr/ogr_dataset.h"


class OGRSFDriver;

namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class OGRDatasetDriver:
    public DatasetDriver
{

    friend class OGRDatasetDriverTest;

public:

                   OGRDatasetDriver    (String const& name);

                   ~OGRDatasetDriver   ();

    bool           exists              (String const& name) const;

    OGRDataset*    create              (String const& name) const;

    void           remove              (String const& name) const;

    OGRDataset*    open                (String const& name) const;

private:

    OGRSFDriver*   _driver;

};

} // namespace geoneric
