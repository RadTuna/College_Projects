
#include <Windows.h>
#include <stdio.h>

#define MAXCNT 1000000

int g_count = 0;

CRITICAL_SECTION cs;

// 1번 스레드 함수. 보이드 포인터 매개변수는 사용하지 않음.
DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i)
	{
		// 임계영역에 들어감.
		// 따라서, 현재 스레드 이외의 다른 스레드에서 EnterCiriticalSection 구문에 접근하면 Lock.
		EnterCriticalSection(&cs);
		g_count += 2;

		// 임계영역을 빠져나감.
		// 따라서, 다른 스레드의 EnterCiriticalSection 구문에서 Lock걸린 것을 해제함.
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// 2번 스레드 함수. 보이드 포인터 매개변수는 사용하지 않음.
DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i)
	{
		// 임계영역에 들어감.
		// 따라서, 현재 스레드 이외의 다른 스레드에서 EnterCiriticalSection 구문에 접근하면 Lock.
		EnterCriticalSection(&cs);
		g_count -= 2;

		// 임계영역을 빠져나감.
		// 따라서, 다른 스레드의 EnterCiriticalSection 구문에서 Lock걸린 것을 해제함.
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// Main함수
int main(int argc, char* argv[])
{
	// 임계영역 객체를 초기화함.
	InitializeCriticalSection(&cs);

	// 스레드 핸들을 선언함.
	HANDLE hThread[2];

	/*
	@ function : CreateThread
	@ param SecurityAttributes : 자식 프로세스가 종료되고 반환된 핸들을 상속할 수 있는지 확인하는 보안변수
	@ param StackSize : 초기 스택크기를 결정하는 변수, Byte
	@ param StartFunction : 스레드에 의해 실행되는 함수의 함수포인터를 받는 변수
	@ param ThreadParameter : 스레드로 전달되는 하나의 변수
	@ param CreationFlags : 스레드가 생성되는 것을 제어하는 플래그
	@ param ThreadId : 스레드를 구분할 수 있는 식별자를 반환하는 변수 (Out)
	@ return : 함수가 정상적으로 동작했다면, 새롭게 생성한 스레드의 핸들을 반환함.
	*/
	// 스레드를 생성하고, 스레드 함수로 1번 함수를 전달함.
	hThread[0] = CreateThread(nullptr, 0, MyThread1, nullptr, 0, nullptr);

	// 스레드를 생성하고, 스레드 함수로 2번 함수를 전달함.
	hThread[1] = CreateThread(nullptr, 0, MyThread2, nullptr, 0, nullptr);

	/*
	@ function : WaitForMultipleObjects
	@ param nCount : 핸들배열의 길이를 입력하는 변수. 해당 인자는 0이 될 수 없음.
	@ param lpHandles : 핸들배열을 입력하는 변수.
	@ param bWaitAll : true라면, 핸들배열의 모든 핸들의 신호가 Set될때 return, false라면, 핸들배열 중 1개의 핸들의 신호가 Set되면 return
	@ param dwMilliseconds : 신호가 Set되지 않더라도 해당 변수에 입력된 시간이 지나면 return
	@ return : 해당 함수가 리턴 된 이벤트를 나타냄 (WAIT_OBJECT_0, WAIT_ABANDONED_0 등)
	*/
	// 2개의 스레드 함수가 종료될 때 까지 Main스레드는 현재 구문에서 대기함.
	// 대기시간은 무한.
	WaitForMultipleObjects(2, hThread, true, INFINITE);

	// 임계영역 객체를 해제함.
	DeleteCriticalSection(&cs);

	// 2개의 스레드함수에 의해 증감이 이루어진 g_count 전역변수를 출력함.
	printf("g_count = %d\n", g_count);
	return 0;
}
