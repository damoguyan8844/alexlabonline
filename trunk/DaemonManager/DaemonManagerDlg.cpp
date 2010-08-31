// DaemonManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DaemonManager.h"
#include "DaemonManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg dialog

CDaemonManagerDlg::CDaemonManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDaemonManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDaemonManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDaemonManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaemonManagerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDaemonManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CDaemonManagerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg message handlers

BOOL CDaemonManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
//	int cxIcon = GetSystemMetrics(SM_CXICON);
//	int cyIcon = GetSystemMetrics(SM_CYICON);
	int cxIcon = GetSystemMetrics(   SM_CXFULLSCREEN   ); 
	int cyIcon = GetSystemMetrics(   SM_CYFULLSCREEN   );

	CRect rect;
	GetClientRect(&rect);

	int x = (cxIcon-rect.Width()   + 1) / 2;
	int y = (cyIcon-rect.Height()   + 1) / 2;
	MoveWindow(x,0,rect.Width(),rect.Height(),TRUE);
	
	CRgn rgn;

	POINT points[4];
	points[0].x=0;
	points[0].y=0;
	
	points[1].x=0+rect.Width();
	points[1].y=0;
	
	int slip=(int)(cxIcon*0.03);

	points[2].x=rect.Width()-slip;
	points[2].y=rect.Height();

	points[3].x=slip;
	points[3].y=rect.Height();
	
	rgn.CreatePolygonRgn(points,4,ALTERNATE);

	SetWindowRgn(rgn,TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDaemonManagerDlg::OnPaint() 
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
HCURSOR CDaemonManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
