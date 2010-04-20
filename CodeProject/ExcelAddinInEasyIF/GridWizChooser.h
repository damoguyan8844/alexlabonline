#if !defined(AFX_GRIDWIZCHOOSER_H__54901A48_4D01_4A74_9060_7262978ECA2E__INCLUDED_)
#define AFX_GRIDWIZCHOOSER_H__54901A48_4D01_4A74_9060_7262978ECA2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridWizChooser.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// GridWizChooser dialog

class GridWizChooser : public CDialog
{
// Construction
public:
  CBitmap m_oBitmaps[3];

  int m_iWizardSelection;

	GridWizChooser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GridWizChooser)
	enum { IDD = IDD_GRIDWIZ_CHOOSER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GridWizChooser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GridWizChooser)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnGridwizCgcolumn();
	afx_msg void OnGridwizCgimport();
	afx_msg void OnGridwizInsgrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDWIZCHOOSER_H__54901A48_4D01_4A74_9060_7262978ECA2E__INCLUDED_)
