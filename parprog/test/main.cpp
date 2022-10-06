#include "omp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <chrono>
using std::cout;
using std::endl;
using std::vector;


// Количество простых чисел мы будем считать с помощью решета Эратосфена
// https://e-maxx.ru/algo/eratosthenes_sieve

vector<bool> prime;  //here we will mark checked numbers

int main (int argc, char* argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    omp_lock_t lock;
    omp_init_lock(&lock);

    uint64_t N = atoll (argv[1]); // number of series' members
    double sum = 0;
    int NT = omp_get_max_threads();  //number of threads
    vector<uint64_t> divider (NT, N);
    prime.resize(N + 1, true);

    uint64_t lastCheckedDivider = 1;
    prime[0] = false;

     #pragma omp parallel
    {
        for (int i = 0; i <= N; i++)
            prime[i] = false;
    }
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // cout << "ElapsedTime = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[us]" << endl;

    cout << "All prime numbers lower " << N << ": " << endl;
    for (uint64_t i = 1; i <= N; i++)
        if (prime[i])
            cout << i << " ";

    return 0;
}