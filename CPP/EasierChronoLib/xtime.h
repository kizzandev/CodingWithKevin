#include <chrono>

class xtime
{
public:
    static std::chrono::_V2::system_clock::time_point now();
    static double result(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end);
};
/*Sets a point in time.*/
std::chrono::_V2::system_clock::time_point xtime::now()
{
    return std::chrono::high_resolution_clock::now();
}
/*Returns the difference. First point in time, second point in time.*/
double xtime::result(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}