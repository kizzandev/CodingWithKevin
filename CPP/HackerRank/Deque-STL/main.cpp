#include <iostream>
#include <deque>
using namespace std;

void printKMax(int arr[], int n, int k)
{
    //Write your code here.
    if (n == 1) // Only 1 element
    {
        cout << arr[0] << endl;
        return;
    }
    int result = 0, i = 0; // cout, iter, num
    if (k == 1)            // Array of 1
    {
        for (i = 0; i < n; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
        return;
    }
    deque<int> dq;
    while (i < n)
    {
        dq.push_back(arr[i]);   // add
        if (dq.back() > result) // checks if the last is gt result
        {
            result = dq.back();
        }
        if (dq.size() == k) // check if it should start printing results
        {
            cout << result << " ";                  // cout
            if (dq[0] == result && dq[1] != result) // find highest if highest was the first value
            {                                       // and the second is different
                result = dq[1];
                for (int j = 2; j < dq.size(); j++)
                {
                    if (dq[j] > result)
                    {
                        result = dq[j];
                    }
                }
            }
            dq.pop_front(); // remove first
        }
        i++;
    }
    cout << endl;
}

int main()
{

    int t;
    cin >> t;
    while (t > 0)
    {
        int n, k;
        cin >> n >> k;
        int i;
        int arr[n];
        for (i = 0; i < n; i++)
            cin >> arr[i];
        printKMax(arr, n, k);
        t--;
    }
    return 0;
}