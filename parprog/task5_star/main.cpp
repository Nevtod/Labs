#include "omp.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using std::cout;
using std::endl;


// In this program we calculates arcsin(x) series

double getNextTerm (double m, double curTerm, double x)
{
    return curTerm * x * x * m * m / ((m + 1) * (m + 2));
}


int main (int argc, char* argv[])
{
    double x = sqrt(2) / 2; // we calculates arcsin(x)

    int N = atoi (argv[1]); // number of series' members
    double sum = 0;
    int NT = omp_get_max_threads();  //number of threads

     #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        double curSum = 0;
        if (rank + 1 < N)   // check that we have more iterations than thread's number
        {
            int blockSize = N / NT;
            int remainder = N % NT;
            int begin = blockSize * rank + ((rank < remainder) ? rank : remainder);
            int end = begin + blockSize - (rank >= remainder ? 1 : 0);
            
            double curTerm = x;
            for (int i = 1; i <= begin; i++)
                curTerm = getNextTerm(2 * (i - 1) + 1, curTerm, x);

            for (int i = begin; i <= end; i++)
            {
                curSum += curTerm;
                curTerm = getNextTerm(2 * i + 1, curTerm, x);
            }

            #pragma omp critical (sum)
            {
                sum += curSum;
            }
        }
    }

    cout.precision(20);
    cout << "arcsin of " << x << " is " << sum << "\nerror is " << fabs(M_PI_4f128 - sum);
    return 0;
}