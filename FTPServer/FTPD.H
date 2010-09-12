// ftpd.h : main header file for the FTPD application
//

#if !defined(AFX_FTPD_H__35B8C781_E1D1_46C4_941E_727AF452196E__INCLUDED_)
#define AFX_FTPD_H__35B8C781_E1D1_46C4_941E_727AF452196E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFtpdApp:
// See ftpd.cpp for the implementation of this class
//

class CFtpdApp : public CWinApp
{
public:
	CFtpdApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpdApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFtpdApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPD_H__35B8C781_E1D1_46C4_941E_727AF452196E__INCLUDED_)
