#include "Result.hpp"
#include "../PrintFunction/print.hpp"

#include <iostream>

nstd::Result divide(float a, float b)
{
    if (b == 0)
        return nstd::Result{false, nstd::Err{"division by zero"}};

    return nstd::Result{true, nstd::Ok{a / b}};
}

int main(int argc, char const *argv[])
{
    nstd::Result result = divide(10, 2);
    result.ok ? nstd::print("Result: ", nstd::get_result<float>(result)) : nstd::print("Error: ", nstd::get_error(result)); // Result: 5
    
    result = divide(10, 0);
    result.ok ? nstd::print("Result: ", nstd::get_result<float>(result)) : nstd::print("Error: ", nstd::get_error(result)); // Error: division by zero

    return 0;
}
