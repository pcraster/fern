#pragma once
#include "geoneric/core/string.h"
#include "geoneric/operation/core/operations.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class OperationXmlParser
{

    friend class OperationXmlParserTest;

public:

                   OperationXmlParser  ();

                   ~OperationXmlParser ();

                   OperationXmlParser  (OperationXmlParser&&)=delete;

    OperationXmlParser& operator=      (OperationXmlParser&&)=delete;

                   OperationXmlParser  (OperationXmlParser const&)=delete;

    OperationXmlParser& operator=      (OperationXmlParser const&)=delete;

    OperationsPtr  parse               (std::istream& stream) const;

    OperationsPtr  parse               (String const& xml) const;

private:

};

} // namespace geoneric
