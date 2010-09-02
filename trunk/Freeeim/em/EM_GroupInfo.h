// Author: Alex
// Date: 2007-2-22
// This class have not tested.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef _GROUP_INFO
#define _GROUP_INFO

class EM_GroupInfo
{
public:
	enum
	{
		GROUP_TOTAL = 7,
		GROUP_ADMINISTRATION = 0,	/* = "Administration" */
		GROUP_PRODUCTION = 1,		/* = "Production" */
		GROUP_ACCOUNT = 2,			/* = "Account" */
		GROUP_DEVELOPMENT = 3,		/* = "Development" */
		GROUP_PURCHASE = 4,			/* = "Purchase" */
		GROUP_MARKET = 5,			/* = "Market" */
		GROUP_QULITY = 6			/* = "Qulity" */
	};
public:
	int GetGroupIndex(char *groupName);
	char * GetGroupName(int index);
	int TotalGroup();
	EM_GroupInfo();
	~EM_GroupInfo();
	friend int GetGroupIndex(char *groupName);
private:
	char m_szGroupName[GROUP_TOTAL][128];
};

#endif		//_GROUP_INFO