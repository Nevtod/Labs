#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>
#include <pthread.h>



#define N 20 // число подсчитывающих сумму потоков.  Основной поток подсчёт не ведёт
#define M 79 // размер массива для подсчёта

pthread_t tids[N]; // для остальных потоков
int array[M] = {0, };  // исходные данные
int sums[N] = {0, };   // сюда складываются подсчитанные суммы


pthread_t ownTid; // Thread id для основного потока
    char keyfilename[] = "mysemaforkey.ipc";
    key_t key;
    int semid;

void do_semop(int semid, int op)
{
    struct sembuf msembuf;
    msembuf.sem_op = op;
    msembuf.sem_flg = 0;
    msembuf.sem_num = 0;

    if (semop(semid, &msembuf, 1) < 0)
    {
        printf("Can\'t perform semafor %d operation\n", op);
        exit(-1);
    }
}

void *mthread(void* _numberOfThread)
{
    int rank = *((int*)(_numberOfThread));
    do_semop(semid, 1);    //перезаписали в локальную переменную из указателя,
                           // можно освобождать доступ

    int blockSize = M / N;
    int remainder = M % N;
    int begin = blockSize * rank 
            + ((rank < remainder) ? rank : remainder);
    int end = begin + blockSize - (rank >= remainder ? 1 : 0);

    int sum = 0;
    for (int i = begin; i <= end; i++)
        sum += array[i];

    sums[rank] = sum;

    printf("Thread %d finished, its' sum is %d\n", rank + 1, sum);
    return NULL;
}

int main(int argc)
{
    //инициализация семафора
    if ((key = ftok(keyfilename, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    } 
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semafor id\n");
        exit(-1);
    } 

    // инициализация массива из M элементов подряд идущими числами
    printf("start array is: ");
    for (int i = 0; i < M; i++)
    {
        array[i] = i;
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < N; i++)
    {
        int result = pthread_create(tids + i, (pthread_attr_t *)NULL, mthread, &i);
        if(result != 0)
        {
            printf ("Error on thread #%d create, return value = %d\n", i + 1, result);
            exit(-1);
        }
        do_semop(semid, -1); // ждем, когда поток инициализирует локальную копию i
    }

for (int i = 0; i < N; i++)
    pthread_join(*(tids + i), (void **)NULL);

printf("All threads finished.\n");

int resultSum = 0;
for (int i = 0; i < N; i++)
    resultSum += sums[i];

printf("Result sum is %d.\n", resultSum);
return 0;
}