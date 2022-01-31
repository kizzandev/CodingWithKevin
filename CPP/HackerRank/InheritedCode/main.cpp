#include <iostream>
#include <string>
#include <sstream>
#include <exception>
using namespace std;

/*
You inherited a piece of code that performs username validation for your company's website. The existing function works reasonably well, but it throws an exception when the username is too short. Upon review, you realize that nobody ever defined the exception.

The inherited code is provided for you in the locked section of your editor. Complete the code so that, when an exception is thrown, it prints Too short: n (where
is the length of the given username).
*/

/* Start Challenge */
struct BadLengthException : exception
{
    string s;
    BadLengthException(int n) { s = to_string(n); }
    const char *what()
    {
        return s.c_str();
    }
};
/* End of Challenge */

bool checkUsername(string username)
{
    bool isValid = true;
    int n = username.length();
    if (n < 5)
    {
        throw BadLengthException(n);
    }
    for (int i = 0; i < n - 1; i++)
    {
        if (username[i] == 'w' && username[i + 1] == 'w')
        {
            isValid = false;
        }
    }
    return isValid;
}

int main()
{
    int T;
    cin >> T;
    while (T--)
    {
        string username;
        cin >> username;
        try
        {
            bool isValid = checkUsername(username);
            if (isValid)
            {
                cout << "Valid" << '\n';
            }
            else
            {
                cout << "Invalid" << '\n';
            }
        }
        catch (BadLengthException e)
        {
            cout << "Too short: " << e.what() << '\n';
        }
    }
    return 0;
}