// EM_UserProperty.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "EM_UserProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EM_UserProperty dialog


EM_UserProperty::EM_UserProperty(CWnd* pParent /*=NULL*/)
	: CDialog(EM_UserProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(EM_UserProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void EM_UserProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EM_UserProperty)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EM_UserProperty, CDialog)
	//{{AFX_MSG_MAP(EM_UserProperty)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EM_UserProperty message handlers
