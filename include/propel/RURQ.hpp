#pragma once
#include <propel/range_queries/TwoFenwickTrees.hpp>

namespace propel {

namespace detail {

template <algebra::Magma Values, algebra::Magma Updates>
struct ImplRURQ {};

template <algebra::Group Algebra>
struct ImplRURQ<Algebra, Algebra> {
    using type = range_queries::TwoFenwickTrees<Algebra>;
};

}  // namespace detail

template <algebra::Magma Values, algebra::Magma Updates = Values>
using RURQ = typename detail::ImplRURQ<Values, Updates>::type;

template <algebra::Magma Algebra>
auto make_rurq(std::size_t n, Algebra algebra) -> RURQ<Algebra> {
    return RURQ<Algebra>(n, algebra);
}

template <class Begin, std::sentinel_for<Begin> End, algebra::Magma Algebra>
auto make_rurq(Begin begin, End end, Algebra algebra) -> RURQ<Algebra> {
    return RURQ<Algebra>(begin, end, algebra);
}

template <std::ranges::range Range, algebra::Magma Algebra>
auto make_rurq(Range &&range, Algebra algebra) -> RURQ<Algebra> {
    return RURQ<Algebra>(std::forward<Range>(range), algebra);
}

}  // namespace propel
