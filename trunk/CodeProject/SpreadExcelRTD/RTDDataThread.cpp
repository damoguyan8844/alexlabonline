/******************************************************************************
* $Id: RTDDataThread.cpp 24 2008-02-20 17:21:19Z jim.kleckner $
*
*  File: RTDDataThread.cpp
*
*  Date: February 5, 2001
*
*  Description:   This file contains the definition of the methods for the 
*  thread that feeds data to the RealTimeData server.  Currently, this thread
*  simply feeds back the current system time.
*
*  Modifications:
*
*     Morphed into a spread toolkit data provider.
*
 * Copyright 2008, James E. Kleckner
 *  Based on example published by Microsoft without any copyright notices.

  This file is part of spread-excel.

  spread-excel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  spread-excel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with spread-excel.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/
#include "windows.h"
#include "windowsx.h"
#include "RTDDataThread.h"
#include "SpreadExcelRTDImpl.h"

UINT_PTR s_slowTimerID;
UINT_PTR s_fastTimerID;

IRTDUpdateEvent* pRTDUpdate = NULL;
DWORD WINAPI RealTimeDataThread( LPVOID pMarshalStream)
{
    CoInitialize( NULL );
    DWORD dwRetVal = 0;
    HRESULT hr = S_OK;

    //Retrieve the RTDUpdate object
    hr = CoGetInterfaceAndReleaseStream( (IStream*)pMarshalStream, 
        IID_IRTDUpdateEvent, (void**)&pRTDUpdate );

    //Set the heartbeat interval to a little more than our timer interval
    if (pRTDUpdate != NULL){
        pRTDUpdate->AddRef();
        hr = pRTDUpdate->put_HeartbeatInterval( 1200 );
   
        //Initiate fast and slow timers
        // See: http://msdn2.microsoft.com/en-us/library/ms644906(VS.85).aspx
        s_slowTimerID = SetTimer( NULL, 0, 1000, NULL );
      //s_fastTimerID = SetTimer( NULL, 0, 2200, NULL );
        s_fastTimerID = SetTimer( NULL, 0,  200, NULL );

        //Spin a message loop so the thread stays alive, and can receive commands 
        //from the parent thread.
        dwRetVal = MessageLoop();
      
        //Kill the timers
        KillTimer( NULL, s_fastTimerID );
        KillTimer( NULL, s_slowTimerID );

        //Clean up the RTDUpdate object
        pRTDUpdate->Release();
    }

    CoUninitialize();

    //All done...
    return dwRetVal;
}

/******************************************************************************
*  MessageLoop -- This method controls a standard Windows message loop.
*  Parameters: none
*  Returns: the status code from GetMessage.
******************************************************************************/
WPARAM MessageLoop()
{
    MSG msg;
    HRESULT hr;

    //wip: JEK: need to sort out how to perform the data read for spread
    //Only WM_QUIT causes GetMessage to return 0.
    // See: http://msdn2.microsoft.com/en-us/library/ms644936(VS.85).aspx
    while (GetMessage( &msg, NULL, 0, 0)){
        //switch on the message id
        switch( msg.message ){
        case WM_TIMER:
            if ( msg.wParam == s_fastTimerID )
            {
                if (g_spreadExcelRTD_instance != NULL)
                {
                    hr = g_spreadExcelRTD_instance->CheckForMessages();
                    if (hr == S_OK)
                    {
                        hr = pRTDUpdate->UpdateNotify();
                    }
                }
            }
            else if ( msg.wParam == s_slowTimerID )
            {
                if (g_spreadExcelRTD_instance != NULL)
                {
                    hr = g_spreadExcelRTD_instance->ProcessTimeTopics();
                    if (hr == S_OK)
                    {
                        hr = pRTDUpdate->UpdateNotify();
                    }
                }
            }
            break;
        case WM_COMMAND:
            HANDLE_WM_COMMAND(msg.hwnd, msg.wParam, msg.lParam, ThreadOnCommand);
            break;
        default:
            break;
        }
    }
    return msg.wParam;
}

/******************************************************************************
*  ThreadOnCommand -- This method handles the WM_COMMAND messages.
*  Parameters: hWnd -- handle to the window that received the message
*              id -- the command that was received
*              hwndCtl
*              codeNotify
*  Returns: none
******************************************************************************/
void ThreadOnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HRESULT hr = S_OK;
    //switch on the command
    switch( id ){
    case WM_TERMINATE:
        hr = pRTDUpdate->Disconnect();
    case WM_SILENTTERMINATE:
        // Kill the timers
        // Try killing them sooner to see if this will get rid of the exceptions.
        KillTimer( NULL, s_fastTimerID );
        KillTimer( NULL, s_slowTimerID );
        PostQuitMessage( hr );
        break;
    default:
        break;
    }
}

