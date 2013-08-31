#pragma once
#include "geoneric/core/string.h"
#include "geoneric/io/domain.h"


namespace geoneric {

//! Class for Feature instances combining Domain with an Attribute.
/*!
  \sa        .
*/
class Feature
{

    friend class FeatureTest;

public:

                   Feature             (Feature const&)=delete;

    Feature&       operator=           (Feature const&)=delete;

    virtual        ~Feature            ();

    String const&  name                () const;

    Domain::Type   domain_type         () const;

protected:

                   Feature             (String const& name,
                                        Domain::Type domain_type);

private:

    String         _name;

    Domain::Type   _domain_type;

};

} // namespace geoneric
