// CatchScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreen.h"
#include "CatchScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Axtive.h"
#include "Self.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
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
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog

CCatchScreenDlg::CCatchScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatchScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatchScreenDlg)
	m_catchNum = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	result=0;
}

void CCatchScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchScreenDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_catchNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCatchScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CCatchScreenDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_START, OnStart)
	ON_COMMAND(ID_FULL, OnFull)
	ON_COMMAND(ID_AXTIVE, OnAxtive)
	ON_COMMAND(ID_SELF, OnSelf)
	ON_MESSAGE(ICON_NOTIFY,OnNotify)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SHOW, OnShow)
	ON_COMMAND(IDM_HIDE, OnHide)
	ON_COMMAND(ID_FULL_SCREEN, OnFullScreen)
	ON_COMMAND(ID_AXTIVE_WINDOW, OnAxtiveWindow)
	ON_COMMAND(ID_SELF_WINDOW, OnSelfWindow)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_COMMAND(IDM_HIDE, OnHide)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg message handlers

BOOL CCatchScreenDlg::OnInitDialog()
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
	CString strTip;
	strTip="  ����������ܹ���ȡ���ַ�Χ��ȫ����ȡ��������ȡ����ѡ�����ȡ��\r\n\r\n";
	strTip+="  �����������VC++��MFC���������ģ�Ҳ�Ǳ������ϵͳ��Ƶ���ҵ������ˮƽ���ޣ�";
	strTip+="������кܶ�ȱ�ݣ�����ʹ����ָ���䲻��֮����";
	SetDlgItemText(IDC_SHOW,strTip);

	CString str;
	str=_T("�������");
	
	//�����ʼͼ��
	m_hIconTray.hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
	strcpy(m_hIconTray.szTip,(LPCTSTR)str);
	
	//��¼����������ͼ�귢��֪ͨ�Ĵ��ھ��
	m_hIconTray.hWnd=this->GetSafeHwnd();
	
	m_hIconTray.uID=0;
	m_hIconTray.uCallbackMessage=ICON_NOTIFY;
	m_hIconTray.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	
/*	//�����ƺõ�ͼ����뵽ϵͳ������
	Shell_NotifyIcon(NIM_ADD,&m_hIconTray);*/
	
	return FALSE;
//	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCatchScreenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
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

void CCatchScreenDlg::OnPaint() 
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
HCURSOR CCatchScreenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCatchScreenDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CCatchScreenDlg::OnExit() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_CLOSE);
}

void CCatchScreenDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg about;
	about.DoModal();
}

void CCatchScreenDlg::OnStart() 
{
	// TODO: Add your command handler code here
	UpdateData(TRUE);
	switch(result)
	{
		
		//ȫ����ȡ��Ļ
	case 0:
		{			
			result=0;
			Catch();
			break;
		}

		//������ͼ
	case 1:
		{

			result=1;
			Catch();
			break;
		}

		//��ѡ�����ͼ
	case 2:
		{
			result=2;
			Catch();
			break;
		}
	}
	
}

void CCatchScreenDlg::OnFull() 
{
	// TODO: Add your command handler code here
	m_catchNum=0;
	UpdateData(FALSE);
}

void CCatchScreenDlg::OnAxtive() 
{
	// TODO: Add your command handler code here
	m_catchNum=1;
	UpdateData(FALSE);
}

void CCatchScreenDlg::OnSelf() 
{
	// TODO: Add your command handler code here
	m_catchNum=2;
	UpdateData(FALSE);
}

void CCatchScreenDlg::CaptureRect(HDC hdcScreen, LPRECT prc)
{
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,prc->right - prc->left,
		prc->bottom - prc->top);
	SelectObject(hdc,hbmp);
	
	BitBlt(hdc,0,0,prc->right - prc->left,prc->bottom - prc->top,
		hdcScreen,prc->left,prc->top,SRCCOPY);
	
	//��������������
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP,hbmp);
	CloseClipboard();
	
	DeleteDC(hdc);
	::ShowWindow(m_hWnd,SW_SHOW);
}

void CCatchScreenDlg::Catch()
{
	switch(m_catchNum)
	{
		//ȫ����ȡ��Ļ
	case 0:
		{			
			::ShowWindow(m_hWnd,SW_HIDE);    //���ظó��򴰿�
			Sleep(200);
			//��ȡ��Ļ�豸���
			HDC hDeskDC = ::GetDC(NULL);
			RECT rc;
			//��ȡ��Ļ�Ĵ�С
			rc.left = 0;
			rc.top = 0;
			rc.right = rc.left + GetSystemMetrics(SM_CXSCREEN);
			rc.bottom = rc.top + GetSystemMetrics(SM_CYSCREEN);
			//���ÿ���ͼ�񵽼�����ĺ���
			CaptureRect(hDeskDC,&rc);
			AfxMessageBox("��ȡͼ���Ѿ���������������!",64);
			break;
		}
		
		//������ͼ
	case 1:
		{
			this->ShowWindow(SW_HIDE);
			Sleep(100);
			CAxtive axtive;
			axtive.DoModal();
			break;
		}
		
		//��ѡ�����ͼ
	case 2:
		{
			this->ShowWindow(SW_HIDE);
			Sleep(100);
			CSelf self;
			self.DoModal();
			break;
		}
	}

}

LRESULT CCatchScreenDlg::OnNotify(WPARAM uID, LPARAM lEvent)
{
	//��������Ҽ������¼������֪ͨ�򷵻�
	if(lEvent==WM_LBUTTONDBLCLK)
	{
		//ɾ������ͼ��
		Shell_NotifyIcon(NIM_DELETE,&m_hIconTray);
		::ShowWindow(m_hWnd,SW_SHOW);
	}
	
	if(lEvent==WM_RBUTTONUP)
	{
		//��õ�ǰ���λ��
		CPoint mouse;
		GetCursorPos(&mouse);
		
		//���뵯���˵���Դ
		CMenu menu;
		menu.LoadMenu(IDR_MENU2);
		
		//��õ����˵�ʵ����ʾ����
		CMenu *pSubMenu=menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,mouse.x,mouse.y,this);
		
	}
	return 1;
}

void CCatchScreenDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//ɾ������ͼ��
	Shell_NotifyIcon(NIM_DELETE,&m_hIconTray);
}

void CCatchScreenDlg::OnShow() 
{
	// TODO: Add your command handler code here
	//ɾ������ͼ��
	Shell_NotifyIcon(NIM_DELETE,&m_hIconTray);
	this->ShowWindow(SW_SHOW);
}

void CCatchScreenDlg::OnHide() 
{
	// TODO: Add your command handler code here
	//�����ƺõ�ͼ����뵽ϵͳ������
	Shell_NotifyIcon(NIM_ADD,&m_hIconTray);
	this->ShowWindow(SW_HIDE);
}

void CCatchScreenDlg::OnFullScreen() 
{
	// TODO: Add your command handler code here
	
	Sleep(200);
	//��ȡ��Ļ�豸���
	HDC hDeskDC = ::GetDC(NULL);
	RECT rc;
	//��ȡ��Ļ�Ĵ�С
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = rc.top + GetSystemMetrics(SM_CYSCREEN);
	//���ÿ���ͼ�񵽼�����ĺ���
	CaptureRect(hDeskDC,&rc);
	::ShowWindow(m_hWnd,SW_HIDE);
	AfxMessageBox("��ȡͼ���Ѿ���������������!",64);
}

void CCatchScreenDlg::OnAxtiveWindow() 
{
	// TODO: Add your command handler code here
	CAxtive axtive;
	axtive.DoModal();
}

void CCatchScreenDlg::OnSelfWindow() 
{
	// TODO: Add your command handler code here
	Sleep(50);
	CSelf self;
	self.DoModal();
}

UINT CCatchScreenDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nHitTest=CDialog::OnNcHitTest(point);
    return (nHitTest==HTCLIENT||HTMENU)? HTCAPTION:nHitTest;

//	return CDialog::OnNcHitTest(point);
}
