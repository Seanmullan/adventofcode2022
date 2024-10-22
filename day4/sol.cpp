#include <parsing.hpp>
#include <range/v3/action/split.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/transform.hpp>

using ranges::accumulate;
using ranges::back;
using ranges::front;
using ranges::action::split;
using ranges::view::transform;

int part1(const auto& ranges)
{
    const auto is_contained = [](const auto& ranges) {
        const auto [first_range, second_range] = ranges;
        const auto [first_lower, first_upper] = first_range;
        const auto [second_lower, second_upper] = second_range;
        return (first_lower >= second_lower && first_upper <= second_upper) ||
               (second_lower >= first_lower && second_upper <= first_upper);
    };
    return accumulate(ranges | transform(is_contained), 0);
}

int part2(const auto& ranges)
{
    const auto overlaps = [](const auto& ranges) {
        const auto [first_range, second_range] = ranges;
        const auto [first_lower, first_upper] = first_range;
        const auto [second_lower, second_upper] = second_range;
        return first_lower <= second_upper && second_lower <= first_upper;
    };
    return accumulate(ranges | transform(overlaps), 0);
}

int main()
{
    const auto inputs = readStdIn();

    const auto get_ranges = [&](const auto& input) {
        const auto ranges = split(input, ',');
        const auto first_range = split(front(ranges), '-');
        const auto second_range = split(back(ranges), '-');
        return std::make_pair(
            std::make_pair(std::stoi(front(first_range)), std::stoi(back(first_range))),
            std::make_pair(std::stoi(front(second_range)), std::stoi(back(second_range))));
    };

    const auto ranges = inputs | transform(get_ranges);
    std::cout << "Part 1: " << part1(ranges) << std::endl;
    std::cout << "Part 2: " << part2(ranges) << std::endl;
}