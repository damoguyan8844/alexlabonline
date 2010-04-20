#if !defined(AFX_FLOATBTN_H__376ACED1_D911_4A34_B006_F816FCD412FB__INCLUDED_)
#define AFX_FLOATBTN_H__376ACED1_D911_4A34_B006_F816FCD412FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FloatBtn window

class FloatBtn : public CButton
{
// Construction
public:
	FloatBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FloatBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FloatBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(FloatBtn)
		// NOTE - the ClassWizard will add and remove member functions here.
  afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATBTN_H__376ACED1_D911_4A34_B006_F816FCD412FB__INCLUDED_)
