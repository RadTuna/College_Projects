
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
	hReadEvent = CreateEvent(nullptr, false, true, nullptr);
	if (hReadEvent == nullptr)
	{
		return 1;
	}

	hWriteEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hWriteEvent == nullptr)
	{
		return 1;
	}

	CreateThread(nullptr, 0, ClientMain, nullptr, 0, nullptr);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	closesocket(sock);

	WSACleanup();

	return 0;
}

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
			EnableWindow(hSendButton, false);
			WaitForSingleObject(hReadEvent, INFINITE);
			GetDlgItemText(hDlg, IDC_EDIT1, buf, BUFSIZE + 1);
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

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit(TEXT("socket()"));
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit(TEXT("connect()"));
	}

	while (true)
	{
		WaitForSingleObject(hWriteEvent, INFINITE);

		if (strlen(buf) == 0)
		{
			EnableWindow(hSendButton, true);
			SetEvent(hReadEvent);
			continue;
		}

		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		DisplayText(TEXT("[TCP 클라이언트] %d바이트를 보냈습니다.\r\n"), retval);

		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(TEXT("recv()"));
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		buf[retval] = '\0';
		DisplayText(TEXT("[TCP 클라이언트] %d바이트를 받았습니다.\r\n"), retval);
		
		DisplayText(TEXT("[받은 데이터] %s\r\n"), buf);

		EnableWindow(hSendButton, true);
		SetEvent(hReadEvent);
	}

	return 0;
}

