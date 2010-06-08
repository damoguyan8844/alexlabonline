// Description:
//    This is a trivial sample that sets the receive timeout
//    option and then attempts to receive a datagram which will
//    fail with a timeout message.
//
// No command line arguments
//
// Link to ws2_32.lib
#include <windows.h>
#include <winsock.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock, create a UDP socket, set the timeout value,
//    and then call recvfrom() will will fail with a timeout since
//    no data is being sent.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKADDR_IN   from;
    SOCKET        s;
    int           ret, iVal=0, fromsz;
    unsigned int  sz = sizeof(iVal);
    char          rcvbuf[1024];

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
	}
	else
		printf("WSAStartup() should be OK!\n");

	// Create a datagram socket
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
	{
		printf("socket() failed with error code %d\n", WSAGetLastError());
        return -1;
	}
	else
		printf("socket() is OK!\n");

    // Set the timeout value
    iVal = 1000;
    ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, sz);
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("setsockopt() is OK!\n");

    printf("Timeout was set...\n");

	// Retrieve the value just to be sure
	printf("Do a verification...\n");
    ret = getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, &sz);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("getsockopt() is fine!\n");

    printf("Timeout value == %d\n", iVal);

	// To test the receive function we need to bind first
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(5150);
   
    ret = bind(s, (SOCKADDR *)&from, sizeof(from));
    if (ret == SOCKET_ERROR)
    {
        printf("bind() failed: %d\n", WSAGetLastError());
        return -1;
    } 

    // Call receive which will fail with WSAETIMEDOUT
    fromsz = sizeof(from);
    ret = recvfrom(s, rcvbuf, 1024, 0, (SOCKADDR *)&from, &fromsz);
    if (ret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
            printf("recvfrom() failed with error code 10060 (WSAETIMEDOUT)\n");
        else
            printf("recvfrom() failed with error code %d\n", WSAGetLastError());
    }
	else
		printf("recvfrom() looks fine!\n");

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
