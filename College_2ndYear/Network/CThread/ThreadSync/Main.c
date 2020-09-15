#include <windows.h>
#include <stdio.h>

#define MAX_CNT 100000000
int g_count = 0;

CRITICAL_SECTION cs;

DWORD WINAPI MyThread1(LPVOID arg)
{
    for (int i = 0; i < MAX_CNT; ++i)
    {
        EnterCriticalSection(&cs);
        g_count += 2;
        LeaveCriticalSection(&cs);
    }
}

DWORD WINAPI MyThread2(LPVOID arg)
{
    for (int i = 0; i < MAX_CNT; ++i)
    {
        EnterCriticalSection(&cs);
        g_count -= 2;
        //LeaveCriticalSection(&cs);
    }

}

int main(int argc, char* argv[])
{
    // 임계영역 초기화.
    InitializeCriticalSection(&cs);

    // 두개의 스레드 생성.
    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

    // 두개의 스레드 종료 대기.
    WaitForMultipleObjects(2, hThreads, FALSE, INFINITE);

    // 임계영역 삭제.
    DeleteCriticalSection(&cs);
    
    // 결과출력
    printf("g_count = %d\n", g_count);
    return 0;
}

