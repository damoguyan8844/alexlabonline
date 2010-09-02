#if !defined(AFX_WAVEIN_H__507BB2C4_6283_4207_8116_5AD7F8322281__INCLUDED_)
#define AFX_WAVEIN_H__507BB2C4_6283_4207_8116_5AD7F8322281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveIn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaveIn thread
// Â¼ÒôÄ£¿é
class CWaveIn : public CWinThread
{
//	DECLARE_DYNCREATE(CWaveIn)
public:
	CWaveIn();           // protected constructor used by dynamic creation

// Attributes
public:
	DWORD m_dwThread;
	CWaveIn(SOCKET sock);
	SOCKET m_sock;
	BOOL m_bRecording;
	MMRESULT m_ret;

	HWAVEIN m_hwi;

	WAVEHDR m_wh1;
	WAVEHDR m_wh2;

	WAVEFORMATEX m_wfx;

	char *m_buf1;
	char *m_buf2;

// Operations
public:
	BOOL Start(SOCKET s);
	virtual ~CWaveIn();
	BOOL StopRecord();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveIn)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaveIn)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void On_WIM_OPEN(UINT wParam, LONG lParam);
	afx_msg void On_WIM_DATA(UINT wParam, LONG lParam);
	afx_msg void On_WIM_CLOSE(UINT wParam, LONG lParam);
	afx_msg void OnWaveInEnd(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEIN_H__507BB2C4_6283_4207_8116_5AD7F8322281__INCLUDED_)
