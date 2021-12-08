#include <iostream>
#include <random>

using namespace std;

// List of all functions
int main();
int step();
double random();
void search();
void searchEven();
void restartProgram();

// The Rabbit step
void step(int &pos)
{
    int prev = pos;
    // checks if the Rabbit is in a border and prevents it from leaving the field
    switch (pos)
    {
    case 99: // upper side of the field
        pos--;
        break;
    case 0: // lower side of the field
        pos++;
        break;
    default: // if it's not a in border, then moves randomly
        if (random() > 0.5)
            pos++;
        else
            pos--;
        break;
    }
    cout << "The rabbit jumped from " << prev << " to " << pos << endl;
}

// returns a random value between 0 and 1
double random()
{
    // from stack overflow, because idk how to random in c++ :(
    // this works nicely
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    return dis(gen);
}

// Looking for the Rabbit when starts at a random position
void search()
{
    // rabbit starts at random position
    int pos = floor(random() * 100);
    cout << "The rabbit started at " << pos << endl;
    // not found before search
    bool found = false;
    // attempting at
    int attempt = 0;
    // search direction -> true goes up, false goes down
    bool trend = true;
    // i check infinitelly until the rabbit is found
    while (!found)
    {
        int i;             // this is for the FORs
        if (trend == true) // going up
        {
            for (i = 0; i < 200; i++)
            {
                cout << "Checking " << attempt << endl;
                if (attempt == pos) // checks if my attempt and the Rabbit position are the same
                {
                    found = true;
                    cout << "The rabbit was found at " << pos << endl;
                    break;
                }
                // next attempted number
                if (i % 2 == 0)
                    attempt++;
                // rabbit step
                step(pos);
            }
            if (!found) // if not found, i reverse the seaching direction
                trend = false;
        }
        else // going down
        {
            for (i = 200; i > 0; i--)
            {
                cout << "Checking " << attempt << endl;
                if (attempt == pos) // checks if my attempt and the Rabbit position are the same
                {
                    found = true;
                    cout << "The rabbit was found at " << pos << endl;
                    break;
                }
                // next attempted number
                if (i % 2 == 0)
                    attempt--;
                // rabbit step
                step(pos);
            }
            if (!found) // if not found, i reverse the seaching direction
                trend = true;
        }
        if (found)
            break;
    }
}

// Looking for the Rabbit when starts at a random even position
void searchEven()
{
    // rabbit starts at random even position
    int pos;
    do
    {
        pos = floor(random() * 100);
    } while (pos % 2 != 0);
    cout << "The rabbit started at " << pos << endl;
    // not found before search
    bool found = false;
    // attempting at
    int attempt = 0;
    for (int i = 0; i < 100; i++)
    {
        cout << "Checking " << attempt << endl;
        // checks if my attempt and the Rabbit position are the same
        if (attempt == pos)
        {
            found = true;
            cout << "The rabbit was found at " << pos << endl;
            break;
        }
        attempt++;
        // rabbit step
        step(pos);
    }
}

// clear console and re run program
void restartProgram()
{
    system("cls");
    main();
}

int main()
{
    cout << "Press '1' to search the Rabbit that starts at a random position\nPress '2' to search the rabbit that starts at an even number\nPress '3' to exit" << endl;
    char x;
    cin >> x;
    if (x == '1')
        search();
    else if (x == '2')
        searchEven();
    else if (x == '3')
        exit(0);
    else
        restartProgram();
    cout << " \n";
    system("pause");
    restartProgram();

    return 0;
}
