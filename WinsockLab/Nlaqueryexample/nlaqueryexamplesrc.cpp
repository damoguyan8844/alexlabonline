// Description:
//    This sample demonstrates how to query and monitor the
//    Network Location Awareness service using the
//    WSALookupServiceBegin, WSALookupServiceNext and the
//    WSALookupServiceEnd APIs to obtain a list of network
//    names available to the system. For each network name
//    found, network characteristics such as speed and
//    connectivity properties are printed.
//
// No command line argument
//    nlaqueryexample
//
// Link to ws2_32.lib
#include <winsock2.h>
#include <mswsock.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    WSADATA        wsd;
    int            Ret;
    char           buff[16384];
	int			   Err;
    DWORD          BufferSize;
    WSAQUERYSET   *qs=NULL;
    GUID           NLANameSpaceGUID = NLA_SERVICE_CLASS_GUID;
    HANDLE         hNLA;
    WSAEVENT       Event;
    PNLA_BLOB		pNLA;
	DWORD BytesReturned;
	WSACOMPLETION WSAComplete;
	WSAOVERLAPPED WSAOverlap;
    DWORD     Offset = 0;

    // Load Winsock
    if ((WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSAStartup() should be OK!\n");

    // Initalize the WSAQUERYSET structure for the query
    qs = (WSAQUERYSET *) buff;
    memset(qs, 0, sizeof(*qs));
    // Required values
    qs->dwSize = sizeof(WSAQUERYSET);
    qs->dwNameSpace = NS_NLA;
    qs->lpServiceClassId = &NLANameSpaceGUID;

    // Optional values
    qs->dwNumberOfProtocols = 0;
    qs->lpszServiceInstanceName = NULL;
    qs->lpVersion = NULL;
    qs->lpNSProviderId = NULL;
    qs->lpszContext = NULL;
    qs->lpafpProtocols = NULL;
    qs->lpszQueryString = NULL;
    qs->lpBlob = NULL;

    if (WSALookupServiceBegin(qs, LUP_RETURN_ALL | LUP_DEEP, &hNLA) == SOCKET_ERROR)
    {
        printf("WSALookupServiceBegin() failed with error %d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSALookupServiceBegin() should be OK!\n");


    if ((Event = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        printf("WSACreateEvent() failed with error code%d\n", WSAGetLastError());
        return -1;
    }
	else
		printf("WSACreateEvent() should be fine!\n");

    while(1)
    {

        printf("Querying for Networks...\n");

        while (1)
        {
            memset(qs, 0, sizeof(*qs));
            BufferSize = sizeof(buff);

            if (WSALookupServiceNext(hNLA, LUP_RETURN_ALL, &BufferSize, qs) == SOCKET_ERROR)
            {
                Err = WSAGetLastError();

                if (Err == WSA_E_NO_MORE)
                {
                    // There is no more data. Stop asking
					printf("No more data, exit...\n");
                    break;
                }

                printf("WSALookupServiceNext failed with error %d\n", WSAGetLastError());
                WSALookupServiceEnd(hNLA);
                return -1;
            }

            printf("\nNetwork Name: %s\n", qs->lpszServiceInstanceName);
            printf("Comment (if any): %s\n", qs->lpszComment);

            if (qs->lpBlob != NULL)
            {
                // Cycle through BLOB data list
                do
                {
                    pNLA = (PNLA_BLOB) &(qs->lpBlob->pBlobData[Offset]);

                    switch (pNLA->header.type)
                    {
                        case NLA_RAW_DATA:
                            printf("\tNLA Data Type: NLA_RAW_DATA\n");
                            break;
                        case NLA_INTERFACE:
                            printf("\tNLA Data Type: NLA_INTERFACE\n");
                            printf("\t\tType: %d\n", pNLA->data.interfaceData.dwType);
                            printf("\t\tSpeed: %d\n", pNLA->data.interfaceData.dwSpeed);
                            printf("\t\tAdapter Name: %s\n", pNLA->data.interfaceData.adapterName);
                            break;
                        case NLA_802_1X_LOCATION:
                            printf("\tNLA Data Type: NLA_802_1X_LOCATION\n");
                            printf("\t\tInformation: %s\n", pNLA->data.locationData.information);
                            break;
                        case NLA_CONNECTIVITY:
                            printf("\tNLA Data Type: NLA_CONNECTIVITY\n");
                            switch(pNLA->data.connectivity.type)
                            {
                                 case NLA_NETWORK_AD_HOC:
                                     printf("\t\tNetwork Type: AD HOC\n");
                                     break;
                                 case NLA_NETWORK_MANAGED:
                                     printf("\t\tNetwork Type: Managed\n");
                                     break;
                                 case NLA_NETWORK_UNMANAGED:
                                     printf("\t\tNetwork Type: Unmanaged\n");
                                     break;
                                 case NLA_NETWORK_UNKNOWN:
                                     printf("\t\tNetwork Type: Unknown\n");
                            }
                            switch(pNLA->data.connectivity.internet)
                            {
                                 case NLA_INTERNET_NO:
                                     printf("\t\tInternet connectivity: No\n");
                                     break;
                                 case NLA_INTERNET_YES:
                                     printf("\t\tInternet connectivity: Yes\n");
                                     break;
                                 case NLA_INTERNET_UNKNOWN:
                                     printf("\t\tInternet connectivity: Unknown\n");
                                     break;
                            }

                            break;
                        case NLA_ICS:
                            printf("\tNLA Data Type: NLA_ICS\n");
                            printf("\t\tSpeed: %d\n", pNLA->data.ICS.remote.speed);
                            printf("\t\tType: %d\n", pNLA->data.ICS.remote.type);
                            printf("\t\tState: %d\n", pNLA->data.ICS.remote.state);
                            printf("\t\tMachine Name: %S\n", pNLA->data.ICS.remote.machineName);
                            printf("\t\tShared Adapter Name: %S\n", pNLA->data.ICS.remote.sharedAdapterName);
                            break;

                        default:
                            printf("\tNLA Data Type: Unknown to this program\n");
                            break;
                    }

                    Offset = pNLA->header.nextOffset;
                }
                while (Offset != 0);
            }
        }

        printf("\nFinished query, Now wait for change notification...\n");

        WSAOverlap.hEvent = Event;
        WSAComplete.Type = NSP_NOTIFY_EVENT;
        WSAComplete.Parameters.Event.lpOverlapped = &WSAOverlap;

        if (WSANSPIoctl(hNLA, SIO_NSP_NOTIFY_CHANGE, NULL, 0, NULL, 0,
            &BytesReturned, &WSAComplete) == SOCKET_ERROR)
        {
            Ret = WSAGetLastError();

            if (Ret != WSA_IO_PENDING)
            {
                printf("WSANSPIoctrl() failed with error code %d\n", Ret);
                return -1;
            }
        }
		else
			printf("WSANSPIoctl() should be OK!\n");

        if (WSAWaitForMultipleEvents(1, &Event, TRUE, WSA_INFINITE, FALSE) == WSA_WAIT_FAILED)
        {
            printf("WSAWaitForMultipleEvents() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
		else
			printf("WSAWaitForMultipleEvents() is OK!\n");

        if(WSAResetEvent(Event) == TRUE)
			printf("WSAResetEvent() is OK!\n");
		else
			printf("WSAResetEvent() failed with error code %d\n", WSAGetLastError());
    }

    if(WSALookupServiceEnd(hNLA) == 0)
		printf("WSALookupServiceEnd() is OK!\n");
	else
		printf("WSALookupServiceEnd() failed with error code %d\n", WSAGetLastError());

    if(WSACleanup() == 0)
		printf("WSACleanup() is OK!\n");
	else
		printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
}
