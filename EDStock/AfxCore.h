/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		some afx-global-functions
*/

#if	!defined( __STKLIB_AFXCORE_H__ )
#define	__STKLIB_AFXCORE_H__

#include "SpString.h"
#include "Profile.h"

//////////////////////////////////////////////////////////////////////
// The following is implemented by MFC
UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
void AFXAPI AfxGetModuleShortFileName(HINSTANCE hInst, CString& strShortName);

BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile,
							   LPTSTR pszPath, int cchPath);

/////////////////////////////////////////////////////////////////////
// The Following is command functions

// ���ݵ�ǰ���Ժ͸߶�ѡ��pDC��Ӧ��Font
CFont * AfxSelectDCFont( CDC * pDC, int nHeight );

// Register Function
	BOOL AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	LONG AfxGetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	BOOL AfxDeleteRegKey(LPCTSTR lpszKey);
	HKEY AfxGetSectionKey(HKEY hKey, LPCTSTR szRegKeyGroup, 
								  LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection );
	BOOL AfxRegSetValue( LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );
	CString	AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, 
									  LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault );
	BOOL AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp,
								   CStringArray &astrName, CStringArray &astrData );


// Goto URL functions
	HINSTANCE AfxGotoURL(LPCTSTR url, int showcmd);

// ActiveX Functions
	BOOL AfxRegisterActiveXCtrls( LPCTSTR lpszOcxFileName );

	CString	AfxGetVersionString( );
	CString AfxGetFileTitle( LPCTSTR lpszPathName );
	CString AfxGetFileTitleNoExt( LPCTSTR lpszPathName );
	CString	AfxGetFileDirectoryExist( LPCTSTR lpszPathName, LPCTSTR lpszDefault );
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// Chinese To English Translation
	BOOL AfxIsEnglish( const char * sz, int maxlen, float ratio = 0.0 );
	CSPString AfxFaintlyChsToEnu( const char * szCh );
	CSPString AfxMakeSpellCode( CSPString & strSrc, int nMode = 0x0001 );

////////////////////////////////////////////////////////////////////////////////////
// global data
	CStProfile &		AfxGetProfile( );
	CStockContainer &	AfxGetStockContainer( );
	CDomainContainer &	AfxGetDomainContainer( );
	CDomainContainer &	AfxGetGroupContainer( );
	CStDatabase &		AfxGetDB();
	void AfxSetDB(CStDatabase * pdb);
	CStock &			AfxGetStockMain(DWORD dwMarket=CStock::marketSHSE);

////////////////////////////////////////////////////////////////////////////////////
// database functions
	BOOL	AfxInitializeDB( LPSTR lpBuf, UINT nMaxSize );
	BOOL	AfxReleaseDB( );
	BOOL	AfxReloadStockMain( );
	BOOL	AfxReloadStock( CStock & stock );
	BOOL	AfxPrepareStockData( CStDatabase * pDatabase, CStock &stock, int nKType, int nKFormat, int nMaindataType, BOOL bFullFill, BOOL bReload );
	int		AfxRefreshStockContainerMinute( CStockContainer & container, LONG lStockType, int nSecondsToRead = -1 );

////////////////////////////////////////////////////////////////////////////////////
// usful functions
	CSPString	AfxGetTimeString( time_t time, LPCTSTR lpszTimeFmt, BOOL bAddWeekDay );
	CSPString	AfxGetReportTypeString( int reporttype );
	CSPString	AfxGetFileDirectory( CSPString strPathName, CSPString strDefault );
	CSPString	AfxGetFilePath( CSPString strDir, CSPString strFileTitle );


////////////////////////////////////////////////////////////////////////////////////
// ���º���ʵ������ Strings.cpp

	CSPString	AfxGetKTypeString( int ktype );
	CSPString	AfxGetMaindataTypeString( int type );

	CSPString	AfxGetSTTName( UINT nTech );		// ��������ָ��Ψһ��ţ��õ�ָ����������
	CSPString	AfxGetSTTShortName( UINT nTech );	// ��������ָ��Ψһ��ţ��õ�ָ��Ӣ�ļ��
	CSPString	AfxGetSTTFullName( UINT nTech );	// ��������ָ��Ψһ��ţ��õ�ָ����������+Ӣ�ļ��
	CSPString	AfxGetSTTClassName( UINT nTech );	// ��������ָ��Ψһ��ţ��õ�ָ���������࣬�磺�����ࡢ������
	CSPString	AfxGetIntensityString( int nIntensity );	// ���������ź�ֵ���õ�����˵��
	CSPString	AfxGetIntensityCodeString( UINT nCode );	// ���������ź�����˵��ֵ���õ�����˵��

	CSPString	AfxGetSLHTitle( UINT nSLH );		// ������Ʊ�б��кţ�����������
	CSPString	AfxGetSLHDescript( UINT nSLH );		// ������Ʊ�б��кţ�������˵��

	void	AfxGetAllDistrict( CSPStringArray & astr );	// ������й��Һ͵��������б�

#define		ACCE_MIN	1
#define		ACCE_01		1	// �ȼ�F1
#define		ACCE_02		2	// �ȼ�F2
#define		ACCE_03		3	// �ȼ�F3
#define		ACCE_04		4	// �ȼ�F4
#define		ACCE_05		5	// �ȼ�F5
#define		ACCE_06		6	// �ȼ�F6
#define		ACCE_07		7	// �ȼ�F7
#define		ACCE_08		8	// �ȼ�F8
#define		ACCE_09		9	// �ȼ�F9
#define		ACCE_10		10	// �ȼ�F10
#define		ACCE_1		11	// ��A����
#define		ACCE_2		12	// ��B����
#define		ACCE_3		13	// ��A����
#define		ACCE_4		14	// ��B����
#define		ACCE_5		15	// ��ծ����
#define		ACCE_6		16	// ��ծ����
#define		ACCE_61		17	// ��A�ǵ�
#define		ACCE_62		18	// ��B�ǵ�
#define		ACCE_63		19	// ��A�ǵ�
#define		ACCE_64		20	// ��B�ǵ�
#define		ACCE_65		21	// ��ծ�ǵ�
#define		ACCE_66		22	// ��ծ�ǵ�
#define		ACCE_71		23	// ��֤��Ϣ
#define		ACCE_72		24	// ��֤��Ϣ
#define		ACCE_73		25	// ȯ����Ϣ
#define		ACCE_81		26	// ��A�ۺ�
#define		ACCE_82		27	// ��B�ۺ�
#define		ACCE_83		28	// ��A�ۺ�
#define		ACCE_84		29	// ��B�ۺ�
#define		ACCE_85		30	// ��ծ�ۺ�
#define		ACCE_86		31	// ��ծ�ۺ�
#define		ACCE_LBDK	32	// ��ʾ��ʱ�������ȶ��ָ��
#define		ACCE_MMLD	33	// ��ʾ��ʱ������������ָ��
#define		ACCE_MAX	33
	CSPString	AfxGetAccelerator( UINT nAcce, int nCharLeft = 8 );	// ������ݼ���ţ����ؿ�ݼ�������������


// End
////////////////////////////////////////////////////////////////////////////////////

#endif // __STKLIB_AFXCORE_H__
