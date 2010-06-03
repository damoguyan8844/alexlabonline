#include <winsock2.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	WSADATA              wsaData;
	SOCKET               SendingSocket;
	// Server/receiver address
	SOCKADDR_IN          ServerAddr;
	// Server/receiver port to connect to
	unsigned int         Port = 5150;
	int  RetCode;

	// Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2,2), &wsaData);
	printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

	// Create a new socket to make a client connection.
	// AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
	SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(SendingSocket == INVALID_SOCKET)
	{
		printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
		// Do the clean up
		WSACleanup();
		// Exit with error
		return -1;
	}
	else
		printf("Client: socket() is OK!\n");

	// Set up a SOCKADDR_IN structure that will be used to connect
	// to a listening server on port 5150. For demonstration
	// purposes, let's assume our server's IP address is 127.0.0.1 or localhost

	// IPv4
	ServerAddr.sin_family = AF_INET;
	// Port no.
	ServerAddr.sin_port = htons(Port);
	// The IP address
	ServerAddr.sin_addr.s_addr = inet_addr("192.168.0.16");

	// Make a connection to the server with socket SendingSocket.
	RetCode = connect(SendingSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
	if(RetCode != 0)
	{
		printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
		// Close the socket
		closesocket(SendingSocket);
		// Do the clean up
		WSACleanup();
		// Exit with error
		return -1;
	}
	else
	{
		printf("Client: connect() is OK, got connected...\n");
		printf("Client: Ready for sending and receiving data...\n");
	}

	// At this point you can start sending or receiving data on
	// the socket SendingSocket. We will describe sending and receiving data
	// later in the chapter.

	// When you are finished sending and receiving data on socket SendingSocket,
	// you should close the socket using the closesocket API. We will
	// describe socket closure later in the chapter.
	if(closesocket(SendingSocket) != 0)
		printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
	else
		printf("Client: Closing \"SendingSocket\" socket...\n");

	// When your application is finished handling the connection, call WSACleanup.
	if(WSACleanup() != 0)
		printf("Client: WSACleanup() failed!...\n");
	else
		printf("Client: WSACleanup() is OK...\n");
	return 0;
}