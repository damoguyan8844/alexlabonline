#if !defined(AFX_WIZGRSEL_H__054609FF_4AB1_47D0_85B6_FF2D04261FC8__INCLUDED_)
#define AFX_WIZGRSEL_H__054609FF_4AB1_47D0_85B6_FF2D04261FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizGrSel.h : header file
//
#include "matrix.h"
#include "mtrxsel.h"


/////////////////////////////////////////////////////////////////////////////
// WizGrSel dialog

class WizGrSel : public CPropertyPage
{
	DECLARE_DYNCREATE(WizGrSel)

  CImageList m_oImgList;

  void BuildTreeView();
  void AddMatrixToCategoryList(CString cCategory, Matrix *pMatrix, CObArray *pList);
  void SortCategoryArray(CObArray *pList);
  void SortMatrixArray(CObArray *pList);

// Construction
public:
  CString m_cMatrix;
  Matrix *m_pMatrix;

	WizGrSel();
	~WizGrSel();

// Dialog Data
	//{{AFX_DATA(WizGrSel)
	enum { IDD = IDD_WIZ_SELECTMATRIX };
	CTreeCtrl	m_oTreeCtrl;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(WizGrSel)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(WizGrSel)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnSelchangedMatrixTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZGRSEL_H__054609FF_4AB1_47D0_85B6_FF2D04261FC8__INCLUDED_)
