// GridWizChooser.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GridWizChooser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GridWizChooser dialog


GridWizChooser::GridWizChooser(CWnd* pParent /*=NULL*/)
	: CDialog(GridWizChooser::IDD, pParent)
{
	//{{AFX_DATA_INIT(GridWizChooser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  m_iWizardSelection = 0;
}


void GridWizChooser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GridWizChooser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GridWizChooser, CDialog)
	//{{AFX_MSG_MAP(GridWizChooser)
	ON_BN_CLICKED(IDC_GRIDWIZ_CGCOLUMN, OnGridwizCgcolumn)
	ON_BN_CLICKED(IDC_GRIDWIZ_CGIMPORT, OnGridwizCgimport)
	ON_BN_CLICKED(IDC_GRIDWIZ_INSGRID, OnGridwizInsgrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GridWizChooser message handlers

BOOL GridWizChooser::OnInitDialog() 
{
  m_oBitmaps[0].LoadBitmap(IDB_GRIDWIZ_CREATEGRIDFORCOLUMN);
  m_oBitmaps[1].LoadBitmap(IDB_GRIDWIZ_INSERTGRIDINTOSPREADSHEET);
  m_oBitmaps[2].LoadBitmap(IDB_GRIDWIZ_CREATEGRIDFROMSPREADSHEET);

  CDialog::OnInitDialog();

  SendDlgItemMessage(IDC_GRIDWIZ_CGCOLUMN,BM_SETCHECK,(m_iWizardSelection == 0));

  SendDlgItemMessage(IDC_GRIDWIZ_CGIMPORT,BM_SETCHECK,(m_iWizardSelection == 1));
  SendDlgItemMessage(IDC_GRIDWIZ_INSGRID,BM_SETCHECK,(m_iWizardSelection == 2));

  ((CStatic *)GetDlgItem(IDC_GRIDWIZ_INDICATOR))->SetBitmap(m_oBitmaps[m_iWizardSelection]);
	
  CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GridWizChooser::OnOK() 
{
  if (SendDlgItemMessage(IDC_GRIDWIZ_CGCOLUMN,BM_GETCHECK))
    m_iWizardSelection = 0;

  if (SendDlgItemMessage(IDC_GRIDWIZ_INSGRID,BM_GETCHECK))
    m_iWizardSelection = 1;

  if (SendDlgItemMessage(IDC_GRIDWIZ_CGIMPORT,BM_GETCHECK))
    m_iWizardSelection = 2;
    
	CDialog::OnOK();
}



void GridWizChooser::OnGridwizCgcolumn() 
{
  ((CStatic *)GetDlgItem(IDC_GRIDWIZ_INDICATOR))->SetBitmap(m_oBitmaps[0]);
}

void GridWizChooser::OnGridwizCgimport() 
{
  ((CStatic *)GetDlgItem(IDC_GRIDWIZ_INDICATOR))->SetBitmap(m_oBitmaps[2]);
}

void GridWizChooser::OnGridwizInsgrid() 
{
  ((CStatic *)GetDlgItem(IDC_GRIDWIZ_INDICATOR))->SetBitmap(m_oBitmaps[1]);
}
