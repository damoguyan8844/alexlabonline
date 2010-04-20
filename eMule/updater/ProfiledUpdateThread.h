#pragma once
#include <afxinet.h>
#include "PrintWriter.h"
#include "FileVersionInfo.h"


// ProfiledUpdateThread
struct FileVersion
{
	UINT		m_iMajor;		//���汾��
	UINT		m_iMinor;		//���汾��
	UINT		m_iUpdate;		//���º�
	UINT		m_iBuild;		//��������
};
class CProfiledUpdateThread : public CWinThread
{
protected:
	DECLARE_DYNCREATE(CProfiledUpdateThread)
	CProfiledUpdateThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CProfiledUpdateThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	void Init(void);

public:
	void	Close(void);

protected:
	BOOL	EstablishConnection(void);
	void	GetCurrentVersion(void);
	void	GetCurrentVersion(LPCTSTR lpszFile,FileVersion & fv);
	int		GetLastError(void);
	DWORD	GetServerType(CString sURL);
	CString	GetUpdateInfo(CString sURL);
	BOOL	Open(CString sURL);
	BOOL	ParseUpdateInfo(CString UpdateInfo);
	void	ProcessHttpRequest(CHttpFile* pFile);
	BOOL	ProcessHttpException(CHttpFile* pFile, CException* pEx);
	void	StartCheckVersion(void);

	void	UpdateDLP();
	BOOL    ParseDLPUpdateInfo(LPCTSTR lpszUpdateInfo);
	void    DownloadDLP();

protected:
	int	m_iDebugLevel;			//0 -> no messages, 1 -> Errors, 2 -> Warnings, 3 -> Information

	CInternetSession*	m_pInternetSession;
	CInternetFile*		m_pInternetFile;
	CHttpConnection*	m_pConnection;

	int					m_iTimeOut;
	CString				m_sURL;
	int					m_iRetryCount;
	long				m_lStartReadingByte;
	CString				m_sPathApp;	
	int					m_iResult;
	int					m_iLastError;

protected:
	UINT		m_iMajor;		//���汾��
	UINT		m_iMinor;		//���汾��
	UINT		m_iUpdate;		//���º�
	UINT		m_iBuild;		//��������

	FileVersion	m_fvAntiLeecher;
	CString	m_szDLPURL;
	CString m_szDLPUpdateInfo;

	CFileVersionInfo  m_fvi;
	CPrintWriter*	m_plog;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnError(WPARAM wParam, LPARAM lParam);
};


