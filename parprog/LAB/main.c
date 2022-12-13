#include "mpi/mpi.h"
#include "stdio.h"
#include <stdlib.h>
#include <math.h>

#define ISIZE 1000
#define JSIZE 1000


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


double a[ISIZE][JSIZE];
int main(int argc, char *argv[])
{
    // каждый поток хранит свою копию исходного массива
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {   
            a[i][j] = 10 * i + j;
        }
    }
    int ran = atoi(argv[1]); 

    int rank, NP;    // NP - количество процессов, rank - номер текущего
    MPI_Init(&argc, &argv);     
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    
    MPI_Barrier(MPI_COMM_WORLD);

    
    double beginTime = MPI_Wtime();
    int begin = get_block_begin(NP, JSIZE - 7, rank);
    int end = get_block_end(NP, JSIZE - 7, rank);
    
    // просто распараллеливаем внешний цикл, разбиваем его на блоки
    for (int i = 1; i < ISIZE - 1; i++)
    {
        for (int j = begin + 6; j <= end + 6; j++)
        {
            a[i][j] = sin(0.00001 * a[i + 1][j - 6]);
        }
        // нам даже не нужна барьерная синхронизация
    }
    
    // Если потребуется вывод подсчитанных значений
    // if (rank == 5)
    // {
    //     FILE *ff;
    //     ff = fopen("result.txt", "w");
    //     // ff = fopen("result.txt","w");
    //     for(int i = 0; i < ISIZE - 1; i++)
    //     {
    //         for (int j = 6 + begin; j < end + 6; j++)
    //         {
    //             fprintf(ff, "%f ", a[i][j]);
    //         }
    //         fprintf(ff, "\n");
    //     }
    //     fclose(ff);    
    // }
    
    MPI_Barrier(MPI_COMM_WORLD);
    double finishTime = MPI_Wtime();
    if (rank == 0)
        printf("%d\t%f\t%d\n", NP, finishTime - beginTime, ran); 
        // выводим количество рабочих процессоров и время работы

    MPI_Finalize();
    return 0;
}