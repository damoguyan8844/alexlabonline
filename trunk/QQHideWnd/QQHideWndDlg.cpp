// QQHideWndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QQHideWnd.h"
#include "QQHideWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//����ģʽ
#define HM_NONE     0   //������
#define HM_TOP      1   //��������
#define HM_BOTTOM   2   //��������
#define HM_LEFT     3   //��������
#define HM_RIGHT    4   //��������

#define CM_ELAPSE   200 //�������Ƿ��뿪���ڵ�ʱ����
#define HS_ELAPSE   5   //���ػ���ʾ����ÿ����ʱ����
#define HS_STEPS    10  //���ػ���ʾ���̷ֳɶ��ٲ�

#define INTERVAL    20  //����ճ��ʱ�������Ļ�߽����С���,��λΪ����
#define INFALTE     10  //��������ʱ����봰�ڱ߽����С���,��λΪ����
#define MINCX       200 //������С���
#define MINCY       400 //������С�߶�


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
// CQQHideWndDlg dialog

CQQHideWndDlg::CQQHideWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQHideWndDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQHideWndDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	m_isSizeChanged = FALSE;
	m_isSetTimer = FALSE;
	m_hsFinished = TRUE;
    m_hiding = FALSE;
	
	m_oldWndHeight = MINCY;
	m_taskBarHeight = 30;
	m_edgeHeight = 0;
	m_edgeWidth  =0;
	m_hideMode = HM_NONE;
}

void CQQHideWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQHideWndDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQQHideWndDlg, CDialog)
	//{{AFX_MSG_MAP(CQQHideWndDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_MOVING()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQHideWndDlg message handlers

BOOL CQQHideWndDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQQHideWndDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQQHideWndDlg::OnPaint() 
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
HCURSOR CQQHideWndDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQQHideWndDlg::OnMoving(UINT fwSide, LPRECT pRect) 
{
    FixMoving(fwSide,pRect); //����pRect
    CDialog::OnMoving(fwSide, pRect);
}

void CQQHideWndDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	FixSizing(fwSide,pRect); //����pRect
	CDialog::OnSizing(fwSide, pRect);
}

UINT CQQHideWndDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format("Mouse (%d,%d)",point.x,point.y);
	GetDlgItem(IDC_CURSOR)->SetWindowText(str);
	if(m_hideMode != HM_NONE && !m_isSetTimer &&
		//��ֹ��곬����Ļ�ұ�ʱ���ұ����������˸
		point.x < GetSystemMetrics(SM_CXSCREEN) + INFALTE)
	{   //������ʱ,����Ǵ�����״̬����ʾ״̬����Timer
		SetTimer(1,CM_ELAPSE,NULL);
		m_isSetTimer = TRUE;
		
		m_hsFinished = FALSE;
		m_hiding = FALSE;
		SetTimer(2,HS_ELAPSE,NULL); //������ʾ����
	}
	return CDialog::OnNcHitTest(point);
}

void CQQHideWndDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1 )
	{
        POINT curPos;
	    GetCursorPos(&curPos);
		
		CString str;
    	str.Format("Timer On(%d,%d)",curPos.x,curPos.y);
	    GetDlgItem(IDC_TIMER)->SetWindowText(str);
		
		CRect tRect;
		//��ȡ��ʱ���ڴ�С
		GetWindowRect(tRect);
		//����tRect,�Դﵽ����뿪���ڱ���һ������Ŵ����¼�
		tRect.InflateRect(INFALTE,INFALTE); 
		
		if(!tRect.PtInRect(curPos)) //�������뿪���������
		{
			KillTimer(1); //�رռ�����Timer
			m_isSetTimer = FALSE;
			GetDlgItem(IDC_TIMER)->SetWindowText("Timer Off");
			
			m_hsFinished = FALSE;			
			m_hiding = TRUE;
			SetTimer(2,HS_ELAPSE,NULL); //������������
		}
	}

	if(nIDEvent == 2)
	{
		if(m_hsFinished) //�����������ʾ���������ر�Timer	  
			KillTimer(2);
		else
            m_hiding ? DoHide() : DoShow();
	}

	CDialog::OnTimer(nIDEvent);
}

void CQQHideWndDlg::DoHide()
{
	if(m_hideMode == HM_NONE)
		return;
	
	CRect tRect;
	GetWindowRect(tRect);
	
	INT height = tRect.Height();
	INT width  = tRect.Width();

	INT steps = 0;

	switch(m_hideMode)
	{
	case HM_TOP:
		steps = height/HS_STEPS;
		tRect.bottom -= steps;
		if(tRect.bottom <= m_edgeWidth)
		{   //����԰�����һ���滻����� ...+=|-=steps �ﵽȡ������Ч��
			//���õİ취����Ӹ�BOOLֵ������,����caseͬ��.
            tRect.bottom = m_edgeWidth;
			m_hsFinished = TRUE;  //������ع���
		}
		tRect.top = tRect.bottom - height; 
		break;
	case HM_BOTTOM:
		steps = height/HS_STEPS;
		tRect.top += steps;
		if(tRect.top >= (GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth))
		{
            tRect.top = GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth;
			m_hsFinished = TRUE;
		}
		tRect.bottom = tRect.top + height;
		break;
	case HM_LEFT:
		steps = width/HS_STEPS;
		tRect.right -= steps;
		if(tRect.right <= m_edgeWidth)
		{
            tRect.right = m_edgeWidth;
			m_hsFinished = TRUE;
		}
		tRect.left = tRect.right - width;
		tRect.top = -m_edgeHeight;
		tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
		break;
	case HM_RIGHT:
		steps = width/HS_STEPS;
		tRect.left += steps;
		if(tRect.left >= (GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth))
		{
            tRect.left = GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth;
			m_hsFinished = TRUE;
		}		
		tRect.right = tRect.left + width;
		tRect.top = -m_edgeHeight;
		tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
		break;
	default:
		break;
	}

	SetWindowPos(&wndTopMost,tRect);
}

void CQQHideWndDlg::DoShow()
{
    if(m_hideMode == HM_NONE)
		return;
	
	CRect tRect;
	GetWindowRect(tRect);
	INT height = tRect.Height();
	INT width  = tRect.Width();

	INT steps = 0;

	switch(m_hideMode)
	{
	case HM_TOP:
		steps = height/HS_STEPS;
		tRect.top += steps;
		if(tRect.top >= -m_edgeHeight)
		{   //����԰�����һ���滻����� ...+=|-=steps �ﵽȡ������Ч��
			//���õİ취����Ӹ�BOOLֵ������,����caseͬ��.
            tRect.top = -m_edgeHeight;
			m_hsFinished = TRUE;  //�����ʾ����
		}
		tRect.bottom = tRect.top + height;
		break;
	case HM_BOTTOM:
		steps = height/HS_STEPS;
		tRect.top -= steps;
		if(tRect.top <= (GetSystemMetrics(SM_CYSCREEN) - height))
		{
            tRect.top = GetSystemMetrics(SM_CYSCREEN) - height;
			m_hsFinished = TRUE;
		}
        tRect.bottom = tRect.top + height;
		break;
	case HM_LEFT:
		steps = width/HS_STEPS;
		tRect.right += steps;
		if(tRect.right >= width)
		{
            tRect.right = width;
			m_hsFinished = TRUE;
		}
		tRect.left = tRect.right - width;
		tRect.top = -m_edgeHeight;
		tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
		break;
	case HM_RIGHT:
		steps = width/HS_STEPS;
		tRect.left -= steps;
		if(tRect.left <= (GetSystemMetrics(SM_CXSCREEN) - width))
		{
            tRect.left = GetSystemMetrics(SM_CXSCREEN) - width;
			m_hsFinished = TRUE;
		}
		tRect.right = tRect.left + width;
		tRect.top = -m_edgeHeight;
		tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
		break;
	default:
		break;
	}
	
	SetWindowPos(&wndTopMost,tRect);
}

BOOL CQQHideWndDlg::SetWindowPos(const CWnd* pWndInsertAfter, LPCRECT pCRect, UINT nFlags)
{
	return CDialog::SetWindowPos(pWndInsertAfter,pCRect->left, pCRect->top,
		pCRect->right - pCRect->left, pCRect->bottom - pCRect->top, nFlags);
}

void CQQHideWndDlg::FixMoving(UINT fwSide, LPRECT pRect)
{
	POINT curPos;
	GetCursorPos(&curPos);
	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	INT screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	INT height = pRect->bottom - pRect->top;
	INT width  = pRect->right - pRect->left;
    
	if (curPos.y <= INTERVAL)
	{   //ճ�����ϱ�
		pRect->bottom = height - m_edgeHeight;
		pRect->top = -m_edgeHeight;
		m_hideMode = HM_TOP;
	}
	else if(curPos.y >= (screenHeight - INTERVAL - m_taskBarHeight))
	{   //ճ�����±�
        pRect->top = screenHeight - m_taskBarHeight - height;
        pRect->bottom = screenHeight - m_taskBarHeight;
		m_hideMode = HM_BOTTOM;
	}
	else if (curPos.x < INTERVAL)
	{	//ճ�������	
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			m_oldWndHeight = tRect.Height();			
		}
		pRect->right = width;
		pRect->left = 0;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_LEFT;
	}
	else if(curPos.x >= (screenWidth - INTERVAL))
	{   //ճ�����ұ�
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			m_oldWndHeight = tRect.Height();						
		}
		pRect->left = screenWidth - width;
		pRect->right = screenWidth;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_RIGHT;
	}
	else
	{   //��ճ��
		if(m_isSizeChanged)
		{   //�������������,���ϳ��������ԭ����С
            //��"�϶�����ʾ����������"ֻ�й�դ���ԭ����С
		    pRect->bottom = pRect->top + m_oldWndHeight;
		    m_isSizeChanged = FALSE;
		}
		if(m_isSetTimer)
		{   //���Timer������,��ر�֮
			if(KillTimer(1) == 1)
				m_isSetTimer = FALSE;
		}
		m_hideMode = HM_NONE;
		GetDlgItem(IDC_TIMER)->SetWindowText("Timer off");
	}
}

void CQQHideWndDlg::FixSizing(UINT fwSide, LPRECT pRect)
{
    CRect curRect(pRect);

	if(curRect.Width() < MINCX || curRect.Height() < MINCY)
	{   //С��ָ������С��Ȼ�߶�
		switch(fwSide)
		{
		case WMSZ_BOTTOM:
            pRect->bottom = pRect->top + MINCY;
			break;
		case WMSZ_BOTTOMLEFT:
			if(curRect.Width() <= MINCX)
				pRect->left = pRect->right - MINCX;
			if(curRect.Height() <= MINCY)
				pRect->bottom = pRect->top + MINCY;
			break;
		case WMSZ_BOTTOMRIGHT:
			if(curRect.Width() < MINCX)
				pRect->right = pRect->left + MINCX;
			if(curRect.Height() < MINCY)
				pRect->bottom = pRect->top + MINCY;
			break;
		case WMSZ_LEFT:
			pRect->left = pRect->right - MINCX;
			break;
		case WMSZ_RIGHT:
			pRect->right = pRect->left + MINCX;
			break;
		case WMSZ_TOP:
            pRect->top = pRect->bottom - MINCY;
			break;
		case WMSZ_TOPLEFT:
			if(curRect.Width() <= MINCX)
				pRect->left = pRect->right - MINCX;
			if(curRect.Height() <= MINCY)
				pRect->top = pRect->bottom - MINCY;
			break;
		case WMSZ_TOPRIGHT:
			if(curRect.Width() < MINCX)
				pRect->right = pRect->left + MINCX;
			if(curRect.Height() < MINCY)
				pRect->top = pRect->bottom - MINCY;
			break;
		}
	}
}

int CQQHideWndDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//����������߶�
    CWnd* p;
	p = this->FindWindow("Shell_TrayWnd",NULL);
	if(p != NULL)
	{
		CRect tRect;
		p->GetWindowRect(tRect);
		m_taskBarHeight = tRect.Height();
	}
	
	//�޸ķ��ʹ����������������ʾ
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	//ȥ���رհ���(����뻭3�������Ļ�)
	//ModifyStyle(WS_SYSMENU,NULL);

	//��ñ�Ե�߶ȺͿ��
	m_edgeHeight = GetSystemMetrics(SM_CYEDGE);
	m_edgeWidth  = GetSystemMetrics(SM_CXFRAME);
	
	//�����������ȡ�ϴιرպ󱣴�Ĵ�С

	return 0;
}
