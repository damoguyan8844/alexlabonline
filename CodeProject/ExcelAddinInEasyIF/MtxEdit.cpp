// MtxEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "MtxEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MatrixEdit

IMPLEMENT_DYNAMIC(MatrixEdit, CPropertySheet)

MatrixEdit::MatrixEdit(Matrix *pMatrix, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  m_pMatrix = pMatrix;

  m_pGeneralTab = new MtxEditGen(m_pMatrix);
  m_pColumnTab =  new MatrixEditCol(m_pMatrix);
  m_pDataTab =    new MatrixEditData(m_pMatrix);
}

MatrixEdit::MatrixEdit(Matrix *pMatrix, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  m_pMatrix = pMatrix;

  m_pGeneralTab = new MtxEditGen(m_pMatrix);
  m_pColumnTab =  new MatrixEditCol(m_pMatrix);
  m_pDataTab =    new MatrixEditData(m_pMatrix);
}

MatrixEdit::~MatrixEdit()
{
  if (m_pGeneralTab)
    delete m_pGeneralTab;

  if (m_pColumnTab)
    delete m_pColumnTab;

  if (m_pDataTab)
    delete m_pDataTab;
}


BEGIN_MESSAGE_MAP(MatrixEdit, CPropertySheet)
	//{{AFX_MSG_MAP(MatrixEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MatrixEdit message handlers

int MatrixEdit::DoModal()
{
  AddPage(m_pGeneralTab);
  AddPage(m_pColumnTab);
  AddPage(m_pDataTab);

  m_psh.dwFlags &= ~PSH_HASHELP;
  m_psh.dwFlags |= PSH_NOAPPLYNOW;

  int iResult = CPropertySheet::DoModal();

  if (iResult == IDOK && m_pMatrix->GetRowCount() < 1)
    {
/// INTERNATIONALIZE
      ::MessageBox(NULL,"There Are No Data Rows Defined For This Grid!\x0D\x0A You Must Define Data Rows Before Using This Grid.","No Data Rows",MB_ICONINFORMATION|MB_OK|MB_TASKMODAL);
    }

  return iResult;
}


BOOL MatrixEdit::CheckColumnsDefined()
{
  int iCount = m_pMatrix->GetColumnCount();

  if (iCount < 1)
    {
/// INTERNATIONALIZE
      ::MessageBox(m_hWnd,"You Must Define At Least One Column\x0D\x0A In Order For The Grid To Function.","No Columns Defined",MB_ICONEXCLAMATION|MB_OK);
    }

  return (iCount > 0);
}



void MatrixEdit::EnableOKButton()
{
  int iCount = m_pMatrix->GetColumnCount();

  GetDlgItem(1)->EnableWindow((iCount > 0));
}
