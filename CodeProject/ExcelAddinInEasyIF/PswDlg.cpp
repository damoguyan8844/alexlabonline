// PswDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "PswDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PasswordEntryDlg dialog


PasswordEntryDlg::PasswordEntryDlg(LPCSTR cpPassword, CWnd* pParent /*=NULL*/)
	: CDialog(PasswordEntryDlg::IDD, pParent)
{
  m_cPassword = cpPassword;

	//{{AFX_DATA_INIT(PasswordEntryDlg)
	m_cPasswordEntered = _T("");
	//}}AFX_DATA_INIT
}


void PasswordEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PasswordEntryDlg)
	DDX_Text(pDX, IDC_PSW_PASSWORD, m_cPasswordEntered);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PasswordEntryDlg, CDialog)
	//{{AFX_MSG_MAP(PasswordEntryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PasswordEntryDlg message handlers

BOOL PasswordEntryDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CenterWindow();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void PasswordEntryDlg::OnOK()
{
  UpdateData();

  if (m_cPassword == m_cPasswordEntered)
    CDialog::OnOK();
  else
    {
      CString cPasswordErrHeading, cPasswordErrMessage;

      if (!cPasswordErrHeading.LoadString(IDS_PASSWORDERRHEADING))
        cPasswordErrHeading = "Password Error";

      if (!cPasswordErrMessage.LoadString(IDS_PASSWORDERRMESSAGE))
        cPasswordErrMessage = "Invalid Password\x0D\x0A Please Try Again.";

      ::MessageBox(m_hWnd,(LPCSTR)cPasswordErrMessage,(LPCSTR)cPasswordErrHeading,MB_ICONHAND|MB_OK);
    }
}
