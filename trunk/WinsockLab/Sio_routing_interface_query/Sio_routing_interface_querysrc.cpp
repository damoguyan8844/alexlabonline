// Description:
//    This sample illustrates how to use the SIO_ROUTING_INTERFACE_QUERY
//    option. This takes a destination IP address and port and calls
//    the interface query to determine which local IP interface should
//    be used to transmit the data on. This is a Windows 2000 specific
//    option.
//
// Command Line Arguments/Parameters
//    Sio_routing_interface_query Remote-IP Remote-Port
//      Remote-IP      Address of remote machine interested in
//      Remote-Port    Port of remote machine interested in
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock and parse the command line. Then create a
//    socket and perform the routing query.  Print out the
//    route that should be taken.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   input, *lpIf=NULL;
    DWORD         dwBytesRet=0, nRet, i;
    char          buf[1024];
    int           ret;

    // Parse the command line and load Winsock
    if (argc != 3)
    {
        printf("Usage: %s remote-ip remote-port\n", argv[0]);
		printf("Example: %s 209.131.36.158 80\n", argv[0]);
        return -1;
    }

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
		printf("WSASocket() is fine!\n");

    // Perform the query on the given IP address
    input.sin_family = AF_INET;
    input.sin_addr.s_addr = inet_addr(argv[1]);
    input.sin_port = htons((short)atoi(argv[2]));

    ret = WSAIoctl(s, SIO_ROUTING_INTERFACE_QUERY, &input, 
            sizeof(input), buf, 1024, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_ROUTING_INTERFACE_QUERY) failed: %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAIoctl(SIO_ROUTING_INTERFACE_QUERY) is fine!\n");

    printf("bytes returned: %d\n", dwBytesRet);
   
    nRet = dwBytesRet / sizeof(SOCKADDR_IN);
    lpIf = (SOCKADDR_IN *)buf;

	// Print out the interface information
    for(i=0; i < nRet ;i++)
    {
        printf("Interface:\n   sin_family = %d\n   sin_addr   = %s\n   sin_port   = %d\n",
            lpIf[i].sin_family, inet_ntoa(lpIf[i].sin_addr), lpIf[i].sin_port);
    }
   
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
