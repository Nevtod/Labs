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

// Количество простых чисел мы искать полным перебором
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
    #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        //split all natural numbers lower N on NT blocks 
        long long blockSize = N / NT;
        long long remainder = N % NT;
        long long begin = blockSize * rank + ((rank < remainder) ? rank : remainder);
        long long end = begin + blockSize - (rank >= remainder ? 1 : 0);
        
        // main algorithm body
        for (long long k = begin; k <= end; k++)
            for (long long j = 2; j <= k / 2; j++)
                if (k % j == 0)
                {
                    prime[k] = false;
                    break;
                }
    }
    double elapsedSeconds = omp_get_wtime() - start;

    cout << "ElapsedTime = " << elapsedSeconds * 1000 << "[ms]" << endl;
    // cout << "All prime numbers lower " << N << ": " << endl;
    // for (long long i = 1; i <= N; i++)
    //     if (prime[i])
    //     {
    //         cout << i << " ";
    //     }
    return 0;
}