// DaemonManager.h : main header file for the DAEMONMANAGER application
//

#if !defined(AFX_DAEMONMANAGER_H__FD9120E8_FC67_4A0E_8BCF_FF855A8FA56E__INCLUDED_)
#define AFX_DAEMONMANAGER_H__FD9120E8_FC67_4A0E_8BCF_FF855A8FA56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerApp:
// See DaemonManager.cpp for the implementation of this class
//

class CDaemonManagerApp : public CWinApp
{
public:
	CDaemonManagerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaemonManagerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDaemonManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAEMONMANAGER_H__FD9120E8_FC67_4A0E_8BCF_FF855A8FA56E__INCLUDED_)
