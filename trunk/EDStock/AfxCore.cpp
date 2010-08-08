/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		some afx-global-functions;
*/

#include	"stdafx.h"

#include	<ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif


CFont * AfxSelectDCFont( CDC * pDC, int nHeight )
{
	static	CFont	font;
	font.DeleteObject();
	font.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
		"楷体" );
	return	pDC->SelectObject( &font );
}

//////////////////////////////////////////////////////////////////////////////
// 注册表读写函数

BOOL AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

LONG AfxGetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

// recursively remove a registry key if and only if it has no subkeys
BOOL AfxDeleteRegKey(LPCTSTR lpszKey)
{
	// copy the string
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);
	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	// work until the end of the string
	while (lpszLast != NULL)
	{
		*lpszLast = '\0';
		lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

		// try to open that key
		HKEY hKey;
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
			break;

		// enumerate the keys underneath
		TCHAR szScrap[_MAX_PATH+1];
		DWORD dwLen = sizeof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
			bItExists = TRUE;
		::RegCloseKey(hKey);

		// found one?  quit looping
		if (bItExists)
			break;

		// otherwise, delete and find the previous backwhack
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}

	// release the string and return
	free(lpszKeyCopy);
	return TRUE;
}

HKEY AfxGetSectionKey( HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection )
{
	HKEY hGroupKey = NULL;
	HKEY hCompanyKey = NULL;
	HKEY hAppKey = NULL;
	HKEY hSectionKey = NULL;
	if (RegOpenKeyEx( hKey, szRegKeyGroup, 0, KEY_WRITE|KEY_READ,
		&hGroupKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hGroupKey, szRegKeyCompany, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS )
		{
			RegCreateKeyEx(hCompanyKey, szRegKeyApp, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hGroupKey != NULL)
		RegCloseKey(hGroupKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);

	DWORD dw;
	if( NULL == hAppKey )
		return FALSE;
	RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hAppKey);
	
	return hSectionKey;
}

BOOL AfxRegSetValue( LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue )
{
	ASSERT( lpszSection && lpszEntry && lpszValue );
	if( !lpszSection || !lpszEntry || !lpszValue )
		return FALSE;
	
	HKEY hSectionKey = AfxGetSectionKey( HKEY_LOCAL_MACHINE, _T("Software"), szRegKeyCompany, szRegKeyApp, lpszSection );

	if (hSectionKey == NULL)
		return FALSE;
	LONG lResult = RegSetValueEx(hSectionKey, lpszEntry, NULL, REG_SZ,
		(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
	RegCloseKey(hSectionKey);

	return lResult == ERROR_SUCCESS;
}

CString	AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = AfxGetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, lpszSection);
	if (hSecKey == NULL)
		return lpszDefault;
	CString strValue;
	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
		NULL, &dwCount);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
		strValue.ReleaseBuffer();
	}
	RegCloseKey(hSecKey);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		return strValue;
	}
	return lpszDefault;
}

BOOL AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp,
					   CStringArray &astrName, CStringArray &astrData )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = AfxGetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, "");
	if (hSecKey == NULL)
		return FALSE;
	
	int	nIndex	=	0;
	while( TRUE )
	{
		CString strName, strData;
		CHAR szName[MAX_PATH];
		BYTE szData[MAX_PATH];
		DWORD dwType, dwNameLen=sizeof(szName), dwDataLen=sizeof(szData);
		if( ERROR_SUCCESS != RegEnumValue(hSecKey, nIndex, szName, &dwNameLen,
				NULL, &dwType, szData, &dwDataLen ) )
			break;
		if( dwType == REG_SZ )
		{
			astrName.Add( (LPCTSTR)szName );
			astrData.Add( (LPCTSTR)szData );
		}
		nIndex	++;
	}

	RegCloseKey(hSecKey);
	return astrName.GetSize() > 0;
}

HINSTANCE AfxGotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR) {

		if (AfxGetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
			lstrcat(key, _T("\\shell\\open\\command"));

			if (AfxGetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {                     // No quotes found
					pos = strstr(key, _T("%1"));       // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';                   // Remove the parameter
				}
				else
					*pos = '\0';                       // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec(key,showcmd);
			}
		}
	}

	return result;
}

BOOL AfxRegisterActiveXCtrls( LPCTSTR lpszOcxFileName )
{
	HINSTANCE hLib = LoadLibrary( lpszOcxFileName );
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		return FALSE; 
  
	FARPROC   lpfnRegister;
	(FARPROC&)lpfnRegister = ::GetProcAddress( hLib, _T("DllRegisterServer") );
  
	if (lpfnRegister!= NULL)
	{
		(*lpfnRegister)();//Register control
		return TRUE;
	}
	return FALSE;
}

CString AfxGetVersionString( )
{
	static	CString	strVersion;
	if( strVersion.IsEmpty() )
	{
		strVersion	=	(LPCTSTR)AfxGetProfile().GetVersion( );
	}
	return strVersion;
}

CString AfxGetFileTitle( LPCTSTR lpszPathName )
{
	CString	strPathName	=	lpszPathName;
	if( strPathName.IsEmpty() )
		return "";
	UINT nTitleLen = AfxGetFileTitle( strPathName, NULL, 0 );
	if( nTitleLen <= 0 || nTitleLen-1 > (UINT)strPathName.GetLength() )
		return "";

	CString strTitle = strPathName.Right( nTitleLen-1 );
	return strTitle;
}

CString AfxGetFileTitleNoExt( LPCTSTR lpszPathName )
{
	CString	strPathName	=	lpszPathName;
	if( strPathName.IsEmpty() )
		return "";
	UINT nTitleLen = AfxGetFileTitle( strPathName, NULL, 0 );
	if( nTitleLen <= 0 || nTitleLen-1 > (UINT)strPathName.GetLength() )
		return "";

	CString strTitle = strPathName.Right( nTitleLen-1 );
	int nIndex = strTitle.ReverseFind( '.' );
	if( -1 != nIndex )
		strTitle = strTitle.Left( nIndex );
	return strTitle;
}

CString AfxGetFileDirectoryExist( LPCTSTR lpszPathName, LPCTSTR lpszDefault )
{
	CString	strPathName	=	lpszPathName;
	CString	strDefault	=	lpszDefault;
	if( strPathName.IsEmpty() )
		return strDefault;

	CString	strResult	=	strPathName;
	while( !strResult.IsEmpty()
		&& FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( strResult ) )
	{
		int	nIndex	=	strResult.ReverseFind( '\\' );
		if( -1 == nIndex )
			nIndex	=	strResult.ReverseFind( '/' );
		if( -1 == nIndex )
		{
			strResult = strDefault;
			break;
		}
		strResult	=	strResult.Left( nIndex );
	}

	if( strResult.GetLength() <= 0 )
		return strDefault;

	int	nLen	=	strResult.GetLength();
	if( nLen > 0 && strResult[nLen-1] != '\\' && strResult[nLen-1] != '/' )
		strResult	+=	STRING_DIRSEP;

	return strResult;
}

////////////////////////////////////////////////////////////////////////////////////
// Chinese To English Translation

BOOL AfxIsEnglish( const char * sz, int maxlen, float ratio )
{
	UINT nAll = 0, nChCount = 0;
	int len = 0;
	while( *sz && len < maxlen )
	{
		if( IsDBCSLeadByte(BYTE(*sz)) )
			nChCount ++;
		nAll ++;
		sz ++;
		len ++;
	}
	if( ratio && nAll && nChCount )
	{
		return ((float)nChCount/(float)nAll) < ratio;
	}

	return 0 == nChCount;
}

////////////////////////////////////////////////////////////////////////////////////
// global data
CStProfile &AfxGetProfile()
{
	static	CStProfile		g_stprofile;
	return g_stprofile;
}

CStockContainer &AfxGetStockContainer( )
{
	static	CStockContainer		g_stockcontainer;
	return g_stockcontainer;
}

CDomainContainer &AfxGetDomainContainer( )
{
	static	CDomainContainer	g_domaincontainer;
	return g_domaincontainer;
}

CDomainContainer &AfxGetGroupContainer( )
{
	static	CDomainContainer	g_groupcontainer;
	if( g_groupcontainer.GetSize() == 0 )
	{
		// Add Default
		CDomain	domain;
		domain.m_strName	=	domain_self;
		g_groupcontainer.AddDomain( domain );
	}
	return g_groupcontainer;
}

static	CStDatabase * g_pdb = NULL;
void AfxSetDB(CStDatabase * pdb)
{
	g_pdb = pdb;
}

CStDatabase & AfxGetDB()
{
	if( g_pdb )	return *g_pdb;

	static	CStDatabase	g_stdatabase;
	return g_stdatabase;
}

CStock & AfxGetStockMain( DWORD dwMarket )
{
	static	CStock	g_stockMain;
	if( !g_stockMain.GetStockInfo().IsValidStock() )
		g_stockMain.SetStockCode( CStock::marketSHSE, STKLIB_CODE_MAIN );

	if( CStock::marketSHSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketSZSE == dwMarket )
	{
		static	CStock	g_stockMain2;
		if( !g_stockMain2.GetStockInfo().IsValidStock() )
			g_stockMain2.SetStockCode( CStock::marketSZSE, STKLIB_CODE_MAINSZN );
		return g_stockMain2;
	}
	else if( CStock::marketCYSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketCHNA == dwMarket )
		return g_stockMain;
	else if( CStock::marketHKEX == dwMarket )
		return g_stockMain;
	else if( CStock::marketTBSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketTKSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketLSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketFLKFSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketNYSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketNASDAQ == dwMarket )
		return g_stockMain;
	else
		return g_stockMain;
}

//////////////////////////////////////////////////////////////////////////////////
// database functions
BOOL AfxInitializeDB( LPSTR lpErr, UINT nMaxSize )
{
	if( lpErr && nMaxSize > 0 )
		memset( lpErr, 0, nMaxSize );

	if( !CStDatabase::CreateSelfDB( AfxGetProfile().GetSelfDBPath() ) )
	{
		if( lpErr )	strncpy( lpErr, db_errcreateselfdb, min(nMaxSize-1,strlen(db_errcreateselfdb)) );
		return FALSE;
	}

	if( !AfxGetDB().SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		if( lpErr )	strncpy( lpErr, db_errrootpath, min(nMaxSize-1,strlen(db_errrootpath)) );
		return FALSE;
	}

	CSPString	strQianlongPath	=	AfxGetProfile().GetQianlongPath();
	if( strQianlongPath.GetLength() > 0
		&& !AfxGetDB().AddAssistantRootPath( strQianlongPath, IStStore::dbtypeQianlong ) )
	{
		if( lpErr )	strncpy( lpErr, db_errqianlongpath, min(nMaxSize-1,strlen(db_errqianlongpath)) );
		return FALSE;
	}

	return TRUE;
}

BOOL AfxReleaseDB( )
{
	AfxGetDB().SetRootPath( NULL );
	AfxGetDB().RemoveAllAssistant( );
	return TRUE;
}

BOOL AfxReloadStockMain( )
{
	static CSPMutex	g_mutexStockMain;
	CSPMutex::Scoped	l(g_mutexStockMain);

	AfxReloadStock( AfxGetStockMain(CStock::marketSHSE) );
	AfxReloadStock( AfxGetStockMain(CStock::marketSZSE) );
	return TRUE;
}

BOOL AfxReloadStock( CStock & stock )
{
	CStockInfo info = stock.GetStockInfo();

	// Reload AfxGetStockMain()
	AfxGetStockContainer().GetStockInfo( info.GetStockCode(), &info );

	stock.Clear( );
	stock.SetStockInfo( &info );
	stock.SetDatabase( &AfxGetDB() );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay );
	
	// Merge New
	stock.GetKDataDay().MergeKData( &(info.m_kdata) );
	
	stock.PrepareData( CStock::dataK, CKData::ktypeWeek );
	stock.PrepareData( CStock::dataK, CKData::ktypeMonth );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin5 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin15 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin30 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin60 );
	return TRUE;
}

BOOL AfxPrepareStockData( CStDatabase * pDatabase, CStock &stock, int nKType, int nKFormat, int nMaindataType, BOOL bFullFill, BOOL bReload )
{
	CStockInfo	info;
	if( !AfxGetStockContainer().GetStockInfo( stock.GetStockCode(), &info ) )
		info	=	stock.GetStockInfo( );

	stock.SetDatabase( pDatabase );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay, bReload );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin5, bReload );
	stock.PrepareData( CStock::dataDR, CKData::ktypeDay, bReload );
	if( bReload || stock.GetKDataDay().GetCurFormat() == CKData::formatOriginal )
		stock.GetKDataDay().MergeKData( &(info.m_kdata) );
	
	CKData	& kday	=	stock.GetKData(CKData::ktypeDay);
	CKData	& kdata	=	stock.GetKData( nKType );
	kdata.SetDRData( stock.GetDRData() );
	kday.SetDRData( stock.GetDRData() );
	if( bFullFill )
		kday.FullFillKData( AfxGetStockMain().GetKDataDay(), FALSE );
	kday.ChangeCurFormat( nKFormat, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
	if( CKData::ktypeWeek == nKType || CKData::ktypeMonth == nKType )
	{
		// 周线和月线不能ChangeCurFormat(...)
		if( CKData::formatOriginal == nKFormat )
			stock.PrepareData( CStock::dataK, nKType, bReload );
		else
			stock.ExtractKData( nKType, TRUE );
	}
	else if( CKData::ktypeDay != nKType )
	{
		stock.PrepareData( CStock::dataK, nKType, bReload );
		kdata.ChangeCurFormat( nKFormat, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
	}
	kdata.SetMaindataType( nMaindataType );
	return TRUE;
}

int AfxRefreshStockContainerMinute( CStockContainer & container, LONG lStockType, int nSecondsToRead )
{
	container.Lock();

	time_t	tmTradeLatest	=	-1;

	int	nCount = 0;
	for( int i=0; i<container.GetSize(); i++ )
	{
		CStockInfo & info = container.ElementAt(i);
		if( info.GetType() != lStockType )
			continue;

		CStock	stock;
		stock.SetStockInfo( &info );
		stock.SetDatabase( &AfxGetDB() );
		AfxGetDB().LoadMinute( &stock );
		info.m_minute.RemoveAll();
		
		CMinute	& minstock = stock.GetMinute();
		if( -1 == tmTradeLatest && stock.GetMinute().GetSize() > 0 )
			tmTradeLatest	=	CSPTime::GetTimeTradeLatest( minstock[0].m_time );
		for( int i=0; i<minstock.GetSize(); i++ )
		{
			if( -1 == nSecondsToRead || tmTradeLatest-minstock[i].m_time<nSecondsToRead )
				info.m_minute.Add( minstock[i] );
		}
		
		nCount	++;
	}
	container.UnLock();
	return nCount;
}

//////////////////////////////////////////////////////////////////////////////////
// usful functions
CSPString AfxGetTimeString( time_t time, LPCTSTR lpszTimeFmt, BOOL bAddWeekDay )
{
	SP_ASSERT( lpszTimeFmt );
	if( !lpszTimeFmt || strlen(lpszTimeFmt) == 0 )
		return "";

	CSPTime	sptime(time);
	CSPString strTime = sptime.Format( lpszTimeFmt );

	if( bAddWeekDay )
	{
		int	nDayOfWeek	=	sptime.GetDayOfWeek();
		switch( nDayOfWeek )
		{
		case 1:	strTime	+=	sz_sunday;		break;
		case 2:	strTime	+=	sz_monday;		break;
		case 3:	strTime	+=	sz_tuesday;		break;
		case 4:	strTime	+=	sz_wednesday;	break;
		case 5:	strTime	+=	sz_thursday;	break;
		case 6:	strTime	+=	sz_friday;		break;
		case 7:	strTime	+=	sz_saturday;	break;
		default:;
		}
	}
	return strTime;
}

CSPString AfxGetReportTypeString( int reporttype )
{
	switch( reporttype )
	{
		case CStock::reportQuarter:
			return sz_reportQuarter;
		case CStock::reportMid:
			return sz_reportMid;
		case CStock::reportQuarter3:
			return sz_reportQuarter3;
		case CStock::reportAnnals:
			return sz_reportAnnals;
		default:
			return "";
	}
}

CSPString AfxGetFileDirectory( CSPString strPathName, CSPString strDefault )
{
	if( strPathName.IsEmpty() )
		return strDefault;

	CSPString	strResult	=	strPathName;
	if( strResult.GetLength() > 0 )
	{
		int	nIndex	=	strResult.ReverseFind( '\\' );
		if( -1 == nIndex )
			nIndex	=	strResult.ReverseFind( '/' );
		if( -1 == nIndex )
			strResult = strDefault;
		else
			strResult	=	strResult.Left( nIndex );
	}

	if( strResult.GetLength() <= 0 )
		return strDefault;

	int	nLen	=	strResult.GetLength();
	if( nLen > 0 && strResult[nLen-1] != '\\' && strResult[nLen-1] != '/' )
		strResult	+=	STRING_DIRSEP;

	return strResult;
}

CSPString	AfxGetFilePath( CSPString strDir, CSPString strFileTitle )
{
	if( strDir.GetLength() > 0 && ( strDir[strDir.GetLength()-1] == '\\' || strDir[strDir.GetLength()-1] == '/' ) )
		strDir	=	strDir.Left( strDir.GetLength()-1 );
	strDir	+=	STRING_DIRSEP;
	return ( strDir + strFileTitle );
}

