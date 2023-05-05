#include "withfile.hpp"
#include "../PrintFunction/print.hpp"

void ReadFile(std::fstream &file)
{
    std::string line;
    while (std::getline(file, line))
        nstd::print(line);
}

void WriteFile(std::fstream &file, const std::string &to_write)
{
    file << to_write << std::endl;
}

int main(int argc, char const *argv[])
{
    nstd::with_file(
        "test.txt", "w", [](std::fstream &file)
        { file << "Hello, world!" << std::endl; });

    nstd::print("WriteFile:");
    if (nstd::with_file("test.txt", "w", WriteFile, "This is a test.").ok)
        nstd::print("It works!");

    nstd::print("\nReadFile:");
    nstd::with_file("test.txt", "r", ReadFile);

    return 0;
}
