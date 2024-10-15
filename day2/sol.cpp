#include <map>
#include <parsing.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/transform.hpp>

using ranges::accumulate;
using ranges::views::transform;

const std::map<std::string, uint> SCORE_MAP_1 = {
    { "A X", 4 }, { "A Y", 8 }, { "A Z", 3 }, { "B X", 1 }, { "B Y", 5 },
    { "B Z", 9 }, { "C X", 7 }, { "C Y", 2 }, { "C Z", 6 },
};

const std::map<std::string, uint> SCORE_MAP_2 = {
    { "A X", 3 }, { "A Y", 4 }, { "A Z", 8 }, { "B X", 1 }, { "B Y", 5 },
    { "B Z", 9 }, { "C X", 2 }, { "C Y", 6 }, { "C Z", 7 },
};

int main()
{
    const auto input = readStdIn();
    const auto part_1 =
        accumulate(input | transform([](const auto& line) { return SCORE_MAP_1.at(line); }), 0);
    const auto part_2 =
        accumulate(input | transform([](const auto& line) { return SCORE_MAP_2.at(line); }), 0);

    std::cout << "Part 1: " << part_1 << std::endl;
    std::cout << "Part 2: " << part_2 << std::endl;
}