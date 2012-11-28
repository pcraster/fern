#pragma once
#include <memory>
#include "ranally/io/attribute.h"
#include "ranally/io/raster_value.h"


namespace ranally {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class RasterAttribute:
    public Attribute
{

    friend class RasterAttributeTest;

public:

                   RasterAttribute     (String const& name);

                   ~RasterAttribute    ();

private:

    //! Value.
    std::unique_ptr<RasterValue> _value;

};

} // namespace ranally