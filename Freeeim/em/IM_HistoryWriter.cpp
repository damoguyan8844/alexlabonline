// IM_HistoryWriter1.cpp : implementation file
//

#include "stdafx.h"
#include "stdemdef.h"
#include "IM_HistoryWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IM_HistoryWriter

IM_HistoryWriter::IM_HistoryWriter()
{
}

IM_HistoryWriter::~IM_HistoryWriter()
{
}

/////////////////////////////////////////////////////////////////////////////
// IM_HistoryWriter message handlers

void IM_HistoryWriter::CreateToday_sFile(LPCTSTR szUserName)
{
	CString fileName;
	CTime today;

	today = CTime::GetCurrentTime();
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
/*	fileName = today.Format("%y-%m-%d");
	fileName += "\\";
	fileName += szUserName;
	fileName += ".imh";*/
	// 建立文件名
	fileName.Format("%shistory\\%s\\%s.imh", buf, today.Format("%y-%m-%d"), szUserName);
//	AfxMessageBox(fileName);
	CFileFind fFind;

	if (fFind.FindFile(fileName))
	{
	//	AfxMessageBox("File existed.");
		m_file.Open(fileName, CFile::modeWrite|CFile::shareDenyNone, NULL);
		m_file.SeekToEnd();
	}
	else
	{
	//	AfxMessageBox("File don't existed.");
		m_file.Open(fileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone, NULL);
	}
}
