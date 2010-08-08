/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CStdatabase;
*/

#if	!defined( __STKLIB_DATABASE_H__ )
#define	__STKLIB_DATABASE_H__

#include	"StdAfx.h"
#include	"Stock.h"

#define	MAX_DATATYPENAME	128

class CDBType
{
public:
	CDBType( );

	int type;
	char name[MAX_DATATYPENAME];
};

#pragma	pack(1)

////////////////////////////////////////////////////////////////////
//	ͨ�����ݰ������ݽṹ(*.STK)
typedef	struct tyday_fheader	{
	char	magic;
	char	code[6];
	char	magic2;
	char	name[8];
	DWORD	date;		//����
	DWORD	datebegin;	//��ʼ����
	DWORD	dateend;	//��ֹ����
	DWORD	gendate;	//��������
	DWORD	gentime;	//����ʱ��
	DWORD	from;		//��Դ
	DWORD	sharecount;	//��Ʊ��
	DWORD	recordcount;//��¼��
}TYDAY_FHEADER;

typedef	struct tyday_record	{
	char	magic;
	char	code[6];
	char	magic2;
	char	name[8];
	DWORD	date;		//����
	DWORD	open;		//����(Ԫ/1000)
	DWORD	high;		//��߼�(Ԫ/1000)
	DWORD	low;		//��ͼ�(Ԫ/1000)
	DWORD	close;		//����(Ԫ/1000)
	DWORD	amount;		//�ɽ���(ǧԪ)
	DWORD	volume;		//�ɽ���(��)
	DWORD	serial;		//��¼���
}TYDAY_RECORD;

////////////////////////////////////////////////////////////////////
//	���������ݰ������ݽṹ(*.DAD)
#define	FXJDAY_FHEADER_MAGIC	0x8C19FC33
typedef	struct fxjday_fheader	{
	DWORD	m_dwMagic;			// 33 FC 19 8C���ϰ汾��Magic
	DWORD	m_dwUnknown;		// δ֪
	DWORD	m_dwStockCount;		// Ϊ���ļ��Ĺ�Ʊ��
	DWORD	m_dwReserved;		// = 0
}FXJDAY_FHEADER;

typedef	union fxjday_record	{
	struct
	{
		DWORD	m_dwMagic;		// = -1
		WORD	m_wMarket;		// �г� SH, SZ
		char	m_szCode[6];
		DWORD	m_dwReserved[5];
	};
	struct
	{
		time_t	m_time;		//����
		float	open;		//����(Ԫ/1000)
		float	high;		//��߼�(Ԫ/1000)
		float	low;		//��ͼ�(Ԫ/1000)
		float	close;		//����(Ԫ/1000)
		float	volume;		//�ɽ���(��)
		float	amount;		//�ɽ���(Ԫ)
		float	reserved;	//
	};
}FXJDAY_RECORD;

#pragma	pack()

BOOL convert_TYDAY_RECORD_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd );
BOOL convert_TYDAY_RECORD_MIN_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd );
BOOL convert_KDATA_to_TYDAY_RECORD( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord );
BOOL convert_KDATA_to_TYDAY_RECORD_MIN( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord );

BOOL convert_FXJDAY_RECORD_to_KDATA( DWORD dwMarket, LPCTSTR lpszCode, int nKType, FXJDAY_RECORD *precord, KDATA *pkd );

typedef BOOL (CALLBACK* PROGRESS_CALLBACK)(DWORD dwCode, DWORD dwProgress, LPCTSTR lpszMsg, VOID * cookie);

#define	PROG_PROGRESS			1
#define	STKLIB_MAX_PROGRESS		10000
#define	STKLIB_MAXF_PROGRESS		10000.

class	CStockContainer;

/***
	��ȡK�����ݵ�ͨ�ýӿڣ����ָ�ʽ�Ķ�ȡK�����������Ӵ���̳�
*/
class IStStore
{
public:
	enum DBTypes {	// ���ݸ�ʽ���ͣ�Ŀǰֻ֧��typeSelfDB�����и�ʽ����typeQianlong��Ǯ����ʽ��
		dbtypeUnknown		=	0x01,
		dbtypeSelfDB		=	0x02,
		dbtypeQianlong		=	0x03,
		dbtypeShenglong		=	0x04,
	};

	static	BOOL IsValidDataType( int nType );
	static	int	GetSupportedDataType ( CDBType * pdbtype, int maxsize );
	static	IStStore * CreateStore( const char * rootpath, int nDBType = dbtypeUnknown );	// ������Ŀ¼�����ͣ��½�һ������

	virtual	int IsOK( ) { return m_bIsOK; }
	virtual	const char * GetRootPath( );	// �õ���ǰ����ĸ�Ŀ¼

	virtual	int	GetDBType( )	=	0;		// �õ���ǰ�������������
	virtual	const char * GetDBTypeName( )	=	0;	// �õ���ǰ�����������������
	virtual	int	GetMaxStockNumber( )	=	0;	// �õ���Ʊ����
	virtual	int	LoadCodetable( CStockContainer & container )	=	0;	// ��ȡ���й�Ʊ����Ϣ
	virtual	int	StoreCodetable( CStockContainer & container )	=	0;	// ��������
	virtual	int	LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )	=	0;	// ��ȡ���й�Ʊ������������ݻ���
	virtual	int	LoadBasetable( CStockContainer & container )	=	0;	// ��ȡĳһ��Ʊ�Ĳ������ϱ�����ÿ�����棬ÿ�ɾ��ʲ��ȣ���CBaseData
	virtual	int	StoreBasetable( CStockContainer & container )	=	0;	// ����ĳһ��Ʊ�Ĳ������ϱ�
	virtual	int	LoadBaseText( CStock *pstock )	=	0;					// ��ȡĳһ��Ʊ�Ļ��������ı�
	virtual	int	LoadKData( CStock *pstock, int nKType )	=	0;			// ��ȡĳһ��Ʊ��ĳ�����ڵ�K������
	virtual	int	LoadDRData( CStock *pstock )	=	0;					// ��ȡĳһ��Ʊ�ĳ�Ȩ��Ϣ����
	virtual	int StoreDRData( CStock *pstock )	=	0;					// ����ĳһ��Ʊ�ĳ�Ȩ��Ϣ����
	virtual int	LoadReport( CStock *pstock )	=	0;					// ��ȡĳһ��Ʊ������ˢ������
	virtual int	LoadMinute( CStock *pstock )	=	0;					// ��ȡĳһ��Ʊ�������ʱ����
	virtual int	LoadOutline( CStock *pstock )	=	0;					// ��ȡĳһ��Ʊ�������������
	virtual int	StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade )	=	0;		// ��������ˢ������
	virtual int	StoreMinute( MINUTE * pMinute, int nCount )	=	0;		// ���������ʱ����
	virtual int	StoreOutline( OUTLINE * pOutline, int nCount )	=	0;	// ���������ʱ����


	virtual	int	InstallCodetbl( const char * filename, const char *orgname )	=	0;	// ��װ���صĴ����
	virtual	int	InstallCodetblBlock( const char * filename, const char *orgname )	=	0;	// ��װ���صİ���
	virtual	int	InstallCodetblFxjBlock( const char * filename, const char *orgname )	=	0;	// ��װ���صķ����Ұ���
	virtual	int	InstallKData( CKData & kdata, BOOL bOverwrite = FALSE )	=	0;			// ��װK������
	virtual	int InstallKDataTy( const char * stkfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie )	=	0;	// ��װ���ص�K��ͨ�ø�ʽ���ݰ�
	virtual	int InstallKDataFxj( const char * dadfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie )	=	0;	// ��װ���ص�K�߷����Ҹ�ʽͨ�����ݰ�
	virtual	int InstallDRData( CDRData & drdata )	=	0;								// ��װ��Ȩ��Ϣ����
	virtual	int	InstallDRDataClk( const char * filename, const char *orgname )	=	0;	// ��װ���صĳ�Ȩ��Ϣ���ݣ�һֻ��Ʊһ���ļ�
	virtual	int	InstallDRDataFxj( const char * fxjfilename )	=	0;					// ��װ�����ҳ�Ȩ��Ϣ����
	virtual	int	InstallBasetable( const char * filename, const char *orgname )	=	0;	// ��װ��������
	virtual	int	InstallBasetableTdx( const char * filename )	=	0;					// ��װͨ���Ų�������
	virtual	int	InstallBasetableFxj( const char * filename )	=	0;					// ��װ�����Ҳ�������
	virtual	int InstallBaseText( const char * filename, const char *orgname )	=	0;	// ��װ���صĻ����������ݣ�һֻ��Ʊһ���ļ�
	virtual	int InstallBaseText( const char * buffer, int nLen, const char *orgname )	=	0;	// ��װ������������
	virtual	int InstallNewsText( const char * filename, const char *orgname )	=	0;	// ��װ���������ļ�
	virtual	int InstallNewsText( const char * buffer, int nLen, const char *orgname )	=	0;	// ��װ��������

	virtual	BOOL GetFileName( CSPString &sFileName, int nDataType,
				CStockInfo * pInfo = NULL, int nKType = CKData::ktypeDay )	=	0;		// �õ�ĳ�����ݵ��ļ�����
protected:
	BOOL	m_bIsOK;
	char	m_szRootPath[1024];
};

#define	ERR_DB_MAX	1000

/***
	���������࣬�ṩ�Դ������ݵĲ����ӿڣ�������Ʊ��Ϣ����������K�����ݣ�����
	���ϣ�ȨϢ���ϵĶ�ȡ�Ͱ�װ��
	
	ʹ�÷������£�
	CStDatabase::CreateSelfDB( "./" );

	CStDatabase	db;
	db.SetRootPath( "./", IStStore::dbtypeSelfDB );

	CStockContainer container;
	nLen	=	db.LoadCodetable( container );
	db.LoadBasetable( container );
	db.LoadKDataCache( container, NULL, NULL, 0, 0 );

	CStock	stock;
	stock.SetDatabase( &db );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay );
	stock.PrepareData( CStock::dataDR );
	stock.PrepareData( CStock::dataK, m_nCurKType );
	CStockInfo	& info	=	stock.GetStockInfo();
	CKData	& kday	=	stock.GetKDataDay();
	CKData	& kdata	=	stock.GetKData(m_nCurKType);
	kday.SetDRData( stock.GetDRData() );
	kday.ChangeCurFormat( CKData::formatXDRdown, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );

	ע�⣺ʹ��CStDatabase������CNetDatabaseҲ����ʵ���������ܣ������ṩ�Ĺ���
	���࣬ʹ�÷�����ͬ��
*/
class CStDatabase : public Object
{
public:
	// Constructors
	CStDatabase();
	virtual ~CStDatabase();

	// Flag values

	// Attributes

	// Operations
	static	BOOL CreateSelfDB( const char * rootpath );		// ��������Ŀ¼
	static	BOOL IsValidDataType( int nType );				// �����Ƿ�Ϸ�
	static	int	GetSupportedDataType ( CDBType * pdbtype, int maxsize );	// �õ�֧�ֵ���������
	BOOL	SetRootPath( const char * rootpath, int nDBType = IStStore::dbtypeUnknown );	// �趨����Ŀ¼

	BOOL	IsOK( );	// ����Ŀ¼�Ƿ����
	const char * GetRootPath( );	// �õ�����Ŀ¼
	int	GetDBType( );				// �õ���������
	const char * GetDBTypeName( );	// �õ�������������

	virtual	DWORD	GetSelfTempPath( char *szTempPath, int size );	// �õ���ʱĿ¼

	virtual	BOOL GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError);	// �õ��������
	virtual	int	GetMaxStockNumber( );		// �õ���Ʊ����
	virtual	int	LoadCodetable( CStockContainer &container );	// ��ȡ���й�Ʊ����Ϣ
	virtual	int	StoreCodetable( CStockContainer & container );	// ��������
	virtual	int	LoadKDataCache( CStockContainer &container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd );	// ��ȡ���й�Ʊ������������ݻ���
	virtual	int	LoadBasetable( CStockContainer & container );	// ��ȡĳһ��Ʊ�Ĳ������ϱ�����ÿ�����棬ÿ�ɾ��ʲ��ȣ���CBaseData
	virtual	int	StoreBasetable( CStockContainer & container );	// ����ĳһ��Ʊ�Ĳ������ϱ�
	virtual	int	LoadBaseText( CStock *pstock );					// ��ȡĳһ��Ʊ�Ļ��������ı�
	virtual	int	LoadKData( CStock *pstock, int nKType );		// ��ȡĳһ��Ʊ��ĳ�����ڵ�K������
	virtual int	LoadDRData( CStock *pstock );					// ��ȡĳһ��Ʊ�ĳ�Ȩ��Ϣ����
	virtual	int StoreDRData( CStock *pstock );					// ����ĳһ��Ʊ�ĳ�Ȩ��Ϣ����
	virtual int	LoadReport( CStock *pstock );					// ��ȡĳһ��Ʊ������ˢ������
	virtual int	LoadMinute( CStock *pstock );					// ��ȡĳһ��Ʊ�������ʱ����
	virtual int	LoadOutline( CStock *pstock );					// ��ȡĳһ��Ʊ�������������
	virtual int	StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade );	// ��������ˢ������
	virtual int	StoreMinute( MINUTE * pMinute, int nCount );	// ���������ʱ����
	virtual int	StoreOutline( OUTLINE * pOutline, int nCount );	// ���������ʱ����

	virtual	int	InstallCodetbl( const char * filename, const char *orgname );		// ��װ���صĴ����
	virtual	int	InstallCodetblBlock( const char * filename, const char *orgname );	// ��װ���صİ���
	virtual	int	InstallCodetblFxjBlock( const char * filename, const char *orgname );	// ��װ���صķ����Ұ���
	virtual	int	InstallKData( CKData & kdata, BOOL bOverwrite = FALSE );			// ��װK������
	virtual	int InstallKDataTy( const char * stkfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );	// ��װ���ص�K��ͨ�ø�ʽ����
	virtual	int InstallKDataFxj( const char * dadfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );	// ��װ���ص�K�߷����Ҹ�ʽ����
	virtual	int InstallDRData( CDRData & drdata );									// ��װ��Ȩ��Ϣ����
	virtual	int	InstallDRDataClk( const char * filename, const char *orgname );		// ��װ���صĳ�Ȩ��Ϣ���ݣ�һֻ��Ʊһ���ļ�
	virtual	int	InstallDRDataFxj( const char * fxjfilename );						// ��װ�����ҳ�Ȩ��Ϣ����
	virtual	int	InstallBasetable( const char * filename, const char *orgname );		// ��װ��������
	virtual	int	InstallBasetableTdx( const char * filename );						// ��װͨ���Ų�������
	virtual	int	InstallBasetableFxj( const char * filename );						// ��װ�����Ҳ�������
	virtual	int InstallBaseText( const char * filename, const char *orgname );		// ��װ���صĻ����������ݣ�һֻ��Ʊһ���ļ�
	virtual	int InstallBaseText( const char * buffer, int nLen, const char *orgname );		// ��װ������������
	virtual	int InstallNewsText( const char * filename, const char *orgname );		// ��װ���������ļ�
	virtual	int InstallNewsText( const char * buffer, int nLen, const char *orgname );		// ��װ��������


	// assistant operation
	BOOL	AddAssistantRootPath( const char * rootpath, int nDBType = IStStore::dbtypeUnknown );	// ָ������һ����������Դ��Ŀ¼���Զ���ȡ
	void	RemoveAssistant( const char * rootpath );	// �Ƴ�����һ����������ԴĿ¼
	void	RemoveAllAssistant(  );						// �Ƴ����и�������ԴĿ¼

	static CSPTime	GetTimeInitial( );		// �õ���ʼ��������
	BOOL	GetTimeLocalRange( CSPTime *ptmLatest, CSPTime * ptmPioneer, CSPTime * ptmInitial );	// �õ�����������������
	BOOL	GetNeedDownloadRange( CStockInfo &info, CSPTime tmBegin, CSPTime tmEnd, CSPTime &tmDLBegin, CSPTime &tmDLEnd );	// �õ���Ҫ���ص�������������

	// ��Ǯ����ʽK������ ���� ͨ�����ݰ�
	static	int		AppendToTyData( const char *code, const char *name, int nKType, LPCTSTR lpszKFile, LPCTSTR lpszTyDataFile, time_t tmBegin, time_t tmEnd );

protected:
	IStStore	* m_pStore;
	CSPPtrArray	m_aptrAssistant;
};

#endif	//__STKLIB_DATABASE_H__

