// _DLG_Advertise.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "_DLG_Advertise.h"
#include "em/stdemdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// _DLG_Advertise dialog


_DLG_Advertise::_DLG_Advertise(CWnd* pParent /*=NULL*/)
	: CDialog(_DLG_Advertise::IDD, pParent)
{
	//{{AFX_DATA_INIT(_DLG_Advertise)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void _DLG_Advertise::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(_DLG_Advertise)
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(_DLG_Advertise, CDialog)
	//{{AFX_MSG_MAP(_DLG_Advertise)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(_DLG_Advertise, CDialog)
    //{{AFX_EVENTSINK_MAP(CHtmlEditorDlg)
	ON_EVENT(_DLG_Advertise, IDC_EXPLORER1, 252 /* NavigateComplete2 */, OnNavigateComplete2Explorer1, VTS_DISPATCH VTS_PVARIANT)
//	ON_EVENT(CSerialTesterDlg, IDC_EXPLORER1, 102 /* StatusTextChange */, OnStatusTextChangeExplorer1, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// _DLG_Advertise message handlers

void _DLG_Advertise::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void _DLG_Advertise::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

BOOL _DLG_Advertise::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
//	m_web.Navigate("http://www.coopic.cn/ts/tsad/adve.htm", NULL,NULL,NULL,NULL);
//	m_web.Navigate("adve.htm", NULL,NULL,NULL,NULL);
	m_web.Navigate("http://www.freeeim.com/index.asp?v29", NULL,NULL,NULL,NULL);

	CRect rtMain;
	GetWindowRect(& rtMain);
	ScreenToClient(& rtMain);

	CRect rt;
	m_web.GetWindowRect(& rt);
	ScreenToClient(& rt);
	rt.left -= 2;
	rt.top -= 2;
	rt.right = rtMain.right;
	rt.bottom += 20;
	m_web.MoveWindow(& rt);

//	SetTimer(TIMER_REFRESH, TIMER_REFRESH_TIME, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void _DLG_Advertise::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	if (nIDEvent == TIMER_REFRESH)
	{
	//	AfxMessageBox("check for new adv");
	//	m_web.Refresh();
//		m_web.Navigate("http://www.coopic.cn/ts/tsad/adve.htm", NULL,NULL,NULL,NULL);
	}
	CDialog::OnTimer(nIDEvent);
}


void _DLG_Advertise::OnNavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	//let's turn on HTML editing capabilities!
//	m_web.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
/*	CString str;
	m_web.GetDocumentHTML(str);
	CString strValue;
	CString strURL;
	int nIdx = str.Find("<!-- ts_adv_value=0 -->");
	if (-1 != nIdx)
	{
		strValue = str.Right(str.GetLength()-nIdx);
		int nUrl = strValue.Find("http://");
		strURL = strValue.Right(strValue.GetLength()-nUrl);
		nUrl = strURL.Find(' ');
		strURL = strURL.Left(nUrl);
	//	AfxMessageBox(strURL);
		_NavNewAdve(strURL);
	}*/
}

void _DLG_Advertise::_NavNewAdve(LPCTSTR lpszURL)
{
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat(buf, "XExplorer.exe");

	CString strCmd;
	strCmd.Format("%s %s %d", buf, lpszURL, 8000);
//	AfxMessageBox(strCmd);
//	return;
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	// Set up members of STARTUPINFO structure.
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpReserved2 = NULL; 
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpDesktop = NULL; 
	siStartInfo.dwFlags = 0;

	// Create the child process.
	BOOL bRet;
	bRet = CreateProcess(
	NULL,
	(LPSTR)(LPCTSTR)strCmd,
	NULL, // process security attributes
	NULL, // primary thread security attributes
	0, // handles are inherited
	0, // creation flags
	NULL, // use parent's environment
	NULL, // use parent's current directory
	&siStartInfo, // STARTUPINFO pointer
	&piProcInfo); // receives PROCESS_INFORMATION

	if (bRet == FALSE)
	{
		CString strError;
		strError.Format("%d", GetLastError());
		AfxMessageBox(strError);
	}

	CloseHandle(piProcInfo.hThread);
	CloseHandle(piProcInfo.hProcess);
}
