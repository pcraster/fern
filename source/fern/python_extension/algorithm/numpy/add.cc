#include "fern/python_extension/algorithm/numpy/add.h"
#include <functional>
#include <map>
#include <tuple>
#include "fern/feature/core/array_reference_traits.h"
#include "fern/algorithm/algebra/elementary/add.h"
#include "fern/python_extension/core/error.h"
#include "fern/python_extension/algorithm/numpy/numpy_type_traits.h"
#include "fern/python_extension/algorithm/numpy/util.h"


namespace fern {
namespace python {
namespace detail {

static void init_numpy()
{
    import_array();
}


namespace array_number {

template<
    typename Value1,
    typename Value2>
PyArrayObject* add(
    PyArrayObject* array_object,
    Value2 const& value)
{
    init_numpy();

    // TODO Switch on number of dimensions.
    assert(PyArray_NDIM(array_object) == 2);

    size_t const size1{static_cast<size_t>(PyArray_DIM(array_object, 0))};
    size_t const size2{static_cast<size_t>(PyArray_DIM(array_object, 1))};

    ArrayReference<Value1, 2> array_2d_reference(
        static_cast<Value1*>(PyArray_DATA(const_cast<PyArrayObject*>(
            array_object))), extents[size1][size2]);

    using result_value_type = algorithm::add::result_value_type<Value1, Value2>;

    PyArrayObject* result_object{(PyArrayObject*)(
        PyArray_SimpleNew(
            PyArray_NDIM(array_object),
            PyArray_DIMS(const_cast<PyArrayObject*>(array_object)),
            NumpyTypeTraits<result_value_type>::data_type))};

    ArrayReference<result_value_type, 2> result_reference(
        static_cast<result_value_type*>(PyArray_DATA(result_object)),
        extents[size1][size2]);

    algorithm::algebra::add(algorithm::parallel, array_2d_reference,
        value, result_reference);

    return result_object;
}


#define ADD_OVERLOAD(                                      \
    algorithm,                                             \
    type1,                                                 \
    type2)                                                 \
PyArrayObject* algorithm##_##type1##_##type2(              \
    PyArrayObject* array,                                  \
    type2##_t const& value)                                \
{                                                          \
    return algorithm<type1##_t, type2##_t>(array, value);  \
}


#define ADD_OVERLOADS(                  \
    algorithm,                          \
    type)                               \
ADD_OVERLOAD(algorithm, type, uint8)    \
ADD_OVERLOAD(algorithm, type, int8)     \
ADD_OVERLOAD(algorithm, type, uint16)   \
ADD_OVERLOAD(algorithm, type, int16)    \
ADD_OVERLOAD(algorithm, type, uint32)   \
ADD_OVERLOAD(algorithm, type, int32)    \
ADD_OVERLOAD(algorithm, type, uint64)   \
ADD_OVERLOAD(algorithm, type, int64)    \
ADD_OVERLOAD(algorithm, type, float32)  \
ADD_OVERLOAD(algorithm, type, float64)


ADD_OVERLOADS(add, uint8)
ADD_OVERLOADS(add, int8)
ADD_OVERLOADS(add, uint16)
ADD_OVERLOADS(add, int16)
ADD_OVERLOADS(add, uint32)
ADD_OVERLOADS(add, int32)
ADD_OVERLOADS(add, uint64)
ADD_OVERLOADS(add, int64)
ADD_OVERLOADS(add, float32)
ADD_OVERLOADS(add, float64)


#undef ADD_OVERLOADS
#undef ADD_OVERLOAD

} // namespace array_number


namespace array_float64 {

using AddOverloadsKey = int;
using AddOverload = std::function<PyArrayObject*(PyArrayObject*, float64_t)>;
using AddOverloads = std::map<AddOverloadsKey, AddOverload>;


static AddOverloads add_overloads = {
    { AddOverloadsKey(NPY_UINT8   ), array_number::add_uint8_float64   },
    { AddOverloadsKey(NPY_INT8    ), array_number::add_int8_float64    },
    { AddOverloadsKey(NPY_UINT16  ), array_number::add_uint16_float64  },
    { AddOverloadsKey(NPY_INT16   ), array_number::add_int16_float64   },
    { AddOverloadsKey(NPY_UINT32  ), array_number::add_uint32_float64  },
    { AddOverloadsKey(NPY_INT32   ), array_number::add_int32_float64   },
    { AddOverloadsKey(NPY_FLOAT32 ), array_number::add_float32_float64 },
    { AddOverloadsKey(NPY_FLOAT64 ), array_number::add_float64_float64 }
};

} // namespace array_float64


namespace array_array {

template<
    typename Value1,
    typename Value2>
PyArrayObject* add(
    PyArrayObject* array_object1,
    PyArrayObject* array_object2)
{
    init_numpy();

    // TODO Switch on number of dimensions.
    assert(PyArray_NDIM(array_object1) == 2);
    assert(PyArray_NDIM(array_object2) == 2);

    // TODO Error handling.
    assert(PyArray_DIM(array_object1, 0) == PyArray_DIM(array_object2, 0));
    assert(PyArray_DIM(array_object1, 1) == PyArray_DIM(array_object2, 1));

    size_t const size1{static_cast<size_t>(PyArray_DIM(array_object1, 0))};
    size_t const size2{static_cast<size_t>(PyArray_DIM(array_object1, 1))};

    ArrayReference<Value1, 2> array_2d_reference1(
        static_cast<Value1*>(PyArray_DATA(const_cast<PyArrayObject*>(
            array_object1))), extents[size1][size2]);

    ArrayReference<Value2, 2> array_2d_reference2(
        static_cast<Value2*>(PyArray_DATA(const_cast<PyArrayObject*>(
            array_object2))), extents[size1][size2]);

    using result_value_type = algorithm::add::result_value_type<Value1, Value2>;

    PyArrayObject* result_object{(PyArrayObject*)(
        PyArray_SimpleNew(
            PyArray_NDIM(array_object1),
            PyArray_DIMS(const_cast<PyArrayObject*>(array_object1)),
            NumpyTypeTraits<result_value_type>::data_type))};

    ArrayReference<result_value_type, 2> result_reference(
        static_cast<result_value_type*>(PyArray_DATA(result_object)),
        extents[size1][size2]);

    algorithm::algebra::add(algorithm::parallel, array_2d_reference1,
        array_2d_reference2, result_reference);

    return result_object;
}


#define ADD_OVERLOAD(                                        \
    algorithm,                                               \
    type1,                                                   \
    type2)                                                   \
PyArrayObject* algorithm##_##type1##_##type2(                \
    PyArrayObject* array1,                                   \
    PyArrayObject* array2)                                   \
{                                                            \
    return algorithm<type1##_t, type2##_t>(array1, array2);  \
}


#define ADD_OVERLOADS(                  \
    algorithm,                          \
    type)                               \
ADD_OVERLOAD(algorithm, type, uint8)    \
ADD_OVERLOAD(algorithm, type, int8)     \
ADD_OVERLOAD(algorithm, type, uint16)   \
ADD_OVERLOAD(algorithm, type, int16)    \
ADD_OVERLOAD(algorithm, type, uint32)   \
ADD_OVERLOAD(algorithm, type, int32)    \
ADD_OVERLOAD(algorithm, type, uint64)   \
ADD_OVERLOAD(algorithm, type, int64)    \
ADD_OVERLOAD(algorithm, type, float32)  \
ADD_OVERLOAD(algorithm, type, float64)


ADD_OVERLOADS(add, uint8)
ADD_OVERLOADS(add, int8)
ADD_OVERLOADS(add, uint16)
ADD_OVERLOADS(add, int16)
ADD_OVERLOADS(add, uint32)
ADD_OVERLOADS(add, int32)
ADD_OVERLOADS(add, uint64)
ADD_OVERLOADS(add, int64)
ADD_OVERLOADS(add, float32)
ADD_OVERLOADS(add, float64)


#undef ADD_OVERLOADS
#undef ADD_OVERLOAD


using AddOverloadsKey = std::tuple<int, int>;
using AddOverload = std::function<PyArrayObject*(PyArrayObject*,
    PyArrayObject*)>;
using AddOverloads = std::map<AddOverloadsKey, AddOverload>;


#define ADD_ADD_OVERLOADS(                                          \
    gdal_type,                                                      \
    type)                                                           \
{ AddOverloadsKey(gdal_type, NPY_UINT8  ), add_##type##_uint8   },  \
{ AddOverloadsKey(gdal_type, NPY_INT8   ), add_##type##_int8    },  \
{ AddOverloadsKey(gdal_type, NPY_UINT16 ), add_##type##_uint16  },  \
{ AddOverloadsKey(gdal_type, NPY_INT16  ), add_##type##_int16   },  \
{ AddOverloadsKey(gdal_type, NPY_UINT32 ), add_##type##_uint32  },  \
{ AddOverloadsKey(gdal_type, NPY_INT32  ), add_##type##_int32   },  \
{ AddOverloadsKey(gdal_type, NPY_UINT64 ), add_##type##_uint64  },  \
{ AddOverloadsKey(gdal_type, NPY_INT64  ), add_##type##_int64   },  \
{ AddOverloadsKey(gdal_type, NPY_FLOAT32), add_##type##_float32 },  \
{ AddOverloadsKey(gdal_type, NPY_FLOAT64), add_##type##_float64 },


static AddOverloads add_overloads = {
    ADD_ADD_OVERLOADS(NPY_UINT8, uint8)
    ADD_ADD_OVERLOADS(NPY_INT8, int8)
    ADD_ADD_OVERLOADS(NPY_UINT16, uint16)
    ADD_ADD_OVERLOADS(NPY_INT16, int16)
    ADD_ADD_OVERLOADS(NPY_UINT32, uint32)
    ADD_ADD_OVERLOADS(NPY_INT32, int32)
    ADD_ADD_OVERLOADS(NPY_UINT64, uint64)
    ADD_ADD_OVERLOADS(NPY_INT64, int64)
    ADD_ADD_OVERLOADS(NPY_FLOAT32, float32)
    ADD_ADD_OVERLOADS(NPY_FLOAT64, float64)
};


#undef ADD_ADD_OVERLOADS
#undef ADD_UNSUPPORTED_ADD_OVERLOADS
#undef ADD_SUPPORTED_ADD_OVERLOADS

} // namespace array_array
} // namespace detail


PyArrayObject* add(
    PyArrayObject* array,
    float64_t value)
{
    using namespace detail::array_float64;

    int data_type = PyArray_TYPE(array);
    AddOverloadsKey key(data_type);

    PyArrayObject* result{nullptr};

    if(add_overloads.find(key) == add_overloads.end()) {
        raise_unsupported_overload_exception(python_object(array));
        result = nullptr;
    }
    else {
        result = add_overloads[key](array, value);
    }

    return result;
}


PyArrayObject* add(
    float64_t value,
    PyArrayObject* array)
{
    return add(array, value);
}


PyArrayObject* add(
    PyArrayObject* array1,
    PyArrayObject* array2)
{
    using namespace detail::array_array;

    int data_type1 = PyArray_TYPE(array1);
    int data_type2 = PyArray_TYPE(array1);
    AddOverloadsKey key(data_type1, data_type2);

    PyArrayObject* result{nullptr};

    if(add_overloads.find(key) == add_overloads.end()) {
        raise_unsupported_overload_exception(python_object(array1),
            python_object(array2));
        result = nullptr;
    }
    else {
        result = add_overloads[key](array1, array2);
    }

    return result;
}

} // namespace python
} // namespace fern
