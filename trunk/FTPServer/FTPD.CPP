// ftpd.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ftpd.h"
#include "ftpdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpdApp

BEGIN_MESSAGE_MAP(CFtpdApp, CWinApp)
	//{{AFX_MSG_MAP(CFtpdApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpdApp construction

CFtpdApp::CFtpdApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFtpdApp object

CFtpdApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFtpdApp initialization

BOOL CFtpdApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFtpdDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
