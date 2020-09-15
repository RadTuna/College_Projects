#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread(LPVOID arg)
{
    while(1); // 무한 루프
    return 0;
}

int main()
{
    // CPU 개수를 알아낸다.
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // CPU 개수만큼 스레드를 생성한다.
    for (int i = 0; i < (int)si.dwNumberOfProcessors; ++i)
    {
        HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
        if (hThread == NULL)
        {
            return 1;
        }

        SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
        CloseHandle(hThread);
    }

    Sleep(1000);
    while(1)
    {
        printf("Running main thread!\n");
        break;
    }

    return 0;
}
