#pragma once
#include <propel/algebra/concepts.hpp>
#include <propel/range_queries/DisjointSparseTable.hpp>
#include <propel/range_queries/PrefixSum.hpp>

namespace propel {

namespace detail {

template <algebra::Magma Algebra>
struct ImplStaticRQ {};

template <algebra::Monoid Algebra>
struct ImplStaticRQ<Algebra> {
    using type = range_queries::DisjointSparseTable<Algebra>;
};

template <algebra::Group Algebra>
struct ImplStaticRQ<Algebra> {
    using type = range_queries::PrefixSum<Algebra>;
};

}  // namespace detail

template <algebra::Magma Algebra>
using StaticRQ = typename detail::ImplStaticRQ<Algebra>::type;

template <class Begin, std::sentinel_for<Begin> End, algebra::Magma Algebra>
auto make_static_rq(Begin begin, End end, Algebra &&algebra) -> StaticRQ<Algebra> {
    return StaticRQ<Algebra>(begin, end, std::forward<Algebra>(algebra));
}

template <std::ranges::range Range, algebra::Magma Algebra>
auto make_static_rq(Range &&range, Algebra &&algebra) -> StaticRQ<Algebra> {
    return StaticRQ<Algebra>(std::forward<Range>(range), std::forward<Algebra>(algebra));
}

}  // namespace propel
