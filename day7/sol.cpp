#include <memory>
#include <parsing.hpp>
#include <range/v3/action/split.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/transform.hpp>
#include <stack>
#include <vector>

using ranges::accumulate;
using ranges::back;
using ranges::end;
using ranges::find_if;
using ranges::front;
using ranges::action::split;
using ranges::view::drop;
using ranges::view::transform;

class Directory;
using DirectoryPtr = std::shared_ptr<Directory>;

struct File
{
    std::string name;
    uint64_t size;
};

struct Directory
{
    std::string name;
    DirectoryPtr parent;
    std::vector<DirectoryPtr> children;
    std::vector<File> files;
    uint64_t size;
};

DirectoryPtr changeDirectory(const DirectoryPtr current_dir, const std::string& command)
{
    const auto cmd_str = split(command, ' ');
    const auto name = back(cmd_str);

    if (name == "..")
    {
        return current_dir->parent;
    }

    const auto found_dir =
        find_if(current_dir->children, [&name](const auto child) { return child->name == name; });

    if (found_dir != end(current_dir->children))
    {
        return *found_dir;
    }

    throw std::invalid_argument("Could not change directory from " + current_dir->name + " to " +
                                name);
}

void addDirectory(DirectoryPtr current_dir, const std::string& command)
{
    const auto cmd_str = split(command, ' ');
    const auto name = back(cmd_str);
    const auto found_dir =
        find_if(current_dir->children, [&name](const auto child) { return child->name == name; });

    if (found_dir == end(current_dir->children))
    {
        DirectoryPtr new_dir = std::make_shared<Directory>(name, current_dir);
        current_dir->children.push_back(new_dir);
    }
}

void addFile(DirectoryPtr current_dir, const std::string& line)
{
    const auto file_str = split(line, ' ');
    const auto size = std::stoi(front(file_str));
    const auto name = back(file_str);
    current_dir->files.emplace_back(name, size);
}

void calculateSize(DirectoryPtr current_dir)
{
    const auto file_sizes =
        accumulate(current_dir->files | transform([](const auto& file) { return file.size; }), 0);

    const auto directory_sizes = accumulate(current_dir->children | transform([](const auto child) {
                                                calculateSize(child);
                                                return child->size;
                                            }),
                                            0);

    current_dir->size = file_sizes + directory_sizes;
}

uint64_t part1(DirectoryPtr current_dir)
{
    const auto size_limit = 100000;
    std::stack<DirectoryPtr> stack;
    stack.push(current_dir);

    uint64_t limited_size = 0;
    while (!stack.empty())
    {
        const auto curr = stack.top();
        stack.pop();
        if (curr->size <= size_limit)
        {
            limited_size += curr->size;
        }
        for (const auto child : curr->children)
        {
            stack.push(child);
        }
    }
    return limited_size;
}

uint64_t part2(DirectoryPtr current_dir)
{
    const auto total_space = 70000000;
    const auto required_space = 30000000;
    const auto free_space = total_space - current_dir->size;
    const auto space_to_free = required_space - free_space;

    std::stack<DirectoryPtr> stack;
    stack.push(current_dir);

    uint64_t min_sufficient_dir_size = total_space;
    while (!stack.empty())
    {
        const auto curr = stack.top();
        stack.pop();
        if (curr->size >= space_to_free && curr->size < min_sufficient_dir_size)
        {
            min_sufficient_dir_size = curr->size;
        }
        for (const auto child : curr->children)
        {
            stack.push(child);
        }
    }
    return min_sufficient_dir_size;
}

int main()
{
    const auto input = readStdIn();

    DirectoryPtr current_dir = std::make_shared<Directory>("/");
    DirectoryPtr root_dir = current_dir;
    for (const auto& line : input | drop(1))
    {
        if (line.starts_with("$ cd"))
        {
            current_dir = changeDirectory(current_dir, line);
        }
        else if (line.starts_with("dir"))
        {
            addDirectory(current_dir, line);
        }
        else if (!line.starts_with("$ ls"))
        {
            addFile(current_dir, line);
        }
    }
    calculateSize(root_dir);

    std::cout << "Part 1: " << part1(root_dir) << std::endl;
    std::cout << "Part 2: " << part2(root_dir) << std::endl;
}