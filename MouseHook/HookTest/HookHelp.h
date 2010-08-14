#if !defined(AFX_HOOKHELP_H__346C392D_14BC_11D5_A025_006067718D04__INCLUDED_)
#define AFX_HOOKHELP_H__346C392D_14BC_11D5_A025_006067718D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HookHelp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHookHelp dialog

class CHookHelp : public CDialog
{
// Construction
public:
	CHookHelp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHookHelp)
	enum { IDD = IDD_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHookHelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHookHelp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKHELP_H__346C392D_14BC_11D5_A025_006067718D04__INCLUDED_)
