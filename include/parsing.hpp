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

std::vector<std::string> splitString(const std::string& str, const char separator)
{
    std::vector<std::string> result;
    std::size_t start_idx = 0, end_idx = 0;
    for (std::size_t i = 0; i <= str.size(); i++)
    {
        if (str[i] == separator || i == str.size())
        {
            // Handle case when start of string is separator
            if (str[i] == separator && i == 0)
            {
                start_idx = i + 1;
                continue;
            }
            end_idx = i;
            std::string temp;
            temp.append(str, start_idx, end_idx - start_idx);
            if (!temp.empty())
            {
                result.push_back(temp);
            }
            start_idx = end_idx + 1;
        }
    }
    return result;
}