#ifndef PROGRESS_H
#define PROGRESS_H

#include <iostream>
#include <chrono>
#include <thread>

void fDisplayProgress(int progress)
{
    // std::cout << progress << "%" << std::endl;

    std::cout << "\033[2K"; // Erase the entire current line
    std::cout << "\033[0G"; // Move the cursor to the beginning of the line
    std::cout << "[ ";
    for (int i = 0; i < 10; i++)
    {
        i < progress / 10 ? std::cout << "# # " : std::cout << "    ";
    }
    std::cout << "] " << progress << "%" << std::flush; // Flush the output stream to update the display
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

#endif