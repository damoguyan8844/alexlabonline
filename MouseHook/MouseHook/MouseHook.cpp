// MouseHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
#include "MouseHook.h"

#pragma data_seg(".JOE")
HWND hWndServer = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.JOE,rws")

HINSTANCE hInst;
UINT UWM_MOUSEMOVE;
HHOOK hook;
static LRESULT CALLBACK msghook(UINT nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain( HINSTANCE hInstance, 
                       DWORD  Reason, 
                       LPVOID Reserved
					 )
{
 switch(Reason)
    { /* reason */
     case DLL_PROCESS_ATTACH:
	hInst = hInstance;
	UWM_MOUSEMOVE = RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
	return TRUE;
     case DLL_PROCESS_DETACH:
	if(hWndServer != NULL)
	   clearMyHook(hWndServer);
	return TRUE;
    } /* reason */
    return TRUE;
}


/****************************************************************************
*                                 setMyHook
* Inputs:
*       HWND hWnd: Window to notify
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Sets the hook
****************************************************************************/

__declspec(dllexport) BOOL setMyHook(HWND hWnd)
    {
     if(hWndServer != NULL)
	return FALSE; // already hooked!
     hook = SetWindowsHookEx(WH_GETMESSAGE,
			    (HOOKPROC)msghook,
			    hInst,
			    0);
     if(hook != NULL)
	{ /* success */
	 hWndServer = hWnd;
	 return TRUE;
	} /* success */
     return FALSE; // failed to set hook
    } // setMyHook

/****************************************************************************
*                                 clearMyHook
* Inputs:
*       HWND hWnd: Window hook
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Removes the hook that has been set
****************************************************************************/

__declspec(dllexport) BOOL clearMyHook(HWND hWnd)
    {
     if(hWnd != hWndServer || hWnd == NULL)
	return FALSE;
     BOOL unhooked = UnhookWindowsHookEx(hook);
     if(unhooked)
	hWndServer = NULL;
     return unhooked;
    } // clearMyHook

/****************************************************************************
*                                   msghook
* Inputs:
*       int nCode: Code value
*	WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       Either 0 or the result of CallNextHookEx
* Effect: 
*       Hook processing function. If the message is a mouse-move message,
*	posts the coordinates to the parent window
****************************************************************************/

static LRESULT CALLBACK msghook(UINT nCode, WPARAM wParam, LPARAM lParam)
    {
     if(nCode < 0)
	{ /* pass it on */
	 CallNextHookEx(hook, nCode, wParam, lParam);
	 return 0;
	} /* pass it on */
     LPMSG msg = (LPMSG)lParam;
     if(msg->message == WM_MOUSEMOVE ||
	msg->message == WM_NCMOUSEMOVE)
	 PostMessage(hWndServer, UWM_MOUSEMOVE, 0, 0);
     return CallNextHookEx(hook, nCode, wParam, lParam);
    } // msghook
