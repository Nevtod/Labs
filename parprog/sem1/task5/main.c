#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 #include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


#define N 10 // число создаваемых потоков, помимо основного

void *mthread(void *dummy)
{
    pthread_t mthid; /* Для идентификатора нити исполнения */
    mthid = pthread_self();

    printf("Thread with ThreadID %lu finished \n", mthid);

    return NULL;
}


int main()
{
    pthread_t ownTid; // Thread id для основного потока
    pthread_t tids[N]; // для остальных потоков
    int result;


    for (int i = 0; i < N; i++)
    {
        result = pthread_create(tids + i, (pthread_attr_t *)NULL, mthread, NULL);
        if(result != 0)
        {
            printf ("Error on thread #%d create, return value = %d\n", i + 1, result);
            exit(-1);
        }
    }

    ownTid = pthread_self();

for (int i = 0; i < N; i++)
    pthread_join(*(tids + i), (void **)NULL);

printf("All threads finished. Tid Of main thread is %lu", ownTid);

return 0;
}