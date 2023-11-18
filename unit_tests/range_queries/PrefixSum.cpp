#include <catch2/catch_test_macros.hpp>
#include <propel/StaticRQ.hpp>
#include <propel/algebra/basic.hpp>
#include <propel/ints.hpp>
#include <propel/range_queries/PrefixSum.hpp>
#include <vector>

using namespace propel::ints;
using propel::StaticRQ;
using propel::algebra::Add;

static_assert(std::is_same_v<StaticRQ<Add<u64>>, propel::range_queries::PrefixSum<Add<u64>>>);

TEST_CASE("PrefixSum", "[range-queries]") {
    auto values = std::vector<u64>{1, 2, 3};
    auto sums = StaticRQ<Add<u64>>(values);
    REQUIRE(sums.fold(0, 0) == 0);
    REQUIRE(sums.fold(0, 1) == 1);
    REQUIRE(sums.fold(0, 2) == 3);
    REQUIRE(sums.fold(0, 3) == 6);
    REQUIRE(sums.fold(1, 1) == 0);
    REQUIRE(sums.fold(1, 2) == 2);
    REQUIRE(sums.fold(1, 3) == 5);
    REQUIRE(sums.fold(2, 3) == 3);
    REQUIRE(sums.fold(3, 3) == 0);
}
