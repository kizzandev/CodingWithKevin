#include "print.hpp"

class Test
{
public:
    // This is called: conversion operator
    operator const char *() const { return "Test"; };
};

int main()
{
    Test test;

    nstd::print("Hello", " ", "World", "!"); // Hello World!
    nstd::print("1 + 1 = ", 2);              // 1 + 1 = 2
    nstd::print(test);                       // Test
    nstd::print();                           // [empty line]

    nstd::print(nstd::format("There was a {} in the {}.", "lion", "cave")); // There was a lion in the cave.
    nstd::print(nstd::format("{} + {} = {}", "1", "2", 3));                 // 1 + 2 = 3

    nstd::print(nstd::format("{} {}", 1, 2, 3)); // 1 2
    nstd::print(nstd::format("{} {}", 1));       // 1 {}
}
