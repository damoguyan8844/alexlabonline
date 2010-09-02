#if !defined(AFX_IM_HISTORYWRITER1_H__8F933B0F_C54B_415C_B7A2_5061ADFB5284__INCLUDED_)
#define AFX_IM_HISTORYWRITER1_H__8F933B0F_C54B_415C_B7A2_5061ADFB5284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IM_HistoryWriter1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IM_HistoryWriter window

class IM_HistoryWriter
{
// Construction
public:
	IM_HistoryWriter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IM_HistoryWriter)
	//}}AFX_VIRTUAL

// Implementation
public:
	void CloseFile();
	void AddHistory(LPCTSTR szHistory);
	void CreateToday_sFile(LPCTSTR szUserName);
	CStdioFile m_file;
	virtual ~IM_HistoryWriter();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IM_HISTORYWRITER1_H__8F933B0F_C54B_415C_B7A2_5061ADFB5284__INCLUDED_)
