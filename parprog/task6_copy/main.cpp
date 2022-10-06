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
    
    for (int i = 0; i < SIZE; i++)
        prime[i] = true;

    prime[0] = false;
    long long lastCheckedDivider = 1;
    #pragma omp parallel //shared(lastCheckedDivider, minStart) 
    {
        while (lastCheckedDivider < N)
        {
            #pragma omp single
            {
                do lastCheckedDivider++;
                while (lastCheckedDivider < N && !prime[lastCheckedDivider]);
            }
            
             #pragma omp for
             for (long long k = lastCheckedDivider * lastCheckedDivider; k <= N; k += lastCheckedDivider) 
                prime[k] = false;

            #pragma omp barrier
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