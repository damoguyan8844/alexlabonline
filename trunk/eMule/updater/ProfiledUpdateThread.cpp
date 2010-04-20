// ProfiledUpdateThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "updater.h"
#include "ProfiledUpdateThread.h"
#include <atlbase.h>

#define BUF_SIZE 1024

#ifdef _DEBUG
	#define DEFAULT_DEBUGLEVEL 2	//0 -> no messages, 1 -> Errors, 2 -> Warnings, 3 -> Information
#else
	#define DEFAULT_DEBUGLEVEL 0
#endif
// ProfiledUpdateThread

IMPLEMENT_DYNCREATE(CProfiledUpdateThread, CWinThread)

CProfiledUpdateThread::CProfiledUpdateThread()
{
}

CProfiledUpdateThread::~CProfiledUpdateThread()
{
}

BOOL CProfiledUpdateThread::InitInstance()
{
	m_iDebugLevel = DEFAULT_DEBUGLEVEL;		

	Init();
	StartCheckVersion();
	return TRUE;
}

int CProfiledUpdateThread::ExitInstance()
{
	Close();
	
	CWinThread::ExitInstance();
	return m_iResult;
}

BEGIN_MESSAGE_MAP(CProfiledUpdateThread, CWinThread)
	ON_THREAD_MESSAGE(UM_ERROR, OnError)
END_MESSAGE_MAP()

void CProfiledUpdateThread::Init(void)
{
	m_pInternetFile		= NULL;
	m_pInternetSession	= NULL;
	m_pConnection		= NULL;

	m_iTimeOut			= 3000;
	m_iRetryCount		= 0;
	m_lStartReadingByte = 0;

	m_iLastError		= ERROR_UNKNOWN;

	m_plog				= NULL;

	//����Ĭ�Ϸ���ֵ
	m_iResult = ERROR_UNKNOWN;

	

	if (m_iDebugLevel> 0)
	{
		TCHAR buffer[490];
		::GetModuleFileName(0, buffer, 490);
		LPTSTR pszFileName = _tcsrchr(buffer, L'\\') + 1;
		*pszFileName = L'\0';
		CString appdir(buffer);

		try
		{
			m_plog = new CPrintWriter(appdir + L"logs\\" + _T("updater.log"));
		}
		catch (CException* e)
		{
			e->Delete();
		}

		m_plog->println(_T("-----------------------------------------"));
		m_plog->println(_T("-----------------------------------------"));
		m_plog->println(_T("strating CProfiledUpdateThread Version 1.0.5"));
		m_plog->println(_T("TimeOut: %d"), m_iTimeOut);
		m_plog->println(_T("Log File: %s"), appdir + L"logs\\" + _T("updater.log"));
		m_plog->println(_T("DebugLevel: %d"), m_iDebugLevel);
		m_plog->println(_T("-----------------------------------------"));
	}
}

void CProfiledUpdateThread::Close(void)
{
	if(m_pInternetFile)
	{
		m_pInternetFile->Close();
		delete m_pInternetFile;
	}

	if(m_pInternetSession)
	{
		m_pInternetSession->Close();
		delete m_pInternetSession;
	}

	if(m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
	}

	if(m_plog)
	{
		m_plog->Close();
		delete m_plog;
	}

	m_pInternetFile = NULL;
	m_pInternetSession = NULL;
	m_pConnection = NULL;
	m_plog = NULL;

	
}

BOOL CProfiledUpdateThread::EstablishConnection(void)
{
	CString sServer;
	CString sObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	if(m_pInternetSession)
	{
		if (m_iDebugLevel > 1)
		{
			m_plog->println(_T("warning : \"EstablishConnection\" : InternetSession must be closed."));
		}

		m_pInternetSession->Close();
		delete m_pInternetSession;
	}

	m_iRetryCount = 0;

	try
	{
		m_pInternetSession = new CInternetSession();
	}
	catch (CException* pEx)
	{
		pEx->Delete();

		if (m_iDebugLevel > 0)
		{
			m_plog->println(_T("warning : \"EstablishConnection\" : InternetSession Failed."));
		}
	}

	m_pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, m_iTimeOut);
	m_pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, m_iTimeOut);
	m_pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);

	if (!AfxParseURL(m_sURL, dwServiceType, sServer, sObject, nPort))
	{
		return FALSE;
	}

	try
	{
		m_pConnection = m_pInternetSession->GetHttpConnection(sServer, nPort);

		unsigned long ulFlags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD;	//�������� | ǿ�ƴӷ���������
		if (dwServiceType == AFX_INET_SERVICE_HTTPS)
		{
			ulFlags = ulFlags | INTERNET_FLAG_SECURE;
		}

		// ��һ��Http����
		m_pInternetFile = m_pConnection->OpenRequest(HTTP_VERB_GET, sObject, NULL, 1, NULL, NULL, ulFlags);

		// ��������(������������)
		ProcessHttpRequest((CHttpFile*)m_pInternetFile);
		return TRUE;

	}
	catch(CException* pEx)
	{
		pEx->Delete();
		return FALSE;
	}
	return TRUE;
}

void CProfiledUpdateThread::GetCurrentVersion(void)
{
	TCHAR buffer[490];
	::GetModuleFileName(0, buffer, 490);
	LPTSTR pszFileName = _tcsrchr(buffer, L'\\') + 1;
	*pszFileName = L'\0';

	m_sPathApp = buffer;

	m_sPathApp = m_sPathApp + _T("emule.exe");

	CString path = m_sPathApp;
	CString sFileVersion;
	CString	strSubBuild;		//��������

	if(m_fvi.Create(path))
	{
		sFileVersion = m_fvi.GetFileVersion();
		m_iMajor = m_fvi.GetFileVersion(3);
		m_iMinor = m_fvi.GetFileVersion(2);
		m_iUpdate = m_fvi.GetFileVersion(1);

		AfxExtractSubString(strSubBuild,sFileVersion, 3, '.');
		strSubBuild.Replace(_T(" Unicode"), _T(""));
		m_iBuild = _ttoi(strSubBuild);
	}
}

void CProfiledUpdateThread::GetCurrentVersion(LPCTSTR lpszFile,FileVersion & fv)
{
	CString path = lpszFile;
	CString sFileVersion;
	CString	strSubBuild;

	if(m_fvi.Create(path))
	{
		sFileVersion = m_fvi.GetFileVersion();

		fv.m_iMajor = m_fvi.GetFileVersion(3);
		fv.m_iMinor = m_fvi.GetFileVersion(2);
		fv.m_iUpdate = m_fvi.GetFileVersion(1);

		AfxExtractSubString(strSubBuild,sFileVersion, 3, '.');
		strSubBuild.Replace(_T(" Unicode"), _T(""));
		fv.m_iBuild = _ttoi(strSubBuild);
	}
}

int CProfiledUpdateThread::GetLastError(void)
{
	return m_iLastError;
}

DWORD CProfiledUpdateThread::GetServerType(CString sURL)
{
	CString sServer, sObject, sProxy, sAuthentication;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	if (PathFileExists(sURL))
	{
		dwServiceType = AFX_INET_SERVICE_FILE;
	}
	else
	{
		if (!AfxParseURL(sURL, dwServiceType, sServer, sObject, nPort))
		{
			return AFX_INET_SERVICE_FILE;
		}
	}

	return dwServiceType;
}

CString CProfiledUpdateThread::GetUpdateInfo(CString sURL)
{
	switch (GetServerType(sURL))
	{
	case AFX_INET_SERVICE_HTTP:
	case AFX_INET_SERVICE_HTTPS:
		if (Open(sURL))
		{
			DWORD dwStatusCode = 0;

			((CHttpFile*) m_pInternetFile)->QueryInfoStatusCode(dwStatusCode);

			if (dwStatusCode == HTTP_STATUS_OK)
			{
				char szBuffer[BUFFER_DOWNLOADFILE];
				memset(szBuffer, 0, BUFFER_DOWNLOADFILE);
				int iFileBytes;			//�ļ���С
				int iFileBytesCopied;	//�Ѿ����Ƶ��ļ�
				int iBytesRead;			//��ȡ���ֽ�

				iFileBytes = static_cast<int>(m_pInternetFile->SeekToEnd());

				// ����Ƿ������ļ�
				if (iFileBytes != m_lStartReadingByte)
				{
					//��������ļ�ָ���ƶ����ļ���ʼ
					m_pInternetFile->Seek(m_lStartReadingByte, CFile::begin);
					iFileBytesCopied = m_lStartReadingByte;

					//��ȡ�汾��Ϣ������szBuffer��
					while (iBytesRead = m_pInternetFile->Read(szBuffer, BUFFER_DOWNLOADFILE))
					{
						iFileBytesCopied += iBytesRead;
					}
				}

				CString VersionInfo(szBuffer);

				if(!VersionInfo.IsEmpty())
				{
					return VersionInfo;
				}
			}
		}
		break;
	}

	return NULL;
}

BOOL CProfiledUpdateThread::Open(CString sURL)
{
	if(m_pInternetFile)
	{
		Close();
	}

	m_sURL = sURL;
	return EstablishConnection();
}

BOOL CProfiledUpdateThread::ParseUpdateInfo(CString UpdateInfo)
{
	try
	{
		GetCurrentVersion();

		CString		strSubMajor;		//���汾��
		CString		strSubMinor;		//���汾��
		CString		strSubUpdate;		//���º�
		CString		strSubBuild;		//��������
		CString		strSubURL;			//���ص�ַ

		UINT     iMajor;				//���汾��
		UINT     iMinor;				//���汾��
		UINT	 iUpdate;				//���º�
		UINT     iBuild;				//��������

		CString eMuleUpdateInfo;
		AfxExtractSubString(eMuleUpdateInfo,UpdateInfo,0,'\n');
		if (!eMuleUpdateInfo.IsEmpty() && eMuleUpdateInfo.Right(1) == '\r')
			eMuleUpdateInfo.Remove('\r');

		AfxExtractSubString(m_szDLPUpdateInfo,UpdateInfo,1,'\n');
		if (!m_szDLPUpdateInfo.IsEmpty() && m_szDLPUpdateInfo.Right(1) == '\r')
			m_szDLPUpdateInfo.Remove('\r');

		AfxExtractSubString(strSubMajor,eMuleUpdateInfo, 0, '\\'); //�����ַ������õ����汾��
		AfxExtractSubString(strSubMinor, eMuleUpdateInfo, 1, '\\');//�����ַ������õ����汾��
		AfxExtractSubString(strSubUpdate, eMuleUpdateInfo, 2, '\\');//�����ַ������õ����º�
		AfxExtractSubString(strSubBuild, eMuleUpdateInfo, 3, '\\');//�����ַ������õ���������
		AfxExtractSubString(strSubURL, eMuleUpdateInfo, 4, '\\');//�����ַ������õ����ص�ַ

		iMajor		= _ttoi((LPCTSTR)strSubMajor);				//���汾��
		iMinor		= _ttoi(strSubMinor);						//���汾��
		iUpdate		= _ttoi(strSubUpdate);	//���º�
		iBuild		= _ttoi(strSubBuild);						//��������

		int iCurrent = m_iMajor*10000000 + m_iMinor*100000 + m_iUpdate*10000 + m_iBuild;	//��ǰ�汾
		int iNewVersion = iMajor*10000000 + iMinor*100000 + iUpdate*10000 + iBuild;			//�������˰汾

		//m_plog->println(_T("��ǰ�汾��m_iMajor: %d m_iMinor: %d m_iUpdate%d m_iBuild: %d"), m_iMajor, m_iMinor, m_iUpdate, m_iBuild);
		//m_plog->println(_T("�������˰汾��iMajor: %d iMinor: %d iUpdate%d iBuild: %d"), iMajor, iMinor, iUpdate, iBuild);

		if(iNewVersion - iCurrent > 0)
		{
			char url[BUFFER_DOWNLOADFILE];
			memset(url, 0, BUFFER_DOWNLOADFILE);
			WideCharToMultiByte(CP_ACP, 0, strSubURL, strSubURL.GetLength(), url, BUFFER_DOWNLOADFILE, NULL, NULL);

			HANDLE hMapping;
			LPSTR lpData;

			hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _T("Update"));

			if(hMapping == NULL)
			{
				m_iLastError = ERROR_MEMNOTOPEN;
				return FALSE;
			}

			lpData=(LPSTR)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
			CopyMemory((PVOID)lpData, url, strlen(url));

			if(lpData == NULL)
			{  
				m_iLastError = ERROR_WRITEMEM;
				return FALSE;
			}

			if(strSubURL.IsEmpty())
			{
				return FALSE;
			}

			return TRUE;
		}
		else
		{
			m_iLastError = ERROR_NONEWVERSION;
			return FALSE;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		m_iLastError = ERROR_CHECKFAIL;
	}

	return FALSE;
}

void CProfiledUpdateThread::ProcessHttpRequest(CHttpFile* pFile)
{
	BOOL bRetry = FALSE;

	// ����Http����
	do 
	{
		try
		{
			pFile->SendRequest();						// ���Ϳյ�����
		}
		catch (CException* pEx)
		{
			bRetry = ProcessHttpException(pFile, pEx);	//����Http�쳣
			pEx->Delete();
		}
	}
	while (bRetry && m_iRetryCount++ <= 1);
}

BOOL CProfiledUpdateThread::ProcessHttpException(CHttpFile* pFile, CException* pEx)
{
	BOOL bReturn = FALSE;
	DWORD dwFlags;

	DWORD dwLastError = ((CInternetException*)pEx)->m_dwError;	//��ȡ����

	// ����Ƿ�����Ч���쳣
	if (pEx && pEx->IsKindOf(RUNTIME_CLASS(CInternetException)))
	{
		CInternetException* pException = (CInternetException*) pEx;
		DWORD dwLastError = pException->m_dwError;		//��ȡ����

		switch (dwLastError)
		{
			// ���԰�ȫ����
		case ERROR_INTERNET_INVALID_CA:

			dwFlags = SECURITY_FLAG_IGNORE_CERT_CN_INVALID  | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_REVOCATION | SECURITY_FLAG_IGNORE_WRONG_USAGE;
			pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, sizeof(dwFlags));

			bReturn = true;
			break;

		case ERROR_INTERNET_INVALID_URL:
			bReturn = false;
			break;

		default:
			bReturn = true;
			break;
		}
	}

	return bReturn;
}

void CProfiledUpdateThread::StartCheckVersion(void)
{
	//��ȡ�汾��Ϣ
	CString UpdateInfo;

#ifdef _DEBUG
	UpdateInfo = GetUpdateInfo(HTTP_LOCAL_ADDRES_DEBUG_DOWNLOAD);
#endif

#ifdef _BETA
	UpdateInfo = GetUpdateInfo(HTTP_REMOTE_ADDRES_BEAT_DOWNLOAD);
#endif

#ifdef _RELEASE
	UpdateInfo = GetUpdateInfo(HTTP_REMOTE_ADDRES_REALEASE_DOWNLOAD);
#endif

#ifdef _VCTEST
	UpdateInfo = GetUpdateInfo(HTTP_REMOTE_ADDRES_VCTEST_DOWNLOAD);
#endif

	if(UpdateInfo.IsEmpty())
	{
		PostThreadMessage(UM_ERROR, ERROR_SERVER, 0);
	}

	//�����汾��Ϣ
	if(ParseUpdateInfo(UpdateInfo))
	{
		m_iResult = RESULT_NEWVERSION;
		PostThreadMessage(WM_QUIT, 0, 0);
	}
	else
	{
		int lastError = GetLastError();

		UpdateDLP();

		PostThreadMessage(UM_ERROR, lastError, 0);
	}
}

void CProfiledUpdateThread::UpdateDLP()
{
/*
	CString UpdateInfo;
	UpdateInfo = GetUpdateInfo(HTTP_REMOTE_ADDRES_DLP_DOWNLOAD);
*/
	if (m_szDLPUpdateInfo.IsEmpty())
		return;

	if (ParseDLPUpdateInfo(m_szDLPUpdateInfo))
	{
		DownloadDLP();
	}
	return;
}

BOOL CProfiledUpdateThread::ParseDLPUpdateInfo(LPCTSTR lpszUpdateInfo)
{
	CString szFilePath;

	TCHAR buffer[490];
	::GetModuleFileName(0, buffer, 490);
	LPTSTR pszFileName = _tcsrchr(buffer, L'\\') + 1;
	*pszFileName = L'\0';

	szFilePath = buffer;
	szFilePath = szFilePath + _T("config\\antiLeech.dll.new");

	if (!PathFileExists(szFilePath))
	{
		szFilePath = buffer;
		szFilePath = szFilePath + _T("config\\antiLeech.dll");
	}

	GetCurrentVersion(szFilePath,m_fvAntiLeecher);

	CString		strSubMajor;		//���汾��
	CString		strSubMinor;		//���汾��
	CString		strSubUpdate;		//���º�
	CString		strSubBuild;		//��������
	CString		strSubURL;			//���ص�ַ

	UINT     iMajor;				//���汾��
	UINT     iMinor;				//���汾��
	UINT	 iUpdate;				//���º�
	UINT     iBuild;				//��������

	AfxExtractSubString(strSubMajor,lpszUpdateInfo, 0, '\\');	//�����ַ������õ����汾��
	AfxExtractSubString(strSubMinor, lpszUpdateInfo, 1, '\\');	//�����ַ������õ����汾��
	AfxExtractSubString(strSubUpdate, lpszUpdateInfo, 2, '\\');	//�����ַ������õ����º�
	AfxExtractSubString(strSubBuild, lpszUpdateInfo, 3, '\\');	//�����ַ������õ���������
	AfxExtractSubString(strSubURL, lpszUpdateInfo, 4, '\\');	//�����ַ������õ����ص�ַ

	iMajor		= _ttoi((LPCTSTR)strSubMajor);				//���汾��
	iMinor		= _ttoi(strSubMinor);						//���汾��
	iUpdate		= _ttoi(strSubUpdate);						//���º�
	iBuild		= _ttoi(strSubBuild);						//��������

	int iCurrent = m_fvAntiLeecher.m_iMajor*10000000 + m_fvAntiLeecher.m_iMinor*100000 + m_fvAntiLeecher.m_iUpdate*10000 + m_fvAntiLeecher.m_iBuild;	//��ǰ�汾
	int iNewVersion = iMajor*10000000 + iMinor*100000 + iUpdate*10000 + iBuild;																			//�������˰汾

	if(iNewVersion - iCurrent > 0)
	{
		m_szDLPURL = strSubURL;
		return TRUE;
	}
	return FALSE;
}

void CProfiledUpdateThread::DownloadDLP()
{
	switch (GetServerType(m_szDLPURL))
	{
	case AFX_INET_SERVICE_HTTP:
	case AFX_INET_SERVICE_HTTPS:
		if (Open(m_szDLPURL))
		{
			DWORD dwStatusCode = 0;

			((CHttpFile*) m_pInternetFile)->QueryInfoStatusCode(dwStatusCode);

			if (dwStatusCode == HTTP_STATUS_OK)
			{
			
				CString szFilePath;
				TCHAR buffer[490];
				::GetModuleFileName(0, buffer, 490);
				LPTSTR pszFileName = _tcsrchr(buffer, L'\\') + 1;
				*pszFileName = L'\0';

				szFilePath = buffer;
				szFilePath = szFilePath + _T("config\\antiLeech.dll.new");

				FILE * fp = NULL;
				fp = _tfopen(szFilePath,_T("wb"));
				if (!fp)
				{
					TCHAR szAppData[1024];


					OSVERSIONINFO os;
					os.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
					::GetVersionEx(&os);
					if(os.dwMajorVersion >= 6)//StrStrI(szFilePath,_T("C:\\Program Files")) != NULL
					{
						//SHGetSpecialFolderPath(NULL,szAppData,CSIDL_LOCAL_APPDATA,0);
						//szFilePath = szAppData;
						//szFilePath +=  _T("\\eMule\\config\\antiLeech.dll.new");

						CRegKey Reg;
						if (Reg.Open(HKEY_CURRENT_USER,_T("Software\\eMule"),KEY_READ) == ERROR_SUCCESS)
						{
							ULONG RetLong = 1024;
							if (Reg.QueryStringValue(_T("ConfigDir"),szAppData,&RetLong) == ERROR_SUCCESS)
							{
								szFilePath = szAppData;
								szFilePath +=  _T("\\config\\antiLeech.dll.new");
								fp = _tfopen(szFilePath,_T("wb"));
							}
						}
					}
				}

				if (!fp)
				{
					MessageBox(NULL,szFilePath,_T("Create File Error!"),MB_OK);
					return;
				}

				char szBuffer[BUFFER_DOWNLOADFILE];
				memset(szBuffer, 0, BUFFER_DOWNLOADFILE);

				int iFileBytes;				//�ļ���С
				int iFileBytesCopied = 0;	//�Ѿ����Ƶ��ļ�
				int iBytesRead = 0;			//��ȡ���ֽ�

				iFileBytes = static_cast<int>(m_pInternetFile->GetLength());

				m_pInternetFile->Seek(0L, CFile::begin);

				while (iBytesRead = m_pInternetFile->Read(szBuffer, BUFFER_DOWNLOADFILE))
				{
					fwrite(szBuffer,sizeof(char),iBytesRead,fp);
					iFileBytesCopied += iBytesRead;
				}
				fflush(fp);
				fclose(fp);
			}
		}
		break;
	}
	return;
}

// ProfiledUpdateThread ��Ϣ�������
void CProfiledUpdateThread::OnError(WPARAM wParam, LPARAM lParam)
{
	int iError = (int)wParam;

	switch (iError)
	{
	case ERROR_NONEWVERSION:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("No new version"));
		}
		
		m_iResult = ERROR_NONEWVERSION;
		break;
	case ERROR_NOCONNECTION:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("No connection"));
		}

		m_iResult = ERROR_NOCONNECTION;
		break;
	case ERROR_SERVER:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Do not connection to Server"));
		}

		m_iResult = ERROR_SERVER;
		break;
	case ERROR_CHECKFAIL:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Check failed"));
		}

		m_iResult = ERROR_CHECKFAIL;
		break;
	case ERROR_MEMNOTCREATE:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Do not create memory"));
		}

		m_iResult = ERROR_MEMNOTCREATE;
		break;
	case ERROR_MEMNOTOPEN:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Do not open memory"));
		}

		m_iResult = ERROR_MEMNOTOPEN;
		break;
	case ERROR_MEMNOTMAP:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Do not map memory"));
		}

		m_iResult = ERROR_MEMNOTMAP;
		break;
	case ERROR_WRITEMEM:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Do not map memory"));
		}

		m_iResult = ERROR_WRITEMEM;
		break;
	case ERROR_UNKNOWN:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Unknow Error"));
		}

		m_iResult = ERROR_UNKNOWN;
		break;
	default:
		if(m_iDebugLevel > 0)
		{
			m_plog->println(_T("error %d: %s"), iError, _T("Other Error"));
		}

		m_iResult = iError;
		break;
	}
	PostThreadMessage(WM_QUIT, 0, 0);
}
