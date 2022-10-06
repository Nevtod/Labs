#include "omp.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main (int argc, char* argv[])
{
    int N = atoi (argv[1]);    // array size
    cout << "N = " << N << endl;

    vector<int> vals (N);      // array initializaition
    for (int i = 0; i < N; i++)
        vals.at (i) = i + 1;
    
    int sum = 0;

    #pragma omp parallel shared (vals, sum)
    {
        #pragma omp for reduction (+:sum) schedule (static)
        for (int i = 0; i < N; i++)
            sum += vals[i];
    }

    cout << "Sum of the first " << N << " natural numbers is " << sum;

    return 0; 
}