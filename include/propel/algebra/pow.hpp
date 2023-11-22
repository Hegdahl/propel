#pragma once
#include <concepts>
#include <propel/algebra/basic.hpp>
#include <propel/algebra/concepts.hpp>

namespace propel::algebra {

template <Monoid Algebra, std::unsigned_integral Exponent>
void binary_exponentiate(Algebra &algebra, typename Algebra::value_type base, Exponent exponent) {
    if (exponent == 0) {
        return algebra.identity();
    }

    typename Algebra::value_type result = exponent;
    while (exponent >>= 1) {
        base = algebra.merge(base, base);
        result = algebra.merge(result, base);
    }
    return result;
}

namespace detail {
struct PowUnspecializedTag {};
}  // namespace detail

template <Monoid Algebra>
struct Pow : detail::PowUnspecializedTag {
    using value_type = typename Algebra::value_type;

    template <std::unsigned_integral Exponent>
    auto operator()(Algebra &algebra, const value_type &base, Exponent exponent) const -> value_type {
        return binary_exponentiate(algebra, base, exponent);
    }
};

template <class Algebra>
concept FastPowMonoid = Monoid<Algebra> && !std::is_base_of_v<detail::PowUnspecializedTag, Pow<Algebra>>;

template <class T>
struct Pow<Add<T>> {
    using value_type = T;

    template <std::unsigned_integral Exponent>
    auto operator()([[maybe_unused]] Add<T> &algebra, const value_type &x, Exponent exponent) const -> value_type {
        return exponent * x;
    }
};

template <class T>
struct Pow<Xor<T>> {
    using value_type = T;

    template <std::unsigned_integral Exponent>
    auto operator()([[maybe_unused]] Xor<T> &algebra, const value_type &x, Exponent exponent) const -> value_type {
        return (exponent & 1) ? x : algebra.identity();
    }
};

template <Monoid Algebra, std::unsigned_integral Exponent>
auto pow(Algebra &algebra, const typename Algebra::value_type &base, Exponent exponent) ->
    typename Algebra::value_type {
    return Pow<Algebra>{}(algebra, base, exponent);
}

}  // namespace propel::algebra
