#pragma comment (lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

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
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0); // IPv6�� ���������� ������.
	if (listen_sock == INVALID_SOCKET) // ������ ������ ��ȿ���� ������ ���� ��� �� ���α׷� ����.
	{
		err_quit("socket()");
	}

	// bind()
	SOCKADDR_IN6 serveraddr; // �����ּҸ� ������.
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(SERVERPORT); // Ŭ���̾�Ʈ ��Ʈ�ּҸ� ��Ʈ��ũ �������� ������.
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr)); // �������Ͽ� �����ּҸ� ���ε���.
	if (retval == SOCKET_ERROR) // ���ε��� �����ϸ� ������� �� ���α׷� ����.
	{
		err_quit("bind()");
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN); // ������ Ŭ���̾�Ʈ�� ������ ����ϵ��� ��.
	if (retval == SOCKET_ERROR) // ������� �����ϸ� ������� �� ���α׷� ����.
	{
		err_quit("listen()");
	}

	// ������ ��ſ� ����� ����
	SOCKET client_sock; // Ŭ���̾�Ʈ�� ����
	SOCKADDR_IN6 clientaddr; // Ŭ���̾�Ʈ�� �ּ�
	int addrlen; // �ּұ���
	char buf[BUFSIZE + 1]; // ������ ����ũ��

	while (1)
	{
		// accpet()
		addrlen = sizeof(clientaddr);

		// Ŭ���̾�Ʈ�� ������ �ö����� ���.
		// Ŭ���̾�Ʈ�� �����ϸ� ���ͷ�Ʈ�� �����ϰ� �������� ������.
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) // ������ �����ϸ� ������ ��� �� ������ ������.
		{
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		WSAAddressToString((SOCKADDR*)& clientaddr, sizeof(clientaddr), nullptr, ipaddr, &ipaddrlen); // �ּҸ� ���ڿ� �������� ��ȯ��.
		printf("\n[TCP ����] Ŭ���Ʈ ����: %s\n", ipaddr); // ��ȯ�� ���ڿ��� ������ �ֿܼ� ���� �����޼����� �����.

		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			// ������ �ޱ�
			retval = recv(client_sock, buf, BUFSIZE, 0); // Ŭ���̾�Ʈ�� ���� ���۸� ����.
			if (retval == SOCKET_ERROR) // ���۸� �޴°������� ������ ������ �ش� ������ ��� �� ������ ������.
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0) // ���ڿ��� ũ�Ⱑ 0�̸� ������ ������.
			{
				break;
			}

			buf[retval] = '\n'; // ������ ���� �ι��ڸ� �߰��Ͽ� ���ڿ��� ���� ǥ����.
			printf("[TCP/%s] %s\n", ipaddr, buf); // IP�ּҿ� ���۸� �����.

			// ������ ������
			retval = send(client_sock, buf, retval, 0); // Ŭ���̾�Ʈ�� ������ ���۸� �۽���.
			if (retval == SOCKET_ERROR) // �۽Ű������� ������ ������ �ش� ������ ��� �� ������ ������.
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock); // Ŭ���̾�Ʈ ������ ����.
		printf("[TCP ����] Ŭ���Ʈ ����: %s\n", ipaddr);
	}

	// closesocket()
	closesocket(listen_sock); // ���� ������ ����.

	WSACleanup(); // ������ ������.

	return 0;
}