// Description:
//    This sample shows how to obtain the broadcast address for a UDP socket.
//
// No command line arguments/parameters
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description: Load Winsock, create a socket and find the broadcast address
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   bcast;
    int           ret;
    DWORD         dwBytesRet;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() should be fine!\n");

    // Create a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    // Call the ioctl 
    ret = WSAIoctl(s, SIO_GET_BROADCAST_ADDRESS, NULL, 0,
            &bcast, sizeof(bcast), &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoclt(SIO_GET_BROADCAST_ADDRESS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAIoclt(SIO_GET_BROADCAST_ADDRESS) should be fine!\n");

    printf("Broadcast address is: '%s'\n", inet_ntoa(bcast.sin_addr));

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
