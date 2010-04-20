#if !defined(AFX_MTXEDIT_H__56FF7F8B_2525_442C_8275_5F07575A2ACB__INCLUDED_)
#define AFX_MTXEDIT_H__56FF7F8B_2525_442C_8275_5F07575A2ACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MtxEdit.h : header file
//

#include "mtxedgen.h"
#include "mtxedcol.h"
#include "mtxeddat.h"

/////////////////////////////////////////////////////////////////////////////
// MatrixEdit

class MatrixEdit : public CPropertySheet
{
	DECLARE_DYNAMIC(MatrixEdit)

public:
  Matrix *m_pMatrix;

  MtxEditGen *m_pGeneralTab;
  MatrixEditCol *m_pColumnTab;
  MatrixEditData *m_pDataTab;

// Construction
public:
  MatrixEdit(Matrix *pMatrix, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  MatrixEdit(Matrix *pMatrix, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

  BOOL CheckColumnsDefined();
  void EnableOKButton();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatrixEdit)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MatrixEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(MatrixEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTXEDIT_H__56FF7F8B_2525_442C_8275_5F07575A2ACB__INCLUDED_)
