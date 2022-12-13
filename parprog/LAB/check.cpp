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


/**
 * @brief return a begin index of the current block  [begin index, end index]
 * 
 * @param nBlocks is the number of blocks which assign itself some index space segment
 * @param N is a number of indexes or it is a size of index space
 * @param curBlockIndex is the index of current block in limits [0, nBlocks - 1]
 * @return int: index of current block begin
 */
int get_block_begin(int nBlocks, int N, int curBlockIndex)
{
    int blockSize = N / nBlocks;
    int remainder = N % nBlocks;
    int beginIndex = blockSize * curBlockIndex + ((curBlockIndex< remainder) ? curBlockIndex : remainder);

    return beginIndex;
}

/**
 * @brief return an end index of the current block  [begin index, end index]
 * 
 * @param nBlocks is the number of blocks which assign itself some index space segment
 * @param N is a number of indexes or it is a size of index space
 * @param curBlockIndex is the index of current block in limits [0, nBlocks - 1]
 * @return int: index of the current block end
 */
int get_block_end(int nBlocks, int N, int curBlockIndex)
{
    int blockSize = N / nBlocks;
    int remainder = N % nBlocks;
    int beginIndex = blockSize * curBlockIndex + ((curBlockIndex< remainder) ? curBlockIndex : remainder);
    int endIndex = beginIndex + blockSize - (curBlockIndex >= remainder ? 1 : 0);

    return endIndex;
}

int main(int argc, char **argv)
{
    int NP = atoi(argv[1]);
    int rank = atoi(argv[2]);
    double arr[ISIZE][JSIZE];
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            arr[i][j] = 10 * i + j;
        }
    }
    
    for (int i = 1; i < ISIZE - 1; i++)
    {
        for (int j = 6; j < JSIZE - 1; j++)
        {
            arr[i][j] = sin(0.00001 * arr[i + 1][j - 6]);
        }
    }

    int begin = get_block_begin(NP, JSIZE - 7, rank);
    int end = get_block_end(NP, JSIZE - 7, rank);

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

 

    
    std::ifstream fin1, fin2;
    fin1.open("result.txt");
    // fin2.open("resultref.txt");
    // ff = fopen("result.txt","w");
    int cnt = 0;
    for(int i = 0; i < ISIZE - 1; i++)
    {
        for (int j = begin + 6; j < 6 + end; j++)
        {
            double a, b;
            fin1 >> a;
            b = arr[i][j];
            if (std::abs(a - b) > 1e-6)
                cnt++;
        }
    }
    fin1.close();
    // fin2.close();
    cout << "Errors: " << cnt;
}