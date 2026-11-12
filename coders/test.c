#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int data = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long get_time_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L);
}

void *thread_function(void *arg)
{
    int i = 0;

    pthread_mutex_lock(&mutex);
    while (i < 1000000)
    {
        data++;
        i++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;

    long start = get_time_ms();

    printf("[%ld ms] Thread started\n", get_time_ms() - start);

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("[%ld ms] Threads finished! Data: %d\n", get_time_ms() - start, data);
    
    pthread_mutex_destroy(&mutex);
    return 0;   
}