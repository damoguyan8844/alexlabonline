#if !defined(AFX_MTXEDCOL_H__9F2EBE78_17A7_48C4_99F5_BEAC5755947A__INCLUDED_)
#define AFX_MTXEDCOL_H__9F2EBE78_17A7_48C4_99F5_BEAC5755947A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MtxEdCol.h : header file
//

#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// MatrixEditCol dialog

class MatrixEditCol : public CPropertyPage
{
	DECLARE_DYNCREATE(MatrixEditCol)
  Matrix *m_pMatrix;

// Construction
public:
	MatrixEditCol();
	MatrixEditCol(Matrix *pMatrix);
	~MatrixEditCol();

  void LoadColumnList();

// Dialog Data
	//{{AFX_DATA(MatrixEditCol)
	enum { IDD = IDD_MTXEDIT_COLUMNS };
	CListCtrl	m_oColList;
	int		m_iReturnType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MatrixEditCol)
	public:
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MatrixEditCol)
	afx_msg void OnColsAdd();
	afx_msg void OnColsProperties();
	afx_msg void OnColsRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkColsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColsMovedown();
	afx_msg void OnColsMoveup();
	afx_msg void OnItemchangedColsList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTXEDCOL_H__9F2EBE78_17A7_48C4_99F5_BEAC5755947A__INCLUDED_)
