// FlotValP.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FlotValP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FloatingValuesPage

IMPLEMENT_DYNCREATE(FloatingValuesPage, CMiniFrameWnd)

FloatingValuesPage::FloatingValuesPage()
{
  m_pMatrixMgr = NULL;
  m_pMatrix = NULL;

  m_hWildCardBmp = NULL;
  m_hInsertRowBmp = NULL;
  m_hDeleteRowBmp = NULL;
  m_hRecalcNowBmp = NULL;

  m_bRecalcExcelOnChanges = ((CEMXApp *)AfxGetApp())->IsRecalcOnChange();
}


FloatingValuesPage::FloatingValuesPage(MatrixManager *pMatrixMgr, Matrix *pMatrix)
{
  m_pMatrixMgr = pMatrixMgr;
  m_pMatrix = pMatrix;

  m_hWildCardBmp = NULL;
  m_hInsertRowBmp = NULL;
  m_hDeleteRowBmp = NULL;
  m_hRecalcNowBmp = NULL;

  m_bRecalcExcelOnChanges = ((CEMXApp *)AfxGetApp())->IsRecalcOnChange();
}



FloatingValuesPage::~FloatingValuesPage()
{
  if (m_hWildCardBmp)
    ::DeleteObject(m_hWildCardBmp);

  if (m_hInsertRowBmp)
    ::DeleteObject(m_hInsertRowBmp);

  if (m_hDeleteRowBmp)
    ::DeleteObject(m_hDeleteRowBmp);

  if (m_hRecalcNowBmp)
    ::DeleteObject(m_hRecalcNowBmp);
}


BEGIN_MESSAGE_MAP(FloatingValuesPage, CMiniFrameWnd)
	//{{AFX_MSG_MAP(FloatingValuesPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
  ON_BN_CLICKED(555, OnSaveClicked)
  ON_BN_CLICKED(556, OnCloseClicked)
  ON_BN_CLICKED(557, OnAutoRecalcClicked)
  ON_BN_CLICKED(559, OnRecalcNowClicked)
	ON_BN_CLICKED(IDC_DATAVALUES_DELETEROW, OnDatavaluesDeleterow)
	ON_BN_CLICKED(IDC_DATAVALUES_INSERTROW, OnDatavaluesInsertrow)
	ON_BN_CLICKED(IDC_DATAVALUES_WILDCARD, OnDatavaluesWildcard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FloatingValuesPage message handlers

int FloatingValuesPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  EnableToolTips();

  CRect oR;
  GetClientRect(&oR);

  CRect oB = oR;
  oB.bottom = oB.top+20;
  oB.right = oB.left+50;
  oR.top = oB.bottom+1;

  CString cButtonText;

  oB.OffsetRect(5,0);
  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_SAVEBTN))
    cButtonText = "Save";

  m_oSaveBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,oB,this,555);


  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_CLOSEBTN))
    cButtonText = "Close";

  m_oCloseBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,oB,this,556);

  oB.OffsetRect(oB.Width()+15,0);  // extra separator between text buttons and icons
  oB.right = oB.left+20;

  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_INSERTROWBTN))
    cButtonText = "Insert Row";

  m_oInsertRowBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,IDC_DATAVALUES_INSERTROW);
  m_hInsertRowBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_INSERTROW));
  m_oInsertRowBtn.SetBitmap(m_hInsertRowBmp);

  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_DELETEROWBTN))
    cButtonText = "Delete Row";

  m_oDeleteRowBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,IDC_DATAVALUES_DELETEROW);
  m_hDeleteRowBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_DELETEROW));
  m_oDeleteRowBtn.SetBitmap(m_hDeleteRowBmp);

  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_WILDCARDBTN))
    cButtonText = "Wildcard";

  m_oWildCardBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,IDC_DATAVALUES_WILDCARD);
  m_hWildCardBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_WILDCARD));
  m_oWildCardBtn.SetBitmap(m_hWildCardBmp);

  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_RECALCNOWBTN))
    cButtonText = "Recalc Now";

  m_oRecalcNowBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,559);
  m_hRecalcNowBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_RECALCNOW));
  m_oRecalcNowBtn.SetBitmap(m_hRecalcNowBmp);

  if (m_bRecalcExcelOnChanges)
    m_oRecalcNowBtn.EnableWindow(FALSE);

  oB.OffsetRect(oB.Width()+15,0);
  oB.right = oB.left + 500;
  if (!cButtonText.LoadString(IDS_FLOATVALUEPAGE_REFRESHCHECKBOXBTN))
    cButtonText = "Auto Recalculate Workbooks";

  m_oRecalcOnChanges.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE,oB,this,557);

  m_oBtnFont.CreatePointFont(80,"MS Sans serif");
  m_oSaveBtn.SetFont(&m_oBtnFont);
  m_oCloseBtn.SetFont(&m_oBtnFont);
  m_oRecalcOnChanges.SetFont(&m_oBtnFont);

  if (m_bRecalcExcelOnChanges)
    m_oRecalcOnChanges.SendMessage(BM_SETCHECK,1);

  m_oGrid.m_pMatrix = m_pMatrix;
  m_oGrid.m_iParentType = 2;
  m_oGrid.CreateGrid(WS_CHILD|WS_VISIBLE,oR,this,560);

  BuildGridStructure();

  m_pMatrix->SetModified(FALSE);
  EnableSaveBtn(FALSE);

  return 0;
}



void FloatingValuesPage::OnDestroy()
{
  // force reload of matrix
  if (m_pMatrix->IsModified())
    {
      CString cMatrixName = m_pMatrix->GetMatrixName();
      CString cFileName = m_pMatrix->GetFileName();
      m_pMatrixMgr->LoadMatrix(cFileName);

      if (m_bRecalcExcelOnChanges)
        ((FloatingThread *)AfxGetThread())->ExcelRecalculateFull();
    }

  CMiniFrameWnd::OnDestroy();
}



BOOL FloatingValuesPage::OnEraseBkgnd(CDC* pDC)
{
  CBrush oBr(GetSysColor(COLOR_BTNFACE));

  CRect oR;
  GetClientRect(&oR);
  pDC->FillRect(&oR,&oBr);

//	return CMiniFrameWnd::OnEraseBkgnd(pDC);

  return TRUE;
}



void FloatingValuesPage::OnSetFocus(CWnd* pOldWnd)
{
	CMiniFrameWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
  m_oGrid.SetFocus();
}



void FloatingValuesPage::OnSize(UINT nType, int cx, int cy)
{
	CMiniFrameWnd::OnSize(nType, cx, cy);

  CRect oR, oB;
  oR.left = 0;
  oR.right = cx;

  m_oSaveBtn.GetWindowRect(&oB);
  ScreenToClient(&oB);
  oR.top = oB.bottom+1;
  if (cy < oB.bottom+2)
    oR.bottom = oB.bottom+2;
  else
    oR.bottom = cy;

  m_oGrid.MoveWindow(&oR);
}



void FloatingValuesPage::OnSaveClicked()
{
  if (m_pMatrix->IsModified())
    {
      if (m_pMatrixMgr->SaveMatrix(m_pMatrix))
        EnableSaveBtn(FALSE);
    }
}



void FloatingValuesPage::OnCloseClicked()
{
  if (m_pMatrix->IsModified())
    {
      CString cPrompt;
      CString cTitle;

      if (!cPrompt.LoadString(IDS_FLOATVALUES_SAVEMODIFIED))
        cPrompt = "You have made changes to the grid values.\nWould you like to save your changes?";

      if (!cTitle.LoadString(IDS_FLOATVALUES_SAVEMODIFIEDCAPTION))
        cTitle = "Save Changes";

      int iRetval = ::MessageBox(m_hWnd,(LPCSTR)cPrompt,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNOCANCEL);

      if (iRetval == IDCANCEL)
        return;

      if (iRetval == IDYES)
        OnSaveClicked();
    }

  PostMessage(WM_CLOSE);
}



void FloatingValuesPage::OnAutoRecalcClicked()
{
  m_bRecalcExcelOnChanges = m_oRecalcOnChanges.SendMessage(BM_GETCHECK);

  if (m_bRecalcExcelOnChanges)
    m_oRecalcNowBtn.EnableWindow(FALSE);
  else
    m_oRecalcNowBtn.EnableWindow(TRUE);
}



void FloatingValuesPage::OnRecalcNowClicked()
{
  ((FloatingThread *)AfxGetThread())->ExcelRecalculateFull();
}



void FloatingValuesPage::OnDatavaluesWildcard()
{
  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = m_oGrid.GetCurrentCol();
  lStartRow = lEndRow = m_oGrid.GetCurrentRow();

  m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  if (iStartCol == -1 && iEndCol == -1)
    {
      iStartCol = 0;
      iEndCol = m_oGrid.GetNumberCols() - 1;
    }

  if (lStartRow == -1 && lEndRow == -1)
    {
      lStartRow = 0;
      lEndRow = m_oGrid.GetNumberRows();
    }

  for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
    {
      if (iCol < 0 || iCol == m_pMatrix->GetColumnCount())
        continue;  // skip headings and return columns

      for (int iRow = lStartRow; iRow <= lEndRow; iRow++)
        {
          if (iRow < 0)
            continue;  // skip heading row

          m_oGrid.QuickSetText(iCol,iRow,"*");
        }
    }

  m_oGrid.RedrawAll();

  m_oGrid.SetFocus();
}



void FloatingValuesPage::OnDatavaluesDeleterow()
{
  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = m_oGrid.GetCurrentCol();
  lStartRow = lEndRow = m_oGrid.GetCurrentRow();

  // m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  m_oGrid.m_GI->m_multiSelect->GetTotalRange(&iStartCol, &lStartRow, &iEndCol, &lEndRow);

  if (lStartRow != lEndRow && (iStartCol == iEndCol || iStartCol != 0))
    return;

  CString cMsg, cTitle;

  if (!cTitle.LoadString(IDS_CONFIRMDELETETITLE))
    cTitle = "Confirm Delete";

  if (!cMsg.LoadString(IDS_CONFIRMDELETEROWS))
    cMsg = "Delete These Rows.\x0D\x0A Are You Sure?";

  if (::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNO) == IDYES)
    {
      for (int iLup = lEndRow; iLup >= lStartRow; iLup--)
        {
          m_pMatrix->DeleteRow(iLup);
          m_oGrid.DeleteRow(iLup);
        }

      // make sure that we always have at least one row...
      if (m_oGrid.GetNumberRows() == 0)
        m_oGrid.AppendRow();

      GridChanged();
    }

  m_oGrid.SetFocus();
}



void FloatingValuesPage::OnDatavaluesInsertrow()
{
  long lRow = m_oGrid.GetCurrentRow();
  if (lRow < 0)
    lRow = 0;

  int iCol = m_oGrid.GetCurrentCol();
  if (iCol < 0)
    iCol = 0;

  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = iCol;
  lStartRow = lEndRow = lRow;

  m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  if (lStartRow == lEndRow && lStartRow != lRow)
    lRow = lStartRow;

  m_pMatrix->InsertRow(lRow);
  m_oGrid.AppendRow();

  m_oGrid.RedrawAll();

  m_oGrid.GotoCell(iCol,lRow);

  m_oGrid.SetFocus();

  GridChanged();
}



void FloatingValuesPage::BuildGridStructure()
{
  int iColCount = m_pMatrix->GetColumnCount();

  // erase current grid
  for (int iCol = m_oGrid.GetNumberCols()-1; iCol >= 0; iCol--)
    m_oGrid.DeleteCol(iCol);

  m_oGrid.SetNumberCols(0);
  m_oGrid.SetNumberRows(0);

  m_oGrid.SetNumberCols(iColCount+1);
  m_oGrid.SetNumberRows(m_pMatrix->GetRowCount() + 1);

  CUGCell oCell;
  short iType, iAlign;
  MatrixColDef *pColDef;
  LPCSTR cpMask;
  char caBuf[128];

  for (int iLup = 0; iLup < iColCount; iLup++)
    {
      pColDef = m_pMatrix->GetColumnDef(iLup);

      m_oGrid.GetColDefault(iLup, &oCell);
      if (pColDef->m_bUsePickList)
        {
	      oCell.SetCellType(UGCT_DROPLIST);
	      oCell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);

          CString cChoices = "*\n";
          for (int iEntry = 0; iEntry < pColDef->m_oPickListChoices.GetSize(); iEntry++)
            cChoices += pColDef->m_oPickListChoices.GetAt(iEntry) + CString("\n");

	      oCell.SetLabelText(cChoices);
        }

      switch (pColDef->m_iColType)
        {
          case COLTYPE_STRING:
          case COLTYPE_FORMULA:
            iType = UGCELLDATA_STRING;
            iAlign = UG_ALIGNLEFT;
            cpMask = "";
            break;

          case COLTYPE_INTEGER:
            oCell.SetNumberDecimals(0);
            cpMask = "#########";
            iType = UGCELLDATA_NUMBER;
            iAlign = UG_ALIGNRIGHT;
            break;

          case COLTYPE_DOUBLE:
            oCell.SetNumberDecimals(-1);
            iType = UGCELLDATA_NUMBER;
            iAlign = UG_ALIGNRIGHT;
            cpMask = "666666666666666";
            break;

          case COLTYPE_DATE:
            iType = UGCELLDATA_TIME;
            iAlign = UG_ALIGNLEFT;

            caBuf[0] = 0;
            m_pMatrix->GetDateMask(caBuf);
            if (caBuf[0])
              cpMask = caBuf;
            else
              cpMask = "99/99/9999";
            break;

          default:
            iType = UGCELLDATA_STRING;
            iAlign = UG_ALIGNLEFT;
            cpMask = "";
            break;
        }

      oCell.SetDataType(iType);
      oCell.SetAlignment(iAlign);
      oCell.SetMask(cpMask);

      m_oGrid.SetColDefault(iLup, &oCell);

      m_oGrid.QuickSetText(iLup, -1, pColDef->m_cColName);
    }

  // now, add result column
  m_oGrid.GetColDefault(iColCount, &oCell);
  switch (m_pMatrix->GetReturnValueColumnType())
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        iType = UGCELLDATA_STRING;
        iAlign = UG_ALIGNLEFT;
        cpMask = "";
        break;

      case COLTYPE_INTEGER:
        oCell.SetNumberDecimals(0);
        cpMask = "#########";
        iType = UGCELLDATA_NUMBER;
        iAlign = UG_ALIGNRIGHT;
        break;

      case COLTYPE_DOUBLE:
        oCell.SetNumberDecimals(-1);
        iType = UGCELLDATA_NUMBER;
        iAlign = UG_ALIGNRIGHT;
        cpMask = "666666666666666";
        break;

      case COLTYPE_DATE:
        iType = UGCELLDATA_TIME;
        iAlign = UG_ALIGNLEFT;

        caBuf[0] = 0;
        m_pMatrix->GetDateMask(caBuf);
        if (caBuf[0])
          cpMask = caBuf;
        else
          cpMask = "99/99/9999";
        break;

      default:
        iType = UGCELLDATA_STRING;
        iAlign = UG_ALIGNLEFT;
        cpMask = "";
        break;
    }

  oCell.SetDataType(iType);
  oCell.SetAlignment(iAlign);
  oCell.SetMask(cpMask);

  m_oGrid.SetColDefault(iColCount, &oCell);

  m_oGrid.BestFit(0,iColCount,m_pMatrix->GetRowCount(),UG_BESTFIT_TOPHEADINGS);

  // expand columns evenly to fill the window...
  int iWidth = 0;
  int iTotalWidth = m_oGrid.GetSH_Width();
  iColCount++;
  for (iLup = 0; iLup < iColCount; iLup++)
    {
      m_oGrid.GetColWidth(iLup,&iWidth);
      iTotalWidth += iWidth;
    }

  CRect oR;
  m_oGrid.GetClientRect(&oR);
  int iWinWidth = oR.Width() - ::GetSystemMetrics(SM_CXVSCROLL);
  if (iTotalWidth < iWinWidth)
    {
      iWinWidth = ((iWinWidth - iTotalWidth) / iColCount) - 1;

      if (iWinWidth > 0)
        {
          for (iLup = 0; iLup < iColCount; iLup++)
            {
              m_oGrid.GetColWidth(iLup,&iWidth);
              iWidth += iWinWidth;
              m_oGrid.SetColWidth(iLup,iWidth);
            }
        }
    }
}


void FloatingValuesPage::GridChanged()
{
  EnableSaveBtn();

  if (m_bRecalcExcelOnChanges)
    ((FloatingThread *)AfxGetThread())->ExcelRecalculateFull();
}
