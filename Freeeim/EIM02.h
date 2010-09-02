// EIM02.h : main header file for the EIM02 application
//

#if !defined(AFX_EIM02_H__E3B24EBA_BA0F_4562_8D5A_D3C3CF085C33__INCLUDED_)
#define AFX_EIM02_H__E3B24EBA_BA0F_4562_8D5A_D3C3CF085C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "thirdpartclass/WinAppEx.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEIM02App:
// See EIM02.cpp for the implementation of this class
//

class CEIM02App : public CWinAppEx
{
public:
	CEIM02App();

	static LONG __stdcall EM_UnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEIM02App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEIM02App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EIM02_H__E3B24EBA_BA0F_4562_8D5A_D3C3CF085C33__INCLUDED_)
