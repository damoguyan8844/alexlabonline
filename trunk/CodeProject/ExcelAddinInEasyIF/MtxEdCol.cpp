// MtxEdCol.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "MtxEdCol.h"
#include "ColEdit.h"
#include "mtxedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MatrixEditCol property page

IMPLEMENT_DYNCREATE(MatrixEditCol, CPropertyPage)

MatrixEditCol::MatrixEditCol()
  : CPropertyPage(MatrixEditCol::IDD)
{
  m_pMatrix = NULL;
}


MatrixEditCol::MatrixEditCol(Matrix *pMatrix)
  : CPropertyPage(MatrixEditCol::IDD)
{
  m_pMatrix = pMatrix;

	//{{AFX_DATA_INIT(MatrixEditCol)
	m_iReturnType = m_pMatrix->GetReturnValueColumnType();
	//}}AFX_DATA_INIT
}

MatrixEditCol::~MatrixEditCol()
{
}

void MatrixEditCol::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MatrixEditCol)
	DDX_Control(pDX, IDC_COLS_LIST, m_oColList);
	DDX_CBIndex(pDX, IDC_COLS_RETURNTYPE, m_iReturnType);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
    {
      if (m_iReturnType != m_pMatrix->GetReturnValueColumnType())
        m_pMatrix->SetReturnValueColumnType(m_iReturnType);
    }
}


BEGIN_MESSAGE_MAP(MatrixEditCol, CPropertyPage)
	//{{AFX_MSG_MAP(MatrixEditCol)
	ON_BN_CLICKED(IDC_COLS_ADD, OnColsAdd)
	ON_BN_CLICKED(IDC_COLS_PROPERTIES, OnColsProperties)
	ON_BN_CLICKED(IDC_COLS_REMOVE, OnColsRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_COLS_LIST, OnDblclkColsList)
	ON_BN_CLICKED(IDC_COLS_MOVEDOWN, OnColsMovedown)
	ON_BN_CLICKED(IDC_COLS_MOVEUP, OnColsMoveup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLS_LIST, OnItemchangedColsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MatrixEditCol message handlers

void MatrixEditCol::OnColsAdd()
{
  // ColumnEdit oColEdit(m_pMatrix,-1,"EasyIF - Add Grid Column");
  ColumnEdit oColEdit(m_pMatrix,-1,IDS_ADDGRIDCOLUMNTITLE);

  int iResult = oColEdit.DoModal();

  if (iResult == IDOK)
    {
      MatrixColDef *pColDef = NULL;

      if (oColEdit.m_pPositionTab->m_bAppend)
        pColDef = m_pMatrix->AddColumn();
      else
        pColDef = m_pMatrix->InsertColumn(oColEdit.m_pPositionTab->m_iInsertBefore);

      if (pColDef)
        {
          pColDef->m_bCompareCol = oColEdit.m_pGeneralTab->m_bCompareCol;
          pColDef->m_cColName = oColEdit.m_pGeneralTab->m_cColName;
          pColDef->m_cNotes = oColEdit.m_pGeneralTab->m_cNotes;
          m_pMatrix->SetColumnType(pColDef,oColEdit.m_pGeneralTab->m_iDataType);
          pColDef->m_iCompareOp = oColEdit.m_pGeneralTab->m_iCompareOp;
          pColDef->m_bUsePickList = oColEdit.m_pPickListTab->m_bUsePickList;
          pColDef->m_bRestrictToList = oColEdit.m_pPickListTab->m_bRestrictToList;

          CString cItem;
          CString cChoices = oColEdit.m_pPickListTab->m_cChoices;
          pColDef->m_oPickListChoices.RemoveAll();

          int iPos = cChoices.Find("\x0D\x0A");
          while (iPos > -1)
            {
              cItem = cChoices.Left(iPos);
              cItem.TrimRight();

              if (!cItem.IsEmpty())
                pColDef->m_oPickListChoices.Add(cItem);

              cChoices = cChoices.Mid(iPos+2);
              iPos = cChoices.Find("\x0D\x0A");
            }

          cChoices.TrimRight();

          if (!cChoices.IsEmpty())
            pColDef->m_oPickListChoices.Add(cChoices);

          m_pMatrix->SetModified();

          LoadColumnList();
        }
    }
}



void MatrixEditCol::OnColsProperties()
{
  int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);
  if (iIndex > -1)
    {
//      ColumnEdit oColEdit(m_pMatrix,iIndex,"EasyIF - Grid Column Properties");
      ColumnEdit oColEdit(m_pMatrix,iIndex,IDS_EDITGRIDCOLUMNTITLE);

      int iResult = oColEdit.DoModal();

      if (iResult == IDOK)
        {
          MatrixColDef *pColDef = m_pMatrix->GetColumnDef(iIndex);
          pColDef->m_bCompareCol = oColEdit.m_pGeneralTab->m_bCompareCol;
          pColDef->m_cColName = oColEdit.m_pGeneralTab->m_cColName;
          pColDef->m_cNotes = oColEdit.m_pGeneralTab->m_cNotes;
          m_pMatrix->SetColumnType(pColDef,oColEdit.m_pGeneralTab->m_iDataType);
          pColDef->m_iCompareOp = oColEdit.m_pGeneralTab->m_iCompareOp;
          pColDef->m_bUsePickList = oColEdit.m_pPickListTab->m_bUsePickList;
          pColDef->m_bRestrictToList = oColEdit.m_pPickListTab->m_bRestrictToList;

          CString cItem;
          CString cChoices = oColEdit.m_pPickListTab->m_cChoices;
          pColDef->m_oPickListChoices.RemoveAll();

          int iPos = cChoices.Find("\x0D\x0A");
          while (iPos > -1)
            {
              cItem = cChoices.Left(iPos);
              cItem.TrimRight();

              if (!cItem.IsEmpty())
                pColDef->m_oPickListChoices.Add(cItem);

              cChoices = cChoices.Mid(iPos+2);
              iPos = cChoices.Find("\x0D\x0A");
            }

          cChoices.TrimRight();

          if (!cChoices.IsEmpty())
            pColDef->m_oPickListChoices.Add(cChoices);

          LoadColumnList();
        }
    }
}



void MatrixEditCol::OnColsRemove()
{
  int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);
  if (iIndex > -1)
    {
      CString cTitle, cMsg;

      if (!cTitle.LoadString(IDS_CONFIRMDELETETITLE))
        cTitle = "Confirm Delete";

      if (!cMsg.LoadString(IDS_DELETECOLUMNMESSAGE))
        cMsg = "Are You Sure That You Wish\x0D\x0ATo Remove This Column?";

      if (::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNO) == IDYES)
        {
          m_pMatrix->DeleteColumn(iIndex);
          LoadColumnList();
        }
    }
}



BOOL MatrixEditCol::OnKillActive()
{
  BOOL bRetval = CPropertyPage::OnKillActive();

  MatrixEdit *pParent = (MatrixEdit *)GetParent();

  if (bRetval && pParent)
    {
      if (!pParent->CheckColumnsDefined())
        bRetval = FALSE;
    }

  return bRetval;
}



void MatrixEditCol::OnOK()
{
  CPropertyPage::OnOK();
}



BOOL MatrixEditCol::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnSetActive();
}

BOOL MatrixEditCol::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  CRect oR;
  m_oColList.GetClientRect(&oR);
  int iOneTenth = oR.Width()/10;

  CString cColHeading;

  if (!cColHeading.LoadString(IDS_COLHEADNAME))
    cColHeading = "Name";
  m_oColList.InsertColumn(0,(LPCSTR)cColHeading,LVCFMT_LEFT,iOneTenth * 6);

  if (!cColHeading.LoadString(IDS_COLHEADTYPE))
    cColHeading = "Type";
  m_oColList.InsertColumn(1,(LPCSTR)cColHeading,LVCFMT_LEFT,iOneTenth * 2);

  if (!cColHeading.LoadString(IDS_COLHEADACTIVE))
    cColHeading = "Active";
  m_oColList.InsertColumn(2,(LPCSTR)cColHeading,LVCFMT_LEFT,iOneTenth * 2);

  ListView_SetExtendedListViewStyleEx(m_oColList.m_hWnd,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

  LoadColumnList();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MatrixEditCol::OnDblclkColsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnColsProperties();

	*pResult = 0;
}



void MatrixEditCol::LoadColumnList()
{
  int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);

  m_oColList.DeleteAllItems();

  int iCount = m_pMatrix->GetColumnCount();
  MatrixColDef *pColDef;
  CString cText;

  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pColDef = m_pMatrix->GetColumnDef(iLup);
      if (pColDef)
        {
          m_oColList.InsertItem(iLup,pColDef->m_cColName);
          switch (pColDef->m_iColType)
            {
              case 0:
                if (!cText.LoadString(IDS_COLTYPETEXT))
                  cText = "Text";
                break;
              case 1:
                if (!cText.LoadString(IDS_COLTYPEWHOLENUMBER))
                  cText = "Whole Number";
                break;
              case 2:
                if (!cText.LoadString(IDS_COLTYPEDECIMALNUMBER))
                  cText = "Decimal Number";
                break;
              case 3:
                if (!cText.LoadString(IDS_COLTYPEDATE))
                  cText = "Date";
                break;
              default:
                cText = "????";
                break;
            }

          m_oColList.SetItemText(iLup,1,cText);

          if (pColDef->m_bCompareCol)
            {
              if (!cText.LoadString(IDS_YES))
                cText = "Yes";
            }
          else
            {
              if (!cText.LoadString(IDS_NO))
                cText = "No";
            }

          m_oColList.SetItemText(iLup,2,cText);
        }
    }

  GetDlgItem(IDC_COLS_PROPERTIES)->EnableWindow((iCount > 0));
  GetDlgItem(IDC_COLS_REMOVE)->EnableWindow((iCount > 0));
  GetDlgItem(IDC_COLS_MOVEUP)->EnableWindow((iCount > 0));
  GetDlgItem(IDC_COLS_MOVEDOWN)->EnableWindow((iCount > 0));

  if (iIndex < 0)
    iIndex = 0;

  if (iIndex >= iCount)
    iIndex = iCount-1;

  if (iCount > 0)
    m_oColList.SetItemState(iIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

  MatrixEdit *pParent = (MatrixEdit *)GetParent();
  if (pParent)
    pParent->EnableOKButton();
}



void MatrixEditCol::OnColsMovedown()
{
  int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);
  if (iIndex > -1)
    {
      m_oColList.SetItemState(iIndex+1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
      m_pMatrix->MoveColumn(iIndex,1);
      LoadColumnList();
    }
}



void MatrixEditCol::OnColsMoveup()
{
  int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);
  if (iIndex > -1)
    {
      m_oColList.SetItemState(iIndex-1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
      m_pMatrix->MoveColumn(iIndex,-1);
      LoadColumnList();
    }
}


void MatrixEditCol::OnItemchangedColsList(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

  int iCount = m_oColList.GetItemCount();
  if (iCount > 1)
    {
      int iIndex = m_oColList.GetNextItem(-1,LVNI_SELECTED);
      if (iIndex > -1)
        {
          GetDlgItem(IDC_COLS_MOVEUP)->EnableWindow((iIndex > 0));
          GetDlgItem(IDC_COLS_MOVEDOWN)->EnableWindow((iIndex < (iCount - 1)));
        }
    }

  *pResult = 0;
}
