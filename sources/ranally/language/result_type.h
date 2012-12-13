#pragma once
#include "ranally/operation/data_types.h"
#include "ranally/operation/value_types.h"


namespace ranally {

class ResultType
{

    friend class ResultTypeTest;

public:

                   ResultType          ()=default;

                   ResultType          (DataTypes const& data_types,
                                        ValueTypes const& value_types);

                   ResultType          (ResultType&&)=default;

    ResultType&    operator=           (ResultType&&)=default;

                   ResultType          (ResultType const&)=default;

    ResultType&    operator=           (ResultType const&)=default;

                   ~ResultType         ()=default;

    DataTypes      data_type           () const;

    ValueTypes     value_type          () const;

    bool           fixed               () const;

private:

    DataTypes      _data_types;

    ValueTypes     _value_types;

};

} // namespace ranally