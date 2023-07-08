#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

int main()
{
  // freopen("in", "r", stdin);
  int t;
  cin >> t;

  for (int i = 0; i < t; i++)
  {
    int n;
    cin >> n;
    set<int> s;

    bool zero = false;
    bool one = false;

    for (int j = 0; j < n; j++)
    {
      int val;
      cin >> val;
      s.insert(val);
      if (val == 0) zero = true;
      if (val == 1) one = true;
    }

    if ((zero && !one) || (!zero && !one))
    {
      cout << "YES" << endl;
      continue;
    }
    if (zero && one)
    {
      cout << "NO" << endl;
      continue;
    }

    int prev = -2;
    bool res = true;
    for (int el : s)
    {
      if (el - prev == 1)
      {
        res = false;
        break;
      }

      prev = el;
    }

    if (res) cout << "YES" << endl;
    else cout << "NO" << endl;
  }
}