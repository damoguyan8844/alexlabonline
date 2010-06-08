// Description:
//    This sample sets the Type-of-Service bit and sends
//    some datagrams. To really see the results of this
//    option you need to capture the packets with Microsoft
//    Network Monitor (netmon) to view the actual packets.
//
// Command Line Arguments/Parameters
//    Ip_tos ip
//		ip	IP Address
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Function: main
// Description:
//    Load Winsock, create a datagram socket, set the IP_TOS
//    option and send some packets.
int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   remote;
    char         *str="This is a test string from sender!";
    int           ret, iTos;

	if(argc < 2)
	{
		printf("Usage: %s <IP_Address_to_send>\n", argv[0]);
		printf("Example %s 74.125.45.100\n", argv[0]);
		exit(1);
	}

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is preety fine!\n");

	// Create a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0,WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSASocket() is OK!\n");

	// Set the TOS bit
    iTos = 1;
    ret = setsockopt(s, IPPROTO_IP, IP_TOS, (char *)&iTos, sizeof(iTos));
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt(IP_TOS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("setsockopt(IP_TOS) should be OK!\n");

	// Perform a sendto()
	printf("Sending all my love....\n");
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(argv[1]);
    remote.sin_port = htons(5150);

    ret = sendto(s, str, strlen(str), 0, (SOCKADDR *)&remote, sizeof(remote));
    if (ret == SOCKET_ERROR)
    {
        printf("sendto() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("sendto() is OK!\n");

    printf("Wrote %d bytes...\n", ret);

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
