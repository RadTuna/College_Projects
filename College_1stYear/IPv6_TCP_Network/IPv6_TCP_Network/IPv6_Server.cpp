#pragma comment (lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 3317
#define BUFSIZE 512


// 에러 출력 함수
// 에러 출력 후 프로그램을 종료함.
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

// 에러 출력 함수
// 에러 출력 후 프로그램을 유지함.
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
	
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0); // IPv6로 서버소켓을 생성함.
	if (listen_sock == INVALID_SOCKET) // 생성된 소켓이 유효하지 않으면 에러 출력 후 프로그램 종료.
	{
		err_quit("socket()");
	}

	// bind()
	SOCKADDR_IN6 serveraddr; // 서버주소를 생성함.
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(SERVERPORT); // 클라이언트 포트주소를 네트워크 형식으로 변경함.
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr)); // 서버소켓에 서버주소를 바인딩함.
	if (retval == SOCKET_ERROR) // 바인딩이 실패하면 에러출력 후 프로그램 종료.
	{
		err_quit("bind()");
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN); // 서버가 클라이언트의 접속을 대기하도록 함.
	if (retval == SOCKET_ERROR) // 대기명령이 실패하면 에러출력 후 프로그램 종료.
	{
		err_quit("listen()");
	}

	// 데이터 통신에 사용할 변수
	SOCKET client_sock; // 클라이언트의 소켓
	SOCKADDR_IN6 clientaddr; // 클라이언트의 주소
	int addrlen; // 주소길이
	char buf[BUFSIZE + 1]; // 데이터 버퍼크기

	while (1)
	{
		// accpet()
		addrlen = sizeof(clientaddr);

		// 클라이언트의 접속이 올때까지 대기.
		// 클라이언트가 접속하면 인터럽트를 해제하고 다음줄을 실행함.
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) // 접속이 실패하면 에러를 출력 후 연결을 해제함.
		{
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		WSAAddressToString((SOCKADDR*)& clientaddr, sizeof(clientaddr), nullptr, ipaddr, &ipaddrlen); // 주소를 문자열 형식으로 변환함.
		printf("\n[TCP 서버] 클라언트 접속: %s\n", ipaddr); // 변환한 문자열을 가지고 콘솔에 접속 성공메세지를 출력함.

		// 클라이언트와 데이터 통신
		while (1)
		{
			// 데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0); // 클라이언트로 부터 버퍼를 받음.
			if (retval == SOCKET_ERROR) // 버퍼를 받는과정에서 에러가 있으면 해당 에러를 출력 후 연결을 해제함.
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0) // 문자열의 크기가 0이면 연결을 종료함.
			{
				break;
			}

			buf[retval] = '\n'; // 버퍼의 끝에 널문자를 추가하여 문자열의 끝을 표시함.
			printf("[TCP/%s] %s\n", ipaddr, buf); // IP주소와 버퍼를 출력함.

			// 데이터 보내기
			retval = send(client_sock, buf, retval, 0); // 클라이언트에 동일한 버퍼를 송신함.
			if (retval == SOCKET_ERROR) // 송신과정에서 에러가 있으면 해당 에러를 출력 후 연결을 해제함.
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock); // 클라이언트 소켓을 닫음.
		printf("[TCP 서버] 클라언트 종료: %s\n", ipaddr);
	}

	// closesocket()
	closesocket(listen_sock); // 서버 소켓을 닫음.

	WSACleanup(); // 윈속을 해제함.

	return 0;
}