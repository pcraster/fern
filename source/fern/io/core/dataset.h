#pragma once
#include <memory>
#include "fern/core/expression_type.h"
#include "fern/core/path.h"
#include "fern/core/string.h"
#include "fern/feature/core/feature.h"
#include "fern/io/core/open_mode.h"


namespace fern {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class Dataset
{

public:

    String const&  name                () const;

    OpenMode       open_mode           () const;

    //! Return the number of features in the dataset.
    /*!
        Nested features are not included in the count.
    */
    virtual size_t nr_features         () const=0;

    virtual std::vector<String>
                   feature_names       () const=0;

    virtual bool   contains_feature    (Path const& path) const=0;

    virtual bool   contains_attribute  (Path const& path) const=0;

    virtual ExpressionType
                   expression_type     (Path const& path) const=0;

    //! Open a feature and return the result.
    /*!
      \param     path Path to the feature to be opened.
      \exception .
      \warning   .
      \sa        open_attribute(Path const&)

      Opening a feature differs from reading it in that no data is read. Only
      properties are read. It is important that opening an feature is
      efficient.
    */
    virtual std::shared_ptr<Feature>
                   open_feature        (Path const& path) const=0;

    //! Open an attribute and return the result.
    /*!
      \param     path Path to the attribute to be opened.
      \exception .
      \warning   .
      \sa        open_feature(Path const&)

      Opening an attribute differs from reading it in that no data is read.
      Only properties are read. It is important that opening an attribute is
      efficient.
    */
    virtual std::shared_ptr<Attribute>
                   open_attribute      (Path const& path) const=0;

    virtual std::shared_ptr<Feature>
                   read_feature        (Path const& path) const=0;

    virtual std::shared_ptr<Attribute>
                   read_attribute      (Path const& path) const=0;

    virtual void   write_attribute     (Attribute const& attribute,
                                        Path const& path)=0;

protected:

                   Dataset             (String const& name,
                                        OpenMode open_mode);

                   Dataset             (Dataset const&)=delete;

    Dataset&       operator=           (Dataset const&)=delete;

                   Dataset             (Dataset&&)=delete;

    Dataset&       operator=           (Dataset&&)=delete;

    virtual        ~Dataset            ()=default;

private:

    String         _name;

    OpenMode       _open_mode;

};

} // namespace fern