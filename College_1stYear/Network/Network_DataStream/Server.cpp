#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 3317
#define BUFSIZE 512

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
		(LPTSTR)&lpMsgBuf,
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
		(LPTSTR)&lpMsgBuf,
		0,
		nullptr);
	printf("[%s] %s", msg, (char*)lpMsgBuf); // 콘솔 창에 에러 메세지 출력
	LocalFree(lpMsgBuf); // FormatMessage함수에서 동적할당 되었으므로 메모리를 해제함
}

/*
* @ param s : recv함수에 넘겨줄 소켓 디스크립터 (클라이언트의 소켓 디스크립터를 받음)
* @ param buf : 메세지를 입력받을 문자열 변수
* @ param len : buf 문자열 배열 변수의 길이
* @ param flags : recv함수에 넘겨줄 데이터 읽는 옵션에 대한 플래그
*/
// 클라이언트에게 데이터를 받는 함수, 최대 len크기의 데이터를 받을 수 있음
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received; // 수신 받은 데이터의 길이를 저장하는 변수
	char* ptr = buf;
	int left = len; // 현재 받은 문자열의 양을 카운트하는 변수

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) // 에러가 발생하면 에러 반환
		{
			return SOCKET_ERROR;
		}
		else if (received == 0) // 수신받은 데이터가 없으면 종료
		{
			break;
		}

		left -= received; // 수신받은 데이터 만큼 카운트
		ptr += received; // 수신받은 데이터 만큼 포인터 주소 증가, 이어서 문자열을 받을 수 있음
	}

	return (len - left); // len, left의 sub연산을 통해 수신받은 데이터의 총량을 반환함
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
	// 파라미터에 맞는 소켓 디스크립터를 획득 (서버)
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// 서버 소켓주소 구조체 변수 선언.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // 구조체 변수의 메모리를 초기화.
	serveraddr.sin_family = AF_INET; // 프로토콜 Family를 지정함 (위에서 AF_INET을 사용하였으므로 동일하게)
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // 주소를 0으로 입력함. 따라서, 시스템이 자동으로 주소를 획득 (htonl함수를 통해 4바이트 호스트 데이터를 네트워크 데이터로 변환)
	serveraddr.sin_port = htons(SERVERPORT); // define해둔 서버 포트를 입력함. (htons함수를 통해 2바이트 호스트 데이터를 네크워크 데이터로 변환)

	/*
	* @ function : bind()
	* @ param s : 바인딩할 소켓 디스크립터를 입력
	* @ param name : 소켓주소 구조체 변수의 포인터를 입력
	* @ param namelen : 소켓주소 구조체 변수의 메모리 크기를 입력
	*/
	// 소켓 디스크립터와 소켓주소 구조체를 바인딩하여 소켓 디스크립터를 통신에 사용할 수 있도록 함
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	/*
	* @ function : listen()
	* @ param s : 리슨 상태로 만들 소켓 디스크립터를 입력
	* @ param backlog : 대기 메세지 큐의 길이를 입력함
	*/
	// 입력한 소켓을 리슨 상태로 만듦 (대기 메세지 큐는 최대 길이로 지정함)
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	SOCKET client_sock; // 클라이언트의 소켓 디스크립터 선언
	SOCKADDR_IN clientaddr; // 클라이언트의 소켓주소 구조체 선언
	int addrlen; // 소켓주소 구조체의 메모리 크기를 저장할 변수
	char buf [BUFSIZE + 1]; // 데이터 버퍼 선언
	int len; // 데이터 버퍼의 길이를 저장할 변수

	while (1)
	{
		addrlen = sizeof(clientaddr); // 메모리 크기 저장.

		/*
		* @ function : accept()
		* @ param s : 소켓 디스크립터를 입력
		* @ param addr : 클라이언트 소켓주소를 받을 구조체를 입력
		* @ param addrlen : 클라이언트 소켓주소 구조체의 메모리 크기를 입력
		* @ return : 클라이언트의 소켓 디스크립터를 반환
		*/
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// 클라이언트과의 접속이 성공하면 클라이언트의 주소와 포트번호를 출력함
		// inet_ntoa() : 네크워크 데이터를 .(dot)으로 구분된 문자열로 변환함
		// ntohs() : 네트워크 데이터를 2바이트 호스트 데이터로 변환함
		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		while (1)
		{
			/*
			* @ param s : recv함수에 넘겨줄 소켓 디스크립터 (클라이언트의 소켓 디스크립터를 받음)
			* @ param buf : 메세지를 입력받을 문자열 변수 (입력 받을 데이터의 길이)
			* @ param len : buf 문자열 배열 변수의 길이 (4바이트 고정)
			* @ param flags : recv함수에 넘겨줄 데이터 읽는 옵션에 대한 플래그
			*/
			// 클라이언트로부터 전송받을 데이터의 크기를 수신받음 (Fixed)
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
			* @ param s : recv함수에 넘겨줄 소켓 디스크립터 (클라이언트의 소켓 디스크립터를 받음)
			* @ param buf : 메세지를 입력받을 문자열 변수
			* @ param len : buf 문자열 배열 변수의 길이 (위의 수신 받은 길이에 따라 가변)
			* @ param flags : recv함수에 넘겨줄 데이터 읽는 옵션에 대한 플래그
			*/
			// 클라이언트로부터 실 데이터를 전송 받음 (Variable)
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

			buf[retval] = '\0'; // 문자열 끝에 널문자를 넣어 문자열을 출력할 수 있도록 함

			// 데이터 수신에 성공하면 문자열을 IP, PORT, 메시지를 출력함
			// inet_ntoa() : 네크워크 데이터를 .(dot)으로 구분된 문자열로 변환함
			// ntohs() : 네트워크 데이터를 2바이트 호스트 데이터로 변환함
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
		}

		// 클라이언트의 소켓 디스크립터를 해제함
		closesocket(client_sock);

		// 연결을 종료한 클라이언트의 주소, 포트번호를 출력함.
		// inet_ntoa() : 네크워크 데이터를 .(dot)으로 구분된 문자열로 변환함
		// ntohs() : 네트워크 데이터를 2바이트 호스트 데이터로 변환함
		printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// 서버의 소켓 디스크립터를 해제함
	closesocket(listen_sock);

	// 윈도우 소켓을 해제함
	WSACleanup();

	return 0;
}

