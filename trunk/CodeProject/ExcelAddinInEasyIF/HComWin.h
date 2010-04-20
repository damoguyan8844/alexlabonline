#if !defined(AFX_HCOMWIN_H__34AA4F94_D9F1_456C_A847_BC26A7D76C47__INCLUDED_)
#define AFX_HCOMWIN_H__34AA4F94_D9F1_456C_A847_BC26A7D76C47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HComWin.h : header file
//
#include "excel9.h"

#define WM_RECALCULATE_EXCEL     (WM_USER+987)

/////////////////////////////////////////////////////////////////////////////
// HiddenComWin frame

class HiddenComWin : public CFrameWnd
{
	DECLARE_DYNCREATE(HiddenComWin)
protected:
	HiddenComWin();           // protected constructor used by dynamic creation

// Attributes
public:
  _Application m_oExcelApplication;

  BOOL m_bInRecalc;
  BOOL m_bNeedsRecalc;
  UINT m_iRecalcInterval;

  UINT m_iTimerID;

// Operations
public:
  HiddenComWin(LPDISPATCH pXLDisp);
	virtual ~HiddenComWin();

  void ResetTimerInterval(UINT iNewInterval);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HiddenComWin)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HiddenComWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
  afx_msg LRESULT OnRecalcExcel(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HCOMWIN_H__34AA4F94_D9F1_456C_A847_BC26A7D76C47__INCLUDED_)
