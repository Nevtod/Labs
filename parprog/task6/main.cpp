#include "omp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <algorithm>
using std::cout;
using std::endl;
using std::vector;
#define SIZE 1000000001

// Количество простых чисел мы будем считать с помощью решета Эратосфена
// https://e-maxx.ru/algo/eratosthenes_sieve

bool prime[SIZE];  //here we will mark checked numbers

int main (int argc, char* argv[])
{
    double start = omp_get_wtime();
    long long N = atoll (argv[1]); // number of series' members
    int NT = omp_get_max_threads();  //number of threads
    
    for (int i = 0; i < SIZE; i++)
        prime[i] = true;

    prime[0] = false;
    long long lastCheckedDivider = 1;
    long long minStart;
    long long curN;
    #pragma omp parallel //shared(lastCheckedDivider, minStart) 
    {
        while (lastCheckedDivider < N)
        {
            int rank = omp_get_thread_num();

            #pragma omp single
            {
                do lastCheckedDivider++;
                while (lastCheckedDivider < N && !prime[lastCheckedDivider]);
                minStart = lastCheckedDivider * lastCheckedDivider;
                curN = N - minStart + 1;
            }
            
            if (curN > 0)
            {
                long long blockSize = curN / NT;
                long long remainder = curN % NT;
                long long begin = blockSize * rank + ((rank < remainder) ? rank : remainder);
                long long end = begin + blockSize - (rank >= remainder ? 1 : 0);
                begin += minStart;
                end += minStart;
                begin = begin + (lastCheckedDivider - begin % lastCheckedDivider) % lastCheckedDivider;
                // if (lastCheckedDivider == 3)
                // {
                // #pragma omp critical
                // {
                //     cout << begin << " " << end << endl;
                // }
                // }

                for (long long k = begin; k <= end; k += lastCheckedDivider)
                    prime[k] = false;

                #pragma omp barrier
            }
            else
                break;
        }
    }
    double elapsedSeconds = omp_get_wtime() - start;

    cout << "ElapsedTime = " << elapsedSeconds * 1000 << "[ms]" << endl;
    cout << "All prime numbers lower " << N << ": " << endl;
    // for (long long i = 1; i <= N; i++)
    //     if (prime[i])
    //     {
    //         cout << i << " ";
    //     }
    // return 0;
}