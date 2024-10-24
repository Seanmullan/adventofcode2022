#include <map>
#include <parsing.hpp>
#include <range/v3/action/split.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/drop_last.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <regex>
#include <stack>
#include <vector>

using ranges::at;
using ranges::back;
using ranges::for_each;
using ranges::front;
using ranges::to;
using ranges::action::split;
using ranges::view::drop_last;
using ranges::view::iota;
using ranges::view::reverse;
using ranges::view::transform;
using ranges::view::zip;

using Stacks = std::map<char, std::stack<char>>;

struct Instruction
{
    int quantity;
    char from_stack;
    char to_stack;
};
using Instructions = std::vector<Instruction>;

std::string part1(Stacks stacks, const Instructions& instructions)
{
    for_each(instructions, [&stacks](const auto& instruction) {
        auto& from_stack = stacks[instruction.from_stack];
        auto& to_stack = stacks[instruction.to_stack];

        for_each(iota(0, instruction.quantity), [&from_stack, &to_stack](const auto _) {
            to_stack.push(from_stack.top());
            from_stack.pop();
        });
    });

    const auto top_crates = stacks | transform([](const auto& label_and_stack) {
                                const auto& [_, stack] = label_and_stack;
                                return stack.empty() ? '\0' : stack.top();
                            });
    return std::string(top_crates.begin(), top_crates.end());
}

std::string part2(Stacks stacks, const Instructions& instructions)
{
    for_each(instructions, [&stacks](const auto& instruction) {
        auto& from_stack = stacks[instruction.from_stack];
        auto& to_stack = stacks[instruction.to_stack];

        std::vector<char> crates;
        for_each(iota(0, instruction.quantity), [&crates, &from_stack](const auto _) {
            crates.push_back(from_stack.top());
            from_stack.pop();
        });
        for_each(crates | reverse, [&to_stack](const auto crate) { to_stack.push(crate); });
    });

    const auto top_crates = stacks | transform([](const auto& label_and_stack) {
                                const auto& [_, stack] = label_and_stack;
                                return stack.empty() ? '\0' : stack.top();
                            });
    return std::string(top_crates.begin(), top_crates.end());
}

int main()
{
    const auto inputs = split(readStdIn(), {});

    const auto make_stacks = [](const auto& input) {
        const auto& stack_labels = back(input);
        Stacks stacks;

        for_each(input | drop_last(1) | reverse, [&stacks, &stack_labels](const auto& crates) {
            for_each(zip(crates, stack_labels), [&stacks](const auto& crate_and_stack_label) {
                const auto [crate, stack_label] = crate_and_stack_label;
                if (std::isalpha(crate))
                {
                    if (!stacks.contains(stack_label))
                    {
                        stacks[stack_label] = std::stack<char>{};
                    }
                    stacks[stack_label].push(crate);
                }
            });
        });
        return stacks;
    };

    const auto make_instructions = [](const auto& input) {
        return input | transform([](const auto& instruction) {
                   const std::regex pattern("(move )(\\d+)( from )(\\d+)( to )(\\d+)");
                   std::smatch matches;
                   if (std::regex_match(instruction, matches, pattern))
                   {
                       const auto quantity = std::stoi(matches[2]);
                       const auto from_stack = matches[4].str()[0];
                       const auto to_stack = matches[6].str()[0];
                       return Instruction(quantity, from_stack, to_stack);
                   }
                   else
                   {
                       throw std::invalid_argument("No match found for " + instruction);
                   }
               }) |
               to<Instructions>();
    };

    const auto stacks = make_stacks(front(inputs));
    const auto instructions = make_instructions(back(inputs));

    std::cout << "Part 1: " << part1(stacks, instructions) << std::endl;
    std::cout << "Part 2: " << part2(stacks, instructions) << std::endl;
}