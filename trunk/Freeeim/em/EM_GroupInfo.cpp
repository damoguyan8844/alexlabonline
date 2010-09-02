#include "StdAfx.h"
#include "em_GroupInfo.h"

EM_GroupInfo::EM_GroupInfo()
{
	strcpy(m_szGroupName[GROUP_ADMINISTRATION], _T("行政部"));
	strcpy(m_szGroupName[GROUP_PRODUCTION], _T("生产部"));
	strcpy(m_szGroupName[GROUP_ACCOUNT],_T("财务部"));
	strcpy(m_szGroupName[GROUP_DEVELOPMENT],_T("开发部"));
	strcpy(m_szGroupName[GROUP_PURCHASE],_T("采购部"));
	strcpy(m_szGroupName[GROUP_MARKET],_T("销售部"));
	strcpy(m_szGroupName[GROUP_QULITY],_T("品质部"));
}

EM_GroupInfo::~EM_GroupInfo()
{
}

char * EM_GroupInfo::GetGroupName(int index)
{
	return m_szGroupName[index];
}

int EM_GroupInfo::TotalGroup()
{
	return GROUP_TOTAL;
}

int EM_GroupInfo::GetGroupIndex(char *groupName)
{
	for (int i=0; i<GROUP_TOTAL; i++)
	{
		if (!strcmp(groupName, m_szGroupName[i]))
			return i;
	}
	// Could not find out.
	return -1;
}
