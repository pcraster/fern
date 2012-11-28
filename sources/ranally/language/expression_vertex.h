#pragma once
#include <tuple>
#include <vector>
#include "ranally/operation/data_type.h"
#include "ranally/operation/value_type.h"
#include "ranally/language/statement_vertex.h"


namespace ranally {

class ExpressionVertex;

typedef std::shared_ptr<ExpressionVertex> ExpressionVertexPtr;

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  TODO Make sure expressions support multiple results.

  \sa        .
*/
class ExpressionVertex:
    public StatementVertex
{

    friend class ExpressionVertexTest;

public:

    typedef std::tuple<DataType, ValueType> ResultType;

    virtual        ~ExpressionVertex   ()=default;

                   ExpressionVertex    (ExpressionVertex&&)=delete;

    ExpressionVertex& operator=        (ExpressionVertex&&)=delete;

                   ExpressionVertex    (ExpressionVertex const&)=delete;

    ExpressionVertex& operator=        (ExpressionVertex const&)=delete;

    String const&  name                () const;

    // void           setDataType         (operation::DataType data_type);

    // operation::DataType data_type         () const;

    // void           setValueType        (operation::ValueType value_type);

    // operation::ValueType value_type       () const;

    void           setResultTypes      (
                                  std::vector<ResultType> const& resultTypes);

    void           addResultType       (DataType data_type,
                                        ValueType value_type);

    std::vector<ResultType> const& resultTypes() const;

    DataType       data_type           (size_t index) const;

    ValueType      value_type          (size_t index) const;

    void           setValue            (ExpressionVertexPtr const& value);

    ExpressionVertexPtr const& value   () const;

protected:

                   ExpressionVertex    (String const& name);

                   ExpressionVertex    (int lineNr,
                                        int colId,
                                        String const& name);

private:

    //! Name of the expression, eg: abs, myDog, 5.
    String         _name;

    // //! Data type of the result(s) of the expression.
    // operation::DataType _data_type;

    // //! Value type of the result(s) of the expression.
    // operation::ValueType _value_type;

    // typedef boost::tuple<operation::DataType, operation::ValueType> ResultType;

    std::vector<ResultType> _resultTypes;

    //! Value of expression, in case this expression is the target of an assignment.
    ExpressionVertexPtr _value;

};

} // namespace ranally