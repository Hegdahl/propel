#pragma once
#include <bit>
#include <iostream>
#include <iterator>
#include <propel/algebra/concepts.hpp>
#include <span>
#include <vector>

namespace propel::range_queries {

template <algebra::Monoid Algebra>
class DisjointSparseTable {
  public:
    using value_type = typename Algebra::value_type;

  private:
    [[no_unique_address]] Algebra algebra_;
    std::size_t n_{};
    std::vector<value_type> data_;

  public:
    template <class Begin, std::sentinel_for<Begin> End>
    DisjointSparseTable(Begin begin, End end, Algebra &&algebra = Algebra())
        : algebra_(std::forward<Algebra>(algebra)), n_(std::distance(begin, end)) {
        std::size_t levels = 1;
        std::size_t half_width = 1;
        while ((half_width << 1U) < n_) {
            ++levels;
            half_width <<= 1U;
        }
        data_.resize(levels * n_);

        std::copy(begin, end, data_.begin());
        half_width = 2;

        for (std::size_t level = 1; level != levels; ++level, half_width *= 2) {
            std::size_t width = 2 * half_width;

            for (std::size_t block_start = 0; block_start + half_width < n_; block_start += width) {
                std::size_t block_width = std::min(n_ - block_start, width);
                auto base = row(0).subspan(block_start, block_width);
                auto block = row(level).subspan(block_start, block_width);

                block[half_width - 1] = base[half_width - 1];
                block[half_width] = base[half_width];
                for (std::size_t i = 1; i != half_width; ++i) {
                    block[half_width - i - 1] = algebra_.merge(base[half_width - i - 1], block[half_width - i]);
                }
                for (std::size_t i = 1; i != block_width - half_width; ++i) {
                    block[half_width + i] = algebra_.merge(block[half_width + i - 1], base[half_width + i]);
                }
            }
        }
    }

    template <std::ranges::range Range,
              class = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<Range>, DisjointSparseTable>>>
    explicit DisjointSparseTable(Range &&range, Algebra &&algebra = Algebra())
        : DisjointSparseTable(range.begin(), range.end(), std::forward<Algebra>(algebra)) {}

    // range merge values from the range [i, j)
    auto fold(std::size_t i, std::size_t j) const -> value_type {
        if (i == j) {
            return algebra_.identity();
        }
        --j;
        if (i == j) {
            return row(0)[i];
        }
        std::size_t level = std::bit_width(i ^ j);
        return algebra_.merge(row(level)[i], row(level)[j]);
    }

    // range merge values from the range [i, j)
    auto fold(std::size_t i, std::size_t j) -> value_type {
        if (i == j) {
            return algebra_.identity();
        }
        --j;
        if (i == j) {
            return row(0)[i];
        }
        std::size_t level = std::bit_width(i ^ j) - 1;
        return algebra_.merge(row(level)[i], row(level)[j]);
    }

  private:
    auto row(std::size_t height) -> std::span<value_type> { return std::span(data_.data() + height * n_, n_); }
};

}  // namespace propel::range_queries
