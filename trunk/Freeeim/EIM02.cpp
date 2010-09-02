// EIM02.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EIM02.h"
#include "EIM02Dlg.h"

#include "XEIM_Database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEIM02App

BEGIN_MESSAGE_MAP(CEIM02App, CWinApp)
	//{{AFX_MSG_MAP(CEIM02App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEIM02App construction

CEIM02App::CEIM02App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEIM02App object

CEIM02App theApp;

/////////////////////////////////////////////////////////////////////////////
// CEIM02App initialization
LONG CEIM02App::EM_UnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
    char szFileName[_MAX_PATH];
    ::GetModuleFileName(NULL, szFileName, _MAX_PATH);
	if (MessageBox(NULL, _T("程序[FreeEIM]遇到问题需要重启，是否重新启动？"), szFileName, MB_YESNO) == IDYES)
	{
		char szCommand[_MAX_PATH];
		sprintf(szCommand, _T("%s %s"), szFileName, "application_restart");
	//	WinExec(szCommand, SW_SHOW);
		PROCESS_INFORMATION piProcInfo; 
		STARTUPINFO siStartInfo;
		// Set up members of STARTUPINFO structure.
		siStartInfo.cb = sizeof(STARTUPINFO); 
		siStartInfo.lpReserved = NULL;
		siStartInfo.lpReserved2 = NULL; 
		siStartInfo.cbReserved2 = 0;
		siStartInfo.lpDesktop = NULL; 
		siStartInfo.dwFlags = 0;

		// Create the child process.
		CreateProcess(
		NULL,
		szCommand,
		NULL, // process security attributes
		NULL, // primary thread security attributes
		0, // handles are inherited
		0, // creation flags
		NULL, // use parent's environment
		NULL, // use parent's current directory
		&siStartInfo, // STARTUPINFO pointer
		&piProcInfo); // receives PROCESS_INFORMATION

		CloseHandle(piProcInfo.hThread);
		CloseHandle(piProcInfo.hProcess);
		// Wait for the processs to finish
	//	DWORD rc = WaitForSingleObject(
	//	piProcInfo.hProcess, // process handle
	//	INFINITE); 
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
BOOL CEIM02App::InitInstance()
{
	BOOL bTryAgain = FALSE;
	// 异常处理
	SetUnhandledExceptionFilter(EM_UnhandledExceptionFilter);

	// 获取命令行参数
	char szCommand[256];
	strcpy(szCommand, AfxGetApp()->m_lpCmdLine);

	// 运用程序重启
	if (! strcmp(szCommand, "application_restart"))
	{
		Sleep(1888);
	//	goto TagTryAgain;
	//	MessageBox(0,0,szCommand,0);
	}

	// 使进程只能用一次
	if( !CWinAppEx::InitInstance( _T("{6A97833B-7441-4711-B96A-C8888D8B8888}") ) )
		return FALSE;

	// 如果是开机启动，就自动最小化到系统托盘
	if (! strcmp(szCommand, "system_tray"))
	{
		//PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		//MessageBox(0, "hELLO", 0, 0);
		bTryAgain = TRUE;
	}
//TagTryAgain:
//	DWORD ns = NETWORK_ALIVE_LAN;
/*	if (! IsNetworkAlive(&ns))
	{
		if (! bTryAgain)
		{
			AfxMessageBox(_T("FreeEIM 错误:\r\n网络连接失败，请检查网络连接是否可用！"));
			return FALSE;
		}
		else
		{
			Sleep(60000);
			bTryAgain = FALSE;
			goto TagTryAgain;
		}
	}*/
	AfxEnableControlContainer();
	AfxInitRichEdit();
	NetworkInitialize();
	OleInitialize(NULL);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	XEIM_Database::StartDB("history.mdb");

	CEIM02Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
