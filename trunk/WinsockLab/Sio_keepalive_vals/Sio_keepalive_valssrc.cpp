// Description:
//    This sample shows you how to set the SO_KEEPALIVE_VALS
//    on a socket. This option allows you to set the keepalive
//    interval on a per socket basis as opposed to the rather
//    useless SO_KEEPALIVE option.
//
// No command line arguments
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Mstcpip.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    DWORD         dwBytesRet=0;
    struct tcp_keepalive   alive;

    // Load Winsock lib
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() should be fine!\n");

    // Create a socket
    s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is fine!\n");

    // Set the keepalive values
    alive.onoff = TRUE;
    alive.keepalivetime = 7200000;
    alive.keepaliveinterval = 6000;

    if (WSAIoctl(s, SIO_KEEPALIVE_VALS, &alive, sizeof(alive),
		NULL, 0, &dwBytesRet, NULL, NULL) == SOCKET_ERROR)
    {
        printf("WSAIotcl(SIO_KEEPALIVE_VALS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAIotcl(SIO_KEEPALIVE_VALS) is working!\n");

    printf("SIO_KEEPALIVE_VALS set:\n");
    printf("   Keepalive Time     = %lu\n", alive.keepalivetime);
    printf("   Keepalive Interval = %lu\n", alive.keepaliveinterval);

    // Cleanup
    if(closesocket(s) == 0)
		printf("closesocket() should be fine!\n");
	else
		printf("closesocket() failed with error code %d\n", WSAGetLastError());
    
	if(WSACleanup() == 0)
		printf("WSACleanup() is OK!\n");
	else
		printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}
