// GridWiz.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GridWiz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GridWiz

IMPLEMENT_DYNAMIC(GridWiz, CPropertySheet)

GridWiz::GridWiz(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  m_pSelect = new WizGrSel;
  m_pInsertPos = new WizGrPos;
  m_pRange = new WizGrRng;
}

GridWiz::GridWiz(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  m_pSelect = new WizGrSel;
  m_pInsertPos = new WizGrPos;
  m_pRange = new WizGrRng;
}

GridWiz::~GridWiz()
{
  if (m_pSelect)
    delete m_pSelect;

  if (m_pInsertPos)
    delete m_pInsertPos;

  if (m_pRange)
    delete m_pRange;
}


BEGIN_MESSAGE_MAP(GridWiz, CPropertySheet)
	//{{AFX_MSG_MAP(GridWiz)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GridWiz message handlers

int GridWiz::DoModal()
{
  AddPage(m_pSelect);
  AddPage(m_pInsertPos);
  AddPage(m_pRange);

  m_psh.dwFlags &= ~PSH_HASHELP;
  m_psh.dwFlags |= PSH_NOAPPLYNOW;

  SetWizardMode();

  int iRetval = CPropertySheet::DoModal();

  if (iRetval == ID_WIZFINISH)
    iRetval = IDOK;

  return iRetval;
}



BOOL GridWiz::OnInitDialog()
{
  BOOL bResult = CPropertySheet::OnInitDialog();

  CenterWindow();

  return bResult;
}
