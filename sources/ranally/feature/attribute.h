#pragma once
#include "ranally/core/string.h"


namespace ranally {

// class Feature;

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class Attribute
{

    friend class AttributeTest;

public:

                   Attribute           (String const& name);

                   Attribute           (Attribute const&)=delete;

    Attribute&     operator=           (Attribute const&)=delete;

                   Attribute           (Attribute&&)=delete;

    Attribute&     operator=           (Attribute&&)=delete;

                   ~Attribute          ()=default;

    String const&  name                () const;

protected:

private:

    String         _name;

    // std::unique_ptr<Feature> _feature;

};

} // namespace ranally
