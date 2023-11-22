// https://cses.fi/problemset/task/1735
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("bmi2")
#include <array>
#include <cassert>
#include <iostream>
#include <propel/RURQ.hpp>
#include <propel/algebra/basic.hpp>
#include <propel/ints.hpp>

using namespace propel::ints;
using propel::RURQ;
using propel::algebra::Add;

template <std::size_t levels>
struct BitSetSet {
    BitSetSet<levels * 0 + 1> upper;
    std::array<BitSetSet<levels - 1>, 64> lower;

    [[gnu::always_inline]] void set(std::size_t i) {
        auto [x, y] = split(i);
        upper.set(x);
        lower[x].set(y);
    }

    [[gnu::always_inline]] void reset(std::size_t i) {
        auto [x, y] = split(i);
        lower[x].reset(y);
        if (!lower[x].any()) {
            upper.reset(x);
        }
    }

    [[nodiscard, gnu::always_inline]] auto any() const -> bool { return upper.any(); }

    [[nodiscard, gnu::always_inline]] auto find_first() const -> std::size_t {
        auto x = upper.find_first();
        auto y = lower[x].find_first();
        return merge(x, y);
    }

    [[nodiscard, gnu::always_inline]] auto find_next(std::size_t i) const -> std::size_t {
        auto [x, y] = split(i);

        y = lower[x].find_next(y);
        if (y != ~std::size_t()) {
            return merge(x, y);
        }

        x = upper.find_next(x);
        if (x == ~std::size_t()) {
            return ~std::size_t();
        }

        y = lower[x].find_first();
        return merge(x, y);
    }

    static constexpr auto size() -> std::size_t { return BitSetSet<levels - 1>::size() * 64; }
    static constexpr auto split(std::size_t i) -> std::array<std::size_t, 2> {
        return {i / BitSetSet<levels - 1>::size(), i % BitSetSet<levels - 1>::size()};
    }
    static constexpr auto merge(std::size_t x, std::size_t y) -> std::size_t {
        return x * BitSetSet<levels - 1>::size() + y;
    }
};

template <>
struct BitSetSet<1> {
    static constexpr auto size() -> std::size_t { return 64; }

    u64 data = 0;

    [[gnu::always_inline]] void set(std::size_t i) { data |= static_cast<u64>(1) << i; }
    [[gnu::always_inline]] void reset(std::size_t i) { data &= ~(static_cast<u64>(1) << i); }
    [[nodiscard, gnu::always_inline]] auto any() const -> bool { return data != 0; }

    [[nodiscard, gnu::always_inline]] auto find_first() const -> std::size_t {
        if (data == 0) {
            return ~std::size_t();
        }
        return static_cast<std::size_t>(__builtin_ctzll(data));
    }

    [[nodiscard, gnu::always_inline]] auto find_next(std::size_t i) const -> std::size_t {
        if (i + 1 == 64 || (data >> (i + 1)) == 0) {
            return ~std::size_t();
        }
        return i + 1 + static_cast<std::size_t>(__builtin_ctzll(data >> (i + 1)));
    }
};

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin.exceptions(std::ios::failbit | std::ios::badbit);

    u32 n = 0;
    u32 q = 0;
    std::cin >> n >> q;

    auto arr = std::vector<u32>(n);
    for (u32 &x : arr) {
        std::cin >> x;
    }

    auto sums = RURQ<Add<u64>>(arr);
    auto cuts = BitSetSet<3>();
    for (u32 i = 0; i <= n; ++i) {
        cuts.set(i);
    }

    for (u32 qq = 0; qq != q; ++qq) {
        char qt = 0;
        std::cin >> qt;
        if (qt == '1') {
            u32 i = 0;
            u32 j = 0;
            u32 x = 0;
            std::cin >> i >> j >> x;
            --i;
            cuts.set(i);
            cuts.set(j);
            sums.update(i, j, x);
        } else if (qt == '2') {
            u32 i = 0;
            u32 j = 0;
            u32 x = 0;
            std::cin >> i >> j >> x;
            --i;

            cuts.set(i);
            cuts.set(j);
            auto cut = cuts.find_next(i);
            auto left_of_cut = sums.fold(i, i + 1);
            while (cut < j) {
                auto right_of_cut = sums.fold(cut, cut + 1);
                sums.update(i, cut, right_of_cut - left_of_cut);
                left_of_cut = right_of_cut;
                cuts.reset(cut);
                cut = cuts.find_next(i);
            }
            sums.update(i, j, x - left_of_cut);
        } else if (qt == '3') {
            u32 i = 0;
            u32 j = 0;
            std::cin >> i >> j;
            --i;
            std::cout << sums.fold(i, j) << '\n';
        } else {
            assert(false);
        }
    }
}
