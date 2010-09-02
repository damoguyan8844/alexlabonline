#include "StdAfx.h"
#include "EIM02Dlg.h"
#include "FreeEIM_History.h"
#include "XEIM_Database.h"

FreeEIM_History::FreeEIM_History()
{
	m_pDB = &(XEIM_Database::GetDB());
}

FreeEIM_History::~FreeEIM_History()
{
	m_pDB=NULL;
}

void FreeEIM_History::SaveHistory(LPCTSTR lpszSend, LPCTSTR lpszUser, LPCTSTR lszData)
{
/*	if (! UserPathExist(lpszUser))
	{
		CreateUserPath(lpszUser);
	}*/
	CTime tmNow = CTime::GetCurrentTime();
	CString strSQL;
	strSQL.Format("INSERT INTO [freeeim]([sender],[receiver],[message],[senddate]) VALUES('%s','%s','%s','%s')",
		lpszSend,
		lpszUser,
		lszData,
		tmNow.Format("%Y-%m-%d %H:%M:%S"));

//	AfxMessageBox(strSQL);
	m_pDB->ExecuteCommand(strSQL);
//	AfxMessageBox(GetHistoryPath());
}


void FreeEIM_History::CreateUserPath(LPCTSTR lpszUser)
{
	CFileFind cff;
	if (cff.FindFile(GetHistoryPath()))
	{
		// Do nothing if directory existed.
		
	}
	else
	{
		CreateHistoryPath();
	}
}


CString FreeEIM_History::GetHistoryPath()
{
	CString strPath;
	strPath = GetMainTop()->GetAppDir();
	strPath += "FreeEIM_History\\";

	return strPath;
}


CEIM02Dlg *FreeEIM_History::GetMainTop()
{
	return (CEIM02Dlg*)AfxGetMainWnd();
}


BOOL FreeEIM_History::UserPathExist(LPCTSTR lpszPath)
{
	return TRUE;
}


void FreeEIM_History::CreateHistoryPath()
{
	CreateDir((LPSTR)(LPCTSTR)GetHistoryPath());
}


void FreeEIM_History::CreateDir(char* Path)
{
	char DirName[256];
	char* p = Path;
	char* q = DirName;
	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
}
