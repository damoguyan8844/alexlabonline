#if !defined(AFX_EM_FILERECEIVING_H__0338F6E6_C737_4CEB_A098_3A0733EC05CC__INCLUDED_)
#define AFX_EM_FILERECEIVING_H__0338F6E6_C737_4CEB_A098_3A0733EC05CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EM_FileReceiving.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// EM_FileReceiving window

class EM_FileReceiving
{
// Construction
public:
	EM_FileReceiving();
	void Run(CWnd *pWnd);

	static DWORD WINAPI ListeningThread(LPVOID lParam);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EM_FileReceiving)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Close();
	static BOOL m_bFileReceivingThread;
	virtual ~EM_FileReceiving();

	// Generated message map functions
private:
	HANDLE m_hThread;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EM_FILERECEIVING_H__0338F6E6_C737_4CEB_A098_3A0733EC05CC__INCLUDED_)
