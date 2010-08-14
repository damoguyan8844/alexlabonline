// HookHelp.cpp : implementation file
//

#include "stdafx.h"
#include "hook.h"
#include "HookHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHookHelp dialog


CHookHelp::CHookHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CHookHelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHookHelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHookHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHookHelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHookHelp, CDialog)
	//{{AFX_MSG_MAP(CHookHelp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookHelp message handlers
