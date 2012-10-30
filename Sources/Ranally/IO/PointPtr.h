#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>


namespace ranally {

#define DEFINE_SHARED_POINTER_TYPE(                                            \
    className)                                                                 \
    class className;                                                           \
    typedef boost::shared_ptr<className> className##Ptr;

DEFINE_SHARED_POINTER_TYPE(PointAttribute)
DEFINE_SHARED_POINTER_TYPE(PointDomain)
DEFINE_SHARED_POINTER_TYPE(PointFeature)
DEFINE_SHARED_POINTER_TYPE(PointValue)

#undef DEFINE_SHARED_POINTER_TYPE

typedef std::vector<PointAttributePtr> PointAttributes;
typedef std::vector<PointValuePtr> PointValues;

} // namespace ranally