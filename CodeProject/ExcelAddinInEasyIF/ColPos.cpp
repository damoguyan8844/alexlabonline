// ColPos.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "ColPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColPosition property page

IMPLEMENT_DYNCREATE(ColPosition, CPropertyPage)

ColPosition::ColPosition() : CPropertyPage(ColPosition::IDD)
{
	//{{AFX_DATA_INIT(ColPosition)
	m_iInsertBefore = 0;
	//}}AFX_DATA_INIT

  m_bAppend = TRUE;
}



ColPosition::ColPosition(Matrix *pMatrix) : CPropertyPage(ColPosition::IDD)
{
  m_pMatrix = pMatrix;
  m_bAppend = TRUE;
  
  m_iInsertBefore = 0;
}
                                             


ColPosition::~ColPosition()
{
}

void ColPosition::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColPosition)
	DDX_LBIndex(pDX, IDC_COLEDIT_INSERTLIST, m_iInsertBefore);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    m_bAppend = SendDlgItemMessage(IDC_COLEDIT_APPEND,BM_GETCHECK);
}


BEGIN_MESSAGE_MAP(ColPosition, CPropertyPage)
	//{{AFX_MSG_MAP(ColPosition)
	ON_BN_CLICKED(IDC_COLEDIT_APPEND, OnColeditAppend)
	ON_BN_CLICKED(IDC_COLEDIT_INSERT, OnColeditInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColPosition message handlers

BOOL ColPosition::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
	
  MatrixColDef *pColDef;
  int iCount = m_pMatrix->GetColumnCount();
  CListBox *pLB = (CListBox *)GetDlgItem(IDC_COLEDIT_INSERTLIST);

  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pColDef = m_pMatrix->GetColumnDef(iLup);
      if (pColDef)
        pLB->AddString(pColDef->m_cColName);
    }

  if (iCount > 0)
    pLB->SetCurSel(0);
  
  SendDlgItemMessage(IDC_COLEDIT_APPEND,BM_SETCHECK,1);
  SendDlgItemMessage(IDC_COLEDIT_INSERT,BM_SETCHECK,0);

  if (iCount < 1)
    GetDlgItem(IDC_COLEDIT_INSERT)->EnableWindow(FALSE);

  OnColeditAppend();
	
  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void ColPosition::OnColeditAppend() 
{
  BOOL bChecked = SendDlgItemMessage(IDC_COLEDIT_APPEND,BM_GETCHECK);
  GetDlgItem(IDC_COLEDIT_INSERTLIST)->EnableWindow(!bChecked);
}



void ColPosition::OnColeditInsert() 
{
  BOOL bChecked = SendDlgItemMessage(IDC_COLEDIT_INSERT,BM_GETCHECK);
  GetDlgItem(IDC_COLEDIT_INSERTLIST)->EnableWindow(bChecked);
}
