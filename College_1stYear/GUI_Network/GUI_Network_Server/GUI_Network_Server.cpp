
#pragma comment(lib, "ws2_32")

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 3317
#define BUFSIZE 512

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DisplayText(char* fmt, ...);

void err_quit(char* msg);
void err_display(char* msg);

DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

HINSTANCE hInst;
HWND hEdit;
CRITICAL_SECTION cs;

// ������ �����Լ�.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstatnce, LPSTR lpCmdLine, int nCmdShow){
	// ���� �ν��Ͻ� ������ HINSTANCE�� ������.
	hInst = hInstance;

	// �Ӱ迵�� ��ü�� �ʱ�ȭ��.
	InitializeCriticalSection(&cs);

	// ������ ����ü ����.
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = TEXT("MyWndClass");

	// ������ ����ü ���
	if (!RegisterClass(&wndclass))
	{
		return 1;
	}

	// ������ ����.
	HWND hWnd = CreateWindow(TEXT("MyWndClass"), TEXT("TCP ����"),
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 200, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr)
	{
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���� �����带 ������.
	CreateThread(nullptr, 0, ServerMain, nullptr, 0, nullptr);

	// ������ �޼��� ����.
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteCriticalSection(&cs);

	return msg.wParam;
}

// ������ ���ν��� �Լ�.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		// �ڵ���ũ�η��� �ǰ� ���Ⱑ �Ұ����� �ڽ�â�� ������.
		// �� â���� ������ �ֿܼ� ��ϵǴ� �޼����� ��ϵ�.
		// WS_VISIBLE�� �ۼ����� �ʾ�, ������ ȭ�鿡 ��µ��� �ʴ� ���װ� �־���.
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, nullptr);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), true);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// printf�� ���� �������ڸ� �޾� editâ�� �Է��ϴ� �Լ�.
void DisplayText(char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, false, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);

	va_end(arg);
}

// ���ø����̼��� �����ϸ鼭 �޼����ڽ��� ������ ����ϴ� �Լ�.
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr);
	MessageBox(nullptr, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���ø����̼��� �������� �ʰ� editâ�� ������ ����ϴ� �Լ�.
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr);
	DisplayText(TEXT("[%s] %s"), msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ���� �����忡�� ȣ��Ǵ� ���������Լ�.
DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	// ������ ������ �ʱ�ȭ��.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// ���� ������ ������.
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit(TEXT("socket()"));
	}

	// ���� �ּ� ��ũ���͸� �ۼ���.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// ���� ���Ͽ� ���� �ּ� ��ũ���͸� ���ε���.
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit(TEXT("bind()"));
	}

	// ���� ������ �������·� ������.
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit(TEXT("listen()"));
	}

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	while (true)
	{
		addrlen = sizeof(clientaddr);

		// Ŭ���̾�Ʈ���� �������� ������ �߻��� �� ���� �����.
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display(TEXT("accept()"));
			break;
		}

		// ������ Ŭ���̾�Ʈ�� ������ editâ�� �����.
		DisplayText(TEXT("\r\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ = %d\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// �޼��� ������ ó���� Ŭ���̾�Ʈ �޼���ó�� �����带 ������.
		// ����, ���� Ŭ���̾�Ʈ�� ������ ���ÿ� ó���� �� ����.
		hThread = CreateThread(nullptr, 0, ProcessClient, (LPVOID)client_sock, 0, nullptr);
		if (hThread == nullptr)
		{
			closesocket(client_sock); // ������ ������ �����ϸ� �״�� ������ ������.
		}
		else
		{
			// ������ ������ �����ϸ� �ڵ��� �����Ѵ�. ��, ������� ������� �ʴ´�.
			// �׷��� �ǹ�. �� ���� ������ �ڵ��� �޾ƾ� �ߴ°�?
			CloseHandle(hThread);
		}
	}
	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

// Ŭ���̾�Ʈ �޼���ó�� �����忡�� ȣ��Ǵ� �Լ�.
// �� �Լ��� ���� Ŭ���̾�Ʈ ������ �������� ó���� �� ����.
DWORD WINAPI ProcessClient(LPVOID arg)
{
	// Ŭ���̾�Ʈ ���� ������ ���̵� �����ͷ� ����.
	SOCKET client_sock = SOCKET(arg);
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	addrlen = sizeof(clientaddr);

	// Ŭ���̾�Ʈ ���� �ּ� ��ũ���͸� �������� �������Ƿ� getpeeranem�Լ��� �̿��� ���� �ּҸ� ȹ����.
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (true)
	{
		// Ŭ���̾�Ʈ���� ���������� �����͸� �޴´�.
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("recv()"));
			break;
		}
		else if (retval == 0) // ���Ź��� ���� ���̰� 0�̸� ��������.
		{
			break;
		}

		buf[retval] = '\0';

		// ����� Ŭ���̾�Ʈ�� �ּ�, ��Ʈ��ȣ, �����͸� ����Ѵ�.
		DisplayText(TEXT("[TCP/%s:%d] %s\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		// �Է¹��� �����͸� �״�� Ŭ���̾�Ʈ�� �������Ѵ�.
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("send()"));
			break;
		}
	}

	// ������ ���������� ����Ǹ� Ŭ���̾�Ʈ ������ �����ϰ�, ���� �޼����� ����Ѵ�.
	closesocket(client_sock);

	// ucrtbased.dll�� ���� �޸� ���� ������ vsprintf���� �߻��Ͽ� �ش� �ڵ幮����� ����������,
	// ��Ʈ��ȣ�� �����ε� %s�� �ۼ��Ͽ� �߻��� ��������. %d�� ������.
	DisplayText(TEXT("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ = %d\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}


