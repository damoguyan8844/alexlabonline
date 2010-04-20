// ColGen.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "ColGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColGeneral property page

IMPLEMENT_DYNCREATE(ColGeneral, CPropertyPage)

ColGeneral::ColGeneral() : CPropertyPage(ColGeneral::IDD)
{
	//{{AFX_DATA_INIT(ColGeneral)
	m_cColName = _T("");
	m_bCompareCol = FALSE;
	m_iCompareOp = -1;
	m_iDataType = -1;
	m_cNotes = _T("");
	//}}AFX_DATA_INIT
}


ColGeneral::ColGeneral(Matrix *pMatrix, int iColIndex)
  : CPropertyPage(ColGeneral::IDD)
{
  if (!m_cColName.LoadString(IDS_NEWCOLUMNNAME))
    m_cColName = _T("New Column");

	m_bCompareCol = TRUE;
	m_iCompareOp = 0;
	m_iDataType = 0;
	m_cNotes = _T("");

  if (iColIndex > -1)
    {
      MatrixColDef *pColDef = pMatrix->GetColumnDef(iColIndex);
      if (pColDef)
        {
          m_cColName = pColDef->m_cColName;
          m_bCompareCol = pColDef->m_bCompareCol;
          m_iCompareOp = pColDef->m_iCompareOp;
          m_iDataType = pColDef->m_iColType;
          m_cNotes = pColDef->m_cNotes;
        }
    }
}


ColGeneral::~ColGeneral()
{
}

void ColGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColGeneral)
	DDX_Text(pDX, IDC_COLEDIT_COLNAME, m_cColName);
	DDX_Check(pDX, IDC_COLEDIT_COMPARECOL, m_bCompareCol);
	DDX_CBIndex(pDX, IDC_COLEDIT_COMPAREOP, m_iCompareOp);
	DDX_CBIndex(pDX, IDC_COLEDIT_DATATYPE, m_iDataType);
	DDX_Text(pDX, IDC_COLEDIT_NOTES, m_cNotes);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      m_cColName.TrimRight();
      if (m_cColName.IsEmpty())
        {
          CString cErrMsg, cErrHeading;
          if (!cErrMsg.LoadString(IDS_COLUMNNAMEBLANK))
            cErrMsg = "Column Name May Not Be Blank!";

          if (!cErrHeading.LoadString(IDS_ENTRYERROR))
            cErrHeading = "Entry Error";

          ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cErrMsg,(LPCSTR)cErrHeading, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_COLEDIT_COLNAME);
          pDX->Fail();
        }
    }
}


BEGIN_MESSAGE_MAP(ColGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(ColGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColGeneral message handlers

BOOL ColGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
