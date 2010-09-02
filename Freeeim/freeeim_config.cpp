#include "StdAfx.h"
#include "EIM02Dlg.h"
#include "thirdpartclass/INI.h"
#include "freeeim_config.h"
#include "em/EM_UserInfo.h"

FreeEIM_Config::FreeEIM_Config()
{
	m_strDisplayName = "";
	m_strGroup = "";

	m_pUserInfo = NULL;
	ReadConfig();
}


FreeEIM_Config::~FreeEIM_Config()
{
	if (NULL != m_pUserInfo)
	{
		delete m_pUserInfo;
		m_pUserInfo = NULL;
	}
}


EM_UserInfo *FreeEIM_Config::GetUserInfo()
{
	m_pUserInfo = new EM_UserInfo((LPSTR)(LPCTSTR)m_strDisplayName, "male", (LPSTR)(LPCTSTR)m_strGroup);
	
	return m_pUserInfo;
}


void FreeEIM_Config::ReadConfig()
{
	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();
	// ����û������ļ�
/**/char szFileName[_MAX_PATH];
/**/m_pMainTop->GetProgramDirectory(szFileName);
/**/strcat(szFileName, "freeeim.ini");

	CIniReader iniReader(szFileName);

	if (iniReader.sectionExists("freeeim"))
	{
		// ��ǰ�û���
		m_strDisplayName = iniReader.getKeyValue("display", "freeeim");

		// ��ǰ����
		m_strGroup = iniReader.getKeyValue("group", "freeeim");
	}
	else // ���û�������ļ����½�һ��������ʹ��Ĭ����Ϣ
	{
		m_strDisplayName = GetComputerName();
		m_strGroup = FreeEIM_Default_Group;

		iniReader.setKey((LPCTSTR)m_strDisplayName, "display", "freeeim");
		iniReader.setKey((LPCTSTR)m_strGroup, "group", "freeeim");
	}
}


void FreeEIM_Config::SaveConfig()
{
	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();
	// ����û������ļ�
/**/char szFileName[_MAX_PATH];
/**/m_pMainTop->GetProgramDirectory(szFileName);
/**/strcat(szFileName, "freeeim.ini");

	CIniReader iniReader(szFileName);

	if (iniReader.sectionExists("freeeim"))
	{
		iniReader.setKey((LPCTSTR)m_strDisplayName, "display", "freeeim");
		iniReader.setKey((LPCTSTR)m_strGroup, "group", "freeeim");
	}
}


CString FreeEIM_Config::GetDisplayName()
{
	return m_strDisplayName;
}


CString FreeEIM_Config::GetGroup()
{
	return m_strGroup;
}


CString FreeEIM_Config::GetComputerName()
{
	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();

	// ��ȡ�����������
	return m_pMainTop->GetComputerName();
}
