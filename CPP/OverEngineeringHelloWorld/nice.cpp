#include <bits/stdc++.h>

int main(int argc, char const *argv[])
{
  for (int i = 0; i < 11; i++)
  {
    switch (i)
    {
    case 0:
      std::cout << "\tH";
      break;
    case 1:
      std::cout << "e";
      break;
    case 2:
    case 3:
    case 9:
      std::cout << "l";
      break;
    case 4:
    case 7:
      std::cout << "o";
      break;
    case 5:
      std::cout << " ";
      break;
    case 6:
      std::cout << "W";
      break;
    case 8:
      std::cout << "r";
      break;
    case 10:
      std::cout << "d";
      break;
    default:
      std::cout << std::endl;
      break;
    }
  }
  return 0;
}
