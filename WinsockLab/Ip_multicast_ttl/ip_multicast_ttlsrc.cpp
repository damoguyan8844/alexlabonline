// Description:
//    This is a simple multicast application which illustrates
//    the use of the IP_TTL option to modify the time-to-live
//    field of the IP header similar to the SIO_MULTIPOINT_SCOPE
//    ioctl does except this applies to anykind of IP traffic.
//
// Command Line Arguments/Parameters
//    ip_multicast_ttl [s|r] ttl
//      s        Sender
//      r        Receiver
//      ttl      Integer TTL value
//
// Vista issue: http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=329162
//
// A program that includes Winsock.h should only link with Wsock32.lib
//
// Link to ws2_32.lib
#include <winsock2.h>
// Windows Server 2003 and Windows XP
#include <Ws2tcpip.h>
#include <stdio.h>

#define MAX_BUF             64

// Change the IP accordingly for testing
// Multicast address range 224.0.0.0 - 239.255.255.255
// http://www.iana.org/assignments/multicast-addresses/
#define MULTICAST_IP       "192.168.1.1"
#define MULTICAST_PORT      24000

// Function: usage
// Description: Prints usage information
int usage(char *progname)
{
    printf("Usage: %s s|r ttl\n", progname);
    printf("          s = sender\n");
    printf("          r = receiver\n");
    printf("        ttl = multicast TTL value\n");
	printf("Example: %s s 100\n", progname);
	printf("Example: %s r 100\n", progname);
    return 0;
}

// Function: main
// Description:
//    Load Winsock, parse the arguments and start either the
//    multicast sender or receiver. Before sending data
//    set the IP_TTL to the specified value.
int main(int argc, char **argv)
{
    WSADATA        wsd;
    SOCKET         s;
	// struct ipv6_mreq for IPv6
    struct ip_mreq mcast; // IPv4
	// or better one, use struct ip_mreq_source/IP_MREQ_SOURCE/*PIP_MREQ_SOURCE
	// http://msdn.microsoft.com/en-us/library/ms738704(VS.85).aspx
	// Vista or later use GROUP_REQ and the GROUP_SOURCE_REQ structures
	//  which for both IPv6 and IPv4
    SOCKADDR_IN    local, from;
    int            ttl, ttlsz, fromsz, ret;
    char           databuf[MAX_BUF];
    BOOL           bReceive=FALSE;

    // Parse the command line
    if (argc < 2)
	{
		usage(argv[0]);
		exit(1);
	}

    if (tolower(argv[1][0]) == 's')
        bReceive = FALSE;
    else if (tolower(argv[1][0]) == 'r')
        bReceive = TRUE;
    else
        usage(argv[0]);

    ttl = atoi(argv[2]);

	// Load winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd))
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET)
    {
        printf("socket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("socket() is OK!\n");

    if (bReceive)
    {
        local.sin_family = AF_INET;
        local.sin_port = htons(MULTICAST_PORT);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(s, (SOCKADDR *)&local, sizeof(local)) == SOCKET_ERROR)
        {
            printf("bind() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
		else
			printf("bind() is OK!\n");
    }

    // Join the multicast group
	// For XP sp2 and Windows 2003 not needed in order to send,
	// only the receivers need to join a multicast group in order to receive
	if(bReceive)
	{
		mcast.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
		mcast.imr_interface.s_addr = INADDR_ANY;
		
		if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast)) == SOCKET_ERROR)
		{
			printf("setsockopt(IP_ADD_MEMBERSHIP) failed with error code %d\n", WSAGetLastError());
			return -1;
		}
		else
			printf("setsockopt(IP_ADD_MEMBERSHIP) is OK!\n");
	}

    // Set the TTL to our value
    ttlsz = sizeof(ttl);
    if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR)
    {
        printf("setsockopt(IP_MULTICAST_TTL) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("setsockopt(IP_MULTICAST_TTL) is OK!\n");

    // Verify
    if (getsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, &ttlsz) == SOCKET_ERROR)
    {
        printf("getsockopt(IP_MULTICAST_TTL) failed: %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("getsockopt(IP_MULTICAST_TTL) is OK!\n");

    printf("Multicast TTL is set to: %d\n", ttl);

    if (bReceive)
    {
        // Receive some data
        fromsz = sizeof(from);
        ret = recvfrom(s, databuf, MAX_BUF, 0, (SOCKADDR *)&from, &fromsz);
        if (ret == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
		else
			printf("recvfrom() is OK!\n");

        databuf[ret] = 0;
        printf("read: [%s] from [%s]\n", databuf, inet_ntoa(from.sin_addr));
    }
    else
    {
        // Send some data
        SOCKADDR_IN to;

        memset(databuf, '$', MAX_BUF);

        to.sin_family = AF_INET;
        to.sin_port = htons(MULTICAST_PORT);
        to.sin_addr.s_addr = inet_addr(MULTICAST_IP);

        ret = sendto(s, databuf, MAX_BUF-1, 0, (SOCKADDR *)&to, sizeof(to));

        if (ret == SOCKET_ERROR)
        {
            printf("sendto() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
		else
			printf("sendto() is OK!\n");

        printf("Sent %d bytes to %s\n", ret, inet_ntoa(to.sin_addr));
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
