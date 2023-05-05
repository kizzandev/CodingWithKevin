#ifndef RESULT_H_
#define RESULT_H_

#include <variant>
#include <string>
#include <any>

namespace nstd
{

    struct Ok
    {
        std::any value;
    };

    struct Err
    {
        std::string message;
    };

    struct Result
    {
        bool ok;
        std::variant<Ok, Err> data;
    };

    template <typename T>
    T get_result(const Result &result)
    {
        return std::any_cast<T>(std::get<Ok>(result.data).value);
    }

    std::string get_error(const Result &result, const char *default_error)
    {
        if (!result.ok)
            return std::get<Err>(result.data).message;

        return default_error;
    }

    std::string get_error(const Result &result)
    {
        if (!result.ok)
            return std::get<Err>(result.data).message;

        return "Default error";
    }

} // namespace nstd

#endif
