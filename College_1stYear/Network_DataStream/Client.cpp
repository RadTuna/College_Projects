#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE 50

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
		(LPTSTR)& lpMsgBuf,
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
		(LPTSTR)& lpMsgBuf,
		0,
		nullptr);
	printf("[%s] %s", msg, (char*)lpMsgBuf); // �ܼ� â�� ���� �޼��� ���
	LocalFree(lpMsgBuf); // FormatMessage�Լ����� �����Ҵ� �Ǿ����Ƿ� �޸𸮸� ������
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
	// �Ķ���Ϳ� �´� ���� ��ũ���͸� ȹ�� (Ŭ���̾�Ʈ)
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// ���� �����ּ� ����ü ���� ����.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // ����ü ������ �޸𸮸� �ʱ�ȭ.
	serveraddr.sin_family = AF_INET; // �������� Family�� ������ (������ AF_INET�� ����Ͽ����Ƿ� �����ϰ�)
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); // �ּҸ� 0���� �Է���. ����, �ý����� �ڵ����� �ּҸ� ȹ�� (inet_addr�Լ��� ���� ȣ��Ʈ �����͸� ��Ʈ��ũ �����ͷ� ��ȯ)
	serveraddr.sin_port = htons(SERVERPORT); // define�ص� ���� ��Ʈ�� �Է���. (htons�Լ��� ���� 2����Ʈ ȣ��Ʈ �����͸� ��ũ��ũ �����ͷ� ��ȯ)

	/*
	* @ function : bind()
	* @ param s : Ŭ���̾�Ʈ�� ���� ��ũ���͸� �Է�
	* @ param name : ������ �����ּ� ����ü ������ �����͸� �Է�
	* @ param namelen : ���� �����ּ� ����ü ������ �޸� ũ�⸦ �Է�
	*/
	// ������ ���� ��ũ���Ϳ� �������� ������ ��û�� (������ �����ּ� ����ü�� �����͸� ������� ������)
	retval = connect(sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// �۽��� ������ ������ ����
	char buf[BUFSIZE];
	char* testData[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�"
	};
	int len; // ���ڿ��� ���̸� ������ ����

	for (int i = 0; i < 4; ++i)
	{
		len = strlen(testData[i]); // ���ڿ� ���̸� ����
		strncpy_s(buf, testData[i], len); // ���ۿ� �ش� ���ڿ��� ������

		/*
		* @ function : send()
		* @ param s : Ŭ���̾�Ʈ�� ���� ��ũ���͸� �Է�
		* @ param buf : ������ ������ ���۸� �Է� (���Ŀ� ������ ���ڿ��� ���̸� ����)
		* @ param len : ������ �������� ���̸� �Է� (4����Ʈ)
		* @ param flags : ���� �� ����ɼǿ� ���� �÷���
		*/
		// ���� ��ũ���Ϳ� ����� ������ �����͸� ������ (Fixed)
		retval = send(sock, (char*)& len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		/*
		* @ function : send()
		* @ param s : Ŭ���̾�Ʈ�� ���� ��ũ���͸� �Է�
		* @ param buf : ������ ������ ���۸� �Է�
		* @ param len : ������ �������� ���̸� �Է�
		* @ param flags : ���� �� ����ɼǿ� ���� �÷���
		*/
		// ���� ��ũ���Ϳ� ����� ������ �����͸� ������ (Variable)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		// ������ �۽ſ� �����ϸ� ���� �������� ���̸� �����.
		printf("[TCP Ŭ���̾�Ʈ] %d + %d����Ʈ�� ���½��ϴ�.\n", sizeof(int), retval);
	}

	// Ŭ���̾�Ʈ�� ���� ��ũ���͸� ������
	closesocket(sock);

	// ������ ������ ������
	WSACleanup();

	return 0;
}

