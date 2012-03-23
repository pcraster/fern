#ifndef INCLUDED_RANALLY_POINTATTRIBUTE
#define INCLUDED_RANALLY_POINTATTRIBUTE

#include "Ranally/IO/Attribute.h"
#include "Ranally/IO/PointPtr.h"



namespace ranally {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class PointAttribute:
  public Attribute
{

  friend class PointAttributeTest;

public:

                   PointAttribute      (UnicodeString const& name,
                                        PointDomainPtr const& domain);

                   ~PointAttribute     ();

  PointDomainPtr const& domain         () const;

  PointFeaturePtr const& feature       () const;

  PointValues const& values            () const;

private:

  PointDomainPtr   _domain;

  PointFeaturePtr  _feature;

  PointValues      _values;

};

} // namespace ranally

#endif
