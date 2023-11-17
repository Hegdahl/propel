#include <catch2/catch_test_macros.hpp>
#include <propel/int_literals.hpp>
#include <propel/ints.hpp>

using namespace propel::ints;
using namespace propel::int_literals;

auto factorial(u32 number) -> u32 { return number <= 1_u32 ? number : factorial(number - 1_u32) * number; }

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(factorial(1_u32) == 1_u32);
    REQUIRE(factorial(2_u32) == 2_u32);
    REQUIRE(factorial(3_u32) == 6_u32);
    REQUIRE(factorial(10_u32) == 3'628'800_u32);
}
