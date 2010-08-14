// installhook.h : main header file for the INSTALLHOOK application
//

#if !defined(AFX_INSTALLHOOK_H__2E50CD89_1DD2_4D48_9E8C_94BA39FF85CA__INCLUDED_)
#define AFX_INSTALLHOOK_H__2E50CD89_1DD2_4D48_9E8C_94BA39FF85CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInstallhookApp:
// See installhook.cpp for the implementation of this class
//

class CInstallhookApp : public CWinApp
{
public:
	CInstallhookApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallhookApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInstallhookApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLHOOK_H__2E50CD89_1DD2_4D48_9E8C_94BA39FF85CA__INCLUDED_)
