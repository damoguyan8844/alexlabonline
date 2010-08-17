// Axtive.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreen.h"
#include "Axtive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxtive dialog


CAxtive::CAxtive(CWnd* pParent /*=NULL*/)
	: CDialog(CAxtive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAxtive)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAxtive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxtive)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxtive, CDialog)
	//{{AFX_MSG_MAP(CAxtive)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxtive message handlers

void CAxtive::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CAxtive::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nWidth=rectCapture.Width();
	int nHeight=rectCapture.Height();
	HDC hdcScreen, hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hdcScreen);
	hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hOldBitmap =(HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen,rectCapture.left,rectCapture.top,SRCCOPY);
	
	hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hdcScreen);
	DeleteDC(hMemDC);
	
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	
	CloseClipboard();
	this->ShowWindow(SW_HIDE);
	Sleep(50);
	MessageBox("屏幕内容已经拷到剪贴板上！");

	AfxGetMainWnd() ->ShowWindow(SW_SHOW);
	
	//终止鼠标捕获
	ReleaseCapture();

	EndDialog(0);

//	CDialog::OnLButtonUp(nFlags, point);
}

void CAxtive::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags==MK_LBUTTON)
	{
		ShowWindow(SW_HIDE);
		HCURSOR cur=LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR1));
		SetCursor(cur);
		SetCapture();//捕获鼠标
		
		ClientToScreen(&point);
		//hwndCapture=(HWND)::WindowFromPoint(point);
		hwndCapture=(HWND)::WindowFromPoint(point);
		
		HDC hDC=::GetDC(NULL);
		HPEN hPen=CreatePen(PS_INSIDEFRAME,6,RGB(255,0,0));
		SetROP2(hDC,R2_NOTXORPEN);
		SetROP2(hDC,R2_NOTXORPEN);//两次调用SetROP2()擦除屏幕图形
		
		HPEN hOldPen=(HPEN)SelectObject(hDC,hPen);
		::GetWindowRect(hwndCapture,&rectCapture);
		POINT pt[5];
		pt[0] = CPoint(rectCapture.left, rectCapture.top);
		pt[1] = CPoint(rectCapture.right,rectCapture.top);
		pt[2] = CPoint(rectCapture.right,rectCapture.bottom);
		pt[3] = CPoint(rectCapture.left, rectCapture.bottom);
		pt[4] = CPoint(rectCapture.left, rectCapture.top);
		
		::Polyline(hDC,pt,5);
		Sleep(100);
		::Polyline(hDC,pt,5);
		::SelectObject(hDC,hOldPen);
		::ReleaseDC(NULL,hDC);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CAxtive::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


