#include "StdAfx.h"
#include <strsafe.h>
#include ".\checkconflict.h"
#include "ini2.h"

const CCheckConflict::CONFILCT_MOD_ENTRY	CCheckConflict::s_conflictModuleList[] = {

	_T("Fortress.dll"), _T("ɽ�����籤��"), 1, CCheckConflict::ThirdPartyError,
	_T("KB8964115.log"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("ESPI11.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("asycfilt.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("jscript.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("imon.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("uxtheme.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("nvappfilter.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("dictnt.dll"), _T("δ֪"), 1, CCheckConflict::ThirdPartyError,
	_T("iKeeper.dll"), _T("���ɹ���ϵͳ"), 1, CCheckConflict::ThirdPartyError,

	
	_T("unispim.ime"), _T("�Ϲ����뷨"), 1, CCheckConflict::ThirdPartyError,
	_T("jpwb.IME"), _T("��Ʒ������뷨"), 1, CCheckConflict::ThirdPartyError,
	_T("PINTLGNT.IME"), _T("���뷨"), 1, CCheckConflict::ThirdPartyError,
	_T("winwb86.IME"), _T("Windows������뷨86"), 1, CCheckConflict::ThirdPartyError,
	_T("WINABC.IME"), _T("ABC���뷨"), 1, CCheckConflict::ThirdPartyError,
	

	_T("mshtml.dll"), _T("WebBrowser"), 0, CCheckConflict::WebBrowserProblem,
	_T("urlmon.dll"), _T("WebBrowser"), 0, CCheckConflict::WebBrowserProblem,
	// VC-Huby[2007-02-10]: build 070206 ��2007-02-08�з���350/680�ı�������flash*.ocx ����Crash
	_T("flash.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	_T("flash9.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	_T("flash9b.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	_T("flash8.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	_T("flash8b.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	_T("flash8a.ocx"), _T("WebBrowser"), 0, CCheckConflict::FlashProblem,
	

	_T("Cjktl32.dll"), _T("��ɽ�ʰ�"), 0, CCheckConflict::SimplyPrompt,
	_T("sockspy.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("K7PSWSEn.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("Iefilter.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("tcpipdog.dll"), _T("δ֪"), 0, CCheckConflict::SimplyPrompt,
	_T("tcpipdog0.dll"), _T("δ֪"), 0, CCheckConflict::SimplyPrompt,
	_T("HintSock.dll"), _T("Hintsoft Pubwin ���ɹ���ϵͳ"), 0, CCheckConflict::SimplyPrompt,
	_T("EagleFlt.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("BtFilter.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("MiFilter2.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("WinFilter.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt,
	_T("iaudit.dll"), _T("δ֪"), 1, CCheckConflict::SimplyPrompt
};

#define MODULE_LIST_COUNT ( sizeof(s_conflictModuleList) / (sizeof (CONFILCT_MOD_ENTRY)) )


BOOL CCheckConflict::SimplyPrompt(int iModuleIndex, HMODULE hMod)
{
	TCHAR	szModuleFileName[MAX_PATH];
	szModuleFileName[0] = _T('\0');
	GetModuleFileName(hMod, szModuleFileName, MAX_PATH);
	szModuleFileName[MAX_PATH - 1] = _T('\0');

	//*Warning* �޸���ʾ��ʽʱ��ע������szOutput������ڴ��Ƿ��㹻����Ҫд����ַ���
	LPCTSTR lpcszOutputFormat = _T("eMule ���������������ͻ�������رա�\r\n")
								_T("\r\n")
								_T("������ͻ��������ƣ� [%s]\r\n")
								_T("������ͻ��ģ�飺 [%s]");

	size_t	nOutputMax = _tcslen(lpcszOutputFormat) + MAX_PATH + DESC_MAX + 1;
	TCHAR	*szOutput = new TCHAR[nOutputMax];

	StringCchPrintf(szOutput, nOutputMax, lpcszOutputFormat, s_conflictModuleList[iModuleIndex].szModuleDescription, szModuleFileName);
	MessageBox(NULL, szOutput, _T("eMule�����������ͻ"), MB_OK);

	delete[] szOutput;
	szOutput = NULL;

	return TRUE;
}

BOOL CCheckConflict::ThirdPartyError(int iModuleIndex, HMODULE /*hMod*/)
{
	//*Warning* �޸���ʾ��ʽʱ��ע������szOutput������ڴ��Ƿ��㹻����Ҫд����ַ���
	LPCTSTR lpcszOutputFormat = _T("[%s]�����������eMule�˳���\r\n");

	size_t	nOutputMax = _tcslen(lpcszOutputFormat) + DESC_MAX + 1;
	TCHAR	*szOutput = new TCHAR[nOutputMax];

	StringCchPrintf(szOutput, nOutputMax, lpcszOutputFormat, s_conflictModuleList[iModuleIndex].szModuleDescription);
	MessageBox(NULL, szOutput, _T("�������������eMule�˳�"), MB_OK);

	delete[] szOutput;
	szOutput = NULL;

	return TRUE;
}

BOOL CCheckConflict::WebBrowserProblem(int /*iModuleIndex*/, HMODULE /*hMod*/)
{
	BOOL	bShowBrowser;

	if (! GetIniBool(_T("eMule"), _T("Showbrowser"), &bShowBrowser)
		|| !bShowBrowser)		//WebBrowser has already been disabled.
		return FALSE;

	if (! WriteIniBool(_T("eMule"), _T("Showbrowser"), FALSE))
		return FALSE;

	LPCTSTR lpcszOutput	= _T("eMuleʹ����ҳ�����ʱ��������\r\n")
							_T("\r\n")
							_T("������Ϊ�����������õ���ҳ��������Գ��Խ��������⡣\r\n")
							_T("����������eMule�����Ƿ��Ѿ���������ʹ�á�\r\n");
	MessageBox(NULL, lpcszOutput, _T("eMule��������"), MB_OK);

	return TRUE;
}

BOOL CCheckConflict::FlashProblem(int /*iModuleIndex*/, HMODULE hMod)
{
	BOOL	bShowBrowser;

	if (! GetIniBool(_T("eMule"), _T("Showbrowser"), &bShowBrowser)
		|| !bShowBrowser)		//WebBrowser has already been disabled.
		return FALSE;

	if (! WriteIniBool(_T("eMule"), _T("Showbrowser"), FALSE))
		return FALSE;


	TCHAR	szModuleFileName[MAX_PATH];
	szModuleFileName[0] = _T('\0');
	GetModuleFileName(hMod, szModuleFileName, MAX_PATH);
	szModuleFileName[MAX_PATH - 1] = _T('\0');

	//*Warning* �޸���ʾ��ʽʱ��ע������szOutput������ڴ��Ƿ��㹻����Ҫд����ַ���
	LPCTSTR lpcszOutputFormat = _T("��Ŀǰʹ�õ�Flashģ��Ϊ[%s].\r\n")
								_T("�����ܴ���һЩ���⵼��eMule�˳���\r\n")
								_T("�����Ѿ������������������ܣ������Գ����ٴδ�eMule���Ƿ�������\r\n")
								_T("\r\n")
								_T("���⣬������Flash���µ����°汾֮��\r\n")
								_T("���Գ�����eMule�ġ�ѡ������������ܣ��Լ�������ʹ��eMule��\r\n");

	size_t	nOutputMax = _tcslen(lpcszOutputFormat) + MAX_PATH + 1;
	TCHAR	*szOutput = new TCHAR[nOutputMax];

	StringCchPrintf(szOutput, nOutputMax, lpcszOutputFormat, szModuleFileName);
	MessageBox(NULL, szOutput, _T("Flash��������"), MB_OK);

	delete[] szOutput;
	szOutput = NULL;

	return TRUE;
}


CCheckConflict::CCheckConflict(void)
{
	m_hPsapiDll						= NULL;
	m_pfnGetModuleInformation		= NULL;

	m_hDbgHelpDll					= NULL;
	m_pfnStackWalk64				= NULL;
	m_pfnSymFunctionTableAccess64	= NULL;
	m_pfnSymGetModuleBase64			= NULL;
	m_pfnEnumProcessModules			= NULL;


	m_hPsapiDll = LoadLibrary(_T("psapi.dll"));
	if (NULL != m_hPsapiDll)
	{
		m_pfnGetModuleInformation = (PFN_GetModuleInformation) GetProcAddress(m_hPsapiDll, "GetModuleInformation");
		m_pfnEnumProcessModules = (PFN_EnumProcessModules) GetProcAddress(m_hPsapiDll, "EnumProcessModules");
	}

	m_hDbgHelpDll = LoadLibrary(_T("dbghelp.dll"));
	if (NULL != m_hDbgHelpDll)
	{
		m_pfnStackWalk64 = (PFN_StackWalk64) GetProcAddress(m_hDbgHelpDll, "StackWalk64");
		m_pfnSymFunctionTableAccess64 = (PFUNCTION_TABLE_ACCESS_ROUTINE64) GetProcAddress(m_hDbgHelpDll, "SymFunctionTableAccess64");
		m_pfnSymGetModuleBase64 = (PGET_MODULE_BASE_ROUTINE64) GetProcAddress(m_hDbgHelpDll, "SymGetModuleBase64");
	}
}

CCheckConflict::~CCheckConflict(void)
{
	if (NULL != m_hPsapiDll)
	{
		FreeLibrary(m_hPsapiDll);
		m_hPsapiDll = NULL;
	}
	
	if (NULL != m_hDbgHelpDll)
	{
		FreeLibrary(m_hDbgHelpDll);
		m_hDbgHelpDll = NULL;
	}
}



BOOL CCheckConflict::CheckConflict(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	if (!IsFuncitonsReady())
		return FALSE;

	if (IsCrashInEmuleExe(pExceptionInfo))
		return FALSE;

	if (PreCheck(pExceptionInfo))
		return TRUE;

	if (CheckModules(pExceptionInfo))
		return TRUE;

	if (PostCheck(pExceptionInfo))
		return TRUE;

	//if (UserAnalyseDiy(pExceptionInfo))		//����û��Լ��Ѿ�֪����������ô�����ˣ��Ͳ����������ʹ��󱨸桱�����ˡ�
	//	return TRUE;



	return FALSE;
}

BOOL CCheckConflict::IsFuncitonsReady()
{
	if (NULL == m_pfnGetModuleInformation)
		return FALSE;

	if (NULL == m_pfnStackWalk64)
		return FALSE;
	if (NULL == m_pfnSymFunctionTableAccess64)
		return FALSE;
	if (NULL == m_pfnSymGetModuleBase64)
		return FALSE;
	if (NULL == m_pfnEnumProcessModules)
		return FALSE;


	return TRUE;
}

BOOL CCheckConflict::IsAddressInModule(PVOID pvAddress, HMODULE hModule)
{
	if (NULL == m_pfnGetModuleInformation)
		return FALSE;

	MODULEINFO	mi;
	if ( m_pfnGetModuleInformation(GetCurrentProcess(), hModule, &mi, sizeof(mi)) )
	{
		if (pvAddress >= mi.lpBaseOfDll
			&& (size_t)pvAddress < (size_t)mi.lpBaseOfDll + (size_t)mi.SizeOfImage)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCheckConflict::IsCrashInEmuleExe(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	HMODULE		hEmuleMod = GetModuleHandle(NULL);
	return IsAddressInModule((PVOID)pExceptionInfo->ExceptionRecord->ExceptionAddress, hEmuleMod);
}

BOOL CCheckConflict::PreCheck(struct _EXCEPTION_POINTERS* /*pExceptionInfo*/)
{

	return FALSE;
}

BOOL CCheckConflict::CheckModules(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	int				i, j, jCount;
	HMODULE			hMod;
	CONTEXT			context;
	STACKFRAME64	sf;

	for (i = 0; i < MODULE_LIST_COUNT; i++)
	{
		hMod = GetModuleHandle(s_conflictModuleList[i].szModuleName);
		if (NULL == hMod)
			continue;

		memcpy(&context, pExceptionInfo->ContextRecord, sizeof(context));

		ZeroMemory(&sf, sizeof(sf));
		sf.AddrPC.Offset = context.Eip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = context.Ebp;
		sf.AddrFrame.Mode = AddrModeFlat;

		jCount = (s_conflictModuleList[i].iStackSearchLevel == 0) ? 100/*������100��*/ : s_conflictModuleList[i].iStackSearchLevel;
		for (j = 0; j < jCount; j++)
		{
			if (! m_pfnStackWalk64(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(),
				&sf, NULL, NULL, m_pfnSymFunctionTableAccess64, m_pfnSymGetModuleBase64, NULL))
			{
				break;
			}

			if (IsAddressInModule((PVOID)sf.AddrPC.Offset, hMod))
			{
				return s_conflictModuleList[i].conflictProc(i, hMod);
			}
		}
	}
	
	return FALSE;
}

BOOL CCheckConflict::PostCheck(struct _EXCEPTION_POINTERS* /*pExceptionInfo*/)
{
	HMODULE hMod = LoadLibrary(_T("SysWin64.Sys"));
	if (NULL != hMod)
	{
		TCHAR	szModuleFileName[MAX_PATH];
		szModuleFileName[0] = _T('\0');
		GetModuleFileName(hMod, szModuleFileName, MAX_PATH);
		szModuleFileName[MAX_PATH - 1] = _T('\0');
		
		LPCTSTR lpcszOutputFormat = _T("eMule �� %s ������ͻ�������رա�\r\n");

		size_t	nOutputMax = _tcslen(lpcszOutputFormat) + MAX_PATH + 1;
		TCHAR	*szOutput = new TCHAR[nOutputMax];

		StringCchPrintf(szOutput, nOutputMax, lpcszOutputFormat, szModuleFileName);
		MessageBox(NULL, szOutput, _T("eMule�����������ͻ"), MB_OK);
		return TRUE;
	}

	return FALSE;
}
BOOL CCheckConflict::UserAnalyseDiy(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	//���û����Լ�����һ�¡�
	int		i;
	int		iModCount;
	DWORD	dwNeedBytes;
	TCHAR	szModName[MAX_PATH];
	HMODULE	*arrMods = NULL;
	int		iChoice = IDNO;


	m_pfnEnumProcessModules(GetCurrentProcess(), NULL, 0, &dwNeedBytes);
	iModCount = dwNeedBytes/sizeof(HMODULE);

	arrMods = new HMODULE[iModCount];
	m_pfnEnumProcessModules(GetCurrentProcess(), arrMods, dwNeedBytes, &dwNeedBytes);

	for (i = 0; i < iModCount; i++)
	{
		if (IsAddressInModule(pExceptionInfo->ExceptionRecord->ExceptionAddress, arrMods[i]))
		{
			GetModuleFileName(arrMods[i], szModName, MAX_PATH);

			//	ѯ���û�	<begin>
			LPCTSTR lpcszOutputFormat = _T("������[%s]ģ��������쳣�����\r\n")
										_T("\r\n")
										_T("������Ѿ�֪�����ģ�鱾��������⣬������ѡ���ǡ�����������\r\n")
										_T("���ߣ�������ѡ���񡱣�Ȼ���ʹ��󱨸棬��������Ϊ������һ��������");
				
			size_t	nOutputMax = _tcslen(lpcszOutputFormat) + MAX_PATH + 1;
			TCHAR	*szOutput = new TCHAR[nOutputMax];

			StringCchPrintf(szOutput, nOutputMax, lpcszOutputFormat, szModName);
			iChoice = MessageBox(NULL, szOutput, _T("�������"), MB_YESNO | MB_DEFBUTTON2);

			delete[] szOutput;
			szOutput = NULL;
			//	ѯ���û�	<end>

			break;
		}
	}


	delete[] arrMods;
	arrMods = NULL;

	return (iChoice == IDYES);
}

BOOL CCheckConflict::GetAppPath(LPTSTR lpszBuffer, DWORD dwBufferCch)
{
	TCHAR szModuleFileName[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	if (NULL == GetModuleFileName(NULL, szModuleFileName, MAX_PATH))
		return FALSE;

	_tsplitpath( szModuleFileName, drive, dir, fname, ext );
	
	if ( FAILED(StringCchCopy(lpszBuffer, dwBufferCch, drive)))
		return FALSE;

	if ( FAILED(StringCchCat(lpszBuffer, dwBufferCch, dir)) )
		return FALSE;

	return TRUE;
}

BOOL CCheckConflict::GetIniPathName(LPTSTR lpszBuffer, DWORD dwBufferCch)
{
	if (!GetAppPath(lpszBuffer, dwBufferCch))
		return FALSE;

	if ( FAILED(StringCchCat(lpszBuffer, dwBufferCch, _T("config\\preferences.ini"))) )
		return FALSE;

	return TRUE;
}

BOOL CCheckConflict::GetIniBool(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL *pbValue)
{
	if (NULL == pbValue)
		return FALSE;

	TCHAR	szIniFile[MAX_PATH];
	if (! GetIniPathName(szIniFile, MAX_PATH))
		return FALSE;

	TCHAR szValue[MAX_INI_BUFFER];
	if (0 == GetPrivateProfileString(lpszSection, lpszEntry, _T("0"), szValue, MAX_INI_BUFFER, szIniFile) )
		return FALSE;

	*pbValue = _tstoi(szValue);
	return TRUE;
}
BOOL CCheckConflict::WriteIniBool(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bValue)
{
	TCHAR	szIniFile[MAX_PATH];
	if (! GetIniPathName(szIniFile, MAX_PATH))
		return FALSE;

	TCHAR szBuffer[2];
	szBuffer[0] = bValue ? _T('1') : _T('0');
	szBuffer[1] = _T('\0');
	return WritePrivateProfileString(lpszSection, lpszEntry, szBuffer, szIniFile);
}
