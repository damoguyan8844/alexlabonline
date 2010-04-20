// GFCMatrixDetails.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GFCMatrixDetails.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GFCMatrixDetails property page

IMPLEMENT_DYNCREATE(GFCMatrixDetails, CPropertyPage)

GFCMatrixDetails::GFCMatrixDetails() : CPropertyPage(GFCMatrixDetails::IDD)
{
	//{{AFX_DATA_INIT(GFCMatrixDetails)
	m_cDefValue = _T("");
/// INTERNATIONALIZE
	m_cMatrixName = _T("New Matrix");
	m_iReturnType = 2;
  m_bLaunchEditor = TRUE;
	//}}AFX_DATA_INIT
}

GFCMatrixDetails::~GFCMatrixDetails()
{
}

void GFCMatrixDetails::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GFCMatrixDetails)
	DDX_Text(pDX, IDC_WIZGRIDINFO_DEFVALUE, m_cDefValue);
	DDX_Text(pDX, IDC_WIZGRIDINFO_GRIDNAME, m_cMatrixName);
	DDX_CBIndex(pDX, IDC_WIZGRIDINFO_RETURNTYPE, m_iReturnType);
	DDX_Check(pDX, IDC_WIZGRIDINFO_LAUNCHEDITOR, m_bLaunchEditor);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate && m_cDefValue == "*")
    m_cDefValue.Empty();

  if (pDX->m_bSaveAndValidate)
    {
      if (((CEMXApp *)AfxGetApp())->m_oMatrixManager.GetMatrix(m_cMatrixName))
        {
          CString cMsg, cTitle;

          if (!cTitle.LoadString(IDS_GRIDEXISTSTITLE))
            cTitle = "Grid Exists";

          if (!cMsg.LoadString(IDS_GRIDEXISTSMESSAGE))
            cMsg = "A Grid With This Name Already Exists!\x0D\x0APlease ReEnter A Unique Name For This Grid.";

          ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_WIZGRIDINFO_GRIDNAME);
          pDX->Fail();
        }

      if ((m_cMatrixName.Find('.') > -1) || (m_cMatrixName.Find('\\') > -1) || (m_cMatrixName.Find('/') > -1))
        {
          CString cMsg, cTitle;

          if (!cTitle.LoadString(IDS_INVALIDGRIDNAMETITLE))
            cTitle = "Invalid Grid Name";

          if (!cMsg.LoadString(IDS_INVALIDGRIDNAMEMESSAGE))
            cMsg = "Invalid Charaters In Grid Name!\x0D\x0A Grid Names May NOT Contain '.', '\\' Or '/'.\x0D\x0A Please ReEnter The Name For This Grid.";

          ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_WIZGRIDINFO_GRIDNAME);
          pDX->Fail();
        }
    }
}


BEGIN_MESSAGE_MAP(GFCMatrixDetails, CPropertyPage)
	//{{AFX_MSG_MAP(GFCMatrixDetails)
	ON_EN_CHANGE(IDC_WIZGRIDINFO_GRIDNAME, OnChangeWizgridinfoGridname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GFCMatrixDetails message handlers

BOOL GFCMatrixDetails::OnSetActive()
{
  BOOL bRetval = CPropertyPage::OnSetActive();
  OnChangeWizgridinfoGridname();
  return bRetval;
}

BOOL GFCMatrixDetails::OnWizardFinish()
{
  UpdateData();

	return CPropertyPage::OnWizardFinish();
}

BOOL GFCMatrixDetails::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GFCMatrixDetails::OnChangeWizgridinfoGridname()
{
  CString cText;
  GetDlgItemText(IDC_WIZGRIDINFO_GRIDNAME,cText);

  if (!cText.IsEmpty())
    ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
  else
    ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_DISABLEDFINISH);
}
