#pragma once
#include <concepts>
#include <iterator>
#include <propel/algebra/concepts.hpp>
#include <utility>
#include <vector>

namespace propel::range_queries {

template <algebra::Group Algebra>
class PrefixSum {
  public:
    using value_type = typename Algebra::value_type;

  private:
    [[no_unique_address]] Algebra algebra_;
    std::vector<value_type> sums_;

  public:
    template <class Begin, std::sentinel_for<Begin> End>
    PrefixSum(Begin begin, End end, Algebra &&algebra = Algebra()) : algebra_(std::forward<Algebra>(algebra)) {
        sums_.reserve(std::distance(begin, end) + 1);
        sums_.push_back(algebra_.identity());
        for (; begin != end; ++begin) {
            sums_.push_back(algebra_.merge(sums_.back(), *begin));
        }
    }

    template <std::ranges::range Range,
              class = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<Range>, PrefixSum<Algebra>>>>
    explicit PrefixSum(Range &&range, Algebra &&algebra = Algebra())
        : PrefixSum(range.begin(), range.end(), std::forward<Algebra>(algebra)) {}

    // range merge values from the range [i, j)
    [[nodiscard]] auto fold(std::size_t i, std::size_t j) const -> value_type {
        return algebra_.merge(algebra_.inverse(sums_[i]), sums_[j]);
    }
    // range merge values from the range [i, j)
    [[nodiscard]] auto fold(std::size_t i, std::size_t j) -> value_type {
        return algebra_.merge(algebra_.inverse(sums_[i]), sums_[j]);
    }
};

}  // namespace propel::range_queries
