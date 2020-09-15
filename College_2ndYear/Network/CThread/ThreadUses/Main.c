#include <windows.h>
#include <stdio.h>

int sum = 0;

DWORD WINAPI MyThread(LPVOID arg)
{
    int num = (int)arg;
    for (int i = 0; i <= num; ++i)
    {
        sum += i;
        Sleep(1);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    int num = 100;
    HANDLE hThread = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);
    if (hThread == NULL)
    {
        return 1;
    }

    printf("Before running thread. Calc result = %d\n", sum);
    ResumeThread(hThread);

    WaitForSingleObject(hThread, 10);

    printf("After running thread. Calc result = %d\n", sum);
    CloseHandle(hThread);

    return 0;
}

