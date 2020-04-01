
#include <Windows.h>
#include <stdio.h>

#define BUFSIZE 10

// 2���� �ڵ� �����͸� ������. (void*)
HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

// Write ������ �Լ�. // ���̵� ������ �Ű������� ������� ����.
// ���۸� �ݺ��� k������ ä��� ����� ������.
DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 0; k <= 500; ++k)
	{
		// ��ȣ�� Set�ɶ� ���� �����.
		// hReadEvent�� Auto�̹Ƿ� �ش� ������ ����� �ٽ� Reset��.
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) // ��ȣ�� Set�Ǽ� �������� �ƴҰ�� �Լ��� ������.
		{
			break;
		}

		// k�� ������ ���۸� ���� ä��.
		for (int i = 0; i < BUFSIZE; ++i)
		{
			buf[i] = k;
		}

		// hWriteEvent�� ��ȣ�� Set���� ������.
		SetEvent(hWriteEvent);
	}

	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 0; k <= 500; ++k)
	{
		// ��ȣ�� Set�ɶ� ���� �����.
		// hWriteEvent�� Auto�̹Ƿ� �ش� ������ ����� �ٽ� Reset��.
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) // ��ȣ�� Set�Ǽ� �������� �ƴҰ�� �Լ��� ������.
		{
			break;
		}

		// k�� ������ ���۸� ���� ä��.
		for (int i = 0; i < BUFSIZE; ++i)
		{
			printf("%3d", buf[i]);
		}
		printf("\n");

		// hReadEvent�� ��ȣ�� Set���� ������.
		SetEvent(hReadEvent);
	}

	return 0;
}

// Main �Լ�.
int main(int argc, char* argv[])
{
	/*
	@ function : CreateEvent
	@ param lpEventAttributes : �ڽ� ���μ����� ����ǰ� ��ȯ�� �ڵ��� ����� �� �ִ��� Ȯ���ϴ� ���Ⱥ���
	@ param bManualReset : true�̸�, ResetEvent�Լ��� ����Ͽ� ��ȣ�� Reset�� ����� ��. false�̸�, �������� ��� ���°� �����Ǵ� ���� Reset���� ��.
	@ param bInitialState : true�̸�, �̺�Ʈ�� ��ȣ �ʱ���°� Set. false�̸� Reset.
	@ param lpName : �̺�Ʈ ��ü�� �̸�.
	@ return : �̺�Ʈ ��ü�� �ڵ��� ��ȯ��. �Լ��� �����ϸ� nullptr�� ��ȯ.
	*/
	// WriteEvent��ü�� ������.
	// �ڵ� - ��ȣ �ʱⰪ�� Reset��.
	hWriteEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hWriteEvent == nullptr) // ������ ����� ���� �ʾ����� �����Լ��� ������.
	{
		return 1;
	}

	// ReadEvent��ü�� ������.
	// �ڵ� - ��ȣ �ʱⰪ�� Set��.
	hReadEvent = CreateEvent(nullptr, false, true, nullptr);
	if (hReadEvent == nullptr) // ������ ����� ���� �ʾ����� �����Լ��� ������.
	{
		return 1;
	}

	//������ �ڵ��� ������.
	HANDLE hThread[3];

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
	// �����带 ������. ������ �Լ��� Write �������Լ��� ���ε�.
	hThread[0] = CreateThread(nullptr, 0, WriteThread, nullptr, 0, nullptr);

	// �����带 ������. ������ �Լ��� Read �������Լ��� ���ε�.
	hThread[1] = CreateThread(nullptr, 0, ReadThread, nullptr, 0, nullptr);

	// �����带 ������. ������ �Լ��� Read �������Լ��� ���ε�.
	hThread[2] = CreateThread(nullptr, 0, ReadThread, nullptr, 0, nullptr);

	/*
	@ function : WaitForMultipleObjects
	@ param nCount : �ڵ�迭�� ���̸� �Է��ϴ� ����. �ش� ���ڴ� 0�� �� �� ����.
	@ param lpHandles : �ڵ�迭�� �Է��ϴ� ����.
	@ param bWaitAll : true���, �ڵ�迭�� ��� �ڵ��� ��ȣ�� Set�ɶ� return, false���, �ڵ�迭 �� 1���� �ڵ��� ��ȣ�� Set�Ǹ� return
	@ param dwMilliseconds : ��ȣ�� Set���� �ʴ��� �ش� ������ �Էµ� �ð��� ������ return
	@ return : �ش� �Լ��� ���� �� �̺�Ʈ�� ��Ÿ�� (WAIT_OBJECT_0, WAIT_ABANDONED_0 ��)
	*/
	// 3���� �����尡 ����ɶ����� Main ������� ���� �������� �����.
	// ���ð��� ����.
	WaitForMultipleObjects(3, hThread, true, INFINITE);

	// 2���� �̺�Ʈ ��ü �ڵ��� ������.
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}
