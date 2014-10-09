#pragma once
#include "fern/feature/core/attributes.h"
#include "fern/io/core/dataset.h"


namespace H5 {
    class DataSet;
    class Group;
    class H5File;
}


namespace fern {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class FernDataset:
    public Dataset
{

public:

                   FernDataset         (String const& name,
                                        OpenMode open_mode);

                   FernDataset         (std::shared_ptr<H5::H5File> const& file,
                                        String const& name,
                                        OpenMode open_mode);

                   FernDataset         (FernDataset const&)=delete;

    FernDataset&   operator=           (FernDataset const&)=delete;

                   FernDataset         (FernDataset&&)=delete;

    FernDataset&   operator=           (FernDataset&&)=delete;

                   ~FernDataset        ();

    size_t         nr_features         () const;

    size_t         nr_features         (Path const& path) const;

    std::vector<String>
                   feature_names       () const;

    size_t         nr_attributes       (Path const& path) const;

    bool           contains_feature    (Path const& path) const;

    bool           contains_attribute  (Path const& path) const;

    std::shared_ptr<Feature>
                   open_feature        (Path const& path) const;

    std::shared_ptr<Attribute>
                   open_attribute      (Path const& path) const;

    ExpressionType expression_type     (Path const& path) const;

    std::shared_ptr<Feature> read_feature(
                                        Path const& Path) const;

    std::shared_ptr<Attribute> read_attribute(
                                        Path const& Path) const;

    void           write_attribute     (Attribute const& attribute,
                                        Path const& Path);

    template<
        class T>
    void           write_attribute     (ConstantAttribute<T> const& constant,
                                        Path const& Path);

private:

    std::shared_ptr<H5::H5File> _file;

    H5::DataSet    dataset             (Path const& path) const;

    ValueType      value_type          (H5::DataSet const& dataset) const;

    std::shared_ptr<H5::Group> group   (Path const& path) const;

    bool           contains_feature    (std::vector<String> const& names) const;

    bool           contains_feature_by_name(
                                        String const& pathname) const;

    bool           contains_attribute_by_name(
                                        String const& pathname) const;

    void           add_feature         (Path const& path);

    void           add_feature         (std::vector<String> const& names);

    template<
        class T>
    ExpressionType expression_type_numeric_attribute(
                                        H5::DataSet const& dataset) const;

    template<
        class T>
    std::shared_ptr<Attribute>
                   open_attribute      (H5::DataSet const& dataset) const;

    template<
        class T>
    std::shared_ptr<Attribute>
                   read_constant_attribute(
                                        H5::DataSet const& dataset) const;

    template<
        class T>
    std::shared_ptr<Attribute>
                   read_attribute      (H5::DataSet const& dataset) const;

};

} // namespace fern