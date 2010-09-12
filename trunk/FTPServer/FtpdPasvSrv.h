#if !defined(AFX_FTPPASVSRV_H__4D149542_EF90_11D3_AAE3_0040C787491D__INCLUDED_)
#define AFX_FTPPASVSRV_H__4D149542_EF90_11D3_AAE3_0040C787491D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FtpPasvSrv.h : header file
//


#include "blocksock.h"
/////////////////////////////////////////////////////////////////////////////
// CFtpPasvSrv command target

class CFtpPasvSrv : public CBlockingSocket
{
// Attributes
public:

// Operations
public:
	CFtpPasvSrv();
	virtual ~CFtpPasvSrv();

// Overrides
public:
	bool m_specify_data_port;
	UINT m_port_from;
	UINT m_port_to;
	void AttachSocket(CSocket* dataSkt);
	CString client_ip;
	SOCKADDR_IN lpsockftpd;
	UINT launchSocket();
	UINT m_current_port;
	CFtpPasvSrv(CSocket* dataSkt,UINT from,UINT to);
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPPASVSRV_H__4D149542_EF90_11D3_AAE3_0040C787491D__INCLUDED_)
