// CrashReporter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CrashReporter.h"
#include "CrashReporterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrashReporterApp

BEGIN_MESSAGE_MAP(CCrashReporterApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrashReporterApp ����

CCrashReporterApp::CCrashReporterApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCrashReporterApp ����

CCrashReporterApp theApp;


// CCrashReporterApp ��ʼ��

BOOL CCrashReporterApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	
	if (NULL == m_lpCmdLine
		|| _T('\0') == m_lpCmdLine[0])
		return FALSE;

	CCrashReporterDlg dlg;
	dlg.m_strFileName = m_lpCmdLine;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	if (dlg.m_bAutoRestart)
		ShellExecute(NULL, "open", "eMule.exe", NULL, NULL, SW_SHOW);

	if (!::IsBadReadPtr(dlg.m_pUploadProc, sizeof(CHttpUploadFileProc)))
		WaitForSingleObject(dlg.m_pUploadProc->m_hThread, 30000);

	//	ɾ�����󱨸�
	DeleteFile(dlg.m_strFileName);

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
