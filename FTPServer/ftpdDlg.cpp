// ftpdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ftpd.h"
#include "ftpdDlg.h"
#include "CFtpd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CList<CFtpd*,CFtpd*> FTPDList;

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
// CFtpdDlg dialog

CFtpdDlg::CFtpdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpdDlg)
	m_ftpserver_name = _T("");
	m_ftpd_port = 0;
	m_rootdir = _T("");
	m_b_ipchk = FALSE;
	m_b_ipchk_pasv = FALSE;
	m_b_switch_pasv_ip = FALSE;
	m_specify_data_port = FALSE;
	m_port_from = 0;
	m_port_to = 0;
	m_b_auto_choose_ip = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFtpdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpdDlg)
	DDX_Control(pDX, IDC_TREE1, m_ftpdtree);
	DDX_Control(pDX, IDC_COMBO1, m_ip_combo_ctrl);
	DDX_Text(pDX, IDC_EDIT1, m_ftpserver_name);
	DDV_MaxChars(pDX, m_ftpserver_name, 32);
	DDX_Text(pDX, IDC_EDIT2, m_ftpd_port);
	DDX_Text(pDX, IDC_EDIT3, m_rootdir);
	DDX_Check(pDX, IDC_CHECK2, m_b_ipchk);
	DDX_Check(pDX, IDC_CHECK3, m_b_ipchk_pasv);
	DDX_Check(pDX, IDC_CHECK1, m_b_switch_pasv_ip);
	DDX_Check(pDX, IDC_CHECK4, m_specify_data_port);
	DDX_Text(pDX, IDC_EDIT4, m_port_from);
	DDX_Text(pDX, IDC_EDIT5, m_port_to);
	DDX_Check(pDX, IDC_CHECK5, m_b_auto_choose_ip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpdDlg, CDialog)
	//{{AFX_MSG_MAP(CFtpdDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENUITEM32771, OnLOAD)
	ON_COMMAND(ID_MENUITEM32772, OnSAVE)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, OnLaunch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpdDlg message handlers

BOOL CFtpdDlg::OnInitDialog()
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
	GetCurrentDirectory(255,AppDir);
	wsprintf(userfile,"%s\\fsftpd.user",AppDir);
	wsprintf(ipfile,"%s\\fsftpd.allow",AppDir);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	int i;
	char sTmp[32]="";
	unsigned long IP;
	IP=-1;
	CFtpd tmpd;
	for (i=0; IP!=0;i++) 
	{
		IP=tmpd.GetLocalNumericIP(i);
		if (IP !=0) 
		{
			wsprintf(sTmp,"%d.%d.%d.%d",(IP&0xFF),((IP>>8 ) & 0xFF),((IP>>16) & 0xFF),((IP>>24) & 0xFF));
			m_ip_combo_ctrl.AddString(sTmp);
		}
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFtpdDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFtpdDlg::OnPaint() 
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

HCURSOR CFtpdDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFtpdDlg::OnLOAD() 
{
	char item[64] = "";
	CFileDialog cf(TRUE, "", "*.ftpd");;
	cf.DoModal();
	CString fullpath = cf.GetPathName();
	GetPrivateProfileString("FTPD", "SERVER_NAME",NULL,item,256,fullpath);
	m_ftpserver_name = item;
	GetPrivateProfileString("FTPD", "IPCHK_CONNECTION",NULL,item,256,fullpath);
	m_b_ipchk = atoi(item);
	GetPrivateProfileString("FTPD", "IPCHK_DATA",NULL,item,256,fullpath);
	m_b_ipchk_pasv = atoi(item);
	GetPrivateProfileString("FTPD", "PASV_IP_ROLLING",NULL,item,256,fullpath);
	m_b_switch_pasv_ip = atoi(item);
	GetPrivateProfileString("FTPD", "AUTO_CHOOSE_IP",NULL,item,256,fullpath);
	m_b_auto_choose_ip = atoi(item);
	GetPrivateProfileString("FTPD", "LISTEN_PORT",NULL,item,256,fullpath);
	m_ftpd_port = atoi(item);
	GetPrivateProfileString("FTPD", "USE_DATAPORT_RANGE",NULL,item,256,fullpath);
	m_specify_data_port = atoi(item);
	GetPrivateProfileString("FTPD", "PORT_FROM",NULL,item,256,fullpath);
	m_port_from = atoi(item);
	GetPrivateProfileString("FTPD", "PORT_TO",NULL,item,256,fullpath);
	m_port_to = atoi(item);
	GetPrivateProfileString("FTPD", "ROOT_PATH",NULL,item,256,fullpath);
	m_rootdir = item;
	UpdateData(FALSE);
}

void CFtpdDlg::OnSAVE() 
{
	char item[64] = "";
	CFileDialog cf(TRUE, "", "*.ftpd");;
	cf.DoModal();
	CString fullpath = cf.GetPathName();
	wsprintf(item,"%s",m_ftpserver_name);
	WritePrivateProfileString("FTPD", "SERVER_NAME",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_b_ipchk);
	WritePrivateProfileString("FTPD", "IPCHK_CONNECTION",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_b_ipchk_pasv);
	WritePrivateProfileString("FTPD", "IPCHK_DATA",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_b_switch_pasv_ip);
	WritePrivateProfileString("FTPD", "PASV_IP_ROLLING",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_b_auto_choose_ip);
	WritePrivateProfileString("FTPD", "AUTO_CHOOSE_IP",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_ftpd_port);
	WritePrivateProfileString("FTPD", "LISTEN_PORT",item,(LPCSTR)fullpath);
	wsprintf(item,"%s",(LPCSTR)m_rootdir);
	WritePrivateProfileString("FTPD", "ROOT_PATH",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_specify_data_port);
	WritePrivateProfileString("FTPD", "USE_DATAPORT_RANGE",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_port_from);
	WritePrivateProfileString("FTPD", "PORT_FROM",item,(LPCSTR)fullpath);
	wsprintf(item,"%d",m_port_to);
	WritePrivateProfileString("FTPD", "PORT_TO",item,(LPCSTR)fullpath);
}

int CFtpdDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 1;
}

void CFtpdDlg::OnLaunch() 
{
	char sadr[32] = "";
	UpdateData(TRUE);
	CFtpd* Ftpdsock = new CFtpd();
	Ftpdsock->m_servername = m_ftpserver_name;
    Ftpdsock->b_ipchk = (bool) m_b_ipchk;
	Ftpdsock->b_ipchk_pasv = (bool) m_b_ipchk_pasv;
	Ftpdsock->b_switch_pasv_ip = (bool) m_b_switch_pasv_ip;
	Ftpdsock->b_auto_choose_ip = (bool) m_b_auto_choose_ip;
	Ftpdsock->m_port = m_ftpd_port;
	Ftpdsock->m_specify_data_port = (bool) m_specify_data_port;
	Ftpdsock->m_port_from = m_port_from;
	Ftpdsock->m_port_to = m_port_to;
	strcpy(Ftpdsock->userfile,userfile);
	strcpy(Ftpdsock->ipfile,ipfile);
	Ftpdsock->Create(m_ftpd_port,SOCK_STREAM,sadr);
	strcpy(Ftpdsock->m_root_dir,(LPCSTR)m_rootdir);
	Ftpdsock->Listen(32);
	FTPDList.AddTail(Ftpdsock);
}
