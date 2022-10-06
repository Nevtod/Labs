#include "omp.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using std::cout;
using std::endl;

int main (int argc, char* argv[])
{
    double x = 1; // the power to which we raise exp

    int N = atoi (argv[1]); // number of series' members for command line
    double sum = 0;
    int NT = omp_get_max_threads();  //number of threads

     #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        double curSum = 0;
        if (rank + 1 < N)   // check that we have more iterations than thread's number
        {
            // split iterations evenly into threads
            int blockSize = N / NT;
            int remainder = N % NT;
            int begin = blockSize * rank + ((rank < remainder) ? rank : remainder);
            int end = begin + blockSize - (rank >= remainder ? 1 : 0);
            
            double curTerm = 1;
            for (int i = 1; i <= begin; i++)
                curTerm *= x / (double) i;      //compute first series' term for current thread 

            for (int i = begin; i <= end; i++)  // compute sum on current thread
            {
                curSum += curTerm;
                curTerm *= x / (double) (i + 1);
            }

            // final sum over all threads
            #pragma omp critical (sum)
            {
                sum += curSum;
            }
        }
    }

    cout.precision(20);
    cout << "exp in power " << x << " is " << sum << "\nerror is " << fabs(sum - M_Ef128);
    return 0;
}