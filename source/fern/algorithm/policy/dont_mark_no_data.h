#pragma once
#include <cstddef>


namespace fern {
namespace algorithm {

/*!
    @ingroup    fern_algorithm_policy_group
*/
class DontMarkNoData {

public:

    static void    mark_as_no_data     ();

    static void    mark_as_no_data     (size_t index);

    static void    mark_as_no_data     (size_t index1,
                                        size_t index2);

    static void    mark_as_no_data     (size_t index1,
                                        size_t index2,
                                        size_t index3);

                   DontMarkNoData      ()=default;

                   DontMarkNoData      (DontMarkNoData&&)=default;

    template<
        typename Mask>
                   DontMarkNoData      (Mask& mask);

    DontMarkNoData&
                   operator=           (DontMarkNoData&&)=default;

                   DontMarkNoData      (DontMarkNoData const&)=default;

    DontMarkNoData&
                   operator=           (DontMarkNoData const&)=default;

                   ~DontMarkNoData     ()=default;

};


template<
    typename Mask>
inline DontMarkNoData::DontMarkNoData(
    Mask& /* mask */)

    : DontMarkNoData()

{
}


inline void DontMarkNoData::mark_as_no_data()
{
}


inline void DontMarkNoData::mark_as_no_data(
    size_t /* index */)
{
}


inline void DontMarkNoData::mark_as_no_data(
    size_t /* index1 */,
    size_t /* index2 */)
{
}


inline void DontMarkNoData::mark_as_no_data(
    size_t /* index1 */,
    size_t /* index2 */,
    size_t /* index3 */)
{
}

} // namespace algorithm
} // namespace fern
