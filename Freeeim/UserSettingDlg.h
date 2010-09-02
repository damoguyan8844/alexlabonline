#if !defined(AFX_USERSETTINGDLG_H__C466245C_FFAC_48D7_A7BE_1AF54517F1B4__INCLUDED_)
#define AFX_USERSETTINGDLG_H__C466245C_FFAC_48D7_A7BE_1AF54517F1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingDlg dialog
class CEIM02Dlg;

class FreeEIM_UserSettingDlg : public CDialog
{
// Construction
public:
	FreeEIM_UserSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserSettingDlg)
	enum { IDD = IDD_USER_SETTING };
	CComboBox	m_cbGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CEIM02Dlg *m_pMainTop;

	// Generated message map functions
	//{{AFX_MSG(CUserSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSETTINGDLG_H__C466245C_FFAC_48D7_A7BE_1AF54517F1B4__INCLUDED_)
