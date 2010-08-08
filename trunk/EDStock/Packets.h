/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Packet convertor functions.
*/

#if	!defined( __STKLIB_PACKETS_H__ )
#define	__STKLIB_PACKETS_H__

#include "Container.h"

PCOMMPACKET	AllocCommPacket( DWORD dwDataType, DWORD dwCount );
void	FreeCommPacket( PCOMMPACKET pCommPacket );

BOOL convert_REPORT_to_MINUTE( REPORT * pReport, MINUTE * pMinute );

BOOL UpdateKDATAByREPORT( KDATA &kd, REPORT * pReport );
BOOL UpdateStockInfoByREPORT( CStockInfo & info, REPORT * pReport );
BOOL UpdateStockContainerByREPORT( CStockContainer &container, REPORT * pReport, BOOL bAddIfNotExist, REPORT * pReportLast = NULL );
BOOL UpdateStockContainerByKData( CStockContainer &container, LPCTSTR lpszCode, CKData & kdata );


#endif	// __STKLIB_PACKETS_H__

