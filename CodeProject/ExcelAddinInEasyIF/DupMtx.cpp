// DupMtx.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "DupMtx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DuplicateMatrix dialog


DuplicateMatrix::DuplicateMatrix(MatrixManager *pMatrixMgr, CWnd* pParent /*=NULL*/)
	: CDialog(DuplicateMatrix::IDD, pParent)
{
  m_pMatrixMgr = pMatrixMgr;

	//{{AFX_DATA_INIT(DuplicateMatrix)
	m_cMatrixName = _T("");
	//}}AFX_DATA_INIT
}


void DuplicateMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DuplicateMatrix)
	DDX_Text(pDX, IDC_COPYMATRIX_NAME, m_cMatrixName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DuplicateMatrix, CDialog)
	//{{AFX_MSG_MAP(DuplicateMatrix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DuplicateMatrix message handlers

BOOL DuplicateMatrix::OnInitDialog()
{
  CDialog::OnInitDialog();

  CenterWindow();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void DuplicateMatrix::OnOK()
{
  UpdateData();

  if (m_pMatrixMgr->GetMatrix(m_cMatrixName))
    {
      CString cGridExistsHeading, cGridExistsMessage;

      if (!cGridExistsHeading.LoadString(IDS_GRIDEXISTSHEADING))
        cGridExistsHeading = "Grid Exists";

      if (!cGridExistsMessage.LoadString(IDS_GRIDEXISTSMESSAGE))
        cGridExistsMessage = "A Grid With This Name Already Exists!\x0D\x0A Please ReEnter A Unique Name For This Grid.";

      ::MessageBox(m_hWnd,(LPCSTR)cGridExistsMessage,(LPCSTR)cGridExistsHeading, MB_ICONEXCLAMATION | MB_OK);
    }
  else
    CDialog::OnOK();
}
