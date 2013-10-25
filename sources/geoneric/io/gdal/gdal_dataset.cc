#include "geoneric/io/gdal/gdal_dataset.h"
#include "gdal_priv.h"
#include "geoneric/core/io_error.h"
#include "geoneric/core/path.h"
#include "geoneric/core/value_type_traits.h"
#include "geoneric/feature/visitor/attribute_type_visitor.h"
#include "geoneric/io/core/file.h"
#include "geoneric/io/gdal/gdal_data_type_traits.h"
#include "geoneric/io/gdal/gdal_type_traits.h"


namespace geoneric {

GDALDataset::GDALDataset(
    String const& name,
    OpenMode open_mode)

    : Dataset(name, open_mode),
      _driver(0),
      _dataset(0)

{
    _driver = open_mode == OpenMode::READ
        ? gdal_driver_for_read(this->name())
        : gdal_driver_for_update(this->name())
        ;

    if(!_driver) {
        // No driver that supports IO for this name.
        assert(false);
    }

    _dataset = open_mode == OpenMode::READ
        ? gdal_open_for_read(this->name())
        : gdal_open_for_update(this->name())
        ;
}


GDALDataset::GDALDataset(
    ::GDALDriver* driver,
    String const& name,
    OpenMode open_mode)

    : Dataset(name, open_mode),
      _driver(driver),
      _dataset(0)

{
    assert(driver);
    _dataset = open_mode == OpenMode::READ
        ? gdal_open_for_read(this->name())
        : gdal_open_for_update(this->name())
        ;
}


GDALDataset::GDALDataset(
    ::GDALDataset* dataset,
    String const& name,
    OpenMode open_mode)

    : Dataset(name, open_mode),
      _driver(dataset->GetDriver()),
      _dataset(dataset)

{
}


GDALDataset::~GDALDataset()
{
    assert(_dataset);
    GDALClose(_dataset);
}


::GDALDriver* GDALDataset::gdal_driver_for_read(
    String const& name)
{
    ::GDALDataset* dataset = static_cast<::GDALDataset*>(GDALOpen(
        name.encode_in_default_encoding().c_str(), GA_ReadOnly));
    ::GDALDriver* result = nullptr;

    if(dataset) {
        result = dataset->GetDriver();
        GDALClose(dataset);
    }

    return result;
}


::GDALDriver* GDALDataset::gdal_driver_for_update(
    String const& name)
{
    ::GDALDataset* dataset = static_cast<::GDALDataset*>(GDALOpen(
        name.encode_in_default_encoding().c_str(), GA_Update));
    ::GDALDriver* result = nullptr;

    if(dataset) {
        result = dataset->GetDriver();
        GDALClose(dataset);
    }

    return result;
}


::GDALDataset* GDALDataset::gdal_open_for_read(
    String const& name)
{
    GDALOpenInfo open_info(name.encode_in_default_encoding().c_str(),
        GA_ReadOnly);
    ::GDALDataset* dataset = static_cast<::GDALDataset*>(_driver->pfnOpen(
        &open_info));

    if(!dataset) {
        if(!file_exists(name)) {
            throw IOError(name,
                Exception::messages()[MessageId::DOES_NOT_EXIST]);
        }
        else {
            throw IOError(name,
                Exception::messages()[MessageId::CANNOT_BE_READ]);
        }
    }

    return dataset;
}


::GDALDataset* GDALDataset::gdal_open_for_update(
    String const& name)
{
    GDALOpenInfo open_info(name.encode_in_default_encoding().c_str(),
        GA_Update);
    ::GDALDataset* dataset = static_cast<::GDALDataset*>(_driver->pfnOpen(
        &open_info));

    if(!dataset) {
        throw IOError(name,
            Exception::messages()[MessageId::CANNOT_BE_WRITTEN]);
    }

    return dataset;
}


size_t GDALDataset::nr_features() const
{
    return 1u;
}


bool GDALDataset::contains_feature(
    Path const& path) const
{
    // GDAL raster datasets contain one root feature, but no sub-features.
    // /<raster>
    return path == String("/") + String(Path(this->name()).stem());
}


bool GDALDataset::contains_attribute(
    Path const& path) const
{
    // The name of the one attribute in a GDAL raster equals the name of the
    // dataset without leading path and extension. It is prefixed by the path
    // to the feature, which is also named after the raster.
    // /<raster>/<raster>
    return Path("/" + String(Path(this->name()).stem()) + "/" +
        String(Path(this->name()).stem())) == path;
}


std::shared_ptr<Feature> GDALDataset::read_feature(
    Path const& path) const
{
    if(!contains_feature(path)) {
        throw IOError(this->name(),
            Exception::messages().format_message(
                MessageId::DOES_NOT_CONTAIN_FEATURE, path));
    }

    Path attribute_path = path + "/" + Path(this->name()).stem();
    assert(contains_attribute(attribute_path));

    std::shared_ptr<Feature> feature(new Feature());
    feature->add_attribute(String(attribute_path.filename()),
        std::dynamic_pointer_cast<Attribute>(read_attribute(attribute_path)));

    return feature;
}


template<
    class T>
std::shared_ptr<Attribute> GDALDataset::read_attribute(
    GDALRasterBand& band) const
{
    int const nr_rows = _dataset->GetRasterYSize();
    int const nr_cols = _dataset->GetRasterXSize();

    double geo_transform[6];

    if(_dataset->GetGeoTransform(geo_transform) != CE_None) {
        // According to the gdal docs:
        // The default transform is (0,1,0,0,0,1) and should be returned even
        // when a CE_Failure error is returned, such as for formats that don't
        // support transformation to projection coordinates.
        geo_transform[0] = 0.0;
        geo_transform[1] = 1.0;
        geo_transform[2] = 0.0;
        geo_transform[3] = 0.0;
        geo_transform[4] = 0.0;
        geo_transform[5] = 1.0;
    }

    assert(geo_transform[1] == std::abs(geo_transform[5]));
    double const cell_size = geo_transform[1];

    d2::Point south_west, north_east;
    set<0>(south_west, geo_transform[0]);
    set<1>(north_east, geo_transform[3]);
    set<0>(north_east, get<0>(south_west) + nr_cols * cell_size);
    set<1>(south_west, get<1>(north_east) - nr_rows * cell_size);

    d2::Box box(south_west, north_east);

    FieldValuePtr<T> array(new FieldValue<T>(extents[nr_rows][nr_cols]));

    assert(band.GetRasterDataType() == GDALTypeTraits<T>::data_type);
    if(band.RasterIO(GF_Read, 0, 0, nr_cols, nr_rows, array->data(), nr_cols,
            nr_rows, GDALTypeTraits<T>::data_type, 0, 0) != CE_None) {
        // This shouldn't happen.
        throw IOError(this->name(),
            Exception::messages()[MessageId::UNKNOWN_ERROR]);
    }

    // http://trac.osgeo.org/gdal/wiki/rfc15_nodatabitmask
    int mask_flags = band.GetMaskFlags();
    if(!(mask_flags & GMF_ALL_VALID)) {
        assert(!(mask_flags & GMF_ALPHA));
        GDALRasterBand* mask_band = band.GetMaskBand();
        assert(mask_band->GetRasterDataType() == GDT_Byte);
        // The mask band has gdal data type GDT_Byte. A value of zero
        // means that the value must be masked.
        ArrayValue<typename GDALDataTypeTraits<GDT_Byte>::type, 2> mask(
            extents[nr_rows][nr_cols]);

        if(mask_band->RasterIO(GF_Read, 0, 0, nr_cols, nr_rows, mask.data(),
                nr_cols, nr_rows, GDT_Byte, 0, 0) != CE_None) {
            // This shouldn't happen.
            throw IOError(this->name(),
                Exception::messages()[MessageId::UNKNOWN_ERROR]);
        }

        array->set_mask(mask);
    }

    FieldAttributePtr<T> attribute(new FieldAttribute<T>());
    typename FieldAttribute<T>::GID gid = attribute->add(box, array);

    return std::dynamic_pointer_cast<Attribute>(attribute);
}


std::shared_ptr<Attribute> GDALDataset::read_attribute(
    Path const& path) const
{
    assert(_dataset);

    if(!contains_attribute(path)) {
        throw IOError(this->name(),
            Exception::messages().format_message(
                MessageId::DOES_NOT_CONTAIN_ATTRIBUTE, path));
    }

    // Assume we need the first band only.
    assert(_dataset->GetRasterCount() == 1);

    GDALRasterBand* band = _dataset->GetRasterBand(1);
    assert(band);

    std::shared_ptr<Attribute> result;

    switch(band->GetRasterDataType()) {
        case GDT_Byte: {
            result = read_attribute<uint8_t>(*band);
            break;
        }
        case GDT_UInt16: {
            result = read_attribute<uint16_t>(*band);
            break;
        }
        case GDT_Int16: {
            result = read_attribute<int16_t>(*band);
            break;
        }
        case GDT_UInt32: {
            result = read_attribute<uint32_t>(*band);
            break;
        }
        case GDT_Int32: {
            result = read_attribute<int32_t>(*band);
            break;
        }
        case GDT_Float32: {
            result = read_attribute<float>(*band);
            break;
        }
        case GDT_Float64: {
            result = read_attribute<double>(*band);
            break;
        }
        case GDT_CInt16: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_CInt16>::name));
        }
        case GDT_CInt32: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_CInt32>::name));
        }
        case GDT_CFloat32: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_CFloat32>::name));
        }
        case GDT_CFloat64: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_CFloat64>::name));
        }
        case GDT_TypeCount: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_TypeCount>::name));
        }
        case GDT_Unknown: {
            throw IOError(this->name(),
                Exception::messages().format_message(
                    MessageId::UNSUPPORTED_VALUE_TYPE,
                    path, GDALDataTypeTraits<GDT_Unknown>::name));
        }
    }

    assert(result);
    return result;
}


template<
    class T>
void GDALDataset::write_attribute(
    FieldAttribute<T> const& field,
    Path const& path)
{
    // It is assumed that the layered dataset is dimensioned correctly and
    // ready to receive values from the field attribute.
    assert(open_mode() == OpenMode::OVERWRITE ||
        open_mode() == OpenMode::UPDATE);
    assert(contains_feature(path.parent_path()));
    assert(contains_attribute(path));
    assert(_dataset);
    assert(_dataset->GetRasterCount() == 1);

    assert(field.values().size() == 1u);
    FieldValue<T> const& array(*field.values().cbegin()->second);
    int nr_rows = array.shape()[0];
    int nr_cols = array.shape()[1];

    double geo_transform[6];
    {
        FieldDomain const& domain(field.domain());
        assert(domain.size() == 1u);
        d2::Box const& box(domain.cbegin()->second);
        geo_transform[0] = get<0>(box.min_corner());  // west
        geo_transform[1] =
            (get<0>(box.max_corner()) - get<0>(box.min_corner())) / nr_cols;
        geo_transform[2] = 0.0;
        geo_transform[3] = get<1>(box.max_corner());  // north
        geo_transform[4] = 0.0;
        geo_transform[5] =
            (get<1>(box.max_corner()) - get<1>(box.min_corner())) / nr_rows;
        assert(geo_transform[1] > 0.0);
        assert(geo_transform[1] == geo_transform[5]);
    }

    if(_dataset->SetGeoTransform(geo_transform) != CE_None) {
        throw IOError(this->name(),
            Exception::messages()[MessageId::CANNOT_BE_WRITTEN]);
    }

    GDALRasterBand* band = _dataset->GetRasterBand(1);
    assert(band);

    if(band->RasterIO(GF_Write, 0, 0, nr_cols, nr_rows, const_cast<T*>(
            array.data()), nr_cols, nr_rows, GDALTypeTraits<T>::data_type,
            0, 0) != CE_None) {
        throw IOError(this->name(),
            Exception::messages()[MessageId::CANNOT_BE_WRITTEN]);
    }

    if(array.has_masked_values()) {
        // This assumes the dataset contains onle one band. The mask is taken to
        // be global to the dataset.
        if(band->CreateMaskBand(GMF_PER_DATASET) != CE_None) {
            throw IOError(this->name(),
                Exception::messages()[MessageId::CANNOT_BE_WRITTEN]);
        }

        GDALRasterBand* mask_band = band->GetMaskBand();
        assert(mask_band->GetRasterDataType() == GDT_Byte);
        // The mask band has gdal data type GDT_Byte. A value of zero
        // means that the value must be masked.
        ArrayValue<typename GDALDataTypeTraits<GDT_Byte>::type, 2> mask(
            extents[nr_rows][nr_cols]);
        array.mask(mask);

        if(mask_band->RasterIO(GF_Write, 0, 0, nr_cols, nr_rows, mask.data(),
                nr_cols, nr_rows, GDT_Byte, 0, 0) != CE_None) {
            throw IOError(this->name(),
                Exception::messages()[MessageId::CANNOT_BE_WRITTEN]);
        }
    }
}


#define WRITE_ATTRIBUTE_CASE(                                                  \
        value_type)                                                            \
case value_type: {                                                             \
    write_attribute(                                                           \
        dynamic_cast<FieldAttribute<                                           \
            ValueTypeTraits<value_type>::type> const&>(attribute), path);      \
    break;                                                                     \
}

void GDALDataset::write_attribute(
    Attribute const& attribute,
    Path const& path)
{
    AttributeTypeVisitor visitor;
    attribute.Accept(visitor);

    switch(visitor.data_type()) {
        case DT_CONSTANT: {
            assert(false); // Data type not supported by driver.
            break;
        }
        case DT_STATIC_FIELD: {
            switch(visitor.value_type()) {
                WRITE_ATTRIBUTE_CASE(VT_UINT8)
                WRITE_ATTRIBUTE_CASE(VT_UINT16)
                WRITE_ATTRIBUTE_CASE(VT_INT16)
                WRITE_ATTRIBUTE_CASE(VT_UINT32)
                WRITE_ATTRIBUTE_CASE(VT_INT32)
                WRITE_ATTRIBUTE_CASE(VT_FLOAT32)
                WRITE_ATTRIBUTE_CASE(VT_FLOAT64)
                case VT_INT8:
                case VT_UINT64:
                case VT_INT64:
                case VT_STRING: {
                    assert(false); // Value type not supported by driver.
                    break;
                }
            }
            break;
        }
    }
}

#undef WRITE_ATTRIBUTE_CASE

} // namespace geoneric