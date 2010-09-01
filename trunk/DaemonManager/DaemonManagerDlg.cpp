// DaemonManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DaemonManager.h"
#include "DaemonManagerDlg.h"
#include <MouseHook.h>
#include <string>
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg dialog

//收缩模式
#define HM_NONE     0 
#define HM_TOP      1  

#define CM_ELAPSE   200 //检测鼠标是否离开窗口的时间间隔
#define HS_ELAPSE   30   //隐藏或显示过程每步的时间间隔
#define HS_STEPS    10  //隐藏或显示过程分成多少步

#define INTERVAL    20  //触发粘附时鼠标与屏幕边界的最小间隔,单位为象素
#define INFALTE     10  //触发收缩时鼠标与窗口边界的最小间隔,单位为象素
#define MINCX       100 //窗口最小宽度
#define MINCY       400 //窗口最小高度

#define SLIP_RATE  0.01


static UINT UWM_MOUSEMOVE = ::RegisterWindowMessage(UWM_MOUSEMOVE_MSG);

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
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDaemonManagerDlg, CDialog)
    ON_REGISTERED_MESSAGE(UWM_MOUSEMOVE, OnMyMouseMove)
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
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg message handlers

void CDaemonManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
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
namespace
{
	bool   SaveBitmapToFile(HBITMAP   hBitmap,   string szfilename)  
	{  

       HDC hDC; //   设备描述表       

       int iBits; //   当前显示分辨率下每个像素所占字节数 

       WORD wBitCount; //   位图中每个像素所占字节数   

       DWORD dwPaletteSize   =   0   ; //   定义调色板大小，  位图中像素字节大小  ， 

       //   位图文件大小  ，  写入文件字节数

       DWORD dwBmBitsSize   ;  

       DWORD dwDIBSize,   dwWritten   ;  

       BITMAP Bitmap;                  

       BITMAPFILEHEADER bmfHdr; //位图属性结构 

       BITMAPINFOHEADER bi; //位图文件头结构  

       LPBITMAPINFOHEADER lpbi;   //位图信息头结构   

       HANDLE                     fh,   hDib,   hPal,hOldPal   =   NULL   ; //指向位图信息头结构,定义文件，分配内存句柄，调色板句柄

       //计算位图文件每个像素所占字节数 

       hDC   =   CreateDC(   "DISPLAY"   ,   NULL   ,   NULL   ,   NULL   )   ;  

       iBits   =   GetDeviceCaps(   hDC   ,   BITSPIXEL   )   *   GetDeviceCaps(   hDC   ,   PLANES   )   ;  

       DeleteDC(   hDC   )   ;  

       if   (   iBits   <=   1   )  

       {

         wBitCount   =   1;  

       } 

       else   if   (   iBits   <=   4   )  

       {

         wBitCount   =   4;  

       }

       else   if   (   iBits   <=   8   )  

       {

         wBitCount   =   8;  

       }  

       else   if   (   iBits   <=   24   )  

       {

         wBitCount   =   24;  

       } 

       else   if   (   iBits   <=   32   )  

       {  

         wBitCount   =   32;  

       }  

       //计算调色板大小 

       if   (   wBitCount   <=   8   )  

       {

         dwPaletteSize   =   (   1   <<   wBitCount   )   *   sizeof(   RGBQUAD   )   ;  

       } 

       //设置位图信息头结构 

       GetObject(   hBitmap   ,   sizeof(   BITMAP   )   ,   (   LPSTR   )   &   Bitmap   )   ;  

       bi.biSize                         =   sizeof(   BITMAPINFOHEADER   );  

       bi.biWidth                       =   Bitmap.bmWidth;     

       bi.biHeight                     =   Bitmap.bmHeight;   

       bi.biPlanes                     =   1;     

       bi.biBitCount                 =   wBitCount;     

       bi.biCompression           =   BI_RGB;  

       bi.biSizeImage               =   0;  

       bi.biXPelsPerMeter       =   0;  

       bi.biYPelsPerMeter       =   0;  

       bi.biClrUsed                   =   0;  

       bi.biClrImportant         =   0;   

       dwBmBitsSize   =   (   (   Bitmap.bmWidth   *   wBitCount   +   31   )   /   32   )   *   4   *   Bitmap.bmHeight   ;  

       //为位图内容分配内存 

       hDib     =   GlobalAlloc(   GHND   ,dwBmBitsSize   +   dwPaletteSize   +   sizeof(   BITMAPINFOHEADER   )   )   ;   

       lpbi   =   (   LPBITMAPINFOHEADER   )GlobalLock(   hDib   )   ;

       *lpbi   =   bi;  

       //   处理调色板     

       hPal   =   GetStockObject(   DEFAULT_PALETTE   );  

       if   (   hPal   )  

       {  

              hDC =   ::GetDC(   NULL   );  

              hOldPal   =   SelectPalette(   hDC,   (HPALETTE   )   hPal   ,   FALSE   )   ;  

              RealizePalette(   hDC   )   ;  

       } 

       //   获取该调色板下新的像素值 

 

       GetDIBits(   hDC,   hBitmap,   0,   (   UINT   )   Bitmap.bmHeight,   

       (   LPSTR   )lpbi   +   sizeof(   BITMAPINFOHEADER   )   +   dwPaletteSize,  

       (   LPBITMAPINFO   )lpbi,   DIB_RGB_COLORS   );  

 

       //恢复调色板   

       if   (   hOldPal   )  

       {  

              SelectPalette(   hDC,   (   HPALETTE   )hOldPal,   TRUE   );  

              RealizePalette(   hDC   )   ;   

              ::ReleaseDC(   NULL,   hDC   )   ;  

       } 

       //创建位图文件

       fh   =   CreateFile(   szfilename.c_str()   ,   GENERIC_WRITE,    

       0   ,   NULL   ,   CREATE_ALWAYS   ,  

       FILE_ATTRIBUTE_NORMAL   |   FILE_FLAG_SEQUENTIAL_SCAN   ,   NULL   )   ; 

       if   (   fh   ==   INVALID_HANDLE_VALUE   )  

       {  

              return   false;  

       }   

 

       //   设置位图文件头 

       bmfHdr.bfType   =   0x4D42;     //   "BM"    

       dwDIBSize =   sizeof(   BITMAPFILEHEADER   )   +   sizeof(   BITMAPINFOHEADER   )   +   dwPaletteSize   +   dwBmBitsSize;       

       bmfHdr.bfSize =   dwDIBSize;  

       bmfHdr.bfReserved1 =   0;  

       bmfHdr.bfReserved2 =   0;  

       bmfHdr.bfOffBits =   (   DWORD   )sizeof(   BITMAPFILEHEADER   )    

       +   (   DWORD   )sizeof(   BITMAPINFOHEADER   )  

       +   dwPaletteSize;  

       //   写入位图文件头 

       WriteFile(   fh,   (   LPSTR   )&bmfHdr,   sizeof   

       (   BITMAPFILEHEADER   ),   &dwWritten,   NULL);  

       //   写入位图文件其余内容 

       WriteFile(   fh,   (   LPSTR   )lpbi,   dwDIBSize,    

       &dwWritten   ,   NULL   );  

       //消除内存分配     

       GlobalUnlock(   hDib   );   

       GlobalFree(   hDib   );  

       CloseHandle(   fh   );  

       return   true;  

	}
	void CaptureRect(HDC hdcScreen, LPRECT prc)
	{
		HDC hdc = CreateCompatibleDC(hdcScreen);
		HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,prc->right - prc->left,
			prc->bottom - prc->top);
		SelectObject(hdc,hbmp);
		
		BitBlt(hdc,0,0,prc->right - prc->left,prc->bottom - prc->top,
			hdcScreen,prc->left,prc->top,SRCCOPY);
		
		//拷贝到剪贴板中
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,hbmp);
		CloseClipboard();
		DeleteDC(hdc);

		char tempFile[64];
		sprintf(tempFile,"C:\\TMP\\%06u.bmp",GetTickCount());
		SaveBitmapToFile(hbmp,string(tempFile));
		::DeleteObject(hbmp);

		vector<long> vecTest(300,0);
		for (int i=0;i<vecTest.size();++i)
			for (int k=0;k<vecTest.size();++k)
				for (int j=0;j<vecTest.size();++j)
					if(vecTest[j]!=vecTest[k])
						break;
	}
};
LRESULT CDaemonManagerDlg::OnMyMouseMove(WPARAM, LPARAM lParam)
{
     CPoint point;
     GetCursorPos(&point);
//   CString str;
// 	 str.Format("My Mouse (%d,%d)",point.x,point.y);
// 	 GetDlgItem(IDC_CURSOR)->SetWindowText(str);

	 HDC hDeskDC = ::GetDC(NULL);
	 RECT rc;
	 //获取屏幕的大小
	 rc.left = point.x;
	 rc.top = point.y;
	 rc.right = rc.left + 100;
	 rc.bottom = rc.top + 100;
	 //CaptureRect(hDeskDC,&rc);

     return 0;
} // CHookDlg::OnMyMouseMove

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

		
		m_btClose.SubclassDlgItem(IDOK,this);
		m_btClose.LoadBitmaps(IDB_CLOSE_N,IDB_CLOSE_H,IDB_CLOSE_N,IDB_CLOSE_D);
		m_btClose.Invalidate(true);
		m_btClose.SizeToContent();
		
		
		m_btFix.SubclassDlgItem(IDC_BUTTON_FIX,this);
		m_btFix.LoadBitmaps(IDB_BITMAP_HIDE,IDB_BITMAP_HIDE,IDB_BITMAP_HIDE,IDB_BITMAP_HIDE);
		m_btFix.Invalidate(true);
		m_btFix.SizeToContent();

		OnNcHitTest(point);

		BOOL result = setMyHook(m_hWnd);
		if(!result)
		{ /* error */
			DWORD err = ::GetLastError();
			CString str;
			str.Format(_T("%d"), err);
			GetDlgItem(IDC_CURSOR)->SetWindowText(str);
		} /* error */
		else
		{ /* set hook */
			CString str;
			str.Format(_T("Set Mouse Hook OK !"));
			GetDlgItem(IDC_CURSOR)->SetWindowText(str);
		} /* set hook */

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
    FixMoving(fwSide,pRect); //修正pRect
    CDialog::OnMoving(fwSide, pRect);
}

void CDaemonManagerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	FixSizing(fwSide,pRect); //修正pRect
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

		//鼠标进入时,如果是从收缩状态到显示状态则开启Timer
		SetTimer(1,CM_ELAPSE+firOpenPlus,NULL);
		m_isSetTimer = TRUE;
		
		m_hsFinished = FALSE;
		m_hiding = FALSE;
		SetTimer(2,HS_ELAPSE,NULL); //开启显示过程
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
		//获取此时窗口大小
		GetWindowRect(tRect);
		//膨胀tRect,以达到鼠标离开窗口边沿一定距离才触发事件
		tRect.InflateRect(INFALTE,INFALTE); 
		
		if(!tRect.PtInRect(curPos)) //如果鼠标离开了这个区域
		{
			KillTimer(1); //关闭检测鼠标Timer
			m_isSetTimer = FALSE;
			GetDlgItem(IDC_TIMER)->SetWindowText("Timer Off");
			
			m_hsFinished = FALSE;			
			m_hiding = TRUE;
			SetTimer(2,HS_ELAPSE,NULL); //开启收缩过程
		}
	}

	if(nIDEvent == 2)
	{
		if(m_hsFinished) //如果收缩或显示过程完毕则关闭Timer	  
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
		{   //你可以把下面一句替换上面的 ...+=|-=steps 达到取消抽屉效果
			//更好的办法是添加个BOOL值来控制,其他case同样.
            tRect.bottom = m_edgeWidth;
			m_hsFinished = TRUE;  //完成隐藏过程
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
		{   //你可以把下面一句替换上面的 ...+=|-=steps 达到取消抽屉效果
			//更好的办法是添加个BOOL值来控制,其他case同样.
            tRect.top = -m_edgeHeight;
			m_hsFinished = TRUE;  //完成显示过程
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
	{   //粘附在上边
		pRect->bottom = height - m_edgeHeight;
		pRect->top = -m_edgeHeight;
		m_hideMode = HM_TOP;
	}
	else
	{   //不粘附
		if(m_isSizeChanged)
		{   //如果收缩到两边,则拖出来后会变回原来大小
            //在"拖动不显示窗口内容下"只有光栅变回原来大小
		    pRect->bottom = pRect->top + m_oldWndHeight;
		    m_isSizeChanged = FALSE;
		}
		if(m_isSetTimer)
		{   //如果Timer开启了,则关闭之
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
	{   //小于指定的最小宽度或高度
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

	//获得任务栏高度
    CWnd* p;
	p = this->FindWindow("Shell_TrayWnd",NULL);
	if(p != NULL)
	{
		CRect tRect;
		p->GetWindowRect(tRect);
		m_taskBarHeight = tRect.Height();
	}
	
	//修改风格使得他不在任务栏显示
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	//去掉关闭按键(如果想画3个按键的话)
	//ModifyStyle(WS_SYSMENU,NULL);

	//获得边缘高度和宽度
	m_edgeHeight = GetSystemMetrics(SM_CYEDGE);
	m_edgeWidth  = GetSystemMetrics(SM_CXFRAME);
	
	//可以在这里读取上次关闭后保存的大小

	
	return 0;
}


void CDaemonManagerDlg::OnButtonFix() 
{
	// TODO: Add your control notification handler code here
	m_isFix=!m_isFix;

	if(m_isFix)
	{
		m_btFix.LoadBitmaps(IDB_BITMAP_FIX,IDB_BITMAP_FIX,IDB_BITMAP_FIX,IDB_BITMAP_FIX);
	}
	else
	{
		m_btFix.LoadBitmaps(IDB_BITMAP_HIDE,IDB_BITMAP_HIDE,IDB_BITMAP_HIDE,IDB_BITMAP_HIDE);
	}
	m_btFix.RedrawWindow();
}

void CDaemonManagerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnMouseMove(nFlags, point);
}
