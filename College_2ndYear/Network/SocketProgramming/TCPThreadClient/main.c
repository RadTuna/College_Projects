
#include <stdlib.h>
#include <stdio.h>

#include "../error_display.h"


#define SERVER_IP ("127.0.0.1")
#define SERVER_PORT (9000)
#define BUFFER_SIZE (512)

static int recvn(SOCKET sock, char* buffer, int len, int flags);


int main(void) 
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        err_quit(TEXT("socket()"));
    }

    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    
    retval = connect(sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (retval == SOCKET_ERROR) {
        err_quit(TEXT("connect()"));
    }

    char buffer[BUFFER_SIZE + 1];
    int buffer_len;

    while (TRUE) {
        printf("\n[Send Data] ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        buffer_len = strlen(buffer);
        if (buffer[buffer_len - 1] == '\n') {
            buffer[buffer_len - 1] = '\0';
            if ((buffer_len - 1) <= 0) {
                break;
            }
        }

        if (buffer_len == 0) {
            break;
        }

        retval = send(sock, buffer, strlen(buffer), 0);
        if (retval == SOCKET_ERROR) {
            err_display(TEXT("send()"));
            break;
        }

        printf("[TCP Client] Send %d Byte.\n", retval);

        retval = recvn(sock, buffer, retval, 0);
        if (retval == SOCKET_ERROR) {
            err_display(TEXT("recv()"));
            break;
        } else if (retval == 0) {
            break;
        }

        buffer[retval] = '\0';
        printf("[TCP Clinet] Receive %d Byte.\n", retval);
        printf("[Received Data] %s\n", buffer);
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}

static int recvn(SOCKET sock, char* buffer, int len, int flags)
{
    int received;
    char* buffer_ptr = buffer;
    int left = len;

    while (left > 0) {
        received = recv(sock, buffer_ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        } else if (received == 0) {
            break;
        }

        left -= received;
        buffer_ptr += received;
    }   

    return (len - left);
}
