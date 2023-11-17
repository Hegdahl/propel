#pragma once
#include <concepts>
#include <tuple>

namespace propel::algebra {

struct AssociativeTag {};
struct CommutativeTag {};

template <class Algebra>
concept Magma = requires(Algebra algebra, const typename Algebra::value_type &x) {
    { algebra.merge(x, x) } -> std::convertible_to<typename Algebra::value_type>;
};

template <class Algebra>
concept Associative = Magma<Algebra> && requires(typename Algebra::tags tags) {
    { std::get<AssociativeTag>(tags) };
};

template <class Algebra>
concept Commutative = Magma<Algebra> && requires(typename Algebra::tags tags) {
    { std::get<CommutativeTag>(tags) };
};

template <class Algebra>
concept Monoid = Associative<Algebra> && requires(Algebra algebra) {
    { algebra.identity() } -> std::convertible_to<typename Algebra::value_type>;
};

template <class Algebra>
concept Group = Monoid<Algebra> && requires(Algebra algebra, const typename Algebra::value_type &x) {
    { algebra.inverse(x) } -> std::convertible_to<typename Algebra::value_type>;
};

}  // namespace propel::algebra
