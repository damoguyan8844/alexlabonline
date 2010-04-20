// FloatBtn.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FloatBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FloatBtn

FloatBtn::FloatBtn()
{
}

FloatBtn::~FloatBtn()
{
}


BEGIN_MESSAGE_MAP(FloatBtn, CButton)
	//{{AFX_MSG_MAP(FloatBtn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FloatBtn message handlers

BOOL FloatBtn::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

char caBuf[256];
sprintf(caBuf,"Number=%d,Hex=%X",pNMHDR->idFrom,pNMHDR->idFrom);
::MessageBox(NULL,caBuf,"DEBUG-TOOLTIPS",MB_OK);

  UINT nID = pNMHDR->idFrom;
  if (pTTT->uFlags & TTF_IDISHWND)
    {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
      if (nID)
        {
          switch (nID)
            {
              case 9873:  // hide btn
                pTTT->lpszText = MAKEINTRESOURCE(IDS_FLOATHIDEBTN);
                break;
              case 9874:  // refresh
                pTTT->lpszText = MAKEINTRESOURCE(IDS_FLOATREFRESHBTN);
                break;
              case 9875:  // Create new
                pTTT->lpszText = MAKEINTRESOURCE(IDS_FLOATCREATENEWBTN);
                break;
              default:
                return FALSE;
                break;
            }

          pTTT->hinst = AfxGetResourceHandle();

          return(TRUE);
        }
    }

  return(FALSE);
}


