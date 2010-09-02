// EM_SendChat.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "EM_SendChat.h"
#include "em/stdemdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EM_SendChat dialog


EM_SendChat::EM_SendChat(CWnd* pParent /*=NULL*/)
	: CDialog(EM_SendChat::IDD, pParent)
{
	//{{AFX_DATA_INIT(EM_SendChat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bChatting = FALSE;
	m_pwo = NULL;
}


void EM_SendChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EM_SendChat)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EM_SendChat, CDialog)
	//{{AFX_MSG_MAP(EM_SendChat)
	ON_BN_CLICKED(IDC_CANCEL, OnChatCancel)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EM_SendChat message handlers

void EM_SendChat::OnChatCancel() 
{
	// TODO: Add your control notification handler code here
	if (! m_bChatting)
	{
		CWnd *pWnd = GetParent();
		pWnd->PostMessage(WM_EMCANCELREQUEST, 2);

		KillTimer(33);
		CDialog::OnCancel();
	}
	else
	{
		if (m_pwo)
			m_pwo->Stop();

		CWnd *pWnd = GetParent();
		pWnd->PostMessage(WM_EMCANCELREQUEST, 1);
	}
}

void EM_SendChat::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int idde = 0;
	static DWORD nSecond = 0;
	// TODO: Add your message handler code here and/or call default
	if (33 == nIDEvent)
	{
		char buf[10];
		for(int i = 0; i < idde; i++)
		{
			buf[i] = '>';
		}
		buf[i] = NULL;
		GetDlgItem(IDC_STATIC8)->SetWindowText(buf);
		idde ++;
		if (5 < idde)
			idde = 0;
	}
	if (55 == nIDEvent)
	{
		CString str;
		str.Format(_T("语音对话    对话进行中，通话时间 %d 秒。"), nSecond++);
		GetDlgItem(IDC_STATIC6)->SetWindowText(str);
	}
	CDialog::OnTimer(nIDEvent);
}

void EM_SendChat::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	SetTimer(33, 300, NULL);
}

BOOL EM_SendChat::DestAcceptChat(LPCTSTR ip)
{
//	AfxMessageBox(ip);
	m_bChatting = TRUE;
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCEL)->SetWindowText("结束通话(&T)");
	SetTimer(55, 1000, NULL);
	
	CRect rect;
	GetDlgItem(IDC_STATIC6)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right += 50;
	GetDlgItem(IDC_STATIC6)->MoveWindow(&rect);

//	AfxMessageBox(ip);
	// 启动收听线程
	m_pwo = (CWaveOut*)AfxBeginThread(
		RUNTIME_CLASS(CWaveOut),
		THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED);
	m_pwo->SetIP(ip);
	m_pwo->ResumeThread();

//	GetDlgItem(IDC_STATIC88)->SetWindowText(ip);

	return TRUE;
}
