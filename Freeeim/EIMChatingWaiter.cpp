// EIMChatingWaiter.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "EIM02Dlg.h"
#include "EIMChatingWaiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEIMChatingWaiter

IMPLEMENT_DYNCREATE(CEIMChatingWaiter, CWinThread)

CEIMChatingWaiter::CEIMChatingWaiter()
{
}

CEIMChatingWaiter::~CEIMChatingWaiter()
{
}

BOOL CEIMChatingWaiter::InitInstance()
{
	// TODO:  perform and per-thread initialization here
//	AfxMessageBox("Thread created.");
	Listen();
	return TRUE;
}

int CEIMChatingWaiter::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CEIMChatingWaiter, CWinThread)
	//{{AFX_MSG_MAP(CEIMChatingWaiter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEIMChatingWaiter message handlers

BOOL CEIMChatingWaiter::Listen()
{
	//CEIM02Dlg *dlg = (CEIM02Dlg*)AfxGetMainWnd();

	m_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_listen)
	{
		AfxMessageBox("socket creating error.");
		return FALSE;
	}

	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(VOICE_RECV_PORT);

	DWORD ret = bind(m_listen, (PSOCKADDR)&sin, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		CString str;
		str.Format("bind failed. %d---", WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}

	ret = listen(m_listen, 5);
	if (SOCKET_ERROR == ret)
	{
		AfxMessageBox("ln 79 listen failed.\n");
		return FALSE;
	}

	SOCKET Accept;
	SOCKADDR_IN client;
	int sinLen = sizeof(client);

	while(TRUE)
	{
		Accept = accept(m_listen, (PSOCKADDR)&client, &sinLen);
		char buf[256];
		sprintf(buf, "%s accepted port:%d.\r\n", inet_ntoa(client.sin_addr), Accept);
		if (INVALID_SOCKET == Accept)
		{
			AfxMessageBox("CEIMChatingWaiter::Listen [accept] failed.");
			break;
		}

		m_wi = new CWaveIn(Accept);
	}

	return TRUE;
}

void CEIMChatingWaiter::Stop()
{
	if (m_wi)
	{
	//	AfxMessageBox("ÓïÒôÍ£Ö¹ÁË");
		m_wi->StopRecord();
		m_wi = NULL;
	}
}
