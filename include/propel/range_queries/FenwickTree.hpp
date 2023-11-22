#pragma once
#include <propel/algebra/concepts.hpp>
#include <vector>

namespace propel::range_queries {

template <algebra::Group Algebra>
class FenwickTree {
  public:
    using value_type = typename Algebra::value_type;

  private:
    [[no_unique_address]] Algebra algebra_;
    std::vector<value_type> sums_;

    // Given (exclusive) right bound of prefix, find next smaller bound
    static constexpr auto next_left(std::size_t i) -> std::size_t { return i - (i & -i); }
    // Parent according to this tree structure:
    // https://cp-algorithms.com/data_structures/binary_indexed_tree.png
    static constexpr auto parent(std::size_t i) -> std::size_t { return i + ((i + 1) & ~i); }

  public:
    explicit FenwickTree(std::size_t n, Algebra algebra = Algebra())
        : algebra_(algebra), sums_(n, algebra.identity()) {}

    template <class Begin, std::sentinel_for<Begin> End>
    FenwickTree(Begin begin, End end, Algebra algebra = Algebra()) : FenwickTree(std::distance(begin, end), algebra) {
        for (std::size_t i = 0; begin != end; ++i, ++begin) {
            sums_[i] = algebra_.merge(sums_[i], *begin);
            if (std::size_t j = parent(i); j < sums_.size()) {
                sums_[j] = algebra.merge(sums_[j], sums_[i]);
            }
        }
    }

    template <std::ranges::range Range,
              class = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<Range>, FenwickTree>>>
    explicit FenwickTree(Range &&range, Algebra algebra = Algebra())
        : FenwickTree(range.begin(), range.end(), algebra) {}

    // merge of all values from [0, end)
    [[nodiscard]] auto prefix_fold(std::size_t end) -> value_type {
        value_type res = algebra_.identity();
        for (; end != 0; end = next_left(end)) {
            res = algebra_.merge(sums_[end - 1], res);
        }
        return res;
    }

    // merge of all values from [i, j)
    [[nodiscard]] auto fold(std::size_t i, std::size_t j) -> value_type {
        return algebra_.merge(algebra_.inverse(prefix_fold(i)), prefix_fold(j));
    }

    // left-add x to position i
    void update(std::size_t i, const value_type &x) {
        for (; i < sums_.size(); i = parent(i)) {
            sums_[i] = algebra_.merge(x, sums_[i]);
        }
    }

    // set the value at position i to x
    void replace(std::size_t i, const value_type &x) { update(i, algebra_.merge(x, algebra_.inverse(fold(i, i + 1)))); }
};

}  // namespace propel::range_queries
