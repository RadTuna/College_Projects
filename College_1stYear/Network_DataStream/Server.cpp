#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 3317
#define BUFSIZE 512

/*
* @ param msg : Error MessageBox�� ��� ���ڿ��� ����.
*/
// ���� �������� �߻��� ������ ���� ������ ��ȯ�ϰ� ���α׷��� ������.
void err_quit(char* msg)
{
	LPVOID lpMsgBuf; // ���� ���ڿ��� �����ϴ� ���̵� �����ͺ���
	FormatMessage( // ��ȯ �� �����ڵ带 ���ڿ��� ��ȯ���ִ� �Լ�
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		nullptr);
	MessageBox(nullptr, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR); // �޼��� �ڽ��� ���� �޼��� ���
	LocalFree(lpMsgBuf); // FormatMessage�Լ����� �����Ҵ� �Ǿ����Ƿ� �޸𸮸� ������
	exit(1); // ���α׷� ����
}

/*
* @ param msg : Error Message [] ������ ���ڿ��� ����.
*/
// ���� �������� �߻��� ������ ���� ������ ��ȯ��.
void err_display(char* msg)
{
	LPVOID lpMsgBuf; // ���� ���ڿ��� �����ϴ� ���̵� �����ͺ���
	FormatMessage( // ��ȯ �� �����ڵ带 ���ڿ��� ��ȯ���ִ� �Լ�
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		nullptr);
	printf("[%s] %s", msg, (char*)lpMsgBuf); // �ܼ� â�� ���� �޼��� ���
	LocalFree(lpMsgBuf); // FormatMessage�Լ����� �����Ҵ� �Ǿ����Ƿ� �޸𸮸� ������
}

/*
* @ param s : recv�Լ��� �Ѱ��� ���� ��ũ���� (Ŭ���̾�Ʈ�� ���� ��ũ���͸� ����)
* @ param buf : �޼����� �Է¹��� ���ڿ� ����
* @ param len : buf ���ڿ� �迭 ������ ����
* @ param flags : recv�Լ��� �Ѱ��� ������ �д� �ɼǿ� ���� �÷���
*/
// Ŭ���̾�Ʈ���� �����͸� �޴� �Լ�, �ִ� lenũ���� �����͸� ���� �� ����
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received; // ���� ���� �������� ���̸� �����ϴ� ����
	char* ptr = buf;
	int left = len; // ���� ���� ���ڿ��� ���� ī��Ʈ�ϴ� ����

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) // ������ �߻��ϸ� ���� ��ȯ
		{
			return SOCKET_ERROR;
		}
		else if (received == 0) // ���Ź��� �����Ͱ� ������ ����
		{
			break;
		}

		left -= received; // ���Ź��� ������ ��ŭ ī��Ʈ
		ptr += received; // ���Ź��� ������ ��ŭ ������ �ּ� ����, �̾ ���ڿ��� ���� �� ����
	}

	return (len - left); // len, left�� sub������ ���� ���Ź��� �������� �ѷ��� ��ȯ��
}

/*
* @ param argc : ���α׷� ����ɼ��� ����
* @ param argv : ���α׷� ����ɼ� ���ڿ� �迭
*/
int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;

	/*
	* @ function : WSAStartup()
	* @ param wVersionRequested : ������ ������ ������ �Է�
	* @ param lpWSAData : WSADATA���� ������ �����͸� �Է�
	*/
	// ������ ������ �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	/*
	* @ function : socket()
	* @ param af : � �������� ��������� ���� �������� Family�� ������ (IPv6 / IPv4 / Local / etc...)
	* @ param type : ������ ��� ����� ������ (UDP / TCP)
	* @ param protocol : Ư���� ���������� ������
	* @ return : �ش� ������ ����Ű�� ���� ��ũ���͸� ��ȯ��
	*/
	// �Ķ���Ϳ� �´� ���� ��ũ���͸� ȹ�� (����)
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// ���� �����ּ� ����ü ���� ����.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // ����ü ������ �޸𸮸� �ʱ�ȭ.
	serveraddr.sin_family = AF_INET; // �������� Family�� ������ (������ AF_INET�� ����Ͽ����Ƿ� �����ϰ�)
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // �ּҸ� 0���� �Է���. ����, �ý����� �ڵ����� �ּҸ� ȹ�� (htonl�Լ��� ���� 4����Ʈ ȣ��Ʈ �����͸� ��Ʈ��ũ �����ͷ� ��ȯ)
	serveraddr.sin_port = htons(SERVERPORT); // define�ص� ���� ��Ʈ�� �Է���. (htons�Լ��� ���� 2����Ʈ ȣ��Ʈ �����͸� ��ũ��ũ �����ͷ� ��ȯ)

	/*
	* @ function : bind()
	* @ param s : ���ε��� ���� ��ũ���͸� �Է�
	* @ param name : �����ּ� ����ü ������ �����͸� �Է�
	* @ param namelen : �����ּ� ����ü ������ �޸� ũ�⸦ �Է�
	*/
	// ���� ��ũ���Ϳ� �����ּ� ����ü�� ���ε��Ͽ� ���� ��ũ���͸� ��ſ� ����� �� �ֵ��� ��
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	/*
	* @ function : listen()
	* @ param s : ���� ���·� ���� ���� ��ũ���͸� �Է�
	* @ param backlog : ��� �޼��� ť�� ���̸� �Է���
	*/
	// �Է��� ������ ���� ���·� ���� (��� �޼��� ť�� �ִ� ���̷� ������)
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	SOCKET client_sock; // Ŭ���̾�Ʈ�� ���� ��ũ���� ����
	SOCKADDR_IN clientaddr; // Ŭ���̾�Ʈ�� �����ּ� ����ü ����
	int addrlen; // �����ּ� ����ü�� �޸� ũ�⸦ ������ ����
	char buf [BUFSIZE + 1]; // ������ ���� ����
	int len; // ������ ������ ���̸� ������ ����

	while (1)
	{
		addrlen = sizeof(clientaddr); // �޸� ũ�� ����.

		/*
		* @ function : accept()
		* @ param s : ���� ��ũ���͸� �Է�
		* @ param addr : Ŭ���̾�Ʈ �����ּҸ� ���� ����ü�� �Է�
		* @ param addrlen : Ŭ���̾�Ʈ �����ּ� ����ü�� �޸� ũ�⸦ �Է�
		* @ return : Ŭ���̾�Ʈ�� ���� ��ũ���͸� ��ȯ
		*/
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// Ŭ���̾�Ʈ���� ������ �����ϸ� Ŭ���̾�Ʈ�� �ּҿ� ��Ʈ��ȣ�� �����
		// inet_ntoa() : ��ũ��ũ �����͸� .(dot)���� ���е� ���ڿ��� ��ȯ��
		// ntohs() : ��Ʈ��ũ �����͸� 2����Ʈ ȣ��Ʈ �����ͷ� ��ȯ��
		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		while (1)
		{
			/*
			* @ param s : recv�Լ��� �Ѱ��� ���� ��ũ���� (Ŭ���̾�Ʈ�� ���� ��ũ���͸� ����)
			* @ param buf : �޼����� �Է¹��� ���ڿ� ���� (�Է� ���� �������� ����)
			* @ param len : buf ���ڿ� �迭 ������ ���� (4����Ʈ ����)
			* @ param flags : recv�Լ��� �Ѱ��� ������ �д� �ɼǿ� ���� �÷���
			*/
			// Ŭ���̾�Ʈ�κ��� ���۹��� �������� ũ�⸦ ���Ź��� (Fixed)
			retval = recvn(client_sock, (char*)& len, sizeof(int), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			/*
			* @ param s : recv�Լ��� �Ѱ��� ���� ��ũ���� (Ŭ���̾�Ʈ�� ���� ��ũ���͸� ����)
			* @ param buf : �޼����� �Է¹��� ���ڿ� ����
			* @ param len : buf ���ڿ� �迭 ������ ���� (���� ���� ���� ���̿� ���� ����)
			* @ param flags : recv�Լ��� �Ѱ��� ������ �д� �ɼǿ� ���� �÷���
			*/
			// Ŭ���̾�Ʈ�κ��� �� �����͸� ���� ���� (Variable)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			buf[retval] = '\0'; // ���ڿ� ���� �ι��ڸ� �־� ���ڿ��� ����� �� �ֵ��� ��

			// ������ ���ſ� �����ϸ� ���ڿ��� IP, PORT, �޽����� �����
			// inet_ntoa() : ��ũ��ũ �����͸� .(dot)���� ���е� ���ڿ��� ��ȯ��
			// ntohs() : ��Ʈ��ũ �����͸� 2����Ʈ ȣ��Ʈ �����ͷ� ��ȯ��
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
		}

		// Ŭ���̾�Ʈ�� ���� ��ũ���͸� ������
		closesocket(client_sock);

		// ������ ������ Ŭ���̾�Ʈ�� �ּ�, ��Ʈ��ȣ�� �����.
		// inet_ntoa() : ��ũ��ũ �����͸� .(dot)���� ���е� ���ڿ��� ��ȯ��
		// ntohs() : ��Ʈ��ũ �����͸� 2����Ʈ ȣ��Ʈ �����ͷ� ��ȯ��
		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// ������ ���� ��ũ���͸� ������
	closesocket(listen_sock);

	// ������ ������ ������
	WSACleanup();

	return 0;
}

