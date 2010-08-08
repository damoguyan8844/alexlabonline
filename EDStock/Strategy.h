/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CRateParam;
		class	CSPTimeContainer;
		class	COpParam;
		class	CStockOwnContainer;
		class	COpRecordContainer;
		class	CAssetSerialContainer;
		class	CTechStock;
		class	CTechStockContainer;
		class	CStrategy;
*/

#if	!defined( __STKLIB_STRATEGY_H__ )
#define	__STKLIB_STRATEGY_H__

/////////////////////////////////////////////////////////////////////////////
// CRateParam
/***
	����˰��������
*/
class  CRateParam
{
public:
	CRateParam( );

	BOOL	IsValid( );
	void	SetDefault( );
	void	Serialize( CSPArchive &ar );

	double	GetRate( CStockInfo & info );

	double	m_dShaa;
	double	m_dShab;
	double	m_dShafund;
	double	m_dSzna;
	double	m_dSznb;
	double	m_dSznfund;
};

/////////////////////////////////////////////////////////////////////////////
// CSPTimeContainer

//template class __declspec(dllexport)	CSPArray<CSPTime, CSPTime &>;
typedef	CSPArray<CSPTime, CSPTime &>	CSPTimeArray;

class		CSPTimeContainer : public CSPTimeArray
{
public:
	CSPTimeContainer() {}
	virtual ~CSPTimeContainer()	{	RemoveAll();	}

	// for access CSPTimeArray
	virtual	int		Add( CSPTime &tm )		{	return CSPTimeArray::Add( tm );	}
	virtual	void	RemoveAll( )			{	CSPTimeArray::RemoveAll();	}
};

/////////////////////////////////////////////////////////////////////////////
// COpParam

	CSPString	AfxGetLogicString( int nLogic );
	CSPString	AfxGetStoreDivString( int nStoreDiv );
/***
	����ģ�����������
*/
class  COpParam
{
public:
	COpParam( );

	// Attributes
	BOOL	IsValid( );
	void	SetDefault( );
	void	SetDefaultOfInvalidMember( );
	void	Serialize( CSPArchive &ar );

	// Operation
	BOOL	IsInTimeZones( CSPTime tm );
	CSPTime	GetBeginTime( );
	CSPTime	GetEndTime( );
	BOOL	GetNextTradeTime( CSPTime tmNow, CSPTime &tmNext );
	DWORD	GetProgress( CSPTime tmNow, DWORD dwProgressMax );

	enum storeDivs {
		storedivMin		=	0x01,
		storedivOnce	=	0x01,
		storedivTwice	=	0x02,
		storedivThird	=	0x03,
		storedivForth	=	0x04,
		storedivFifth	=	0x05,
		storedivSixth	=	0x06,
		storedivSeventh	=	0x07,
		storedivEighth	=	0x08,
		storedivNinth	=	0x09,
		storedivTenth	=	0x0A,
		storedivMax		=	0x0A,
	};

	enum Logical {
		logicMin	=	0x01,
		logicAnd	=	0x01,
		logicOr		=	0x02,
		logicMax	=	0x02,
	};

	int		m_nKType;
	int		m_nMaindataType;
	int		m_nBuyLogic;
	int		m_nBuyLimit;
	double	m_dBuyMulti;
	int		m_nSellLogic;
	int		m_nSellLimit;
	double	m_dSellMulti;
	CSPTimeContainer	m_atmBegin;
	CSPTimeContainer	m_atmEnd;
	long	m_nStartAmount;		//	Ԫ
	int		m_nStoreDiv;

	BOOL	m_bStopLosing;
	BOOL	m_bStopProfit;
	double	m_dStopLosing;
	double	m_dStopProfit;
	BOOL	m_bLongTrend;
	BOOL	m_bIndexTrend;
	int		m_nKTypeLong;
	int		m_nKTypeIndex;
};

/////////////////////////////////////////////////////////////////////////////
// stockown_t

#pragma pack(1)

typedef	struct stockown_t	{
	char	szCode[STKLIB_MAX_CODE];
	DWORD	dwShare;
	double	dBuyPrice;
	DWORD	dwMarket;
	char	szReserved[20];
} STOCKOWN, * LPSTOCKOWN;

//template class __declspec(dllexport)	CSPArray< STOCKOWN, STOCKOWN & >;
typedef	CSPArray< STOCKOWN, STOCKOWN & >	CStockOwnArray;

/***
	ӵ�й�Ʊ����
*/
class  CStockOwnContainer : public CStockOwnArray
{
public:
	virtual ~CStockOwnContainer()	{	RemoveAll();	}

	// for access CStockOwnArray
	virtual	void	RemoveAll( )			{	CStockOwnArray::RemoveAll();	}

	BOOL	AddStock( CStockInfo & info, DWORD dwShare, double dBuyPrice );
	BOOL	RemoveStock( CStockInfo & info, DWORD dwShare );
	BOOL	HasThisStock( CStockInfo & info, LPSTOCKOWN lpOwn );
};

/////////////////////////////////////////////////////////////////////////////
// oprecord_t

// ��������
#define	STRATEGY_OPTYPE_BUY			0x01	// ����
#define	STRATEGY_OPTYPE_SELL		0x02	// ����
#define	STRATEGY_OPTYPE_ADDSTOCK	0x03	// ��ӹ�Ʊ
#define	STRATEGY_OPTYPE_REMOVESTOCK	0x04	// �Ƴ���Ʊ
#define	STRATEGY_OPTYPE_ADDCASH		0x05	// ����ʽ�
#define	STRATEGY_OPTYPE_REMOVECASH	0x06	// ȡ���ʽ�

	CSPString	AfxGetStrategyOpTypeString( long lOpType );

typedef	struct oprecord_t	{
	long	lOpType;
	time_t	time;
	char	szCode[STKLIB_MAX_CODE];
	DWORD	dwShare;
	double	dSharePrice;
	double	dRateCost;
	long	bViewed;	// for next op use in realop. 
	DWORD	dwMarket;
	char	szReserved[24];
} OPRECORD, * LPOPRECORD;

//template class __declspec(dllexport)	CSPArray< OPRECORD, OPRECORD & >;
typedef	CSPArray< OPRECORD, OPRECORD & >	COpRecordArray;
/***
	������¼����
*/
class  COpRecordContainer : public COpRecordArray
{
public:
	virtual ~COpRecordContainer()	{	RemoveAll();	}

	// for access COpRecordArray
	virtual	int		Add( OPRECORD &newElement){	return COpRecordArray::Add(newElement);	}
	virtual	void	RemoveAt(int nIndex, int nCount = 1){	COpRecordArray::RemoveAt(nIndex,nCount);	}
	virtual	void	RemoveAll( )			{	COpRecordArray::RemoveAll();	}
	virtual	void	Copy(const COpRecordArray& src)	{	COpRecordArray::Copy(src);	}

	// operation
	int		AddRecord(	long lOpType, CSPTime tm, const char *szCode,
						DWORD dwShare, double dSharePrice, double dRateCost );
	int		AddRecordUniqueStock(long lOpType, CSPTime tm, const char *szCode,
					   DWORD dwShare, double dSharePrice, double dRateCost );
	int		GetBuyRecordCount();
	BOOL	HasBuyStock( const char * szCode );
	BOOL	StatResults( int *pnTimes, int *pnVictoryTimes, double *pdYieldMax, double *pdYieldMin );
};

typedef	struct assetserial_t	{
	time_t	time;
	double	dAsset;
	double	dCash;
	char	szReserved[32];
} ASSETSERIAL, * LPASSETSERIAL;

//template class __declspec(dllexport)	CSPArray< ASSETSERIAL, ASSETSERIAL & >;
typedef	CSPArray< ASSETSERIAL, ASSETSERIAL & >	CAssetSerialArray;
/***
	�ʲ�����
*/
class  CAssetSerialContainer : public CAssetSerialArray
{
public:
	virtual	~CAssetSerialContainer()	{	RemoveAll();	}

	// for access CAssetSerialArray
	virtual	void	RemoveAt(int nIndex, int nCount = 1){	CAssetSerialArray::RemoveAt(nIndex,nCount);	}
	virtual	void	RemoveAll( )			{	CAssetSerialArray::RemoveAll();	}
	virtual	void	Copy(const CAssetSerialArray& src)	{	CAssetSerialArray::Copy(src);	}

	// operation
	BOOL	SortInsert( ASSETSERIAL serial );
	BOOL	GetStdDev( double *pdStdDev, double * pdXiapu );
};

#pragma pack()

//////////////////////////////////////////////////////////////////////
// CTechStock
/***
	һֻ�������Ĺ�Ʊ��������Ʊ����Ϣ��K�����ݣ�������K�����ݣ����輼��ָ�꣬���賤���ڼ���ָ��
*/
class  CTechStock
{
public:
	CTechStock( );
	CTechStock( const CTechStock & src );
	virtual	~CTechStock( );

	CTechStock	& operator = ( const CTechStock &src );
	void	SetAutoDelete( BOOL bAutoDelete = TRUE );
	void	Clear( );

	BOOL	GetPriceOK( long lOpType, CSPTime tmCur, DWORD dwShare, double dSharePrice, double *pdPriceOK );
	BOOL	IsStopTrading( CSPTime tmCur );
	BOOL	GetClosePrice( CSPTime tmCur, double *pdPrice );
	BOOL	GetCloseDiffPercent( CSPTime tmCur, double *pdDiffPercent );

	CStockInfo		m_info;			// ��Ʊ��Ϣ
	CKData			m_kdata;		// ����K������
	CKData			m_kdataLong;	// ������K������
	CSPPtrArray		m_techs;		// ���輼��ָ��
	CSPPtrArray		m_techsLong;	// ���賤���ڼ���ָ��

protected:
	BOOL	m_bAutoDelete;
};

//template class __declspec(dllexport)	CSPArray< CTechStock, CTechStock & >;
typedef	CSPArray< CTechStock, CTechStock & >	CTechStockArray;
/***
	CTechStock����
*/
class  CTechStockContainer : public CTechStockArray
{
public:
	virtual	~CTechStockContainer( );

	// for access CTechStockArray
	virtual	void SetSize(int nNewSize, int nGrowBy = -1)	{	CTechStockArray::SetSize(nNewSize,nGrowBy);	}

	// operation
	void	Clear( );
	
	// Operation
	BOOL	GetShouldBuyShare(	CTechStock & techstock, CSPTime tmCur, double dCash, CRateParam &rate, COpParam &opparam, int nOwnedStockCount,
								CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice );
	BOOL	GetShouldSellShare(	CTechStock & techstock, CSPTime tmCur, STOCKOWN &own, COpParam &opparam,
								CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice );

	BOOL	GetClosePrice( const char * szCode, CSPTime tmCur, double * pdPrice );
	BOOL	GetSumAsset( CSPTime tmCur, CStockOwnContainer &container, double * pdAsset );
	BOOL	GetNextExistTradeTime( CSPTime tmCur, CSPTime & tmNext );

	CStockInfo		m_infoIndex;
	CKData			m_kdataIndex;
	CSPPtrArray		m_techsIndex;
};

/////////////////////////////////////////////////////////////////////////////
// CStrategy

typedef BOOL (CALLBACK* SIMULATION_CALLBACK)(DWORD dwType, DWORD dwValue, LPCTSTR lpszMsg, VOID * cookie);

#define	STRATEGY_MAX_TITLE			256

#define	SIMULATION_PROGRESS			1
#define	SIMULATION_YIELD			2

#define	STRATEGY_MAX_PROGRESS		10000
#define	STRATEGY_MAXF_PROGRESS	10000.

#define	STRATEGY_BASE_YIELD		10000
#define	STRATEGY_BASEF_YIELD		10000.

/***
	����
*/
class  CStrategy
{
public:
	CStrategy();
	virtual ~CStrategy();

	static CSPString	GetName( LPCTSTR lpszPath );
// Overrides
public:
	virtual	BOOL	OpenStrategyFile( LPCTSTR lpszPathName, LPSTR lpErr, UINT nMaxSize );
	virtual	BOOL	SaveStrategyFile( LPCTSTR lpszPathName );
	virtual BOOL	Serialize(CSPArchive& ar, LPSTR lpErr, UINT nMaxSize);
	virtual	BOOL	DoFileSave( );
	virtual	BOOL	OnClose( );
	virtual	void	OnRealOpViewed( );

protected:
	// �����ļ�·�������ƣ���д�������ļ���
	CSPString	m_strPathName;

// Attributes
public:
	void		SetPathName( LPCTSTR lpszPathName );
	CSPString	GetPathName( );
	void		SetName( LPCTSTR lpszName );
	CSPString	GetName( );
	void		SetDescript( LPCTSTR lpszDescript );
	CSPString	GetDescript( );
	CSPStringArray	&	GetStocks( );
	void				SetStocks( CSPStringArray & astr );
	void				AddStock( LPCTSTR lpszCode );
	void				RemoveStock( LPCTSTR lpszCode );
	CSPString			GetStockTechString( );
	CRateParam		&	GetRateParam( );
	COpParam		&	GetOpParam( );
	CTechParameters	&	GetTechParametersBuy( );
	CTechParameters	&	GetTechParametersSell( );
	CTechParameters	&	GetTechParametersLongTrend( );
	CTechParameters	&	GetTechParametersIndexTrend( );
	CSPDWordArray	&	GetSelectedTechsBuy( );
	CSPDWordArray	&	GetSelectedTechsSell( );
	CSPDWordArray	&	GetSelectedTechsLongTrend( );
	CSPDWordArray	&	GetSelectedTechsIndexTrend( );

// Operations
public:
	// Simulation
	enum	SimuStatus	{
		statusInit		=	0x01,
		statusRunning	=	0x02,
		statusPaused	=	0x03,
		statusFinished	=	0x04,
	};

	// Data Cache
	BOOL	PrepareData( SIMULATION_CALLBACK fnCallback, void * cookie );
	void	ClearLastIntensity( );
	void	ClearCache( );
	CTechStockContainer &	GetTechStockContainer( );
	BOOL	CanModifiedNow()	{	return ( SimuIsStatusInit() || SimuIsStatusFinished() );	}

	// Simulation Status
	int		SimuGetCurStatus( )		{	return m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusInit( )		{	return statusInit == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusRunning( )	{	return statusRunning == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusPaused( )	{	return statusPaused == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusFinished( )	{	return statusFinished == m_SimuCurrentStatus;	}
	void	SimuSetStatusInit( )	{	m_SimuCurrentStatus	=	statusInit;	}
	void	SimuSetStatusRunning( )	{	m_SimuCurrentStatus	=	statusRunning;	}
	void	SimuSetStatusPaused( )	{	m_SimuCurrentStatus	=	statusPaused;	}
	void	SimuSetStatusFinished( ){	m_SimuCurrentStatus	=	statusFinished;	}

	// Simulation
	void	SimuReset( );
	BOOL	SimuGotoNextTime( );
	CSPTime	SimuGetCurrentTime( );
	double	SimuGetCurrentCash( );
	BOOL	SimuOperate( OPRECORD record, BOOL bTimeStrict = TRUE );
	COpRecordContainer &	SimuGetOpRecord( );
	COpRecordContainer &	SimuGetNextOp( );
	CStockOwnContainer &	SimuGetStockOwn( );
	CAssetSerialContainer &	SimuGetAssetSerial( );
	DWORD	SimuGetCurrentProgress( DWORD dwProgressMax = STRATEGY_MAX_PROGRESS );	//	Not Same as Real
	double	SimuGetAsset( CSPTime tmCur );
	double	SimuGetCurrentYield( );
	double	SimuGetCurrentYieldIndexPercent( );
	double	SimuGetCurrentYieldPercent( );
	BOOL	SimuOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock );
	virtual	BOOL	SimuRun( SIMULATION_CALLBACK fnCallback, void * cookie );

	// Real
	void	RealReset( );
	BOOL	RealGotoNextTime( );
	CSPTime	RealGetBeginTime( );	//	Not Same as Simu
	CSPTime	RealGetCurrentTime( );
	double	RealGetCurrentCash( );
	BOOL	RealOperate( OPRECORD record, BOOL bTimeStrict = TRUE );
	COpRecordContainer &	RealGetOpRecord( );
	COpRecordContainer &	RealGetNextOp( );
	CStockOwnContainer &	RealGetStockOwn( );
	CAssetSerialContainer &	RealGetAssetSerial( );
	double	RealGetAsset( CSPTime tmCur );
	double	RealGetCurrentYield( );
	double	RealGetCurrentYieldIndexPercent( );
	double	RealGetCurrentYieldPercent( );
	BOOL	RealOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock );
	virtual	BOOL	RealRun( SIMULATION_CALLBACK fnCallback, void * cookie );
	BOOL	RealAddOpRecordStrict( OPRECORD record );	//	Not Same as Simu
	BOOL	RealDeleteOpRecordStrict( int nRecord );	//	Not Same as Simu

protected:
	// Properties
	DWORD			m_dwFileVersion;
	CSPString		m_strName;		// ��������
	CSPString		m_strDescript;	// ����˵��

	// Stocks
	CDomain			m_stocks;		// ���Ա�ѡ��Ʊ

	// Rate
	CRateParam		m_rate;			// ���׷���
	
	// Tech parameters
	CTechParameters	m_paramBuy;			// ����ָ�����
	CTechParameters	m_paramSell;		// ����ָ�����
	CTechParameters	m_paramLongTrend;	// ����ָ�����
	CTechParameters	m_paramIndexTrend;	// ָ��ָ�����
	CSPDWordArray	m_anTechsBuy;		// �������ݵ�ָ��
	CSPDWordArray	m_anTechsSell;		// �������ݵ�ָ��
	CSPDWordArray	m_anTechsLongTrend;	// �������������ݵ�ָ��
	CSPDWordArray	m_anTechsIndexTrend;// ָ���������ݵ�ָ��

	// Operate Parameters
	COpParam		m_opparam;		// ��������

	// Simulation
	int				m_SimuCurrentStatus;	// Not Same as Real, ��ʼ�����ڽ��У���ͣ�����
	CSPTime			m_SimuCurrentTime;		// ģ�⵱ǰʱ��
	double			m_SimuCurrentCash;		// ģ�⵱ǰ�ʽ�
	CStockOwnContainer	m_SimuStockOwn;		// ģ�⵱ǰӵ�й�Ʊ
	COpRecordContainer	m_SimuOpRecord;		// ģ���¼
	COpRecordContainer	m_SimuNextOp;		// ģ����һ������
	CAssetSerialContainer	m_SimuAssetSerial;	// ģ���ʲ�ֵ����

	// Real Operate
	CSPTime			m_RealBeginTime;		//	Not Same as Simu��ʵս��ʼʱ��
	CSPTime			m_RealCurrentTime;		// ʵս��ǰʱ��
	double			m_RealCurrentCash;		// ʵս��ǰ�ʽ�
	CStockOwnContainer	m_RealStockOwn;		// ʵս��ǰӵ�й�Ʊ
	COpRecordContainer	m_RealOpRecord;		// ʵս������¼
	COpRecordContainer	m_RealNextOp;		// ʵս��һ������
	CAssetSerialContainer	m_RealAssetSerial;	// ʵս�ʲ�ֵ����

	// Cache Data, Not Saved
	CTechStockContainer	m_techstocks;		// ��ѡ��Ʊ��������Ϣ�����ݡ�����ָ���
};

#endif // __STKLIB_STRATEGY_H__
