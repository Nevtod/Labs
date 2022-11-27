/******************************************************************************
 * ЗАДАНИЕ: bugged4.c
 * ОПИСАНИЕ:
 *   Очень простая программа параллельных манипуляций с двумерным массивом
 *   зависимо от количества исполнителей... но с segmentation fault.
 ******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

double a[N][N]; // вынесли "а" со стека в кучу. На стеке 1е6 double-ов - это многовато
int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;

    #pragma omp parallel shared(nthreads) private(i, j, tid) // убрали a из private
                                                            // зачем свои копии
                                                            // на каждый поток?
    {
        tid = omp_get_thread_num();
        printf("Im here %d\n", tid);
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                a[i][j] = tid + i + j;

        printf("Thread %d done. Last element= %f\n", tid, a[N - 1][N - 1]);
    } 
}
