#if !defined(AFX_FTPDSOCK_H__4DAE8981_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_)
#define AFX_FTPDSOCK_H__4DAE8981_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "FtpdPi.h"

bool GetItemFromCString(int index,CString* CSTARGET,CString* CSITEM,char sep);
bool FileExists(char *fname);
void FileToCsl(char* finame,CStringList* CSL);

class CFtpd : public CSocket
{
// Attributes
public:

// Operations
public:
	CFtpd();
	virtual ~CFtpd();

// Overrides
public:
	char userfile[256];
	char ipfile[256];
	int m_buf_length;
	CCriticalSection absCrit;
	bool m_specify_data_port;
	UINT m_port_from;
	UINT m_port_to;
	void RemoveClient(CFtpdPi *pBind);
	void AddClient(CFtpdPi* pBind);
	UINT m_port;
	DWORD* lpDW2;
	DWORD* lpDW;
	LPHOSTENT lpHostEnt;
	char HostName[100];
	unsigned long IP;
	char sTmp[32];
	int i;
	CString Citem;
	int result;
//	SECFileSystem fs;
	char buf[256];
	char tmp[32];
	BOOL allow;
	POSITION pos;
	CString CSIP;
	int sl;
//	int m_kbs;
	bool b_speed_limit_in;
	bool b_speed_limit_out;
	int default_speed_in;
	int default_speed_out;
	char m_root_dir[256];
	bool b_auto_choose_ip;
	bool b_switch_pasv_ip;
	bool b_ipchk_pasv;
	bool b_ipchk;
	CString m_servername;
	DWORD dwNumIf;
	DWORD GetLocalNumericIP(int Index);
	CStringList ipList;
	CFtpdPi* ftpdbindsock;
	CList<CFtpdPi*,CFtpdPi*> ClientList;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpd)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFtpd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPDSOCK_H__4DAE8981_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_)
