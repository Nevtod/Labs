#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>
#include <pthread.h>
#include <time.h>

#define h 0.01 // шаг интегрирования


double leftXBorder = 0;
double rightXBorder = 2;
int N; // число подсчитывающих интеграл потоков. Основной поток подсчёт не ведёт
double* integrals;   // подсчитанные части
pthread_t* tids;     // идентификаторы потоков

char keyfilename[] = "mysemaforkey7.ipc";
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
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);
    
    int rank = *((int*)(_numberOfThread));
    do_semop(semid, 1);    //перезаписали в локальную переменную из указателя,
                           // можно освобождать доступ

    double blockLen = (rightXBorder - leftXBorder) / N;
    double leftX = leftXBorder + blockLen * rank;
    double rightX = leftX + blockLen;

    double curX = leftX;
    double integral = 0;
    while (curX < rightX)
    {
        integral += (curX + curX + h) / 2 * h;
        curX += h;
    }

    integrals[rank] = integral;
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = end.tv_sec - begin.tv_sec 
                + 1e-9 * (end.tv_nsec - begin.tv_nsec);
    printf("Thread %d spent %f seconds\n", rank + 1, elapsed);
    return NULL;
}

int main(int argc, char* argv[])
{
    N = atoi(argv[1]);
    integrals = (double*)malloc(sizeof(double) * N);
    tids = (pthread_t*)malloc(sizeof(pthread_t) * N);

    printf("N = %d - number of computing threads\n", N);

    //запускаем таймер
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

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

double resultIntegral = 0;
for (int i = 0; i < N; i++)
    resultIntegral += integrals[i];

clock_gettime(CLOCK_REALTIME, &end);
double elapsed = end.tv_sec - begin.tv_sec 
                + 1e-9 * (end.tv_nsec - begin.tv_nsec);

printf("Result integral is %f.\n", resultIntegral);
printf("Summary spent time is %f seconds.\n", elapsed);


free(integrals);
free(tids);
return 0;
}