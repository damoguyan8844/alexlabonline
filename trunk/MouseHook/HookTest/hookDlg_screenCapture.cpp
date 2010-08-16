// hookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hook.h"
#include "MouseHook.h"
#include "HookHelp.h"
#include "Cat.h"
#include "hookDlg.h"

#include <string>
#include <vector>
using namespace std;

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
LRESULT CHookDlg::OnMyMouseMove(WPARAM, LPARAM lParam)
    {
     CPoint pt;
     GetCursorPos(&pt);
     CString s;
     s.Format(_T("%d, %d"), pt.x, pt.y);
     c_XY.SetWindowText(s);

	 HDC hDeskDC = ::GetDC(NULL);
	 RECT rc;
	 //获取屏幕的大小
	 rc.left = pt.x;
	 rc.top = pt.y;
	 rc.right = rc.left + 100;
	 rc.bottom = rc.top + 100;
	 CaptureRect(hDeskDC,&rc);

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
