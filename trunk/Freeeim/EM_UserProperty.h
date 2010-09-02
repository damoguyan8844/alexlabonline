#if !defined(AFX_EM_USERPROPERTY_H__CBCD9DA8_5B1B_41C8_B518_09101036AE59__INCLUDED_)
#define AFX_EM_USERPROPERTY_H__CBCD9DA8_5B1B_41C8_B518_09101036AE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EM_UserProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EM_UserProperty dialog

class EM_UserProperty : public CDialog
{
// Construction
public:
	EM_UserProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EM_UserProperty)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EM_UserProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EM_UserProperty)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EM_USERPROPERTY_H__CBCD9DA8_5B1B_41C8_B518_09101036AE59__INCLUDED_)
