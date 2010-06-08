// Description:
//    This sample shows how to use the SIO_ADDRESS_LIST_QUERY
//    ioctl. This is a Windows 2000 command which lists the 
//    local interfaces availability.
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description: Load Winsock, create a socket, and perform the interface query
int main(int argc, char **argv)
{
	WSADATA       wsd;
    SOCKET        s;
    DWORD         dwBytesRet=0;
    char          buf[1024];
    int           ret,
                  i;
    SOCKET_ADDRESS_LIST *slist;
    SOCKADDR_IN  *addr;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    // Get the address list
    ret = WSAIoctl(s, SIO_ADDRESS_LIST_QUERY, NULL, 0, buf, 1024, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) is OK!\n");

    printf("Bytes returned: %d bytes\n", dwBytesRet);

	// Print the results
    slist = (SOCKET_ADDRESS_LIST *)buf;
   
    printf("      Addr Count: %d\n", slist->iAddressCount);

    for(i=0; i < slist->iAddressCount ;i++)
    {
        addr = (SOCKADDR_IN *) slist->Address[i].lpSockaddr;
        printf("       Addr [%02d]: %s\n", i, inet_ntoa(addr->sin_addr));
        printf("Addr Length [%02d]: %d bytes\n", i, slist->Address[i].iSockaddrLength);
    }

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
