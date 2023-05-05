#ifndef WITHFILE_H_
#define WITHFILE_H_

#include "../ResultOkErr/Result.hpp"

#include <fstream>
#include <functional>

namespace nstd
{

    /*
     * Opens a file at the given path with the specified mode and executes a
     * user-provided function on the file object. Any additional arguments
     * passed to the function are also forwarded. The file is automatically
     * closed at the end of the function call.
     *
     * Parameters:
     * - path: A string containing the file path.
     * - mode: A string containing the file mode.
     * - func: A callable object that takes a std::fstream reference and any
     *         number of additional arguments.
     * - args: Any number of additional arguments to forward to the function.
     *
     * Return:
     * - If the file was opened successfully and the function executed without
     *   errors, returns a Result object containing an empty Ok variant.
     *   Otherwise, returns a Result object containing an Err variant with a
     *   description of the error.
     */
    template <typename Func, typename... Args>
    Result with_file(const std::string &path, const std::string &mode, Func func, Args &&...args)
    {
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::app);

        if (!file.is_open())
            return Result{false, nstd::Err{"Error: Unable to open file."}};

        func(file, std::forward<Args>(args)...);
        file.close();
        return Result{true, nstd::Ok{}};
    }

} // namespace nstd

#endif
