#if !defined(AFX_PSWDLG_H__E20626B8_AF6C_4436_B0E6_01DC58AF2CE1__INCLUDED_)
#define AFX_PSWDLG_H__E20626B8_AF6C_4436_B0E6_01DC58AF2CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PswDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PasswordEntryDlg dialog

class PasswordEntryDlg : public CDialog
{
  CString m_cPassword;

// Construction
public:
	PasswordEntryDlg(LPCSTR cpPassword, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PasswordEntryDlg)
	enum { IDD = IDD_PASSWORD_ENTRY };
	CString	m_cPasswordEntered;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PasswordEntryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PasswordEntryDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSWDLG_H__E20626B8_AF6C_4436_B0E6_01DC58AF2CE1__INCLUDED_)
