#if !defined(AFX_EIM_VIEWHISTORY_H__E97AA42D_4C88_460E_8A91_82214CE90D88__INCLUDED_)
#define AFX_EIM_VIEWHISTORY_H__E97AA42D_4C88_460E_8A91_82214CE90D88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EIM_ViewHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EIM_ViewHistory dialog

class EIM_ViewHistory : public CDialog
{
// Construction
public:
	CImageList& EIM_ViewHistory::GetImageList();
	HBITMAP GetImage(CImageList& ilist, int num);
	void AddRecvText(LPCTSTR msg);
	void _InsertFace(DWORD dwFace);
	void _InsertText(LPCTSTR lpszText);
	void ReadUserHistory(LPCTSTR szUserName);
	void AddUserList(LPCTSTR szDate);
	EIM_ViewHistory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EIM_ViewHistory)
	enum { IDD = IDD_VIEWHISTORY };
	CRichEditCtrl	m_richHistory;
	CDateTimeCtrl	m_date;
	CComboBox	m_cbUserList;
	char *m_strDate;
	char *m_strFullDate;
	char *m_strSelUser;
	char *m_szProgramDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EIM_ViewHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	inline void AddFailedMsg();
	HICON m_hIcon;
	CImageList m_imgListFaces;

	// Generated message map functions
	//{{AFX_MSG(EIM_ViewHistory)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EIM_VIEWHISTORY_H__E97AA42D_4C88_460E_8A91_82214CE90D88__INCLUDED_)
