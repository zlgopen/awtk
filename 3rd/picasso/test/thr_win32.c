

#include <windows.h>
#include "picasso.h"


extern void thread_func1(void* data);
extern void thread_func2(void* data);

int thr_run1;
int thr_run2;

DWORD WINAPI win32_thread_1(void *p1)
{

    thread_func1(p1);
    return 0;
}

DWORD WINAPI win32_thread_2(void *p1)
{

    thread_func2(p1);
    return 0;
}

static HANDLE th1, th2;

void start_worker_threads(void* cs)
{
    thr_run1 = 1;
    thr_run2 = 1;
    th1 = CreateThread(0, 0, win32_thread_1, cs, 0, 0);
    th2 = CreateThread(0, 0, win32_thread_2, cs, 0, 0);
}

void stop_worker_threads(void)
{
    thr_run1 = 0;
    thr_run2 = 0;
    TerminateThread(th1, 0);
    TerminateThread(th2, 0);
}

void loop_wait(int ms)
{
    Sleep(ms);
}
