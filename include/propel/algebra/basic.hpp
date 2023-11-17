#pragma once
#include <functional>
#include <limits>
#include <propel/algebra/helper.hpp>
#include <propel/extra_functional.hpp>

namespace propel::algebra {
template <class T>
using Add = GroupHelper<T, std::plus<T>, std::integral_constant<T, T(0)>, std::negate<T>, CommutativeTag>;

template <class T>
using Min =
    MonoidHelper<T, extra_functional::Min<T>, std::integral_constant<T, std::numeric_limits<T>::max()>, CommutativeTag>;

template <class T>
using Max =
    MonoidHelper<T, extra_functional::Max<T>, std::integral_constant<T, std::numeric_limits<T>::min()>, CommutativeTag>;

template <class T>
using Xor = GroupHelper<T, std::bit_xor<T>, std::integral_constant<T, T(0)>, std::bit_xor<T>, CommutativeTag>;

template <class T>
using And = MonoidHelper<T, std::bit_and<T>, std::integral_constant<T, static_cast<T>(~T(0))>, CommutativeTag>;

template <class T>
using Or = MonoidHelper<T, std::bit_or<T>, std::integral_constant<T, T(0)>, CommutativeTag>;

}  // namespace propel::algebra
