// EMX.h : main header file for the EMX DLL
//

#if !defined(AFX_EMX_H__0F11A406_F2FB_11D4_B288_00105A995496__INCLUDED_)
#define AFX_EMX_H__0F11A406_F2FB_11D4_B288_00105A995496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "matrix.h"
#include "flotthrd.h"
#include "HComWin.h"

/////////////////////////////////////////////////////////////////////////////
// CEMXApp
// See EMX.cpp for the implementation of this class
//

class CEMXApp : public CWinApp
{
public:
  MatrixManager m_oMatrixManager;

  DWORD m_dwRecalcInterval;

  FloatingThread *m_pFloatingGridsThread;

  HiddenComWin *m_pHiddenComWnd;

  BOOL m_bRecalcOnChange;

  CEMXApp();

  void RefreshFloatingGrids()  { m_pFloatingGridsThread->RefreshFloatingGrids(); }

  void ToggleFloatingGridDisplay()  { m_pFloatingGridsThread->ToggleFloatingGridDisplay(); }


  BOOL IsRecalcOnChange()                      { return m_bRecalcOnChange; }
  void SetRecalcOnChange(BOOL bRecalc = TRUE)  { m_bRecalcOnChange = bRecalc; }

  void SaveSettings();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMXApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEMXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMX_H__0F11A406_F2FB_11D4_B288_00105A995496__INCLUDED_)
