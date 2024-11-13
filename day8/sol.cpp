#include <parsing.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/slice.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

using ranges::accumulate;
using ranges::distance;
using ranges::end;
using ranges::find_if;
using ranges::max;
using ranges::size;
using ranges::to;
using ranges::view::enumerate;
using ranges::view::reverse;
using ranges::view::slice;
using ranges::view::transform;

using Grid = std::vector<std::vector<int>>;

Grid transpose(const Grid& grid)
{
    const auto rows = grid.size();
    const auto cols = grid.front().size();
    Grid grid_t;
    grid_t.resize(cols, std::vector<int>(rows));

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            grid_t[j][i] = grid[i][j];
        }
    }
    return grid_t;
}

int isVisible(const Grid& grid, const Grid& grid_t, size_t row_idx, size_t col_idx)
{
    if (row_idx == 0 || col_idx == 0 || row_idx == grid.size() - 1 || col_idx == grid_t.size() - 1)
    {
        return true;
    }
    const auto& row = grid[row_idx];
    const auto& col = grid_t[col_idx];
    const auto tree_height = grid[row_idx][col_idx];

    const auto left_trees = row | slice((size_t)0, col_idx);
    const auto right_trees = row | slice(col_idx + 1, grid_t.size());
    const auto up_trees = col | slice((size_t)0, row_idx);
    const auto down_trees = col | slice(row_idx + 1, grid.size());

    if (max(left_trees) < tree_height || max(right_trees) < tree_height ||
        max(up_trees) < tree_height || max(down_trees) < tree_height)
    {
        return true;
    }
    return false;
}

int scenicScore(const Grid& grid, const Grid& grid_t, size_t row_idx, size_t col_idx)
{
    const auto& row = grid[row_idx];
    const auto& col = grid_t[col_idx];
    const auto tree_height = grid[row_idx][col_idx];

    const auto left_trees = row | slice((size_t)0, col_idx) | reverse;
    const auto right_trees = row | slice(col_idx + 1, grid_t.size());
    const auto up_trees = col | slice((size_t)0, row_idx) | reverse;
    const auto down_trees = col | slice(row_idx + 1, grid.size());

    const auto is_blocked = [&tree_height](const auto height) { return tree_height <= height; };

    int left_score = 0;
    if (col_idx > 0)
    {
        const auto blocked_left_it = find_if(left_trees, is_blocked);
        left_score = blocked_left_it == end(left_trees) ?
                         size(left_trees) :
                         distance(begin(left_trees), blocked_left_it) + 1;
    }

    int right_score = 0;
    if (col_idx < grid_t.size() - 1)
    {
        const auto blocked_right_it = find_if(right_trees, is_blocked);
        right_score = blocked_right_it == end(right_trees) ?
                          size(right_trees) :
                          distance(begin(right_trees), blocked_right_it) + 1;
    }

    int up_score = 0;
    if (row_idx > 0)
    {
        const auto blocked_up_it = find_if(up_trees, is_blocked);
        up_score = blocked_up_it == end(up_trees) ? size(up_trees) :
                                                    distance(begin(up_trees), blocked_up_it) + 1;
    }

    int down_score = 0;
    if (row_idx < grid.size() - 1)
    {
        const auto blocked_down_it = find_if(down_trees, is_blocked);
        down_score = blocked_down_it == end(down_trees) ?
                         size(down_trees) :
                         distance(begin(down_trees), blocked_down_it) + 1;
    }

    return left_score * right_score * up_score * down_score;
}

int part1(const Grid& grid, const Grid& grid_t)
{
    return accumulate(grid | enumerate | transform([&grid, &grid_t](const auto& idx_and_row) {
                          const auto& [row_idx, row] = idx_and_row;
                          return accumulate(
                              row | enumerate |
                                  transform([&grid, &grid_t, &row_idx](const auto& idx_and_height) {
                                      const auto [col_idx, _] = idx_and_height;
                                      return isVisible(grid, grid_t, row_idx, col_idx);
                                  }),
                              0);
                      }),
                      0);
}

int part2(const Grid& grid, const Grid& grid_t)
{
    return max(grid | enumerate | transform([&grid, &grid_t](const auto& idx_and_row) {
                   const auto& [row_idx, row] = idx_and_row;
                   return max(row | enumerate |
                              transform([&grid, &grid_t, &row_idx](const auto& idx_and_height) {
                                  const auto [col_idx, _] = idx_and_height;
                                  return scenicScore(grid, grid_t, row_idx, col_idx);
                              }));
               }));
}

int main()
{
    const auto input = readStdIn();

    const auto grid = input | transform([](const auto& row) {
                          return row | transform([](const auto height) { return height - '0'; }) |
                                 to<std::vector>();
                      }) |
                      to<std::vector>();

    const auto grid_t = transpose(grid);

    std::cout << "Part 1: " << part1(grid, grid_t) << std::endl;
    std::cout << "Part 2: " << part2(grid, grid_t) << std::endl;
}