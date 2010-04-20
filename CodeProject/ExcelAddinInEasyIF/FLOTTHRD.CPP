// FlotThrd.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FlotThrd.h"

#include "xlcall.h"
#include "framewrk.h"

#include <afxdisp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FloatingThread

IMPLEMENT_DYNCREATE(FloatingThread, CWinThread)

FloatingThread::FloatingThread()
{
  m_hComWnd = NULL;
}

FloatingThread::~FloatingThread()
{
}

BOOL FloatingThread::InitInstance()
{
  // create popup grid list tool...
  m_pFloatingGrid = new FloatingGridWnd(&(((CEMXApp *)m_pMainApp))->m_oMatrixManager);
/// INTERNATIONALIZE
  m_pFloatingGrid->Create(NULL,
                          "EasyIF Grids",
                          MFS_THICKFRAME|MFS_SYNCACTIVE|MFS_BLOCKSYSMENU|WS_POPUP|WS_CAPTION|WS_VISIBLE,
                          CRect(0,0,150,200),
                          CWnd::FromHandle(m_hExcelMainWnd));

	return TRUE;
}

int FloatingThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
  if (m_pFloatingGrid)
    {
      m_pFloatingGrid->DestroyWindow();
    }

	return CWinThread::ExitInstance();
}



void FloatingThread::ExcelRecalculateFull()
{
  if (IsRecalcOnChange() && m_hComWnd)
    ::PostMessage(m_hComWnd,WM_RECALCULATE_EXCEL,999,999);
}


BEGIN_MESSAGE_MAP(FloatingThread, CWinThread)
	//{{AFX_MSG_MAP(FloatingThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FloatingThread message handlers


BOOL FloatingThread::IsRecalcOnChange()
{
  return ((CEMXApp *)AfxGetApp())->m_bRecalcOnChange;
}



void FloatingThread::SetRecalcOnChange(BOOL bRecalc /*=TRUE*/)
{
  ((CEMXApp *)AfxGetApp())->m_bRecalcOnChange = bRecalc;
}



