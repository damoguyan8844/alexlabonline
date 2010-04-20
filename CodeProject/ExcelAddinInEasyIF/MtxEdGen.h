#if !defined(AFX_MTXEDGEN_H__09CFCD66_D496_44B6_B4C2_89A32048B809__INCLUDED_)
#define AFX_MTXEDGEN_H__09CFCD66_D496_44B6_B4C2_89A32048B809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MtxEdGen.h : header file
//
#include "matrix.h"


/////////////////////////////////////////////////////////////////////////////
// MtxEditGen dialog

class MtxEditGen : public CPropertyPage
{
	DECLARE_DYNCREATE(MtxEditGen)

  Matrix *m_pMatrix;

// Construction
public:
  MtxEditGen();
  MtxEditGen(Matrix *pMatrix);
  ~MtxEditGen();

// Dialog Data
	//{{AFX_DATA(MtxEditGen)
	enum { IDD = IDD_MTXEDIT_GENERAL };
	CString	m_cCreator;
	CString	m_cCategory;
	CString	m_cPathName;
	CString	m_cMatrixName;
	CString	m_cNotes;
	CString	m_cPswAccess;
	CString	m_cPswModify;
	BOOL	m_bReqPswAccess;
	BOOL	m_bReqPswModify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MtxEditGen)
	public:
	virtual BOOL OnKillActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MtxEditGen)
	afx_msg void OnGenReqpswaccess();
	afx_msg void OnGenReqpswmodify();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeGenName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTXEDGEN_H__09CFCD66_D496_44B6_B4C2_89A32048B809__INCLUDED_)
