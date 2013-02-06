#include "socket_manager.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

socket_manager::socket_manager(void)
{
	WSADATA d = {0};
	WSAStartup(MAKEWORD(2,2), &d);
}

socket_manager::~socket_manager(void)
{
	WSACleanup();
}
