#include "support.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	WSADATA      wsd;
    SOCKET       s;
    DWORD        deviceID, bytes, dwAddrLen = 256;
    ATM_ADDRESS  addr;
    SOCKADDR_ATM atm_addr;
    char         szAddress[256];
    WSAPROTOCOL_INFO pProtocolInfo;

    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() is OK!\n");

    if (FindProtocol(&pProtocolInfo) == FALSE)
    {
        printf("FindProtocol() returned NULL for ATM. Check the ATM adapter/NIC!!\n");
        WSACleanup();
        return -1;
    }
	else
		printf("FindProtocol() returned something for ATM!\n");

    s = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO,
                  &pProtocolInfo, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        WSACleanup();
		return -1;
    }
	else
		printf("WSASocket() is OK!\n");

    deviceID = 0;
    if (WSAIoctl(s, SIO_GET_ATM_ADDRESS, (LPVOID) &deviceID, 
        sizeof(DWORD), (LPVOID) &addr, sizeof(ATM_ADDRESS), 
        &bytes, NULL, NULL) == SOCKET_ERROR)
    {
        printf("WSAIoctl() error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
	else
		printf("WSAIoctl() is OK!\n");

    ZeroMemory((PVOID)&atm_addr, sizeof(atm_addr));
    atm_addr.satm_family                 = AF_ATM;
    atm_addr.satm_number.AddressType     = ATM_NSAP;
    atm_addr.satm_number.NumofDigits     = ATM_ADDR_SIZE;
    atm_addr.satm_blli.Layer2Protocol    = SAP_FIELD_ANY;
    atm_addr.satm_blli.Layer3Protocol    = SAP_FIELD_ABSENT;
    atm_addr.satm_bhli.HighLayerInfoType = SAP_FIELD_ABSENT;

    memcpy(atm_addr.satm_number.Addr, &addr.Addr, ATM_ADDR_SIZE);
	ZeroMemory((PVOID)szAddress, sizeof(szAddress));

    if (WSAAddressToString((LPSOCKADDR)&atm_addr, sizeof(atm_addr),
        &pProtocolInfo, (LPWSTR)szAddress, &dwAddrLen))
    {
        printf("WSAAddressToString() error %d\n", WSAGetLastError());
        return(FALSE);
    }
	else
		printf("WSAAddressToString() is fine!\n");


	printf("The atm address (if any): <%S>\n", szAddress);

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
