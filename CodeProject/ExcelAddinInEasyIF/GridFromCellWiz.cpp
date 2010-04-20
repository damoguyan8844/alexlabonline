// GridFromCellWiz.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GridFromCellWiz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GridFromCellWiz

IMPLEMENT_DYNAMIC(GridFromCellWiz, CPropertySheet)

GridFromCellWiz::GridFromCellWiz(CObArray *pFields, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  m_pFields = pFields;

  m_pGridInfo = new GFCMatrixDetails;
}

GridFromCellWiz::GridFromCellWiz(CObArray *pFields, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  m_pFields = pFields;

  m_pGridInfo = new GFCMatrixDetails;
}

GridFromCellWiz::~GridFromCellWiz()
{
  if (m_pGridInfo)
    delete m_pGridInfo;

  FLDProps *pDlg;
  for (int iLup = 0; iLup < m_oFldProps.GetSize(); iLup++)
    {
      pDlg = (FLDProps *)m_oFldProps.GetAt(iLup);
      if (pDlg)
        delete pDlg;
    }

  m_oFldProps.RemoveAll();
}


BEGIN_MESSAGE_MAP(GridFromCellWiz, CPropertySheet)
	//{{AFX_MSG_MAP(GridFromCellWiz)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GridFromCellWiz message handlers

int GridFromCellWiz::DoModal() 
{
  for (int iLup = 0; iLup < m_pFields->GetSize(); iLup++)
    {
      FLDProps *pDlg = new FLDProps(m_pFields,iLup);
      
      AddPage(pDlg);
      
      m_oFldProps.Add(pDlg);
    }
	
  AddPage(m_pGridInfo);

  m_psh.dwFlags &= ~PSH_HASHELP;
  m_psh.dwFlags |= PSH_NOAPPLYNOW;

  SetWizardMode();

  int iRetval = CPropertySheet::DoModal();

  if (iRetval == ID_WIZFINISH)
    iRetval = IDOK;

  return iRetval;
}



BOOL GridFromCellWiz::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
  CenterWindow();
	
	return bResult;
}
