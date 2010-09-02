#if !defined(AFX_FIM_SERIALNUMBERDLG_H__0E05FA31_4BB7_46DA_AA24_E0336DD55D5F__INCLUDED_)
#define AFX_FIM_SERIALNUMBERDLG_H__0E05FA31_4BB7_46DA_AA24_E0336DD55D5F__INCLUDED_

#include "thirdpartclass/HtmlEditCtrl2.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FIM_SerialNumberDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFIM_SerialNumberDlg dialog

class CFIM_SerialNumberDlg : public CDialog
{
// Construction
public:
	BOOL _IsRegistered();
	void GetProgramDirectory(char *buf);
	CFIM_SerialNumberDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_strSN;
	BOOL m_bConfirm;
// Dialog Data
	//{{AFX_DATA(CFIM_SerialNumberDlg)
	enum { IDD = IDD_DIALOG5 };
	CString	m_strSN1;
	CString	m_strSN2;
	CString	m_strSN3;
	CString	m_strSN4;
	CString	m_strHDDSerial;
	CHtmlEditCtrl2	m_ctrlWeb;
	CString	m_strLicence;
	BOOL	m_bAccept;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFIM_SerialNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFIM_SerialNumberDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnNavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIM_SERIALNUMBERDLG_H__0E05FA31_4BB7_46DA_AA24_E0336DD55D5F__INCLUDED_)
