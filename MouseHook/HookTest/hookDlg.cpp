// hookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hook.h"
#include "MouseHook.h"
#include "HookHelp.h"
#include "Cat.h"
#include "hookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT UWM_MOUSEMOVE = ::RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
static UINT UWM_GIVE_HINT = ::RegisterWindowMessage(UWM_GIVE_HINT_MSG);

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
// CHookDlg dialog

CHookDlg::CHookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHookDlg)
	DDX_Control(pDX, IDC_HINT, c_Hint);
	DDX_Control(pDX, IDC_CAT, c_Cat);
	DDX_Control(pDX, IDC_XY, c_XY);
	DDX_Control(pDX, IDC_ENABLE, c_Enable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHookDlg, CDialog)
        ON_REGISTERED_MESSAGE(UWM_MOUSEMOVE, OnMyMouseMove)
	ON_REGISTERED_MESSAGE(UWM_GIVE_HINT, OnGiveHint)
	//{{AFX_MSG_MAP(CHookDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_HELPME, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookDlg message handlers

BOOL CHookDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHookDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHookDlg::OnPaint() 
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
HCURSOR CHookDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHookDlg::OnCancel() 
{
}

void CHookDlg::OnOK() 
{
}

void CHookDlg::OnClose() 
   {
    CDialog::OnOK();
   }

void CHookDlg::OnEnable() 
   {
    if(c_Enable.GetCheck() == BST_CHECKED)
       { /* enable it */
	BOOL result = setMyHook(m_hWnd);
	c_Cat.SetActive(result);
	if(!result)
	    { /* error */
	     DWORD err = ::GetLastError();
	     c_Enable.SetCheck(BST_UNCHECKED);
             CString s;
	     s.Format(_T("%d"), err);
	     c_XY.SetWindowText(s);
	    } /* error */
	else
	   { /* set hook */
	    SetWindowPos(&wndTopMost, 0, 0, 0, 0,
			 SWP_NOMOVE | SWP_NOSIZE);
	   } /* set hook */
       } /* enable it */
    else
       { /* disable it */
	clearMyHook(m_hWnd);
	c_XY.SetWindowText(_T(""));
	c_Cat.SetActive(FALSE);
	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0,
		     SWP_NOMOVE | SWP_NOSIZE);
       } /* disable it */
	
   }

/****************************************************************************
*                           CHookDlg::OnMyMouseMove
* Inputs:
*       WPARAM: unused
*	LPARAM: (x,y) coordinates of mouse
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Handles the mouse move event notification
****************************************************************************/

LRESULT CHookDlg::OnMyMouseMove(WPARAM, LPARAM lParam)
    {
     CPoint pt;
     GetCursorPos(&pt);
     CString s;
     s.Format(_T("%d, %d"), pt.x, pt.y);
     c_XY.SetWindowText(s);
     c_Cat.Redraw();
     return 0;
    } // CHookDlg::OnMyMouseMove


/****************************************************************************
*                            CHookDlg::OnGiveHint
* Inputs:
*       WPARAM: (WPARAM)(BOOL)TRUE to show hint, FALSE to remove it if visible
*	LPARAM: ignored
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Displays the hint
****************************************************************************/

LRESULT CHookDlg::OnGiveHint(WPARAM wParam, LPARAM)
    {
     if(wParam)
	{ /* show hint */
	 c_Hint.ShowWindow(SW_SHOW);
	} /* show hint */
     else
	{ /* remove hint */
	 c_Hint.ShowWindow(SW_HIDE);
	} /* remove hint */
     return 0;
    } // CHookDlg::OnGiveHint

void CHookDlg::OnHelp() 
{
 CHookHelp dlg;
 dlg.DoModal();
}
