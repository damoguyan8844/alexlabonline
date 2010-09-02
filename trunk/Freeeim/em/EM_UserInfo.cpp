// Author: Alex
// Date: 2007-2-10
// 更新日期：2009年12月5日
// 更新内容：去掉一些修改注册表的代码，目的是让程序更绿色；
// 作者：蔡子楠（http://www.freeeim.com/）

#include "StdAfx.h"
#include "winsock2.h"
#include "EM_UserInfo.h"
#include "string.h"
#include "stdemdef.h"


EM_UserInfo::EM_UserInfo()
{
	ZeroMemory(m_szDisplayName, DISPLAY_NAME);
	ZeroMemory(m_szGender, GENDER);
	ZeroMemory(m_szGroupName, GROUP_NAME);
	ZeroMemory(m_szHostName, HOST_NAME);
	ZeroMemory(m_szIP, IP);
}

EM_UserInfo::EM_UserInfo(char *szDisplayName, char *szGender, char *szGroupName)
{
	char buf[1024];
	// 把用户信息COPY过来
	strcpy(m_szDisplayName, szDisplayName);
	strcpy(m_szGender, szGender);
	strcpy(m_szGroupName, szGroupName);
	GetLocalHostName(buf);
	strcpy(m_szHostName, buf);
	GetLocalHostIP(buf);
	strcpy(m_szIP, buf);
	// 写进文件
	UpdateFile();
}

EM_UserInfo::~EM_UserInfo()
{
}

void EM_UserInfo::SetGender(char *szGender)
{
	if (strlen(szGender) >= GENDER)
	{
		// badly error
		::MessageBox(0, "Getder too long", 0, 0);
	}
	strcpy(m_szGender, szGender);
	// 写进文件
	UpdateFile();
}
void EM_UserInfo::SetGroupName(LPCTSTR lpszGroupName)
{
	if (strlen(lpszGroupName) >= DISPLAY_NAME)
	{
		// Badly error
		::MessageBox(0, "Group Name too long", 0, 0);
	}
	ZeroMemory(m_szGroupName, DISPLAY_NAME);
	strcpy(m_szGroupName, lpszGroupName);
	// 写进文件
	UpdateFile();
}

/********************************************************************************/
// 用户信息获取
/********************************************************************************/

BOOL EM_UserInfo::GetDisplayName(char *szDisplayName)
{
	if ( szDisplayName != NULL )
	{
		ASSERT(NULL != m_szDisplayName);
		strcpy(szDisplayName, m_szDisplayName);
	}
	else
	{
		ErrorOccured();
		return FALSE;
	}

	return TRUE;
}
/********************************************************************************/
// 用户信息设置
/********************************************************************************/

void EM_UserInfo::SetDisplayName(LPCTSTR lpszDisplayName)
{
	if (NULL == lpszDisplayName)
	{
		::EM_Error("EM_UserInfo::SetDisplayName szDisplayName == NULL.", 0);
		return;
	}
	if (strlen(lpszDisplayName) >= DISPLAY_NAME)
	{
		// Badly error.
		::MessageBox(0, "Display too long", 0, 0);
	}
	strcpy(m_szDisplayName, lpszDisplayName);
	// 写进文件
	UpdateFile();
}

BOOL EM_UserInfo::GetHostName(char *szHostName)
{
	if ( szHostName != NULL )
		strcpy(szHostName, m_szHostName);
	else
	{
		ErrorOccured();
		return FALSE;
	}
		
	return TRUE;
}
BOOL EM_UserInfo::GetGender(char *szGender)
{
	if ( szGender != NULL )
		strcpy(szGender, m_szGender);
	else
	{
		ErrorOccured();
		return FALSE;
	}

	return TRUE;
}

BOOL EM_UserInfo::GetGroupName(char *szGroupName)
{
	if ( szGroupName != NULL )
		strcpy(szGroupName, m_szGroupName);
	else
	{
		ErrorOccured();
		return FALSE;
	}

	return TRUE;
}

char * EM_UserInfo::GetIP(char *szIP)
{
	if ( szIP != NULL )
		strcpy(szIP, m_szIP);

	return m_szIP;
}

BOOL EM_UserInfo::GetLocalHostName(char *szBuf/* = NULL*/)
{
	ASSERT(NULL != szBuf);

	char buf[512];
	gethostname(buf, 512);

	if (szBuf != NULL)
		strcpy(szBuf, buf);
	else
	{
		ErrorOccured();
		return FALSE;
	}

	return TRUE;
}

BOOL EM_UserInfo::GetLocalHostIP(char *szBuf)
{
	ASSERT(NULL != szBuf);
	char buf[256]={NULL};
	GetLocalHostName(buf);

	char ip[128] = {NULL};
	hostent *host = gethostbyname(buf);

	if (host != NULL)
	{
		sockaddr_in sin;
		memcpy(& sin.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
		strcpy(ip, inet_ntoa(sin.sin_addr));

		if (szBuf != NULL)
			strcpy(szBuf, ip);
		else
		{
			ErrorOccured();
			return FALSE;
		}
		strcpy(m_szIP, ip);
	}
	else
	{
		ErrorOccured();
		return FALSE;
	}
	return TRUE;
}

void EM_UserInfo::UpdateFile()
{
/*	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat(buf, USER_INFO_FILE_NAME);*/

/*	CRegistry regMyReg;
	if ( regMyReg.Open("Software\\FreeEIM", 
		HKEY_CURRENT_USER) )
	{
		// 用户显示的名字
		regMyReg["DisplayName"] = m_szDisplayName;
		regMyReg["Gender"] = m_szGender;
		regMyReg["GroupName"] = m_szGroupName;

		regMyReg.Close();
	}
	else
	{
		::MessageBox(NULL, "Unable to open key!", "Error", MB_OK | MB_ICONHAND);
	}*/
}

void EM_UserInfo::ErrorOccured()
{
	AfxMessageBox("May be some error occured.\n");
}

int GetGenderIndex(char *gender)
{
	char buf[64];
	strcpy(buf, gender);
	strupr(buf);

	if (! strcmp(buf, "FEMALE"))
		return 0;
	else if (! strcmp(buf, "MALE"))
		return 1;
	else
		return -1;
}
