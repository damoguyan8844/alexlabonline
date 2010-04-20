// HComWin.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "HComWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HiddenComWin

IMPLEMENT_DYNCREATE(HiddenComWin, CFrameWnd)

HiddenComWin::HiddenComWin()
{
  m_iTimerID = 0;
}

HiddenComWin::HiddenComWin(LPDISPATCH pXLDisp)
{
  AfxOleInit();

  m_bInRecalc = FALSE;

  m_oExcelApplication.AttachDispatch(pXLDisp);

  m_iRecalcInterval = 750;
  m_iTimerID = 0;
}


HiddenComWin::~HiddenComWin()
{
  if (m_iTimerID)
    KillTimer(m_iTimerID);
}


BEGIN_MESSAGE_MAP(HiddenComWin, CFrameWnd)
	//{{AFX_MSG_MAP(HiddenComWin)
	ON_WM_CREATE()
	ON_WM_TIMER()
  ON_MESSAGE(WM_RECALCULATE_EXCEL, OnRecalcExcel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HiddenComWin message handlers

int HiddenComWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
  m_bNeedsRecalc = FALSE;
  m_bInRecalc = FALSE;
  m_iTimerID = SetTimer(9876,m_iRecalcInterval,NULL);

	return 0;
}


void HiddenComWin::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

  if (nIDEvent != 1111)
    CFrameWnd::OnTimer(nIDEvent);

  if (m_bNeedsRecalc && m_oExcelApplication.m_lpDispatch)
    {
      if (!m_bInRecalc)
        {
          m_bInRecalc = TRUE;
          m_bNeedsRecalc = FALSE;
          m_oExcelApplication.CalculateFull();
          m_bInRecalc = FALSE;
        }
      // else just wait for the next timer event...
    }
}



LRESULT HiddenComWin::OnRecalcExcel(WPARAM wParam, LPARAM lParam)
{
  m_bNeedsRecalc = TRUE;
  if (m_iTimerID == 0)
    OnTimer(1111);

  return 0;
}


void HiddenComWin::ResetTimerInterval(UINT iNewInterval)
{
  if (iNewInterval != m_iRecalcInterval)
    {
      m_iRecalcInterval = iNewInterval;

      if (m_iTimerID)
        KillTimer(m_iTimerID);

      m_iTimerID = SetTimer(9876,m_iRecalcInterval,NULL);
    }
}
