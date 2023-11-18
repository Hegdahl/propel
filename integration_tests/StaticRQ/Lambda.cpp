// https://cses.fi/problemset/task/1646
#include <iostream>
#include <propel/StaticRQ.hpp>
#include <propel/algebra/helper.hpp>
#include <propel/ints.hpp>

using namespace propel::ints;
using propel::make_static_rq;
using propel::algebra::GroupHelper;

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

    auto sums = make_static_rq(arr, GroupHelper(
                                        /*merge   =*/[](u64 x, u64 y) -> u64 { return x + y; },
                                        /*identity=*/[]() -> u64 { return 0; },
                                        /*inverse =*/[](u64 x) -> u64 { return -x; }));

    for (u32 qq = 0; qq != q; ++qq) {
        u32 i = 0;
        u32 j = 0;
        std::cin >> i >> j;
        --i;
        std::cout << sums.fold(i, j) << '\n';
    }
}
