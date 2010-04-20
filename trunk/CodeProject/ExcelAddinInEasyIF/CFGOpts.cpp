// CFGOpts.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "CFGOpts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConfigOptions dialog


ConfigOptions::ConfigOptions(MatrixManager *pMatrixMgr, CWnd* pParent /*=NULL*/)
	: CDialog(ConfigOptions::IDD, pParent)
{
  m_pMatrixMgr = pMatrixMgr;

	//{{AFX_DATA_INIT(ConfigOptions)
	m_cPathName = _T("");
	//}}AFX_DATA_INIT
  m_bRecalcOnChange = ((CEMXApp *)AfxGetApp())->IsRecalcOnChange();
  m_iRefreshIntervalIndex = (((CEMXApp *)AfxGetApp())->m_dwRecalcInterval / 500) - 1;

  m_bPathChanged = FALSE;
}


void ConfigOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConfigOptions)
	DDX_Text(pDX, IDC_CONFIG_PATHNAME, m_cPathName);
	DDX_Check(pDX, IDC_CONFIG_RECALCONCHANGE, m_bRecalcOnChange);
	DDX_CBIndex(pDX, IDC_CONFIG_REFRESHINTERVAL, m_iRefreshIntervalIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfigOptions, CDialog)
	//{{AFX_MSG_MAP(ConfigOptions)
	ON_BN_CLICKED(IDC_CONFIG_BROWSEPATH, OnConfigBrowsepath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ConfigOptions message handlers

BOOL ConfigOptions::OnInitDialog()
{
  m_cPathName = m_pMatrixMgr->m_cMatrixDirectory;

	CDialog::OnInitDialog();

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void ConfigOptions::OnOK()
{
  UpdateData();

  ((CEMXApp *)AfxGetApp())->SetRecalcOnChange(m_bRecalcOnChange);
  ((CEMXApp *)AfxGetApp())->m_dwRecalcInterval = (m_iRefreshIntervalIndex + 1) * 500;

  if (m_cPathName != m_pMatrixMgr->m_cMatrixDirectory)
    {
      m_pMatrixMgr->SetMatrixDirectory(m_cPathName);
      m_pMatrixMgr->LoadMatrices();

      m_bPathChanged = TRUE;
    }

  CDialog::OnOK();
}



void ConfigOptions::OnConfigBrowsepath()
{
  char caPathName[MAX_PATH];

  GetDlgItemText(IDC_CONFIG_PATHNAME,caPathName,MAX_PATH);

  CString cMsg;
  if (!cMsg.LoadString(IDS_SELECTREPOSITORYHEADING))
    cMsg = "Select the folder that contains the grid repository that you wish to work with.";

  BROWSEINFO sBI;
  memset(&sBI,0,sizeof(BROWSEINFO));

  sBI.hwndOwner = m_hWnd;
  sBI.pidlRoot = NULL;
  sBI.pszDisplayName = caPathName;
  sBI.lpszTitle = (LPCSTR)cMsg;
  sBI.ulFlags = BIF_RETURNONLYFSDIRS;
  sBI.lpfn = NULL;

  LPITEMIDLIST pidl = SHBrowseForFolder(&sBI);
  if (pidl)
    {
      SHGetPathFromIDList(pidl,caPathName);
      SetDlgItemText(IDC_CONFIG_PATHNAME,caPathName);
      GotoDlgCtrl(GetDlgItem(IDC_CONFIG_PATHNAME));
    }
}
