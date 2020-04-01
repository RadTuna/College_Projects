
#include <Windows.h>
#include <stdio.h>

#define MAXCNT 1000000

int g_count = 0;

CRITICAL_SECTION cs;

// 1�� ������ �Լ�. ���̵� ������ �Ű������� ������� ����.
DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i)
	{
		// �Ӱ迵���� ��.
		// ����, ���� ������ �̿��� �ٸ� �����忡�� EnterCiriticalSection ������ �����ϸ� Lock.
		EnterCriticalSection(&cs);
		g_count += 2;

		// �Ӱ迵���� ��������.
		// ����, �ٸ� �������� EnterCiriticalSection �������� Lock�ɸ� ���� ������.
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// 2�� ������ �Լ�. ���̵� ������ �Ű������� ������� ����.
DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i)
	{
		// �Ӱ迵���� ��.
		// ����, ���� ������ �̿��� �ٸ� �����忡�� EnterCiriticalSection ������ �����ϸ� Lock.
		EnterCriticalSection(&cs);
		g_count -= 2;

		// �Ӱ迵���� ��������.
		// ����, �ٸ� �������� EnterCiriticalSection �������� Lock�ɸ� ���� ������.
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// Main�Լ�
int main(int argc, char* argv[])
{
	// �Ӱ迵�� ��ü�� �ʱ�ȭ��.
	InitializeCriticalSection(&cs);

	// ������ �ڵ��� ������.
	HANDLE hThread[2];

	/*
	@ function : CreateThread
	@ param SecurityAttributes : �ڽ� ���μ����� ����ǰ� ��ȯ�� �ڵ��� ����� �� �ִ��� Ȯ���ϴ� ���Ⱥ���
	@ param StackSize : �ʱ� ����ũ�⸦ �����ϴ� ����, Byte
	@ param StartFunction : �����忡 ���� ����Ǵ� �Լ��� �Լ������͸� �޴� ����
	@ param ThreadParameter : ������� ���޵Ǵ� �ϳ��� ����
	@ param CreationFlags : �����尡 �����Ǵ� ���� �����ϴ� �÷���
	@ param ThreadId : �����带 ������ �� �ִ� �ĺ��ڸ� ��ȯ�ϴ� ���� (Out)
	@ return : �Լ��� ���������� �����ߴٸ�, ���Ӱ� ������ �������� �ڵ��� ��ȯ��.
	*/
	// �����带 �����ϰ�, ������ �Լ��� 1�� �Լ��� ������.
	hThread[0] = CreateThread(nullptr, 0, MyThread1, nullptr, 0, nullptr);

	// �����带 �����ϰ�, ������ �Լ��� 2�� �Լ��� ������.
	hThread[1] = CreateThread(nullptr, 0, MyThread2, nullptr, 0, nullptr);

	/*
	@ function : WaitForMultipleObjects
	@ param nCount : �ڵ�迭�� ���̸� �Է��ϴ� ����. �ش� ���ڴ� 0�� �� �� ����.
	@ param lpHandles : �ڵ�迭�� �Է��ϴ� ����.
	@ param bWaitAll : true���, �ڵ�迭�� ��� �ڵ��� ��ȣ�� Set�ɶ� return, false���, �ڵ�迭 �� 1���� �ڵ��� ��ȣ�� Set�Ǹ� return
	@ param dwMilliseconds : ��ȣ�� Set���� �ʴ��� �ش� ������ �Էµ� �ð��� ������ return
	@ return : �ش� �Լ��� ���� �� �̺�Ʈ�� ��Ÿ�� (WAIT_OBJECT_0, WAIT_ABANDONED_0 ��)
	*/
	// 2���� ������ �Լ��� ����� �� ���� Main������� ���� �������� �����.
	// ���ð��� ����.
	WaitForMultipleObjects(2, hThread, true, INFINITE);

	// �Ӱ迵�� ��ü�� ������.
	DeleteCriticalSection(&cs);

	// 2���� �������Լ��� ���� ������ �̷���� g_count ���������� �����.
	printf("g_count = %d\n", g_count);
	return 0;
}
