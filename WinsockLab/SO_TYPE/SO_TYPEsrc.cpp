// Description:
//    This sample creates a socket and then calls the SO_TYPE option
//    to retrieve the socket type (SOCK_STREAM, SOCK_DGRAM, etc) for
//    that socket.
// 
// Command line arguments
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock, create a socket, and then call the SO_TYPE
//    option and print the results.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    int           ret, iVal, iSize;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() should be fine!\n");

    // In this case create a UDP SOCK_DGRAM socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    // Retrieve the type and print its value
    iSize = sizeof(iVal);
    ret = getsockopt(s, SOL_SOCKET, SO_TYPE, (char *)&iVal, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_TYPE) failed with error code %d\n", iVal);
        return -1;
    }
	else
		printf("getsockopt(SO_TYPE) is pretty fine!\n");

	printf("Protocol type: ");

    if (iVal == SOCK_STREAM)
		printf("SOCK_STREAM\n"); 
    else if (iVal == SOCK_DGRAM)
        printf("SOCK_DGRAM\n"); 
    else if (iVal == SOCK_RDM)
        printf("SOCK_RDM\n"); 
    else if (iVal == SOCK_SEQPACKET)
        printf("SOCK_SEQPKACKET\n"); 
    else if (iVal == SOCK_RAW)
        printf("SOCK_RAW\n"); 
    else
        printf("Unknown\n");

    // Clean-up
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
