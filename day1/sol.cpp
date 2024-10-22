#include <parsing.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/at.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/take_last.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

using ranges::accumulate;
using ranges::back;
using ranges::to;
using ranges::action::sort;
using ranges::view::chunk_by;
using ranges::view::filter;
using ranges::view::take_last;
using ranges::view::transform;

int main()
{
    const auto inputs = readStdIn();

    const auto group_inputs = [](const auto& inputs) {
        return inputs | chunk_by([](const auto& curr, const auto& next) {
                   return !curr.empty() && !next.empty();
               }) |
               filter([](const auto& input) { return !input.empty() && input[0] != ""; });
    };

    const auto calories_per_elf =
        group_inputs(inputs) | transform([](const auto& calories) {
            return accumulate(
                calories | transform([](const auto& calorie) { return std::stoi(calorie); }), 0);
        }) |
        to<std::vector>() | sort;

    std::cout << "Part 1: " << back(calories_per_elf) << std::endl;
    std::cout << "Part 2: " << accumulate(calories_per_elf | take_last(3), 0) << std::endl;
}