#pragma once
#include <cassert>
#include <propel/ints.hpp>

namespace propel::int_literals {

#define PROPEL_DEFINE_INT_LITERAL(type)                      \
    template <char... chars>                                 \
    constexpr auto operator""_##type()->propel::ints::type { \
        using propel::ints::type;                            \
        type res = 0;                                        \
        auto push_char = [&](char digit) {                   \
            if (digit == '\'') return;                       \
            assert('0' <= digit <= '9');                     \
            res *= 10;                                       \
            res += static_cast<type>(digit - '0');           \
        };                                                   \
        (push_char(chars), ...);                             \
        return res;                                          \
    }
PROPEL_DEFINE_INT_LITERAL(i8)
PROPEL_DEFINE_INT_LITERAL(i16)
PROPEL_DEFINE_INT_LITERAL(i32)
PROPEL_DEFINE_INT_LITERAL(i64)
PROPEL_DEFINE_INT_LITERAL(u8)
PROPEL_DEFINE_INT_LITERAL(u16)
PROPEL_DEFINE_INT_LITERAL(u32)
PROPEL_DEFINE_INT_LITERAL(u64)
#undef PROPEL_DEFINE_LITERAL

}  // namespace propel::int_literals
