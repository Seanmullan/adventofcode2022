#include <parsing.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/slice.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/unique.hpp>

using ranges::accumulate;
using ranges::front;
using ranges::to;
using ranges::action::sort;
using ranges::view::chunk;
using ranges::view::set_intersection;
using ranges::view::slice;
using ranges::view::transform;
using ranges::view::unique;

int toPriority(const char c)
{
    return std::isupper(c) ? c - '0' + 10 : c - '0' - 48;
}

int part1(const auto& inputs)
{
    const auto get_priority = [](const auto& elements) {
        const auto half = (int)elements.size() / 2;
        const auto first = elements | slice(0, half) | to<std::vector>() | sort;
        const auto second = elements | slice(half, (int)elements.size()) | to<std::vector>() | sort;
        const auto common_elements = set_intersection(first, second) | unique;
        return toPriority(front(common_elements));
    };

    const auto priorities = inputs | transform(get_priority);
    return accumulate(priorities, 0);
}

int part2(const auto& inputs)
{
    const auto get_priority = [](const auto& elements) {
        const auto first = elements[0] | to<std::vector>() | sort;
        const auto second = elements[1] | to<std::vector>() | sort;
        const auto third = elements[2] | to<std::vector>() | sort;
        const auto common_elements =
            set_intersection(set_intersection(first, second), third) | unique;
        return toPriority(front(common_elements));
    };
    const auto priorities = inputs | chunk(3) | transform(get_priority);
    return accumulate(priorities, 0);
}

int main()
{
    const auto inputs = readStdIn();

    std::cout << "Part 1: " << part1(inputs) << std::endl;
    std::cout << "Part 2: " << part2(inputs) << std::endl;
}