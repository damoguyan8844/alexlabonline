#if !defined(AFX_EIMCHATINGWAITER_H__1C26A949_01CA_4735_B5E0_C68843C7940C__INCLUDED_)
#define AFX_EIMCHATINGWAITER_H__1C26A949_01CA_4735_B5E0_C68843C7940C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EIMChatingWaiter.h : header file
//

#include "WaveIn.h"

/////////////////////////////////////////////////////////////////////////////
// CEIMChatingWaiter thread

class CEIMChatingWaiter : public CWinThread
{
	DECLARE_DYNCREATE(CEIMChatingWaiter)
protected:
	CEIMChatingWaiter();           // protected constructor used by dynamic creation

// Attributes
public:
	CWaveIn *m_wi;

// Operations
public:
	void Stop();
	SOCKET m_listen;
	BOOL Listen();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEIMChatingWaiter)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEIMChatingWaiter();

	// Generated message map functions
	//{{AFX_MSG(CEIMChatingWaiter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EIMCHATINGWAITER_H__1C26A949_01CA_4735_B5E0_C68843C7940C__INCLUDED_)
