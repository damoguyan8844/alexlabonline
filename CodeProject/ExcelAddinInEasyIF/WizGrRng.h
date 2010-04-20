#if !defined(AFX_WIZGRRNG_H__A780CD03_1DF0_4D21_A665_C80B2488AB19__INCLUDED_)
#define AFX_WIZGRRNG_H__A780CD03_1DF0_4D21_A665_C80B2488AB19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizGrRng.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WizGrRng dialog

class WizGrRng : public CPropertyPage
{
	DECLARE_DYNCREATE(WizGrRng)

// Construction
public:
  CMapWordToPtr m_oColRange;

	WizGrRng();
	~WizGrRng();

// Dialog Data
	//{{AFX_DATA(WizGrRng)
	enum { IDD = IDD_WIZGRID_RANGE };
	CListBox	m_oList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(WizGrRng)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(WizGrRng)
	afx_msg void OnDblclkWizrangeList();
	afx_msg void OnWizrangeRemove();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZGRRNG_H__A780CD03_1DF0_4D21_A665_C80B2488AB19__INCLUDED_)
