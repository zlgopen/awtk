

#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "picasso.h"


extern void thread_func1(void* data);
extern void thread_func2(void* data);

int thr_run1;
int thr_run2;

void* linux_thread_1(void *p1)
{

    thread_func1(p1);
    return NULL;
}

void* linux_thread_2(void *p1)
{

    thread_func2(p1);
    return NULL;
}

static pthread_t th1, th2;

void start_worker_threads(void* cs)
{
    thr_run1 = 1;
    thr_run2 = 1;
    pthread_create(&th1, NULL, linux_thread_1, cs);
    pthread_create(&th2, NULL, linux_thread_2, cs);
}

void stop_worker_threads(void)
{
    thr_run1 = 0;
    thr_run2 = 0;
    pthread_join(th1, 0);
    pthread_join(th2, 0);
}

void loop_wait(int ms)
{
    usleep(ms*1000);
}
