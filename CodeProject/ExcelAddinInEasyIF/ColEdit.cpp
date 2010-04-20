// ColEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "ColEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColumnEdit

IMPLEMENT_DYNAMIC(ColumnEdit, CPropertySheet)

ColumnEdit::ColumnEdit(Matrix *pMatrix, int iColIndex, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  m_pMatrix = pMatrix;
  m_iColIndex = iColIndex;

  m_pGeneralTab = new ColGeneral(pMatrix,iColIndex);

  if (iColIndex < 0)
    m_pPositionTab = new ColPosition(pMatrix);
  else
    m_pPositionTab = NULL;

  m_pPickListTab = new ColPickList(pMatrix,iColIndex);
}

ColumnEdit::ColumnEdit(Matrix *pMatrix, int iColIndex, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  m_pMatrix = pMatrix;
  m_iColIndex = iColIndex;

  m_pGeneralTab = new ColGeneral(pMatrix,iColIndex);

  if (iColIndex < 0)
    m_pPositionTab = new ColPosition(pMatrix);
  else
    m_pPositionTab = NULL;

  m_pPickListTab = new ColPickList(pMatrix,iColIndex);
}



ColumnEdit::~ColumnEdit()
{
  if (m_pGeneralTab)
    delete m_pGeneralTab;

  if (m_pPositionTab)
    delete m_pPositionTab;

  if (m_pPickListTab)
    delete m_pPickListTab;
}


BEGIN_MESSAGE_MAP(ColumnEdit, CPropertySheet)
	//{{AFX_MSG_MAP(ColumnEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColumnEdit message handlers

int ColumnEdit::DoModal() 
{
  AddPage(m_pGeneralTab);
  
  if (m_iColIndex < 0)
    AddPage(m_pPositionTab);

  AddPage(m_pPickListTab);

  m_psh.dwFlags &= ~PSH_HASHELP;
  m_psh.dwFlags |= PSH_NOAPPLYNOW;

  int iResult = CPropertySheet::DoModal();

  return iResult;
}
