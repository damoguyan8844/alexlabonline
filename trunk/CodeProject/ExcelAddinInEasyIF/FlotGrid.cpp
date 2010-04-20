// FlotGrid.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FlotGrid.h"

#include "mtxedit.h"
#include "pswdlg.h"
#include "dupmtx.h"
#include "flotvalp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FloatingGridWnd

IMPLEMENT_DYNCREATE(FloatingGridWnd, CMiniFrameWnd)

FloatingGridWnd::FloatingGridWnd()
{
  m_pMatrixMgr = NULL;
  m_bIsWindowVisible = FALSE;

  m_hNewBmp = NULL;
  m_hRefreshBmp = NULL;
  m_hHideBmp = NULL;
}

FloatingGridWnd::FloatingGridWnd(MatrixManager *pMatrixMgr)
{
  m_pMatrixMgr = pMatrixMgr;
  m_bIsWindowVisible = FALSE;

  m_hNewBmp = NULL;
  m_hRefreshBmp = NULL;
  m_hHideBmp = NULL;
}


FloatingGridWnd::~FloatingGridWnd()
{
  if (m_hNewBmp)
    ::DeleteObject(m_hNewBmp);

  if (m_hRefreshBmp)
    ::DeleteObject(m_hRefreshBmp);

  if (m_hHideBmp);
    ::DeleteObject(m_hHideBmp);
}



BEGIN_MESSAGE_MAP(FloatingGridWnd, CMiniFrameWnd)
	//{{AFX_MSG_MAP(FloatingGridWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
  ON_BN_CLICKED(9874, OnListRefresh)
  ON_BN_CLICKED(9875, OnCreateNew)
  ON_BN_CLICKED(9873, OnHideGridPad)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FloatingGridWnd message handlers

static char cpRegKey[] = "Software\\Webogy\\EasyIF";
static char cpFloatGridPos[] = "FloatingPalettePos";
static char cpFloatGridVisible[] = "FloatingPaletteVisible";

void FloatingGridWnd::SaveSizeAndVisibility()
{
  //Save off size and position...
  HKEY hRegKey;
  DWORD dw;

  if (!::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                        cpRegKey,
                        0,
                        REG_NONE,
                        REG_OPTION_NON_VOLATILE,
                        KEY_READ | KEY_WRITE,
                        NULL,
                        &hRegKey,
                        &dw))
    {
//      DWORD dwType;
      char caBuf[256];
      WINDOWPLACEMENT sWP;

      GetWindowPlacement(&sWP);

      sprintf(caBuf,"%05d,%05d,%05d,%05d",
                    sWP.rcNormalPosition.left,
                    sWP.rcNormalPosition.top,
                    sWP.rcNormalPosition.right,
                    sWP.rcNormalPosition.bottom);

      DWORD dwSize = strlen(caBuf);
      ::RegSetValueEx(hRegKey,cpFloatGridPos,NULL,REG_SZ,(unsigned char *)caBuf,dwSize);

      if (m_bIsWindowVisible)
        strcpy(caBuf,"1");
      else
        strcpy(caBuf,"0");

      dwSize = 1;
      ::RegSetValueEx(hRegKey,cpFloatGridVisible,NULL,REG_SZ,(unsigned char *)caBuf,dwSize);

      ::RegCloseKey(hRegKey);
    }
}



BOOL FloatingGridWnd::PreCreateWindow(CREATESTRUCT& cs)
{
  // restore size and position...
  HKEY hRegKey;

  if (!::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                      cpRegKey,
                      0,
                      KEY_READ,
                      &hRegKey))
    {
      char caBuf[256];

      DWORD dwSize = sizeof(caBuf)-1;
      memset(caBuf,0,sizeof(caBuf));
      ::RegQueryValueEx(hRegKey,cpFloatGridPos,NULL,NULL,(unsigned char *)caBuf,&dwSize);


      // format = "%05d,%05d,%05d,%05d"
      if (caBuf[0])
        {
          int iLeft = atoi(caBuf);
          cs.x = iLeft;
          int iTop = atoi(caBuf+6);
          cs.y = iTop;

          int iWidth = atoi(caBuf+12) - iLeft;
          if (iWidth > 30)
            cs.cx = iWidth;

          int iHeight = atoi(caBuf+18) - iTop;
          if (iHeight > 50)
            cs.cy = iHeight;
        }

      dwSize = sizeof(caBuf)-1;
      memset(caBuf,0,sizeof(caBuf));
      ::RegQueryValueEx(hRegKey,cpFloatGridVisible,NULL,NULL,(unsigned char *)caBuf,&dwSize);

      if (caBuf[0] == 0)
        caBuf[0] = '1';

      if (caBuf[0] == '0')
        {
          cs.style &= ~WS_VISIBLE;
          m_bIsWindowVisible = FALSE;
        }
      else
        {
          m_bIsWindowVisible = TRUE;
        }

      ::RegCloseKey(hRegKey);
    }

  return CMiniFrameWnd::PreCreateWindow(cs);
}



int FloatingGridWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
  EnableToolTips();

  CRect oR;
  GetClientRect(&oR);

  CRect oB = oR;
  oB.bottom = oB.top+20;
  oB.right = oB.left+20;
  oR.top = oB.bottom+1;

  CString cButtonText;
  if (!cButtonText.LoadString(IDS_FLOATCREATENEWBTN))
    cButtonText = "New...";

  m_oCreateNewBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,9875);

  m_hNewBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_FGNEW));
  m_oCreateNewBtn.SetBitmap(m_hNewBmp);

  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATREFRESHBTN))
    cButtonText = "Refresh";
  m_oRefreshBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,9874);


  m_hRefreshBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_FGREFRESH));
  m_oRefreshBtn.SetBitmap(m_hRefreshBmp);

  oB.OffsetRect(oB.Width()+5,0);

  if (!cButtonText.LoadString(IDS_FLOATHIDEBTN))
    cButtonText = "Hide";
  m_oHideBtn.Create((LPCSTR)cButtonText,BS_PUSHBUTTON|BS_BITMAP|WS_CHILD|WS_VISIBLE,oB,this,9873);


  m_hHideBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_FGHIDE));
  m_oHideBtn.SetBitmap(m_hHideBmp);

  m_oCreateNewBtn.EnableToolTips();
  m_oRefreshBtn.EnableToolTips();
  m_oHideBtn.EnableToolTips();

  m_oListBox.CreateEx(WS_EX_CLIENTEDGE,"LISTBOX",NULL,LBS_SORT|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_WANTKEYBOARDINPUT|WS_VISIBLE|WS_CHILD|WS_VSCROLL,oR,this,9876);

  m_oLBFont.CreatePointFont(80,"MS Sans serif");
  m_oCreateNewBtn.SetFont(&m_oLBFont);
  m_oListBox.SetFont(&m_oLBFont);

  LoadMatrices();

  return 0;
}



void FloatingGridWnd::LoadMatrices()
{
  CWaitCursor oWait;

  m_oListBox.ResetContent();

  if (m_pMatrixMgr)
    {
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

              m_oListBox.AddString(pMatrix->GetMatrixName());
            }
        }

    }
  else
    {
/// INTERNATIONALIZE
      m_oListBox.AddString("Error Loading Matrix Engine");
    }
}



void FloatingGridWnd::OnSize(UINT nType, int cx, int cy)
{
	CMiniFrameWnd::OnSize(nType, cx, cy);

  CRect oR, oB;
  oR.left = 0;
  oR.right = cx;

  m_oCreateNewBtn.GetWindowRect(&oB);
  ScreenToClient(&oB);
  oR.top = oB.bottom+1;
  if (cy < oB.bottom+2)
    oR.bottom = oB.bottom+2;
  else
    oR.bottom = cy;

  m_oListBox.MoveWindow(&oR);

  SaveSizeAndVisibility();
}



void FloatingGridWnd::OnSetFocus(CWnd* pOldWnd)
{
	CMiniFrameWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
  m_oListBox.SetFocus();
}



BOOL FloatingGridWnd::OnEraseBkgnd(CDC* pDC)
{
  CBrush oBr(GetSysColor(COLOR_BTNFACE));

  CRect oR;
  GetClientRect(&oR);
  pDC->FillRect(&oR,&oBr);

//	return CMiniFrameWnd::OnEraseBkgnd(pDC);

  return TRUE;
}



void FloatingGridWnd::OnListRefresh()
{
  LoadMatrices();
}



void FloatingGridWnd::OnHideGridPad()
{
  ToggleDisplay();
}



void FloatingGridWnd::OnCreateNew()
{
  CString cDefaultCategory;
  if (!cDefaultCategory.LoadString(IDS_DEFCATEGORY))
    cDefaultCategory = "General";

  Matrix *pMatrix = new Matrix;
  pMatrix->SetCategory(cDefaultCategory);

  MatrixEdit oEdit(pMatrix,IDS_ADDNEWGRIDTITLE,this);

  int iResult = oEdit.DoModal();

  if (iResult == IDOK)
    {
      // add and save matrix...
      if (m_pMatrixMgr->SaveMatrix(pMatrix))
        {
          m_pMatrixMgr->m_oMatrices.SetAt(pMatrix->GetMatrixName(),(CObject *)pMatrix);
          LoadMatrices();
        }
    }
  else
    {
      delete pMatrix;
    }
}



void FloatingGridWnd::EditGrid(LPCSTR cpGridName)
{
  Matrix *pMatrix = m_pMatrixMgr->GetMatrix(cpGridName);
  if (pMatrix)
    {
      if (!CheckModifyRights(pMatrix))
        return;

      CString cOrigCategory = pMatrix->GetCategory();

      MatrixEdit oEdit(pMatrix,IDS_EDITGRIDTITLE,this);
      int iResult = oEdit.DoModal();
      if (iResult == IDOK)
        {
          CWaitCursor oWait;

          if (m_pMatrixMgr->SaveMatrix(pMatrix))
            {
              // SEND MESSAGE TO EXCEL!!!!!!!! m_bMatricesChanged = TRUE;
              ((FloatingThread *)AfxGetThread())->ExcelRecalculateFull();
            }
        }
    }
}



BOOL FloatingGridWnd::CheckModifyRights(Matrix *pMatrix)
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



BOOL FloatingGridWnd::CheckAccessRights(Matrix *pMatrix)
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



void FloatingGridWnd::DoContextMenu(LPCSTR cpGridName, CPoint *opOrigPt)
{
  CMenu oMenu;

  if (oMenu.LoadMenu(IDR_FLOATCTXT_MENU))
    {
      CMenu *pPopupMenu = oMenu.GetSubMenu(0);
      if (pPopupMenu == NULL)
        return;

      pPopupMenu->SetDefaultItem(1000);


      int iSelection = TrackPopupMenu(pPopupMenu->m_hMenu,
                                      TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON,
                                      opOrigPt->x,
                                      opOrigPt->y,
                                      0,       // reserved, must be zero
                                      m_hWnd,
                                      NULL);

      switch (iSelection)
        {
          case 1000: // data values page
            ShowValuesPage(cpGridName);
            break;
          case 2000: // properties page
            EditGrid(cpGridName);
            break;
          case 3000: // delete
            DeleteGrid(cpGridName);
            break;
          case 4000: // duplicate
            CopyGrid(cpGridName);
            break;
          default:
            break;
        }
    }
  else
    ::MessageBox(NULL,"Menu Resource Missing!","Context Menu Error",MB_OK|MB_ICONHAND);
}



void FloatingGridWnd::DeleteGrid(LPCSTR cpGridName)
{
  Matrix *pMatrix = m_pMatrixMgr->GetMatrix(cpGridName);
  if (pMatrix)
    {
      if (!CheckModifyRights(pMatrix))
        return;

      CString cFmt, cTitle;
      if (!cTitle.LoadString(IDS_CONFIRMDELETETITLE))
        cTitle = "Confirm Delete";

      if (!cFmt.LoadString(IDS_DELETEGRIDPROMPT))
        cFmt = "Permanantly Remove '%s'.\x0D\x0A Are You Sure?";

      char caBuf[512];
      sprintf(caBuf,(LPCSTR)cFmt,pMatrix->GetMatrixName());

      if (::MessageBox(m_hWnd,caBuf,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNO) == IDYES)
        {
          CWaitCursor oWait;

          m_pMatrixMgr->RemoveMatrix(pMatrix);
          LoadMatrices();

          // SEND MESSAGE TO EXCEL!!!!!!!! m_bMatricesChanged = TRUE;
          ((FloatingThread *)AfxGetThread())->ExcelRecalculateFull();
        }
    }
}



void FloatingGridWnd::CopyGrid(LPCSTR cpGridName)
{
  Matrix *pMatrix = m_pMatrixMgr->GetMatrix(cpGridName);
  if (pMatrix)
    {
      if (!CheckAccessRights(pMatrix))
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
                  LoadMatrices();
                }
              else
                {
                  delete pNewMatrix;
                }
            }
        }
    }
}



void FloatingGridWnd::ToggleDisplay()
{
  if (m_bIsWindowVisible)
    ShowWindow(SW_HIDE);
  else
    ShowWindow(SW_SHOW);

  m_bIsWindowVisible = !m_bIsWindowVisible;

  SaveSizeAndVisibility();
}



void FloatingGridWnd::ShowValuesPage(LPCSTR cpGridName)
{
  Matrix *pMatrix = m_pMatrixMgr->GetMatrix(cpGridName);
  if (pMatrix)
    {
      if (!CheckModifyRights(pMatrix))
        return;

      // check for already open page...

      // create edit window...
      FloatingValuesPage *pVP = new FloatingValuesPage(m_pMatrixMgr,pMatrix);
      CString cVPTitle;
      if (!cVPTitle.LoadString(IDS_FLOATVALUEPAGE_CAPTION))
        cVPTitle = "Data Values For %s";

      char caBuf[512];
      sprintf(caBuf,(LPCSTR)cVPTitle,(LPCSTR)pMatrix->GetMatrixName());
      pVP->Create(NULL,
                  caBuf,
                  MFS_THICKFRAME|MFS_SYNCACTIVE|WS_POPUP|WS_SYSMENU|WS_CAPTION|WS_VISIBLE,
                  CRect(0,0,300,200),
                  GetParent());

    }
}



void FloatingGridWnd::OnMove(int x, int y)
{
	CMiniFrameWnd::OnMove(x, y);

  SaveSizeAndVisibility();
}
