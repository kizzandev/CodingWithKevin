#include <iostream>
#include <string>

#include "headers/huffman.hpp"
#include "headers/progress.hpp"

int main(int argc, char const *argv[])
{
   std::string path;
   bool fileExists = false;

   // Ask the user for the file path until a valid file is provided
   while (!fileExists)
   {
      std::cout << "Enter the file path: ";
      std::getline(std::cin, path);

      // Check if file exists
      FILE *f = fopen(path.c_str(), "r");
      if (f != nullptr)
      {
         fileExists = true;
         fclose(f);
      }
      else
         std::cerr << "There's no such file, please try again." << std::endl;
   }

   // Check if the path is valid and if it ends with .huf
   bool is_compressed = (path.size() > 4 && path.substr(path.size() - 4) == ".huf");

   // To compress or not to compress!
   is_compressed ? fDecompress(path, fDisplayProgress) : fCompress(path, fDisplayProgress);

   return 0;
}
