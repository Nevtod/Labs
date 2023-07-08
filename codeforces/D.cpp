#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

int main()
{
  int n;
  cin >> n;
  for (int i = 0; i < n; i++)
  {
    long long k;
    cin >> k;
    if (k % 2 == 1)
    {
      cout << "2" << endl;
      continue;
    }

    while (k % 2 == 0) k /= 2;
    if (k == 1) 
      cout << "-1" << endl;
    else
      cout << k << endl;
  }

}