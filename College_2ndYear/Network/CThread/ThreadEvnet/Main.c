#include <windows.h>
#include <stdio.h>

#define BUF_SIZE 10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int g_buf[BUF_SIZE];

// Write thread
DWORD WINAPI WriteThread(LPVOID arg)
{
    DWORD retval;

    for (int i = 1; i <= 20; ++i)
    {
        // 읽기 완료 대기
        retval = WaitForSingleObject(hReadEvent, INFINITE);
        if (retval != WAIT_OBJECT_0)
        {
            return retval;
        }

        // 공유 버퍼에 데이터 저장.
        for (int k = 0; k < BUF_SIZE; ++k)
        {
            g_buf[k] = i;
        }

        SetEvent(hWriteEvent);
    }

    WaitForSingleObject(hReadEvent, INFINITE);

    return 0;
}

// Read thread
DWORD WINAPI ReadThread(LPVOID arg)
{
    DWORD retval;
    while(1)
    {
        // 쓰기 완료 대기.
        retval = WaitForSingleObject(hWriteEvent, INFINITE);
        if (retval != WAIT_OBJECT_0)
        {
            break;
        }

        // 읽은 데이터 출력.
        printf("Thread %4d : ", GetCurrentThreadId());
        for(int i = 0; i < BUF_SIZE; ++i)
        {
            printf("%3d / ", g_buf[i]);
        }
        printf("\n");

        // 버퍼 초기화.
        ZeroMemory(g_buf, BUF_SIZE);

        // 읽기 완료 알림.
        SetEvent(hReadEvent);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    // 두개의 자동 리셋 이벤트 생성 (각각 비신호, 신호)
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hWriteEvent == NULL)
    {
        return 1;
    }

    hReadEvent= CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hReadEvent == NULL)
    {
        return 1;
    }

    // 세개의 스레드 생성.
    HANDLE hThreads[3];
    hThreads[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
    hThreads[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

    // 세 개의 스레드 종료대기.
    // WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
    WaitForSingleObject(hThreads[0], INFINITE);

    // 이벤트 제거.
    CloseHandle(hWriteEvent);
    CloseHandle(hReadEvent);

    return 0;
}

