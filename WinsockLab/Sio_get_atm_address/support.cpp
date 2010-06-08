#include "support.h"
#include <stdio.h>

UCHAR BtoH( CHAR ch );

int  GetNumATMInterfaces(SOCKET s)
{
    DWORD  dwNum, nbytes = sizeof(DWORD);

    if (WSAIoctl(s, SIO_GET_NUMBER_OF_ATM_DEVICES, NULL, 0,
        (LPVOID)&dwNum, sizeof(DWORD), &nbytes, NULL, NULL) == SOCKET_ERROR)
    {
        return -1;
    }
    return dwNum;
}

BOOL GetATMAddress(SOCKET s,int device, ATM_ADDRESS *atmaddr)
{
    DWORD bytes;

    if (WSAIoctl(s, SIO_GET_ATM_ADDRESS, (LPVOID)&device, sizeof(DWORD),
        (LPVOID)atmaddr, sizeof(ATM_ADDRESS), &bytes, NULL, NULL) == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL FindProtocol(WSAPROTOCOL_INFO *lpProto)
{
    WSAPROTOCOL_INFO *lpProtocolBuf=NULL;
    DWORD             dwErr, dwRet, dwBufLen=0, i;
	
	if (WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen) != SOCKET_ERROR)
    {
        // This should never happen as there is a NULL buffer
		printf("WSAEnumProtocols() failed!\n");
		return FALSE;
	}
    else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
    {
		// We failed for some reason not relating to buffer size - also odd
		printf("WSAEnumProtocols() failed with error code %d\n", dwErr);
		return FALSE;
    }
	else
		printf("WSAEnumProtocols() should be fine!\n");

    // Allocate the correct buffer size for WSAEnumProtocols as
    // well as the buffer to return
    lpProtocolBuf = (WSAPROTOCOL_INFO *)GlobalAlloc(GMEM_FIXED, dwBufLen);

    if (lpProtocolBuf == NULL)
    {
		printf("GlobalAlloc() for protocol buffer failed with error code %d\n", GetLastError());
		return FALSE;
    }
	else
		printf("GlobalAlloc() is OK for protocol buffer...\n");
	
	dwRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
    if (dwRet == SOCKET_ERROR)
    {
		printf("WSAEnumProtocols() failed wit herror code %d\n", WSAGetLastError());
		GlobalFree(lpProtocolBuf);
		return FALSE;
    }
	else
		printf("WSAEnumProtocols() is OK!\n");

    // Loop through the returned protocol information looking for those
    // that are in the AF_ATM address family.
    for (i=0; i < dwRet ;i++)
    {
		printf("Protocol: %S\n", lpProtocolBuf[i].szProtocol);
		
		if ( (lpProtocolBuf[i].iAddressFamily == AF_ATM) &&
             (lpProtocolBuf[i].iSocketType == SOCK_RAW) &&
             (lpProtocolBuf[i].iProtocol == ATMPROTO_AAL5) )
        {
            memcpy(lpProto, &lpProtocolBuf[i], sizeof(WSAPROTOCOL_INFO));
            GlobalFree(lpProtocolBuf);
            return TRUE;
        }
    }
    GlobalFree(lpProtocolBuf);

    return FALSE;
}

// AtoH () coverts the ATM address specified in the string(ascii) format to
// the binary(hexadecimal) format.
void AtoH( CHAR *szDest, CHAR *szSource, INT iCount )
{
    
    while (iCount--)
    {
        *szDest++ = ( BtoH ( *szSource++ ) << 4 ) + BtoH ( *szSource++ );
    }
    return;
}

// BtoH () returns the equivalent binary value for an individual
// character specified in the ascii format.
UCHAR BtoH( CHAR ch )
{
    if ( ch >= '0' && ch <= '9' )
    {
        return ( ch - '0' );
    }

    if ( ch >= 'A' && ch <= 'F' )
    {
        return ( ch - 'A' + 0xA );
    }

    if (ch >= 'a' && ch <= 'f' )
    {
        return ( ch - 'a' + 0xA );
    }

	// Illegal values in the address will not be excepted
    return -1;
}
