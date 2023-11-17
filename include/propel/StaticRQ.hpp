#pragma once
#include <propel/algebra/concepts.hpp>
#include <propel/range_queries/PrefixSum.hpp>

namespace propel {

namespace detail {

template <algebra::Magma Algebra>
struct ImplStaticRQ {};

template <algebra::Group Algebra>
struct ImplStaticRQ<Algebra> {
    using type = range_queries::PrefixSum<Algebra>;
};

}  // namespace detail

template <algebra::Magma Algebra>
using StaticRQ = typename detail::ImplStaticRQ<Algebra>::type;

}  // namespace propel
