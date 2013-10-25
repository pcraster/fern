#include "geoneric/io/core/feature.h"


namespace geoneric {

Feature::Feature(
    String const& name,
    Domain::Type domainType)

    : _name(name),
      _domain_type(domainType)

{
}


Feature::~Feature()
{
}


String const& Feature::name() const
{
    return _name;
}


Domain::Type Feature::domain_type() const
{
    return _domain_type;
}

} // namespace geoneric