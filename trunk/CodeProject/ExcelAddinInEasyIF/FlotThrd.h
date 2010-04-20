#if !defined(AFX_FLOTTHRD_H__998A5EFB_5C41_4DAF_AF6E_3B22A6E52F93__INCLUDED_)
#define AFX_FLOTTHRD_H__998A5EFB_5C41_4DAF_AF6E_3B22A6E52F93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlotThrd.h : header file
//
#include "flotgrid.h"
//#include "excel9.h"

/////////////////////////////////////////////////////////////////////////////
// FloatingThread thread

class FloatingThread : public CWinThread
{
	DECLARE_DYNCREATE(FloatingThread)
protected:
	FloatingThread();           // protected constructor used by dynamic creation

// Attributes
public:

  HWND m_hExcelMainWnd;

  CWinApp *m_pMainApp;

  HWND m_hComWnd;

// Operations
public:

  FloatingGridWnd *m_pFloatingGrid;

  // void RefreshFloatingGrids()  { if (m_pFloatingGrid) m_pFloatingGrid->LoadMatrices(); }
  void RefreshFloatingGrids()  { if (m_pFloatingGrid)
                                   m_pFloatingGrid->PostMessage(WM_COMMAND,MAKEWPARAM(9874,BN_CLICKED)); }

  void ToggleFloatingGridDisplay()  { if (m_pFloatingGrid) m_pFloatingGrid->ToggleDisplay(); }

  void ExcelRecalculateFull();

  BOOL IsRecalcOnChange();
  void SetRecalcOnChange(BOOL bRecalc = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FloatingThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~FloatingThread();

	// Generated message map functions
	//{{AFX_MSG(FloatingThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOTTHRD_H__998A5EFB_5C41_4DAF_AF6E_3B22A6E52F93__INCLUDED_)
