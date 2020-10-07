
#include <stdlib.h>
#include <stdio.h>

#include "../error_display.h"


#define SERVER_PORT (9000)
#define BUFFER_SIZE (512)

static DWORD WINAPI process_client(LPVOID arg);


int main(void) 
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        err_quit(TEXT("socket()"));
    }

    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    retval = bind(listen_sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (retval == SOCKET_ERROR) {
        err_quit(TEXT("listen()"));
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) {
        err_quit(TEXT("listen()"));
    }

    SOCKET client_sock;
    SOCKADDR_IN client_addr;
    int addr_len;
    HANDLE hThread;

    while (TRUE) {
        addr_len = sizeof(client_addr);
        client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addr_len);
        if (client_sock == INVALID_SOCKET) {
            err_display(TEXT("accept()"));
            break;
        }

        printf("\n[TCP Server] Connected client : IP Address=%s, Port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        hThread = CreateThread(NULL, 0, process_client, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) {
            closesocket(client_sock);
        } else {
            CloseHandle(hThread);
        }
    }

    closesocket(listen_sock);

    WSACleanup();

    return 0;
}

static DWORD WINAPI process_client(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN client_addr;
    int addr_len;
    char buffer[BUFFER_SIZE + 1];

    addr_len = sizeof(client_addr);
    getpeername(client_sock, (SOCKADDR*)&client_addr, &addr_len);

    while (TRUE) {
        retval = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display(TEXT("recv()"));
            break;
        } else if (retval == 0) {
            break;
        }

        buffer[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        retval = send(client_sock, buffer, retval, 0);
        if (retval == SOCKET_ERROR) {
            err_display(TEXT("send()"));
            break;
        }
    }

    closesocket(client_sock);
    printf("[TCP Server] End of client : IP Address=%s, Port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return 0;
}

