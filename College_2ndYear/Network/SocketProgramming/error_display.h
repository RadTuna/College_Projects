#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "user32")
#pragma comment(lib, "ws2_32")

#include <winsock2.h>

void err_quit(const TCHAR* msg);
void err_display(const TCHAR* msg);
