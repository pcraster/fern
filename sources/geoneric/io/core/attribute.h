#pragma once
#include "geoneric/core/string.h"


namespace geoneric {

//! Class for Attribute instances.
/*!
  \sa        .
*/
class Attribute
{

    friend class AttributeTest;

public:

                   Attribute           (Attribute const&)=delete;

    Attribute&     operator=           (Attribute const&)=delete;

    virtual        ~Attribute          ();

    String const&  name                () const;

protected:

                   Attribute           (String const& name);

private:

    String         _name;

};

} // namespace geoneric
