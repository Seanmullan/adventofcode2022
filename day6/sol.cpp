#include <parsing.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/sliding.hpp>

using ranges::begin;
using ranges::distance;
using ranges::find_if;
using ranges::front;
using ranges::size;
using ranges::to;
using ranges::action::sort;
using ranges::action::unique;
using ranges::view::sliding;

int findUniqueMarker(const auto& packet, size_t window_size)
{
    const auto windows = packet | sliding(window_size);
    const auto unique_marker_it = find_if(windows, [&window_size](const auto& window) {
        return size(window | to<std::vector> | sort | unique) == window_size;
    });
    return distance(begin(windows), unique_marker_it) + window_size;
}

int main()
{
    const auto input = readStdIn();
    std::cout << "Part 1: " << findUniqueMarker(front(input), 4) << std::endl;
    std::cout << "Part 2: " << findUniqueMarker(front(input), 14) << std::endl;
}