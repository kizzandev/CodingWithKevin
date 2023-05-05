#ifndef PRINT_H_
#define PRINT_H_

#include <iostream>
#include <sstream>

namespace nstd
{

    /*
    Variadic templates allow function or class templates to accept an arbitrary
    number of arguments of any type.
    Example: template <typename... Args>

    Fold expressions allow you to apply a binary operator to a parameter pack.
    Example: (std::cout << ... << args)
    This is equivalent to: std::cout << arg1 << arg2 ...
    */

    template <typename... Args>
    void print(Args... args) { (std::cout << ... << args) << std::endl; }

    // Base case: no more arguments to replace
    void _replace_placeholders(std::string &result) {}

    template <typename Arg, typename... Args>
    void _replace_placeholders(std::string &result, Arg arg, Args... args)
    {
        std::size_t pos = result.find("{}");

        if (pos != std::string::npos)
        {
            std::stringstream ss;
            ss << arg;
            result.replace(pos, 2, ss.str());
            _replace_placeholders(result, args...);
        }
    }

    template <typename... Args>
    std::string format(const std::string &fmt, Args... args)
    {
        std::string result = fmt;
        _replace_placeholders(result, args...);
        return result;
    }

} // namespace nstd

#endif
