#include "stdafx.h"
#include "winsock2.h"
#include "stdemdef.h"
#include "dos.h"


DWORD EM_DATA::GetLength()
{
	return (sizeof(DWORD)+  // msg
				sizeof(DWORD) // len
					+len);		// Buffer's len
}

BOOL EM_DATA::AddFileMsg(LPCTSTR lpszFileName, LPCTSTR lpszFileExt)
{
	ASSERT(lpszFileName != NULL);
	ASSERT(lpszFileExt != NULL);

	BOOL ret; 

	LPEM_FILEINFO finfo = new EM_FILEINFO;

	ret = finfo->AddFile(lpszFileName, lpszFileExt);
	if (FALSE == ret)
		return ret;

	msg = EM_FILE;
	len = sizeof(EM_FILEINFO);
	buf = (char*)finfo;

	return ret;
}

char* EM_DATA::GetBuffer()
{
	// 严重内存泄露
	int BufLen = GetLength();
	tmpBuf = new char[BufLen];

	if (NULL == tmpBuf)
	{
		delete buf;
		buf = NULL;
		return NULL;
	}
	else
	{
		memcpy(tmpBuf, (char*)&msg, sizeof(DWORD));
		memcpy(tmpBuf+sizeof(DWORD), (char*)&len, sizeof(DWORD));
		memcpy(tmpBuf+sizeof(DWORD) + sizeof(DWORD), buf, len);
		return tmpBuf;
	}
}

EM_DATA::EM_DATA(char *data, DWORD dwlen)
{
	msg = 0;
	len = 0;
	buf = NULL;
	tmpBuf = NULL;

	if (NULL == data)
		return;

	if (0 == dwlen)
		return;

	memcpy((char*)&msg, data, sizeof(DWORD));
	memcpy((char*)&len, data+sizeof(DWORD), sizeof(DWORD));
	buf = data+sizeof(DWORD)+sizeof(DWORD);
}

EM_DATA::EM_DATA()
{
	msg =		0;
	len =		0;
	buf =		NULL;
	tmpBuf =	NULL;
}
EM_DATA::~EM_DATA()
{
	if(NULL != tmpBuf)
	{
		delete tmpBuf;
		tmpBuf = NULL;
	}
}

char* EM_FILEINFO::FileName()
{
	return szName;
}

char* EM_FILEINFO::FileFullName()
{
	return szFullName;
}
char *EM_FILEINFO::FileExt()
{
	return szExt;
}
BOOL EM_FILEINFO::AddFile(LPCTSTR lpszFileFullName, LPCTSTR lpszFileExt)
{
	ASSERT(lpszFileFullName != NULL);

	CFile file;
	BOOL bResult = file.Open(lpszFileFullName, CFile::modeRead|CFile::shareDenyNone, NULL);
	if(!bResult)
	{
		EM_Error("EM_FILEINFO::AddFile could not open file.", GetLastError());
		return FALSE;
	}
	dwSize = file.GetLength();
	strcpy(szName, (LPCTSTR)file.GetFileName());

	strcpy(szFullName, lpszFileFullName);
	strcpy(szExt, lpszFileExt);

	// 关闭文件
	file.Close();

	return bResult;
}

void EM_Error(CString strError, DWORD dwErrorCode)
{
	CString str;
	str.Format("%s\r\nError Code: %d", strError, dwErrorCode);
	MessageBox(NULL, strError, "EMError", MB_OK|MB_ICONERROR);
	//printf("%s\r\n", (LPCTSTR)str);
}

void NetworkInitialize()
{
	static BOOL g_bnetwork = FALSE;

	if (g_bnetwork == TRUE)
		return;
	WSADATA data;
	WSAStartup(0x0202, &data);
	g_bnetwork = TRUE;
}

BOOL EM_GetHostName_(const char *lpszIP, char *buf)
{
	HOSTENT *host;
	DWORD dwAddr = inet_addr(lpszIP);
	host = gethostbyaddr((char*)&dwAddr, 4, AF_INET);
	if (NULL == host)
	{
		EM_Error("EM_GetHostName failed.", WSAGetLastError());
		return FALSE;
	}
	// 转换成大写统一
	strcpy(buf, strupr(host->h_name));
	return TRUE;
}

BOOL EM_GetIPByPCName(const char *name, char *buf)
{
/*	PROCESS_INFORMATION ProcessInformation;
	STARTUPINFO StartupInfo;
	memset( &StartupInfo, 0, sizeof(StartupInfo) );

	StartupInfo.cb = sizeof(STARTUPINFO);

	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;

	StartupInfo.wShowWindow = SW_HIDE;//SW_SHOWMINIMIZED;

	::CreateProcess(NULL,"c:\\windows\\SYSTEM32\\nbtstat.exe -R",NULL,NULL,true,0,NULL,NULL,&StartupInfo,&ProcessInformation);
	// 要关闭句柄哦
	CloseHandle(ProcessInformation.hThread);
	CloseHandle(ProcessInformation.hProcess);
*/
	hostent *host;
	host = gethostbyname(name);
	if (NULL == host)
	{
	//	EM_Error("EM_GetIPByPCName NULL == host", 0);
		return FALSE;
	}
	strcpy(buf, inet_ntoa(*(LPIN_ADDR)host->h_addr_list[0]));
	return TRUE;
}

void GetProgramDirectory(char *buf)
{
	ASSERT(NULL != buf);
	
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	buf[nIdx+1] = NULL;
}

BOOL IsIPAddress(const char* szip)
{
	int i = 0;
	int pCount = 0;;
	while(szip[i++])
	{
		if (szip[i] == '.')
			pCount ++;
	}
	if (pCount == 3)
		return TRUE;
	else
		return FALSE;
}

// 暂时取消这个函数，稍后以DLL形式替代
void GetMAC(LPCTSTR lpszIP, char *buf)
{
/*	LPBYTE buff;
	char MAC[18];
	DWORD ulMACAddr[2], ulSize = 6;
	HRESULT hr = SendARP(inet_addr(lpszIP), 0, ulMACAddr, &ulSize);
	if (hr == NO_ERROR)
	{
		buff = (LPBYTE)ulMACAddr;
		sprintf(MAC, "%02x-%02x-%02x-%02x-%02x-%02x",
			buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
		memcpy(buf, MAC, 18);
	}
	else
	{*/
		memcpy(buf, "00-00-00-00-00-00", 18);
//	}
}


// 获取文件扩展名，返回的内存必须用 delete 删除
LPSTR GetFileExt(CString filename)
{
	char *buf = new char[128];
	buf[0] = NULL;

	int pos = filename.ReverseFind('.');
	if (pos >= 0)
	{
		strcpy(buf, filename.Right(filename.GetLength() - pos - 1));
	}

	return buf;
}

int EM_ChattingData::s_nOffset = 0;
