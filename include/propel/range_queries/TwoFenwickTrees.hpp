#pragma once
#include <propel/algebra/concepts.hpp>
#include <propel/algebra/pow.hpp>
#include <propel/range_queries/FenwickTree.hpp>

namespace propel::range_queries {

template <algebra::Group Algebra>
    requires(algebra::Commutative<Algebra>)
class TwoFenwickTrees {
  public:
    using value_type = typename Algebra::value_type;

  private:
    [[no_unique_address]] Algebra algebra_;
    FenwickTree<Algebra> slope_;
    FenwickTree<Algebra> offset_;

  public:
    explicit TwoFenwickTrees(std::size_t n, Algebra algebra = Algebra())
        : algebra_(algebra), slope_(n, algebra_), offset_(n, algebra_) {}

    template <class Begin, std::sentinel_for<Begin> End>
    TwoFenwickTrees(Begin begin, End end, Algebra algebra = Algebra())
        : algebra_(algebra), slope_(std::distance(begin, end), algebra_), offset_(begin, end, algebra_) {}

    template <std::ranges::range Range,
              class = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<Range>, TwoFenwickTrees>>>
    explicit TwoFenwickTrees(Range &&range, Algebra algebra = Algebra())
        : TwoFenwickTrees(range.begin(), range.end(), algebra) {}

    auto prefix_fold(std::size_t end) -> value_type {
        return algebra_.merge(algebra::pow(algebra_, slope_.prefix_fold(end), end), offset_.prefix_fold(end));
    }

    auto fold(std::size_t i, std::size_t j) -> value_type {
        return algebra_.merge(algebra_.inverse(prefix_fold(i)), prefix_fold(j));
    }

    void suffix_update(std::size_t begin, const value_type &x) {
        slope_.update(begin, x);
        offset_.update(begin, algebra::pow(algebra_, algebra_.inverse(x), begin));
    }

    void update(std::size_t i, std::size_t j, const value_type &x) {
        suffix_update(i, x);
        suffix_update(j, algebra_.inverse(x));
    }
};

}  // namespace propel::range_queries
