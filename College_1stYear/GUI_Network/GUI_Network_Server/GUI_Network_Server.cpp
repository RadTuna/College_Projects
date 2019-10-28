
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

// 윈도우 메인함수.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstatnce, LPSTR lpCmdLine, int nCmdShow){
	// 전역 인스턴스 변수에 HINSTANCE를 저장함.
	hInst = hInstance;

	// 임계영역 객체를 초기화함.
	InitializeCriticalSection(&cs);

	// 윈도우 구조체 정의.
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

	// 윈도우 구조체 등록
	if (!RegisterClass(&wndclass))
	{
		return 1;
	}

	// 윈도우 생성.
	HWND hWnd = CreateWindow(TEXT("MyWndClass"), TEXT("TCP 서버"),
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 200, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr)
	{
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 서버 스레드를 생성함.
	CreateThread(nullptr, 0, ServerMain, nullptr, 0, nullptr);

	// 윈도우 메세지 루프.
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteCriticalSection(&cs);

	return msg.wParam;
}

// 윈도우 프로시저 함수.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		// 자동스크로롤이 되고 쓰기가 불가능한 자식창을 생성함.
		// 이 창에는 기존의 콘솔에 기록되던 메세지가 기록됨.
		// WS_VISIBLE을 작성하지 않아, 내용이 화면에 출력되지 않는 버그가 있었음.
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

// printf와 같이 가변인자를 받아 edit창에 입력하는 함수.
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

// 애플리케이션을 종료하면서 메세지박스로 에러를 출력하는 함수.
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

// 애플리케이션을 종료하지 않고 edit창에 에러를 출력하는 함수.
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

// 서버 스레드에서 호출되는 서버메인함수.
DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	// 윈도우 소켓을 초기화함.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// 서버 소켓을 생성함.
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit(TEXT("socket()"));
	}

	// 서버 주소 디스크립터를 작성함.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// 서버 소켓에 서버 주소 디스크립터를 바인딩함.
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit(TEXT("bind()"));
	}

	// 서버 소켓을 리슨상태로 변경함.
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

		// 클라이언트에서 서버로의 접속이 발생할 때 까지 대기함.
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display(TEXT("accept()"));
			break;
		}

		// 접속한 클라이언트의 정보를 edit창에 출력함.
		DisplayText(TEXT("\r\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트번호 = %d\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 메세지 수신을 처리할 클라이언트 메세지처리 스레드를 생성함.
		// 따라서, 많은 클라이언트의 접속을 동시에 처리할 수 있음.
		hThread = CreateThread(nullptr, 0, ProcessClient, (LPVOID)client_sock, 0, nullptr);
		if (hThread == nullptr)
		{
			closesocket(client_sock); // 스레드 생성에 실패하면 그대로 연결을 종료함.
		}
		else
		{
			// 스레드 생성에 성공하면 핸들을 해제한다. 단, 스레드는 종료되지 않는다.
			// 그러면 의문. 왜 굳이 스레드 핸들을 받아야 했는가?
			CloseHandle(hThread);
		}
	}
	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

// 클라이언트 메세지처리 스레드에서 호출되는 함수.
// 이 함수를 통해 클라이언트 연결을 다중으로 처리할 수 있음.
DWORD WINAPI ProcessClient(LPVOID arg)
{
	// 클라이언트 소켓 정보를 보이드 포인터로 받음.
	SOCKET client_sock = SOCKET(arg);
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	addrlen = sizeof(clientaddr);

	// 클라이언트 소켓 주소 디스크립터를 가져오지 못했으므로 getpeeranem함수를 이용해 소켓 주소를 획득함.
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (true)
	{
		// 클라이언트에게 고정길이의 데이터를 받는다.
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("recv()"));
			break;
		}
		else if (retval == 0) // 수신받은 값의 길이가 0이면 연결종료.
		{
			break;
		}

		buf[retval] = '\0';

		// 연결된 클라이언트의 주소, 포트번호, 데이터를 출력한다.
		DisplayText(TEXT("[TCP/%s:%d] %s\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		// 입력받은 데이터를 그대로 클라이언트에 재전송한다.
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("send()"));
			break;
		}
	}

	// 연결이 정상적으로 종료되면 클라이언트 소켓을 해제하고, 해제 메세지를 출력한다.
	closesocket(client_sock);

	// ucrtbased.dll에 대한 메모리 접근 오류가 vsprintf에서 발생하여 해당 코드문제라고 생각했으나,
	// 포트번호가 정수인데 %s로 작성하여 발생한 오류였음. %d로 수정함.
	DisplayText(TEXT("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\r\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}


