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

//����ģʽ
#define HM_NONE     0 
#define HM_TOP      1  

#define CM_ELAPSE   200 //�������Ƿ��뿪���ڵ�ʱ����
#define HS_ELAPSE   30   //���ػ���ʾ����ÿ����ʱ����
#define HS_STEPS    10  //���ػ���ʾ���̷ֳɶ��ٲ�

#define INTERVAL    20  //����ճ��ʱ�������Ļ�߽����С���,��λΪ����
#define INFALTE     10  //��������ʱ����봰�ڱ߽����С���,��λΪ����
#define MINCX       100 //������С���
#define MINCY       400 //������С�߶�

#define SLIP_RATE  0.01

CDaemonManagerDlg::CDaemonManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDaemonManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDaemonManagerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconHide=AfxGetApp()->LoadIcon(IDI_ICON_HIDE);
	m_hIconFix=AfxGetApp()->LoadIcon(IDI_ICON_FIX);
	
	m_isSizeChanged = FALSE;
	m_isSetTimer = FALSE;
	m_hsFinished = TRUE;
    m_hiding = FALSE;
	
	m_oldWndHeight = MINCY;
	m_taskBarHeight = 30;
	m_edgeHeight = 0;
	m_edgeWidth  =0;
	m_hideMode = HM_TOP;

	m_initialed=FALSE;
	m_isFix=FALSE;
}

void CDaemonManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaemonManagerDlg)
	DDX_Control(pDX, IDC_BUTTON_FIX, m_btFix);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDaemonManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CDaemonManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_MOVING()
	ON_BN_CLICKED(IDC_BUTTON_FIX, OnButtonFix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg message handlers

void CDaemonManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

BOOL CDaemonManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
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
	
	int slip=(int)(cxIcon*SLIP_RATE);

	points[2].x=rect.Width()-slip;
	points[2].y=rect.Height();

	points[3].x=slip;
	points[3].y=rect.Height();
	
	rgn.CreatePolygonRgn(points,4,ALTERNATE);

	SetWindowRgn(rgn,TRUE);

	if(!m_initialed)
	{
		CPoint point;
		point.x=x;
		point.y=0;

		OnNcHitTest(point);

		m_btClose.SubclassDlgItem(IDOK,this);
		m_btClose.LoadBitmaps(IDB_CLOSE_N,IDB_CLOSE_H,IDB_CLOSE_F,IDB_CLOSE_D);
		m_btClose.Invalidate(true);
		m_btClose.SizeToContent();
		
	
		m_btFix.SetIcon(m_hIconHide);

//		m_btFix.SubclassDlgItem(IDC_BUTTON_FIX,this);
// 		HICON   hIcon=AfxGetApp()->LoadIcon(IDI_ICON_HIDE);
// 		m_btFix.SetIcon(hIcon);
	}
	

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


void CDaemonManagerDlg::OnMoving(UINT fwSide, LPRECT pRect) 
{
    FixMoving(fwSide,pRect); //����pRect
    CDialog::OnMoving(fwSide, pRect);
}

void CDaemonManagerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	FixSizing(fwSide,pRect); //����pRect
	CDialog::OnSizing(fwSide, pRect);
}

UINT CDaemonManagerDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format("Mouse (%d,%d)",point.x,point.y);
	GetDlgItem(IDC_CURSOR)->SetWindowText(str);
	
	if(m_hideMode != HM_NONE && !m_isSetTimer)
	{   
		int firOpenPlus=0;
		if(!m_initialed)
		{
			firOpenPlus=1000;
		}

		//������ʱ,����Ǵ�����״̬����ʾ״̬����Timer
		SetTimer(1,CM_ELAPSE+firOpenPlus,NULL);
		m_isSetTimer = TRUE;
		
		m_hsFinished = FALSE;
		m_hiding = FALSE;
		SetTimer(2,HS_ELAPSE,NULL); //������ʾ����
	}
	m_initialed=TRUE;
	return CDialog::OnNcHitTest(point);
}

void CDaemonManagerDlg::OnTimer(UINT nIDEvent) 
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

void CDaemonManagerDlg::DoHide()
{
	if(m_isFix)
		return;
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
	default:
		break;
	}

	SetWindowPos(&wndTopMost,tRect);
}

void CDaemonManagerDlg::DoShow()
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
	default:
		break;
	}
	
	SetWindowPos(&wndTopMost,tRect);
}

BOOL CDaemonManagerDlg::SetWindowPos(const CWnd* pWndInsertAfter, LPCRECT pCRect, UINT nFlags)
{
	return CDialog::SetWindowPos(pWndInsertAfter,pCRect->left, pCRect->top,
		pCRect->right - pCRect->left, pCRect->bottom - pCRect->top, nFlags);
}

void CDaemonManagerDlg::FixMoving(UINT fwSide, LPRECT pRect)
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

void CDaemonManagerDlg::FixSizing(UINT fwSide, LPRECT pRect)
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

int CDaemonManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

//	((CBitmapButton *)GetDlgItem(IDOK))->LoadBitmaps(IDB_CLOSE_N,IDB_CLOSE_H,IDB_CLOSE_F,IDB_CLOSE_D);
 //   ((CBitmapButton *)GetDlgItem(IDOK))->Invalidate(false);
//	((CBitmapButton *)GetDlgItem(IDOK))->SizeToContent();
	
	return 0;
}


void CDaemonManagerDlg::OnButtonFix() 
{
	// TODO: Add your control notification handler code here
	m_isFix=!m_isFix;

	if(m_isFix)
	{
		m_btFix.SetIcon(m_hIconFix);
	}
	else
	{
		m_btFix.SetIcon(m_hIconHide);
	}
}
