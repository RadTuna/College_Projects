#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE 50

/*
* @ param msg : Error MessageBox의 헤더 문자열을 지정.
*/
// 가장 마지막에 발생한 윈도우 소켓 에러를 반환하고 프로그램을 종료함.
void err_quit(char* msg)
{
	LPVOID lpMsgBuf; // 에러 문자열을 저장하는 보이드 포인터변수
	FormatMessage( // 반환 된 에러코드를 문자열로 변환해주는 함수
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0,
		nullptr);
	MessageBox(nullptr, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR); // 메세지 박스로 에러 메세지 출력
	LocalFree(lpMsgBuf); // FormatMessage함수에서 동적할당 되었으므로 메모리를 해제함
	exit(1); // 프로그램 종료
}

/*
* @ param msg : Error Message [] 내부의 문자열을 지정.
*/
// 가장 마지막에 발생한 윈도우 소켓 에러를 반환함.
void err_display(char* msg)
{
	LPVOID lpMsgBuf; // 에러 문자열을 저장하는 보이드 포인터변수
	FormatMessage( // 반환 된 에러코드를 문자열로 변환해주는 함수
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0,
		nullptr);
	printf("[%s] %s", msg, (char*)lpMsgBuf); // 콘솔 창에 에러 메세지 출력
	LocalFree(lpMsgBuf); // FormatMessage함수에서 동적할당 되었으므로 메모리를 해제함
}

/*
* @ param argc : 프로그램 실행옵션의 개수
* @ param argv : 프로그램 실행옵션 문자열 배열
*/
int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;

	/*
	* @ function : WSAStartup()
	* @ param wVersionRequested : 윈도우 소켓의 버전을 입력
	* @ param lpWSAData : WSADATA형의 변수의 포인터를 입력
	*/
	// 윈도우 소켓을 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	/*
	* @ function : socket()
	* @ param af : 어떤 범위에서 통신할지에 대한 프로토콜 Family를 지정함 (IPv6 / IPv4 / Local / etc...)
	* @ param type : 데이터 통신 방식을 지정함 (UDP / TCP)
	* @ param protocol : 특정한 프로토콜을 지정함
	* @ return : 해당 소켓을 가리키는 소켓 디스크립터를 반환함
	*/
	// 파라미터에 맞는 소켓 디스크립터를 획득 (클라이언트)
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// 서버 소켓주소 구조체 변수 선언.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // 구조체 변수의 메모리를 초기화.
	serveraddr.sin_family = AF_INET; // 프로토콜 Family를 지정함 (위에서 AF_INET을 사용하였으므로 동일하게)
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); // 주소를 0으로 입력함. 따라서, 시스템이 자동으로 주소를 획득 (inet_addr함수를 통해 호스트 데이터를 네트워크 데이터로 변환)
	serveraddr.sin_port = htons(SERVERPORT); // define해둔 서버 포트를 입력함. (htons함수를 통해 2바이트 호스트 데이터를 네크워크 데이터로 변환)

	/*
	* @ function : bind()
	* @ param s : 클라이언트의 소켓 디스크립터를 입력
	* @ param name : 서버의 소켓주소 구조체 변수의 포인터를 입력
	* @ param namelen : 서버 소켓주소 구조체 변수의 메모리 크기를 입력
	*/
	// 생성한 소켓 디스크립터와 서버와의 연결을 요청함 (서버는 소켓주소 구조체의 데이터를 기반으로 접속함)
	retval = connect(sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// 송신할 데이터 변수를 선언
	char buf[BUFSIZE];
	char* testData[] = {
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요"
	};
	int len; // 문자열의 길이를 저장할 변수

	for (int i = 0; i < 4; ++i)
	{
		len = strlen(testData[i]); // 문자열 길이를 저장
		strncpy_s(buf, testData[i], len); // 버퍼에 해당 문자열을 복사함

		/*
		* @ function : send()
		* @ param s : 클라이언트의 소켓 디스크립터를 입력
		* @ param buf : 전송할 데이터 버퍼를 입력 (이후에 전송할 문자열의 길이를 전송)
		* @ param len : 전송할 데이터의 길이를 입력 (4바이트)
		* @ param flags : 전송 시 연결옵션에 대한 플래그
		*/
		// 소켓 디스크립터에 연결된 서버로 데이터를 전송함 (Fixed)
		retval = send(sock, (char*)& len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		/*
		* @ function : send()
		* @ param s : 클라이언트의 소켓 디스크립터를 입력
		* @ param buf : 전송할 데이터 버퍼를 입력
		* @ param len : 전송할 데이터의 길이를 입력
		* @ param flags : 전송 시 연결옵션에 대한 플래그
		*/
		// 소켓 디스크립터에 연결된 서버로 데이터를 전송함 (Variable)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		// 데이터 송신에 성공하면 보낸 데이터의 길이를 출력함.
		printf("[TCP 클라이언트] %d + %d바이트를 보냈습니다.\n", sizeof(int), retval);
	}

	// 클라이언트의 소켓 디스크립터를 해제함
	closesocket(sock);

	// 윈도우 소켓을 해제함
	WSACleanup();

	return 0;
}

