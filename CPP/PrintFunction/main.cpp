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

    print("Hello", " ", "World", "!"); // Hello World!
    print("1 + 1 = ", 2);              // 1 + 1 = 2
    print(test);                       // Test
    print();                           // [empty line]

    print(format("There was a {} in the {}.", "lion", "cave")); // There was a lion in the cave.
    print(format("{} + {} = {}", "1", "2", 3));                 // 1 + 2 = 3

    print(format("{} {}", 1, 2, 3)); // 1 2
    print(format("{} {}", 1));       // 1 {}
}
