// Description:
//    This sample illustrates how to use the WSALookupServiceXXX
//    functions with the DNS name space. Because the DNS name
//    space is static, we can't dynamically register our services
//    with it; however, we can query it using these functions.
//    This sample shows how to do this using the special GUID
//    which corresponds to the gethostbyname() function.
//
// Command line argument
//    Dnsquery hostname
//      hostname       Name of host to resolve
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SvcGuid.h>
#include <stdio.h>

#define RNR_BUFFER_SIZE (sizeof(WSAQUERYSET) + sizeof(struct hostent) + 1024)

// Function: FixList
// Description:
//    This function takes the array of points within the HOSTENT
//    BLOB returned from the WSALookupServiceNext function and
//    corrects them. When the BLOB is returned the pointers
//    are actually offsets from the start of the HOSTENT 
//    structure. We need to follow the array of pointers and
//    make them absolute addresses until we hit the array
//    entry with a zero in it.
//
VOID FixList(PCHAR ** List, PCHAR Base)
{
	if (*List)
    {
        PCHAR * Addr;

        Addr = *List = (PCHAR *)( ((DWORD)*List + Base) );
        while(*Addr)
        {
            *Addr = (PCHAR)(((DWORD)*Addr + Base));
            Addr++;
        }
    }
}

// Function: UnpackHostEnt
// Description:
//    This function calls the FixList function on the two
//    arrays of pointers. Otherwise it simply fixes the
//    h_name field of the HOSTENT so its an absolute
//    pointer rather than an offset
VOID UnpackHostEnt(struct hostent * hostent)
{
    PCHAR pch;

    pch = (PCHAR)hostent;

    if(hostent->h_name)
    {
        // To make an absolute address, add the base address of
        // the HOSTENT to the offset value and put it back
        hostent->h_name = (PCHAR)((DWORD)hostent->h_name + pch);
    }
    FixList(&hostent->h_aliases, pch);
    FixList(&hostent->h_addr_list, pch);
}

// Function: main
// Description:
//    Initialize Winsock and set up the WSAQUERYSET structure
//    for querying DNS. We use the special GUID 
//    SVCID_INET_HOSTADDRBYNAME for name resolution and we
//    query on the UDP and TCP protocols. The information
//    is returned as a BLOB so we must specify the LUP_RETURN_BLOB
//    flag. Once the call to WSALookupServiceBegin/Next is 
//    successful then decode the BLOB data into a real HOSTENT
//    and print the results.
int main(int argc, char **argv)
{
    WSADATA        wsd;
    WSAQUERYSET   *qs=NULL;
    char           buff[RNR_BUFFER_SIZE];
    GUID           HostnameGuid = SVCID_INET_HOSTADDRBYNAME;
    HANDLE         hRnr;
    AFPROTOCOLS    afproto[2] = { {AF_INET, IPPROTO_UDP}, {AF_INET, IPPROTO_TCP} };
    DWORD          dwLength=RNR_BUFFER_SIZE;
    int            ret, i;
    LPBLOB         pvRet=NULL;
    HOSTENT       *hp=NULL;
    SOCKADDR_IN    addr;

    // Check for proper usage
    if (argc != 2)
    {
        printf("Usage: %s hostname\n", argv[0]);
		printf("Example: %s www.google.com\n", argv[0]);
        return -1;
    }

    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    // Initalize the WSAQUERYSET structure for the query
    qs = (WSAQUERYSET *)buff;

    memset(qs, 0, sizeof(*qs));
    qs->dwSize = sizeof(WSAQUERYSET);
    qs->lpszServiceInstanceName = (LPWSTR)argv[1]; // name to resolve
    qs->lpServiceClassId = &HostnameGuid;
    qs->dwNameSpace = NS_ALL;              // or ND_DNS
    qs->dwNumberOfProtocols = 2;           // TCP and UDP
    qs->lpafpProtocols = afproto;
    
    ret = WSALookupServiceBegin(qs, LUP_RETURN_BLOB | LUP_RETURN_NAME, &hRnr);
    if (ret == NO_ERROR)
    {
		printf("WSALookupServiceBegin() is OK!\n");

        // We only have to call WSALookupServiceNext once because
        // the BLOB HOSTENT return will contain multiple addresses
        // if there are mutliple IP addresses associated with the given name

		qs = (WSAQUERYSET *) malloc(sizeof(WSAQUERYSET) + 100);
		dwLength = sizeof(WSAQUERYSET) + 100;

        ret = WSALookupServiceNext(hRnr, 0, &dwLength, qs);
        if (ret != NO_ERROR)
        {
			// WSASERVICE_NOT_FOUND == 10108
            if (WSAGetLastError() == WSASERVICE_NOT_FOUND)
            {
                printf("No such name found!\n");
                WSALookupServiceEnd(hRnr);
                return -1;
            }
			else
				printf("The given hostname found!\n");

            printf("WSALookupServiceNext() failed with error code %d\n", WSAGetLastError());
        }
		else
			printf("WSALookupServiceNext() should be OK!\n");
        
		if(WSALookupServiceEnd(hRnr) == 0)
			printf("Handle to hRnr was released!\n");
		else
			printf("Failed to release hRnr handle, error %d\n", WSAGetLastError());
    }
	else
		printf("WSALookupServiceBegin() failed with error code %d\n", WSAGetLastError());

    // Make sure we got something returned
    if (qs->lpBlob == NULL)
	{
		printf("qs->lpBlob returned NULL lol!\n");
        return -1;
	}
	else
		printf("qs->lpBlob returned something lol!\n");

	// Create our own buffer to hold the blob data
    hp = (HOSTENT *)LocalAlloc(LPTR, qs->lpBlob->cbSize);
    if (!hp)
    {
        printf("LocalAlloc() for HOSTENT failed with error code %d\n", GetLastError());
        return -1;
    }
	else
		printf("LocalAlloc() for HOSTENT is OK!\n");

    memcpy(hp, qs->lpBlob->pBlobData, qs->lpBlob->cbSize);

	// Unpack the BLOB data so its meaningful
    UnpackHostEnt(hp);

	// Print out the addresses
    for(i=0; hp->h_addr_list[i] ;i++)
    {
        memcpy(&addr.sin_addr, hp->h_addr_list[i], hp->h_length);
        printf("%s IP: %s\n", argv[1], inet_ntoa(addr.sin_addr));
    }

    for(i=0; hp->h_aliases[i] ;i++)
        printf("Alias: %s\n", hp->h_aliases[i]);

    // Cleanup
    LocalFree(hp);
    if(WSACleanup() == 0)
		printf("WSACleanup() is OK!\n");
	else
		printf("WSACleanup() failed with error code %d\n", WSAGetLastError());

    return 0;
}
