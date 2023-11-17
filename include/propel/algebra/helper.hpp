#pragma once
#include <propel/algebra/concepts.hpp>
#include <tuple>
#include <utility>

namespace propel::algebra {

template <class T, class Merge, class... Tags>
class MagmaHelper {
    Merge merge_;

  public:
    using tags = std::tuple<Tags...>;

    explicit MagmaHelper(Merge &&merge = Merge()) : merge_(std::forward<Merge>(merge)) {}

    using value_type = T;

    constexpr auto merge(const T &x, const T &y) -> T { return merge_(x, y); }
};

template <class T, class Merge, class Identity, class... ExtraTags>
class MonoidHelper : public MagmaHelper<T, Merge, AssociativeTag, ExtraTags...> {
    Identity identity_;

  public:
    explicit MonoidHelper(Merge &&merge = Merge(), Identity &&identity = Identity())
        : MagmaHelper<T, Merge, AssociativeTag, ExtraTags...>(std::forward<Merge>(merge)),
          identity_(std::forward<Identity>(identity)) {}

    constexpr auto identity() -> T { return identity_(); }
};

template <class T, class Merge, class Identity, class Inverse, class... ExtraTags>
class GroupHelper : public MonoidHelper<T, Merge, Identity, ExtraTags...> {
    Inverse inverse_;

  public:
    explicit GroupHelper(Merge &&merge = Merge(), Identity &&identity = Identity(), Inverse &&inverse = Inverse())
        : MonoidHelper<T, Merge, Identity, ExtraTags...>(std::forward<Merge>(merge), std::forward<Identity>(identity)),
          inverse_(std::forward<Inverse>(inverse)) {}

    constexpr auto inverse(const T &x) -> T { return inverse_(x); }
};

}  // namespace propel::algebra
