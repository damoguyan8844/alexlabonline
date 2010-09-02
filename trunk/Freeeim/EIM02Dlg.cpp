// EIM02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "EIM02Dlg.h"
#include "winsock2.h"
#include "UserSettingDlg.h"
#include "EM_UserProperty.h"
#include "atlbase.h"
//#include "Registry.h"
#include "EIM_ViewHistory.h"
//#include "FIM_SerialNumberDlg.h"

#include "XEIM_Database.h"

#pragma comment(lib, "ws2_32")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strInfo.LoadString(IDS_STRING_DESCRIPTION);
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEIM02Dlg dialog

CEIM02Dlg::CEIM02Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEIM02Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEIM02Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i=0; i<256; i++)
	{
		m_Chatdlg[i] = NULL;
	}
	m_cw = NULL;

	m_dwStatus = STATUS_OFFLINE;

	m_bIsLogin = FALSE;
	m_bVoiceChat = FALSE;

//	m_pAdv = NULL;
}

void CEIM02Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEIM02Dlg)
	DDX_Control(pDX, IDC_TREE1, m_treeCtrlList);
//	DDX_Control(pDX, IDC_TAB1, m_tabCtrlFunction);
	DDX_Control(pDX, IDC_LOADGIF, m_pictureLoading);
	DDX_Control(pDX, IDC_EXPLORER1, m_webBrowser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEIM02Dlg, CDialog)
	//{{AFX_MSG_MAP(CEIM02Dlg)
	ON_MESSAGE(WM_RECVFILE, FileComming)
	ON_MESSAGE(WM_FILESENDCOMPLETED, EM_FileSendCompleted)
	ON_COMMAND(ID_TRAYQUIT, OnQuit)
	ON_COMMAND(IDM_HELPTOPIC, OnHelptopic)
	ON_COMMAND(IDM_SHOWNAME, EM_UserSetting)
	ON_COMMAND(IDM_ABOUTEIM, OnAbouteim)
	ON_COMMAND(ID_TRAYRESTORE, OnTrayRestore)
	ON_COMMAND(IDM_QUITEIM, OnQuiteim)
//	ON_COMMAND(IDM_AUTORUN, EM_OnAutorun)
	ON_COMMAND(IDM_RELESH, OnRelesh)
	ON_COMMAND(IDM_SENDALL, OnSendall)
	ON_COMMAND(IDM_VIEWHISTORY, OnViewHistory)
	ON_COMMAND(IDM_LANEXPLORER, OnLanexplorer)
	ON_COMMAND(IDM_LOCKSCREEN, OnLockscreen)
	ON_COMMAND(IDM_CLOSESCREEN, OnClosescreen)
	ON_COMMAND(IDM_SNOW, OnSnow)
	ON_COMMAND(IDM_WININFO, OnWininfo)
	ON_COMMAND(IDM_DISKNAME, OnDiskname)
	ON_COMMAND(IDM_NOTE, OnNote)
	ON_MESSAGE(WM_NEW_FILE, EM_NewFile)
	ON_MESSAGE(WM_NEW_MSG, EM_NewMsg)
	ON_MESSAGE(WM_DOWNLOADFILE, EM_DownloadFile)
	ON_MESSAGE(WM_DESTACCEPTFILE, EM_DestAcceptFile)
	ON_MESSAGE(WM_DELETEFILE, EM_DeleteFileFromList)
	ON_MESSAGE(WM_ADDTOFILELIST, EM_AddFileToList)
	ON_MESSAGE(WM_REFUSEFILE, EM_RefuseFile)
	ON_MESSAGE(WM_REPLYBCMSG, EM_ReplyUDPMsg)
	ON_MESSAGE(WM_USERLOGOUT, EM_UserLogout)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_MESSAGE(WM_EMDBLCLICKTREE, OnEM_DblClickTree)
	ON_MESSAGE(WM_EMREFLESH, EM_Reflesh)
	ON_MESSAGE(WM_EMSENDALL, EM_SendAll)
	ON_MESSAGE(WM_EMPROPERTY, EM_EMProperty)
	ON_MESSAGE(WM_REQUESTVOICE, EM_Voice_Income)
	ON_MESSAGE(WM_CANCELREQUESTVOICE, EM_Voice_CancelIncome)
	ON_MESSAGE(WM_DONOTACCEPTCHAT, EM_Voice_DoNotAcceptChat)
	ON_MESSAGE(WM_ACCEPTCHAT, EM_Voice_DestAcceptChat)
	ON_MESSAGE(WM_SHAKEWINDOW, EM_ShakeWindow)
	ON_MESSAGE(WM_EMHISTORY, EM_ViewHistory)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_WM_ENDSESSION()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, OnRclickTree1)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_MESSAGE(WM_DESKUPDATEINFO, _Dest_User_Update)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEIM02Dlg message handlers

BOOL CEIM02Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// �߳�ͬ��
	InitializeCriticalSection(& cs);

	// FreeEIM ��ʼ������
	EM_OnInitDlg();

	// ����
	InitUI();

	// ��ʷ��¼�ļ���
	//CreateToday_sFile();

	// ������
	EM_CheckForUpdate("http://www.freeeim.com/?v=3.28");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEIM02Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (SC_MINIMIZE == nID)
	{
		ShowWindow(SW_HIDE);
		m_sysTray.ShowIcon();
		return;
	}
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

// ����µ��ļ����б��б�ֻ�����256
void CEIM02Dlg::EM_AddFileToList(WPARAM wParam, LPARAM lParam)
{
	for (int i=0; i<256; i++)
	{
		if (m_fInfo[i] == NULL)
		{
			m_fInfo[i] = new EM_FILEINFO;
			memcpy(m_fInfo[i], (void*)wParam, sizeof(EM_FILEINFO));
			break;
		}
	}
}
void CEIM02Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEIM02Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// FreeEIM ��ʼ������
void CEIM02Dlg::EM_OnInitDlg()
{
	// �����ļ������߳�
	m_fileRecv.Run(this);

	// ��½��Ϣ�����߳�����
	m_UserLogin.Run(m_treeCtrlList);

	// ��Ϣ�����߳�����
	m_MSGrecv.Run(m_hWnd);
	SetTimer(33, 1888, NULL);

	// ���������Ի������߳�
	// ����¼�������߳�
	m_cw = (CEIMChatingWaiter*)
		AfxBeginThread(RUNTIME_CLASS(CEIMChatingWaiter),THREAD_PRIORITY_HIGHEST);

}

LRESULT CEIM02Dlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Delegate all the work back to the default implementation in CSystemTray.
	return m_sysTray.OnTrayNotification(wParam, lParam);	
}

void CEIM02Dlg::OnTrayRestore()
{
	ShowWindow(SW_SHOW);
/*	if (m_pAdv)
	{
		if (::IsWindow(m_pAdv->GetSafeHwnd()))
			m_pAdv->UpdateWindow();
	}*/
//	m_sysTray.HideIcon();
}

void CEIM02Dlg::OnQuit()
{
	// TODO: Add your command handler code here
	//SendMessage(WM_CLOSE);
//	OnCancel();
	if (m_bIsLogin)
	{
		CString strInfo;
		strInfo.Format(_T("Ҫ�˳� %s��"), APP_TITLE);
		if (IDYES == MessageBox(strInfo, _T("����"), MB_YESNO|MB_ICONQUESTION))
		{
			EM_QuitEIM();
		}
	}
	else
	{
		EM_QuitEIM();
	}
}

void CEIM02Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

HBRUSH CEIM02Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(CTLCOLOR_DLG  ==nCtlColor)
        return m_Brush;
    else if(CTLCOLOR_STATIC   == nCtlColor )
    {
        pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crStatic);
        return m_Brush;
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// �������������Ĵ���
void CEIM02Dlg::EM_HideLoading()
{
	m_pictureLoading.Stop();
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_LOADGIF), SW_HIDE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_LOADING), SW_HIDE);
	// ��ԭ�Ի�����ɫ
//	m_Brush.m_hObject = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	m_crStatic = RGB(10,36,106);
	// ��ʾTab ����
//	m_tabCtrlFunction.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_OL)->ShowWindow(SW_SHOW);
	// ��ʾ�û��б�
	m_treeCtrlList.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_LINE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_GROUP)->ShowWindow(SW_SHOW);
	RedrawWindow();

	m_bIsLogin = TRUE;



	//////////////////////////////////////////////////////////////////////////
	// ���ģ��
/*	m_pAdv = new _DLG_Advertise;
	m_pAdv->Create(_DLG_Advertise::IDD, this);
	if (::IsWindow(m_pAdv->GetSafeHwnd()))
	{
		CRect rtTree;
		m_treeCtrlList.GetWindowRect(& rtTree);
		ScreenToClient(& rtTree);
		CRect rtAdv;
		m_pAdv->GetWindowRect(& rtAdv);
		ScreenToClient(& rtAdv);
		rtAdv.top = rtTree.bottom;
		rtAdv.bottom = rtTree.bottom + rtAdv.bottom;
		rtAdv.right = rtTree.right;

		m_pAdv->MoveWindow(& rtAdv);
		m_pAdv->ShowWindow(SW_SHOW);
	}*/
	//////////////////////////////////////////////////////////////////////////
}

void CEIM02Dlg::OnAbouteim() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CEIM02Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 33)
	{
		EM_HideLoading();
		m_UserLogin.OnLine();
		SetTimer(22, 268, NULL);
		KillTimer(nIDEvent);
	}
	if (nIDEvent == 22)
	{
	//	m_treeCtrlList.Expand(m_treeCtrlList.GetRootItem(), TVE_EXPAND);
		SetTimer(55, 1888, NULL);
		KillTimer(nIDEvent);
	}
	if (nIDEvent == 55)
	{
		m_UserLogin.OnLine();
		SetTimer(66, 3888, NULL);
		KillTimer(nIDEvent);
	}
	if (nIDEvent == 66)
	{
		m_UserLogin.OnLine();
		KillTimer(nIDEvent);
		SetTimer(88, 10000, NULL);
	}
	if (nIDEvent == 88)
	{
		EM_Reflesh(NULL, NULL);
	}
	CDialog::OnTimer(nIDEvent);
}


// ѡ�����û���˫�������жԻ�
void CEIM02Dlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_treeCtrlList.GetSelectedItem();

	// ����Ƿ���
	if (m_treeCtrlList.ItemHasChildren(hItem))
	{
		*pResult = 0;
		return;
	}
	else
	{
		// ��ȡ�Ի���
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		// Ϊʲô��ֱ�� pDlg->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ��֪����ʲô������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)1);
	}
	*pResult = 0;
}


// ˫��User list treectrl��Ϣ����
void CEIM02Dlg::OnEM_DblClickTree(WPARAM wParam, LPARAM lParam)
{
	EM_MsgDlg *pDlg = (EM_MsgDlg*)wParam;

/*	char tmp[128];
	sprintf(tmp, "�� %s �ĶԻ�", pDlg->m_szDisplayName);
	m_Chatdlg[(int)wParam]->SetWindowText(tmp);*/

	if (1 == lParam)
	{
		pDlg->ShowWindow(SW_NORMAL);
	}
	else if (2 == lParam)
	{
		if(! (GetWindowLong(pDlg->m_hWnd, GWL_STYLE) & WS_VISIBLE))
		{
			pDlg->ShowWindow(SW_MINIMIZE);

			/// 2009��12��3��
			/*CWnd* pParentWnd = GetTopLevelParent();
			if ( HINSTANCE hUser = LoadLibrary( _T("User32") ) )
			{
				BOOL (WINAPI *pfnFlashWindowEx)(PFLASHWINFO pfwi);
				(FARPROC&)pfnFlashWindowEx = GetProcAddress( hUser, "FlashWindowEx" );
				if ( pfnFlashWindowEx )
				{
					FLASHWINFO pFWX;
					pFWX.cbSize		= sizeof(pFWX);
					pFWX.dwFlags	= FLASHW_ALL | FLASHW_TIMERNOFG;
					pFWX.uCount		= 3;
					pFWX.dwTimeout	= 0;
					pFWX.hwnd		= pDlg->m_hWnd;//pParentWnd->GetSafeHwnd();
					(*pfnFlashWindowEx)( &pFWX );
				}
				FreeLibrary( hUser );
			}*/
			/// 2009��12��3��
		}
		else
		{
			if ( GetForegroundWindow() != GetTopLevelParent() )
			{
				//CWnd* pParentWnd = GetTopLevelParent();
				/// 2009��12��3��
			/*	if ( HINSTANCE hUser = LoadLibrary( _T("User32") ) )
				{
					BOOL (WINAPI *pfnFlashWindowEx)(PFLASHWINFO pfwi);

					(FARPROC&)pfnFlashWindowEx = GetProcAddress( hUser, "FlashWindowEx" );
					if ( pfnFlashWindowEx )
					{
						FLASHWINFO pFWX;

						pFWX.cbSize		= sizeof(pFWX);
						pFWX.dwFlags	= FLASHW_ALL | FLASHW_TIMERNOFG;
						pFWX.uCount		= 3;
						pFWX.dwTimeout	= 0;
						pFWX.hwnd		= pDlg->m_hWnd;//pParentWnd->GetSafeHwnd();

						(*pfnFlashWindowEx)( &pFWX );
					}
					FreeLibrary( hUser );
				}*/
				/// 2009��12��3��
			}
		}
	}
}


// ��ʱȡ����ԭ���Ǻ����޷�ִ�� 2009��12��3��
void CEIM02Dlg::OnHelptopic() 
{
	// TODO: Add your command handler code here
//	MessageBox("��������");
//	HtmlHelp(NULL, "help.chm", HH_DISPLAY_TOPIC, 0);
}



// �鿴��ʷ��¼
void CEIM02Dlg::OnViewHistory()
{
	EM_ViewHistory(NULL, NULL);
}// End �鿴��ʷ��¼





// �Է�����Ϣ����������������
void CEIM02Dlg::EM_NewMsg(WPARAM wParam, LPARAM lParam)
{
	char *pBuf = (char*)wParam;

	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		pDlg->AddRecvText(pBuf);

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}
	else
	{
		CString strInfo;
		strInfo.Format("����[%s]����Ϣ", szIP);
		MessageBox(pBuf, strInfo, MB_OK | MB_ICONINFORMATION);
	}


	// ������Ϣ��ʾ����
/*	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat(szDir, "sounds\\");
	strcat(szDir, "sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);*/

	delete pBuf;
}




// �Է����㷢���ļ�
void CEIM02Dlg::EM_NewFile(WPARAM wParam, LPARAM lParam)
{
	LPEM_FILEINFO pFinfo = (LPEM_FILEINFO)wParam;
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);


		pDlg->NewFile(pFinfo);
		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}

	// ������Ϣ��ʾ����
/*	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat(szDir, "sounds\\");
	strcat(szDir, "sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);*/

	// ��̬����ģ�����Ҫɾ��
	delete pFinfo;
}



// �ܾ������ļ�
void CEIM02Dlg::EM_RefuseFile(WPARAM wParam, LPARAM lParam)
{
	LPEM_FILEINFO pFinfo = (LPEM_FILEINFO)wParam;
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		pDlg->File_DestDenyFile(pFinfo);
	}



	// ��̬����ģ�����Ҫɾ��
	delete pFinfo;
}





// �Է�ͬ������ļ�
void CEIM02Dlg::EM_DestAcceptFile(WPARAM wParam, LPARAM lParam)
{
	//9999999999999
	LPEM_FILEINFO pFinfo = (LPEM_FILEINFO)wParam;
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		pDlg->File_DestAcceptFile(pFinfo);
	}
	// ��̬����ģ�����Ҫɾ��
	delete pFinfo;
}




void CEIM02Dlg::OnQuiteim()
{
	// TODO: Add your command handler code here
	//SendMessage(WM_CLOSE);
//	OnQuiteim();
	OnQuit();
}

void CEIM02Dlg::EM_UserSetting()
{
	// TODO: Add your command handler code here
	FreeEIM_UserSettingDlg dlg;
	dlg.DoModal();
}

/* 2009��12��3�� // �����û��Զ��������������������
void CEIM02Dlg::EM_OnAutorun() 
{
	// TODO: Add your command handler code here
	CRegistry regMyReg( NULL );
	if (regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", HKEY_CURRENT_USER))
	{

		CMenu *pMenu = GetMenu();
		// �Ѵ�
		if ((pMenu->GetMenuState(IDM_AUTORUN, MF_BYCOMMAND)&MF_CHECKED))
		{
			// ��������ֵ
			regMyReg["tsapp"] = "";

			pMenu->CheckMenuItem(IDM_AUTORUN, MF_BYCOMMAND|MF_UNCHECKED);
		}
		else // δ��
		{
			// ��������ֵ
			char szFileName[_MAX_PATH];
			::GetModuleFileName(NULL, szFileName, _MAX_PATH);
			strcat(szFileName, " system_tray");
			regMyReg["tsapp"] = szFileName;

			pMenu->CheckMenuItem(IDM_AUTORUN, MF_BYCOMMAND|MF_CHECKED);
		}

		pMenu->Detach();
		regMyReg.Close();
	}

}*/

void CEIM02Dlg::OnEndSession(BOOL bEnding) 
{
	EM_QuitEIM();
}

// �����˳�����
void CEIM02Dlg::EM_QuitEIM()
{
	// �˳�����
	m_sysTray.HideIcon();
	m_sysTray.RemoveIcon();
	m_UserLogin.OffLine();
	m_UserLogin.Close();
	m_MSGrecv.Close();
	m_fileRecv.Close();

	// �����ڴ�
	/*
	for (int i=0; i<256; i++)
	{
		if (m_Chatdlg[i] != NULL)
		{
			delete [] m_Chatdlg[i];
			m_Chatdlg[i] = NULL;
		}
	}*/
	// �ȴ������߳̽��� // ���� 2008-01-19
//	Sleep(888); 

	CDialog::OnCancel();
}

// �ظ��û�BC��½��Ϣ������UDP��½��Ϣ
void CEIM02Dlg::EM_ReplyUDPMsg(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)wParam;
	m_UserLogin.SendLogin_UDP(szIP);

	// Ҫɾ������ֹ�ڴ�й¶
	delete szIP;
}

// �Ҽ��˵� -> ˢ��
void CEIM02Dlg::EM_Reflesh(WPARAM wParam, LPARAM lParam)
{
//	m_treeCtrlList.Expand(m_treeCtrlList.GetRootItem(), TVE_COLLAPSE);
//	SetTimer(22, 268, NULL);
	m_UserLogin.OnLine();
}

// �Ҽ��˵� -> Ⱥ��
void CEIM02Dlg::EM_SendAll(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("7788999Ⱥ���������ڸ�����");
/*	if (NULL == m_Chatdlg[255])
	{
		m_Chatdlg[255] = new EM_MsgDlg;
		m_Chatdlg[255]->m_szDisplayName = "������";
		m_Chatdlg[255]->m_szUserPCName = "asdf--->>>xxxxxxasiew*3234234*sdfsdf";
		m_Chatdlg[255]->Create(IDD_DIALOG1, GetDesktopWindow());
		m_Chatdlg[255]->GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_HIDE);
		m_Chatdlg[255]->GetDlgItem(IDC_VOICECHAT)->ShowWindow(SW_HIDE);

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)255, (LPARAM)1);
	}
	else
	{
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)255, (LPARAM)1);
	}*/
}

// �Ҽ��˵� -> ����
void CEIM02Dlg::EM_EMProperty(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("EM_EMProperty");
	HTREEITEM hItem = m_treeCtrlList.GetSelectedItem();

//	CString str=m_treeCtrlList.GetItemText(hItem);
	EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
//	int index = m_treeCtrlList.GetItemData(hItem);

	EM_UserProperty *pUser = new EM_UserProperty;
	pUser->Create(IDD_DIALOG2, GetDesktopWindow());
	pUser->SetDlgItemText(IDC_EDIT1, pDlg->m_strDisplayName);
	pUser->SetDlgItemText(IDC_EDIT3, pDlg->m_strUserIP);
	pUser->SetDlgItemText(IDC_EDIT2, ""/*WENTI*/);
	char buf[18];
	GetMAC(pDlg->m_strUserIP, buf);
	pUser->SetDlgItemText(IDC_EDIT4, buf);
	pUser->ShowWindow(SW_NORMAL);
}

void CEIM02Dlg::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD SelectionMade;
	UINT flags;
	CPoint point;
	GetCursorPos(&point);
	m_treeCtrlList.ScreenToClient(&point);

	HTREEITEM hItem = m_treeCtrlList.HitTest(point, &flags);


	if(hItem && (flags & TVHT_ONITEM) && !(flags & TVHT_ONITEMRIGHT))
	{
		if (m_treeCtrlList.ItemHasChildren(hItem))
		{
		}
		else
		{
			m_treeCtrlList.SelectItem(hItem);
			ClientToScreen(&point);
			CMenu add;
			VERIFY(add.CreatePopupMenu());
			VERIFY(add.AppendMenu(MF_STRING, 33, _T("������Ϣ")));
			VERIFY(add.AppendMenu(MF_STRING, 66, _T("�鿴��¼")));
			VERIFY(add.AppendMenu(MF_STRING, 55, _T("����")));
			SelectionMade = add.TrackPopupMenu(TPM_LEFTALIGN |
				TPM_LEFTBUTTON |
				TPM_NONOTIFY |
				TPM_RETURNCMD,
				point.x, point.y, this);

			add.DestroyMenu();
		}
	}
	else
	{
		ClientToScreen(&point);
		CMenu add;
		VERIFY(add.CreatePopupMenu());
		VERIFY(add.AppendMenu(MF_STRING, 11, _T("ˢ��")));
		VERIFY(add.AppendMenu(MF_STRING, 22, _T("Ⱥ��")));
		SelectionMade = add.TrackPopupMenu(TPM_LEFTALIGN |
			TPM_LEFTBUTTON |
			TPM_NONOTIFY |
			TPM_RETURNCMD, point.x, point.y, this);
		add.DestroyMenu();
	}
	switch(SelectionMade)
	{
		// ..... do something here .....
	case 11:									// ˢ��
		PostMessage(WM_EMREFLESH);
		break;
	case 22:									// Ⱥ��
		PostMessage(WM_EMSENDALL);
		break;
	case 33:									// ������Ϣ
		{
			OnDblclkTree1(pNMHDR, pResult);
		}
		break;
	case 55:									// ����
		PostMessage(WM_EMPROPERTY);
		break;
	case 66:
		{
			PostMessage(WM_EMHISTORY);
			break;
		}
	}
	*pResult = 0;
}


void CEIM02Dlg::OnRelesh() 
{
	// TODO: Add your command handler code here
	EM_Reflesh(NULL, NULL);
}

void CEIM02Dlg::SendMSGToAll(LPEM_DATA lpData)
{
	MessageBox("SendMSGToAll");
/*	ASSERT( NULL != lpData );
	for (int i=0; i<255; i++)
	{
		if (NULL != m_treeCtrlList.m_szPCName[i])
		{
			m_MSGrecv.SendMsg(m_treeCtrlList.m_szUserIP[i], lpData);
		}
	}*/
}

void CEIM02Dlg::OnSendall() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_EMSENDALL);
}

void CEIM02Dlg::OnLanexplorer() 
{
	// TODO: Add your command handler code here
	WinExec("lanspy.exe", SW_SHOW);
}

void CEIM02Dlg::OnLockscreen() 
{
	// TODO: Add your command handler code here
	WinExec("locknt.exe", SW_HIDE);
}

void CEIM02Dlg::OnClosescreen() 
{
	// TODO: Add your command handler code here
	::SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

void CEIM02Dlg::EM_UpdateUserInfo(LPCTSTR lpszName, LPCTSTR lpszGroup)
{
	CString strGroup;
	strGroup = lpszGroup;
	CString strName;
	strName = lpszName;

	if (strGroup != m_config.m_strGroup)
	{
		m_config.m_strGroup = lpszGroup;
	//	g_uiLocal.SetGroupName();
	//	strcpy(m_szLocalGroup, lpszGroup);
		// �����
		m_UserLogin.ReGroup(lpszGroup);
	}
	if (strName != m_config.m_strDisplayName)
	{
	//	g_uiLocal.SetDisplayName();
		m_config.m_strDisplayName = lpszName;
	//	strcpy(m_szLocalName, lpszName);
		// �����
		m_UserLogin.ReName(lpszName);
	}

	m_config.SaveConfig();
/*	CString str;
	str = m_szLocalGroup;
	str += m_szLocalName;
	SetWindowText(str);*/
}

void CEIM02Dlg::EM_UserLogout(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)wParam;
//	MessageBox(szIP);
//	_User_DeleteUser();
//	HTREEITEM hItem = _User_GetUserItem(szIP);
//	if (NULL != hItem)
//	{
//		_Msg_DelDialog(hItem);
//	}

	map<HTREEITEM, string>::iterator _i;
	map<HTREEITEM, string>::iterator _e;
	_i = m_mapUsers.begin();
	_e = m_mapUsers.end();

	for (_i; _i!=_e; _i++)
	{
		if (! stricmp(szIP, _i->second.c_str()))
		{
			_Msg_DelDialog(_i->first);
			m_treeCtrlList.DeleteItem(_i->first);
			m_mapUsers.erase(_i);
			return;
		}
	}

	// Ҫɾ������ֹ�ڴ�й¶
	delete szIP;

	CString strContact;
	strContact.Format(_T("��������(%d)"), m_mapUsers.size());
	SetDlgItemText(IDC_STATIC_OL, strContact);
}

void CEIM02Dlg::OnSnow() 
{
	// TODO: Add your command handler code here
	WinExec("snow.exe", SW_SHOW);
}

void CEIM02Dlg::OnWininfo() 
{
	// TODO: Add your command handler code here
	WinExec("wininfo.exe", SW_SHOW);
}

void CEIM02Dlg::OnDiskname() 
{
	// TODO: Add your command handler code here
	WinExec("panfu.exe", SW_SHOW);
}

void CEIM02Dlg::OnNote() 
{
	// TODO: Add your command handler code here
	WinExec("notebook.exe", SW_SHOW);
}
void CEIM02Dlg::OnOK()
{
	//CDialog::OnOK();
}
void CEIM02Dlg::OnCancel()
{
	return;
/*	if (m_bIsLogin)
	{
		CString strInfo;
		strInfo.Format(_T("Ҫ�˳� %s��"), APP_TITLE);
		if (IDYES == MessageBox(strInfo, _T("����"), MB_YESNO|MB_ICONQUESTION))
		{
			EM_QuitEIM();
		}
	}
	else
	{
		EM_QuitEIM();
	}*/
}

// ɾ���ļ��б���wParamָ�����ļ�
// �ظ��Է����Ҿܾ����������ļ�
void CEIM02Dlg::EM_DeleteFileFromList(WPARAM wParam, LPARAM lParam)
{
	// �ظ��Է����Ҿܾ����������ļ�
	EM_FILEINFO finfo;
	finfo.AddFile("c:\\outlook.pst", "pst");
	EM_DATA data;
	data.msg = EM_REFUSEFILE;
	data.buf = (char*)&finfo;
	data.len = sizeof(EM_FILEINFO);
	m_MSGrecv.SendMsg((char*)lParam, &data);
}

// ��ʼ����wParamָ�����ļ�
void CEIM02Dlg::EM_DownloadFile(WPARAM wParam, LPARAM lParam)
{
	for (int i=0; i<256; i++)
	{
		if (m_fInfo[i] != NULL)
		{
			int current = m_fInfo[i]->index1;
			if (current == (int)wParam)
			{
				CString strFileExt;
				strFileExt.Format("*.%s|*.%s||", m_fInfo[i]->FileExt(), m_fInfo[i]->FileExt());

				CFileDialog filedlg(FALSE, NULL, NULL,
					OFN_HIDEREADONLY |
					OFN_OVERWRITEPROMPT,
					strFileExt, NULL);

				if (IDOK == filedlg.DoModal())
				{
				/*	m_fileSender.RecvFile(m_fInfo[i],	// Ҫ���ص��ļ���Ϣ
						(LPCTSTR)filedlg.GetPathName(), // ���Ϊ �ļ���
						(char*)lParam);					// ���ͷ�������*/
				}
				else
				{
					char *szIP = (char*)lParam;

					HTREEITEM hItem = _User_GetUserItem(szIP);
					if (NULL != hItem)
					{
						EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
						pDlg->File_RecvCancel(m_fInfo[i]);
					}

					// ������Ҫ���ļ��б���ɾ��
					// ������һ��Ҫ�ŵ����
					EM_DeleteFileFromList((WPARAM)m_fInfo[i]->index2, lParam);
				}
			}
		}
	}
}


void CEIM02Dlg::FileComming(WPARAM wParam, LPARAM lParam)
{
	SOCKET	Sock = (SOCKET)wParam;
	// ���ݶԷ�IP��ַ��ȷ�϶�Ӧ�ĶԻ���
	
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
		pDlg->File_RecvFile(Sock);
	}
	

/*	int nIndex = m_treeCtrlList.GetUserIndexByIP(szDestIP);
	if (-1 == nIndex)
		return;
*/
	// �ڶԻ�������ʾ���ս���
//	m_Chatdlg[nIndex]->File_RecvFile(Sock);

	delete szIP;
}

// �ļ��������
void CEIM02Dlg::EM_FileSendCompleted(WPARAM wParam, LPARAM lParam)
{

}

void CEIM02Dlg::OnRecvChat(WPARAM wParam, LPARAM lParam)
{
}




void CEIM02Dlg::EM_Voice_CancelIncome(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
		pDlg->Voice_CancelIncome(nValue);

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}
}






// �Է����������Ի�����
void CEIM02Dlg::EM_Voice_DestAcceptChat(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
		pDlg->Voice_DestAcceptChat();
		
		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}
}
// end �Է����������Ի�����





void CEIM02Dlg::EM_Voice_DoNotAcceptChat(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		pDlg->Voice_DoNotAcceptChat(nValue);

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}
}

// �Է����㷢�������Ի�����
void CEIM02Dlg::EM_Voice_Income(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		// һ��ֻ��һ�������Ի�
		if (m_bVoiceChat)
		{
			pDlg->Voice_DoNotAcceptChat(0);
		}
		else
		{
			pDlg->Voice_Income();
		}

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);
	}

//	delete pBuf;

}// End �Է����㷢�������Ի�����




// �鿴��Է�����ʷ��¼
void CEIM02Dlg::EM_ViewHistory(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hiUser = m_treeCtrlList.GetSelectedItem();
	if (m_treeCtrlList.ItemHasChildren(hiUser))
	{
		AfxMessageBox("��ѡ��һ���û����鿴�����������¼��");
		return;
	}

	CString str;
	str = m_treeCtrlList.GetItemText(hiUser);
	EIM_ViewHistory *pDlg;
	pDlg = new EIM_ViewHistory;
	pDlg->Create(IDD_VIEWHISTORY, GetDesktopWindow());
	pDlg->ShowWindow(SW_NORMAL);
	pDlg->ReadUserHistory((LPCTSTR)str);
	pDlg->UpdateWindow();
//	AfxMessageBox(str);
}// End �鿴��Է�����ʷ��¼



// �Է����󴰿ڶ���
void CEIM02Dlg::EM_ShakeWindow(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)lParam;

	HTREEITEM hItem = _User_GetUserItem(szIP);
	if (NULL != hItem)
	{
		EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		PostMessage(WM_EMDBLCLICKTREE, (WPARAM)pDlg, (LPARAM)2);

		pDlg->ShowWindow(SW_NORMAL);
		pDlg->Shake_Window();
	}

}// End �Է����󴰿ڶ���


void CEIM02Dlg::CreateToday_sFile()
{
	CFileFind cff;
	CTime ntime = CTime::GetCurrentTime();
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat(buf, "history\\");
	CString fileName;
	fileName.Format("%s%s", buf, ntime.Format("%y-%m-%d"));
//	MessageBox(fileName);
	if (cff.FindFile(fileName))
	{
		// Do nothing if directory existed.
	//	AfxMessageBox("File existed.");
	}
	else
	{
		// Create the directory.
	//	if (IDYES ==
	//		AfxMessageBox("File don't existed.\nDo you want to create it.", MB_YESNO))
		{
			CreateDirectory(buf, NULL);
			strcat(buf, (LPCTSTR)ntime.Format("%y-%m-%d"));
			CreateDirectory(buf, NULL);
		}
	}

}

void CEIM02Dlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	OnHelptopic();
//	CDialog::WinHelp(dwData, nCmd);
}

void CEIM02Dlg::InitUI()
{
/*	CRect rtMain;
	GetWindowRect(& rtMain);
	ScreenToClient(& rtMain);*/

//	strcpy(m_szLocalName, (LPCTSTR)m_config.m_strDisplayName);
//	strcpy(m_szLocalGroup, (LPCTSTR)m_config.m_strGroup);

	// ���öԻ���ı�����ɫ
	m_Brush.CreateSolidBrush(RGB(255,255,255));
	m_crStatic = RGB(41,109,193);

	// ���ִ���������
	GetDlgItem(IDC_STATIC_OL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_LINE)->ShowWindow(SW_HIDE);
	m_treeCtrlList.ShowWindow(SW_HIDE);

	// �û��б�������ɫ
	m_treeCtrlList.SetTextColor(RGB(25, 79, 149));
	// ���������û��б�------------------------------------------------------------------
	CBitmap bm;
	m_ilUser.Create (16, 16, ILC_COLOR32 , 5,5);
	bm.LoadBitmap(IDB_BITMAP1);
	m_ilUser.Add(& bm, RGB(255,0,255));
	m_treeCtrlList.SetImageList(& m_ilUser, TVSIL_NORMAL);
	//===================================================================================

	// �������洴��----------------------------------------------------------------------
	if (m_pictureLoading.Load(MAKEINTRESOURCE(IDR_GIFLOADING),_T("GIF")))
		m_pictureLoading.Draw();
	m_pictureLoading.SetBkColor(RGB(255,255,255));
	//===================================================================================
	
	// ϵͳ����ͼ�괴��------------------------------------------------------------------
	char buf[512];
	LoadString(NULL, IDS_STRING_TITLE, buf, 512);
	if (!m_sysTray.Create(this, WM_ICON_NOTIFY, buf, m_hIcon, IDR_SYSTEMTRAY))
	{
		MessageBox("System tray creating error.");
		return;
	}
//	else m_sysTray.HideIcon(); // ������
	//===================================================================================

	// ����Tab Tools---------------------------------------------------------------------
	CBitmap bm2;
	bm2.LoadBitmap(IDB_LIST);
	m_ilTools.Create(16, 16, ILC_COLOR32, 5, 5);
	m_ilTools.Add(& bm2, RGB(255,0,255));
	//===================================================================================

	// ����ǿ������������Զ���С����ϵͳ����
	if (! strcmp(AfxGetApp()->m_lpCmdLine, "system_tray"))
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}

	
	// ���������������
	SetWindowText(APP_TITLE);
	// ==============================================================================

/*	CRect rct;
	CRgn rgn;

	GetWindowRect( &rct );
	rgn.CreateRoundRectRgn( rct.left, rct.top, rct.right, rct.bottom, 9, 9 );
	SetWindowRgn( rgn, TRUE );*/
}


// �ж��û��Ƿ��Ѵ��ڣ����ڷ��� FALSE
BOOL CEIM02Dlg::_User_IsNewUser(LPCTSTR lpszIP)
{
	map<HTREEITEM, string>::iterator _i;
	map<HTREEITEM, string>::iterator _e;
	_i = m_mapUsers.begin();
	_e = m_mapUsers.end();

	string strIP;
	strIP = lpszIP;
	for (_i; _i!=_e; _i++)
	{
		if (strIP == _i->second)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// ���һ�����û�
void CEIM02Dlg::_User_AddNewUser(LPCTSTR lpszName, EM_USERINFO & ui)
{
	EnterCriticalSection(& cs);

	if (_User_IsNewUser(ui.m_szIP))
	{
		HTREEITEM hGroup = _User_GetGroupItem(ui.m_szGroup);
		HTREEITEM hItem = m_treeCtrlList.InsertItem(lpszName, 4, 4, hGroup);
		m_treeCtrlList.SetItemData(hItem, NULL);
		m_treeCtrlList.SortChildren(hGroup);

		m_mapUsers.insert(pair<HTREEITEM, string>(hItem, ui.m_szIP));
	}

	LeaveCriticalSection(& cs);

	CString strContact;
	strContact.Format(_T("��������(%d)"), m_mapUsers.size());
	SetDlgItemText(IDC_STATIC_OL, strContact);
}



// �û����������Ϣ
void CEIM02Dlg::_User_UserUpdate(LPCTSTR lpszName, EM_USERINFO & ui)
{
	AfxMessageBox("_User_UserUpdate");
	//m_treeCtrlList.ItemReName((LPSTR)lpszName, ui);
}

// �û����������Ϣ
void CEIM02Dlg::_User_UserUpdate(LPCTSTR lpszIP, LPCTSTR lpszName, LPCTSTR lpszGroup)
{
	ASSERT(NULL != lpszIP);

	CString strName;
	CString strGroup;
	CString strIP;

	EnterCriticalSection(& cs);

	map<HTREEITEM, string>::iterator _i;
	map<HTREEITEM, string>::iterator _e;
	_i = m_mapUsers.begin();
	_e = m_mapUsers.end();

	strIP = lpszIP;

	for (_i; _i!=_e; _i++)
	{
		if (strIP == _i->second.c_str())
		{
			HTREEITEM hItem = _i->first;
			HTREEITEM hParent = m_treeCtrlList.GetParentItem(hItem);

			strName = m_treeCtrlList.GetItemText(hItem);
			m_treeCtrlList.DeleteItem(hItem);
			if (! m_treeCtrlList.ItemHasChildren(hParent))
			{
				m_treeCtrlList.DeleteItem(hParent);
			}
			m_mapUsers.erase(_i);
			_i = _e;
		}
	}

	HTREEITEM hGroup = _User_GetGroupItem(lpszGroup);
	HTREEITEM hItem = m_treeCtrlList.InsertItem(strName, 4, 4, hGroup);
	m_mapUsers.insert(pair<HTREEITEM, string>(hItem, lpszIP));

	LeaveCriticalSection(& cs);
}

// ����ָ�������Ƿ���ڣ����ڷ��ظ���ITEM�����򴴽�һ����ITEM
HTREEITEM CEIM02Dlg::_User_GetGroupItem(LPCTSTR lpszGroup)
{
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_treeCtrlList.GetChildItem(TVI_ROOT);
	CString strItemText;

	for(;hChildItem != NULL; hChildItem = hNextItem)
	{
		strItemText = m_treeCtrlList.GetItemText(hChildItem);
		if (strItemText == lpszGroup)
		{
			return hChildItem;
		}
		else
		{
			hNextItem = m_treeCtrlList.GetNextSiblingItem(hChildItem);
		}
	}

	return m_treeCtrlList.InsertItem(lpszGroup, 1, 1);
}


// ��ȡ�Ի���
EM_MsgDlg* CEIM02Dlg::_Msg_GetDialog(HTREEITEM hItem)
{
	EM_MsgDlg *pDlg;

	int index = m_treeCtrlList.GetItemData(hItem);

	// ����Ի���û�б�������index == NULL�������½�һ���Ի���
	if (index == NULL)
	{
		pDlg = new EM_MsgDlg;
		pDlg->m_strDisplayName = m_treeCtrlList.GetItemText(hItem);
		pDlg->m_strUserIP = m_mapUsers[hItem].c_str();

		pDlg->Create(IDD_DIALOG1, GetDesktopWindow());

		m_treeCtrlList.SetItemData(hItem, (int)pDlg);

		return pDlg;
	}
	else
	{
		return (EM_MsgDlg*)index;
	}
}

HTREEITEM CEIM02Dlg::_User_GetUserItem(LPCTSTR lpszIP)
{
	map<HTREEITEM, string>::iterator _i;
	map<HTREEITEM, string>::iterator _e;
	_i = m_mapUsers.begin();
	_e = m_mapUsers.end();

	string strIP;
	strIP = lpszIP;
	for (_i; _i!=_e; _i++)
	{
		if (strIP == _i->second)
		{
			return _i->first;
		}
	}

	return NULL;
}

/* �����û�������������������� 2009��12��3��
void CEIM02Dlg::_Init_Autorun()
{
	CRegistry regMyReg( NULL );
	if ( regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", HKEY_CURRENT_USER) )
	{
		CMenu *pMenu = GetMenu();
		// ע�������
		if ( regMyReg["tsapp"].Exists() )
		{
		//	pMenu->CheckMenuItem(IDM_AUTORUN, MF_BYCOMMAND|MF_CHECKED);
			CString strValue = (LPTSTR)regMyReg["tsapp"];

			if (strValue.IsEmpty())
			{	// �հ״���������
			}
			else
			{	// ��������ֵ
				char szFileName[_MAX_PATH];
				::GetModuleFileName(NULL, szFileName, _MAX_PATH);
				strcat(szFileName, " system_tray");
				regMyReg["tsapp"] = szFileName;
				pMenu->CheckMenuItem(IDM_AUTORUN, MF_BYCOMMAND|MF_CHECKED);
			}
		}
		else // ������
		{
			// ��������ֵ
			char szFileName[_MAX_PATH];
			::GetModuleFileName(NULL, szFileName, _MAX_PATH);
			strcat(szFileName, " system_tray");
			regMyReg["tsapp"] = szFileName;
			pMenu->CheckMenuItem(IDM_AUTORUN, MF_BYCOMMAND|MF_CHECKED);
		}

		pMenu->Detach();
		regMyReg.Close();
	}
	else
	{
		AfxMessageBox("Unable to open key!", MB_OK | MB_ICONHAND);
	}
}
*/




// �Է��û��޸Ĳ�������
void CEIM02Dlg::_Dest_User_Update(WPARAM wParam, LPARAM lParam)
{
	DWORD dwMsg = wParam;
	FreeEIM_USER *pUser = (FreeEIM_USER*)lParam;

	switch (dwMsg)
	{
	case EM_USERREGROUP:
		_Dest_User_Update_Group(pUser);
		break;

	case EM_USERRENAME:
		_Dest_User_Update_Name(pUser);
		break;
	}

	delete pUser;
}

void CEIM02Dlg::_Dest_User_Update_Group(FreeEIM_USER *pUser)
{
	map<HTREEITEM, string>::iterator _iter;
	map<HTREEITEM, string>::iterator _end;
	_iter = m_mapUsers.begin();
	_end = m_mapUsers.end();

	DWORD itemData;
	for (_iter; _iter!=_end; _iter++)
	{
		if (! stricmp(_iter->second.c_str(), pUser->m_szIP))
		{
			HTREEITEM hItem = _iter->first;
			HTREEITEM hParent = m_treeCtrlList.GetParentItem(hItem);
			strcpy(pUser->m_szName, m_treeCtrlList.GetItemText(hItem));

			//  �Ի����ڲ�Ҫɾ�������º����ʹ��
			itemData = m_treeCtrlList.GetItemData(hItem);

			m_treeCtrlList.DeleteItem(hItem);
			if (! m_treeCtrlList.ItemHasChildren(hParent))
			{
				m_treeCtrlList.DeleteItem(hParent);
			}
			m_mapUsers.erase(_iter);
			break;
		}
	}

	// ɾ��֮��Ҫ���û���ӻ�ȥ
	HTREEITEM hGroup = _User_GetGroupItem(pUser->m_szGroup);
	HTREEITEM hNewItem = m_treeCtrlList.InsertItem(pUser->m_szName, 4, 4, hGroup);
	m_treeCtrlList.SetItemData(hNewItem, itemData);
	m_mapUsers.insert(pair<HTREEITEM, string>(hNewItem, pUser->m_szIP));

}


void CEIM02Dlg::_Dest_User_Update_Name(FreeEIM_USER *pUser)
{
	HTREEITEM hItem = _User_GetUserItem(pUser->m_szIP);
	if (NULL != hItem)
	{
		m_treeCtrlList.SetItemText(hItem, pUser->m_szName);
	}
}


// �û��˳���ɾ���û��Ի���
void CEIM02Dlg::_Msg_DelDialog(HTREEITEM hItem)
{
	EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);
	pDlg->ShowWindow(SW_HIDE);
	pDlg->DestroyWindow();

	delete pDlg;
	pDlg = NULL;

	m_treeCtrlList.SetItemData(hItem, NULL);
}



void CEIM02Dlg::GetProgramDirectory(char *buf)
{
	ASSERT(NULL != buf);
	
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	buf[nIdx+1] = NULL;
}


CString CEIM02Dlg::GetAppDir()
{
	char buf[MAX_PATH];
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	
	buf[nIdx+1] = NULL;
	CString strRet = buf;

	return strRet;
}


CString CEIM02Dlg::GetComputerName()
{
	CString strPCName = "";

	char buf[512];
	gethostname(buf, 512);

	if (NULL != buf)
		strPCName = strupr(buf);	// ת���ɴ�дͳһ

	return strPCName;
}

void CEIM02Dlg::EM_CheckForUpdate(LPCTSTR lpszURL)
{
	m_webBrowser.Navigate(lpszURL, NULL,NULL,NULL,NULL);
}

LPCTSTR CEIM02Dlg::GetLocalDisplayName()
{
	return (LPCTSTR)m_config.m_strDisplayName;
}
