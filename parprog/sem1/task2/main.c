#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
// #include "mpi.h"

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int get_sum(int start, int end)
{
    int sum = 0;
    for (int i = start; i <= end; i++)
        sum += i;
    return sum;
}

int main(int argc, char* argv[])
{
    int rank, procCnt;
    int N = 10;
    //  MPI_Status Status;
    MPI_Init(&argc,&argv);     
    MPI_Comm_size(MPI_COMM_WORLD,&procCnt);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);    
    MPI_Barrier(MPI_COMM_WORLD);

    int sum = 0;    

    // find own range
    int numsPerProc = max(1, N / procCnt + (N % procCnt == 0 ? 0 : 1));
    int start = 1 + numsPerProc * rank;
    int end = min(start + numsPerProc - 1, N);

    //calc sum
    sum = get_sum(start, end);
    MPI_Status status;
    if (rank == 0)
    {
        int blocksRecieved = 0;
        while (blocksRecieved < procCnt - 1)
        {
            int recievedSum = 0;
            MPI_Recv(&recievedSum, 1, MPI_INT,
                    MPI_ANY_SOURCE,
                    MPI_ANY_TAG,
                    MPI_COMM_WORLD,
                    &status);
            
            sum += recievedSum;
            blocksRecieved++;
            // printf("Total blocks recieved: %d\n", blocksRecieved);    
        }

        printf("Total sum: %d", sum);
    }
    else MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    MPI_Finalize();
} 