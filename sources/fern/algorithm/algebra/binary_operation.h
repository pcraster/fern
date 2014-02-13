#pragma once
#include <type_traits>
#include <utility>
#include "fern/core/argument_traits.h"
#include "fern/core/assert.h"


namespace fern {
namespace detail {
namespace dispatch {

template<class A1, class A2, class R,
    class OutOfDomainPolicy,
    class OutOfRangePolicy,
    class NoDataPolicy,
    class Algorithm,
    class A1CollectionCategory,
    class A2CollectionCategory>
class BinaryOperation
{
};


template<class A1, class A2, class R,
    class OutOfDomainPolicy,
    class OutOfRangePolicy,
    class NoDataPolicy,
    class Algorithm>
class BinaryOperation<A1, A2, R,
        OutOfDomainPolicy,
        OutOfRangePolicy,
        NoDataPolicy,
        Algorithm,
        constant_tag,
        constant_tag>:

    public OutOfDomainPolicy,
    public OutOfRangePolicy,
    public NoDataPolicy

{

    FERN_STATIC_ASSERT(std::is_arithmetic, A1)
    FERN_STATIC_ASSERT(std::is_arithmetic, A2)

public:

    BinaryOperation(
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(),
          _algorithm(algorithm)
    {
    }

    BinaryOperation(
        NoDataPolicy&& no_data_policy,
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(std::forward<NoDataPolicy>(no_data_policy)),
          _algorithm(algorithm)
    {
    }

    // constant + constant
    inline void calculate(
        A1 const& argument1,
        A2 const& argument2,
        R& result)
    {
        _algorithm(argument1, argument2, result);
    }

private:

    Algorithm      _algorithm;

};


template<class A1, class A2, class R,
    class OutOfDomainPolicy,
    class OutOfRangePolicy,
    class NoDataPolicy,
    class Algorithm>
class BinaryOperation<A1, A2, R,
        OutOfDomainPolicy,
        OutOfRangePolicy,
        NoDataPolicy,
        Algorithm,
        collection_tag,
        collection_tag>:

    public OutOfDomainPolicy,
    public OutOfRangePolicy,
    public NoDataPolicy

{

    FERN_STATIC_ASSERT(std::is_arithmetic,
        typename ArgumentTraits<A1>::value_type)
    FERN_STATIC_ASSERT(std::is_arithmetic,
        typename ArgumentTraits<A2>::value_type)

public:

    BinaryOperation(
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(),
          _algorithm(algorithm)
    {
    }

    BinaryOperation(
        NoDataPolicy&& no_data_policy,
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(std::forward<NoDataPolicy>(no_data_policy)),
          _algorithm(algorithm)
    {
    }

    // collection + collection
    void calculate(
        A1 const& argument1,
        A2 const& argument2,
        R& result)
    {
        assert(fern::size(argument1) == fern::size(argument2));
        assert(fern::size(argument1) == fern::size(result));

        size_t const size = fern::size(argument1);

        for(size_t i = 0; i < size; ++i) {
            if(!NoDataPolicy::is_no_data(i)) {
                typename ArgumentTraits<A1>::value_type a1(fern::get(
                    argument1, i));
                typename ArgumentTraits<A2>::value_type a2(fern::get(
                    argument2, i));

                if(!OutOfDomainPolicy::within_domain(a1, a2)) {
                    NoDataPolicy::mark_as_no_data(i);
                }

                _algorithm(
                    static_cast<typename ArgumentTraits<R>::value_type>(a1),
                    static_cast<typename ArgumentTraits<R>::value_type>(a2),
                    fern::get(result, i));

                if(!OutOfRangePolicy::within_range(a1, a2, fern::get(
                        result, i))) {
                    NoDataPolicy::mark_as_no_data(i);
                }
            }
        }
    }

private:

    Algorithm _algorithm;

};


template<class A1, class A2, class R,
    class OutOfDomainPolicy,
    class OutOfRangePolicy,
    class NoDataPolicy,
    class Algorithm>
class BinaryOperation<A1, A2, R,
        OutOfDomainPolicy,
        OutOfRangePolicy,
        NoDataPolicy,
        Algorithm,
        constant_tag,
        collection_tag>:

    public OutOfDomainPolicy,
    public OutOfRangePolicy,
    public NoDataPolicy

{

    FERN_STATIC_ASSERT(std::is_arithmetic, A1)
    FERN_STATIC_ASSERT(std::is_arithmetic,
        typename ArgumentTraits<A2>::value_type)

public:

    BinaryOperation(
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(),
          _algorithm(algorithm)
    {
    }

    BinaryOperation(
        NoDataPolicy&& no_data_policy,
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(std::forward<NoDataPolicy>(no_data_policy)),
          _algorithm(algorithm)
    {
    }

    // constant + collection
    inline void calculate(
        A1 const& argument1,
        A2 const& argument2,
        R& result)
    {
        assert(fern::size(argument2) == fern::size(result));

        size_t const size = fern::size(argument2);

        typename ArgumentTraits<A2>::value_type const& a1(argument1);

        for(size_t i = 0; i < size; ++i) {
            if(!NoDataPolicy::is_no_data(i)) {
                typename ArgumentTraits<A1>::value_type a2(fern::get(
                    argument2, i));

                if(!OutOfDomainPolicy::within_domain(a1, a2)) {
                    NoDataPolicy::mark_as_no_data(i);
                }

                _algorithm(
                    static_cast<typename ArgumentTraits<R>::value_type>(a1),
                    static_cast<typename ArgumentTraits<R>::value_type>(a2),
                    fern::get(result, i));

                if(!OutOfRangePolicy::within_range(a1, a2, fern::get(
                        result, i))) {
                    NoDataPolicy::mark_as_no_data(i);
                }
            }
        }
    }

private:

    Algorithm      _algorithm;

};


template<class A1, class A2, class R,
    class OutOfDomainPolicy,
    class OutOfRangePolicy,
    class NoDataPolicy,
    class Algorithm>
class BinaryOperation<A1, A2, R,
        OutOfDomainPolicy,
        OutOfRangePolicy,
        NoDataPolicy,
        Algorithm,
        collection_tag,
        constant_tag>:

    public OutOfDomainPolicy,
    public OutOfRangePolicy,
    public NoDataPolicy

{

    FERN_STATIC_ASSERT(std::is_arithmetic,
        typename ArgumentTraits<A1>::value_type)
    FERN_STATIC_ASSERT(std::is_arithmetic, A2)

public:

    BinaryOperation(
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(),
          _algorithm(algorithm)
    {
    }

    BinaryOperation(
        NoDataPolicy&& no_data_policy,
        Algorithm const& algorithm)
        : OutOfDomainPolicy(),
          OutOfRangePolicy(),
          NoDataPolicy(std::forward<NoDataPolicy>(no_data_policy)),
          _algorithm(algorithm)
    {
    }

    // collection + constant
    inline void calculate(
        A1 const& argument1,
        A2 const& argument2,
        R& result)
    {
        assert(fern::size(argument1) == fern::size(result));

        size_t const size = fern::size(argument1);

        typename ArgumentTraits<A2>::value_type const& a2(argument2);

        for(size_t i = 0; i < size; ++i) {
            if(!NoDataPolicy::is_no_data(i)) {
                typename ArgumentTraits<A1>::value_type a1(fern::get(
                    argument1, i));

                if(!OutOfDomainPolicy::within_domain(a1, a2)) {
                    NoDataPolicy::mark_as_no_data(i);
                }

                _algorithm(
                    static_cast<typename ArgumentTraits<R>::value_type>(a1),
                    static_cast<typename ArgumentTraits<R>::value_type>(a2),
                    fern::get(result, i));

                if(!OutOfRangePolicy::within_range(a1, a2, fern::get(
                        result, i))) {
                    NoDataPolicy::mark_as_no_data(i);
                }
            }
        }
    }

private:

    Algorithm      _algorithm;

};

} // namespace dispatch
} // namespace detail
} // namespace fern
