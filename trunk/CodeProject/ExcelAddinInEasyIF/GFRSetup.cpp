// GFRSetup.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GFRSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GFRSetup dialog


GFRSetup::GFRSetup(CRect *pSelRange, CWnd* pParent /*=NULL*/)
	: CDialog(GFRSetup::IDD, pParent)
{
  m_oSelRange = *pSelRange;

	//{{AFX_DATA_INIT(GFRSetup)
  m_bFirstRowColHeadings = TRUE;
	m_cMatrixName = _T("");
	//}}AFX_DATA_INIT
}


void GFRSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GFRSetup)
	DDX_Check(pDX, IDC_GWFR_FIRSTROWCOLHEADERS, m_bFirstRowColHeadings);
	DDX_Text(pDX, IDC_GWFR_MATRIXNAME, m_cMatrixName);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      if (((CEMXApp *)AfxGetApp())->m_oMatrixManager.GetMatrix(m_cMatrixName))
        {
          CString cMsg, cTitle;

          if (!cTitle.LoadString(IDS_GRIDEXISTSTITLE))
            cTitle = "Grid Exists";

          if (!cMsg.LoadString(IDS_GRIDEXISTSMESSAGE))
            cMsg = "A Grid With This Name Already Exists!\x0D\x0APlease ReEnter A Unique Name For This Grid.";

          ::MessageBox(m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_GWFR_MATRIXNAME);
          pDX->Fail();
        }

      if ((m_cMatrixName.Find('.') > -1) || (m_cMatrixName.Find('\\') > -1) || (m_cMatrixName.Find('/') > -1))
        {
          CString cMsg, cTitle;

          if (!cTitle.LoadString(IDS_INVALIDGRIDNAMETITLE))
            cTitle = "Invalid Grid Name";

          if (!cMsg.LoadString(IDS_INVALIDGRIDNAMEMESSAGE))
            cMsg = "Invalid Charaters In Grid Name!\x0D\x0A Grid Names May NOT Contain '.', '\\' Or '/'.\x0D\x0A Please ReEnter The Name For This Grid.";

          ::MessageBox(m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle, MB_ICONEXCLAMATION | MB_OK);
          pDX->PrepareEditCtrl(IDC_GWFR_MATRIXNAME);
          pDX->Fail();
        }
    }
}


BEGIN_MESSAGE_MAP(GFRSetup, CDialog)
	//{{AFX_MSG_MAP(GFRSetup)
	ON_EN_CHANGE(IDC_GWFR_MATRIXNAME, OnChangeGwfrMatrixname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GFRSetup message handlers

BOOL GFRSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

  CString cTitle;
  GetWindowText(cTitle);

  char caBuf[256];
  char caRangeBuf[41];

  RowColToA1(m_oSelRange.top,m_oSelRange.left,caBuf);
  strcpy(caRangeBuf,caBuf);
  strcat(caRangeBuf,":");
  RowColToA1(m_oSelRange.bottom,m_oSelRange.right,caBuf);
  strcat(caRangeBuf,caBuf);

  sprintf(caBuf,(LPCSTR)cTitle,caRangeBuf);
  SetWindowText(caBuf);

  GetDlgItemText(IDC_GWFR_FIRSTROWCOLHEADERS,cTitle);
  RowColToA1(m_oSelRange.top,m_oSelRange.left,caBuf);
  strcpy(caRangeBuf,caBuf);
  strcat(caRangeBuf,":");
  RowColToA1(m_oSelRange.top,m_oSelRange.right,caBuf);
  strcat(caRangeBuf,caBuf);

  sprintf(caBuf,(LPCSTR)cTitle,caRangeBuf);
  SetDlgItemText(IDC_GWFR_FIRSTROWCOLHEADERS,caBuf);

  OnChangeGwfrMatrixname();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void GFRSetup::RowColToA1(int iRow, int iCol, char *cpBuffer)
{
  if (iCol > 25)
    {
      int iUpper = (iCol / 26) - 1;
      int iLower = iCol - ((iUpper+1) * 26);
      sprintf(cpBuffer,"%c%c%d",(char)('A'+iUpper),(char)('A' + iLower),iRow+1);
    }
  else
    sprintf(cpBuffer,"%c%d",(char)('A'+iCol),iRow+1);
}



void GFRSetup::OnChangeGwfrMatrixname() 
{
  CString cText;
  GetDlgItemText(IDC_GWFR_MATRIXNAME,cText);
  
  GetDlgItem(IDOK)->EnableWindow(!cText.IsEmpty());
}
