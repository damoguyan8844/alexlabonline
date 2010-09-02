// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__94786A7B_A5EC_4D5F_A70B_BA494FA5962A__INCLUDED_)
#define AFX_STDAFX_H__94786A7B_A5EC_4D5F_A70B_BA494FA5962A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define	_WIN32_WINNT	0x0510
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers   
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxrich.h>		// MFC rich edit classes

/*
#include "IPHlpApi.h"
#pragma comment(lib, "IPHlpApi.Lib")
*/

// 默认分组名称
#define FreeEIM_Default_Group	_T("FreeEIM")

// 软件标题
#define APP_TITLE _T("FreeEIM")

#include "winsock2.h"

/* 暂时取消 2009年12月3日 受影响函数 GetMac();
#include "sensapi.h"
#pragma comment(lib, "sensapi.lib")
*/


#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

/* 暂时取消了。2009年12月3日
#include "htmlhelp.h"
#pragma comment(lib, "htmlhelp.lib")
*/

#define BUF_LEN				1024 * 8
#define	VOICE_RECV_PORT		7899

#pragma warning(disable:4786) // Disable warning messages

#include <list>
#include <algorithm>
#include <map>
#include <string>
using namespace std;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__94786A7B_A5EC_4D5F_A70B_BA494FA5962A__INCLUDED_)
