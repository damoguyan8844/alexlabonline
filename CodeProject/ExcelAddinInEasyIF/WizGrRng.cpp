// WizGrRng.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "WizGrRng.h"
#include "GridWiz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WizGrRng property page

IMPLEMENT_DYNCREATE(WizGrRng, CPropertyPage)

WizGrRng::WizGrRng() : CPropertyPage(WizGrRng::IDD)
{
	//{{AFX_DATA_INIT(WizGrRng)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

WizGrRng::~WizGrRng()
{
}

void WizGrRng::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WizGrRng)
	DDX_Control(pDX, IDC_WIZRANGE_LIST, m_oList);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      m_oColRange.RemoveAll();

      int iCount = m_oList.GetCount();
      for (int iLup = 0; iLup < iCount; iLup++)
        {
          DWORD dwData = m_oList.GetItemData(iLup);
          if (dwData > 0 && dwData != LB_ERR)
            m_oColRange.SetAt((WORD)(dwData-1),(void *)(dwData-1));
        }
    }
}


BEGIN_MESSAGE_MAP(WizGrRng, CPropertyPage)
	//{{AFX_MSG_MAP(WizGrRng)
	ON_LBN_DBLCLK(IDC_WIZRANGE_LIST, OnDblclkWizrangeList)
	ON_BN_CLICKED(IDC_WIZRANGE_REMOVE, OnWizrangeRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WizGrRng message handlers

void WizGrRng::OnDblclkWizrangeList()
{
  OnWizrangeRemove();
}



void WizGrRng::OnWizrangeRemove()
{
  int iIndex = m_oList.GetCurSel();

  if (iIndex > -1)
    m_oList.DeleteString(iIndex);
}



BOOL WizGrRng::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL WizGrRng::OnSetActive()
{
  BOOL bRetval = CPropertyPage::OnSetActive();

  ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

  m_oList.ResetContent();

  Matrix *pMatrix = ((GridWiz *)GetParent())->m_pSelect->m_pMatrix;
  CString cTitle;
  BOOL bOK;
  int iCount = pMatrix->GetColumnCount();
  int iAdded = 0;

  for (int iLup = 0; iLup < (iCount-1); iLup++)
    {
      MatrixColDef *pColDef = pMatrix->GetColumnDef(iLup);
      MatrixColDef *pNextColDef = pMatrix->GetColumnDef(iLup+1);

      if (pColDef->m_iColType == pNextColDef->m_iColType)
        {
          if ((pColDef->m_iCompareOp == COMPAREOP_GT || pColDef->m_iCompareOp == COMPAREOP_GE) &&
              (pNextColDef->m_iCompareOp == COMPAREOP_LT || pNextColDef->m_iCompareOp == COMPAREOP_LE))
            {
              bOK = FALSE;

              cTitle = pColDef->m_cColName;
              int iPos = cTitle.Find(' ');
              if (iPos > 0)
                {
                  bOK = (pNextColDef->m_cColName.Find(cTitle.Left(iPos)) > -1);
                  if (!bOK)
                    {
                      cTitle = cTitle.Mid(iPos+1);
                      iPos = cTitle.Find(' ');
                      if (iPos > -1)
                        bOK = (pNextColDef->m_cColName.Find(cTitle.Left(iPos)) > -1);
                      else
                        bOK = (pNextColDef->m_cColName.Find(cTitle) > -1);
                    }
                }

              if (bOK)
                {
                  cTitle = pColDef->m_cColName + CString(" - ") + pNextColDef->m_cColName;
                  m_oList.SetItemData(m_oList.AddString(cTitle),iLup+1);
                  iAdded++;
                }
            }
        }
    }

  if (iAdded < 1)
    {
      cTitle.LoadString(IDS_NORANGEITEMSDETECTED);
      m_oList.SetItemData(m_oList.AddString(cTitle),0);
      m_oList.SetCurSel(-1);
    }
  else
    m_oList.SetCurSel(0);

  GetDlgItem(IDC_WIZRANGE_REMOVE)->EnableWindow(iAdded > 0);
  m_oList.EnableWindow(iAdded > 0);

  return bRetval;
}

BOOL WizGrRng::OnWizardFinish() 
{
  UpdateData();
	
	return CPropertyPage::OnWizardFinish();
}
