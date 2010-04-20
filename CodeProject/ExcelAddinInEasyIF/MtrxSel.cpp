// MtrxSel.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "MtrxSel.h"
#include "mtxedit.h"
#include "pswdlg.h"
#include "xlcall.h"
#include "framewrk.h"
#include "dupmtx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MatrixSel dialog


MatrixSel::MatrixSel(MatrixManager *pMatrixMgr, CWnd* pParent /*=NULL*/)
	: CDialog(MatrixSel::IDD, pParent)
{
  m_pMatrixMgr = pMatrixMgr;

  m_bMatricesChanged = FALSE;

	//{{AFX_DATA_INIT(MatrixSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



MatrixSel::~MatrixSel()
{
  FreeMemoryStructs();
}


void MatrixSel::FreeMemoryStructs()
{
  MTXTREEITEMSTRUCT *spTI;

  int iCount = m_oItemsToFreeList.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      spTI = (MTXTREEITEMSTRUCT *)m_oItemsToFreeList.GetAt(iLup);
      if (spTI)
        {
          if (!spTI->m_bLeafItem)
            {
              MatrixEntry *pME = (MatrixEntry *)spTI->m_vpData;
              if (pME)
                delete pME;
            }

          delete spTI;
        }
    }

  m_oItemsToFreeList.RemoveAll();
}



void MatrixSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MatrixSel)
	DDX_Control(pDX, IDC_MATRIX_TREE, m_oTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MatrixSel, CDialog)
	//{{AFX_MSG_MAP(MatrixSel)
	ON_BN_CLICKED(IDC_MATRIX_ADD, OnMatrixAdd)
	ON_BN_CLICKED(IDC_MATRIX_PROPERTIES, OnMatrixProperties)
	ON_BN_CLICKED(IDC_MATRIX_REMOVE, OnMatrixRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_MATRIX_TREE, OnDblclkMatrixTree)
	ON_NOTIFY(NM_RCLICK, IDC_MATRIX_TREE, OnRclickMatrixTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MATRIX_TREE, OnSelchangedMatrixTree)
	ON_BN_CLICKED(IDC_MATRIX_COPY, OnMatrixCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MatrixSel message handlers

void MatrixSel::OnMatrixAdd()
{
  CString cDefaultCategory;
  if (!cDefaultCategory.LoadString(IDS_DEFCATEGORY))
    cDefaultCategory = "General";

  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      MTXTREEITEMSTRUCT *spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hItem);
      if (spTI)
        {
          if (!spTI->m_bLeafItem)
            cDefaultCategory = m_oTreeCtrl.GetItemText(hItem);
          else
            cDefaultCategory = ((Matrix *)(spTI->m_vpData))->GetCategory();
        }
    }

  Matrix *pMatrix = new Matrix;
  pMatrix->SetCategory(cDefaultCategory);

  MatrixEdit oEdit(pMatrix,IDS_ADDNEWGRIDTITLE,this);
  // MatrixEdit oEdit(pMatrix,"EasyIF - Add New Grid",this);

  int iResult = oEdit.DoModal();

  if (iResult == IDOK)
    {
      // add and save matrix...
      if (m_pMatrixMgr->SaveMatrix(pMatrix))
        {
          m_pMatrixMgr->m_oMatrices.SetAt(pMatrix->GetMatrixName(),(CObject *)pMatrix);
          ((CEMXApp *)AfxGetApp())->RefreshFloatingGrids();
          BuildTreeView();
        }
    }
  else
    {
      delete pMatrix;
    }
}



void MatrixSel::OnMatrixProperties()
{
  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      MTXTREEITEMSTRUCT *spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hItem);
      if (spTI && spTI->m_bLeafItem)
        {
          Matrix *pMatrix = (Matrix *)spTI->m_vpData;

          if (!CheckModifyRights(pMatrix))
            return;

          CString cOrigCategory = pMatrix->GetCategory();

          // edit pMatrix here...
          MatrixEdit oEdit(pMatrix,IDS_EDITGRIDTITLE,this);
//          MatrixEdit oEdit(pMatrix,"EasyIF - Edit Grid Properties",this);

          int iResult = oEdit.DoModal();

          if (iResult == IDOK)
            {
              if (m_pMatrixMgr->SaveMatrix(pMatrix))
                {
//                  Excel(xlcCalculateNow,0,0);
                  m_bMatricesChanged = TRUE;

                  ((CEMXApp *)AfxGetApp())->RefreshFloatingGrids();

                  if (cOrigCategory != pMatrix->GetCategory())
                    BuildTreeView();
                }
            }
          else
            {
              CString cMatrixName = pMatrix->GetMatrixName();
              CString cFileName = pMatrix->GetFileName();
              m_pMatrixMgr->LoadMatrix(cFileName);
              spTI->m_vpData = m_pMatrixMgr->GetMatrix(cMatrixName);
              delete pMatrix;
            }
        }
    }
}



void MatrixSel::OnMatrixRemove()
{
  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      MTXTREEITEMSTRUCT *spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hItem);
      if (spTI && spTI->m_bLeafItem)
        {
          Matrix *pMatrix = (Matrix *)spTI->m_vpData;
          char caBuf[512];

          if (!CheckModifyRights(pMatrix))
            return;

          CString cFmt, cTitle;
          if (!cTitle.LoadString(IDS_CONFIRMDELETETITLE))
            cTitle = "Confirm Delete";

          if (!cFmt.LoadString(IDS_DELETEGRIDPROMPT))
            cFmt = "Permanantly Remove '%s'.\x0D\x0A Are You Sure?";

          sprintf(caBuf,(LPCSTR)cFmt,pMatrix->GetMatrixName());

          if (::MessageBox(m_hWnd,caBuf,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNO) == IDYES)
            {
              m_pMatrixMgr->RemoveMatrix(pMatrix);
              spTI->m_vpData = NULL;
              m_bMatricesChanged = TRUE;
              ((CEMXApp *)AfxGetApp())->RefreshFloatingGrids();
              BuildTreeView();
            }
        }
    }
}



void MatrixSel::BuildTreeView()
{
  CWaitCursor oWait;

  CString cSelCategory;
  CString cSelMatrixName;
  MTXTREEITEMSTRUCT *spTI;
  HTREEITEM hOldSelItem = m_oTreeCtrl.GetSelectedItem();
  if (hOldSelItem)
    {
      spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hOldSelItem);
      if (spTI)
        {
          if (spTI->m_bLeafItem)
            {
              if (spTI->m_vpData)
                {
                  cSelCategory = ((Matrix *)spTI->m_vpData)->GetCategory();
                  cSelMatrixName = ((Matrix *)spTI->m_vpData)->GetMatrixName();
                }
            }
          else
            cSelCategory = m_oTreeCtrl.GetItemText(hOldSelItem);
        }
    }

  m_oTreeCtrl.DeleteAllItems();
  FreeMemoryStructs();

  CMapStringToOb *pMap= &(m_pMatrixMgr->m_oMatrices);

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
          spTI = new MTXTREEITEMSTRUCT;
          spTI->m_bLeafItem = FALSE;
          spTI->m_vpData = pEntry;
          m_oTreeCtrl.SetItemData(hCurrParent,(DWORD)spTI);
          m_oItemsToFreeList.Add((CObject *)spTI);

          if (!hSelectedItem)
            hSelectedItem = hCurrParent;

          if (cSelCategory == pEntry->m_cCategory)
            hSelectedItem = hCurrParent;

          // now, add child items...
          int iMatrixCount = pEntry->m_oMatrixList.GetSize();
          for (int iLup = 0; iLup < iMatrixCount; iLup++)
            {
              pMatrix = (Matrix *)pEntry->m_oMatrixList.GetAt(iLup);
              if (pMatrix)
                {
                  hItem = m_oTreeCtrl.InsertItem(pMatrix->GetMatrixName(),1,1,hCurrParent);
                  spTI = new MTXTREEITEMSTRUCT;
                  spTI->m_bLeafItem = TRUE;
                  spTI->m_vpData = pMatrix;
                  m_oTreeCtrl.SetItemData(hItem,(DWORD)spTI);
                  m_oItemsToFreeList.Add((CObject *)spTI);

                  if (cSelCategory == pEntry->m_cCategory && cSelMatrixName == pMatrix->GetMatrixName())
                    hSelectedItem = hItem;
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



void MatrixSel::AddMatrixToCategoryList(CString cCategory, Matrix *pMatrix, CObArray *pList)
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



void MatrixSel::SortCategoryArray(CObArray *pList)
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



void MatrixSel::SortMatrixArray(CObArray *pList)
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



BOOL MatrixSel::OnInitDialog()
{
  CDialog::OnInitDialog();

//  oImgList.Create(IDB_TREEICONS,16,1,RGB(0,0,0));
  m_oImgList.Create(16,16,ILC_COLOR16,2,1);
  CBitmap oBmp;
  oBmp.LoadBitmap(IDB_TREEICONS);
  m_oImgList.Add(&oBmp,RGB(0,255,0));
  m_oTreeCtrl.SetImageList(&m_oImgList,TVSIL_NORMAL);
  m_oTreeCtrl.SetImageList(&m_oImgList,TVSIL_STATE);

  BuildTreeView();

  CenterWindow();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void MatrixSel::OnDblclkMatrixTree(NMHDR* pNMHDR, LRESULT* pResult)
{
  OnMatrixProperties();

  *pResult = 0;
}



void MatrixSel::OnRclickMatrixTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}



void MatrixSel::OnSelchangedMatrixTree(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      MTXTREEITEMSTRUCT *spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hItem);
      BOOL bEnable = (spTI && spTI->m_bLeafItem);

      GetDlgItem(IDC_MATRIX_PROPERTIES)->EnableWindow(bEnable);
      GetDlgItem(IDC_MATRIX_REMOVE)->EnableWindow(bEnable);
      GetDlgItem(IDC_MATRIX_COPY)->EnableWindow(bEnable);

      CString cNotes;
      if (bEnable)
        {
          Matrix *pMatrix = (Matrix *)spTI->m_vpData;
          if (pMatrix)
            {
              cNotes = pMatrix->m_cNotes;
              cNotes.TrimRight();

/// INTERNATIONALIZE
              cNotes += "\r\n\r\nColumns\r\n--------------";
              int iCount = pMatrix->GetColumnCount();
              MatrixColDef *pColDef;
              for (int iLup = 0; iLup < iCount; iLup++)
                {
                  pColDef = pMatrix->GetColumnDef(iLup);
                  if (pColDef)
                    cNotes += CString("\x0D\x0A") + pColDef->m_cColName;
                }

/// INTERNATIONALIZE
              if (iLup == 0)
                cNotes += CString("\x0D\x0A(none)");
            }
        }

      SetDlgItemText(IDC_GRID_NOTES,cNotes);
    }

  *pResult = 0;
}



BOOL MatrixSel::CheckModifyRights(Matrix *pMatrix)
{
  if (!pMatrix->RequiresModifyPassword() || (pMatrix->m_iSecurityPassedFlags & 2))
    return TRUE;

  // do password entry dialog here
  BOOL bRetval = FALSE;

  PasswordEntryDlg oPswDialog(pMatrix->GetModifyPassword(),this);
  if (oPswDialog.DoModal() == IDOK)
    {
      bRetval = TRUE;
      pMatrix->m_iSecurityPassedFlags |= 2;
    }

  return bRetval;
}



BOOL MatrixSel::CheckAccessRights(Matrix *pMatrix)
{
  if (!pMatrix->RequiresAccessPassword() || (pMatrix->m_iSecurityPassedFlags & 1))
    return TRUE;

  // do password entry dialog here
  BOOL bRetval = FALSE;

  PasswordEntryDlg oPswDialog(pMatrix->GetAccessPassword(),this);
  if (oPswDialog.DoModal() == IDOK)
    {
      bRetval = TRUE;
      pMatrix->m_iSecurityPassedFlags |= 1;
    }

  return bRetval;
}



void MatrixSel::OnMatrixCopy()
{
  HTREEITEM hItem = m_oTreeCtrl.GetSelectedItem();
  if (hItem)
    {
      MTXTREEITEMSTRUCT *spTI = (MTXTREEITEMSTRUCT *)m_oTreeCtrl.GetItemData(hItem);
      if (spTI && spTI->m_bLeafItem)
        {
          Matrix *pMatrix = (Matrix *)spTI->m_vpData;

          if (!pMatrix || !CheckAccessRights(pMatrix))
            return;

          DuplicateMatrix oDuplicateMatrix(m_pMatrixMgr,this);

          if (oDuplicateMatrix.DoModal() == IDOK)
            {
              Matrix *pNewMatrix = new Matrix(oDuplicateMatrix.m_cMatrixName,pMatrix);
              if (pNewMatrix)
                {
                  CString cFileName = oDuplicateMatrix.m_cMatrixName + CString(".xlg");
                  pNewMatrix->SetFileName(cFileName);

                  if (m_pMatrixMgr->SaveMatrix(pNewMatrix))
                    {
                      m_pMatrixMgr->m_oMatrices.SetAt(pNewMatrix->GetMatrixName(),(CObject *)pNewMatrix);
                      ((CEMXApp *)AfxGetApp())->RefreshFloatingGrids();
                      BuildTreeView();
                    }
                  else
                    {
                      delete pNewMatrix;
                    }
                }
            }
        }
    }
}
