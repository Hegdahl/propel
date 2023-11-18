#include <catch2/catch_test_macros.hpp>
#include <propel/algebra/basic.hpp>
#include <propel/ints.hpp>

using namespace propel::ints;
using namespace propel::algebra;

static_assert(Group<Add<u64>>);
static_assert(!Group<Min<u64>>);

TEST_CASE("Add group", "[algebra]") {
    REQUIRE(Add<u64>{}.merge(1, 1) == 2);
    REQUIRE(Add<u64>{}.identity() == 0);
    REQUIRE(Add<u64>{}.inverse(1) == ~u64());
}
