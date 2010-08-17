#if !defined(AFX_AXTIVE_H__AF9EEC23_1C99_4E7B_993E_963E865DD863__INCLUDED_)
#define AFX_AXTIVE_H__AF9EEC23_1C99_4E7B_993E_963E865DD863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Axtive.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxtive dialog

class CAxtive : public CDialog
{
// Construction
public:
	HWND hwndCapture;
	CRect rectCapture;
	CAxtive(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAxtive)
	enum { IDD = IDD_AXTIVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxtive)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAxtive)
	virtual void OnOK();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXTIVE_H__AF9EEC23_1C99_4E7B_993E_963E865DD863__INCLUDED_)
