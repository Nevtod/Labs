#include "omp.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

long long get_window_value (int previous, int current, int next)
{
    return previous * current * next / 3;
}

int main (int argc, char* argv[])
{
    // Initial array size;
    int N = 1e1;

    vector<int> a (N);      // initial array initializaition
    for (int i = 0; i < N; i++)
        a.at (i) = i + 1;
    vector<double> b (N);       // result array with calculated window

    
    #pragma omp parallel
    {
        #pragma omp for schedule (static)
        for (int i = 0; i < N; i++)
        {
            // On the edges we take missing edge element for zero     
            int prev = (i == 0) ? 0 : a.at (i - 1);
            int next = (i == N - 1) ? 0 : a.at (i + 1);

            b.at (i) = get_window_value (prev, a.at (i), next);
        }
    }

    for (int i = 0; i < N; i++)
        cout << b.at (i) << " ";

    return 0; 
}