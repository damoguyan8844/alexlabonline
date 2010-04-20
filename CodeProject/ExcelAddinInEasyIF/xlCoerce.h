///***************************************************************************
// File:				xlCoerce.H
//
// Purpose:			Header file for xlCoerce
//
// Platform:    Microsoft Windows
//
// Comments:
//              Include this file in any source files
//              that use XLCoerce() or XLCoercev().
//
// From the Microsoft Excel Developer's Kit, Version 5
// Copyright (c) 1996 Microsoft Corporation. All rights reserved.
///***************************************************************************
#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

// 
// Function prototypes
//
int WINAPI XLCoercev(LPXLOPER pxRes, int count, LPXLOPER far opers[]);
int far _cdecl XLCoerce(LPXLOPER pxRes, int count,... ); 
HRESULT InitXLCoerce();
VOID TermXLCoerce();

#ifdef __cplusplus
}			/* End of extern "C" { */
#endif	/* __cplusplus */
#endif   /* End of _WIN32 */
