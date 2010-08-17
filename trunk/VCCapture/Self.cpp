// Self.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreen.h"
#include "Self.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelf dialog


CSelf::CSelf(CWnd* pParent /*=NULL*/)
	: CDialog(CSelf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelf)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//��ʼ����Ƥ����
	m_rectTracker.m_nStyle=CRectTracker::resizeInside|CRectTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
	
    m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  
    
	
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bQuit=FALSE;
	m_bShowMsg=FALSE;
    m_startPt=0;
    
	//��ȡ��Ļ�ֱ���
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);
	
	//��ȡ��Ļ��λͼ��
	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_pBitmap=CBitmap::FromHandle(CopyScreenToBitmap(&rect));
    
	//һ��Ҫ��ʼ��m_rgn��
    //m_rgn.CreateRectRgn(0,0,50,50);
	m_rgn.CreateRectRgn(0,0,50,50);
}


void CSelf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelf)
	DDX_Control(pDX, IDC_EDIT1, m_tipEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelf, CDialog)
	//{{AFX_MSG_MAP(CSelf)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelf message handlers

BOOL CSelf::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//�ѶԻ������ó�ȫ�����㴰��
	ModifyStyle(WS_CAPTION,0,0);   
    SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	
	//	SetWindowPos(&wndTopMost,-1,-1,m_xScreen+1,m_yScreen+1,SWP_SHOWWINDOW);
    
	/*	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,        
	GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);    
    HINSTANCE hInst = LoadLibrary("User32.DLL");     
    if(hInst)         
    {         
	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);         
	MYFUNC fun = NULL;        
	//ȡ��SetLayeredWindowAttributes����ָ��         
	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");        
	if(fun)fun(this->GetSafeHwnd(),0,130,2); 
	Invalidate();
	FreeLibrary(hInst);         
}*/
	
	//�ƶ�������ʾ����
	CRect rect;
	m_tipEdit.GetWindowRect(&rect);
    m_tipEdit.MoveWindow(0,0,rect.Width(),rect.Height());
    
	//��ʾ������ʾ��������
	DrawTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSelf::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//�������������ȫ���Ի��򱳾�
	BITMAP bmp;
	m_pBitmap->GetBitmap(&bmp);
	
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	
	dcCompatible.SelectObject(m_pBitmap);
	
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CSelf::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
	
	if(nHitTest==8)
	{
		CopyScreenToBitmap(m_rectTracker.m_rect,TRUE);
		m_bFirstDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		PaintWindow();
		
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		if(IDOK==AfxMessageBox("��ͼ�����Ѿ�������������!",MB_OKCANCEL))
		{
			PostMessage(WM_CLOSE);
			AfxGetMainWnd() ->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
			return;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CSelf::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
	
	m_bShowMsg=TRUE;
	//PaintWindow();
	PaintWindow();
	
    //�жϻ���λ��
	if(nHitTest<0)
	{
		if(!m_bFirstDraw)
		{
			m_startPt=point;
			m_bDraw=TRUE;
			
			PaintWindow();
		}
	}
	else
	{
		if(m_bFirstDraw)
		{
			m_rectTracker.Track(this,point,TRUE);
			SendMessage(WM_LBUTTONUP,NULL,NULL);
			PaintWindow();
			
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CSelf::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bShowMsg=FALSE;
	m_bDraw=FALSE;
	
	DrawTip();
	PaintWindow();

	CDialog::OnLButtonUp(nFlags, point);
}

void CSelf::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDraw)
	   {
		m_rectTracker.m_rect.SetRect(m_startPt.x+1,m_startPt.y+1,point.x,point.y);
		m_bFirstDraw=TRUE;
		
		PaintWindow();
	   }
	DrawTip();

	CDialog::OnMouseMove(nFlags, point);
}

void CSelf::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	//��ʾ��ȡ���δ�С��Ϣ
	if(m_bShowMsg&&m_bFirstDraw)
	{
		CRect rect;
		m_rectTracker.GetTrueRect(&rect);
		DrawMessage(rect);
	}
	
	//������Ƥ�����
	if(m_bFirstDraw)
	{
		m_rectTracker.Draw(&dc);
	}
	
		CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}

void CSelf::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bFirstDraw)
	{
		//����Ѿ���ȡ���������ȡ����
		m_bFirstDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		PaintWindow();
	}
	else
	{
		//�رճ���
		PostMessage(WM_CLOSE);
		AfxGetMainWnd() ->ShowWindow(SW_SHOW);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CSelf::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	//���øı��ȡ���δ�Сʱ���
	if (pWnd == this && m_rectTracker.SetCursor(this, nHitTest)) 
    {
		
		return TRUE; 
	}
	else
	{
		//���ò�ɫ���
		SetCursor(m_hCursor);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

HBITMAP CSelf::CopyScreenToBitmap(LPRECT lpRect, BOOL bSave)
{
	HDC       hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;
	
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	
	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > m_xScreen)
		nX2 = m_xScreen;
	if (nY2 > m_yScreen)
		nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	if(bSave)
	{
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
        
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			dcCompatible, nX, nY, SRCCOPY);
		
	}
	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}
	
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//�õ���Ļλͼ�ľ��
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	if(bSave)
	{
		
		if (OpenClipboard()) 
		{
			//��ռ�����
			EmptyClipboard();
			//����Ļ����ճ������������,
			//hBitmap Ϊ�ղŵ���Ļλͼ���
			SetClipboardData(CF_BITMAP, hBitmap);
			//�رռ�����
			CloseClipboard();
		}
	}
	return hBitmap;
}

void CSelf::PaintWindow()
{
	//��ȡ��ǰȫ���Ի��򴰿ڴ�С
	CRect rect1;
	GetWindowRect(rect1);
	
	//��ȡ�༭�򴰿ڴ�С
	CRect rect2;
	m_tipEdit.GetWindowRect(rect2);
	
	CRgn rgn1,rgn2;
	rgn1.CreateRectRgnIndirect(rect1);
	rgn2.CreateRectRgnIndirect(rect2);
	
	//��ȡ��������,���ǳ��˱༭�򴰿ڲ�����
	m_rgn.CombineRgn(&rgn1,&rgn2,RGN_DIFF);
	
	InvalidateRgn(&m_rgn);
}

void CSelf::DrawMessage(CRect &inRect)
{
	//��ȡ���δ�С��Ϣ�������
	const int space=3;
    
	//����������ɫ��С
	CClientDC dc(this);
	CPoint pt;
	CPen pen(PS_SOLID,1,RGB(0,0,255));
	dc.SetTextColor(RGB(0,0,255));
	CFont font;
	font.CreatePointFont(90,"����");
	dc.SelectObject(&font);
	
	//�õ������Ⱥ͸߶�
	GetCursorPos(&pt);
	dc.SetBkMode(TRANSPARENT);
	TEXTMETRIC tm;
	int charHeight;
	CSize size;
	int	lineLength;
	dc.GetTextMetrics(&tm);
	charHeight = tm.tmHeight+tm.tmExternalLeading;
	size=dc.GetTextExtent("����λ��  ",strlen("����λ��  "));
	lineLength=size.cx;
    
	//��ʼ������, �Ա�֤д����������
	CRect rect(pt.x+space,pt.y-charHeight*6-space,pt.x+lineLength+space,pt.y-space);
	
	int x = GetDeviceCaps(dc, HORZRES);
	int y = GetDeviceCaps(dc, VERTRES);
	
    //������ʱ����
    CRect rectTemp;
	//�����ε��������Եʱ��������ʹ�С
	if((pt.x+rect.Width())>=x)
	{
		//�����Ϸ���ʾ���¾���
		rectTemp=rect;
		rectTemp.left=rect.left-rect.Width()-space*2;
		rectTemp.right=rect.right-rect.Width()-space*2;;
		rect=rectTemp;
	}
	
	if((pt.y-rect.Height())<=0)
	{
		//�����ҷ���ʾ���¾���
		rectTemp=rect;
		rectTemp.top=rect.top+rect.Height()+space*2;;
		rectTemp.bottom=rect.bottom+rect.Height()+space*2;;
		rect=rectTemp;
	}
    
	//�����ջ�ˢ������
	dc.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.Rectangle(rect);
   	rect.top+=2;
	//�ھ�������ʾ����
	CString string(" ����λ�� ");
    dc.TextOut(rect.left,rect.top,string);
	
	string.Format(" (%d,%d)",inRect.left,inRect.top);
	dc.TextOut(rect.left,rect.top+charHeight,string);
	
	string=" ���δ�С ";
	dc.TextOut(rect.left,rect.top+charHeight*2,string);
	
	string.Format(" (%d,%d)",inRect.Width(),inRect.Height());
	dc.TextOut(rect.left,rect.top+charHeight*3,string);
	
	string=" ������� ";
	dc.TextOut(rect.left,rect.top+charHeight*4,string);
	
	string.Format(" (%d,%d)",pt.x,pt.y);
	dc.TextOut(rect.left,rect.top+charHeight*5,string);
}

void CSelf::DrawTip()
{
	//�õ�ǰ��������,
	CPoint pt;
	GetCursorPos(&pt);
	
	//�õ���ǰR,G,B,������ֵ
	COLORREF color;
	CClientDC dc(this);
	color=dc.GetPixel(pt);
	BYTE rValue,gValue,bValue;
	rValue=GetRValue(color);
	gValue=GetGValue(color);
	bValue=GetGValue(color);
	
	//����ʽ�ŷ��ַ���
	CString string;
	CString strTemp;
	string.Format("\r\n\r\n\r\n\r\n ����ǰ����RGB (%d,%d,%d)\r\n",rValue,gValue,bValue);
    
	if(!m_bDraw&&!m_bFirstDraw)
	{
		strTemp="\r\n ����������������ѡ���ȡ\r\n ��Χ\r\n\r\n ����ESC��������Ҽ��˳�";
	}
	else
		if(m_bDraw && m_bFirstDraw)
		{
			strTemp="\r\n ���ɿ�������ȷ����ȡ��Χ\r\n\r\n ����ESC���˳�";
		}
		else
			if(m_bFirstDraw)
			{
				strTemp="\r\n ����������������ȡ��Χ��\r\n ��С��λ��\r\n\r\n����ȡ��Χ��˫������������\r\n ͼ�񣬽�������\r\n\r\n���������Ҽ�����ѡ��";
			}
			string+=strTemp;
			//��ʾ���༩����,������ʾ����
	m_tipEdit.SetWindowText(string);
}

void CSelf::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

BOOL CSelf::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			PostMessage(WM_CLOSE);
			AfxGetMainWnd() ->ShowWindow(SW_SHOW);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
