// EDStock.h : main header file for the EDSTOCK application
//

#if !defined(AFX_EDSTOCK_H__D7CBE61D_6A05_4848_ADAC_3F2AB25C272E__INCLUDED_)
#define AFX_EDSTOCK_H__D7CBE61D_6A05_4848_ADAC_3F2AB25C272E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEDStockApp:
// See EDStock.cpp for the implementation of this class
//

class CEDStockApp : public CWinApp
{
public:
	CEDStockApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEDStockApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEDStockApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDSTOCK_H__D7CBE61D_6A05_4848_ADAC_3F2AB25C272E__INCLUDED_)
