// QQHideWnd.h : main header file for the QQHIDEWND application
//

#if !defined(AFX_QQHIDEWND_H__D8D4FD0A_FED0_495E_A5D4_0D2980449FBE__INCLUDED_)
#define AFX_QQHIDEWND_H__D8D4FD0A_FED0_495E_A5D4_0D2980449FBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQHideWndApp:
// See QQHideWnd.cpp for the implementation of this class
//

class CQQHideWndApp : public CWinApp
{
public:
	CQQHideWndApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQHideWndApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQQHideWndApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQHIDEWND_H__D8D4FD0A_FED0_495E_A5D4_0D2980449FBE__INCLUDED_)
