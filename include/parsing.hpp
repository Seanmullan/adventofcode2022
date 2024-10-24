#include <iostream>
#include <vector>

std::vector<std::string> readStdIn()
{
    std::vector<std::string> result;
    for (std::string line; std::getline(std::cin, line);)
    {
        result.push_back(line);
    }
    return result;
}
