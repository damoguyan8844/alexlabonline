#if !defined(AFX_MTRXSEL_H__38956A24_F5FF_11D4_B28A_50FE6A000000__INCLUDED_)
#define AFX_MTRXSEL_H__38956A24_F5FF_11D4_B28A_50FE6A000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MtrxSel.h : header file
//

typedef struct tagMTXTREEITEMSTRUCT
{
  BOOL m_bLeafItem;
  void *m_vpData;
} MTXTREEITEMSTRUCT, * LPMTXTREEITEMSTRUCT;


class MatrixEntry
{
public:
  CString m_cCategory;
  CObArray m_oMatrixList;
};

/////////////////////////////////////////////////////////////////////////////
// MatrixSel dialog

class MatrixSel : public CDialog
{
  CImageList m_oImgList;

  CObArray m_oItemsToFreeList;

  void FreeMemoryStructs();
  void BuildTreeView();
  void AddMatrixToCategoryList(CString cCategory, Matrix *pMatrix, CObArray *pList);
  void SortCategoryArray(CObArray *pList);
  void SortMatrixArray(CObArray *pList);

  BOOL CheckModifyRights(Matrix *pMatrix);
  BOOL CheckAccessRights(Matrix *pMatrix);

// Construction
public:
  MatrixManager *m_pMatrixMgr;

  BOOL m_bMatricesChanged;

  MatrixSel(MatrixManager *pMatrixMgr, CWnd* pParent = NULL);   // standard constructor
  virtual ~MatrixSel();

// Dialog Data
	//{{AFX_DATA(MatrixSel)
	enum { IDD = IDD_MATRIX_SELECTOR };
	CTreeCtrl	m_oTreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatrixSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MatrixSel)
	afx_msg void OnMatrixAdd();
	afx_msg void OnMatrixProperties();
	afx_msg void OnMatrixRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkMatrixTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickMatrixTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedMatrixTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMatrixCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTRXSEL_H__38956A24_F5FF_11D4_B28A_50FE6A000000__INCLUDED_)
