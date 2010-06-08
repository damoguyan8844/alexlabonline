// Description:
//    This module illustrates the SO_ACCEPTCONN option to find 
//    out whether a socket is listening for connections.
//
// No command line arguments
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <stdio.h>
//
// Function: main
// Description:
//    Loads Winsock, creates a listening socket and get the state of SO_ACCEPTCONN.
//
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   local;
    int           ret, iSize;
    BOOL          bOpt;

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    // Creat a socket, bind to it, and listen
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(5150);

    iSize = sizeof(bOpt);
    ret = getsockopt(s, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bOpt, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_ACCEPTCONN) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("getsockopt(SO_ACCEPTCONN) is OK!\n");


    if (bOpt == TRUE)
        printf("getsockopt(SO_ACCEPTCONN) returned TRUE!\n");
    else
        printf("getsockopt(SO_ACCEPTCONN) returned FALSE!\n");


    if (bind(s, (SOCKADDR *)&local, sizeof(local)) == SOCKET_ERROR)
    {
        printf("bind() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("bind() is OK!\n");

    listen(s, 7);

	// Get the option value
    iSize = sizeof(bOpt);
    ret = getsockopt(s, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bOpt, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_ACCEPTCONN) failed with error code %d\n", WSAGetLastError());
        return -1;
    }

    if (bOpt == TRUE)
        printf("getsockopt(SO_ACCEPTCONN) returned TRUE!\n");
    else
        printf("getsockopt(SO_ACCEPTCONN) returned FALSE!\n");

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
