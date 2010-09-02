// Author: Alxe
// Date: 2007-3-8
// Not tested yet

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _MSG_RECEIVER
#define _MSG_RECEIVER
#include "em/stdemdef.h"
#include "winsock2.h"

// 这个消息告诉线程该结束了
#ifndef EM_MSG23
#define EM_MSG23
#define WM_MSG_CLOSE WM_USER + 0x23
#endif // EM_MSG23

class EM_MSGReceiver
{
public:

	void Close();
	EM_MSGReceiver();
	~EM_MSGReceiver();

	void Run(const HWND hWnd);
	void SendMsg(char *szIP, LPEM_DATA lpMsg);

private:

	static DWORD WINAPI UDPMsgProc(LPVOID lParam);
	static BOOL m_bMessageReceiving;

	SOCKET m_sock;
	DWORD m_tid;
	HANDLE m_hThread;
	HWND m_hWnd;

};

#endif		//_MSG_RECEIVER