// EM_RecvChat.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "EM_RecvChat.h"
#include "em/stdemdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EM_RecvChat dialog


EM_RecvChat::EM_RecvChat(CWnd* pParent /*=NULL*/)
	: CDialog(EM_RecvChat::IDD, pParent)
{
	//{{AFX_DATA_INIT(EM_RecvChat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bChatting = FALSE;
}


void EM_RecvChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EM_RecvChat)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EM_RecvChat, CDialog)
	//{{AFX_MSG_MAP(EM_RecvChat)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	ON_BN_CLICKED(IDC_DENY, OnDeny)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EM_RecvChat message handlers

void EM_RecvChat::OnAccept() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetParent();
	pWnd->PostMessage(WM_SRCACCEPTCHAT);
	KillTimer(33);
	GetDlgItem(IDC_ACCEPT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
	m_process.ShowWindow(SW_HIDE);
	m_bChatting = TRUE;
	GetDlgItem(IDC_DENY)->SetWindowText("结束通话(&T)");

	CRect rect;
	GetDlgItem(IDC_STATIC8)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right += 170;
	GetDlgItem(IDC_STATIC8)->MoveWindow(&rect);

	SetTimer(55, 1000, NULL);

	// 启动收听线程
	m_pwo = (CWaveOut*)AfxBeginThread(
	RUNTIME_CLASS(CWaveOut),
	THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED);
	m_pwo->SetIP(m_strIP);
//	AfxMessageBox(m_strIP);
	m_pwo->ResumeThread();

//	GetDlgItem(IDC_STATIC88)->SetWindowText(m_strIP);

//	CDialog::OnOK();
}

void EM_RecvChat::OnDeny() 
{
	// TODO: Add your control notification handler code here
	if (! m_bChatting)
	{
		CWnd *pWnd = GetParent();
		pWnd->PostMessage(WM_EMDONOTCHAT, 0, sizeof(DWORD));
		CDialog::OnOK();
	}
	else
	{
		CWnd *pWnd = GetParent();
		pWnd->PostMessage(WM_EMDONOTCHAT, 1, sizeof(DWORD));

		if (m_pwo)
			m_pwo->Stop();
	}
}

void EM_RecvChat::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static DWORD nSecond = 0;
	if (33 == nIDEvent)
	{
		int nPos = m_process.StepIt();
		CString str;
		str.Format("将在 %d 秒后自动取消", int((100-nPos)/10)+1);
		GetDlgItem(IDC_STATIC6)->SetWindowText(str);
		if (100 == nPos)
		{
			KillTimer(nIDEvent);
			OnDeny();
		}
	}

	if (55 == nIDEvent)
	{
		CString str;
		str.Format(_T("语音对话    对话进行中，通话时间 %d 秒。"), nSecond++);
		GetDlgItem(IDC_STATIC8)->SetWindowText(str);
	}
	CDialog::OnTimer(nIDEvent);
}

void EM_RecvChat::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	GetDlgItem(IDC_ACCEPT)->SetFocus();
	m_process.SetRange(1, 100);
	m_process.SetStep(1);
	SetTimer(33, 100, NULL);
}