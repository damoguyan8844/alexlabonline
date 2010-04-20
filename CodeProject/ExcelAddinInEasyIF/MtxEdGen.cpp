// MtxEdGen.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "MtxEdGen.h"
#include "mtrxsel.h"
#include "mtxedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MtxEditGen property page

IMPLEMENT_DYNCREATE(MtxEditGen, CPropertyPage)

MtxEditGen::MtxEditGen()
  : CPropertyPage(MtxEditGen::IDD)
{
  m_pMatrix = NULL;
}

MtxEditGen::MtxEditGen(Matrix *pMatrix)
  : CPropertyPage(MtxEditGen::IDD)
{
  m_pMatrix = pMatrix;

	//{{AFX_DATA_INIT(MtxEditGen)
	m_cCreator = m_pMatrix->GetCreator();
	m_cCategory = m_pMatrix->GetCategory();
	m_cPathName = m_pMatrix->GetFileName();
	m_cMatrixName = m_pMatrix->GetMatrixName();
	m_cNotes = m_pMatrix->GetMatrixNotes();
	m_cPswAccess = m_pMatrix->GetAccessPassword();
	m_cPswModify = m_pMatrix->GetModifyPassword();
	m_bReqPswAccess = m_pMatrix->RequiresAccessPassword();
	m_bReqPswModify = m_pMatrix->RequiresModifyPassword();
	//}}AFX_DATA_INIT
}

MtxEditGen::~MtxEditGen()
{
}

void MtxEditGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MtxEditGen)
	DDX_Text(pDX, IDC_GEN_AUTHOR, m_cCreator);
	DDX_CBString(pDX, IDC_GEN_CATEGORY, m_cCategory);
	DDX_Text(pDX, IDC_GEN_FILEPATH, m_cPathName);
	DDX_Text(pDX, IDC_GEN_NAME, m_cMatrixName);
	DDX_Text(pDX, IDC_GEN_NOTES, m_cNotes);
	DDX_Text(pDX, IDC_GEN_PSWACCESS, m_cPswAccess);
	DDX_Text(pDX, IDC_GEN_PSWMODIFY, m_cPswModify);
	DDX_Check(pDX, IDC_GEN_REQPSWACCESS, m_bReqPswAccess);
	DDX_Check(pDX, IDC_GEN_REQPSWMODIFY, m_bReqPswModify);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      if (m_cCreator.Compare(m_pMatrix->GetCreator()))
        m_pMatrix->SetCreator(m_cCreator);

      if (m_cCategory.Compare(m_pMatrix->GetCategory()))
        m_pMatrix->SetCategory(m_cCategory);

      // CHECK UNIQUENESS OF FILENAME AND MATRIX NAME!!!!!
      if (m_pMatrix->m_bNewMatrix)
        {
          // MatrixSel *pSelDlg = (MatrixSel *)GetParent()->GetParent();

          if (((CEMXApp *)AfxGetApp())->m_oMatrixManager.GetMatrix(m_cMatrixName))
            {
              CString cMsg, cTitle;

              if (!cTitle.LoadString(IDS_GRIDEXISTSTITLE))
                cTitle = "Grid Exists";

              if (!cMsg.LoadString(IDS_GRIDEXISTSMESSAGE))
                cMsg = "A Grid With This Name Already Exists!\x0D\x0APlease ReEnter A Unique Name For This Grid.";

              ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
              pDX->PrepareEditCtrl(IDC_GEN_NAME);
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
              pDX->PrepareEditCtrl(IDC_GEN_NAME);
              pDX->Fail();
            }

          if (m_cMatrixName.Compare(m_pMatrix->GetMatrixName()))
            m_pMatrix->SetMatrixName(m_cMatrixName);

          if (m_cPathName.IsEmpty())
            m_cPathName = m_cMatrixName + CString(".xlg");

          if (m_cPathName.Compare(m_pMatrix->GetFileName()))
            m_pMatrix->SetFileName(m_cPathName);
	    }

      if (m_cNotes.Compare(m_pMatrix->GetMatrixNotes()))
        m_pMatrix->SetMatrixNotes(m_cNotes);

      m_cPswAccess.TrimRight();
      m_cPswModify.TrimRight();

      if (m_bReqPswAccess && m_cPswAccess.IsEmpty())
        {
          CString cTitle, cMessage;

          if (!cTitle.LoadString(IDS_PASSWORDERRHEADING))
            cTitle = "Invalid Password";

          if (!cMessage.LoadString(IDS_PASSWORDNOTBLANKMSG))
            cMessage = "Passwords May Not Be Blank.";

          ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMessage,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_GEN_PSWACCESS);
          pDX->Fail();
        }

      if (m_bReqPswModify && m_cPswModify.IsEmpty())
        {
          CString cTitle, cMessage;

          if (!cTitle.LoadString(IDS_PASSWORDERRHEADING))
            cTitle = "Invalid Password";

          if (!cMessage.LoadString(IDS_PASSWORDNOTBLANKMSG))
            cMessage = "Passwords May Not Be Blank.";

          ::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMessage,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_GEN_PSWMODIFY);
          pDX->Fail();
        }

      if (m_cPswAccess.Compare(m_pMatrix->GetAccessPassword()))
        m_pMatrix->SetAccessPassword(m_cPswAccess);

      if (m_cPswModify.Compare(m_pMatrix->GetModifyPassword()))
        m_pMatrix->SetModifyPassword(m_cPswModify);

      if (m_bReqPswAccess != m_pMatrix->RequiresAccessPassword())
        m_pMatrix->SetRequiresAccessPassword(m_bReqPswAccess);

      if (m_bReqPswModify != m_pMatrix->RequiresModifyPassword())
        m_pMatrix->SetRequiresModifyPassword(m_bReqPswModify);
    }
}


BEGIN_MESSAGE_MAP(MtxEditGen, CPropertyPage)
	//{{AFX_MSG_MAP(MtxEditGen)
	ON_BN_CLICKED(IDC_GEN_REQPSWACCESS, OnGenReqpswaccess)
	ON_BN_CLICKED(IDC_GEN_REQPSWMODIFY, OnGenReqpswmodify)
	ON_EN_CHANGE(IDC_GEN_NAME, OnChangeGenName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MtxEditGen message handlers

void MtxEditGen::OnGenReqpswaccess()
{
  BOOL bChecked = SendDlgItemMessage(IDC_GEN_REQPSWACCESS,BM_GETCHECK);
  GetDlgItem(IDC_GEN_PSWACCESS_LBL)->EnableWindow(bChecked);
  GetDlgItem(IDC_GEN_PSWACCESS)->EnableWindow(bChecked);
}



void MtxEditGen::OnGenReqpswmodify()
{
  BOOL bChecked = SendDlgItemMessage(IDC_GEN_REQPSWMODIFY,BM_GETCHECK);
  GetDlgItem(IDC_GEN_PSWMODIFY_LBL)->EnableWindow(bChecked);
  GetDlgItem(IDC_GEN_PSWMODIFY)->EnableWindow(bChecked);
}



BOOL MtxEditGen::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnKillActive();
}



BOOL MtxEditGen::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  // MatrixSel *pSelDlg = (MatrixSel *)GetParent()->GetParent();

  CMapStringToOb *pMap= &(((CEMXApp *)AfxGetApp())->m_oMatrixManager.m_oMatrices);

  CString cDummy, cCategory;
  CMapStringToPtr oCategoryList;
  Matrix *pMatrix;
  void *vpDummy;

  POSITION pPos = pMap->GetStartPosition();
  while (pPos)
    {
      pMap->GetNextAssoc(pPos,cDummy,(CObject *&)pMatrix);
      if (pMatrix)
        {
          cCategory = pMatrix->GetCategory();

          cCategory.TrimRight();
          if (cCategory.IsEmpty() || cCategory == "")
            {
              if (!cCategory.LoadString(IDS_UNCATEGORIZED))
                cCategory = "(uncategorized)";
            }

          if (!oCategoryList.Lookup(cCategory,vpDummy))
            oCategoryList.SetAt(cCategory,NULL);
        }
    }

  CComboBox *pCB = (CComboBox *)GetDlgItem(IDC_GEN_CATEGORY);
  pPos = oCategoryList.GetStartPosition();
  while (pPos)
    {
      oCategoryList.GetNextAssoc(pPos,cCategory,vpDummy);
      pCB->AddString(cCategory);
    }

  GetDlgItem(IDC_GEN_PSWACCESS_LBL)->EnableWindow(m_bReqPswAccess);
  GetDlgItem(IDC_GEN_PSWACCESS)->EnableWindow(m_bReqPswAccess);
  GetDlgItem(IDC_GEN_PSWMODIFY_LBL)->EnableWindow(m_bReqPswModify);
  GetDlgItem(IDC_GEN_PSWMODIFY)->EnableWindow(m_bReqPswModify);

  if (!m_pMatrix->m_bNewMatrix)
    GetDlgItem(IDC_GEN_NAME)->EnableWindow(FALSE);

  MatrixEdit *pParent = (MatrixEdit *)GetParent();
  if (pParent)
    pParent->EnableOKButton();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void MtxEditGen::OnChangeGenName()
{
  if (m_pMatrix->m_bNewMatrix)
    {
      CString cName;
      GetDlgItemText(IDC_GEN_NAME,cName);
      cName = cName + CString(".xlg");
      SetDlgItemText(IDC_GEN_FILEPATH,cName);
    }
}

void MtxEditGen::OnOK()
{
  CPropertyPage::OnOK();
}
