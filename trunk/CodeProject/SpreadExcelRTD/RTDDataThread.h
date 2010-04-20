/******************************************************************************
* $Id: RTDDataThread.h 17 2008-02-17 17:12:16Z jim.kleckner $
*
*  File: RTDDataThread.h
*
*  Date: February 5, 2001
*
*  Description:   This file contains the declaration of the methods for the 
*  thread that feeds data to the RealTimeData server.  Currently, this thread
*  simply feeds back the current system time.
*
*  Modifications:
******************************************************************************/
#include "IRTDServer.h"

DWORD WINAPI RealTimeDataThread( LPVOID CallbackObject);
WPARAM MessageLoop();
void ThreadOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

//Commands to the thread
#define WM_TERMINATE 100
#define WM_SILENTTERMINATE 101
