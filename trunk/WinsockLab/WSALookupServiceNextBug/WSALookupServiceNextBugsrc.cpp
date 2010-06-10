// Standard I/O
#include <stdio.h>
// Link to ws2_32.lib
#include <winsock2.h>
// For guids
#include <svcguid.h>

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	DWORD dwResult;
	HANDLE hLookup = 0;
	WSAQUERYSET lpRestrictions;
	GUID guid = SVCID_HOSTNAME;
	
	dwResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	
	if (dwResult != 0)
	{
		printf("Cannot startup Winsock, error code %d\n", dwResult);
		exit(1);
	}
	else
		printf("WSAStartup() is OK!\n");
	
	ZeroMemory(&lpRestrictions, sizeof(WSAQUERYSET));
	lpRestrictions.dwSize = sizeof(WSAQUERYSET);
	lpRestrictions.lpServiceClassId = &guid;
	
	dwResult = WSALookupServiceBegin(&lpRestrictions, LUP_RETURN_NAME, &hLookup);
	if (dwResult != SOCKET_ERROR)
	{
		DWORD dwLength = 0;
		WSAQUERYSET * pqs = NULL;

		printf("WSALookupServiceBegin() is OK!\n");
		
		// Picking an arbitrary value works fine
		// UNCOMMENT below for success on Windows 2000, XP
		
		pqs = (WSAQUERYSET *) malloc(sizeof(WSAQUERYSET) + 100);
		dwLength = sizeof(WSAQUERYSET) + 100;
		
		do
		{
			if (WSALookupServiceNext(hLookup, 0, &dwLength, pqs) != 0)
			{
				dwResult = WSAGetLastError();
				if((dwResult == WSA_E_NO_MORE) || (dwResult == WSAENOMORE))
				{
					printf("No more record found!\n");
					break;
				}
				printf("WSALookupServiceNext() failed with error code %d\n", WSAGetLastError());
			}
			else
			{
				dwResult = 0;
				printf("WSALookupServiceNext() looks fine!\n");
			}
			
			if (dwResult == WSAEFAULT)
			{
				if (pqs)
				{
					printf("Freeing pqs...\n");
					free(pqs);
				}
				
				// Reallocate
				pqs = (WSAQUERYSET *) malloc(dwLength);
				if (!pqs)
				{
					printf("Could not allocate memory: %d\n", GetLastError());
					exit(2);
				}
				else
				{
					printf("Memory allocated for pqs successfully!\n");
					continue;
				}
			}
			
			// Output it since we have it now
			if ((dwResult == 0) && (pqs))
			{
				printf("  Service instane name: %S\n", pqs->lpszServiceInstanceName);
				printf("  Name space num: %d\n", pqs->dwNameSpace);
				printf("  Num of protocols: %d\n", pqs->dwNumberOfProtocols);
				printf("  Version: %d\n", pqs->lpVersion);
			}
		} while ((dwResult != WSA_E_NO_MORE) && (dwResult != WSAENOMORE));
		
		// clean-up
		free(pqs);
		if(WSALookupServiceEnd(hLookup) == 0)
			printf("hLookup handle was released!\n");
		else
			printf("WSALookupServiceEnd(hLookup) failed with error code %d\n", WSAGetLastError());
	}
	else
	{
		printf("Error on WSALookupServiceBegin: %d\n", WSAGetLastError());
		exit(3);
	}
	
	if(WSACleanup() == 0)
		printf("WSACleanup() is OK!\n");
	else
		printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
	return 0;
}
