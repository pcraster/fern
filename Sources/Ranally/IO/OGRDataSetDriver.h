#ifndef INCLUDED_RANALLY_IO_OGRDATASETDRIVER
#define INCLUDED_RANALLY_IO_OGRDATASETDRIVER

#include "Ranally/IO/DataSetDriver.h"
#include "Ranally/IO/OGRDataSet.h"



namespace ranally {
namespace io {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class OGRDataSetDriver:
  public DataSetDriver
{

  friend class OGRDataSetDriverTest;

public:

                   OGRDataSetDriver    ();

                   ~OGRDataSetDriver   ();

  bool             exists              (UnicodeString const& name) const;

  OGRDataSet*      create              (UnicodeString const& name) const;

  void             remove              (UnicodeString const& name) const;

  OGRDataSet*      open                (UnicodeString const& name) const;

private:

};

} // namespace io
} // namespace ranally

#endif
