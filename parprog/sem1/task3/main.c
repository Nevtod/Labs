#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
// #include "mpi.h"

int main(int argc, char* argv[])
{
    int rank, procCnt;
    //  MPI_Status Status;
    MPI_Init(&argc,&argv);     
    MPI_Comm_size(MPI_COMM_WORLD,&procCnt);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Request request;

        // массив, который далее будет либо заполнен числами на первом потоке,
        // либо будет получен от предыдущего потока
    int arr[procCnt];
    if (rank == 0)
    {
        // для простоты проинициализируем исходный массив подряд идущими числами от 
        // 0 до NP - 1
        printf("Initial array is: ");
        for (int i = 0; i < procCnt; i++)
        {
            arr[i] = i;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    else
    {
        MPI_Irecv(arr, procCnt, MPI_INT,
                rank - 1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &request);
        MPI_Status status;
        MPI_Wait(&request, &status);
        // if (status. == MPI_SUCCESS)
        //     printf("SUCCESS!\n");
        // if (request == MPI_ERR_REQUEST)
        // {
        //     printf("Something goes wrong in thread %d\n", rank);
        //     exit(-1);
        // }
    }

    printf("Thread %d recieved array. Element with index %d is %d\n", rank, rank, arr[rank]);
        
    if (rank < procCnt - 1)
        MPI_Send(arr, procCnt, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
    else
        printf("Last thread with rank %d recieved array, end of the program\n", rank);

    MPI_Finalize();
}