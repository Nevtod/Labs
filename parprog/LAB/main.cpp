#include "omp.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cmath>
// #define ISIZE 1000
// #define JSIZE 1000
#define ISIZE 10000
#define JSIZE 10000

using std::cout;
using std::endl;


double a[ISIZE][JSIZE];
double start = 0, finish = 0;

int main(int argc, char *argv[])
{

    int ran = atoi(argv[1]); 
    #pragma omp parallel for
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }
    

    start = omp_get_wtime();
    int i = 1;
    #pragma omp parallel shared(i) 
    {
        #pragma omp master
        {
            int nthreads = omp_get_num_threads();
            cout << nthreads << "\t";
        }
        while (i < ISIZE - 1)
        {
            // #pragma omp barrier
            // cout << j << endl;
            #pragma omp for
            for (int j = 6; j < JSIZE - 1; j++)
            {
                a[i][j] = sin(0.00001 * a[i + 1][j - 6]);
            }
            
            #pragma omp single
            i++;
        }
    }
    finish = omp_get_wtime();
    
    // std::ofstream ff;
    // ff.open("result.txt");
    // // ff = fopen("result.txt","w");
    // for(int i = 0; i < ISIZE; i++)
    // {
    //     for (int j = 0; j < JSIZE; j++)
    //     {
    //         ff <<  a[i][j] << " ";
    //     }
    //     ff << endl;
    // }
    // ff.close();
    cout << (finish - start) << "\t" << ran << endl;
}