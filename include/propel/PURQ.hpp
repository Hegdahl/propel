#pragma once
#include <propel/range_queries/FenwickTree.hpp>

namespace propel {

namespace detail {

template <algebra::Magma Algebra>
struct ImplPURQ {};

template <algebra::Group Algebra>
struct ImplPURQ<Algebra> {
    using type = range_queries::FenwickTree<Algebra>;
};

}  // namespace detail

template <algebra::Magma Algebra>
using PURQ = typename detail::ImplPURQ<Algebra>::type;

template <algebra::Magma Algebra>
auto make_purq(std::size_t n, Algebra algebra) -> PURQ<Algebra> {
    return PURQ<Algebra>(n, algebra);
}

template <class Begin, std::sentinel_for<Begin> End, algebra::Magma Algebra>
auto make_purq(Begin begin, End end, Algebra algebra) -> PURQ<Algebra> {
    return PURQ<Algebra>(begin, end, algebra);
}

template <std::ranges::range Range, algebra::Magma Algebra>
auto make_purq(Range &&range, Algebra algebra) -> PURQ<Algebra> {
    return PURQ<Algebra>(std::forward<Range>(range), algebra);
}

}  // namespace propel
