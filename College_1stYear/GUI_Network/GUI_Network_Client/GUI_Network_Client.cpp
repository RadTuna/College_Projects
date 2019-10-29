
#pragma comment(lib, "ws2_32")

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE 512

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DisplayText(char* fmt, ...);

void err_quit(char* msg);
void err_display(char* msg);

// 커스텀 메세지 처리함수.
int recvn(SOCKET s, char* buf, int len, int flags);

DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock;
char buf[BUFSIZE + 1];
HANDLE hReadEvent, hWriteEvent;
HWND hSendButton;
HWND hEdit1, hEdit2;

// 윈도우 메인함수.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstatnce, LPSTR lpCmdLine, int nCmdShow)
{
	// Read이벤트를 생성함. 자동에 초기값 true
	hReadEvent = CreateEvent(nullptr, false, true, nullptr);
	if (hReadEvent == nullptr)
	{
		return 1;
	}

	// Write이벤트를 생성함. 자동에 초기값 false
	hWriteEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hWriteEvent == nullptr)
	{
		return 1;
	}

	// 클라이언트 스레드를 생성함.
	// 서버로의 메세지 송신을 처리하는 스레드.
	CreateThread(nullptr, 0, ClientMain, nullptr, 0, nullptr);

	// 다이얼로그 창을 출력함.
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

	// 이벤트 핸들을 해제함.
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	// 클라이언트 소켓 해제.
	closesocket(sock);

	// 윈도우 소켓 해제.
	WSACleanup();

	return 0;
}

// 다이얼로그 프로시저 함수.
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
		hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
		hSendButton = GetDlgItem(hDlg, IDOK);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EnableWindow(hSendButton, false); // 전송버튼을 비활성화.

			// hReadEvent가 signal이 될때까지(버퍼입력이 없었거나, 전송 및 수신이 끝난경우) 무한히 대기함.
			WaitForSingleObject(hReadEvent, INFINITE);

			// 문자열 버퍼에 edit컨트롤의 문자열을 저장함.
			GetDlgItemText(hDlg, IDC_EDIT1, buf, BUFSIZE + 1);

			// 다이얼로그에서 전송버튼(확인)을 누르면 hWriteEvent가 signal되어 실제 전송이 이루어짐.
			SetEvent(hWriteEvent); // 해당 구문을 적지 않아 ClientMain에서 무한히 Wait하는 버그가 있었음.
			SetFocus(hEdit1);
			SendMessage(hEdit1, EM_SETSEL, 0, -1);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return true;
		}
		return false;
	}
	return false;
}

// printf와 같이 가변인자를 받아 edit창에 입력하는 함수.
void DisplayText(char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	int nLength = GetWindowTextLength(hEdit2);
	SendMessage(hEdit2, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit2, EM_REPLACESEL, false, (LPARAM)cbuf);

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

// 서버로부터 데이터를 수신 받는 함수.
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) // 최대길이를 초과하면 탈출.
	{
		// 서버로 부터 데이터를 수신 받음.
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0) // 수신받은 데이터가 없으면 탈출.
		{
			break;
		}

		left -= received; // 수신받은 데이터길이 만큼 left값을 줄임.
		ptr += received; // 버퍼에 포인터주소값을 데이터길이만큼 더하여 입력을 이어서 할 수 있도록 함.
	}

	// 입력받은 모든 데이터의 길이반환.
	return (len - left);
}

DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// 윈도우 소켓 초기화.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// 클라이언트 소켓을 생성함.
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit(TEXT("socket()"));
	}

	// 서버 소켓 주소 디스크립터를 작성함.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	// 디스크립터를 이용해 서버에 연결을 시도함.
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit(TEXT("connect()"));
	}

	while (true)
	{
		// 다이얼로그에서 signal해주기 이전까지 무한히 대기함.
		WaitForSingleObject(hWriteEvent, INFINITE);

		// 버퍼가 비어있으면(아무값도 전송하지 않았으면) 전송버튼을 활성화함.
		// 또한, hReadEvent를 signal로 하여 전송버튼을 누르면 Wait에 걸리지 않도록함.
		if (strlen(buf) == 0)
		{
			EnableWindow(hSendButton, true);
			SetEvent(hReadEvent);
			continue;
		}

		// 문자열 버퍼에 있는 데이터를 서버로 전송함.
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		DisplayText(TEXT("[TCP 클라이언트] %d바이트를 보냈습니다.\r\n"), retval);

		// 서버로부터의 수신을 대기함.
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("recv()"));
			break;
		}
		else if (retval == 0) // 수신된 데이터의 길이가 0이면 탈출.
		{
			break;
		}

		buf[retval] = '\0';
		DisplayText(TEXT("[TCP 클라이언트] %d바이트를 받았습니다.\r\n"), retval);
		
		DisplayText(TEXT("[받은 데이터] %s\r\n"), buf);

		// 전송버튼 활성화.
		EnableWindow(hSendButton, true);

		// hReadEvent를 signal로 하여 데이터를 전송할때 Wait에 걸리지 않도록 함.
		SetEvent(hReadEvent);
	}

	return 0;
}

// hReadEvnet의 경우 EnableWindow와 같이 동작하면서, 수신이 완전히 종료되기 이전에 전송을 할 수 없도록함.
// 일종의 2중 안전장치 같은 느낌을 받았음.

