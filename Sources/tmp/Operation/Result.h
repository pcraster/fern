#pragma once
#include "Ranally/Language/Operation/Parameter.h"


namespace ranally {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class Result:
    public Parameter
{

    friend class ResultTest;

public:

                   Result              ();

                   ~Result             ();

private:

};

} // namespace ranally