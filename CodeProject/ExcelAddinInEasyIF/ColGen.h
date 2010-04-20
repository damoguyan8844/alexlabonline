#if !defined(AFX_COLGEN_H__3D8215AF_AAE3_4E34_B4E4_1B7B56E9D7DB__INCLUDED_)
#define AFX_COLGEN_H__3D8215AF_AAE3_4E34_B4E4_1B7B56E9D7DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColGen.h : header file
//
#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// ColGeneral dialog

class ColGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(ColGeneral)

// Construction
public:
  ColGeneral();
  ColGeneral(Matrix *pMatrix, int iColIndex);
  ~ColGeneral();

// Dialog Data
	//{{AFX_DATA(ColGeneral)
	enum { IDD = IDD_COLEDIT_GENERAL };
	CString	m_cColName;
	BOOL	m_bCompareCol;
	int		m_iCompareOp;
	int		m_iDataType;
	CString	m_cNotes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ColGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ColGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
};


#endif // !defined(AFX_COLGEN_H__3D8215AF_AAE3_4E34_B4E4_1B7B56E9D7DB__INCLUDED_)
