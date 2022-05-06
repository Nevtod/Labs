#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
#include "math.h"
// #include "mpi.h"

int N; // количество узлов сетки вдоль оси
int run; // номер запуска
double X = 1.0, T = 1.0;
double h;   // шаг по оси х
double tau; // шаг по оси t

double **u = NULL;

int atU(int m, int begin)
{
    return m - begin + 1;
}

double phi(double x)
{
    return exp(x);
}

double psi(double t)
{
    return cos(t);
}

double f(double t, double x)
{
    return exp(x * x * sin(t));
}

double calcNode(int m, int n, int begin)
{
    if (m == 0)
        return psi(n * tau);
    else if (n == 0)
        return phi(m * h);
    else
        return u[atU(m, begin)][n - 1] + 
            tau / h * (u[atU(m - 1, begin)][n - 1] - u[atU(m, begin)][n - 1]) +
            tau * f(m * h, (n - 1) * tau);
}

int main(int argc, char* argv[])
{
    N = atoi(argv[1]);
    run = atoi(argv[2]);

    tau = (T / (N - 1.0)); 
    h = (X / (N - 1.0));   

    int rank, NP;    // NP - количество процессов, rank - номер текущего
    MPI_Init(&argc, &argv);     
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    
    MPI_Barrier(MPI_COMM_WORLD);
    double beginTime = MPI_Wtime();

    int blockSize = N / NP;
    int remainder = N % NP;
    int begin = blockSize * rank + ((rank < remainder) ? rank : remainder);
    int end = begin + blockSize - (rank >= remainder ? 1 : 0);
    
    // храним только необходимые нам значения для вычислений
    u = (double**)malloc((end - begin + 2) * sizeof(double*));
    if (u == NULL)
    {
        printf("You have no enough memory\n");
        return -1;
    }
    for (int i = 0; i < end - begin + 2; i++)
    {
        u[i] = (double*)malloc(sizeof(double) * N);
        if (u[i] == NULL)
        {
            printf("You have no enough memory\n");
            return -1;
        }
    }
    MPI_Request requestSend, requestRecv;
    MPI_Status status;
    for(int n = 0; n < N; n++)       // n - индекс для узла по t
        for (int m = end; m >= begin; m--) // m - индекс для узла по х
        {
            if (m == 0 || n == 0)
            {
                u[atU(m, begin)][n] = calcNode(m, n, begin);
            }
            else
            {
                if (m == begin)
                {
                    MPI_Irecv(&u[atU(m - 1, begin)][n - 1], 1, MPI_DOUBLE,
                            rank - 1, n - 1, MPI_COMM_WORLD, &requestRecv);
                    MPI_Wait(&requestRecv, MPI_STATUS_IGNORE);
                }

                u[atU(m, begin)][n] = calcNode(m, n, begin);
            }

            if (m == end && rank != NP - 1)
                MPI_Isend(&u[atU(m, begin)][n], 1, MPI_DOUBLE,
                        rank + 1, n, MPI_COMM_WORLD,
                        &requestSend);
        }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        printf("run = %d N = %d NP = %d Time = %f\n", run, N, NP, MPI_Wtime() - beginTime);

    // if (rank == 0)
    // for (int i = 0; i < N; i++)
    // {
    //     printf("\n");
    //     for (int j = 1; j < end - begin + 2; j++)
    //         printf("%f ", u[j][i]);
    // }
    MPI_Finalize();
}