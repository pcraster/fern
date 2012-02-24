#ifndef INCLUDED_RANALLY_POLYGONDOMAIN
#define INCLUDED_RANALLY_POLYGONDOMAIN

#include "Ranally/IO/Domain.h"
#include "Ranally/IO/Geometry.h"



namespace ranally {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class PolygonDomain:
  public Domain
{

  friend class PolygonDomainTest;

public:

                   PolygonDomain       (PolygonsPtr const& polygons);

                   ~PolygonDomain      ();

private:

  PolygonsPtr      _polygons;

};

} // namespace ranally

#endif
