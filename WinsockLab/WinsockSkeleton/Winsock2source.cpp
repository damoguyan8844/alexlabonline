#include <winsock2.h>
#include <stdio.h>

int main(void)
{
	WSADATA wsaData;
	int RetCode;

	// Initialize Winsock version 2.2
	if ((RetCode = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", RetCode);
		return 1;
	}
	else
	{
		printf("The Winsock dll found!\n");
		printf("The current status is: %s.\n", wsaData.szSystemStatus);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{
		//Tell the user that we could not find a usable WinSock DLL
		printf("The dll do not support the Winsock version %u.%u!\n",
			LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));
		// When your application is finished call WSACleanup
		WSACleanup();
		// and exit
		return 0;
	}
	else
	{
		printf("The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),
			HIBYTE(wsaData.wVersion));
		printf("The highest version this dll can support: %u.%u\n", LOBYTE(wsaData.wHighVersion),
			HIBYTE(wsaData.wHighVersion));

		// Setup Winsock communication code here

		// When your application is finished call WSACleanup
		if (WSACleanup() == SOCKET_ERROR)
			printf("WSACleanup failed with error %d\n", WSAGetLastError());
		// and exit
		return 1;
	}
}