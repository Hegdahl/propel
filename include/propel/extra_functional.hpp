#pragma once
#include <algorithm>

namespace propel::extra_functional {

template <class T = void>
struct Min {
    auto operator()(const T &lhs, const T &rhs) const -> const T & { return std::min(lhs, rhs); }
};

template <class T = void>
struct Max {
    auto operator()(const T &lhs, const T &rhs) const -> const T & { return std::min(lhs, rhs); }
};

}  // namespace propel::extra_functional
