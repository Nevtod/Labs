#include "omp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <string>
#include <iterator>
#include <fstream>
#include <cmath>
// #define ISIZE 1000
// #define JSIZE 1000
#define ISIZE 1000
#define JSIZE 1000

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::string;


int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }
    
    for (int i = 1; i < ISIZE - 1; i++)
    {
        for (int j = 6; j < JSIZE - 1; j++)
        {
            a[i][j] = sin(0.00001 * a[i + 1][j - 6]);
        }
    }

    // int j = 6;
    // #pragma omp parallel shared(j)
    // {
    //     while (j < JSIZE - 1)
    //     {

    //         #pragma omp single
    //         j++;

    //         // cout << j << endl;
    //         #pragma omp for schedule (static) nowait
    //         for (int i = 1; i < ISIZE - 1; i++)
    //         {
    //             a[i][j] = sin(0.00001 * a[i + 1][j - 6]);
    //         }
    //     }
    // }

    std::ofstream ff;
    ff.open("resultref.txt");
    // ff = fopen("result.txt","w");
    for(int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            ff <<  a[i][j] << " ";
        }
        ff << endl;
    }
    ff.close();
}