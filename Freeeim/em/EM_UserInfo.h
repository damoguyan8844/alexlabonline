// Author: Alex
// Date: 2007-2-24
// Have not tested.
// �������û�г�ʼ�� WSAStartup ����������صĴ���
// �����ʼ��������Ϣ

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _USER_INFO
#define _USER_INFO

#define USER_INFO_DEFAULT_GENDER	"��"
#define USER_INFO_DEFAULT_GROUP		"������"

// �û���ʾ����
#define		DISPLAY_NAME		512
// �û��Ա�
#define		GENDER				16
// �û����ڷ���
#define		GROUP_NAME			256
// �û���������
#define		HOST_NAME			256
// �û�IP
#define		IP					128


class EM_UserInfo
{
public:
	// ����Ϣ���µ��ļ�
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
	// �����������ʾ����
	void ErrorOccured();

	char m_szDisplayName[DISPLAY_NAME];
	char m_szGender[GENDER];
	char m_szGroupName[GROUP_NAME];
	char m_szHostName[HOST_NAME];
	char m_szIP[IP];
};

#endif		//_USER_INFO