#if !defined(AFX_FTPDBINDSOCK_H__4DAE8982_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_)
#define AFX_FTPDBINDSOCK_H__4DAE8982_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "FtpdPasvSrv.h"

class CFtpdPi : public CSocket// CBlockingSocket
{
// Attributes
public:

// Operations
public:
	CFtpdPi();
	virtual ~CFtpdPi();
	void ListEngine(char *vpath, bool longList);
public:
	char ipfile[256];
	char userfile[256];
	char AppDir[256];
	CBlockingSocket* dbsocket;
	void LoadIpRange();
	void addClientIp(char* ip);
	void SafeSend(char* str);
	void SetInStreamSpeed(int kbs);
	void SetOutStreamSpeed(int kbs);
	void ProcessMsg(char* cmd);
	void slashFixUnix(char* path);
	void slashFix(char* path);
	bool matchpass(char* buf,char* uname);
	bool isAllowedIp(char* ip);
	bool validuser(char* buf);
	bool vRmdir(char* path);
	bool vMkdir(char* dir);
	bool vRemove(char* file);
	bool isVpathPresent(char* vpath);
	bool isFilePresent(char* path);
	bool vMoveFile(LPCTSTR lpExistingFileName,LPCTSTR lpNewFileName);
	char* replaceDotInVpath(char* vPath);
	char* fixVpath(char* vPath);
	char* RemoveLeadingSpace(char *str);
	HANDLE h_dbsocket_ready_to_send;
	HANDLE h_inprogress;
	UINT m_port_from_preset;
	UINT m_maxlogin;
	UINT m_ratio;
	UINT m_port_to;
	UINT m_port_from;
	UINT data_port;
	int clientIpCount;
	int m_buf_length_in;
	int m_buf_length_out;
	bool m_no_ipchk;
	bool b_ipchk_pasv;
	bool b_switch_pasv_ip;
	bool b_auto_choose_ip;
	bool m_specify_data_port;
	bool b_use_bsock;
	bool b_speed_limit_out;
	bool b_speed_limit_in;
	bool b_hashmark;
	bool b_debugging;
	bool b_globbing;
	bool b_prompt;
	bool b_bell;
	bool b_verbose;
	bool decided;
	bool bconnected;
	bool bPASS;
	bool bUSER;
	bool bLOGINOK;
	bool b_passive;
	char curCmdth[256];
	char bufcmd_history[256];
	char m_group[32];
	char data_ip[32];
	char sadr[32];
	char vCurrentDir[512];
	char str[512];
	char bufcmd[256];
	char client_ip[32];
	char vRealRoot[256];
	char new_filename[256];
	char old_filename[256];
	char vCurrentDirParent[256];
	char monthStr[13][5];
	char clientIp[32][32];
	unsigned long restart_marker;
	unsigned char* lpDataBuf_in;
	unsigned char* lpDataBuf_out;
	short type;
	int result;
	int ipRoller;
	int kbs;
	int iCLASS;
	LPVOID pFtpd;
	SYSTEMTIME MyTime;
	DWORD dwNumIf;
	SOCKADDR_IN lpsockftpd;
	POSITION pos;
//	SECFileSystem fs;
//	CStringList clienIpList;
//	CString Citem;
//	CString CSIP;
	CStringList ipList;
	char m_password[32];
	char m_username[32];
	CFtpPasvSrv* svSocketSrv;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpdPi)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFtpdPi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CCriticalSection recvCrit;
	bool goParent(char* path);
private:
	int sl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPDBINDSOCK_H__4DAE8982_ED5A_11D3_AAE1_0080C87BF896__INCLUDED_)
