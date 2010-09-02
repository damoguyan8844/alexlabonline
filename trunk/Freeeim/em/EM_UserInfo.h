// Author: Alex
// Date: 2007-2-24
// Have not tested.
// 如果程序没有初始化 WSAStartup 程序会有严重的错误
// 程序初始化配置信息

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _USER_INFO
#define _USER_INFO

#define USER_INFO_DEFAULT_GENDER	"男"
#define USER_INFO_DEFAULT_GROUP		"行政部"

// 用户显示名字
#define		DISPLAY_NAME		512
// 用户性别
#define		GENDER				16
// 用户所在分组
#define		GROUP_NAME			256
// 用户电脑名称
#define		HOST_NAME			256
// 用户IP
#define		IP					128


class EM_UserInfo
{
public:
	// 把信息更新到文件
	void UpdateFile();

	~EM_UserInfo();
	EM_UserInfo();
	EM_UserInfo(char *szDisplayName,
			 char *szGender,
			 char *szGroupName);

	void SetDisplayName(LPCTSTR lpszDisplayName);
	BOOL GetDisplayName(char *szDisplayName = NULL);

	void SetGender(char *szGender);
	BOOL GetGender(char *szGender = NULL);

	void SetGroupName(LPCTSTR lpszGroupName);
	BOOL GetGroupName(char *szGroupName = NULL);

	BOOL GetHostName(char *szHostName = NULL);

	char * GetIP(char *szIP = NULL);

	BOOL GetLocalHostName(char *szBuf = NULL);
	BOOL GetLocalHostIP(char *szBuf = NULL);

	friend int GetGenderIndex(char *gender);
private:
	// 用这个函数显示错误
	void ErrorOccured();

	char m_szDisplayName[DISPLAY_NAME];
	char m_szGender[GENDER];
	char m_szGroupName[GROUP_NAME];
	char m_szHostName[HOST_NAME];
	char m_szIP[IP];
};

#endif		//_USER_INFO