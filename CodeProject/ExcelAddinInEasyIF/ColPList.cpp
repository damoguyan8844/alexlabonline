// ColPList.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "ColPList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColPickList property page

IMPLEMENT_DYNCREATE(ColPickList, CPropertyPage)

ColPickList::ColPickList() : CPropertyPage(ColPickList::IDD)
{
	//{{AFX_DATA_INIT(ColPickList)
	m_cChoices = _T("");
	m_bRestrictToList = FALSE;
	m_bUsePickList = FALSE;
	//}}AFX_DATA_INIT
}



ColPickList::ColPickList(Matrix *pMatrix, int iColIndex)
  : CPropertyPage(ColPickList::IDD)
{
	m_cChoices = _T("");
	m_bRestrictToList = FALSE;
	m_bUsePickList = FALSE;

  if (iColIndex > -1)
    {
      MatrixColDef *pColDef = pMatrix->GetColumnDef(iColIndex);
      if (pColDef)
        {
          m_bUsePickList = pColDef->m_bUsePickList;
          m_bRestrictToList = pColDef->m_bRestrictToList;

          int iCount = pColDef->m_oPickListChoices.GetSize();
          for (int iLup = 0; iLup < iCount; iLup++)
            {                
              m_cChoices += pColDef->m_oPickListChoices.GetAt(iLup);
              if (iLup < (iCount-1))
                m_cChoices += CString("\x0D\x0A");
            }
        }
    }
}



ColPickList::~ColPickList()
{
}

void ColPickList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColPickList)
	DDX_Text(pDX, IDC_PICKLIST_CHOICES, m_cChoices);
	DDX_Check(pDX, IDC_PICKLIST_RESTRICT2LIST, m_bRestrictToList);
	DDX_Check(pDX, IDC_PICKLIST_USEPICKLIST, m_bUsePickList);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(ColPickList, CPropertyPage)
	//{{AFX_MSG_MAP(ColPickList)
	ON_BN_CLICKED(IDC_PICKLIST_USEPICKLIST, OnPicklistUsepicklist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColPickList message handlers

BOOL ColPickList::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
	
  OnPicklistUsepicklist();
	
  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void ColPickList::OnPicklistUsepicklist() 
{
  BOOL bChecked = SendDlgItemMessage(IDC_PICKLIST_USEPICKLIST,BM_GETCHECK);
  GetDlgItem(IDC_PICKLIST_CHOICES_LBL)->EnableWindow(bChecked);
  GetDlgItem(IDC_PICKLIST_CHOICES)->EnableWindow(bChecked);
  GetDlgItem(IDC_PICKLIST_RESTRICT2LIST)->EnableWindow(bChecked);
}
