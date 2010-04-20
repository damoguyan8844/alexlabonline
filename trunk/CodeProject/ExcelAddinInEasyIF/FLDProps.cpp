// FLDProps.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FLDProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FLDProps property page

IMPLEMENT_DYNCREATE(FLDProps, CPropertyPage)

FLDProps::FLDProps() : CPropertyPage(FLDProps::IDD)
{
  m_iFieldIndex = 0;
  m_pFields = NULL;
  m_pColInfo = NULL;
}


FLDProps::FLDProps(CObArray *pFields, int iFieldIndex) : CPropertyPage(FLDProps::IDD)
{
  m_iFieldIndex = iFieldIndex;
  m_pFields = pFields;
  m_pColInfo = (ExcelColInfo *)m_pFields->GetAt(m_iFieldIndex);

	//{{AFX_DATA_INIT(FLDProps)
  m_cColCaption = m_pColInfo->m_caColHeading;
	//}}AFX_DATA_INIT
}

FLDProps::~FLDProps()
{
}

void FLDProps::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FLDProps)
	DDX_Text(pDX, IDC_WIZFLD_CAPTION, m_cColCaption);
  DDX_CBIndex(pDX, IDC_WIZFLD_DATATYPE, m_pColInfo->m_iColDataType);
  DDX_CBIndex(pDX, IDC_WIZFLD_FIELDTYPE, m_pColInfo->m_iFieldType);
  DDX_Check(pDX, IDC_WIZFLD_KEYFIELD, m_pColInfo->m_bUseColumn);
  DDX_CBIndex(pDX, IDC_WIZFLD_OPERATION, m_pColInfo->m_iCompareOp);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      strcpy(m_pColInfo->m_caColHeading,(LPCSTR)m_cColCaption);
    }
}


BEGIN_MESSAGE_MAP(FLDProps, CPropertyPage)
	//{{AFX_MSG_MAP(FLDProps)
	ON_CBN_SELCHANGE(IDC_WIZFLD_FIELDTYPE, OnSelchangeWizfldFieldtype)
	ON_BN_CLICKED(IDC_WIZFLD_KEYFIELD, OnWizfldKeyfield)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FLDProps message handlers

BOOL FLDProps::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

  CTabCtrl *pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_WIZFLD_COLINDICATOR);

  ExcelColInfo *pColInfo;
  for (int iLup = 0; iLup < m_pFields->GetSize(); iLup++)
    {
      pColInfo = (ExcelColInfo *)m_pFields->GetAt(iLup);
      pTabCtrl->InsertItem(iLup,pColInfo->m_caExcelHeading);
    }

  pTabCtrl->SetCurSel(m_iFieldIndex);

  CString cChoice;
  CComboBox *pCB = (CComboBox *)GetDlgItem(IDC_WIZFLD_OPERATION);
  for (iLup = 0; iLup < pCB->GetCount(); iLup++)
    {
      pCB->GetLBText(iLup,cChoice);
      m_oOrigOperationChoices.Add(cChoice);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void FLDProps::OnSelchangeWizfldFieldtype()
{
  int iIndex = SendDlgItemMessage(IDC_WIZFLD_FIELDTYPE,CB_GETCURSEL);
  if (iIndex != m_pColInfo->m_iFieldType)
    {
      if (iIndex == 1 || m_pColInfo->m_iFieldType == 1)
        {
          CComboBox *pCB = (CComboBox *)GetDlgItem(IDC_WIZFLD_OPERATION);
          int iOpIndex = pCB->GetCurSel();
          pCB->ResetContent();
          if (iIndex == 1)
            {
              pCB->AddString(" >= -- <");
              pCB->AddString(" >= -- <=");
              pCB->AddString(" > -- <=");
              pCB->AddString(" > -- <");

              CString cTitle;
              GetDlgItemText(IDC_WIZFLD_CAPTION,cTitle);
              if (cTitle.Find(',') < 0)
                {
/// INTERNATIONALIZE
                  CString cNew = cTitle + CString(" From, ") + cTitle + CString(" To");
                  SetDlgItemText(IDC_WIZFLD_CAPTION,cNew);
                }
            }
          else
            {
              for (int iLup = 0; iLup < m_oOrigOperationChoices.GetSize(); iLup++)
                pCB->AddString(m_oOrigOperationChoices.GetAt(iLup));

              CString cTitle;
              GetDlgItemText(IDC_WIZFLD_CAPTION,cTitle);
              int iPos = cTitle.Find(',');
              if (iPos > -1)
                {
                  cTitle = cTitle.Left(iPos);
/// INTERNATIONALIZE
                  if (cTitle.Right(4) == "From")
                    cTitle = cTitle.Left(cTitle.GetLength()-4);

                  cTitle.TrimRight();
                  SetDlgItemText(IDC_WIZFLD_CAPTION,cTitle);
                }
            }

          if (iOpIndex >= pCB->GetCount())
            iOpIndex = 0;

          pCB->SetCurSel(iOpIndex);
        }

      m_pColInfo->m_iFieldType = iIndex;

      BOOL bEnabled = (BOOL)SendDlgItemMessage(IDC_WIZFLD_KEYFIELD,BM_GETCHECK);
      if (bEnabled)
        {
          if (iIndex == 3) // param only
            bEnabled = FALSE;

          GetDlgItem(IDC_WIZFLD_CAPTION)->EnableWindow(bEnabled);
          GetDlgItem(IDC_WIZFLD_DATATYPE)->EnableWindow(bEnabled);
          GetDlgItem(IDC_WIZFLD_OPERATION)->EnableWindow(bEnabled);
        }
    }
}



void FLDProps::OnWizfldKeyfield()
{
  BOOL bEnabled = (BOOL)SendDlgItemMessage(IDC_WIZFLD_KEYFIELD,BM_GETCHECK);

  GetDlgItem(IDC_WIZFLD_CAPTION)->EnableWindow(bEnabled);
  GetDlgItem(IDC_WIZFLD_DATATYPE)->EnableWindow(bEnabled);
  GetDlgItem(IDC_WIZFLD_FIELDTYPE)->EnableWindow(bEnabled);
  GetDlgItem(IDC_WIZFLD_OPERATION)->EnableWindow(bEnabled);
}



BOOL FLDProps::OnSetActive()
{
  ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
  return CPropertyPage::OnSetActive();
}
