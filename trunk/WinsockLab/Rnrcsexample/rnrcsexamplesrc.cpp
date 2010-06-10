// Description:
//    This sample illustrates how a server can create a socket and
//    advertise the existance of the service to clients.  It also
//    shows how a client with knowledge of the server's name only
//    can find out how to connect to the server in order to transmit
//    data. The server accomplishes this by installing a service
//    class which describes the basic characteristics of the service
//    and then registering an instance of the server which references
//    the given class. The service can be registered for multiple
//    name spaces such as IPX and NTDS.
//
//    The client can query the available name spaces for knowledge
//    of a service of the given name. If the query completes 
//    successfully, the address of the service is returned.  All
//    the client needs to do is use that address in either a
//    connect or sendto call.
//
// Command line arguments
//     rnrcsexample -c:ServiceName -s:ServiceName -t:ID -n:NameSpace
//       -c:ServiceName      Act as client and query for this service name
//       -s:ServiceName      Act as server and register service as this name
//       -t:ID               SAP ID to register under
//       -n:NameSpace        Name space to register service under
//                             Supported namespaces are NS_ALL, NS_SAP, NS_NTDS
//
// Link to ws2_32.lib
#include <winsock2.h>
// windows.h should be after winsock2.h
#include <windows.h>
#include <ws2tcpip.h>
#include <svcguid.h>
#include <wsipx.h>
#include <wsnwlink.h> 
#include <nspapi.h>
#include <stdio.h>

#define MAX_NUM_CSADDRS        20
#define MAX_INTERFACE_LIST     20
#define MAX_NUM_SOCKETS        20
#define MAX_BUFFER_SZ         512
#define MAX_SERVER_NAME_SZ     64
#define MAX_GUID_SZ        256
#define DEFAULT_SERVER_NAME    "GedikServer"

char    szServerName[MAX_SERVER_NAME_SZ];   // Server name
BOOL    bServer;                            // Client or server?
DWORD   dwNameSpace,                        // Name space to register on
        dwSapId;                            // Used to create a GUID
SOCKET  socks[MAX_NUM_SOCKETS];             // Socket handles for our server
HANDLE  hEvents[MAX_NUM_SOCKETS];

BOOL  ClientSend(int socktype, int proto, SOCKADDR *server, int len, int count);
char *FormatIPXAddr(SOCKADDR_IPX *addr);

// Function: usage
// Description: Print usage information.
void usage(char *progname)
{
	printf("Usage: %s  -c:ServiceName -s:ServiceName  -t:ID -n:[NS_ALL|NS_NTDS|NS_SAP]\n", progname);
	printf("Server:\n");
    printf("  -s:ServiceName      Act as server and register service as this name\n");
    printf("  -t:ID               SAP ID to register under\n");
    printf("                        This is arbitrary, just needed to create a GUID\n");
    printf("  -n:NameSpace        Name space to register service under\n");
    printf("                        Can be one of the strings: NS_ALL, NS_NTDS, or NS_SAP\n");
	printf("Client:\n");
    printf("  -c:ServiceName      Act as client and query for this service name\n");
    ExitProcess(-1);
}

// Function: ValidateArgs
// Description:
//    Parse command line parameters and set some global variables used by the application.
void ValidateArgs(int argc, char **argv)
{
    int    i;

    // Set some default values
    strcpy_s(szServerName, sizeof(szServerName), DEFAULT_SERVER_NAME);
    dwNameSpace = NS_ALL;
    dwSapId = 200;

    for(i=0; i < MAX_NUM_SOCKETS ;i++)
        socks[i] = SOCKET_ERROR;

    for (i=1; i < argc ;i++) 
    {
        if ((argv[i][0] == '-') || (argv[i][0] == '/')) 
        {
            switch (tolower(argv[i][1])) 
            {
                case 't':        // SAP id used to generate GUID
                    if (strlen(argv[i]) > 3)
                        dwSapId = atoi(&argv[i][3]);
                    break;
                case 'c':        // Client, query for given service
                    bServer = FALSE;
                    if (strlen(argv[i]) > 3)
                        strcpy_s(szServerName, sizeof(szServerName), &argv[i][3]);
                    break;
                case 's':        // Server, register as the given service
                    bServer = TRUE;
                    if (strlen(argv[i]) > 3)
                        strcpy_s(szServerName, sizeof(szServerName), &argv[i][3]);
                    break;
                case 'n':        // Name spaces to register service under
                    if (!_strnicmp("NS_NTDS", &argv[i][3], strlen("NS_NTDS")))
                        dwNameSpace = NS_NTDS;
                    else if (!_strnicmp("NS_SAP", &argv[i][3], strlen("NS_SAP")))
                        dwNameSpace = NS_SAP;
                    break;
                default:
                    usage(argv[0]);
                    return;
            }
        }
        else
            usage(argv[0]);
    }
    return;
}

// Function: EnumNameSpaceProviders
// Description:
//    Returns an array of those name spaces which our application
//    supports. If one day, NS_DNS becomes dynamic, modify this
//    function to return that structure as well. 
WSANAMESPACE_INFO *EnumNameSpaceProviders(int *count)
{
    WSANAMESPACE_INFO *nsinfo, *nsinfocpy;
    BYTE              *pbuf=NULL, *pbufcpy=NULL;
	// WCHAR              szGuid[MAX_GUID_SZ];
    DWORD              dwBytes;
    int                i, j, ret;
	//int				 h;

    *count = 0;
    dwBytes = 0;

	// First find out how big of a buffer we need
    ret = WSAEnumNameSpaceProviders(&dwBytes, NULL);
    if (ret == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEFAULT)
        {
            printf("WSAEnumNameSpaceProviders() failed with error code %d\n", WSAGetLastError());
            return NULL;
        }
    }
	else
		printf("WSAEnumNameSpaceProviders() should be fine!\n");

    // Allocate this buffer and call the function again
    pbuf = (BYTE *)GlobalAlloc(GPTR, dwBytes);
    if (!pbuf)
    {
        printf("GlobaAlloc() for pbuf buffer failed with error code %d\n", GetLastError());
        return NULL;
    }
	else
		printf("GlobaAlloc() for pbuf buffer is OK!\n");


    nsinfo = (WSANAMESPACE_INFO *)pbuf;
    ret = WSAEnumNameSpaceProviders(&dwBytes, nsinfo);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAEnumNameSpaceProviders() failed with error code %d\n", WSAGetLastError());
        HeapFree(GetProcessHeap(), 0, pbuf);
        return NULL;
    }
	else
		printf("WSAEnumNameSpaceProviders() is OK!\n");

    // Make a copy buffer which we will return our data in
    pbufcpy = GlobalAlloc(GPTR, dwBytes);
    if (!pbufcpy)
    {
        printf("GlobaAlloc() for copy buffer failed with error code %d\n", GetLastError());
        return NULL;
    }
	else
		printf("GlobaAlloc() for copy buffer is OK!\n");

    // Just for informational
    /*for(h=0; h < ret ;h++)
    {
        printf("\nName Space: %S\n", nsinfo[h].lpszIdentifier);
        printf("        ID: ");
        switch (nsinfo[h].dwNameSpace)
        {
            case NS_ALL:
                printf("NS_ALL\n");
                break;
            case NS_SAP:
                printf("NS_SAP\n");
                break;
            case NS_NDS:
                printf("NS_NDS\n");
                break;
            case NS_PEER_BROWSE:
                printf("NS_PEER_BROWSE\n");
                break;
            case NS_TCPIP_LOCAL:
                printf("NS_TCPIP_LOCAL\n");
                break;
            case NS_TCPIP_HOSTS:
                printf("NS_TCPIP_HOSTS\n");
                break;
            case NS_DNS:
                printf("NS_DNS\n");
                break;
            case NS_NETBT:
                printf("NS_NETBT\n");
                break;
            case NS_WINS:
                printf("NS_WINS\n");
                break;
            case NS_NBP:
                printf("NS_NBP\n");
                break;
            case NS_MS:
                printf("NS_MS\n");
                break;
            case NS_STDA:
                printf("NS_STDA\n");
                break;
            case NS_NTDS:
                printf("NS_NTDS\n");
                break;
            case NS_X500:
                printf("NS_X500\n");
                break;
            case NS_NIS:
                printf("NS_NIS\n");
                break;
            case NS_NISPLUS:
                printf("NS_NISPLUS\n");
                break;
            case NS_WRQ:
                printf("NS_WRQ\n");
                break;
            default:
                printf("%d\n", nsinfo[h].dwNameSpace);
        }

        StringFromGUID2(&nsinfo[h].NSProviderId, szGuid, MAX_GUID_SZ);
        printf("      GUID: %S\n", szGuid);
        printf("    Active: %s\n", ((nsinfo[h].fActive) ? "YES" : "NO"));
        printf("   Version: %d\n", nsinfo[h].dwVersion);
    }*/

    // Loop through the returned name space structures picking
    // those which our app supports
    nsinfocpy = (WSANAMESPACE_INFO *)pbufcpy;
    printf("Well, %d name spaces are available\n", ret);

    j = 0;
    for(i=0; i < ret ;i++)
    {
        switch (nsinfo[i].dwNameSpace)
        {
            // Currently we only support SAP or NTDS name spaces
            // so ignore anything else (like DNS)
            case NS_SAP:
            case NS_NTDS: 
                memcpy(&nsinfocpy[j++], &nsinfo[i], sizeof(WSANAMESPACE_INFO));
                break;
        }
    }

    // Free the original buffer and return our copy of useable name spaces
    GlobalFree(pbuf);
    printf("Found %d useable name spaces\n", j);
    *count = j;
    return nsinfocpy;
}

// Function: InstallServiceClass
// Description:
//    This function installs the service class which is required before
//    registering an instance of a service. A service class defines the
//    generic attributes of a class of services such as whether it is
//    connection oriented or not as well as what protocols is supports
//    (IPX, IP, etc).
BOOL InstallServiceClass(GUID *svcguid, WSANAMESPACE_INFO *nsinfo, int nscount)
{
    WSASERVICECLASSINFO  sci;
    WSANSCLASSINFO      *nsclass=NULL;
    DWORD                dwZero=0, dwUdpPort=0;
    char                 szServiceClassName[64];
    int                  i, ret;

    // Generate a name of our service class
    wsprintf((LPWSTR)szServiceClassName, L"ServiceClass: %003d", dwSapId);
    printf("Installing service class: '%S'\n", szServiceClassName);

	// There are 2 attributes we need to set for every name space
    // we want to register in: Connection Oriented/Connectionless as
    // well as the address family of the protocols we support.
    nsclass = GlobalAlloc(GPTR, sizeof(WSANSCLASSINFO) * nscount * 2);
    if (!nsclass)
    {
        printf("GlobalAlloc() for WSANSCLASSINFO failed with error code %d\n", GetLastError());
        return FALSE;
    }
	else
		printf("GlobalAlloc() for WSANSCLASSINFO should be fine!\n");

    // Initialize the structure
    memset(&sci, 0, sizeof(sci));
    sci.lpServiceClassId = svcguid;
    sci.lpszServiceClassName = (LPWSTR)szServiceClassName;
    sci.dwCount = nscount * 2;
    sci.lpClassInfos = nsclass;

    for(i=0; i < nscount ;i++)
    {
        // No matter what name space we use we set the connection
        // oriented attribute to false (i.e. connectionless)
        nsclass[i*2].lpszName = SERVICE_TYPE_VALUE_CONN;
        nsclass[i*2].dwNameSpace = nsinfo[i].dwNameSpace;
        nsclass[i*2].dwValueType = REG_DWORD;
        nsclass[i*2].dwValueSize = sizeof(DWORD);
        nsclass[i*2].lpValue = &dwZero;

        if (nsinfo[i].dwNameSpace == NS_NTDS)
        {
            // If NS_NTDS is available we will be running a UDP
            // based service on the given port number
            printf("Setting NS_NTDS info...\n");
            nsclass[(i*2)+1].lpszName = SERVICE_TYPE_VALUE_UDPPORT;
            nsclass[(i*2)+1].dwNameSpace = nsinfo[i].dwNameSpace;
            nsclass[(i*2)+1].dwValueType = REG_DWORD;
            nsclass[(i*2)+1].dwValueSize = sizeof(DWORD);
            nsclass[(i*2)+1].lpValue = &dwUdpPort;            
        }
        else if (nsinfo[i].dwNameSpace == NS_SAP)
        {
            // If NS_SAP is available we will run an IPX based
            // service on the given SAP ID
            printf("Setting NS_SAP info...\n");
            nsclass[(i*2)+1].lpszName = SERVICE_TYPE_VALUE_SAPID;
            nsclass[(i*2)+1].dwNameSpace = nsinfo[i].dwNameSpace;
            nsclass[(i*2)+1].dwValueType = REG_DWORD;
            nsclass[(i*2)+1].dwValueSize = sizeof(DWORD);
            nsclass[(i*2)+1].lpValue = &dwSapId;            
        }
    }

    // Install the service class
    ret = WSAInstallServiceClass(&sci);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAInstallServiceClass() failed with error code %d\n", WSAGetLastError());
        return FALSE;
    }
	else
		printf("WSAInstallServiceClass() is OK!\n");

    GlobalFree(nsclass);

    return TRUE;
}

// Function: GetIPInterfaceList
// Description:
//    This function returns an array of SOCKADDR_IN structures,
//    one for every local IP interface on the machine. We use
//    the ioctl command SIO_GET_INTERFACE_LIST to do this although
//    we could have used any number of method such as gethostbyname(),
//    SIO_INTERFACE_LIST_QUERY, or the IP helper APIs.
SOCKADDR_IN *GetIPInterfaceList(SOCKET s, int *count)
{
    static SOCKADDR_IN  sa_in[MAX_NUM_CSADDRS];
    INTERFACE_INFO      iflist[MAX_INTERFACE_LIST];
    DWORD               dwBytes;
    int                 ret, i;

    *count = 0;

    ret = WSAIoctl(s, SIO_GET_INTERFACE_LIST, NULL, 0, &iflist, sizeof(iflist), &dwBytes, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_GET_INTERFACE_LIST) failed with error code %d\n", WSAGetLastError());
        return NULL;
    }
	else
		printf("WSAIoctl(SIO_GET_INTERFACE_LIST) is OK!\n");

    // Loop through the interfaces and copy them into the SOCKADDR_IN array
    *count = dwBytes / sizeof(INTERFACE_INFO); 
    for(i=0; i < *count ;i++)
    {
        memcpy(&sa_in[i], &iflist[i].iiAddress.AddressIn, sizeof(SOCKADDR_IN));
    }
    
    return sa_in;
}

// Function: GetIPXInterfaceList
// Description:
//    This function enumerates the local IPX interfaces and returns
//    them in an array of SOCKADDR_IPX structures. The only way to
//    do this is to use the socket option IPX_MAX_ADAPTER_NUM to
//    find out how many interfaces there are and then call 
//    IPX_ADDRESS on each one to find the address.
SOCKADDR_IPX *GetIPXInterfaceList(SOCKET s, int *count)
{
    static SOCKADDR_IPX sa_ipx[MAX_NUM_CSADDRS];
    IPX_ADDRESS_DATA    ipxdata;
    int                 ifcount, nSize, ret, i;
    
    *count = 0;
    ifcount = 0;

	// Find out how many IPX interfaces are available
    nSize = sizeof(ifcount);
    ret = getsockopt(s, NSPROTO_IPX, IPX_MAX_ADAPTER_NUM, (char *)&ifcount, &nSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(IPX_MAX_ADAPTER_NUM) failed with error code %d\n", WSAGetLastError());
        return NULL;
    }
	else
		printf("getsockopt(IPX_MAX_ADAPTER_NUM) is OK!\n");

    // Loop through and get the address of each interface
    for(i=0; i < ifcount ;i++)
    {
        nSize = sizeof(ipxdata);

        memset(&ipxdata, 0, sizeof(ipxdata));
        ipxdata.adapternum = i;
        ret = getsockopt(s, NSPROTO_IPX, IPX_ADDRESS, (char *)&ipxdata, &nSize);
        if (ret == SOCKET_ERROR)
        {
            printf("getsockopt(IPX_ADDRESS) failed with error code %d\n", WSAGetLastError());
            continue;
        }

        // Make sure that this interface is actually up and running
        if (ipxdata.status == TRUE)
        {
            memcpy(&sa_ipx[i].sa_netnum, &ipxdata.netnum, sizeof(ipxdata.netnum));
            memcpy(&sa_ipx[i].sa_nodenum, &ipxdata.nodenum, sizeof(ipxdata.nodenum));
        }
    }

    *count = ifcount;    
    return sa_ipx;
}

// Function: Advertise
// Description:
//    This function advertises an instance of the server. This 
//    function also creates the server for each available name
//    space. To advertise you need all the local interfaces that
//    the client can connect the to the server.  This is done
//    by filling out a WSAQUERYSET structure along with the
//    appropriate CSADDR_INFO structures.  The CSADDR_INFO
//    structures define the interfaces the service is listening on.
BOOL Advertise(GUID *guid, WSANAMESPACE_INFO *nsinfo, int nscount, char *servicename)
{
    WSAQUERYSET     qs;
    CSADDR_INFO     csaddrs[MAX_NUM_CSADDRS];
    int             ret,i, j, iSize, addrcnt;

    // Initialize the WSAQUERYSET structure
    memset(&qs, 0, sizeof(WSAQUERYSET));

    qs.dwSize = sizeof(WSAQUERYSET);
    qs.lpszServiceInstanceName = (LPWSTR)servicename;
    qs.lpServiceClassId = guid;
    qs.dwNameSpace = NS_ALL;
    qs.lpNSProviderId = NULL;
    qs.lpcsaBuffer = csaddrs;
    qs.lpBlob = NULL;

    addrcnt=0;

	// For each valid name space we create an instance of the
    // service and find out what local interfaces are available
    // that the client can connect to and communicate with the server.
    for (i=0; i < nscount ;i++)
    {
        if (nsinfo[i].dwNameSpace == NS_NTDS)
        {
            SOCKADDR_IN     localip;
            SOCKADDR_IN    *iflist=NULL;
            int             ipifcount;

            // Create a UDP based server
            printf("Setting up NTDS entry...\n");
            socks[i] = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
            if (socks[i] == INVALID_SOCKET)
            {
                printf("WSASocket() failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("WSASocket() is fine!\n");

            localip.sin_family = AF_INET;
            localip.sin_port = htons(0);
            localip.sin_addr.s_addr = htonl(INADDR_ANY);
 
            ret = bind(socks[i], (SOCKADDR *)&localip, sizeof(localip));
            if (ret == SOCKET_ERROR)
            {
                printf("bind() failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("bind() should be fine lol!\n");
			
			// Get the port number back since we don't specifically give one
            iSize = sizeof(localip);
            ret = getsockname(socks[i], (SOCKADDR *)&localip, &iSize);
            if (ret == SOCKET_ERROR)
            {
                printf("getsockname(IP) failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("getsockname(IP) is pretty fine!\n");

            // Get a list of the IP interfaces   
            iflist = GetIPInterfaceList(socks[i], &ipifcount);
            if (!iflist)
            {
                printf("Unable to enumerate IP interfaces!\n");
                return FALSE;
            }
			else
				printf("Enumerate IP interfaces is OK!\n");

            // Fill out the CSADDR_INFO structures with each IP interface
            for (j=0; j < ipifcount ;j++)
            {
				iflist[j].sin_family = AF_INET;
                iflist[j].sin_port = localip.sin_port;

                csaddrs[addrcnt].iSocketType = SOCK_DGRAM;
                csaddrs[addrcnt].iProtocol = IPPROTO_UDP;
                csaddrs[addrcnt].LocalAddr.lpSockaddr = (SOCKADDR *)&iflist[j];
                csaddrs[addrcnt].LocalAddr.iSockaddrLength = sizeof(iflist[j]);
                csaddrs[addrcnt].RemoteAddr.lpSockaddr = (SOCKADDR *)&iflist[j];
                csaddrs[addrcnt].RemoteAddr.iSockaddrLength = sizeof(iflist[j]);

                printf("\t[%d] Local  IP [%s:%d]\n", j,
                    inet_ntoa(((SOCKADDR_IN *)(csaddrs[addrcnt].LocalAddr.lpSockaddr))->sin_addr),
                    ntohs(((SOCKADDR_IN *)(csaddrs[addrcnt].LocalAddr.lpSockaddr))->sin_port));

                printf("\t[%d] Remote IP [%s:%d]\n", j,
                    inet_ntoa(((SOCKADDR_IN *)(csaddrs[addrcnt].RemoteAddr.lpSockaddr))->sin_addr),
                    ntohs(((SOCKADDR_IN *)(csaddrs[addrcnt].RemoteAddr.lpSockaddr))->sin_port));

                addrcnt++;
            }
        }
        else if (nsinfo[i].dwNameSpace == NS_SAP)
        {
            SOCKADDR_IPX        localipx, *ipxlist;
            int                 ipxifcount;

            // Create an intance of our IPX server
            printf("Setting up IPX entry...\n");
            socks[i] = WSASocket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX, NULL, 0, WSA_FLAG_OVERLAPPED);
            if (socks[i] == INVALID_SOCKET)
            {
                printf("WSASocket() failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("WSASocket() is OK!\n");

            memset(&localipx, 0, sizeof(SOCKADDR_IPX));
            localipx.sa_family = AF_IPX;
             
            ret = bind(socks[i], (SOCKADDR *)&localipx, sizeof(localipx));
            if (ret == SOCKET_ERROR)
            {
                printf("bind() failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("bind() is pretty fine!\n");

            // Find out the socket number that the IPX server is listening
            // on since we didn't explicitly specify one.
            iSize = sizeof(localipx);
            ret = getsockname(socks[i], (SOCKADDR *)&localipx, &iSize);
            if (ret == SOCKET_ERROR)
            {
                printf("getsockname(IPX) failed with error code %d\n", WSAGetLastError());
                return FALSE;
            }
			else
				printf("getsockname(IPX) is OK!\n");

            // Enumerate the local IPX interfaces on which clients can connect to us
            ipxlist = GetIPXInterfaceList(socks[i], &ipxifcount);
            if (!ipxlist)
            {
                printf("Unable to get the IPX interface list!\n");
                return FALSE;
            }
			else
				printf("Getting the IPX interface list is OK!\n");


            printf("Number IPX interfaces: %d\n", ipxifcount);
            for(j=0; j < ipxifcount ;j++)
            {
				ipxlist[j].sa_family = AF_IPX;
                ipxlist[j].sa_socket = localipx.sa_socket;

                csaddrs[addrcnt].iSocketType = SOCK_DGRAM;
                csaddrs[addrcnt].iProtocol = NSPROTO_IPX;
                csaddrs[addrcnt].LocalAddr.lpSockaddr = (SOCKADDR *)&ipxlist[j];
                csaddrs[addrcnt].LocalAddr.iSockaddrLength = sizeof(ipxlist[j]);
                csaddrs[addrcnt].RemoteAddr.lpSockaddr = (SOCKADDR *)&ipxlist[j];
                csaddrs[addrcnt].RemoteAddr.iSockaddrLength = sizeof(ipxlist[j]);

                printf("\t[%d] Local  IPX [%S]\n", j, FormatIPXAddr((SOCKADDR_IPX *)(csaddrs[addrcnt].LocalAddr.lpSockaddr)));
                printf("\t[%d] Remote IPX [%S]\n", j, FormatIPXAddr((SOCKADDR_IPX *)(csaddrs[addrcnt].RemoteAddr.lpSockaddr)));

                addrcnt++;
            }
        }

        // Create an event for the server to use with WSAEventSelect
        hEvents[i] = WSACreateEvent();
        if (hEvents[i] == WSA_INVALID_EVENT)
        {
            printf("WSACreateEvent() failed with error code %d\n", WSAGetLastError());
            return FALSE;
        }
		else
			printf("WSACreateEvent() is pretty fine!\n");
    }

    qs.dwNumberOfCsAddrs = addrcnt;

    // Register our service(s)
    ret = WSASetService(&qs, RNRSERVICE_REGISTER, 0L);
    if (ret == SOCKET_ERROR)
    {
        printf("WSASetService() failed with error code %d\n", WSAGetLastError());
        return FALSE;
    }

    printf("WSASetService() succeeded!!!\n");
    return TRUE;
}

// Function: LookupService
// Description:
//    This function queries for an instance of the given service
//    running on the network. You can either query for a specific
//    service name or specify the wildcard string "*". If an instance
//    is found, send some data to it.
void LookupService(GUID *guid, int sapid, int ns, char *servername)
{
	WSAQUERYSET  qs, *pqs = NULL;
    AFPROTOCOLS  afp[2] = {{AF_IPX,  NSPROTO_IPX}, {AF_INET, IPPROTO_UDP}}, afpx[1] = {{AF_IPX, NSPROTO_IPX}};
    char         querybuf[sizeof(WSAQUERYSET) + 4096];
    DWORD        nSize = sizeof(WSAQUERYSET) + 4096, i, j = 0, ret;
    HANDLE       hLookup;

    // Initialize the WSAQUERYSET structure
    memset(&qs, 0, sizeof(WSAQUERYSET));

    qs.dwSize = sizeof(WSAQUERYSET);
    qs.lpszServiceInstanceName = (LPWSTR)servername;
    qs.lpServiceClassId = guid;
    qs.dwNameSpace = NS_SAP;
    qs.dwNumberOfProtocols = 1; 
    qs.lpafpProtocols = afpx;

    // Begin the lookup. We want the name and address back
    ret = WSALookupServiceBegin(&qs, LUP_RETURN_ADDR | LUP_RETURN_NAME, &hLookup);

	  if (ret != SOCKET_ERROR)
	  {
		  printf("WSALookupServiceBegin() is pretty fine!\n");
		  
		  // This is quite wierd for Win 2000		  
  		  pqs = (WSAQUERYSET *) malloc(sizeof(querybuf));
		  nSize = sizeof(WSAQUERYSET) + 4096;
		  memset(querybuf, 0, nSize);
  
		  do
		  {
			  if (WSALookupServiceNext(hLookup, 0, &nSize, pqs) != 0)
			  {
				  ret = WSAGetLastError();
				  printf(" WSALookupServiceNext() #%d return value is = %d\n", j, ret);
			  }
			  else
			  {
				  printf("WSALookupServiceNext() #%d is OK!\n", j);
				  ret = 0;
			  }
			  j++;
			  
			  if (ret == WSAEFAULT)
			  {
				  if (pqs)
					  free(pqs);
				  pqs = (WSAQUERYSET *) malloc(nSize);
				  if (!pqs)
				  {
					  printf("Could not allocate memory: %d\n", GetLastError());
					  break;
				  }
				  else
				  {
					  printf("Memory allocation should be fine for pqs\n");
				  }
			  }
			  
			  // Some testing...look like failed lor, wierd..wierd
			  if ((ret == 0) && (pqs))
				  printf("Service instance name is %S\n", pqs->lpszServiceInstanceName);
		  } while ((ret != WSA_E_NO_MORE) && (ret != WSAENOMORE));
	  }
	  else
	  {
		  printf("Error on WSALookupServiceBegin() %d\n", WSAGetLastError());
	  }
	  
	  // This part also wierdddddddddddddd....
	  // First attempt for WSALookupServiceNext() already failed? :-(
	  if(pqs->lpszServiceInstanceName != NULL)
	  {
		  printf("pqs->lpszServiceInstanceName is not NULL\n");
		  // Assuming that we've found a server out there, print some info and
		  // send some data to it
		  printf("Found service: %s\n", pqs->lpszServiceInstanceName);
		  printf("Returned %d CSADDR structures\n", pqs->dwNumberOfCsAddrs);
		  
		  for(i=0; i < pqs->dwNumberOfCsAddrs ;i++)
		  {
			  switch (pqs->lpcsaBuffer[i].iProtocol)
			  {
			  case IPPROTO_UDP:
				  printf("IPPROTO_UDP\n");
				  ((SOCKADDR_IN *)pqs->lpcsaBuffer[i].RemoteAddr.lpSockaddr)->sin_family = AF_INET;
				  break;
			  case NSPROTO_IPX:
				  printf("NSPROTO_IPX\n");
				  ((SOCKADDR_IPX *)pqs->lpcsaBuffer[i].RemoteAddr.lpSockaddr)->sa_family = AF_IPX;
				  printf("\t[%d] Local  IPX [%s]\n", i, FormatIPXAddr((SOCKADDR_IPX *)(pqs->lpcsaBuffer[i].LocalAddr.lpSockaddr)));
				  printf("\t[%d] Remote IPX [%s]\n", i, FormatIPXAddr((SOCKADDR_IPX *)(pqs->lpcsaBuffer[i].RemoteAddr.lpSockaddr)));
				  break;
			  default:
				  printf("Unknown!\n");
				  break;
			  }
			  
			  // Send data
			  ClientSend(pqs->lpcsaBuffer[i].iSocketType, pqs->lpcsaBuffer[i].iProtocol,
				  (SOCKADDR *)pqs->lpcsaBuffer[i].RemoteAddr.lpSockaddr,
				  pqs->lpcsaBuffer[i].RemoteAddr.iSockaddrLength, 4);
		  }
	  }
	  else
	  {
		  printf("\npqs->lpszServiceInstanceName is NULL\n");
		  printf("Found service: %s\n", qs.lpszServiceInstanceName);
		  printf("Returned %d CSADDR structures\n", qs.dwNumberOfCsAddrs);

		  // More action here!	  
	  }
	  
	  free(pqs);
	  WSALookupServiceEnd(hLookup);
	  return;
}

// Function: ClientSend
// Description: Create a socket of the given type and send some data to it
BOOL ClientSend(int socktype, int proto, SOCKADDR *server, int len, int count)
{
    WSABUF   wbuf;
    char     sndbuf[MAX_BUFFER_SZ];
    DWORD    dwBytesSent;
    int      i, ret;
  
    if (proto == NSPROTO_IPX)
    {
        socks[0] = WSASocket(AF_IPX, socktype, proto, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (socks[0] == INVALID_SOCKET)
        {
            printf("WSASocket() failed with error code %d\n", WSAGetLastError());
            return FALSE;
        }
		else
			printf("WSASocket() is OK!\n");
    }
    else if (proto == IPPROTO_UDP)
    {
        socks[0] = WSASocket(AF_INET, socktype, proto, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (socks[0] == INVALID_SOCKET)
        {
            printf("WSASocket() failed with error code %d\n", WSAGetLastError());
            return FALSE;
        }
		else
			printf("WSASocket() is OK!\n");
    }

    memset(sndbuf, '$', MAX_BUFFER_SZ);
    for(i=0; i < count ;i++)
    {
        wbuf.buf = sndbuf;
        wbuf.len = MAX_BUFFER_SZ -1;

        ret = WSASendTo(socks[0], &wbuf, 1, &dwBytesSent, 0, server, len , NULL, NULL);
        if (ret == SOCKET_ERROR)
        {
            printf("WSASendTo() failed with error code %d\n", WSAGetLastError());
            closesocket(socks[0]);
            return FALSE;
        }
		else
			printf("WSASendTo() looks OK!\n");
    }
    closesocket(socks[0]);

    return TRUE;
}

// Function: ServerRecv
// Description: Read data pending on the given socket.
BOOL ServerRecv(int index)
{
    WSABUF   wbuf;
    char     rcvbuf[MAX_BUFFER_SZ];
    SOCKADDR from;
    DWORD    dwBytesRecv,
             dwFlags,
             dwFromLen;
    int      ret;

    wbuf.buf = rcvbuf;
    dwFlags = 0;
    dwFromLen = sizeof(from);
    wbuf.len = MAX_BUFFER_SZ-1;
        
    ret = WSARecvFrom(socks[index], &wbuf, 1, &dwBytesRecv, &dwFlags, &from, &dwFromLen, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
            return TRUE;
        else
        {
            printf("WSARecvFrom() failed with error code %d\n", WSAGetLastError());
            return FALSE;
        }
    }
	else
		printf("WSARecvFrom() should be fine!\n");

    rcvbuf[dwBytesRecv] = 0;
    printf("Read: [%s]\n", rcvbuf);

    return TRUE;
}

// Function: FormatIPXAddr
// Description:
//    Print an IPX address to stdout. We don't use the function
//    WSAAddressToString because it doesn't support IPX.
char *FormatIPXAddr(SOCKADDR_IPX *addr)
{
    static char dest[128];
    wsprintf((LPWSTR)dest, L"%02X%02X%02X%02X.%02X%02X%02X%02X%02X%02X:%04X", 
        (unsigned char)addr->sa_netnum[0],
        (unsigned char)addr->sa_netnum[1],
        (unsigned char)addr->sa_netnum[2],
        (unsigned char)addr->sa_netnum[3],
        (unsigned char)addr->sa_nodenum[0],
        (unsigned char)addr->sa_nodenum[1],
        (unsigned char)addr->sa_nodenum[2],
        (unsigned char)addr->sa_nodenum[3],
        (unsigned char)addr->sa_nodenum[4],
        (unsigned char)addr->sa_nodenum[5],
        ntohs(addr->sa_socket));
    return dest;
}

// Function: main
// Description:
//    Initialize Winsock, parse the arguments, and start either the
//    client or server depending on the arguments.
int main(int argc, char **argv)
{
    WSANAMESPACE_INFO *nsinfo=NULL;
    WSADATA            wsd;
    GUID               svcguid;
    int                nscount, ret, i;
    WCHAR              szTemp[256];

	if(argc < 2)
	{
		usage(argv[0]);
		exit(1);
	}

    ValidateArgs(argc, argv);

    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is pretty fine!\n");

    // Create the GUID that everyone will use, client and server
    SET_NETWARE_SVCID(&svcguid, dwSapId); 

    StringFromGUID2(&svcguid, szTemp, 256);
    printf("GUID: [%S]\n", szTemp);

	// Enumerate the name spaces that we can use
    nsinfo = EnumNameSpaceProviders(&nscount);
    if (!nsinfo)
    {
        printf("unable to enumerate name space info!\n");
        return -1;
    }

    for(i=0; i < nscount ;i++)
        printf("Found NS: %S\n", nsinfo[i].lpszIdentifier);
    
    // If server
	if (bServer)
    {
        if (szServerName[0] == '*')
        {
            printf("You must specify a server name!\n");
            usage(argv[0]);
            return -1;
        }
		else
			printf("Server name provided!\n");

        // Install the service class
        if (!InstallServiceClass(&svcguid, nsinfo, nscount))
        {
            printf("Unable to install service class!\n");
            return -1;
        }
		else
			printf("Service class installed successfully!\n");

        // Advertise our service
        if (!Advertise(&svcguid, nsinfo, nscount, szServerName))
        {
            printf("Unable to advertise service!\n");
            return -1;
        }
		else
			printf("Service advertised successfully!\n");

        // Register our services for the read event
        for(i=0; i < nscount ;i++)
        {
            ret = WSAEventSelect(socks[i], hEvents[i], FD_READ);
            if (ret == SOCKET_ERROR)
            {
                printf("WSAEventSelect() failed with error code: %d\n", WSAGetLastError());
                return -1;
            }
			else
				printf("WSAEventSelect() is OK!\n");
        }

        while (1)
        {
            // Read any incoming data
            ret = WSAWaitForMultipleEvents(nscount, hEvents, FALSE, INFINITE, TRUE);
            ServerRecv(ret - WSA_WAIT_EVENT_0);
        }
    }
    else // If client
    {
        // Lookup the service
        LookupService(&svcguid, dwSapId, dwNameSpace, szServerName);
    }

    HeapFree(GetProcessHeap(), 0, nsinfo);
    WSACleanup();
    return 0;
}
