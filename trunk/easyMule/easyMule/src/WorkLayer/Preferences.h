/* 
 * $Id: Preferences.h 18513 2010-03-24 09:41:45Z huby $
 * 
 * this file is part of eMule
 * Copyright (C)2002-2006 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once
#include "AntiVirusIDs.h"

#ifdef _FOREIGN_VERSION
	#define	DEFAULT_NICK	_T("easyMule.com") //VC-dgkang 2008年7月10日
#else
	#define	DEFAULT_NICK	_T("[CHN][VeryCD]yourname") //VeryCD added by kernel1983 2006.08.01
#endif

#define DFLT_MAXHALFOPEN		256
#define RANGE_MAXHALFOPEN_MIN	9
#define RANGE_MAXHALFOPEN_MAX	1024

#define	DFLT_MAXCONPERFIVE		250
#define RANGE_MAXCONN_PER5_MIN	10
#define RANGE_MAXCONN_PER5_MAX	1000

//EastShare Start - added by AndCycle, IP to Country
enum IP2CountryNameSelection{

	IP2CountryName_DISABLE = 0,
	IP2CountryName_SHORT,
	IP2CountryName_MID,
	IP2CountryName_LONG
};
//EastShare End - added by AndCycle, IP to Country

const CString strDefaultToolbar = _T("129903040599091011");	//Changed by Dash on 2006.08.17, 修改工具栏默认样式


enum EViewSharedFilesAccess{
	vsfaEverybody = 0,
	vsfaFriends = 1,
	vsfaNobody = 2
};

enum ENotifierSoundType{
	ntfstNoSound = 0,
	ntfstSoundFile = 1,
	ntfstSpeech = 2
};

enum EDefaultDirectory{
	EMULE_CONFIGDIR = 0,
	EMULE_MODULEDIR = 1,
	EMULE_INCOMINGDIR = 2,
	EMULE_LOGDIR = 3,
	EMULE_ADDLANGDIR = 4, // directories with languages installed by the eMule (parent: EMULE_EXPANSIONDIR)
	EMULE_INSTLANGDIR = 5, // directories with languages installed by the user or installer (parent: EMULE_EXECUTEABLEDIR)
	EMULE_WEBSERVERDIR = 6,
	EMULE_SKINDIR = 7,
	EMULE_DATABASEDIR = 8, // the parent directory of the incoming/temp folder
	EMULE_CONFIGBASEDIR = 9, // the parent directory of the config folder 
	EMULE_EXECUTEABLEDIR = 10, // assumed to be not writeable (!)
	EMULE_TOOLBARDIR = 11,
	EMULE_EXPANSIONDIR = 12, // this is a base directory accessable for all users for things eMule installs
	EMULE_UPDATEDIR = 13,	//
	EMULE_METBAKDIR = 14,
	EMULE_SECURITYDIR = 15,
	EMULE_TEMPDIR = 16
};

enum EConnectionType{
	CONN_TYPE_UNKNOWN,
	CONN_TYPE_CUSTOMIZE,
	CONN_TYPE_512K,
	CONN_TYPE_1M,
	CONN_TYPE_2M,
	CONN_TYPE_8M,
	CONN_TYPE_LAN
};
struct SConnTypeInfo
{
	BOOL bLimit;
	UINT uDownCapacity;
	UINT uUpCapacity;
};
extern SConnTypeInfo g_arrConnTypeInfo[];

enum EToolbarLabelType;
enum ELogFileFormat;

// DO NOT EDIT VALUES like making a uint16 to uint32, or insert any value. ONLY append new vars
#pragma pack(1)
struct Preferences_Ext_Struct{
	uint8	version;
	uchar	userhash[16];
	WINDOWPLACEMENT EmuleWindowPlacement;
};
#pragma pack()

// deadlake PROXYSUPPORT
struct ProxySettings{
	uint16		type;
	uint16		port;
	CStringA	name;
	CStringA	user;
	CStringA	password;
	bool		EnablePassword;
	bool		UseProxy;
};

#pragma pack(1)
struct Category_Struct{
	CString	strIncomingPath;
	CString	strTitle;
	CString	strComment;
	DWORD	color;
	UINT	prio;
	CString autocat;
	CString	regexp;
	int		filter;
	bool	filterNeg;
	bool	care4all;
	bool	ac_regexpeval;
	BOOL    downloadInAlphabeticalOrder; // ZZ:DownloadManager
};
#pragma pack()

#pragma pack(1)
typedef struct _STAT_INSTALL_PACK
{
	BYTE	byOpCode;
	DWORD	dwInstallTime;
	DWORD	dwInstallTickCount;
	BYTE	byUserHash[ 16 ];

}STAT_INSTALL_PACK, *LPSTAT_INSTALL_PACK;

typedef struct _STAT_STARTUP_INFO
{
	BYTE byOpCode;
	BYTE byOpType;
	BYTE rgbyUserHash[16];
	DWORD dwTick;
	DWORD dwTime;
	DWORD dwStartupCount;
}STAT_STARTUP_INFO, *LPSTAT_STARTUP_INFO;

#pragma pack()

class CPreferences
{
	static	uint32	m_uUploadClients;	//Added by thilon on 2006.08.08, 固定上传线程
	static  uint32		m_uPowerReleaseFilePriority;
public:

	static	uint32	GetUploadClients()					{return m_uUploadClients;}	//Added by thilon on 2006.08.08, 固定上传线程
	static	uint32	GetPowerReleaseFilePriority()		{return m_uPowerReleaseFilePriority;}

	//Added by thilon on 2006.10.19, for ICSFirewall
	//static UINT		m_uICFSupport;
	//static bool		m_bICFClearOnClose;

	//static	void	SetICFSupport(BOOL on)			{ m_uICFSupport = on; }
	//static	bool	GetICFSupport()					{ return m_uICFSupport == TRUE; }
	//static	bool	GetICFSupportFirstTime()		{ return m_uICFSupport == 2; }

	//static	bool	GetICFClearOnClose()			{ return m_bICFClearOnClose; }

	//EastShare Start - added by AndCycle, IP to Country
	static IP2CountryNameSelection	m_iIP2CountryNameMode;
	static bool		m_bIP2CountryShowFlag;
	//EastShare - added by AndCycle, IP to Country
	//EastShare - added by AndCycle, IP to Country
	IP2CountryNameSelection	GetIP2CountryNameMode()	{return m_iIP2CountryNameMode;}
	bool	IsIP2CountryShowFlag()				{return m_bIP2CountryShowFlag;}
	//EastShare - added by AndCycle, IP to Country
	static	CString	strNick;
	// ZZ:UploadSpeedSense -->
	static	uint16	minupload;
	// ZZ:UploadSpeedSense <--
	static	uint16	maxupload;
	static	uint16	maxdownload;
	static	LPCSTR	m_pszBindAddrA;
	static	CStringA m_strBindAddrA;
	static	LPCWSTR	m_pszBindAddrW;
	static	CStringW m_strBindAddrW;
	static	uint16	port;
	static	uint16	udpport;
	static	uint16	nServerUDPPort;
	static	UINT	maxconnections;
	static	UINT	maxhalfconnections;
	static	UINT	maxL2Lhalfconnections;
	static	bool	m_bConditionalTCPAccept;
	static	bool	reconnect;
	static	bool	m_bUseServerPriorities;
	static	bool	m_bUseUserSortedServerList;
	static  CString	m_strUpdateDir;	//Added by thilon on 2006.12.30
	static  CString m_strPartFileCommentUrl; //the VeryCD comment url address
	static  CString m_strFinishedFileCommentUrl;

	static  CStringArray m_aFilterExt;
	static  CStringArray m_aFilterWCExt;

	static	CString	m_strIncomingDir;		//Added by thilon on 2007.05.28
	static  CString m_strSecurityDir;
	static	CStringArray	tempdir;
	static	bool	ICH;
	static	bool	m_bAutoUpdateServerList;
	static	bool	updatenotify;
	static	bool	mintotray;
	static	bool	autoconnect;
	static	bool	m_bAutoConnectToStaticServersOnly; // Barry
	static	bool	autotakeed2klinks;	   // Barry
	static	bool	addnewfilespaused;	   // Barry
	static	UINT	depth3D;			   // Barry
	static	bool	m_bEnableMiniMule;
	static	int		m_iStraightWindowStyles;
	static	bool	m_bRTLWindowsLayout;
	static	CString	m_strSkinProfile;
	static	CString	m_strSkinProfileDir;
	static	bool	m_bAddServersFromServer;
	static	bool	m_bAddServersFromClients;
	static	UINT	maxsourceperfile;
	static	UINT	trafficOMeterInterval;
	static	UINT	statsInterval;
	static	uchar	userhash[16];
	static	WINDOWPLACEMENT EmuleWindowPlacement;
	static	int		maxGraphDownloadRate;
	static	int		maxGraphUploadRate;
	static	uint32	maxGraphUploadRateEstimated;
	static	bool	beepOnError;
	static	bool	confirmExit;
	static	DWORD	m_adwStatsColors[15];
	static  bool	m_bIconflashOnNewMessage;

	static	bool	splashscreen;
	//static  bool	m_bUpdateDownloading;	// Added by thilon on 2007.01.15
	static  BOOL	m_bUpdate;				//Added bythilon on 2007.01.11
	//static  bool	m_bUpdateDownloaded;		//Added by thilon on 2007.01.15
	static  CString m_UpdateHash;

	static	bool	filterLANIPs;
	static	bool	m_bAllocLocalHostIP;
	static	bool	onlineSig;
	static  bool    m_bShowBroswer;// Added by thilon on 2006.08.03
	static	bool	m_bShowNewTaskDlg;// VC-kernel[2007-03-17]:
	static	WORD	m_wHotKeyValue;
	static	WORD	m_wHotKeyMod;

	// -khaos--+++> Struct Members for Storing Statistics

	// Saved stats for cumulative downline overhead...
	static	uint64	cumDownOverheadTotal;
	static	uint64	cumDownOverheadFileReq;
	static	uint64	cumDownOverheadSrcEx;
	static	uint64	cumDownOverheadServer;
	static	uint64	cumDownOverheadKad;
	static	uint64	cumDownOverheadTotalPackets;
	static	uint64	cumDownOverheadFileReqPackets;
	static	uint64	cumDownOverheadSrcExPackets;
	static	uint64	cumDownOverheadServerPackets;
	static	uint64	cumDownOverheadKadPackets;

	// Saved stats for cumulative upline overhead...
	static	uint64	cumUpOverheadTotal;
	static	uint64	cumUpOverheadFileReq;
	static	uint64	cumUpOverheadSrcEx;
	static	uint64	cumUpOverheadServer;
	static	uint64	cumUpOverheadKad;
	static	uint64	cumUpOverheadTotalPackets;
	static	uint64	cumUpOverheadFileReqPackets;
	static	uint64	cumUpOverheadSrcExPackets;
	static	uint64	cumUpOverheadServerPackets;
	static	uint64	cumUpOverheadKadPackets;

	// Saved stats for cumulative upline data...
	static	uint32	cumUpSuccessfulSessions;
	static	uint32	cumUpFailedSessions;
	static	uint32	cumUpAvgTime;
	// Cumulative client breakdown stats for sent bytes...
	static	uint64	cumUpData_EDONKEY;
	static	uint64	cumUpData_EDONKEYHYBRID;
	static	uint64	cumUpData_EMULE;
	static	uint64	cumUpData_MLDONKEY;
	static	uint64	cumUpData_AMULE;
	static	uint64	cumUpData_EMULECOMPAT;
	static	uint64	cumUpData_SHAREAZA;
	// Session client breakdown stats for sent bytes...
	static	uint64	sesUpData_EDONKEY;
	static	uint64	sesUpData_EDONKEYHYBRID;
	static	uint64	sesUpData_EMULE;
	static	uint64	sesUpData_MLDONKEY;
	static	uint64	sesUpData_AMULE;
	static	uint64	sesUpData_EMULECOMPAT;
	static	uint64	sesUpData_SHAREAZA;

	// Cumulative port breakdown stats for sent bytes...
	static	uint64	cumUpDataPort_4662;
	static	uint64	cumUpDataPort_OTHER;
	static	uint64	cumUpDataPort_PeerCache;
	// Session port breakdown stats for sent bytes...
	static	uint64	sesUpDataPort_4662;
	static	uint64	sesUpDataPort_OTHER;
	static	uint64	sesUpDataPort_PeerCache;

	// Cumulative source breakdown stats for sent bytes...
	static	uint64	cumUpData_File;
	static	uint64	cumUpData_Partfile;
	// Session source breakdown stats for sent bytes...
	static	uint64	sesUpData_File;
	static	uint64	sesUpData_Partfile;

	// Saved stats for cumulative downline data...
	static	uint32	cumDownCompletedFiles;
	static	uint32	cumDownSuccessfulSessions;
	static	uint32	cumDownFailedSessions;
	static	uint32	cumDownAvgTime;

	// Cumulative statistics for saved due to compression/lost due to corruption
	static	uint64	cumLostFromCorruption;
	static	uint64	cumSavedFromCompression;
	static	uint32	cumPartsSavedByICH;

	// Session statistics for download sessions
	static	uint32	sesDownSuccessfulSessions;
	static	uint32	sesDownFailedSessions;
	static	uint32	sesDownAvgTime;
	static	uint32	sesDownCompletedFiles;
	static	uint64	sesLostFromCorruption;
	static	uint64	sesSavedFromCompression;
	static	uint32	sesPartsSavedByICH;

	// Cumulative client breakdown stats for received bytes...
	static	uint64	cumDownData_EDONKEY;
	static	uint64	cumDownData_EDONKEYHYBRID;
	static	uint64	cumDownData_EMULE;
	static	uint64	cumDownData_MLDONKEY;
	static	uint64	cumDownData_AMULE;
	static	uint64	cumDownData_EMULECOMPAT;
	static	uint64	cumDownData_SHAREAZA;
	static	uint64	cumDownData_URL;
	// Session client breakdown stats for received bytes...
	static	uint64	sesDownData_EDONKEY;
	static	uint64	sesDownData_EDONKEYHYBRID;
	static	uint64	sesDownData_EMULE;
	static	uint64	sesDownData_MLDONKEY;
	static	uint64	sesDownData_AMULE;
	static	uint64	sesDownData_EMULECOMPAT;
	static	uint64	sesDownData_SHAREAZA;
	static	uint64	sesDownData_URL;

	// Cumulative port breakdown stats for received bytes...
	static	uint64	cumDownDataPort_4662;
	static	uint64	cumDownDataPort_OTHER;
	static	uint64	cumDownDataPort_PeerCache;
	// Session port breakdown stats for received bytes...
	static	uint64	sesDownDataPort_4662;
	static	uint64	sesDownDataPort_OTHER;
	static	uint64	sesDownDataPort_PeerCache;

	// Saved stats for cumulative connection data...
	static	float	cumConnAvgDownRate;
	static	float	cumConnMaxAvgDownRate;
	static	float	cumConnMaxDownRate;
	static	float	cumConnAvgUpRate;
	static	float	cumConnMaxAvgUpRate;
	static	float	cumConnMaxUpRate;
	static	time_t	cumConnRunTime;
	static	uint32	cumConnNumReconnects;
	static	uint32	cumConnAvgConnections;
	static	uint32	cumConnMaxConnLimitReached;
	static	uint32	cumConnPeakConnections;
	static	uint32	cumConnTransferTime;
	static	uint32	cumConnDownloadTime;
	static	uint32	cumConnUploadTime;
	static	uint32	cumConnServerDuration;

	// Saved records for servers / network...
	static	uint32	cumSrvrsMostWorkingServers;
	static	uint32	cumSrvrsMostUsersOnline;
	static	uint32	cumSrvrsMostFilesAvail;

	// Saved records for shared files...
	static	uint32	cumSharedMostFilesShared;
	static	uint64	cumSharedLargestShareSize;
	static	uint64	cumSharedLargestAvgFileSize;
	static	uint64	cumSharedLargestFileSize;

	// Save the date when the statistics were last reset...
	static	time_t	stat_datetimeLastReset;

	// Save new preferences for PPgStats
	static	UINT	statsConnectionsGraphRatio; // This will store the divisor, i.e. for 1:3 it will be 3, for 1:20 it will be 20.
	// Save the expanded branches of the stats tree
	static	TCHAR	statsExpandedTreeItems[256];

	static	UINT	statsSaveInterval;
	static  bool	m_bShowVerticalHourMarkers;
	// <-----khaos- End Statistics Members


	// Original Stats Stuff
	static	uint64	totalDownloadedBytes;
	static	uint64	totalUploadedBytes;
	// End Original Stats Stuff
	static	WORD	m_wLanguageID;
	static	bool	transferDoubleclick;
	static	EViewSharedFilesAccess m_iSeeShares;
	static	UINT	m_iToolDelayTime;	// tooltip delay time in seconds
	static	bool	bringtoforeground;
	static	UINT	splitterbarPosition;
	static  UINT    splitterbarPositionDownload;
	static	UINT	splitterbarPositionSvr;
    static  bool    m_moreOptions;

	static	UINT	m_uTransferWnd1;
	static	UINT	m_uTransferWnd2;
	//MORPH START - Added by SiRoB, Splitting Bar [O瞉
	static	UINT	splitterbarPositionStat;
	static	UINT	splitterbarPositionStat_HL;
	static	UINT	splitterbarPositionStat_HR;
	static	UINT	splitterbarPositionFriend;
	static	UINT	splitterbarPositionIRC;
	static	UINT	splitterbarPositionShared;
	//MORPH END - Added by SiRoB, Splitting Bar [O瞉
	static	UINT	m_uDeadServerRetries;
	static	DWORD	m_dwServerKeepAliveTimeout;
	// -khaos--+++> Changed data type to avoid overflows
	static	UINT	statsMax;
	// <-----khaos-
	static	UINT	statsAverageMinutes;

	static	CString	notifierConfiguration;
	static	bool	notifierOnDownloadFinished;
	static	bool	notifierOnNewDownload;
	static	bool	notifierOnChat;
	static	bool	notifierOnLog;
	static	bool	notifierOnImportantError;
	static	bool	notifierOnEveryChatMsg;
	static	bool	notifierOnNewVersion;
	static	ENotifierSoundType notifierSoundType;
	static	CString	notifierSoundFile;

	static	TCHAR	m_sircserver[50];
	static	TCHAR	m_sircnick[30];
	static	TCHAR	m_sircchannamefilter[50];
	static	bool	m_bircaddtimestamp;
	static	bool	m_bircusechanfilter;
	static	UINT	m_iircchanneluserfilter;
	static	TCHAR	m_sircperformstring[255];
	static	bool	m_bircuseperform;
	static	bool	m_birclistonconnect;
	static	bool	m_bircacceptlinks;
	static	bool	m_bircacceptlinksfriends;
	static	bool	m_bircsoundevents;
	static	bool	m_bircignoremiscmessage;
	static	bool	m_bircignorejoinmessage;
	static	bool	m_bircignorepartmessage;
	static	bool	m_bircignorequitmessage;
	static	bool	m_bircignoreemuleprotoaddfriend;
	static	bool	m_bircallowemuleprotoaddfriend;
	static	bool	m_bircignoreemuleprotosendlink;
	static	bool	m_birchelpchannel;

	static	bool	m_bRemove2bin;
	static	bool	m_bShowCopyEd2kLinkCmd;
	static	bool	m_bpreviewprio;
	static	bool	m_bSmartServerIdCheck;
	static	uint8	smartidstate;
	static	bool	m_bSafeServerConnect;
	static	bool	startMinimized;
	static	bool	m_bAutoStart;
	static	bool	m_bRestoreLastMainWndDlg;
	static	int		m_iLastMainWndDlgID;
	static	bool	m_bRestoreLastLogPane;
	static	int		m_iLastLogPaneID;
	static	UINT	MaxConperFive;
	static	bool	checkDiskspace;
	static	UINT	m_uMinFreeDiskSpace;
	static	bool	m_bSparsePartFiles;
	static	CString	m_strYourHostname;
	static	bool	m_bEnableVerboseOptions;
	static	bool	m_bVerbose;
	static	bool	m_bFullVerbose;
	static  int		m_byLogLevel;
	static	bool	m_bDebugSourceExchange; // Sony April 23. 2003, button to keep source exchange msg out of verbose log
	static	bool	m_bLogBannedClients;
	static	bool	m_bLogRatingDescReceived;
	static	bool	m_bLogSecureIdent;
	static	bool	m_bLogFilteredIPs;
	static	bool	m_bLogFileSaving;
    static  bool    m_bLogA4AF; // ZZ:DownloadManager
	static	bool	m_bLogUlDlEvents;
	static	bool	m_bUseDebugDevice;
	static	int		m_iDebugServerTCPLevel;
	static	int		m_iDebugServerUDPLevel;
	static	int		m_iDebugServerSourcesLevel;
	static	int		m_iDebugServerSearchesLevel;
	static	int		m_iDebugClientTCPLevel;
	static	int		m_iDebugClientUDPLevel;
	static	int		m_iDebugClientKadUDPLevel;
	static	int		m_iDebugSearchResultDetailLevel;
	static	bool	m_bupdatequeuelist;
	static	bool	m_bManualAddedServersHighPriority;
	static	bool	m_btransferfullchunks;
	static	int		m_istartnextfile;
	static	bool	m_bshowoverhead;
	static	bool	m_bDAP;
	static	bool	m_bUAP;
	static	bool	m_bDisableKnownClientList;
	static	bool	m_bDisableQueueList;
	static	bool	m_bExtControls;
	static	bool	m_bTransflstRemain;

	static UINT	MaxSourceConnect;
	static UINT	RetryNumber;
	static UINT	RetryDelay;
	static UINT PublicMaxConnectLimit;

	static	UINT	versioncheckdays;
	static	bool	showRatesInTitle;

	static	TCHAR	TxtEditor[MAX_PATH];
	static	CString	m_strVideoPlayer;
	static	CString	m_strVideoPlayerArgs;
	static	bool	moviePreviewBackup;
	static	int		m_iPreviewSmallBlocks;
	static	bool	m_bPreviewCopiedArchives;
	static	int		m_iInspectAllFileTypes;
	static	bool	m_bPreviewOnIconDblClk;
	static	bool	indicateratings;
	static	bool	watchclipboard;
	static	bool	watchclipboardUrl;
	static	bool	filterserverbyip;
	static	bool	m_bFirstStart;
	static	bool	m_bCreditSystem;

	static	bool	log2disk;
	static	bool	debug2disk;
	static	int		iMaxLogBuff;
	static	UINT	uMaxLogFileSize;
	static	ELogFileFormat m_iLogFileFormat;
	static	bool	scheduler;
	static	bool	dontcompressavi;
	static	bool	msgonlyfriends;
	static	bool	msgsecure;

	static	UINT	filterlevel;
	static	UINT	m_iFileBufferSize;
#ifdef _SUPPORT_MEMPOOL
	static  UINT    m_iFileBufferTime; // VC-SearchDream[2006-12-26]: Added for Memcache, the time of write to disk 
#endif
	static	UINT	m_iQueueSize;
	static	int		m_iCommitFiles;

	static	UINT	maxmsgsessions;
	static	uint32	versioncheckLastAutomatic;
	static	CString	messageFilter;
	static	CString	commentFilter;
	static	CString	filenameCleanups;
	static	TCHAR	datetimeformat[64];
	static	TCHAR	datetimeformat4log[64];
	static	LOGFONT m_lfHyperText;
	static	LOGFONT m_lfLogText;
	static	COLORREF m_crLogError;
	static	COLORREF m_crLogWarning;
	static	COLORREF m_crLogSuccess;
	static	int		m_iExtractMetaData;
	static	bool	m_bAdjustNTFSDaylightFileTime;
	static  bool    m_bAllocFull;

	static  int     m_iFilterPeer; // VC-Huby[2006-12-29]: 0-不过滤,1-过滤HighId,2-过滤lowid  
	static  int		 m_iDebugUpQueue;
#if defined(_DEBUG) || defined(_VCALPHA)
	static  CString	 m_sDebugUserName;	
#endif
	static	bool	m_bEnableHybridSearch;
	// Web Server [kuchin]
	static	TCHAR	m_sWebPassword[256];
	static	TCHAR	m_sWebLowPassword[256];
	static	uint16	m_nWebPort;
	static	bool	m_bWebEnabled;
	static	bool	m_bWebUseGzip;
	static	int		m_nWebPageRefresh;
	static	bool	m_bWebLowEnabled;
	static	TCHAR	m_sWebResDir[MAX_PATH];
	static	int		m_iWebTimeoutMins;
	static	int		m_iWebFileUploadSizeLimitMB;

	static	CString	m_strTemplateFile;
	static	ProxySettings proxy; // deadlake PROXYSUPPORT
	static  bool	m_bAllowAdminHiLevFunc;
	static	CUIntArray m_aAllowedRemoteAccessIPs;

	static	bool	showCatTabInfos;
	static	bool	resumeSameCat;
	static	bool	dontRecreateGraphs;
	static	bool	autofilenamecleanup;
	//static	int		allcatType;
	//static	bool	allcatTypeNeg;
	static	bool	m_bUseAutocompl;
	static	bool	m_bShowDwlPercentage;
	static	bool	m_bRemoveFinishedDownloads;
	static	UINT	m_iMaxChatHistory;
	static	bool	m_bShowActiveDownloadsBold;

	static	int		m_iSearchMethod;
	static	bool	m_bAdvancedSpamfilter;
	static	bool	m_bUseSecureIdent;
	// mobilemule
	static	TCHAR	m_sMMPassword[256];
	static	bool	m_bMMEnabled;
	static	uint16	m_nMMPort;

	static	bool	networkkademlia;
	static	bool	networked2k;

	// toolbar
	static	EToolbarLabelType m_nToolbarLabels;
	static	CString	m_sToolbarBitmap;
	static	CString	m_sToolbarBitmapFolder;
	static	CString	m_sToolbarSettings;
	static	bool	m_bReBarToolbar;
	static	CSize	m_sizToolbarIconSize;

	static	bool	m_bWinaTransToolbar;

	//preview
	static	bool	m_bPreviewEnabled;
	static	bool	m_bAutomaticArcPreviewStart;

	// ZZ:UploadSpeedSense -->
	static	bool	m_bDynUpEnabled;
	static	int		m_iDynUpPingTolerance;
	static	int		m_iDynUpGoingUpDivider;
	static	int		m_iDynUpGoingDownDivider;
	static	int		m_iDynUpNumberOfPings;
	static  int		m_iDynUpPingToleranceMilliseconds;
	static  bool	m_bDynUpUseMillisecondPingTolerance;
	// ZZ:UploadSpeedSense <--

    static bool     m_bA4AFSaveCpu; // ZZ:DownloadManager

    static bool     m_bHighresTimer;

	static	CStringList shareddir_list;
	static	CStringList addresses_list;

	static	int		m_iDbgHeap;
	static	UINT	m_nWebMirrorAlertLevel;
	static	bool	m_bRunAsUser;
	static	bool	m_bPreferRestrictedOverUser;

	static  bool	m_bUseOldTimeRemaining;

	// PeerCache
	static	uint32	m_uPeerCacheLastSearch;
	static	bool	m_bPeerCacheWasFound;
	static	bool	m_bPeerCacheEnabled;
	static	uint16	m_nPeerCachePort;
	static	bool	m_bPeerCacheShow;

	// Firewall settings
	static bool		m_bOpenPortsOnStartUp;

	//AICH Options
	static bool		m_bTrustEveryHash;
	
	// files
	static bool		m_bRememberCancelledFiles;
	static bool		m_bRememberDownloadedFiles;

	//emil notifier
	//static bool		m_bNotifierSendMail;
	//static CString	m_strNotifierMailServer;
	//static CString	m_strNotifierMailSender;
	//static CString	m_strNotifierMailReceiver;

	// encryption / obfuscation / verification
	static bool		m_bCryptLayerRequested;
	static bool		m_bCryptLayerSupported;
	static bool		m_bCryptLayerRequired;
	static uint8	m_byCryptTCPPaddingLength;
	static uint32   m_dwKadUDPKey;

	// Spam
	static bool		m_bEnableSearchResultFilter;

	static BOOL		m_bIsRunningAeroGlass;
	static bool		m_bStoreSearches;
	static bool		m_bAdvancePageShowed;

	static int		m_iCloseMode;

	static int		m_iConnectionType;

	// {Begin} Added by Soar Chin (8/31/2007)
	static bool		m_bAddToIEMenu;
	static bool		m_bMonitorLinks;
	static bool		m_bMonitorEd2k;
	// {End}   Added by Soar Chin (8/31/2007)

	enum Table
	{
		tableDownload, 
		tableUpload, 
		tableQueue, 
		tableSearch,
		tableShared, 
		tableServer, 
		tableClientList,
		tableFilenames,
		tableIrcMain,
		tableIrcChannels,
		tableDownloadClients
	};

	friend class CPreferencesWnd;
	friend class CPPgGeneral;
	friend class CPPgConnection;
	friend class CPPgServer;
	friend class CPPgDirectories;
	friend class CPPgFiles;
//	friend class CPPgNotify;
//	friend class CPPgIRC;
	friend class Wizard;
	friend class CPPgTweaks;
	friend class CPPgDisplay;
//	friend class CPPgSecurity;
	friend class CPPgScheduler;
	friend class CPPgDebug;

	CPreferences();
	~CPreferences();

	static	void	Init();
	static	void	Uninit();

	static	LPCTSTR GetTempDir(int id = 0)				{return (LPCTSTR)tempdir.GetAt((id < tempdir.GetCount()) ? id : 0);}
	static	int		GetTempDirCount()					{return tempdir.GetCount();}
	static	bool	CanFSHandleLargeFiles(const CString strFilePath=_T(""));
	static	LPCTSTR GetConfigFile();
	static	const CString& GetWebServerDir()			{return m_strWebServerDir;}
	static	const CString& GetFileCommentsFilePath()	{return m_strFileCommentsFilePath;}
	static	const CString& GetLogDir()					{return m_strLogDir;}

	static	bool	IsTempFile(const CString& rstrDirectory, const CString& rstrName);
	static	bool	IsConfigFile(const CString& rstrDirectory, const CString& rstrName);
	static	bool	IsShareableDirectory(const CString& rstrDirectory);
	static	bool	IsInstallationDirectory(const CString& rstrDir);

	static	CString	GetMuleDirectory(EDefaultDirectory eDirectory, bool bCreate = true);
	static	void	SetMuleDirectory(EDefaultDirectory eDirectory, CString strNewDir);
	static	void	ChangeUserDirMode(int nNewMode);
	static  void	ChangeUserAccount();
	static  CString GetConfigBaseDirByDirMode( int nDirMode );

	static	bool	Save();
	static	void	SaveCats();
	static	void	SavePort();


	static	bool	GetUseServerPriorities()			{return m_bUseServerPriorities;}
	static	bool	GetUseUserSortedServerList()		{return m_bUseUserSortedServerList;}
	static	bool	Reconnect()							{return reconnect;}
	static	const CString& GetUserNick()				{return strNick;}
	static  const CString GetUserNickVC();		//VeryCD added by kernel 1983 2006.08.01
	static	void	SetUserNick(LPCTSTR pszNick);
	static	int		GetMaxUserNickLength()				{return 50;}

	static	LPCSTR	GetBindAddrA()						{return m_pszBindAddrA; }
	static	LPCWSTR	GetBindAddrW()						{return m_pszBindAddrW; }

	//upnp_start
	static	uint16	GetPort();//						{return port;}
	static	uint16	GetUDPPort();//						{return udpport;}
	//upnp_end

	static	uint16	GetServerUDPPort()					{return nServerUDPPort;}
	static	uchar*	GetUserHash()						{return userhash;}
	// ZZ:UploadSpeedSense -->
	static	uint16	GetMinUpload()						{return minupload;}
	// ZZ:UploadSpeedSense <--
	static	uint16	GetMaxUpload()						{return maxupload;}
	static	bool	IsICHEnabled()						{return ICH;}
	static	bool	GetAutoUpdateServerList()			{return m_bAutoUpdateServerList;}
	static	bool	UpdateNotify()						{return updatenotify;}
	static	bool	GetMinToTray()						{return mintotray;}
	static	bool	DoAutoConnect()						{return autoconnect;}
	static	void	SetAutoConnect(bool inautoconnect)	{autoconnect = inautoconnect;}
	static	bool	GetAddServersFromServer()			{return m_bAddServersFromServer;}
	static	bool	GetAddServersFromClients()			{return m_bAddServersFromClients;}
	static	bool*	GetMinTrayPTR()						{return &mintotray;}
	static	UINT	GetTrafficOMeterInterval()			{return trafficOMeterInterval;}
	static	void	SetTrafficOMeterInterval(UINT in)	{trafficOMeterInterval=in;}
	static	UINT	GetStatsInterval()					{return statsInterval;}
	static	void	SetStatsInterval(UINT in)			{statsInterval=in;}

	// -khaos--+++> Many, many, many, many methods.
	static	void	SaveStats(int bBackUp = 0);
	static	void	SetRecordStructMembers();
	static	void	SaveCompletedDownloadsStat();
	static	bool	LoadStats(int loadBackUp = 0);
	static	void	ResetCumulativeStatistics();

	static	void	Add2DownCompletedFiles()			{cumDownCompletedFiles++;}
	static	void	SetConnMaxAvgDownRate(float in)		{cumConnMaxAvgDownRate = in;}
	static	void	SetConnMaxDownRate(float in)		{cumConnMaxDownRate = in;}
	static	void	SetConnAvgUpRate(float in)			{cumConnAvgUpRate = in;}
	static	void	SetConnMaxAvgUpRate(float in)		{cumConnMaxAvgUpRate = in;}
	static	void	SetConnMaxUpRate(float in)			{cumConnMaxUpRate = in;}
	static	void	SetConnPeakConnections(int in)		{cumConnPeakConnections = in;}
	static	void	SetUpAvgTime(int in)				{cumUpAvgTime = in;}
	static	void	Add2DownSAvgTime(int in)			{sesDownAvgTime += in;}
	static	void	SetDownCAvgTime(int in)				{cumDownAvgTime = in;}
	static	void	Add2ConnTransferTime(int in)		{cumConnTransferTime += in;}
	static	void	Add2ConnDownloadTime(int in)		{cumConnDownloadTime += in;}
	static	void	Add2ConnUploadTime(int in)			{cumConnUploadTime += in;}
	static	void	Add2DownSessionCompletedFiles()		{sesDownCompletedFiles++;}
	static	void	Add2SessionTransferData(UINT uClientID, UINT uClientPort, BOOL bFromPF, BOOL bUpDown, uint32 bytes, bool sentToFriend = false);
	static	void	Add2DownSuccessfulSessions()		{sesDownSuccessfulSessions++;
														 cumDownSuccessfulSessions++;}
	static	void	Add2DownFailedSessions()			{sesDownFailedSessions++;
														 cumDownFailedSessions++;}
	static	void	Add2LostFromCorruption(uint64 in)	{sesLostFromCorruption += in;}
	static	void	Add2SavedFromCompression(uint64 in) {sesSavedFromCompression += in;}
	static	void	Add2SessionPartsSavedByICH(int in)	{sesPartsSavedByICH += in;}

	// Saved stats for cumulative downline overhead
	static	uint64	GetDownOverheadTotal()				{return cumDownOverheadTotal;}
	static	uint64	GetDownOverheadFileReq()			{return cumDownOverheadFileReq;}
	static	uint64	GetDownOverheadSrcEx()				{return cumDownOverheadSrcEx;}
	static	uint64	GetDownOverheadServer()				{return cumDownOverheadServer;}
	static	uint64	GetDownOverheadKad()				{return cumDownOverheadKad;}
	static	uint64	GetDownOverheadTotalPackets()		{return cumDownOverheadTotalPackets;}
	static	uint64	GetDownOverheadFileReqPackets()		{return cumDownOverheadFileReqPackets;}
	static	uint64	GetDownOverheadSrcExPackets()		{return cumDownOverheadSrcExPackets;}
	static	uint64	GetDownOverheadServerPackets()		{return cumDownOverheadServerPackets;}
	static	uint64	GetDownOverheadKadPackets()			{return cumDownOverheadKadPackets;}

	// Saved stats for cumulative upline overhead
	static	uint64	GetUpOverheadTotal()				{return cumUpOverheadTotal;}
	static	uint64	GetUpOverheadFileReq()				{return cumUpOverheadFileReq;}
	static	uint64	GetUpOverheadSrcEx()				{return cumUpOverheadSrcEx;}
	static	uint64	GetUpOverheadServer()				{return cumUpOverheadServer;}
	static	uint64	GetUpOverheadKad()					{return cumUpOverheadKad;}
	static	uint64	GetUpOverheadTotalPackets()			{return cumUpOverheadTotalPackets;}
	static	uint64	GetUpOverheadFileReqPackets()		{return cumUpOverheadFileReqPackets;}
	static	uint64	GetUpOverheadSrcExPackets()			{return cumUpOverheadSrcExPackets;}
	static	uint64	GetUpOverheadServerPackets()		{return cumUpOverheadServerPackets;}
	static	uint64	GetUpOverheadKadPackets()			{return cumUpOverheadKadPackets;}

	// Saved stats for cumulative upline data
	static	uint32	GetUpSuccessfulSessions()			{return cumUpSuccessfulSessions;}
	static	uint32	GetUpFailedSessions()				{return cumUpFailedSessions;}
	static	uint32	GetUpAvgTime()						{return cumUpAvgTime;}

	// Saved stats for cumulative downline data
	static	uint32	GetDownCompletedFiles()				{return cumDownCompletedFiles;}
	static	uint32	GetDownC_SuccessfulSessions()		{return cumDownSuccessfulSessions;}
	static	uint32	GetDownC_FailedSessions()			{return cumDownFailedSessions;}
	static	uint32	GetDownC_AvgTime()					{return cumDownAvgTime;}
	
	// Session download stats
	static	uint32	GetDownSessionCompletedFiles()		{return sesDownCompletedFiles;}
	static	uint32	GetDownS_SuccessfulSessions()		{return sesDownSuccessfulSessions;}
	static	uint32	GetDownS_FailedSessions()			{return sesDownFailedSessions;}
	static	uint32	GetDownS_AvgTime()					{return GetDownS_SuccessfulSessions() ? sesDownAvgTime / GetDownS_SuccessfulSessions() : 0;}

	// Saved stats for corruption/compression
	static	uint64	GetCumLostFromCorruption()			{return cumLostFromCorruption;}
	static	uint64	GetCumSavedFromCompression()		{return cumSavedFromCompression;}
	static	uint64	GetSesLostFromCorruption()			{return sesLostFromCorruption;}
	static	uint64	GetSesSavedFromCompression()		{return sesSavedFromCompression;}
	static	uint32	GetCumPartsSavedByICH()				{return cumPartsSavedByICH;}
	static	uint32	GetSesPartsSavedByICH()				{return sesPartsSavedByICH;}


	// Cumulative client breakdown stats for sent bytes
	static	uint64	GetUpTotalClientData()				{return   GetCumUpData_EDONKEY()
																+ GetCumUpData_EDONKEYHYBRID()
																+ GetCumUpData_EMULE()
																+ GetCumUpData_MLDONKEY()
																+ GetCumUpData_AMULE()
																+ GetCumUpData_EMULECOMPAT()
																+ GetCumUpData_SHAREAZA();}
	static	uint64	GetCumUpData_EDONKEY()				{return (cumUpData_EDONKEY +		sesUpData_EDONKEY );}
	static	uint64	GetCumUpData_EDONKEYHYBRID()		{return (cumUpData_EDONKEYHYBRID +	sesUpData_EDONKEYHYBRID );}
	static	uint64	GetCumUpData_EMULE()				{return (cumUpData_EMULE +			sesUpData_EMULE );}
	static	uint64	GetCumUpData_MLDONKEY()				{return (cumUpData_MLDONKEY +		sesUpData_MLDONKEY );}
	static	uint64	GetCumUpData_AMULE()				{return (cumUpData_AMULE +			sesUpData_AMULE );}
	static	uint64	GetCumUpData_EMULECOMPAT()			{return (cumUpData_EMULECOMPAT +	sesUpData_EMULECOMPAT );}
	static	uint64	GetCumUpData_SHAREAZA()				{return (cumUpData_SHAREAZA +		sesUpData_SHAREAZA );}
	
	// Session client breakdown stats for sent bytes
	static	uint64	GetUpSessionClientData()			{return   sesUpData_EDONKEY 
																+ sesUpData_EDONKEYHYBRID 
																+ sesUpData_EMULE 
																+ sesUpData_MLDONKEY 
																+ sesUpData_AMULE
																+ sesUpData_EMULECOMPAT
																+ sesUpData_SHAREAZA;}
	static	uint64	GetUpData_EDONKEY()					{return sesUpData_EDONKEY;}
	static	uint64	GetUpData_EDONKEYHYBRID()			{return sesUpData_EDONKEYHYBRID;}
	static	uint64	GetUpData_EMULE()					{return sesUpData_EMULE;}
	static	uint64	GetUpData_MLDONKEY()				{return sesUpData_MLDONKEY;}
	static	uint64	GetUpData_AMULE()					{return sesUpData_AMULE;}
	static	uint64	GetUpData_EMULECOMPAT()				{return sesUpData_EMULECOMPAT;}
	static	uint64	GetUpData_SHAREAZA()				{return sesUpData_SHAREAZA;}

	// Cumulative port breakdown stats for sent bytes...
	static	uint64	GetUpTotalPortData()				{return   GetCumUpDataPort_4662() 
																+ GetCumUpDataPort_OTHER()
																+ GetCumUpDataPort_PeerCache();}
	static	uint64	GetCumUpDataPort_4662()				{return (cumUpDataPort_4662 +		sesUpDataPort_4662 );}
	static	uint64	GetCumUpDataPort_OTHER()			{return (cumUpDataPort_OTHER +		sesUpDataPort_OTHER );}
	static	uint64	GetCumUpDataPort_PeerCache()		{return (cumUpDataPort_PeerCache +	sesUpDataPort_PeerCache );}

	// Session port breakdown stats for sent bytes...
	static	uint64	GetUpSessionPortData()				{return   sesUpDataPort_4662 
																+ sesUpDataPort_OTHER
																+ sesUpDataPort_PeerCache;}
	static	uint64	GetUpDataPort_4662()				{return sesUpDataPort_4662;}
	static	uint64	GetUpDataPort_OTHER()				{return sesUpDataPort_OTHER;}
	static	uint64	GetUpDataPort_PeerCache()			{return sesUpDataPort_PeerCache;}

	// Cumulative DS breakdown stats for sent bytes...
	static	uint64	GetUpTotalDataFile()				{return (GetCumUpData_File() +		GetCumUpData_Partfile() );}
	static	uint64	GetCumUpData_File()					{return (cumUpData_File +			sesUpData_File );}
	static	uint64	GetCumUpData_Partfile()				{return (cumUpData_Partfile +		sesUpData_Partfile );}
	// Session DS breakdown stats for sent bytes...
	static	uint64	GetUpSessionDataFile()				{return (sesUpData_File +			sesUpData_Partfile );}
	static	uint64	GetUpData_File()					{return sesUpData_File;}
	static	uint64	GetUpData_Partfile()				{return sesUpData_Partfile;}

	// Cumulative client breakdown stats for received bytes
	static	uint64	GetDownTotalClientData()			{return   GetCumDownData_EDONKEY() 
																+ GetCumDownData_EDONKEYHYBRID() 
																+ GetCumDownData_EMULE() 
																+ GetCumDownData_MLDONKEY() 
																+ GetCumDownData_AMULE()
																+ GetCumDownData_EMULECOMPAT()
																+ GetCumDownData_SHAREAZA()
																+ GetCumDownData_URL();}
	static	uint64	GetCumDownData_EDONKEY()			{return (cumDownData_EDONKEY +			sesDownData_EDONKEY);}
	static	uint64	GetCumDownData_EDONKEYHYBRID()		{return (cumDownData_EDONKEYHYBRID +	sesDownData_EDONKEYHYBRID);}
	static	uint64	GetCumDownData_EMULE()				{return (cumDownData_EMULE +			sesDownData_EMULE);}
	static	uint64	GetCumDownData_MLDONKEY()			{return (cumDownData_MLDONKEY +			sesDownData_MLDONKEY);}
	static	uint64	GetCumDownData_AMULE()				{return (cumDownData_AMULE +			sesDownData_AMULE);}
	static	uint64	GetCumDownData_EMULECOMPAT()		{return (cumDownData_EMULECOMPAT +		sesDownData_EMULECOMPAT);}
	static	uint64	GetCumDownData_SHAREAZA()			{return (cumDownData_SHAREAZA +			sesDownData_SHAREAZA);}
	static	uint64	GetCumDownData_URL()				{return (cumDownData_URL +				sesDownData_URL);}
	
	// Session client breakdown stats for received bytes
	static	uint64	GetDownSessionClientData()			{return   sesDownData_EDONKEY 
																+ sesDownData_EDONKEYHYBRID 
																+ sesDownData_EMULE 
																+ sesDownData_MLDONKEY 
																+ sesDownData_AMULE
																+ sesDownData_EMULECOMPAT
																+ sesDownData_SHAREAZA
																+ sesDownData_URL;}
	static	uint64	GetDownData_EDONKEY()				{return sesDownData_EDONKEY;}
	static	uint64	GetDownData_EDONKEYHYBRID()			{return sesDownData_EDONKEYHYBRID;}
	static	uint64	GetDownData_EMULE()					{return sesDownData_EMULE;}
	static	uint64	GetDownData_MLDONKEY()				{return sesDownData_MLDONKEY;}
	static	uint64	GetDownData_AMULE()					{return sesDownData_AMULE;}
	static	uint64	GetDownData_EMULECOMPAT()			{return sesDownData_EMULECOMPAT;}
	static	uint64	GetDownData_SHAREAZA()				{return sesDownData_SHAREAZA;}
	static	uint64	GetDownData_URL()					{return sesDownData_URL;}

	// Cumulative port breakdown stats for received bytes...
	static	uint64	GetDownTotalPortData()				{return   GetCumDownDataPort_4662() 
																+ GetCumDownDataPort_OTHER()
																+ GetCumDownDataPort_PeerCache();}
	static	uint64	GetCumDownDataPort_4662()			{return cumDownDataPort_4662		+ sesDownDataPort_4662;}
	static	uint64	GetCumDownDataPort_OTHER()			{return cumDownDataPort_OTHER		+ sesDownDataPort_OTHER;}
	static	uint64	GetCumDownDataPort_PeerCache()		{return cumDownDataPort_PeerCache	+ sesDownDataPort_PeerCache;}

	// Session port breakdown stats for received bytes...
	static	uint64	GetDownSessionDataPort()			{return   sesDownDataPort_4662 
																+ sesDownDataPort_OTHER
																+ sesDownDataPort_PeerCache;}
	static	uint64	GetDownDataPort_4662()				{return sesDownDataPort_4662;}
	static	uint64	GetDownDataPort_OTHER()				{return sesDownDataPort_OTHER;}
	static	uint64	GetDownDataPort_PeerCache()			{return sesDownDataPort_PeerCache;}

	// Saved stats for cumulative connection data
	static	float	GetConnAvgDownRate()				{return cumConnAvgDownRate;}
	static	float	GetConnMaxAvgDownRate()				{return cumConnMaxAvgDownRate;}
	static	float	GetConnMaxDownRate()				{return cumConnMaxDownRate;}
	static	float	GetConnAvgUpRate()					{return cumConnAvgUpRate;}
	static	float	GetConnMaxAvgUpRate()				{return cumConnMaxAvgUpRate;}
	static	float	GetConnMaxUpRate()					{return cumConnMaxUpRate;}
	static	time_t	GetConnRunTime()					{return cumConnRunTime;}
	static	uint32	GetConnNumReconnects()				{return cumConnNumReconnects;}
	static	uint32	GetConnAvgConnections()				{return cumConnAvgConnections;}
	static	uint32	GetConnMaxConnLimitReached()		{return cumConnMaxConnLimitReached;}
	static	uint32	GetConnPeakConnections()			{return cumConnPeakConnections;}
	static	uint32	GetConnTransferTime()				{return cumConnTransferTime;}
	static	uint32	GetConnDownloadTime()				{return cumConnDownloadTime;}
	static	uint32	GetConnUploadTime()					{return cumConnUploadTime;}
	static	uint32	GetConnServerDuration()				{return cumConnServerDuration;}

	// Saved records for servers / network
	static	uint32	GetSrvrsMostWorkingServers()		{return cumSrvrsMostWorkingServers;}
	static	uint32	GetSrvrsMostUsersOnline()			{return cumSrvrsMostUsersOnline;}
	static	uint32	GetSrvrsMostFilesAvail()			{return cumSrvrsMostFilesAvail;}

	// Saved records for shared files
	static	uint32	GetSharedMostFilesShared()			{return cumSharedMostFilesShared;}
	static	uint64	GetSharedLargestShareSize()			{return cumSharedLargestShareSize;}
	static	uint64	GetSharedLargestAvgFileSize()		{return cumSharedLargestAvgFileSize;}
	static	uint64	GetSharedLargestFileSize()			{return cumSharedLargestFileSize;}

	// Get the long date/time when the stats were last reset
	static	time_t GetStatsLastResetLng()				{return stat_datetimeLastReset;}
	static	CString GetStatsLastResetStr(bool formatLong = true);
	static	UINT	GetStatsSaveInterval()				{return statsSaveInterval;}

	// Get and Set our new preferences
	static	void	SetStatsMax(UINT in)				{statsMax = in;}
	static	void	SetStatsConnectionsGraphRatio(UINT in) {statsConnectionsGraphRatio = in;}
	static	UINT	GetStatsConnectionsGraphRatio()		{return statsConnectionsGraphRatio;}
	static	void	SetExpandedTreeItems(CString in)	{_stprintf(statsExpandedTreeItems,_T("%s"),in);}
	static	CString GetExpandedTreeItems()				{return statsExpandedTreeItems;}

	static	uint64	GetTotalDownloaded()				{return totalDownloadedBytes;}
	static	uint64	GetTotalUploaded()					{return totalUploadedBytes;}

	static	bool	IsErrorBeepEnabled()				{return beepOnError;}
	static	bool	IsConfirmExitEnabled()				{return confirmExit;}
	static	bool	UseSplashScreen()					{return splashscreen;}
	static	bool	FilterLANIPs()						{return filterLANIPs;}
	static	bool	GetAllowLocalHostIP()				{return m_bAllocLocalHostIP;}
	static	bool	IsOnlineSignatureEnabled()			{return onlineSig;}
	static	int		GetMaxGraphUploadRate(bool bEstimateIfUnlimited);
	static	int		GetMaxGraphDownloadRate()			{return maxGraphDownloadRate;}
	static	void	SetMaxGraphUploadRate(int in);
	static	void	SetMaxGraphDownloadRate(int in)		{maxGraphDownloadRate=(in)?in:256;} //Changed by thilon on 2006.09.24

	static	uint16	GetMaxDownload();
	static	uint64	GetMaxDownloadInBytesPerSec(bool dynamic = false);
	static	UINT	GetMaxConnections()					{return maxconnections;}
	static	UINT	GetMaxHalfConnections()				{return maxhalfconnections;}
	static	UINT	GetMaxL2LHalfConnections()			{return maxL2Lhalfconnections;}
	static	UINT	GetMaxSourcePerFileDefault()		{return maxsourceperfile;}
	static	UINT	GetDeadServerRetries()				{return m_uDeadServerRetries;}
	static	DWORD	GetServerKeepAliveTimeout()			{return m_dwServerKeepAliveTimeout;}
	static	bool	GetConditionalTCPAccept()			{return m_bConditionalTCPAccept;}

	static	WORD	GetLanguageID();
	static	void	SetLanguageID(WORD lid);
	static	void	GetLanguages(CWordArray& aLanguageIDs);
	static	void	SetLanguage();
	static	const CString& GetLangDir()					{return m_strLangDir;}
	static	bool	IsLanguageSupported(LANGID lidSelected, bool bUpdateBefore);
	static	CString GetLangDLLNameByID(LANGID lidSelected);
	static	void	InitThreadLocale();
	static	void	SetRtlLocale(LCID lcid);
	static	CString GetHtmlCharset();

	static	bool	IsDoubleClickEnabled()				{return transferDoubleclick;}
	static	EViewSharedFilesAccess CanSeeShares(void)	{return m_iSeeShares;}
	static	UINT	GetToolTipDelay(void)				{return m_iToolDelayTime;}
	static	bool	IsBringToFront()					{return bringtoforeground;}

	static	UINT	GetSplitterbarPosition()			{return splitterbarPosition;}
	static	void	SetSplitterbarPosition(UINT pos)	{splitterbarPosition=pos;}

	static	UINT	GetSplitterbarPositionDownload()			{return splitterbarPositionDownload;}
	static	void	SetSplitterbarPositionDownload(UINT pos)	{splitterbarPositionDownload=pos;}

	static	UINT	GetSplitterbarPositionServer()		{return splitterbarPositionSvr;}
	static	void	SetSplitterbarPositionServer(UINT pos)	{splitterbarPositionSvr=pos;}
	static	UINT	GetTransferWnd1()					{return m_uTransferWnd1;}
	static	void	SetTransferWnd1(UINT uWnd1)			{m_uTransferWnd1 = uWnd1;}
	static	UINT	GetTransferWnd2()					{return m_uTransferWnd2;}
	static	void	SetTransferWnd2(UINT uWnd2)			{m_uTransferWnd2 = uWnd2;}
	//MORPH START - Added by SiRoB, Splitting Bar [O瞉
	static	UINT	GetSplitterbarPositionStat()		{return splitterbarPositionStat;}
	static	void	SetSplitterbarPositionStat(UINT pos) {splitterbarPositionStat=pos;}
	static	UINT	GetSplitterbarPositionStat_HL()		{return splitterbarPositionStat_HL;}
	static	void	SetSplitterbarPositionStat_HL(UINT pos) {splitterbarPositionStat_HL=pos;}
	static	UINT	GetSplitterbarPositionStat_HR()		{return splitterbarPositionStat_HR;}
	static	void	SetSplitterbarPositionStat_HR(UINT pos) {splitterbarPositionStat_HR=pos;}
	static	UINT	GetSplitterbarPositionFriend()		{return splitterbarPositionFriend;}
	static	void	SetSplitterbarPositionFriend(UINT pos) {splitterbarPositionFriend=pos;}
	static	UINT	GetSplitterbarPositionIRC()			{return splitterbarPositionIRC;}
	static	void	SetSplitterbarPositionIRC(UINT pos) {splitterbarPositionIRC=pos;}
	static	UINT	GetSplitterbarPositionShared()		{return splitterbarPositionShared;}
	static	void	SetSplitterbarPositionShared(UINT pos) {splitterbarPositionShared=pos;}
	//MORPH END   - Added by SiRoB, Splitting Bar [O瞉
	// -khaos--+++> Changed datatype to avoid overflows
	static	UINT	GetStatsMax()						{return statsMax;}
	// <-----khaos-
	static	bool	UseFlatBar()						{return (depth3D==0);}
	static	int		GetStraightWindowStyles()			{return m_iStraightWindowStyles;}

	static	const CString& GetSkinProfile()				{return m_strSkinProfile;}
	static	void	SetSkinProfile(LPCTSTR pszProfile)	{m_strSkinProfile = pszProfile;}

	static	const CString& GetSkinProfileDir()			{return m_strSkinProfileDir;}
	static	void	SetSkinProfileDir(LPCTSTR pszDir)	{m_strSkinProfileDir = pszDir;}

	static	UINT	GetStatsAverageMinutes()			{return statsAverageMinutes;}
	static	void	SetStatsAverageMinutes(UINT in)	{statsAverageMinutes=in;}

	static	const CString& GetNotifierConfiguration()	{return notifierConfiguration;}
	static	void	SetNotifierConfiguration(LPCTSTR pszConfigPath) {notifierConfiguration = pszConfigPath;}
	static	bool	GetNotifierOnDownloadFinished()		{return notifierOnDownloadFinished;}
	static	bool	GetNotifierOnNewDownload()			{return notifierOnNewDownload;}
	static	bool	GetNotifierOnChat()					{return notifierOnChat;}
	static	bool	GetNotifierOnLog()					{return notifierOnLog;}
	static	bool	GetNotifierOnImportantError()		{return notifierOnImportantError;}
	static	bool	GetNotifierOnEveryChatMsg()			{return notifierOnEveryChatMsg;}
	static	bool	GetNotifierOnNewVersion()			{return notifierOnNewVersion;}
	static	ENotifierSoundType GetNotifierSoundType()	{return notifierSoundType;}
	static	const CString& GetNotifierSoundFile()		{return notifierSoundFile;}

	static	bool	GetEnableMiniMule()					{return m_bEnableMiniMule;}
	static	bool	GetRTLWindowsLayout()				{return m_bRTLWindowsLayout;}

	static	CString GetIRCNick()						{return m_sircnick;}
	static	void	SetIRCNick(LPCTSTR in_nick)			{_tcscpy(m_sircnick, in_nick);}
	static	CString GetIRCServer()						{return m_sircserver;}
	static	bool	GetIRCAddTimestamp()				{return m_bircaddtimestamp;}
	static	CString GetIRCChanNameFilter()				{return m_sircchannamefilter;}
	static	bool	GetIRCUseChanFilter()				{return m_bircusechanfilter;}
	static	UINT	GetIRCChannelUserFilter()			{return m_iircchanneluserfilter;}
	static	CString GetIrcPerformString()				{return m_sircperformstring;}
	static	bool	GetIrcUsePerform()					{return m_bircuseperform;}
	static	bool	GetIRCListOnConnect()				{return m_birclistonconnect;}
	static	bool	GetIrcAcceptLinks()					{return m_bircacceptlinks;}
	static	bool	GetIrcAcceptLinksFriends()			{return m_bircacceptlinksfriends;}
	static	bool	GetIrcSoundEvents()					{return m_bircsoundevents;}
	static	bool	GetIrcIgnoreMiscMessage()			{return m_bircignoremiscmessage;}
	static	bool	GetIrcIgnoreJoinMessage()			{return m_bircignorejoinmessage;}
	static	bool	GetIrcIgnorePartMessage()			{return m_bircignorepartmessage;}
	static	bool	GetIrcIgnoreQuitMessage()			{return m_bircignorequitmessage;}
	static	bool	GetIrcIgnoreEmuleProtoAddFriend()	{return m_bircignoreemuleprotoaddfriend;}
	static	bool	GetIrcAllowEmuleProtoAddFriend()	{return m_bircallowemuleprotoaddfriend;}
	static	bool	GetIrcIgnoreEmuleProtoSendLink()	{return m_bircignoreemuleprotosendlink;}
	static	bool	GetIrcHelpChannel()					{return m_birchelpchannel;}

	static	WORD	GetWindowsVersion();
	static  bool	IsRunningAeroGlassTheme();

	static	bool	GetStartMinimized()					{return startMinimized;}
	static	void	SetStartMinimized( bool instartMinimized) {startMinimized = instartMinimized;}
	static	bool	GetAutoStart()						{return m_bAutoStart;}
	static	void	SetAutoStart( bool val)				{m_bAutoStart = val;}

	static	bool	GetRestoreLastMainWndDlg()			{return m_bRestoreLastMainWndDlg;}
	static	int		GetLastMainWndDlgID()				{return m_iLastMainWndDlgID;}
	static	void	SetLastMainWndDlgID(int iID)		{m_iLastMainWndDlgID = iID;}

	static	bool	GetRestoreLastLogPane()				{return m_bRestoreLastLogPane;}
	static	int		GetLastLogPaneID()					{return m_iLastLogPaneID;}
	static	void	SetLastLogPaneID(int iID)			{m_iLastLogPaneID = iID;}

	static	bool	GetSmartIdCheck()					{return m_bSmartServerIdCheck;}
	static	void	SetSmartIdCheck(bool in_smartidcheck) {m_bSmartServerIdCheck = in_smartidcheck;}
	static	uint8	GetSmartIdState()					{return smartidstate;}
	static	void	SetSmartIdState(uint8 in_smartidstate) {smartidstate = in_smartidstate;}
	static	bool	GetPreviewPrio()					{return m_bpreviewprio;}
	static	void	SetPreviewPrio(bool in)				{m_bpreviewprio=in;}
	static	bool	GetUpdateQueueList()				{return m_bupdatequeuelist;}
	static	bool	GetManualAddedServersHighPriority()	{return m_bManualAddedServersHighPriority;}
	static	bool	TransferFullChunks()				{return m_btransferfullchunks;}
	static	void	SetTransferFullChunks( bool m_bintransferfullchunks )				{m_btransferfullchunks = m_bintransferfullchunks;}
	static	int		StartNextFile()						{return m_istartnextfile;}
	static	bool	ShowOverhead()						{return m_bshowoverhead;}
	static	void	SetNewAutoUp(bool m_bInUAP)			{m_bUAP = m_bInUAP;}
	static	bool	GetNewAutoUp()						{return m_bUAP;}
	static	void	SetNewAutoDown(bool m_bInDAP)		{m_bDAP = m_bInDAP;}
	static	bool	GetNewAutoDown()					{return m_bDAP;}
	static	bool	IsKnownClientListDisabled()			{return m_bDisableKnownClientList;}
	static	bool	IsQueueListDisabled()				{return m_bDisableQueueList;}
	static	bool	IsFirstStart()						{return m_bFirstStart;}
	static	bool	UseCreditSystem()					{return m_bCreditSystem;}
	static	void	SetCreditSystem(bool m_bInCreditSystem) {m_bCreditSystem = m_bInCreditSystem;}

	static	TCHAR*	GetTxtEditor()						{return TxtEditor;}
	static	const CString& GetVideoPlayer()				{return m_strVideoPlayer;}
	static	const CString& GetVideoPlayerArgs()			{return m_strVideoPlayerArgs;}

	static	UINT	GetFileBufferSize()					{return m_iFileBufferSize;}
#ifdef _SUPPORT_MEMPOOL
	static  UINT    GetFileBufferTime()                 {return m_iFileBufferTime;} // VC-SearchDream[2006-12-26]: File buffer time
#endif
	
	static	UINT	GetQueueSize()						{return m_iQueueSize;}
	static	int		GetCommitFiles()					{return m_iCommitFiles;}
	static	bool	GetShowCopyEd2kLinkCmd()			{return m_bShowCopyEd2kLinkCmd;}

	// Barry
	static	UINT	Get3DDepth()						{return depth3D;}
	static	bool	AutoTakeED2KLinks()					{return autotakeed2klinks;}
	static	bool	AddNewFilesPaused()					{return addnewfilespaused;}

	static	bool	TransferlistRemainSortStyle()		{return m_bTransflstRemain;}
	static	void	TransferlistRemainSortStyle(bool in){m_bTransflstRemain=in;}

	static	DWORD	GetStatsColor(int index)			{return m_adwStatsColors[index];}
	static	void	SetStatsColor(int index, DWORD value){m_adwStatsColors[index] = value;}
	static	int		GetNumStatsColors()					{return ARRSIZE(m_adwStatsColors);}
	static	void	GetAllStatsColors(int iCount, LPDWORD pdwColors);
	static	bool	SetAllStatsColors(int iCount, const DWORD* pdwColors);
	static	void	ResetStatsColor(int index);

	static	void	SetMaxConsPerFive(UINT in)			{MaxConperFive=in;}
	static	LPLOGFONT GetHyperTextLogFont()				{return &m_lfHyperText;}
	static	void	SetHyperTextFont(LPLOGFONT plf)		{m_lfHyperText = *plf;}
	static	LPLOGFONT GetLogFont()						{return &m_lfLogText;}
	static	void	SetLogFont(LPLOGFONT plf)			{m_lfLogText = *plf;}
	static	COLORREF GetLogErrorColor()					{return m_crLogError;}
	static	COLORREF GetLogWarningColor()				{return m_crLogWarning;}
	static	COLORREF GetLogSuccessColor()				{return m_crLogSuccess;}

	static	UINT	GetMaxSourceConnect()				{return MaxSourceConnect;}
	static	UINT	GetRetryNumber()					{return RetryNumber;}
	static	UINT	GetRetryDelay()						{return RetryDelay;}
	static  UINT    GetPublicMaxConnectLimit()          {return PublicMaxConnectLimit;}

	static	void	SetMaxSourceConnect(UINT in)		{MaxSourceConnect=in;}
	static	void	SetRetryNumbe(UINT in)				{RetryNumber=in;}
	static	void	SetRetryDelay(UINT in)				{RetryDelay=in;}
	static  void    SetPublicMaxConnectLimit(UINT in)   {PublicMaxConnectLimit = in;}

	static	UINT	GetMaxConperFive()					{return MaxConperFive;}
	static	UINT	GetDefaultMaxConperFive();

	static	bool	IsSafeServerConnectEnabled()		{return m_bSafeServerConnect;}
	static	void	SetSafeServerConnectEnabled(bool in){m_bSafeServerConnect=in;}
	static	bool	IsMoviePreviewBackup()				{return moviePreviewBackup;}
	static	int		GetPreviewSmallBlocks()				{return m_iPreviewSmallBlocks;}
	static	bool	GetPreviewCopiedArchives()			{return m_bPreviewCopiedArchives;}
	static	int		GetInspectAllFileTypes()			{return m_iInspectAllFileTypes;}
	static	int		GetExtractMetaData()				{return m_iExtractMetaData;}
	static	bool	GetAdjustNTFSDaylightFileTime()		{return m_bAdjustNTFSDaylightFileTime;}

	static	const CString& GetYourHostname()			{return m_strYourHostname;}
	static	void	SetYourHostname(LPCTSTR pszHostname){m_strYourHostname = pszHostname;}
	static	bool	IsCheckDiskspaceEnabled()			{return checkDiskspace;}
	static	UINT	GetMinFreeDiskSpace()				{return m_uMinFreeDiskSpace;}
	static	bool	GetSparsePartFiles()				{return m_bSparsePartFiles;}
	static	void	SetSparsePartFiles(bool bEnable)	{m_bSparsePartFiles = bEnable;}

	static	void	SetMaxUpload(UINT in);
	static	void	SetMaxDownload(UINT in);

	static	WINDOWPLACEMENT GetEmuleWindowPlacement()	{return EmuleWindowPlacement;}
	static	void	SetWindowLayout(WINDOWPLACEMENT in) {EmuleWindowPlacement=in;}

	static	bool	GetAutoConnectToStaticServersOnly() {return m_bAutoConnectToStaticServersOnly;}
	static	UINT	GetUpdateDays()						{return versioncheckdays;}
	static	uint32	GetLastVC()							{return versioncheckLastAutomatic;}
	static	void	UpdateLastVC();
	static	int		GetIPFilterLevel()					{return filterlevel;}
	static	const CString& GetMessageFilter()			{return messageFilter;}
	static	const CString& GetCommentFilter()			{return commentFilter;}
	static	const CString& GetFilenameCleanups()		{return filenameCleanups;}

	static	bool	ShowRatesOnTitle()					{return showRatesInTitle;}
	static	void	LoadCats();
	static	CString GetDateTimeFormat()					{return CString(datetimeformat);}
	static	CString GetDateTimeFormat4Log()				{return CString(datetimeformat4log);}

	// Download Categories (Ornis)
	static	int		AddCat(Category_Struct* cat)		{catMap.Add(cat); return catMap.GetCount()-1;}
	static	bool	MoveCat(UINT from, UINT to);
	static	void	RemoveCat(int index);
	static	int		GetCatCount()						{return catMap.GetCount();}
	static  bool	SetCatFilter(int index, int filter);
	static  int		GetCatFilter(int index);
	static	bool	GetCatFilterNeg(int index);
	static	void	SetCatFilterNeg(int index, bool val);
	static	Category_Struct* GetCategory(int index)		{if (index>=0 && index<catMap.GetCount()) return catMap.GetAt(index); else return NULL;}
	static	const CString &GetCatPath(int index)		{return catMap.GetAt(index)->strIncomingPath;}
	static	DWORD	GetCatColor(int index)				{if (index>=0 && index<catMap.GetCount()) return catMap.GetAt(index)->color; else return 0;}

	static	bool	GetPreviewOnIconDblClk()			{return m_bPreviewOnIconDblClk;}
	static	bool	ShowRatingIndicator()				{return indicateratings;}
	static	bool	WatchClipboard4ED2KLinks()			{return watchclipboard;}
	static	bool	WatchClipboard4UrlLinks()			{return watchclipboardUrl;}
	static	bool	GetRemoveToBin()					{return m_bRemove2bin;}
	static	bool	FilterServerByIP()					{return filterserverbyip;}
	static	bool	GetFilterServerByIP()				{return filterserverbyip;}

	static	bool	GetLog2Disk()						{return log2disk;}
	static	bool	GetDebug2Disk()						{return m_bVerbose && debug2disk;}
	static	int		GetMaxLogBuff()						{return iMaxLogBuff;}
	static	UINT	GetMaxLogFileSize()					{return uMaxLogFileSize;}
	static	ELogFileFormat GetLogFileFormat()			{return m_iLogFileFormat;}

	// WebServer
	static	uint16	GetWSPort()							{return m_nWebPort;}
	static	void	SetWSPort(uint16 uPort)				{m_nWebPort=uPort;}
	static	CString GetWSPass()							{return CString(m_sWebPassword);}
	static	void	SetWSPass(CString strNewPass);
	static	bool	GetWSIsEnabled()					{return m_bWebEnabled;}
	static	void	SetWSIsEnabled(bool bEnable)		{m_bWebEnabled=bEnable;}
	static	bool	GetWebUseGzip()						{return m_bWebUseGzip;}
	static	void	SetWebUseGzip(bool bUse)			{m_bWebUseGzip=bUse;}
	static	int		GetWebPageRefresh()					{return m_nWebPageRefresh;}
	static	void	SetWebPageRefresh(int nRefresh)		{m_nWebPageRefresh=nRefresh;}
	static	bool	GetWSIsLowUserEnabled()				{return m_bWebLowEnabled;}
	static	void	SetWSIsLowUserEnabled(bool in)		{m_bWebLowEnabled=in;}
	static	CString GetWSLowPass()						{return CString(m_sWebLowPassword);}
	static	int		GetWebTimeoutMins()					{return m_iWebTimeoutMins;}
	static  bool	GetWebAdminAllowedHiLevFunc()		{return m_bAllowAdminHiLevFunc;}
	static	void	SetWSLowPass(CString strNewPass);
	static  const CUIntArray& GetAllowedRemoteAccessIPs(){return m_aAllowedRemoteAccessIPs;}
	static	uint32	GetMaxWebUploadFileSizeMB()			{return m_iWebFileUploadSizeLimitMB;}

	static	void	SetMaxSourcesPerFile(UINT in)		{maxsourceperfile=in;}
	static	void	SetMaxConnections(UINT in)			{maxconnections =in;}
	static	void	SetMaxHalfConnections(UINT in)		{maxhalfconnections =in;}
	static	bool	IsSchedulerEnabled()				{return scheduler;}
	static	void	SetSchedulerEnabled(bool in)		{scheduler=in;}
	static	bool	GetDontCompressAvi()				{return dontcompressavi;}

	static	bool	MsgOnlyFriends()					{return msgonlyfriends;}
	static	bool	MsgOnlySecure()						{return msgsecure;}
	static	UINT	GetMsgSessionsMax()					{return maxmsgsessions;}
	static	bool	IsSecureIdentEnabled()				{return m_bUseSecureIdent;} // use clientcredits->CryptoAvailable() to check if crypting is really available and not this function
	static	bool	IsAdvSpamfilterEnabled()			{return m_bAdvancedSpamfilter;}
	static	CString GetTemplate()						{return m_strTemplateFile;}
	static	void	SetTemplate(CString in)				{m_strTemplateFile = in;}
	static	bool	GetNetworkKademlia()				{return networkkademlia;}
	static	void	SetNetworkKademlia(bool val);
	static	bool	GetNetworkED2K()					{return networked2k;}
	static	void	SetNetworkED2K(bool val)			{networked2k = val;}

	// mobileMule
	static	CString GetMMPass()							{return CString(m_sMMPassword);}
	static	void	SetMMPass(CString strNewPass);
	static	bool	IsMMServerEnabled()					{return m_bMMEnabled;}
	static	void	SetMMIsEnabled(bool bEnable)		{m_bMMEnabled=bEnable;}
	static	uint16	GetMMPort()							{return m_nMMPort;}
	static	void	SetMMPort(uint16 uPort)				{m_nMMPort=uPort;}

	// deadlake PROXYSUPPORT
	static	const ProxySettings& GetProxySettings()		{return proxy;}
	static	void	SetProxySettings(const ProxySettings& proxysettings) {proxy = proxysettings;}

	static	bool	ShowCatTabInfos()					{return showCatTabInfos;}
	static	void	ShowCatTabInfos(bool in)			{showCatTabInfos=in;}

	static	bool	AutoFilenameCleanup()				{return autofilenamecleanup;}
	static	void	AutoFilenameCleanup(bool in)		{autofilenamecleanup=in;}
	static	void	SetFilenameCleanups(CString in)		{filenameCleanups=in;}

	static	bool	GetResumeSameCat()					{return resumeSameCat;}
	static	bool	IsGraphRecreateDisabled()			{return dontRecreateGraphs;}
	static	bool	IsExtControlsEnabled()				{return m_bExtControls;}
	static	void	SetExtControls(bool in)				{m_bExtControls=in;}
	static	bool	GetRemoveFinishedDownloads()		{return m_bRemoveFinishedDownloads;}

	static	UINT	GetMaxChatHistoryLines()			{return m_iMaxChatHistory;}
	static	bool	GetUseAutocompletion()				{return m_bUseAutocompl;}
	static	bool	GetUseDwlPercentage()				{return m_bShowDwlPercentage;}
	static	void	SetUseDwlPercentage(bool in)		{m_bShowDwlPercentage=in;}
	static	bool	GetShowActiveDownloadsBold()		{return m_bShowActiveDownloadsBold;}

	//Toolbar
	static	const CString& GetToolbarSettings()					{return m_sToolbarSettings;}
	static	void	SetToolbarSettings(const CString& in)		{m_sToolbarSettings = in;}
	static	const CString& GetToolbarBitmapSettings()			{return m_sToolbarBitmap;}
	static	void	SetToolbarBitmapSettings(const CString& path){m_sToolbarBitmap = path;}
	static	const CString& GetToolbarBitmapFolderSettings()		{return m_sToolbarBitmapFolder;}
	static	void	SetToolbarBitmapFolderSettings(const CString& path){m_sToolbarBitmapFolder = path;}
	static	EToolbarLabelType GetToolbarLabelSettings()			{return m_nToolbarLabels;}
	static	void	SetToolbarLabelSettings(EToolbarLabelType eLabelType) {m_nToolbarLabels = eLabelType;}
	static	bool	GetReBarToolbar()							{return m_bReBarToolbar;}
	static	bool	GetUseReBarToolbar();
	static	CSize	GetToolbarIconSize()				{return m_sizToolbarIconSize;}
	static	void	SetToolbarIconSize(CSize siz)		{m_sizToolbarIconSize = siz;}

	static	bool	IsTransToolbarEnabled()				{return m_bWinaTransToolbar;}

	static	int		GetSearchMethod()					{return m_iSearchMethod;}
	static	void	SetSearchMethod(int iMethod)		{m_iSearchMethod = iMethod;}

	// ZZ:UploadSpeedSense -->
	static	bool	IsDynUpEnabled();
	static	void	SetDynUpEnabled(bool newValue)		{m_bDynUpEnabled = newValue;}
	static	int		GetDynUpPingTolerance()				{return m_iDynUpPingTolerance;}
	static	int		GetDynUpGoingUpDivider()			{return m_iDynUpGoingUpDivider;}
	static	int		GetDynUpGoingDownDivider()			{return m_iDynUpGoingDownDivider;}
	static	int		GetDynUpNumberOfPings()				{return m_iDynUpNumberOfPings;}
    static  bool	IsDynUpUseMillisecondPingTolerance(){return m_bDynUpUseMillisecondPingTolerance;} // EastShare - Added by TAHO, USS limit
	static  int		GetDynUpPingToleranceMilliseconds() {return m_iDynUpPingToleranceMilliseconds;} // EastShare - Added by TAHO, USS limit
	static  void	SetDynUpPingToleranceMilliseconds(int in){m_iDynUpPingToleranceMilliseconds = in;}
	// ZZ:UploadSpeedSense <--

    static bool     GetA4AFSaveCpu()                    {return m_bA4AFSaveCpu;} // ZZ:DownloadManager

    static bool     GetHighresTimer()                   {return m_bHighresTimer;}

	static	CString	GetHomepageBaseURL()				{return GetHomepageBaseURLForLevel(GetWebMirrorAlertLevel());}
	static	CString	GetVersionCheckBaseURL();					
	static	void	SetWebMirrorAlertLevel(uint8 newValue){m_nWebMirrorAlertLevel = newValue;}
	static	bool	IsDefaultNick(const CString strCheck);
	static	UINT	GetWebMirrorAlertLevel();
	static	bool	UseSimpleTimeRemainingComputation()	{return m_bUseOldTimeRemaining;}

	static	bool	IsRunAsUserEnabled();
	static	bool	IsPreferingRestrictedOverUser()		{return m_bPreferRestrictedOverUser;}

	// PeerCache
	static	bool	IsPeerCacheDownloadEnabled()		{return (m_bPeerCacheEnabled && !IsClientCryptLayerRequested());}
	static	uint32	GetPeerCacheLastSearch()			{return m_uPeerCacheLastSearch;}
	static	bool	WasPeerCacheFound()					{return m_bPeerCacheWasFound;}
	static	void	SetPeerCacheLastSearch(uint32 dwLastSearch) {m_uPeerCacheLastSearch = dwLastSearch;}
	static	void	SetPeerCacheWasFound(bool bFound)	{m_bPeerCacheWasFound = bFound;}
	static	uint16	GetPeerCachePort()					{return m_nPeerCachePort;}
	static	void	SetPeerCachePort(uint16 nPort)		{m_nPeerCachePort = nPort;}
	static	bool	GetPeerCacheShow()					{return m_bPeerCacheShow;}

	// Verbose log options
	static	bool	GetEnableVerboseOptions()			{return m_bEnableVerboseOptions;}
	static	bool	GetVerbose()						{return m_bVerbose;}
	static	bool	GetFullVerbose()					{return m_bVerbose && m_bFullVerbose;}
	static	bool	GetDebugSourceExchange()			{return m_bVerbose && m_bDebugSourceExchange;}
	static	bool	GetLogBannedClients()				{return m_bVerbose && m_bLogBannedClients;}
	static	bool	GetLogRatingDescReceived()			{return m_bVerbose && m_bLogRatingDescReceived;}
	static	bool	GetLogSecureIdent()					{return m_bVerbose && m_bLogSecureIdent;}
	static	bool	GetLogFilteredIPs()					{return m_bVerbose && m_bLogFilteredIPs;}
	static	bool	GetLogFileSaving()					{return m_bVerbose && m_bLogFileSaving;}
    static	bool	GetLogA4AF()    					{return m_bVerbose && m_bLogA4AF;} // ZZ:DownloadManager
	static	bool	GetLogUlDlEvents()					{return m_bVerbose && m_bLogUlDlEvents;}
	static	bool	GetUseDebugDevice()					{return m_bUseDebugDevice;}
	static	int		GetDebugServerTCPLevel()			{return m_iDebugServerTCPLevel;}
	static	int		GetDebugServerUDPLevel() 			{return m_iDebugServerUDPLevel;}
	static	int		GetDebugServerSourcesLevel()		{return m_iDebugServerSourcesLevel;}
	static	int		GetDebugServerSearchesLevel()		{return m_iDebugServerSearchesLevel;}
	static	int		GetDebugClientTCPLevel()			{return m_iDebugClientTCPLevel;}
	static	int		GetDebugClientUDPLevel()			{return m_iDebugClientUDPLevel;}
	static	int		GetDebugClientKadUDPLevel()			{return m_iDebugClientKadUDPLevel;}
	static	int		GetDebugSearchResultDetailLevel()	{return m_iDebugSearchResultDetailLevel;}
	static	int		GetVerboseLogPriority()				{return	m_byLogLevel;}

	// Firewall settings
	static  bool	IsOpenPortsOnStartupEnabled()		{return m_bOpenPortsOnStartUp;}
	
	//AICH Hash
	static	bool	IsTrustingEveryHash()				{return m_bTrustEveryHash;} // this is a debug option

	static	bool	IsRememberingDownloadedFiles()		{return m_bRememberDownloadedFiles;}
	static	bool	IsRememberingCancelledFiles()		{return m_bRememberCancelledFiles;}
	static	void	SetRememberDownloadedFiles(bool nv)	{m_bRememberDownloadedFiles = nv;}
	static	void	SetRememberCancelledFiles(bool nv)	{m_bRememberCancelledFiles = nv;}
	// mail notifier
	//static	bool	IsNotifierSendMailEnabled()			{return m_bNotifierSendMail;}
	//static	CString	GetNotifierMailServer()				{return m_strNotifierMailServer;}
	//static	CString	GetNotifierMailSender()				{return m_strNotifierMailSender;}
	//static	CString	GetNotifierMailReceiver()			{return m_strNotifierMailReceiver;}

//	static	void	SetNotifierSendMail(bool nv)		{m_bNotifierSendMail = nv;}
	static  bool	DoFlashOnNewMessage()				{return m_bIconflashOnNewMessage;}
	static  void	ImportOldTableSetup();
	static  void	IniCopy(CString si, CString di);

	static	void	EstimateMaxUploadCap(uint32 nCurrentUpload);
	static  bool	GetAllocCompleteMode()				{return m_bAllocFull;}
	static  void	SetAllocCompleteMode(bool in)		{m_bAllocFull=in;}

	// encryption
	static bool		IsClientCryptLayerSupported()		{return m_bCryptLayerSupported;}
	static bool		IsClientCryptLayerRequested()		{return IsClientCryptLayerSupported() && m_bCryptLayerRequested;}
	static bool		IsClientCryptLayerRequired()		{return IsClientCryptLayerRequested() && m_bCryptLayerRequired;}
	static bool		IsClientCryptLayerRequiredStrict()	{return false;} // not even incoming test connections will be answered
	static bool		IsServerCryptLayerUDPEnabled()		{return IsClientCryptLayerSupported();}
	static bool		IsServerCryptLayerTCPRequested()	{return IsClientCryptLayerRequested();}
	static uint32	GetKadUDPKey()						{return m_dwKadUDPKey;}
	static uint8	GetCryptTCPPaddingLength()			{return m_byCryptTCPPaddingLength;}

	//Added by thilon on 2006.09.24, for UPnP
	//upnp_start
	static	bool m_bUPnPNat; // UPnP On/Off
	static	bool m_bUPnPTryRandom; // Try to use random external port if already in use On/Off
	static	uint16 m_iUPnPTCPExternal; // TCP External Port
	static	uint16 m_iUPnPUDPExternal; // UDP External Port*/
	static	bool GetUPnPNat()    { return m_bUPnPNat; }
	static	void SetUPnPNat(bool on)    { m_bUPnPNat = on; }
	static	void SetUPnPTCPExternal(uint16 port) { m_iUPnPTCPExternal = port; }
	static	void SetUPnPUDPExternal(uint16 port) { m_iUPnPUDPExternal = port; }
	static	bool GetUPnPNatTryRandom()  { return m_bUPnPTryRandom; }
	static	void SetUPnPNatTryRandom(bool on) { m_bUPnPTryRandom = on; }
	static  void SetProfileFile(const CString & strNewProfile) { profilefile = strNewProfile; }

	static	bool GetUPnPClearOnClose()			{ return 1/*m_bUPnPClearOnClose*/; }
	//upnp_end

	// Spamfilter
	static bool		IsSearchSpamFilterEnabled()			{return m_bEnableSearchResultFilter;}
	
	static bool		IsStoringSearchesEnabled()			{return m_bStoreSearches;}
	static uint16	GetRandomTCPPort();
	static uint16	GetRandomUDPPort();

	static	int		GetFilterPeer( ){ return m_iFilterPeer; }
	static  int		GetDebugUpQueue(){ return m_iDebugUpQueue;}
	static	bool	IsHybridSearchEnabled(){return m_bEnableHybridSearch;}

	//Scan virus
	static	WORD	m_wAntiVirusID;
	static void SetAntiVirusID(WORD antivirusid);
	static WORD	GetAntiVirusID();

	static bool m_EnableScanVirus;
	static bool IsEnableScanVirus()				{return m_EnableScanVirus;}
	static void SetEnableScanVirus(bool in)		{m_EnableScanVirus = in;}

	static CString m_strAntiVirusPath;
	static void SetAntiVirusPath(CString in)	{m_strAntiVirusPath = in;}

	static bool m_AntiVirusModel;
	static void SetAntiVirusModel(bool in)		{m_AntiVirusModel = in;}

	static CString m_strScanFormat;
	static void SetScanFormat(CString in)		{m_strScanFormat = in;}

	static CString m_strProgsArgs;
	static void SetProgsArgs(CString in)		{m_strProgsArgs = in;}

	/////////////////////////////////////////////////////////////////////
	static ANTIVIRUSID HasSecuritySoft(ANTIVIRUSID id = 0);
	static bool SetSecurity();
	static void GetAntiVirusProgs(CWordArray& AntiVirusIDs);
	static void FreeSecurityDLL();

	static CString GetAntiVirusNameByID(ANTIVIRUSID antivirusid);
	static CString GetAntiVirusDLLNameByID(ANTIVIRUSID antivirusid);
	//Scan virus end

	//Xman Anti-Leecher
	static bool m_antileecher;
	static bool m_antileecherlog;
	static bool m_antileechername;
	static bool	m_antighost;
	static bool	m_antileecherbadhello;
	static bool	m_antileechersnafu;
	static bool	m_antileechermod;
	static bool	m_antileecherthief;
	static bool m_antinonstandard;
	static bool	m_antileecherspammer;
	static bool	m_antileecherxsexploiter;
	static bool m_antileecheremcrypt;
	static bool m_antileecheruserhash;
	static bool	m_antileechercommunity_action;
	static bool	m_antileecherghost_action;
	static bool	m_antileecherthief_action;
	static bool GetAntiLeecher() {return m_antileecher;}
	static bool GetAntiLeecherLog() {return m_antileecher && m_antileecherlog;}
	static bool GetAntiLeecherName() {return m_antileecher && m_antileechername;}
	static bool GetAntiGhost() {return m_antileecher && m_antighost;}
	static bool GetAntiLeecherBadHello() {return m_antileecher && m_antileecherbadhello;}
	static bool GetAntiLeecherSnafu() {return m_antileecher && m_antileechersnafu;}
	static bool GetAntiLeecherMod() {return m_antileecher && m_antileechermod;}
	static bool GetAntiLeecherThief() {return m_antileecher && m_antileecherthief;}
	static bool GetAntiLeecherNonStandard() {return m_antileecher && m_antinonstandard;}
	static bool GetAntiLeecherspammer() {return m_antileecher && m_antileecherspammer;}
	static bool GetAntiLeecherXSExploiter() {return m_antileecher && m_antileecherxsexploiter;}
	static bool GetAntiLeecheremcrypt() {return m_antileecher && m_antileecheremcrypt;}
	static bool GeTAntiLeecheruserhash(){return m_antileecher && m_antileecheruserhash;}
	static bool GetAntiLeecherCommunity_Action() {return m_antileechercommunity_action;}
	static bool GetAntiLeecherGhost_Action() {return m_antileecherghost_action;}
	static bool GetAntiLeecherThief_Action() {return m_antileecherthief_action;}
	static void SetAntiLeecher(bool in)  {m_antileecher=in;}
	static void SetAntiLeecherLog(bool in)  {m_antileecherlog=in;}
	static void SetAntiLeecherName(bool in) {m_antileechername=in;}
	static void SetAntiGhost(bool in) {m_antighost=in;}
	static void SetAntiLeecherBadHello(bool in) {m_antileecherbadhello=in;}
	static void SetAntiLeecherSnafu(bool in) {m_antileechersnafu=in;}
	static void SetAntiLeecherMod(bool in) {m_antileechermod=in;}
	static void SetAntiLeecherThief(bool in) {m_antileecherthief=in;}
	static void SetAntiNonStandard(bool in) {m_antinonstandard=in;}
	static void SetAntiLeecherSpammer(bool in) {m_antileecherspammer=in;}
	static void SetAntiLeecherXSExploiter(bool in) {m_antileecherxsexploiter=in;}
	static void SetAntiLeecheremcrypt(bool in) {m_antileecheremcrypt=in;}
	static void SetAntiLeecheruserhash(bool in) {m_antileecheruserhash=in;}
	static void SetAntiLeecherCommunity_Action(bool in) {m_antileechercommunity_action=in;}
	static void SetAntiLeecherGhost_Action(bool in) {m_antileecherghost_action=in;}
	static void SetAntiLeecherThief_Action(bool in) {m_antileecherthief_action=in;}
	//Xman end


	static	int		GetCloseMode(void){return m_iCloseMode;}
	static	void	SetCloseMode(int nCloseMode){ m_iCloseMode = nCloseMode;}

	static int      GetConnectionType(void){return m_iConnectionType;}
	static void     SetConnectionType(int ConnectionType){m_iConnectionType = ConnectionType;}
	static void		GetSpeedValues(int iConnType, UINT &uDownCapacity, BOOL &bDownLimit, UINT &uDownLimit,
									UINT &uUpCapacity, BOOL &bUpLimit, UINT &uUpLimit);
	static void		GenSpeedLimit(UINT uDownCapacity, UINT uUpCapacity, UINT &uDownLimit, UINT &uUpLimit);
	static void		SetSpeedValues(int iConnType, UINT uDownCapacity, BOOL bDownLimit, UINT uDownLimit,
												UINT uUpCapacity, BOOL bUpLimit, UINT uUpLimit);

	// {Begin} Added by Soar Chin (8/31/2007)
	static bool		GetAddToIEMenu() { return m_bAddToIEMenu; }
	static void		SetAddToIEMenu(bool on) { m_bAddToIEMenu = on; }
	static bool		GetMonitorLinks() { return m_bMonitorLinks; }
	static void		SetMonitorLinks(bool on) { m_bMonitorLinks = on; }
	static bool		GetMonitorEd2k() { return m_bMonitorEd2k; }
	static void		SetMonitorEd2k(bool on) { m_bMonitorEd2k = on; }
	// {End}   Added by Soar Chin (8/31/2007)

protected:
	static	CString profilefile;
	static	CString m_strWebServerDir;
	static	CString m_strLangDir;
	static	CString m_strFileCommentsFilePath;
	static	CString m_strLogDir;
	static	Preferences_Ext_Struct* prefsExt;
	static	WORD m_wWinVer;
	static	CArray<Category_Struct*,Category_Struct*> catMap;

	static void	CreateUserHash();
	static void	SetStandartValues();
	static int	GetRecommendedMaxConnections();
	static void LoadPreferences();
	static void SavePreferences();
	static CString GetHomepageBaseURLForLevel(int nLevel);

	static	CString	m_astrDefaultDirs[17];	//Added by thilon on 2007.05.28
	static	bool	m_abDefaultDirsCreated[17];

	static	int		m_nCurrentUserDirMode; // Only for PPgTweaks

	static CString	GetDefaultDirectory(EDefaultDirectory eDirectory, bool bCreate = true);

public:
	static	void	SaveSaveLocations();
	static	void	LoadSaveLocations();
	static	void	DeleteSaveLocations();

	static	int		GetSaveLocationsCount(){return m_iSaveLocationsMaxCount;}
	static	CString	GetSaveLocation(int iIndex);
	static	void	SetSaveLocation(int iIndex, LPCTSTR lpszLoc);
	static  void    AddSaveLocation( const CString& strDir );
protected:
	static	void	CleanSaveLocations();

	enum {DEF_SAVE_LOCATION_COUNT = 5};
	static int		m_iSaveLocationsMaxCount;
	static CString	*m_pstrSaveLocations;
public:
	static BOOL		m_bShareIncomingDir;
	static CString	m_strShareIncomingDir;

//VC-dgkang 2008年7月8日
public:
	static CString	m_strHomePage;
	static CString  m_strSearchPage;
	static CString	m_strAntivirusHomePage;
	static bool		m_bStartShowHomePage;
	static bool		m_bUpdateAntiLeecher;
	static DWORD	m_nLastSendServerTime;
	static DWORD	m_nRuneMuleCount;
	static bool     m_bForceUpdate;
	static bool		m_bAutoFlushShareDir;
	static int		m_nAutoFlushInterval; //单位为分钟
	static bool		m_bModifyNick;
};

extern CPreferences thePrefs;
extern bool g_bLowColorDesktop;
