#if !defined(AFX_FLDPROPS_H__9683D2EE_DADC_4000_AEA6_072EB972A9E7__INCLUDED_)
#define AFX_FLDPROPS_H__9683D2EE_DADC_4000_AEA6_072EB972A9E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLDProps.h : header file
//

class ExcelColInfo
{
public:
  char m_caExcelHeading[256];
  char m_caColHeading[256];
  int  m_iColType;

  int  m_iFieldType;

  int  m_iColDataType;
  int  m_iCompareOp;

  int m_iExcelCol;
  BOOL m_bUseColumn;

  ExcelColInfo() { m_iColType = 0x0100; m_caColHeading[0] = 0; m_caExcelHeading[0] = 0; }

};


/////////////////////////////////////////////////////////////////////////////
// FLDProps dialog

class FLDProps : public CPropertyPage
{
	DECLARE_DYNCREATE(FLDProps)

  int m_iFieldIndex;
  CObArray *m_pFields;
  ExcelColInfo *m_pColInfo;

  CStringArray m_oOrigOperationChoices;

// Construction
public:
  FLDProps();
  FLDProps(CObArray *pFields, int iFieldIndex);
	~FLDProps();

// Dialog Data
	//{{AFX_DATA(FLDProps)
	enum { IDD = IDD_WIZ_FIELDDEF };
	CString	m_cColCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FLDProps)
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(FLDProps)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWizfldFieldtype();
	afx_msg void OnWizfldKeyfield();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLDPROPS_H__9683D2EE_DADC_4000_AEA6_072EB972A9E7__INCLUDED_)
