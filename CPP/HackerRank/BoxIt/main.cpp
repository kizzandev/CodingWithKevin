#include <bits/stdc++.h>

using namespace std;
//Implement the class Box
//l,b,h are integers representing the dimensions of the box

// The class should have the following functions :

// Constructors:
// Box();
// Box(int,int,int);
// Box(Box);

// int getLength(); // Return box's length
// int getBreadth (); // Return box's breadth
// int getHeight ();  //Return box's height
// long long CalculateVolume(); // Return the volume of the box

//Overload operator < as specified
//bool operator<(Box& b)

//Overload operator << as specified
//ostream& operator<<(ostream& out, Box& B)

/* Challenge starts */
class Box
{
private:
    int l, b, h;

public:
    // constructors
    Box()
    {
        l = 0;
        b = 0;
        h = 0;
    }
    Box(int l_, int b_, int h_)
    {
        l = l_;
        b = b_;
        h = h_;
    }
    Box(Box &B2)
    {
        l = B2.getLength();
        b = B2.getBreadth();
        h = B2.getHeight();
    }
    // get
    int getLength() { return l; }
    int getBreadth() { return b; }
    int getHeight() { return h; }
    // calculate
    long long CalculateVolume() { return l * (long long)(b)*h; }
    // overload
    bool operator<(Box &B2) { return l < B2.getLength() || (b < B2.getBreadth() && l == B2.getLength()) || (h < B2.getHeight() && b < B2.getBreadth() && l < B2.getLength()); }
    friend ostream &operator<<(ostream &out, Box &B2) { return out << B2.getLength() << " " << B2.getBreadth() << " " << B2.getHeight(); }
};
/* End of challenge */

void check2()
{
    int n;
    cin >> n;
    Box temp;
    for (int i = 0; i < n; i++)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            cout << temp << endl;
        }
        if (type == 2)
        {
            int l, b, h;
            cin >> l >> b >> h;
            Box NewBox(l, b, h);
            temp = NewBox;
            cout << temp << endl;
        }
        if (type == 3)
        {
            int l, b, h;
            cin >> l >> b >> h;
            Box NewBox(l, b, h);
            if (NewBox < temp)
            {
                cout << "Lesser\n";
            }
            else
            {
                cout << "Greater\n";
            }
        }
        if (type == 4)
        {
            cout << temp.CalculateVolume() << endl;
        }
        if (type == 5)
        {
            Box NewBox(temp);
            cout << NewBox << endl;
        }
    }
}

int main()
{
    check2();
}