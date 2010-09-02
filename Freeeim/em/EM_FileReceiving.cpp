// EM_FileReceiving.cpp : implementation file
//

#include "stdafx.h"
#include "../EIM02Dlg.h"
#include "EM_FileReceiving.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EM_FileReceiving

BOOL EM_FileReceiving::m_bFileReceivingThread = TRUE;

EM_FileReceiving::EM_FileReceiving()
{
}

EM_FileReceiving::~EM_FileReceiving()
{
}

void EM_FileReceiving::Run(CWnd *pWnd)
{
	DWORD dwTID = 0;
	m_hThread = CreateThread(NULL, 0, ListeningThread, pWnd, 0, & dwTID);
	if (NULL == m_hThread)
	{
		MessageBox(0,"CreateThread() failed.",0,0);
		return;
	}
	CloseHandle(m_hThread);
}

DWORD EM_FileReceiving::ListeningThread(LPVOID lParam)
{
	SOCKET sListen;
	CEIM02Dlg *pDlg = (CEIM02Dlg *)lParam;
	// Create our listening socket
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sListen)
	{
		MessageBox(0,"socket() failed:%d\n",0,0);
		return 1;
	}
	// Select the local interface and bind to it

	SOCKADDR_IN local;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(5150);

	int ret = bind(sListen, (PSOCKADDR)&local, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		MessageBox(0,"bind() failed:%d\n",0,0);
		return 1;
	}

	listen(sListen, 5);
	int iAddress = sizeof(SOCKADDR_IN);
	SOCKET sClient;
	SOCKADDR_IN addrClient;

	while( m_bFileReceivingThread )
	{
		sClient = accept(sListen, (PSOCKADDR)&addrClient, &iAddress);
		if (INVALID_SOCKET == sClient)
		{
			MessageBox(0,"accept() failed:%d\n", 0,0);
			break;
		}
		char *szClientIP = new char [64];
		strcpy(szClientIP, inet_ntoa(addrClient.sin_addr));
		pDlg->PostMessage(WM_RECVFILE, (WPARAM)sClient, (LPARAM)szClientIP);
	}
	closesocket(sListen);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// EM_FileReceiving message handlers

void EM_FileReceiving::Close()
{
	m_bFileReceivingThread = FALSE;
}
