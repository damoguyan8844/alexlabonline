// WizGrPos.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "WizGrPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WizGrPos property page

IMPLEMENT_DYNCREATE(WizGrPos, CPropertyPage)

WizGrPos::WizGrPos() : CPropertyPage(WizGrPos::IDD)
{
	//{{AFX_DATA_INIT(WizGrPos)
  m_cInsertPos = _T("A1");
	//}}AFX_DATA_INIT
}

WizGrPos::~WizGrPos()
{
}

void WizGrPos::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WizGrPos)
	DDX_Text(pDX, IDC_INSPOS_CELL, m_cInsertPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WizGrPos, CPropertyPage)
	//{{AFX_MSG_MAP(WizGrPos)
	ON_EN_CHANGE(IDC_INSPOS_CELL, OnChangeInsposCell)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WizGrPos message handlers

void WizGrPos::OnChangeInsposCell()
{
  UpdateData();

  if (m_cInsertPos.IsEmpty())
    ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK);
  else
    ((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
}


BOOL WizGrPos::OnSetActive()
{
  OnChangeInsposCell();

	return CPropertyPage::OnSetActive();
}
