// Description:
//    This sample shows how to set the ioctl SIO_ENABLE_CIRCULAR_QUEUEING.
//    This ioctl is very difficult to prove that it works. You need to
//    start blasting datagram packets as quickly as possible and capture
//    the results with Microsoft Network Monitor to find out which packets
//    got dropped.
//
// No command Line arguments
//    Sio_enable_circular_queueing
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock, create a socket, and set the ioctl. Afterwards
//    call the ioctl again to retrieve its value.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    int           ret;
    DWORD         dwBytesRet;
    BOOL          bOpt;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    // Creat a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    // Set the ioctl
	printf("Setting the ioctl...\n");
    bOpt = TRUE;
    ret = WSAIoctl(s, SIO_ENABLE_CIRCULAR_QUEUEING, &bOpt, sizeof(bOpt),
		NULL, 0, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf(" WSAIoctl(SIO_ENABLE_CIRCULAR_QUEUEING) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf(" WSAIoctl(SIO_ENABLE_CIRCULAR_QUEUEING) is OK!\n");

    // Get the ioctl
	printf("Getting the ioctl...\n");
    ret = WSAIoctl(s, SIO_ENABLE_CIRCULAR_QUEUEING, NULL, 0,
		&bOpt, sizeof(bOpt), &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf(" WSAIoctl(SIO_ENABLE_CIRCULAR_QUEUEING) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf(" WSAIoctl(SIO_ENABLE_CIRCULAR_QUEUEING) is OK!\n");

    if (bOpt == TRUE)
        printf("Circular queueing is TRUE!\n");
    else
        printf("Circular queueing is FALSE!\n");

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
