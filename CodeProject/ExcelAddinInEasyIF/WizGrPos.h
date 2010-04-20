#if !defined(AFX_WIZGRPOS_H__91B9DD8E_6302_41A3_834B_EA743203C90E__INCLUDED_)
#define AFX_WIZGRPOS_H__91B9DD8E_6302_41A3_834B_EA743203C90E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizGrPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WizGrPos dialog

class WizGrPos : public CPropertyPage
{
	DECLARE_DYNCREATE(WizGrPos)

// Construction
public:
	WizGrPos();
	~WizGrPos();

// Dialog Data
	//{{AFX_DATA(WizGrPos)
	enum { IDD = IDD_WIZ_INSERTPOS };
	CString	m_cInsertPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(WizGrPos)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(WizGrPos)
	afx_msg void OnChangeInsposCell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZGRPOS_H__91B9DD8E_6302_41A3_834B_EA743203C90E__INCLUDED_)
