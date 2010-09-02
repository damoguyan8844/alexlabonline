#if !defined(AFX_WAVEOUT_H__236F3DA0_6987_4EAC_903D_70EF1310303F__INCLUDED_)
#define AFX_WAVEOUT_H__236F3DA0_6987_4EAC_903D_70EF1310303F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveOut.h : header file
//

#include "mmsystem.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveOut thread

class CWaveOut : public CWinThread
{
	DECLARE_DYNCREATE(CWaveOut)
protected:
	CWaveOut();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	BOOL Stop();
	BOOL SetIP(LPCTSTR ip);
//	BOOL ReadAndPlay();
	BOOL ReadAndPlay(LPWAVEHDR hdr);
	DWORD m_dwTID;
	HWAVEOUT m_hwo;
	WAVEHDR m_hdr1, m_hdr2;
	SOCKET m_sock;
	static char m_szip[256];
	BOOL Start();
	PCMWAVEFORMAT m_pcm;
	DWORD m_dwSize;
	DWORD m_curSize;
	HMMIO m_hmmio;
	BOOL m_bChatting;
//	BOOL OpenFile(LPSTR name);
	BOOL Connect(LPSTR ip);
	BOOL SendRequest();
	BOOL GetWavePCM();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveOut)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWaveOut();

	// Generated message map functions
	//{{AFX_MSG(CWaveOut)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void On_MM_Done(UINT wParam, LONG lParam);
	afx_msg void On_MM_Open(UINT wParam, LONG lParam);
	afx_msg void On_MM_Close(UINT wParam, LONG lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEOUT_H__236F3DA0_6987_4EAC_903D_70EF1310303F__INCLUDED_)
