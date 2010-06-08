// Description:
//    This is a very short sample that simply calls the
//    SO_MAX_MSG_SIZE option to determine the largest datagram
//    size possible.
//
// No command line arguments
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock, creat a UDP socket, and then call the option.
//    Print the results.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    int           iSize, iVal, ret;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is fine!\n");

    // Create a UDP socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() should be OK!\n");

    // Retrieve the option and print the result
    iSize = sizeof(iVal);
    ret = getsockopt(s, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iVal, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_MAX_MSG_SIZE) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("getsockopt(SO_MAX_MSG_SIZE) is pretty good!\n");

    printf("\nThe max message size = %d\n\n", iVal);

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
