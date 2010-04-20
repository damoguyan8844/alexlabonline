//this file is part of eMule
//Copyright (C)2002-2006 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#pragma once
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"

#include "MemPool/MemPoolMgr.h"
using namespace MemPool;
#include "UPnpMgr.h"	//ADDED by fengwen on 2006/11/01 :

#include "SplashScreen.h"


//#define	DEFAULT_NICK		thePrefs.GetHomepageBaseURL()
#define	DEFAULT_NICK	_T("[CHN][VeryCD]yourname") //VeryCD added by kernel1983 2006.08.01
#define	DEFAULT_TCP_PORT	4662
#define	DEFAULT_UDP_PORT	(DEFAULT_TCP_PORT+10)

#define PORTTESTURL			_T("http://www.emule.org.cn/porttest/?tcpport=%i&udpport=%i")

class CIP2Country; //EastShare - added by AndCycle, IP to Country
class CDLP;	//Xman DLP

class CSearchList;
class CUploadQueue;
class CListenSocket;
class CDownloadQueue;
class CScheduler;
class UploadBandwidthThrottler;
class LastCommonRouteFinder;
class CemuleDlg;
class CClientList;
class CKnownFileList;
class CServerConnect;
class CServerList;
class CSharedFileList;
class CClientCreditsList;
class CFriendList;
class CClientUDPSocket;
class CIPFilter;
class CWebServer;
class CMMServer;
class CAbstractFile;
class CUpDownClient;
class CPeerCacheFinder;
class CFirewallOpener;
class CInternalSocket;	// VC-kernel[2007-01-11]:
class CNatThread;

struct SLogItem;

enum AppState{
	APP_STATE_RUNNING = 0,
   	APP_STATE_SHUTTINGDOWN,
	APP_STATE_DONE
};

class CemuleApp : public CWinApp
{
public:
	CemuleApp(LPCTSTR lpszAppName = NULL);

	CIP2Country*		ip2country; //EastShare - added by AndCycle, IP to Country

	CDLP*				dlp;

	// ZZ:UploadSpeedSense -->
    UploadBandwidthThrottler* uploadBandwidthThrottler;
    LastCommonRouteFinder* lastCommonRouteFinder;
	// ZZ:UploadSpeedSense <--
	CemuleDlg*			emuledlg;
	CClientList*		clientlist;
	CKnownFileList*		knownfiles;
	CServerConnect*		serverconnect;
	CServerList*		serverlist;	
	CSharedFileList*	sharedfiles;
	CSearchList*		searchlist;
	CListenSocket*		listensocket;
	CUploadQueue*		uploadqueue;
	CDownloadQueue*		downloadqueue;
	CClientCreditsList*	clientcredits;
	CFriendList*		friendlist;
	CClientUDPSocket*	clientudp;
	//  added by yunchenn, 2006/11/23
	CNatThread*			natthread;
	CInternalSocket*	internalsocket;    // VC-kernel[2007-01-11]:
	CIPFilter*			ipfilter;
	CWebServer*			webserver;
	CScheduler*			scheduler;
	CMMServer*			mmserver;
	CPeerCacheFinder*	m_pPeerCache;
	CFirewallOpener*	m_pFirewallOpener;

#ifdef _SUPPORT_MEMPOOL
	CMemPoolMgr * m_pMemoryPool; // Added by SearchDream@2006/01/05
#endif

	HANDLE				m_hMutexOneInstance;
	int					m_iDfltImageListColorFlags;
	CFont				m_fontHyperText;
	CFont				m_fontDefaultBold;
	CFont				m_fontSymbol;
	CFont				m_fontLog;
	CBrush				m_brushBackwardDiagonal;
	
	static const UINT	m_nVersionMjr;
	static const UINT	m_nVersionMin;
	static const UINT	m_nVersionUpd;
	static const UINT	m_nVersionBld;
	static const UINT	m_nVCVersionBld;	//Added by thilon on 2006.01.10, for VeryCD版Build号
	
	DWORD				m_dwProductVersionMS;
	DWORD				m_dwProductVersionLS;
	CString				m_strCurVersionLong;
	CString				m_strCurVersionLongDbg;
	UINT				m_uCurVersionShort;
	UINT				m_uCurVersionCheck;
	//Chocobo Start
	//eMule自动更新,added by Chocobo on 2006.07.31
	BOOL                m_bUpdateDownloaded; // 升级文件下载完成标志 --GGSoSo
	CString				m_strCurVersionCheck;
	//Chocobo End
	ULONGLONG			m_ullComCtrlVer;
	AppState			m_app_state; // defines application state for shutdown 
	CMutex				hashing_mut;
	CString*			pstrPendingLink;
	COPYDATASTRUCT		sendstruct;

	CString				m_LocalBindAddress; // added by thilon on 2006.10.18, for BindToAdapter

// Implementierung
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ed2k link functions
	void		AddEd2kLinksToDownload(CString strLinks, int cat);
	void		SearchClipboard();
	void		IgnoreClipboardLinks(CString strLinks) {m_strLastClipboardContents = strLinks;}
	void		PasteClipboard(int cat = 0);
	bool		IsEd2kFileLinkInClipboard();
	bool		IsEd2kServerLinkInClipboard();
	bool		IsEd2kLinkInClipboard(LPCSTR pszLinkType, int iLinkTypeLen);
	LPCTSTR		GetProfileFile()		{ return m_pszProfileName; }

	CString		CreateED2kSourceLink(const CAbstractFile* f);
//	CString		CreateED2kHostnameSourceLink(const CAbstractFile* f);
	CString		CreateKadSourceLink(const CAbstractFile* f);

	// clipboard (text)
	bool		CopyTextToClipboard(CString strText);
	CString		CopyTextFromClipboard();

	void		OnlineSig();
	void		UpdateReceivedBytes(uint32 bytesToAdd);
	void		UpdateSentBytes(uint32 bytesToAdd, bool sentToFriend = false);
	int			GetFileTypeSystemImageIdx(LPCTSTR pszFilePath, int iLength = -1);
	HIMAGELIST	GetSystemImageList() { return m_hSystemImageList; }
	CSize		GetSmallSytemIconSize() { return m_sizSmallSystemIcon; }
	void		CreateBackwardDiagonalBrush();
	void		CreateAllFonts();
	bool		IsPortchangeAllowed();
	bool		IsConnected();
	bool		IsFirewalled();
	bool		CanDoCallback( CUpDownClient *client );
	uint32		GetID();
	uint32		GetPublicIP(bool bIgnoreKadIP = false) const;	// return current (valid) public IP or 0 if unknown
	void		SetPublicIP(const uint32 dwIP);

	void		SetTCPIPValue(DWORD dwValue);		//Added by thilon on 2006.08.07
	DWORD		GetTCPIPVaule(void);				//Added by thilon on 2006.08.07

	LPCTSTR		GetBindAddress() { if (m_LocalBindAddress.IsEmpty()) { return NULL; } else { return m_LocalBindAddress; } }//Added by thilon on 2006.10.18
	void		BindToAddress(LPCTSTR LocalBindAddress = NULL);	//Added by thilon on 2006.10.18, for BindToAdapter

	// because nearly all icons we are loading are 16x16, the default size is specified as 16 and not as 32 nor LR_DEFAULTSIZE
	HICON		LoadIcon(LPCTSTR lpszResourceName, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR) const;
	HICON		LoadIcon(UINT nIDResource) const;
	HBITMAP		LoadImage(LPCTSTR lpszResourceName, LPCTSTR pszResourceType) const;
	HBITMAP		LoadImage(UINT nIDResource, LPCTSTR pszResourceType) const;
	bool		LoadSkinColor(LPCTSTR pszKey, COLORREF& crColor) const;
	bool		LoadSkinColorAlt(LPCTSTR pszKey, LPCTSTR pszAlternateKey, COLORREF& crColor) const;
	CString		GetSkinFileItem(LPCTSTR lpszResourceName, LPCTSTR pszResourceType) const;
	void		ApplySkin(LPCTSTR pszSkinProfile);
	void		EnableRTLWindowsLayout();
	void		DisableRTLWindowsLayout();
	void		UpdateDesktopColorDepth();

	bool		GetLangHelpFilePath(CString& strResult);
	void		SetHelpFilePath(LPCTSTR pszHelpFilePath);
	void		ShowHelp(UINT uTopic, UINT uCmd = HELP_CONTEXT);
	bool		ShowWebHelp(UINT uTopic);

    // Elandal:ThreadSafeLogging -->
    // thread safe log calls
    void			QueueDebugLogLine(bool bAddToStatusBar, LPCTSTR line,...);
    void			QueueDebugLogLineEx(UINT uFlags, LPCTSTR line,...);
    void			HandleDebugLogQueue();
    void			ClearDebugLogQueue(bool bDebugPendingMsgs = false);

	void			QueueLogLine(bool bAddToStatusBar, LPCTSTR line,...);
    void			QueueLogLineEx(UINT uFlags, LPCTSTR line,...);
    void			HandleLogQueue();
    void			ClearLogQueue(bool bDebugPendingMsgs = false);
    // Elandal:ThreadSafeLogging <--

	bool			DidWeAutoStart() { return m_bAutoStart; }

protected:
	bool ProcessCommandline(bool);
	void SetTimeOnTransfer();
	static BOOL CALLBACK SearchEmuleWindow(HWND hWnd, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelp();

	HIMAGELIST m_hSystemImageList;
	CMapStringToPtr m_aExtToSysImgIdx;
	CSize m_sizSmallSystemIcon;

	bool		m_bGuardClipboardPrompt;
	CString		m_strLastClipboardContents;

    // Elandal:ThreadSafeLogging -->
    // thread safe log calls
    CCriticalSection m_queueLock;
    CTypedPtrList<CPtrList, SLogItem*> m_QueueDebugLog;
    CTypedPtrList<CPtrList, SLogItem*> m_QueueLog;
    // Elandal:ThreadSafeLogging <--

	uint32 m_dwPublicIP;
	bool m_bAutoStart;

	
private:
    UINT     m_wTimerRes;

	int		m_dwTCPIPValue;		//Added by thilon on 2006.08.07

//Addded by thilon on 2006.09.24, for UPnP
//upnp_start
public:
	BOOL   m_app_ready;
//upnp_end

//ADDED by fengwen on 2006/11/01 <begin> : 
public:
	CUPnpMgr	m_upnpMgr;
	CWinThread* m_pSplashThread;

//ADDED by fengwen on 2006/11/01 <end> : 
	
	void AddED2KUpdateToDownload(CString strLinks, BOOL isUpdate = TRUE);
};

extern CemuleApp theApp;


//////////////////////////////////////////////////////////////////////////////
// CTempIconLoader

class CTempIconLoader
{
public:
	// because nearly all icons we are loading are 16x16, the default size is specified as 16 and not as 32 nor LR_DEFAULTSIZE
	CTempIconLoader(LPCTSTR pszResourceID, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR);
	CTempIconLoader(UINT uResourceID, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR);
	~CTempIconLoader();

	operator HICON() const{
		return this == NULL ? NULL : m_hIcon;
	}

protected:
	HICON m_hIcon;
};
