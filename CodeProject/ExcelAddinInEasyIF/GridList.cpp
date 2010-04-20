// GridList.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "GridList.h"

#include "flotgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridList

CGridList::CGridList()
{
}

CGridList::~CGridList()
{
}


BEGIN_MESSAGE_MAP(CGridList, CListBox)
	//{{AFX_MSG_MAP(CGridList)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridList message handlers

void CGridList::OnDblclk()
{
  int iCurSel = GetCurSel();

  if (iCurSel > -1)
    {
      CString cName;
      GetText(iCurSel,cName);

//      ((FloatingGridWnd *)GetParent())->EditGrid(cName);
      ((FloatingGridWnd *)GetParent())->ShowValuesPage(cName);
    }
}

void CGridList::OnContextMenu(CWnd* pWnd, CPoint point)
{
  BOOL bOutside;

  CPoint oOrigPt(point);

  ScreenToClient(&point);
  int iNewSel = ItemFromPoint(point,bOutside);
  int iCurSel = GetCurSel();

  if (iNewSel != iCurSel && iNewSel > -1)
    {
      SetCurSel(iNewSel);
      iCurSel = iNewSel;
    }

  if (iCurSel > -1)
    {
      CString cName;
      GetText(iCurSel,cName);

      ((FloatingGridWnd *)GetParent())->DoContextMenu((LPCSTR)cName,&oOrigPt);
    }
}

