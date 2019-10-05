#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "::1"
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

// 사용자 정의 데이터 수신 함수
// 서버로 부터 수신을 1번만 받는 것이 아닌 n번의 수신을 1번의 수신으로 통합하여 처리함.
// 단, 버퍼의 용량을 초과하는 수신이나, 더이상 수신이 없을 경우에는 종료함.
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) // 버퍼의 길이를 초과하는 입력을 받으면 탈출함.
	{
		received = recv(s, ptr, left, flags); // 서버로 부터 데이터를 받음.
		if (received == SOCKET_ERROR) // 수신과정에서 실패하면 SOCKET_ERROR를 반환함.
		{
			return SOCKET_ERROR;
		}
		else if (received == 0) // 받은 문자열의 길이가 0이면 루프를 탈출함.
		{
			break;
		}

		left -= received; // 받은 문자열 길이만큼 감소함.
		ptr += received; // 문자열을 이어서 추가할 수 있도록 문자열 길이값을 더함.
	}

	return (len - left); // 최종적으로 수신이 끝난 버퍼의 문자열 길이값을 출력함.
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
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0); // 클라이언트 소켓을 생성함.
	if (sock == INVALID_SOCKET) // 소켓이 유효하지 않으면 에러 출력 후 프로그램 종료.
	{
		err_quit("socket()");
	}

	// connect()
	SOCKADDR_IN6 serveraddr; // 서버 주소를 생성.
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	int addrlen = sizeof(serveraddr);
	// 문자열로 되어있는 주소를 주소 구조체로 변환함.
	WSAStringToAddress(SERVERIP, AF_INET6, nullptr, (SOCKADDR*)&serveraddr, &addrlen);
	retval = connect(sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr)); // 변환한 서버주소를 가지고 서버에 접속을 요청함.
	if (retval == SOCKET_ERROR) // 접속이 실패하면 해당 에러를 출력 후 프로그램을 종료함.
	{
		err_quit("connect()");
	}

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	// 서버와 데이터 통신
	while (1)
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == nullptr) // 문자열을 입력받음. 유효하지 않으면 연결해제.
		{
			break;
		}

		// '\n'문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n') // 개행문자를 널문자로 대체함으로써 문자열을 끝을 표시함.
		{
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) // 문자열의 길이가 0이면 연결을 해제함.
		{
			break;
		}

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0); // 서버에 버퍼를 송신함.
		if (retval == SOCKET_ERROR) // 송신을 실패하면 해당 에러를 출력하고 연결을 해제함.
		{
			err_display("send()");
			break;
		}

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0); // 서버로 부터 데이터를 받음.
		if (retval == SOCKET_ERROR) // 수신과정에서 실패하면 해당 에러 출력 후 연결해제.
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0) // 수신받은 문자열의 길이가 0이면 연결해제.
		{
			break;
		}

		// 받은 데이터 출력
		buf[retval] = '\0'; // 문자열의 끝에 널문자 추가.
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	closesocket(sock); // 클라이언트 소켓을 해제함.

	// 윈속종료
	WSACleanup();
	
	return 0;
}

