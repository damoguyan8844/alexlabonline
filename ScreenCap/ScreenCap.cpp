// ScreenCap.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ScreenCap.h"
#include <stdio.h>
#include "avifile.h"
#include <shellapi.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100
#define	WM_NOTIFYICON_MESSAGE	WM_USER+1000

#define	ErrorMessage(x)		MessageBox(NULL,x,"Error",MB_OK|MB_ICONERROR)

#define BITSPERPIXEL		32

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

void	SaveBitmap(char *szFilename,HBITMAP hBitmap);

UINT_PTR	nTimerId=0;
LPVOID		pBits=NULL;
CAviFile	*pAviFile=NULL;
HMENU		ghMenu=NULL;
bool		gbCapturing=false;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SCREENCAPTURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SCREENCAPTURE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SCREENCAPTURE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_SCREENCAPTURE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE,
      480, 400, 320, 240, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

  // ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP	hDesktopCompatibleBitmap=NULL;
	static HDC		hDesktopCompatibleDC=NULL;
	static HDC		hDesktopDC=NULL;
	static HWND		hDesktopWnd=NULL;
	static RECT		clientRect = {0,0,0,0};
	static NOTIFYICONDATA	nid;

	static bool		bMinimized=false;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_SYSCOMMAND:
		{
			if(wParam==SC_CLOSE)
			{
				SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
				return 0;
			}
			if(wParam==SC_MINIMIZE)
			{
				bMinimized=true;
				LONG_PTR ret=DefWindowProc(hWnd,message,wParam,lParam);
				SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(ID_FILE_HIDEWINDOW,0),0);
				return ret;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case ID_FILE_STARTCAPTURINGTOAVI:
		case ID_CAPTURE_START:
			{
				if(pAviFile==NULL)
				{
					OPENFILENAME	ofn;
					char	szFileName[512];
					strcpy(szFileName,"Output.avi");
					ZeroMemory(&ofn,sizeof(ofn));
					ofn.lStructSize=sizeof(OPENFILENAME);
					ofn.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
					ofn.lpstrFilter="Avi Files (*.avi)\0*.avi\0";
					ofn.lpstrDefExt="avi";				
					ofn.lpstrFile=szFileName;
					ofn.nMaxFile=512;
					ofn.hwndOwner = hWnd;
					if(!GetSaveFileName(&ofn))	break;

					pAviFile = new CAviFile(szFileName);
				}
			
				nid.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_PAUSE));
				strcpy(nid.szTip,"Capturing the Screen - Double Click to Pause");

				if(!Shell_NotifyIcon(NIM_MODIFY,&nid))				//Modify the Icon State
					MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);

				SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);

                gbCapturing = true;

				break;
			}
		case ID_FILE_PAUSECAPTURINGTOAVI:
		case ID_CAPTURE_STOP:
			{				
				nid.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_START));
				strcpy(nid.szTip,"Screen Capture Paused - Double Click to Resume");

				if(!Shell_NotifyIcon(NIM_MODIFY,&nid))				//Modify the Icon State
					MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);

				gbCapturing = false;

				break;
			}
		case ID_FILE_SHOWWINDOW:
			{
				if(bMinimized)	ShowWindow(hWnd,SW_RESTORE);
				else			ShowWindow(hWnd,SW_SHOW);
				SetForegroundWindow(hWnd);
				bMinimized=false;
				break;
			}
		case ID_FILE_HIDEWINDOW:
			{
				ShowWindow(hWnd,SW_HIDE);
				break;
			}
		case ID_FILE_CAPTURESCREENSHOT:
		case ID_EDIT_SAVEASBMP:
			{
				OPENFILENAME	ofn;
				char	szFileName[512]; 

				strcpy(szFileName,"ScreenShot.bmp");

                ZeroMemory(&ofn,sizeof(ofn));
				ofn.lStructSize=sizeof(OPENFILENAME);
				ofn.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
				ofn.lpstrFilter="Bitmap Files (*.bmp)\0*.bmp\0";
				ofn.lpstrDefExt="bmp";
				ofn.lpstrFile=szFileName;
				ofn.nMaxFile=512;
				ofn.hwndOwner = hWnd;
				if(!GetSaveFileName(&ofn))	break;

				SetCursor(LoadCursor(NULL,IDC_WAIT));	
				int		nWidth=GetSystemMetrics(SM_CXSCREEN);
				int		nHeight=GetSystemMetrics(SM_CYSCREEN);
				HDC		hBmpFileDC=CreateCompatibleDC(hDesktopDC);
				HBITMAP	hBmpFileBitmap=CreateCompatibleBitmap(hDesktopDC,nWidth,nHeight);
				HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC,hBmpFileBitmap);
				BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
				SelectObject(hBmpFileDC,hOldBitmap);

				SaveBitmap(ofn.lpstrFile,hBmpFileBitmap);

				DeleteDC(hBmpFileDC);
				DeleteObject(hBmpFileBitmap);
				SetCursor(LoadCursor(NULL,IDC_ARROW));
				break;
			}
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_INITMENU:
		{
			HMENU	hMenu = (HMENU)wParam;
			EnableMenuItem(hMenu,ID_FILE_SHOWWINDOW,IsWindowVisible(hWnd)?MF_GRAYED:MF_ENABLED|MF_BYCOMMAND);
			EnableMenuItem(hMenu,ID_FILE_HIDEWINDOW,IsWindowVisible(hWnd)?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);
			EnableMenuItem(hMenu,ID_FILE_CAPTURESCREENSHOT,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(hMenu,ID_FILE_STARTCAPTURINGTOAVI,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(hMenu,ID_FILE_PAUSECAPTURINGTOAVI,gbCapturing?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);
			EnableMenuItem(hMenu,ID_CAPTURE_START,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(hMenu,ID_CAPTURE_STOP,gbCapturing?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);			
			break;
		}
	case WM_INITMENUPOPUP:
		{
			EnableMenuItem(ghMenu,ID_FILE_SHOWWINDOW,IsWindowVisible(hWnd)?MF_GRAYED:MF_ENABLED|MF_BYCOMMAND);
			EnableMenuItem(ghMenu,ID_FILE_HIDEWINDOW,IsWindowVisible(hWnd)?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);
			EnableMenuItem(ghMenu,ID_FILE_CAPTURESCREENSHOT,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(ghMenu,ID_FILE_STARTCAPTURINGTOAVI,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(ghMenu,ID_FILE_PAUSECAPTURINGTOAVI,gbCapturing?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);
			EnableMenuItem(ghMenu,ID_CAPTURE_START,gbCapturing?MF_GRAYED|MF_BYCOMMAND:MF_ENABLED);
			EnableMenuItem(ghMenu,ID_CAPTURE_STOP,gbCapturing?MF_ENABLED:MF_GRAYED|MF_BYCOMMAND);			
			break;
		}
	case WM_CREATE:
		{
			BITMAPINFO	bmpInfo;
			ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
			bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			bmpInfo.bmiHeader.biBitCount=BITSPERPIXEL;
			bmpInfo.bmiHeader.biCompression = BI_RGB;
			bmpInfo.bmiHeader.biWidth=GetSystemMetrics(SM_CXSCREEN);
			bmpInfo.bmiHeader.biHeight=GetSystemMetrics(SM_CYSCREEN);
			bmpInfo.bmiHeader.biPlanes=1;
			bmpInfo.bmiHeader.biSizeImage=abs(bmpInfo.bmiHeader.biHeight)*bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount/8;

			hDesktopWnd=GetDesktopWindow();
			hDesktopDC=GetDC(hDesktopWnd);
			hDesktopCompatibleDC=CreateCompatibleDC(hDesktopDC);
			hDesktopCompatibleBitmap=CreateDIBSection(hDesktopDC,&bmpInfo,DIB_RGB_COLORS,&pBits,NULL,0);
			if(hDesktopCompatibleDC==NULL || hDesktopCompatibleBitmap == NULL)
			{
				ErrorMessage("Unable to Create Desktop Compatible DC/Bitmap");	return -1;
			}
			SelectObject(hDesktopCompatibleDC,hDesktopCompatibleBitmap);

			ZeroMemory(&nid,sizeof(nid));
			nid.cbSize=sizeof(nid);
			nid.uID=1000;
			nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			nid.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_SCREENCAPTURE));
			nid.hWnd=hWnd;
			strcpy(nid.szTip,"Screen Capture Application - Double Click to Start Capturing");
			nid.uCallbackMessage=WM_NOTIFYICON_MESSAGE;
			if(!Shell_NotifyIcon(NIM_ADD,&nid))	MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);

			if((ghMenu=LoadMenu(hInst,MAKEINTRESOURCE(IDC_SCREENCAPTURE)))==NULL)
			{
				ErrorMessage("Unable to Load Menu");	return -1;
			}

			nTimerId=SetTimer(hWnd,12345,500,NULL);	//Timer set to 500 ms.
			break;
		}
	case WM_CHAR:
		{
			if(wParam==VK_ESCAPE)
				DestroyWindow(hWnd);
			break;
		}
	case WM_SIZE:
		{
			clientRect.right = LOWORD(lParam);
			clientRect.bottom = HIWORD(lParam);
			break;
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			StretchBlt(hdc,0,0,clientRect.right,clientRect.bottom,hDesktopCompatibleDC,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),SRCCOPY);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_TIMER:
		{
			if(gbCapturing == false)	break;

			int	nWidth=GetSystemMetrics(SM_CXSCREEN);
			int	nHeight=GetSystemMetrics(SM_CYSCREEN);

			BitBlt(hDesktopCompatibleDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

			InvalidateRect(hWnd,NULL,false);

			pAviFile->AppendNewFrame(nWidth,nHeight,pBits);			//Append this Captured ScreenShot to the Movie

			break;
		}
	case WM_NOTIFYICON_MESSAGE:
		{
			switch(lParam)
			{
				case WM_MOUSEMOVE:break;
				case WM_LBUTTONDBLCLK:
					{						
						SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(gbCapturing?ID_FILE_PAUSECAPTURINGTOAVI:ID_FILE_STARTCAPTURINGTOAVI,0),0);
						break;
					}
				case WM_RBUTTONDOWN:
					{
						POINT pt;GetCursorPos(&pt);
						SetForegroundWindow(hWnd);
						TrackPopupMenu(GetSubMenu(ghMenu,0),TPM_LEFTALIGN|TPM_BOTTOMALIGN,pt.x,pt.y,0,hWnd,NULL);
						break;
					}
			}
			break;
		}
	case WM_DESTROY:
		{
			if(nTimerId)
			{
				KillTimer(hWnd,nTimerId);
				nTimerId=0;
			}
			if(pAviFile)
			{
				delete pAviFile;
				pAviFile = NULL;
			}
			if(ghMenu)	
				DestroyMenu(ghMenu);
			if(hDesktopCompatibleDC)
				DeleteDC(hDesktopCompatibleDC);
			if(hDesktopCompatibleBitmap)
				DeleteObject(hDesktopCompatibleBitmap);
			ReleaseDC(hDesktopWnd,hDesktopDC);
			Shell_NotifyIcon(NIM_DELETE,&nid);
			PostQuitMessage(0);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void	SaveBitmap(char *szFilename,HBITMAP hBitmap)
{
	HDC					hdc=NULL;
	FILE*				fp=NULL;
	LPVOID				pBuf=NULL;
	BITMAPINFO			bmpInfo;
	BITMAPFILEHEADER	bmpFileHeader;

	do{

		hdc=GetDC(NULL);
		ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		GetDIBits(hdc,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

		if(bmpInfo.bmiHeader.biSizeImage<=0)
			bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

		if((pBuf=malloc(bmpInfo.bmiHeader.biSizeImage))==NULL)
		{
			MessageBox(NULL,_T("Unable to Allocate Bitmap Memory"),_T("Error"),MB_OK|MB_ICONERROR);
			break;
		}
		
		bmpInfo.bmiHeader.biCompression=BI_RGB;
		GetDIBits(hdc,hBitmap,0,bmpInfo.bmiHeader.biHeight,pBuf,&bmpInfo,DIB_RGB_COLORS);	

		if((fp=fopen(szFilename,"wb"))==NULL)
		{
			MessageBox(NULL,_T("Unable to Create Bitmap File"),_T("Error"),MB_OK|MB_ICONERROR);
			break;
		}

		bmpFileHeader.bfReserved1=0;
		bmpFileHeader.bfReserved2=0;
		bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
		bmpFileHeader.bfType='MB';
		bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

		fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);
		fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
		fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp);

	}while(false);

		if(hdc)
			ReleaseDC(NULL,hdc);

		if(pBuf)
			free(pBuf);

		if(fp)
			fclose(fp);
}