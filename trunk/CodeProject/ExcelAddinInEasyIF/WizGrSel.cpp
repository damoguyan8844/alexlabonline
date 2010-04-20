// WizGrSel.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "WizGrSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WizGrSel property page

IMPLEMENT_DYNCREATE(WizGrSel, CPropertyPage)

WizGrSel::WizGrSel() : CPropertyPage(WizGrSel::IDD)
{
	//{{AFX_DATA_INIT(WizGrSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  m_pMatrix = NULL;
}

WizGrSel::~WizGrSel()
{
}

void WizGrSel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WizGrSel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MATRIX_TREE, m_oTreeCtrl);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      m_pMatrix = NULL;
      m_cMatrix.Empty();

      HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
      if (hItem)
        {
          m_pMatrix = (Matrix *)m_oTreeCtrl.GetItemData(hItem);
          if (m_pMatrix)
            m_cMatrix = m_pMatrix->GetMatrixName();
        }
    }
}


BEGIN_MESSAGE_MAP(WizGrSel, CPropertyPage)
	//{{AFX_MSG_MAP(WizGrSel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MATRIX_TREE, OnSelchangedMatrixTree)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WizGrSel message handlers


void WizGrSel::BuildTreeView()
{
  CWaitCursor oWait;

  CMapStringToOb *pMap = &(((CEMXApp *)AfxGetApp())->m_oMatrixManager.m_oMatrices);

  CString cDummy, cCategory;
  CObArray oCategoryList;
  Matrix *pMatrix;

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

          AddMatrixToCategoryList(cCategory,pMatrix,&oCategoryList);
        }
    }

  MatrixEntry *pEntry;

  int iCount = oCategoryList.GetSize();
  if (iCount > 0)
    {
      SortCategoryArray(&oCategoryList);

      for (int iLup = 0; iLup < iCount; iLup++)
        {
          pEntry = (MatrixEntry *)oCategoryList.GetAt(iLup);
          if (pEntry)
            SortMatrixArray(&pEntry->m_oMatrixList);
        }
    }

  // finally, add category items to tree
  HTREEITEM hSelectedItem = NULL;
  HTREEITEM hCurrParent;
  HTREEITEM hItem;

  for (int iItem = 0; iItem < iCount; iItem++)
    {
      pEntry = (MatrixEntry *)oCategoryList.GetAt(iItem);
      if (pEntry)
        {
          hCurrParent = m_oTreeCtrl.InsertItem(pEntry->m_cCategory,0,0);
          m_oTreeCtrl.SetItemData(hCurrParent,(DWORD)0);

          if (!hSelectedItem)
            hSelectedItem = hCurrParent;

          // now, add child items...
          int iMatrixCount = pEntry->m_oMatrixList.GetSize();
          for (int iLup = 0; iLup < iMatrixCount; iLup++)
            {
              pMatrix = (Matrix *)pEntry->m_oMatrixList.GetAt(iLup);
              if (pMatrix)
                {
                  hItem = m_oTreeCtrl.InsertItem(pMatrix->GetMatrixName(),1,1,hCurrParent);
                  m_oTreeCtrl.SetItemData(hItem,(DWORD)pMatrix);
                }
            }
        }
    }

  if (iCount > 0)
    {
      m_oTreeCtrl.EnsureVisible(hSelectedItem);
      m_oTreeCtrl.SelectItem(hSelectedItem);

      if (m_oTreeCtrl.ItemHasChildren(hSelectedItem))
        m_oTreeCtrl.Expand(hSelectedItem,TVE_EXPAND);
    }
}



void WizGrSel::AddMatrixToCategoryList(CString cCategory, Matrix *pMatrix, CObArray *pList)
{
  int iCount = pList->GetSize();

  MatrixEntry *pExistingEntry = NULL;
  MatrixEntry *pEntry = NULL;

  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pEntry = (MatrixEntry *)pList->GetAt(iLup);
      if (pEntry && pEntry->m_cCategory == cCategory)
        {
          pExistingEntry = pEntry;
          break;
        }
    }

  if (pExistingEntry == NULL)
    {
      pExistingEntry = new MatrixEntry;
      pExistingEntry->m_cCategory = cCategory;
      pList->Add((CObject *)pExistingEntry);
    }

  pExistingEntry->m_oMatrixList.Add((CObject *)pMatrix);
}



void WizGrSel::SortCategoryArray(CObArray *pList)
{
  int iCount = pList->GetSize();
  if (iCount < 2)
    return;

  BOOL bChanged = FALSE;
  int iLup;
  MatrixEntry *pFirst, *pSecond;

  do {
    bChanged = FALSE;
    for (iLup = 0; iLup < iCount; iLup++)
      {
        if (iLup == (iCount-1))
          break;

        pFirst = (MatrixEntry *)pList->GetAt(iLup);
        pSecond = (MatrixEntry *)pList->GetAt(iLup+1);
        if (pFirst->m_cCategory.Compare(pSecond->m_cCategory) > 0)
          {
            pList->SetAt(iLup,(CObject *)pSecond);
            pList->SetAt(iLup+1,(CObject *)pFirst);
            bChanged = TRUE;
          }
      }
  }
  while (bChanged);
}



void WizGrSel::SortMatrixArray(CObArray *pList)
{
  int iCount = pList->GetSize();
  if (iCount < 2)
    return;

  BOOL bChanged = FALSE;
  int iLup;
  Matrix *pFirst, *pSecond;

  do {
    bChanged = FALSE;
    for (iLup = 0; iLup < iCount; iLup++)
      {
        if (iLup == (iCount-1))
          break;

        pFirst = (Matrix *)pList->GetAt(iLup);
        pSecond = (Matrix *)pList->GetAt(iLup+1);
        if (strcmp(pFirst->GetMatrixName(),pSecond->GetMatrixName()) > 0)
          {
            pList->SetAt(iLup,(CObject *)pSecond);
            pList->SetAt(iLup+1,(CObject *)pFirst);
            bChanged = TRUE;
          }
      }
  }
  while (bChanged);
}



BOOL WizGrSel::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  m_oImgList.Create(16,16,ILC_COLOR16,2,1);
  CBitmap oBmp;
  oBmp.LoadBitmap(IDB_TREEICONS);
  m_oImgList.Add(&oBmp,RGB(0,255,0));
  m_oTreeCtrl.SetImageList(&m_oImgList,TVSIL_NORMAL);
  m_oTreeCtrl.SetImageList(&m_oImgList,TVSIL_STATE);

  BuildTreeView();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void WizGrSel::OnSelchangedMatrixTree(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      Matrix *pMatrix = (Matrix *)m_oTreeCtrl.GetItemData(hItem);
      BOOL bEnable = pMatrix ? TRUE : FALSE;

      ((CPropertySheet *)GetParent())->SetWizardButtons(bEnable ? PSWIZB_NEXT : 0);
    }

  *pResult = 0;
}




BOOL WizGrSel::OnSetActive()
{
  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      Matrix *pMatrix = (Matrix *)m_oTreeCtrl.GetItemData(hItem);
      BOOL bEnable = pMatrix ? TRUE : FALSE;

      ((CPropertySheet *)GetParent())->SetWizardButtons(bEnable ? PSWIZB_NEXT : 0);
    }

	return CPropertyPage::OnSetActive();
}
