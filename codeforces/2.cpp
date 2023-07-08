#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main()
{
  int t;
  cin >> t;
  for (int i = 0; i < t; i++)
  {
      int n;
      cin >> n;

      int mMin = 1000000001;
      int mMax = 0;
      int left = 0, right = 0;
      for (int j = 0; j < n; j++)
      {
        int val;
        cin >> val;
        if (val > mMax) 
        {
            right = j;
            mMax = val;
        }
        if (val < mMin)
        {
            left = j;
            mMin = val;
        } 
      }
      cout << left + 1 << " " << right + 1 << endl;
  }
}