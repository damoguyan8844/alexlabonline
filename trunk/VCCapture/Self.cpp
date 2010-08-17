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

	//初始化像皮筋类
	m_rectTracker.m_nStyle=CRectTracker::resizeInside|CRectTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
	
    m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  
    
	
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bQuit=FALSE;
	m_bShowMsg=FALSE;
    m_startPt=0;
    
	//获取屏幕分辩率
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);
	
	//截取屏幕到位图中
	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_pBitmap=CBitmap::FromHandle(CopyScreenToBitmap(&rect));
    
	//一定要初始化m_rgn，
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
	//把对化框设置成全屏顶层窗口
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
	//取得SetLayeredWindowAttributes函数指针         
	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");        
	if(fun)fun(this->GetSafeHwnd(),0,130,2); 
	Invalidate();
	FreeLibrary(hInst);         
}*/
	
	//移动操作提示窗口
	CRect rect;
	m_tipEdit.GetWindowRect(&rect);
    m_tipEdit.MoveWindow(0,0,rect.Width(),rect.Height());
    
	//显示操作提示窗口文字
	DrawTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSelf::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//用整个桌面填充全屏对话框背景
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
		if(IDOK==AfxMessageBox("截图内容已经拷贝到剪贴板!",MB_OKCANCEL))
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
	
    //判断击中位置
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
	
	//显示截取矩形大小信息
	if(m_bShowMsg&&m_bFirstDraw)
	{
		CRect rect;
		m_rectTracker.GetTrueRect(&rect);
		DrawMessage(rect);
	}
	
	//画出像皮筋矩形
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
		//如果已经截取矩则清除截取矩形
		m_bFirstDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		PaintWindow();
	}
	else
	{
		//关闭程序
		PostMessage(WM_CLOSE);
		AfxGetMainWnd() ->ShowWindow(SW_SHOW);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CSelf::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	//设置改变截取矩形大小时光标
	if (pWnd == this && m_rectTracker.SetCursor(this, nHitTest)) 
    {
		
		return TRUE; 
	}
	else
	{
		//设置彩色光标
		SetCursor(m_hCursor);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

HBITMAP CSelf::CopyScreenToBitmap(LPRECT lpRect, BOOL bSave)
{
	HDC       hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;
	
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	
	//确保选定区域是可见的
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
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
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
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	if(bSave)
	{
		
		if (OpenClipboard()) 
		{
			//清空剪贴板
			EmptyClipboard();
			//把屏幕内容粘贴到剪贴板上,
			//hBitmap 为刚才的屏幕位图句柄
			SetClipboardData(CF_BITMAP, hBitmap);
			//关闭剪贴板
			CloseClipboard();
		}
	}
	return hBitmap;
}

void CSelf::PaintWindow()
{
	//获取当前全屏对话框窗口大小
	CRect rect1;
	GetWindowRect(rect1);
	
	//获取编辑框窗口大小
	CRect rect2;
	m_tipEdit.GetWindowRect(rect2);
	
	CRgn rgn1,rgn2;
	rgn1.CreateRectRgnIndirect(rect1);
	rgn2.CreateRectRgnIndirect(rect2);
	
	//获取更新区域,就是除了编辑框窗口不更新
	m_rgn.CombineRgn(&rgn1,&rgn2,RGN_DIFF);
	
	InvalidateRgn(&m_rgn);
}

void CSelf::DrawMessage(CRect &inRect)
{
	//截取矩形大小信息离鼠标间隔
	const int space=3;
    
	//设置字体颜色大小
	CClientDC dc(this);
	CPoint pt;
	CPen pen(PS_SOLID,1,RGB(0,0,255));
	dc.SetTextColor(RGB(0,0,255));
	CFont font;
	font.CreatePointFont(90,"宋体");
	dc.SelectObject(&font);
	
	//得到字体宽度和高度
	GetCursorPos(&pt);
	dc.SetBkMode(TRANSPARENT);
	TEXTMETRIC tm;
	int charHeight;
	CSize size;
	int	lineLength;
	dc.GetTextMetrics(&tm);
	charHeight = tm.tmHeight+tm.tmExternalLeading;
	size=dc.GetTextExtent("顶点位置  ",strlen("顶点位置  "));
	lineLength=size.cx;
    
	//初始化矩形, 以保证写下六行文字
	CRect rect(pt.x+space,pt.y-charHeight*6-space,pt.x+lineLength+space,pt.y-space);
	
	int x = GetDeviceCaps(dc, HORZRES);
	int y = GetDeviceCaps(dc, VERTRES);
	
    //创建临时矩形
    CRect rectTemp;
	//当矩形到达桌面边缘时调整方向和大小
	if((pt.x+rect.Width())>=x)
	{
		//桌面上方显示不下矩形
		rectTemp=rect;
		rectTemp.left=rect.left-rect.Width()-space*2;
		rectTemp.right=rect.right-rect.Width()-space*2;;
		rect=rectTemp;
	}
	
	if((pt.y-rect.Height())<=0)
	{
		//桌面右方显示不下矩形
		rectTemp=rect;
		rectTemp.top=rect.top+rect.Height()+space*2;;
		rectTemp.bottom=rect.bottom+rect.Height()+space*2;;
		rect=rectTemp;
	}
    
	//创建空画刷画矩形
	dc.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.Rectangle(rect);
   	rect.top+=2;
	//在矩形中显示文字
	CString string(" 顶点位置 ");
    dc.TextOut(rect.left,rect.top,string);
	
	string.Format(" (%d,%d)",inRect.left,inRect.top);
	dc.TextOut(rect.left,rect.top+charHeight,string);
	
	string=" 矩形大小 ";
	dc.TextOut(rect.left,rect.top+charHeight*2,string);
	
	string.Format(" (%d,%d)",inRect.Width(),inRect.Height());
	dc.TextOut(rect.left,rect.top+charHeight*3,string);
	
	string=" 光标坐标 ";
	dc.TextOut(rect.left,rect.top+charHeight*4,string);
	
	string.Format(" (%d,%d)",pt.x,pt.y);
	dc.TextOut(rect.left,rect.top+charHeight*5,string);
}

void CSelf::DrawTip()
{
	//得当前坐标像素,
	CPoint pt;
	GetCursorPos(&pt);
	
	//得到当前R,G,B,各像素值
	COLORREF color;
	CClientDC dc(this);
	color=dc.GetPixel(pt);
	BYTE rValue,gValue,bValue;
	rValue=GetRValue(color);
	gValue=GetGValue(color);
	bValue=GetGValue(color);
	
	//按格式排放字符串
	CString string;
	CString strTemp;
	string.Format("\r\n\r\n\r\n\r\n ·当前像素RGB (%d,%d,%d)\r\n",rValue,gValue,bValue);
    
	if(!m_bDraw&&!m_bFirstDraw)
	{
		strTemp="\r\n ·按下鼠标左键不放选择截取\r\n 范围\r\n\r\n ·按ESC键或鼠标右键退出";
	}
	else
		if(m_bDraw && m_bFirstDraw)
		{
			strTemp="\r\n ·松开鼠标左键确定截取范围\r\n\r\n ·按ESC键退出";
		}
		else
			if(m_bFirstDraw)
			{
				strTemp="\r\n ·用鼠标左键调整截取范围的\r\n 大小和位置\r\n\r\n·截取范围内双击鼠标左键保存\r\n 图像，结束操作\r\n\r\n·点击鼠标右键重新选择";
			}
			string+=strTemp;
			//显示到编缉框中,操作提示窗口
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
