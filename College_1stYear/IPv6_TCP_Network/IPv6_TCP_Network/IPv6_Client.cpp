#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "::1"
#define SERVERPORT 3317
#define BUFSIZE 512

// ���� ��� �Լ�
// ���� ��� �� ���α׷��� ������.
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0,
		nullptr);
	MessageBox(nullptr, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� ��� �Լ�
// ���� ��� �� ���α׷��� ������.
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0,
		nullptr);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
// ������ ���� ������ 1���� �޴� ���� �ƴ� n���� ������ 1���� �������� �����Ͽ� ó����.
// ��, ������ �뷮�� �ʰ��ϴ� �����̳�, ���̻� ������ ���� ��쿡�� ������.
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) // ������ ���̸� �ʰ��ϴ� �Է��� ������ Ż����.
	{
		received = recv(s, ptr, left, flags); // ������ ���� �����͸� ����.
		if (received == SOCKET_ERROR) // ���Ű������� �����ϸ� SOCKET_ERROR�� ��ȯ��.
		{
			return SOCKET_ERROR;
		}
		else if (received == 0) // ���� ���ڿ��� ���̰� 0�̸� ������ Ż����.
		{
			break;
		}

		left -= received; // ���� ���ڿ� ���̸�ŭ ������.
		ptr += received; // ���ڿ��� �̾ �߰��� �� �ֵ��� ���ڿ� ���̰��� ����.
	}

	return (len - left); // ���������� ������ ���� ������ ���ڿ� ���̰��� �����.
}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket()
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0); // Ŭ���̾�Ʈ ������ ������.
	if (sock == INVALID_SOCKET) // ������ ��ȿ���� ������ ���� ��� �� ���α׷� ����.
	{
		err_quit("socket()");
	}

	// connect()
	SOCKADDR_IN6 serveraddr; // ���� �ּҸ� ����.
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	int addrlen = sizeof(serveraddr);
	// ���ڿ��� �Ǿ��ִ� �ּҸ� �ּ� ����ü�� ��ȯ��.
	WSAStringToAddress(SERVERIP, AF_INET6, nullptr, (SOCKADDR*)&serveraddr, &addrlen);
	retval = connect(sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr)); // ��ȯ�� �����ּҸ� ������ ������ ������ ��û��.
	if (retval == SOCKET_ERROR) // ������ �����ϸ� �ش� ������ ��� �� ���α׷��� ������.
	{
		err_quit("connect()");
	}

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == nullptr) // ���ڿ��� �Է¹���. ��ȿ���� ������ ��������.
		{
			break;
		}

		// '\n'���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n') // ���๮�ڸ� �ι��ڷ� ��ü�����ν� ���ڿ��� ���� ǥ����.
		{
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) // ���ڿ��� ���̰� 0�̸� ������ ������.
		{
			break;
		}

		// ������ ������
		retval = send(sock, buf, strlen(buf), 0); // ������ ���۸� �۽���.
		if (retval == SOCKET_ERROR) // �۽��� �����ϸ� �ش� ������ ����ϰ� ������ ������.
		{
			err_display("send()");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		retval = recvn(sock, buf, retval, 0); // ������ ���� �����͸� ����.
		if (retval == SOCKET_ERROR) // ���Ű������� �����ϸ� �ش� ���� ��� �� ��������.
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0) // ���Ź��� ���ڿ��� ���̰� 0�̸� ��������.
		{
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0'; // ���ڿ��� ���� �ι��� �߰�.
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);
	}

	// closesocket()
	closesocket(sock); // Ŭ���̾�Ʈ ������ ������.

	// ��������
	WSACleanup();
	
	return 0;
}

