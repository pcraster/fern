// VT_BOOL and VT_STRING are not exposed in Python, so they shouldn't
// be passed in.
#define SWITCH_ON_VALUE_TYPE(   \
    value_type,                 \
    case_)                      \
switch(value_type) {            \
    case_(VT_UINT8, uint8_t)    \
    case_(VT_INT8, int8_t)      \
    case_(VT_UINT16, uint16_t)  \
    case_(VT_INT16, int16_t)    \
    case_(VT_UINT32, uint32_t)  \
    case_(VT_INT32, int32_t)    \
    case_(VT_UINT64, uint64_t)  \
    case_(VT_INT64, int64_t)    \
    case_(VT_FLOAT32, float)    \
    case_(VT_FLOAT64, double)   \
    case VT_BOOL:               \
    case VT_STRING: {           \
        assert(false);          \
    }                           \
}


#define SWITCH_ON_FLOATING_POINT_VALUE_TYPE( \
    value_type,                              \
    case_)                                   \
switch(value_type) {                         \
    case_(VT_FLOAT32, float)                 \
    case_(VT_FLOAT64, double)                \
    default: {                               \
        assert(false);                       \
    }                                        \
}