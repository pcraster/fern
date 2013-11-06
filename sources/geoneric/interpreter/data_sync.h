#pragma once
#include "geoneric/core/expression_type.h"
#include "geoneric/operation/core/argument.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        DataSource
*/
class DataSync
{

public:

    virtual void   write               (Argument const& argument)=0;

protected:

                   DataSync            ()=default;

                   DataSync            (DataSync const&)=delete;

    DataSync&      operator=           (DataSync const&)=delete;

                   DataSync            (DataSync&&)=delete;

    DataSync&      operator=           (DataSync&&)=delete;

    virtual        ~DataSync           ()=default;

private:

};

} // namespace geoneric
