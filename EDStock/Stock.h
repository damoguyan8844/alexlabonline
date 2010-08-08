/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CKData
		class	CDRData
		class	CBaseData
		class	CReport
		class	CMinute
		class	COutline
		class	CStockInfo
		class	CStock
*/

#if	!defined( __STKLIB_STOCK_H__ )
#define	__STKLIB_STOCK_H__

#define	STKLIB_DATA_INVALID	0x7FFFFFFF

#define	STKLIB_DAYS_INONEYEAR	240
#define	STKLIB_DAYS_CACHE		60
#define	STKLIB_DAYS_CALCULATE	30
#define	STKLIB_AUTORESUMEDR_DATEBEGIN	20040519
#define	STKLIB_AUTORESUMEDR_LIMIT		11

#define	STKLIB_CODE_MAIN			"1A0001"
#define	STKLIB_CODE_MAINSZN			"399001"
#define	STKLIB_CODE_SZZS			"1A0001"	// ��ָ֤��
#define	STKLIB_CODE_SZYBL			"1A0009"	// ��֤180ָ��
#define	STKLIB_CODE_SZAG			"1A0002"	// ��֤A��
#define	STKLIB_CODE_SZBG			"1A0003"	// ��֤B��
#define	STKLIB_CODE_SZNCZ			"399001"	// ��֤��ָ
#define	STKLIB_CODE_SZNZZ			"399106"	// ��֤��ָ
#define	STKLIB_CODE_SZNCFAZ			"399002"	// ��֤�ɷ�Aָ
#define	STKLIB_CODE_SZNCFBZ			"399003"	// ��֤�ɷ�Bָ
#define	STKLIB_CODE_SZNFIRST		"000001"	// ��֤��һ֧��Ʊ
#define	STKLIB_CODE_SHAFIRST		"600000"	// ��֤��һ֧��Ʊ
#define	STKLIB_CODE_ZLDD			"000000"	// ������


// some fixed length
#define	STKLIB_MAX_CODE			8
#define	STKLIB_MAX_CODE2		16	// ���ݽϳ���Ʊ����
#define	STKLIB_MAX_NAME			16
#define	STKLIB_MAX_NAMEENU		80
#define	STKLIB_MAX_SHORTNAME	16
#define	STKLIB_MAX_DOMAIN		32
#define	STKLIB_MAX_PROVINCE		32

// some enum values

#pragma pack(1)

// ��ͷ�ļ��ж������ֵ��λ��
// 		�۸������йصĵ�λ����ɽ�����Ӫ���롢ÿ�����桢����ֵ�ȣ�Ԫ
// 		�ɽ�����λ���֣�=100�ɣ�

/////////////////////////////////////////////////////////////////////////////////////////////
// ��Ʊ��̬���ݽṹ

// K�����ݽṹ	CStock::dataK
typedef struct kdata_t {
	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// ֤ȯ����,��'\0'��β

	DWORD	m_date;				//����	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	time_t	m_time;				//ʱ�䡡��m_dateһ�£��������ĳ�գ���Сʱ�����Ϊ0

	float	m_fOpen;			// ���̼ۣ�Ԫ��
	float	m_fHigh;			// ��߼ۣ�Ԫ��
	float	m_fLow;				// ��ͼۣ�Ԫ��
	float	m_fClose;			// ���̼ۣ�Ԫ��
	float	m_fVolume;			// �ɽ������ɣ�
	float	m_fAmount;			// �ɽ���Ԫ��

	DWORD	m_dwAdvance;		// ��ָ����Ч
	DWORD	m_dwDecline;		// ��ָ����Ч

	DWORD	m_dwReserved;
} KDATA, *PKDATA;

// ��Ȩ���ݽṹ CStock::dataDR
typedef struct drdata_t
{
	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// ֤ȯ����,��'\0'��β

	DWORD	m_date;				//����	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	time_t	m_time;				//ʱ�䡡��m_dateһ�£��������ĳ�գ���Сʱ�����Ϊ0

	float	m_fGive;			// ÿ����
	float	m_fPei;				// ÿ����
	float	m_fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
	float	m_fProfit;			// ÿ�ɺ���

	DWORD	m_dwReserved;
} DRDATA, * PDRDATA;

// ������Ϣ���ݽṹ CStock::dataBasetbl
typedef struct basedata_t {
	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];		// ֤ȯ����,��'\0'��β
	
	char	m_szDomain[STKLIB_MAX_DOMAIN];		// ���
	char	m_szProvince[STKLIB_MAX_PROVINCE];	// ʡ��

	time_t	m_time;						// ���ڣ�time_t
	DWORD	m_date;				 		// ����	Format is YYYYMMDD for base data
	LONG	m_reporttype;				// �������ͣ��걨���б�������
	float	m_fErate_dollar;			// ������Ԫ����
	float	m_fErate_hkdollar;			// ���ڸ۱һ���

	// �ﳥծ����
	float	m_fRatio_liquidity;			// ��������
	float	m_fRatio_quick;				// �ٶ�����
	float	m_fVelocity_receivables;	// Ӧ���ʿ�����
	// �ﾭӪ����
	float	m_fVelocity_merchandise;	// �����ת��
	float	m_fMain_income;				// ��Ӫҵ������
	float	m_fCash_ps;					// ÿ�ɾ��ֽ�����
	// ��ӯ������
	float	m_fProfit_margin;			// ��Ӫҵ�������� %
	float	m_fNetasset_yield;			// ���ʲ������� %
	// ���ʱ��ṹ
	DWORD	m_datebegin;				// �������� Format is YYYYMMDD
	float	m_fShare_count_total;		// �ܹɱ�
	float	m_fShare_count_a;			// ��ͨA��
	float	m_fShare_count_b;			// ��ͨB��
	float	m_fShare_count_h;			// ��ͨH��
	float	m_fShare_count_national;	// ���й�
	float	m_fShare_count_corp;		// ���˹�
	float	m_fProfit_psud;				// ÿ��δ��������
	float	m_fAsset;					// ���ʲ�
	float	m_fRatio_holderright;		// �ɶ�Ȩ�����
	float	m_fRatio_longdebt;			// ���ڸ�ծ��
	float	m_fRatio_debt;				// �ʲ���ծ��
	// ��Ͷ����������
	float	m_fNetasset_ps;				// ÿ�ɾ��ʲ�
	float	m_fNetasset_ps_regulate;	// ����ÿ�ɾ��ʲ�
	float	m_fEps;						// ÿ������
	float	m_fEps_deduct;				// �۳���ÿ������
	float	m_fNet_profit;				// ������
	float	m_fMain_profit;				// ��Ӫҵ������
	float	m_fTotal_profit;			// �����ܶ�
	// ��������
	float	m_fProfit_inc;				// ��Ӫ����������%
	float	m_fIncome_inc;				// ��Ӫ����������%
	float	m_fAsset_inc;				// ���ʲ�������%
	// �＼������ͳ��ֵ
	float	m_fYield_average;			// ƽ��������%
	float	m_fYield_stddev;			// �����׼��%
	float	m_fBeite;					// ��ֵ

	DWORD	m_dwReserved[4];
}BASEDATA, *PBASEDATA;

// ��Ʊ��̬���ݽṹ����
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
// ʵʱ����ʹ�����ݽṹ

// ����ˢ�����ݽṹ CStock::dataReport
typedef struct report_t {
	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// ֤ȯ����,��'\0'��β

	char	m_szName[STKLIB_MAX_NAME];		// ֤ȯ����,��'\0'��β

	time_t	m_time;						// ����ʱ��

	DWORD	m_dwFlag;					// ͣ�Ʊ�־

	float	m_fLast;					// ���գ�Ԫ��
	float	m_fOpen;					// �񿪣�Ԫ��
	float	m_fHigh;					// ��ߣ�Ԫ��
	float	m_fLow;						// ��ͣ�Ԫ��
	float	m_fNew;						// ���£�Ԫ��
	float	m_fVolume;					// �ɽ������ɣ�
	float	m_fAmount;					// �ɽ��Ԫ��

	float	m_fBuyPrice[5];				// �����1,2,3,4��Ԫ��
	float	m_fBuyVolume[5];			// ������1,2,3,4���ɣ�
	float	m_fSellPrice[5];			// ������1,2,3,4��Ԫ��
	float	m_fSellVolume[5];			// ������1,2,3,4���ɣ�
} REPORT, *PREPORT;

// �����ʱ���ݽṹ CStock::dataMinute
typedef struct minute_t {
	DWORD	m_dwType;	// 1 min, 5 min, 15 min, 30 min line

	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// ֤ȯ����,��'\0'��β

	time_t	m_time;				// ����ʱ��
	float	m_fHigh;			// ��߼ۣ�Ԫ��
	float	m_fLow;				// ��ͼۣ�Ԫ��
	float	m_fNew;				// ���¼ۣ�Ԫ��
	float	m_fVolume;			// �ɽ������ɣ�
	float	m_fAmount;			// �ɽ��Ԫ��
} MINUTE, *PMINUTE;

// ����������ݽṹ CStock::dataOutline
typedef struct outline_t {
	time_t	m_time;

	DWORD	m_dwShHq;
	DWORD	m_dwSzHq;
	DWORD	m_dwShPriceUp;
	DWORD	m_dwShPriceDown;
	DWORD	m_dwSzPriceUp;
	DWORD	m_dwSzPriceDown;
	DWORD	m_dwShWbUp;
	DWORD	m_dwShWbDown;
	DWORD	m_dwSzWbUp;
	DWORD	m_dwSzWbDown;
	DWORD	m_dwShStockCount;
	DWORD	m_dwSzStockCount;
} OUTLINE, *POUTLINE;

// �ۺ��������ݽṹ CStock::dataMultisort
//�ۺ�����
typedef struct multisort_item_t {
	BYTE	m_type;					// stock's type
	char	m_code[6];				// stock code
	long	m_data1;
	long	m_data2;
} MULTISORT_ITEM;
typedef struct multisort_t {
	DWORD	m_class;				// 1-�Ƿ� 2-���� 4-5�����Ƿ� 8-5���ӵ��� 16-ί��ǰ�� 32-ί�Ⱥ���
									// 64-��� 128-���� 256-�ܽ��
	DWORD	m_tag;					// 0xf0 00 06 00
	MULTISORT_ITEM	m_stocks[6];
} MULTISORT, *PMULTISORT;

// ��Ʊ���� CStock::dataCode
typedef	struct stockcode {
	DWORD	m_dwMarket;						// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];		// ֤ȯ����,��'\0'��β
} STOCKCODE, *PSTOCKCODE;

// ����ͨ�����ݰ�
#define	STKLIB_COMMPACKET_TAG	'KPMC'
typedef struct commpacket_t {
	DWORD	m_dwTag;			// = STKLIB_COMMPACKET_TAG
	DWORD	m_dwDataType;		// see CStock::DataType
	DWORD	m_dwCount;

	union
	{
		REPORT *	m_pReport;		// ����ˢ������
		MINUTE *	m_pMinute;		// ��ʱ�ɽ�����
		MULTISORT *	m_pMultisort;	// �ۺ���������
		OUTLINE *	m_pOutline;		// ��������
		KDATA *		m_pKdata;		// ������ʷ��������
		DRDATA *	m_pDrdata;		// ����ȨϢ����
		STOCKCODE *	m_pStockcode;	// ��Ʊ����
		void *		m_pData;
	};
} COMMPACKET, *PCOMMPACKET;

// ʵʱ����ʹ�����ݽṹ����
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack()

/***
	��Ȩ��Ϣ������
*/
class  CDRData
{
public:
// Construction
	CDRData();
	CDRData( const CDRData &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	DRDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, DRDATA newElement);
	DRDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const DRDATA* GetData() const;
	DRDATA* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, DRDATA newElement);
	int		Add(DRDATA newElement);

	// overloaded operator helpers
	DRDATA	operator[](int nIndex) const;
	DRDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, DRDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CDRData* pNewArray);
	int		InsertDRDataSort( DRDATA newElement );
	void	Sort( );
	CDRData	& operator = ( const CDRData &src );
	void	Copy( const CDRData &src );
	BOOL	IsSameAs( CDRData * psrc );

// Implementation
protected:
	DRDATA* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CDRData();
};

/***
	K������������
*/
class  CKData
{
public:
	// Constructors
	CKData();
	CKData( int ktype, int maindatatype = mdtypeClose );
	CKData( const CKData & src );

	CKData	& operator = ( const CKData &src );

	static	BOOL	IsDayOrMin( int nKType );

	// Flag values
	enum KTypes {
		ktypeNone		=	0x00,
		ktypeMin		=	0x01,
		ktypeMin5		=	0x01,
		ktypeMin15		=	0x02,
		ktypeMin30		=	0x03,
		ktypeMin60		=	0x04,
		ktypeDay		=	0x05,
		ktypeWeek		=	0x06,
		ktypeMonth		=	0x07,
		ktypeMax		=	0x07,
	};

	enum Formats {
		formatNone		=	0x00,
		formatMin		=	0x01,
		formatOriginal	=	0x01,
		formatXDRup		=	0x02,
		formatXDRdown	=	0x03,
		formatMax		=	0x03,
	};

	enum MaindataTypes {
		mdtypeNone		=	0x00,
		mdtypeMin		=	0x01,
		mdtypeClose		=	0x01,
		mdtypeOpen		=	0x02,
		mdtypeAverage	=	0x03,
		mdtypeMax		=	0x03,
	};

	// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	KDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, KDATA newElement);
	KDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const KDATA* GetData() const;
	KDATA* GetData();
	
	float	MaindataAt(int nIndex) const;	// �õ�CKData��nIndex�յ������ݣ��������������Ͳ�ͬ������ֵ�����ǿ��̼ۡ����̼ۻ���ƽ����
	BOOL	DateAt(int nIndex,
					int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const;
	BOOL	LatestDate(
					int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const;
	DWORD	GetDate(int nIndex );
	DWORD	GetDateDay( int nIndex );
	int		GetIndexByDate( DWORD date );
	int		GetAboutIndexByDate( DWORD date );

	// Potentially growing the array
	void	SetAtGrow(int nIndex, KDATA newElement);
	int		Add(KDATA newElement);

	// overloaded operator helpers
	KDATA	operator[](int nIndex) const;
	KDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, KDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CKData* pNewArray);
	int		InsertKDataSort( KDATA newElement );

	// Operation of Analysis
	BOOL	IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays );
	BOOL	GetMinMaxInfo( int nStart, int nEnd, float * pfMin, float *pfMax );

// Implementation
public:
	virtual ~CKData();

#ifdef _DEBUG
	void AssertValid() const;
	void Dump() const;
#endif

public:
	// Extra operations
	void	SetKType( int ktype );
	void	AutoSetKType( );
	int		GetKType( );
	int		GetCurFormat( );
	void	SetDRData( CDRData &drdata );
	CDRData &GetDRData( );
	void	ChangeCurFormat( int format, DWORD dateAutoDRBegin, double dAutoDRLimit );
	void	SetMaindataType( int type );
	int		GetMaindataType( );
	void	Clear( );

	int		CompareLatestDate( CKData &kd );
	static	int Min5ToMin15( CKData &kdm5, CKData &kdm15 );
	static	int Min5ToMin30( CKData &kdm5, CKData &kdm30 );
	static	int Min5ToMin60( CKData &kdm5, CKData &kdm60 );
	static	int DayToMonth( CKData &kdday, CKData &kdmonth );
	static	int DayToWeek( CKData &kdday, CKData &kdweek );

	DWORD	ToDayDate( DWORD date );
	
	int		MergeKData( CKData * pother );
	int		FullFillKData( CKData & kdataMain, BOOL bFillToEnd );

	BOOL	IsAdjacentDays( int nIndex, int nDays );
	BOOL	GetDiff( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetDiffPercent( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetScope( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetVolumeSum( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRatioVolume( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRS( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetMA( double * pValue, int nIndex, int nDays );

protected:
	static int		ConvertKData( CKData &kdSrc, CKData &kdDest, int multiple );
	void	CopyData( const CKData &src );
	void	LoadDataOriginal( );
	void	StoreDataOriginal( );
	void	ConvertXDR( BOOL bUP, DWORD dateAutoDRBegin, double dAutoDRLimit );

	static	float	GetRatio( float fLastClose, DRDATA & dr );

protected:
	int		m_nKType;
	int		m_nCurFormat;
	int		m_nCurMaindataType;
	CDRData	m_drdata;

	struct kdata_t * m_pDataOriginal;
	int m_nSizeOriginal;     // # of elements (upperBound - 1)
	int m_nMaxSizeOriginal;  // max allocated
	struct kdata_t * m_pData;
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

/***
	�������������࣬���Լ�¼����Ļ�������
*/
class  CBaseData
{
public:
// Construction
	CBaseData();
	CBaseData( const CBaseData &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	BASEDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, BASEDATA newElement);
	BASEDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const BASEDATA* GetData() const;
	BASEDATA* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, BASEDATA newElement);
	int		Add(BASEDATA newElement);

	// overloaded operator helpers
	BASEDATA	operator[](int nIndex) const;
	BASEDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, BASEDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CBaseData* pNewArray);
	int		InsertBaseDataSort( BASEDATA newElement );
	void	Sort( );
	CBaseData	& operator = ( const CBaseData &src );
	void	Copy( const CBaseData &src );

// Implementation
protected:
	BASEDATA* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CBaseData();
};

/***
	������Ϣ����������
*/
class  CReport
{
public:
// Construction
	CReport();
	CReport( const CReport &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	REPORT	GetAt(int nIndex) const;
	void	SetAt(int nIndex, REPORT newElement);
	REPORT&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const REPORT* GetData() const;
	REPORT* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, REPORT newElement);
	int		Add(REPORT newElement);

	// overloaded operator helpers
	REPORT	operator[](int nIndex) const;
	REPORT&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, REPORT newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CReport* pNewArray);
	int		InsertReportSort( REPORT newElement );
	void	Sort( );
	void	RemoveDirty( );
	CReport	& operator = ( const CReport &src );
	void	Copy( const CReport &src );

	BOOL	GetMMLD( int nIndex, double *pdVolBuy, double *pdVolSell, double * pdVolDiff );	// ȡ����������ָ��ֵ
	BOOL	GetMMLDMinMaxInfo( double *pdMin, double *pdMax );			// ȡ����������ָ�����ֵ��Сֵ
	BOOL	StatBuySellEx( float * fSellPrice, float * fSellVolume, float * fBuyPrice, float * fBuyVolume, int nSize );

// Implementation
protected:
	REPORT* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CReport();
};

/***
	�����ʱ����������
*/
class CSPDWordArray;
class  CMinute
{
public:
// Construction
	CMinute();
	CMinute( const CMinute &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	MINUTE	GetAt(int nIndex) const;
	void	SetAt(int nIndex, MINUTE newElement);
	MINUTE&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const MINUTE* GetData() const;
	MINUTE* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, MINUTE newElement);
	int		Add(MINUTE newElement);

	// overloaded operator helpers
	MINUTE	operator[](int nIndex) const;
	MINUTE&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, MINUTE newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CMinute* pNewArray);
	int		InsertMinuteSort( MINUTE newElement );
	void	Sort( );
	void	RemoveDirty( );
	CMinute	& operator = ( const CMinute &src );
	void	Copy( const CMinute &src );
	
	BOOL	StatVolumeInfo( double *pdVolNow, double *pdVolOuter, double *pdVolInner );	// ͳ����������
	BOOL	StatDealInfo( CSPDWordArray & adwPrice, CSPDWordArray & adwVolume, double * pdMaxVolume ); // ͳ�Ƽ�����ϵ������ֵ�еļ۸������0.001
	BOOL	GetDiffPercentMin5( double * pValue );
	BOOL	GetLBDKMinMaxInfo( double dVolAverage, double *pdMin, double *pdMax );	// ȡ�����ȶ��ָ�����ֵ��Сֵ

	BOOL	GetIndexWave( double *pdWave, int nIndex );

	int		ToKData( CKData & kdata );

// Implementation
protected:
	MINUTE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CMinute();
};

/***
	���������Ϣ����������
*/
class  COutline
{
public:
// Construction
	COutline();
	COutline( const COutline &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	OUTLINE	GetAt(int nIndex) const;
	void	SetAt(int nIndex, OUTLINE newElement);
	OUTLINE&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const OUTLINE* GetData() const;
	OUTLINE* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, OUTLINE newElement);
	int		Add(OUTLINE newElement);

	// overloaded operator helpers
	OUTLINE		operator[](int nIndex) const;
	OUTLINE&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, OUTLINE newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, COutline* pNewArray);
	int		InsertOutlineSort( OUTLINE newElement );
	void	Sort( );
	COutline	& operator = ( const COutline &src );
	void	Copy( const COutline &src );

// Implementation
protected:
	OUTLINE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~COutline();
};

class CStDatabase;

/***
	������Ʊ��Ϣ�� CStock::dataInfo
*/
class 	CStockInfo
{
public:
	CStockInfo( );
	CStockInfo( const CStockInfo &src );

	// Operation
	void Clear( );
	CStockInfo & operator = ( const CStockInfo & si );

	BOOL	IsEqualTo( DWORD dwMarket, const char * szCode );
	DWORD	GetMarket( );
	void	SetMarket( DWORD dwMarket );
	const char * GetStockCode( );
	BOOL	SetStockCode( DWORD dwMarket, const char * szCode );
	LONG	GetType( );
	void	SetType( LONG t );
	void	ResolveTypeAndMarket( );
	const char * GetStockName( );
	void	SetStockName( const char * szName );
	const char * GetStockNameEnu( );
	void	SetStockNameEnu( const char * szNameEnu );
	const char * GetStockNameChs( );
	void	SetStockNameChs( const char * szName );
	const char * GetStockShortName( );
	void	SetStockShortName( const char *szShortName );
	const char * GetStockDomain( );
	void	SetStockDomain( const char * szDomain );
	const char * GetStockProvince( );
	void	SetStockProvince( const char * szProvince );

	BOOL	StatTechIndex( DWORD dwDate );	// �趨��ǰ�����������ڣ�Format is YYYYMMDD
	BOOL	StatBaseIndex( DWORD dwDate );	// �趨��ǰ�����������ڣ�Format is YYYYMMDD
	
	// Attributes
	BOOL	IsValidStock( );	// ��Ʊ��Ϣ�Ƿ���ȷ
	BOOL	IsShangHai( );		// ��֤��Ʊ
	BOOL	IsShenZhen( );		// ��֤��Ʊ
	BOOL	IsIndex( );			// �Ƿ�ָ��
	BOOL	IsBond( );			// �Ƿ�ծȯ
	BOOL	IsBondIndex( );		// �Ƿ��ծָ��
	BOOL	IsFund( );			// �Ƿ����
	BOOL	IsRight( );			// �Ƿ�Ȩ֤
	int		DigitBit( );		// �õ��ùɼ۸��м�λС��λ��������Ϻ�B������λ

	// Variant
	BOOL	GetAverage( double *pValue );	// ���ճɽ�����
	BOOL	GetDiff( double *pValue, DWORD dateCur, int nDays );	// �ǵ�
	BOOL	GetDiffPercent( double *pValue, DWORD dateCur, int nDays );	// �ǵ�����
	BOOL	GetScope( double *pValue, DWORD dateCur, int nDays );	// �ǵ���Χ
	BOOL	GetDiffPercentMin5( double *pValue );

	BOOL	GetPE( double * pValue );		// ��ӯ��
	BOOL	GetPNetAsset( double *pValue );	// �о���
	BOOL	GetPMainIncome( double * pValue );	// ������
	BOOL	GetRatioPCash( double * pValue );	// �۸��ֽ�����

	BOOL	GetShareCurrency( double * pValue );	// ��ͨ����
	BOOL	GetRatioCurrency( double * pValue );	// ��ͨ����
	BOOL	GetRatioChangeHand( double * pValue, double dVolume );
	BOOL	GetRatioChangeHand( CKData &kdata, double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRatioChangeHand( double * pValue, DWORD dateCur, int nDays );	// n�ջ�����
	BOOL	GetRatioVolume( double * pValue, DWORD dateCur, int nDays );		// n������
	BOOL	GetRS( double * pValue, DWORD dateCur, int nDays );					// n�����ǿ��
	BOOL	GetSellBuyRatio( double *pdRatio, double *pdDiff );					// ί�Ⱥ�ί��

	BOOL	GetMarketValue( double * pValue );	// ��ֵ
	BOOL	GetMarketValueA( double * pValue );	// A����ֵ
	BOOL	GetMarketValueB( double * pValue );	// B����ֵ

	BOOL	GetXiapu( double * pValue );		// ���շ���ָ��

protected:
	// data from codetable
	DWORD	m_dwMarket;							// ֤ȯ�г�����,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];			// ֤ȯ����,��'\0'��β

	LONG	m_type;								// see CStock::StockType
	char	m_szName[STKLIB_MAX_NAME];			// ����
	char	m_szNameEnu[STKLIB_MAX_NAMEENU];		// Ӣ������
	char	m_szShortname[STKLIB_MAX_SHORTNAME];	// ƴ�����

	char	m_szDomain[STKLIB_MAX_DOMAIN];		// ���
	char	m_szProvince[STKLIB_MAX_PROVINCE];	// ʡ��

public:

	DWORD	m_datetech;	//����	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day

	float	m_fLast;			// ���գ�Ԫ��
	float	m_fOpen;			// ���̣�Ԫ��
	float	m_fHigh;			// ��߼ۣ�Ԫ��
	float	m_fLow;				// ��ͼۣ�Ԫ��
	float	m_fClose;			// ���̣�Ԫ��

	float	m_fVolume;			// �ɽ���(��)
	float	m_fAmount;			// �ɽ���(Ԫ)

	DWORD	m_dwAdvance;		// ��ָ����Ч
	DWORD	m_dwDecline;		// ��ָ����Ч

	float	m_fBuyPrice[5];
	float	m_fBuyVolume[5];
	float	m_fSellPrice[5];
	float	m_fSellVolume[5];

public:
	CKData		m_kdata;		// �������AfxGetProfile().GetCacheDays()��K������
	CBaseData	m_basedata;		// ���������������
	CMinute		m_minute;		// ���5���ӵ�MINUTE��ʱ����
	REPORT		m_reportLatest;	// ���������REPORT��������
	
public:
	// From Base Data
	DWORD	m_datebase;					//����	Format is YYYYMMDD for base data
	LONG	m_reporttype;
	float	m_fErate_dollar;			// ������Ԫ����
	float	m_fErate_hkdollar;			// ���ڸ۱һ���

	// �ﳥծ����
	float	m_fRatio_liquidity;			// ��������
	float	m_fRatio_quick;				// �ٶ�����
	float	m_fVelocity_receivables;	// Ӧ���ʿ�����
	// �ﾭӪ����
	float	m_fVelocity_merchandise;	// �����ת��
	float	m_fMain_income;				// ��Ӫҵ������
	float	m_fCash_ps;					// ÿ�ɾ��ֽ�����
	// ��ӯ������
	float	m_fProfit_margin;			// ��Ӫҵ�������� %
	float	m_fNetasset_yield;			// ���ʲ������� %
	// ���ʱ��ṹ
	DWORD	m_datebegin;				// �������� Format is YYYYMMDD
	float	m_fShare_count_total;		// �ܹɱ�
	float	m_fShare_count_a;			// ��ͨA��
	float	m_fShare_count_b;			// ��ͨB��
	float	m_fShare_count_h;			// ��ͨH��
	float	m_fShare_count_national;	// ���й�
	float	m_fShare_count_corp;		// ���˹�
	float	m_fProfit_psud;				// ÿ��δ��������
	float	m_fAsset;					// ���ʲ�
	float	m_fRatio_holderright;		// �ɶ�Ȩ����� %
	float	m_fRatio_longdebt;			// ���ڸ�ծ�� % 
	float	m_fRatio_debt;				// �ʲ���ծ�� %
	// ��Ͷ����������
	float	m_fNetasset_ps;				// ÿ�ɾ��ʲ�
	float	m_fNetasset_ps_regulate;	// ����ÿ�ɾ��ʲ�
	float	m_fEps;						// ÿ������
	float	m_fEps_deduct;				// �۳���ÿ������
	float	m_fNet_profit;				// ������
	float	m_fMain_profit;				// ��Ӫҵ������
	float	m_fTotal_profit;			// �����ܶ�
	// ��������
	float	m_fProfit_inc;				// ��Ӫ����������%
	float	m_fIncome_inc;				// ��Ӫ����������%
	float	m_fAsset_inc;				// ���ʲ�������%
	// �＼������ͳ��ֵ
	float	m_fYield_average;			// ƽ��������%
	float	m_fYield_stddev;			// �����׼��%
	float	m_fBeite;					// ��ֵ
};

/***
	������Ʊ�����ݣ�������Ʊ��Ϣ��K�����ݣ��������ϣ���Ȩ��Ϣ���ݵȡ�
*/
class CStock
{
public:
	// Constructors
	CStock( );
	CStock( CStDatabase * pDatabase );
	CStock( CStDatabase * pDatabase, DWORD dwMarket, const char * code );
	virtual	~CStock();

	// Flag values
	enum StockMarket {
		marketUnknown	=	'UKNW',	// δ֪
		marketSHSE		=	'ESHS',	// �Ϻ�֤ȯ������
		marketSZSE		=	'ESZS',	// ����֤ȯ������
		marketCYSE		=	'ESYC',	// �й���½��ҵ���г�
		marketCHNA		=	'ANHC',	// �й���½�Ϻ������ڻ�ҵ��֤ȯ������
		marketHKEX		=	'XEKH',	// ������Ͻ�����
		marketTBSE		=	'ESBT',	// ̨��֤ȯ������
		marketTKSE		=	'EST',	// ����֤ȯ������
		marketLSE		=	'ESL',	// �׶�֤ȯ������
		marketFLKFSE	=	'FKLF',	// �����˸�֤ȯ������
		marketNYSE		=	'ESYN',	// ŦԼ֤ȯ������
		marketNASDAQ	=	'QDSN',	// ��˹���֤ȯ������
	};

	enum DataType {
		dataUnknown		=	0x00,		// δ֪����
		dataInfo		=	0x01,		// ��Ʊ��Ϣ
		dataK			=	0x02,		// K������
		dataDR			=	0x03,		// ȨϢ����
		dataBasetable	=	0x04,		// ��������
		dataBasetext	=	0x05,		// F10����
		dataNews		=	0x06,		// ������Ѷ
		dataReport		=	0x07,		// ����ˢ������
		dataMinute		=	0x08,		// �����ʱ����
		dataOutline		=	0x09,		// �����������
		dataCode		=	0x0A,		// ��Ʊ��������
		dataMultisort	=	0x0B,		// �ۺ�����
		dataDetail		=	0x0C,		// �ɽ���ϸ
	};

	enum StockType {
		typeNone		=	0x00,
		typeshIndex		=	0x10,	//	�Ϻ�ָ��
		typeshA			=	0x11,	//	�Ϻ�A��
		typeshB			=	0x12,	//	�Ϻ�B��
		typeshBond		=	0x13,	//	�Ϻ�ծȯ
		typeshRight		=	0x14,	//	�Ϻ�Ȩ֤
		typeshBonus		=	0x15,	//	�Ϻ�����
		typeshAdmeasure	=	0x16,	//	�Ϻ����
		typeshInfo		=	0x1F,	//	�Ϻ�֤������Ϣ
		typeszIndex		=	0x20,	//	����ָ�� 0x20
		typeszA			=	0x21,	//	����A�� 0x21
		typeszB			=	0x22,	//	����B�� 0x22
		typeszBond		=	0x23,	//	����ծȯ 0x23
		typeszRight		=	0x24,	//	����Ȩ֤ 0x24
		typeszBonus		=	0x25,	//	���ں��� 0x25
		typeszAdmeasure	=	0x26,	//	������� 0x26
		typeszMsmall	=	0x27,	//	������С��ҵ
		typeszInfo		=	0x2F,	//	����֤������Ϣ 0x2f
		typeGeneralInfo	=	0x05,	//	�ۺϲƾ���Ϣ 0x05
	};

	enum StockTypeMark {
		markShanghai	=	0x10,	// �Ϻ�
		markShenzhen	=	0x20,	// ����
		markGeneralInfo	=	0x05,	// �ۺϲƾ���Ϣ
	};

	enum ReportType	{
		reportUnknown	=	0x00,	//	δ֪
		reportQuarter	=	0x01,	//	��һ���ȼ���
		reportMid		=	0x02,	//	�б�
		reportQuarter3	=	0x03,	//	�������ȼ���
		reportAnnals	=	0x04,	//	�걨
	};

	enum MultisortType {
		classRise		=	0x01,	// �Ƿ�
		classFall		=	0x02,	// ����
		claddRiseMin5	=	0x04,	// 5�����Ƿ�
		classFallMin5	=	0x08,	// 5���ӵ���
		classBSRatioAsc	=	0x10,	// ί��ǰ��
		classBSRatioDesc=	0x20,	// ί�Ⱥ���
		classDiff		=	0x40,	// ���
		classVolRatio	=	0x80,	// ����
		classAmount		=	0x100,	// �ܽ��
	};

	static	double GetReportFactor( int nReportType );

	// Attributes
	void	SetDatabase( CStDatabase * pDatabase );
	CStDatabase * GetDatabase( );
	void	SetStockCode( DWORD dwMarket, const char * szCode );
	const char * GetStockCode( );
	BOOL	SetStockInfo( CStockInfo * pInfo );
	CStockInfo	&	GetStockInfo( );
	CKData	&		GetKData( int ktype );
	CKData	&		GetKDataMonth( );
	CKData	&		GetKDataWeek( );
	CKData	&		GetKDataDay( );
	CKData	&		GetKDataMin60( );
	CKData	&		GetKDataMin30( );
	CKData	&		GetKDataMin15( );
	CKData	&		GetKDataMin5( );
	CDRData	&		GetDRData( );
	CReport &		GetReport( );
	CMinute &		GetMinute( );
	COutline &		GetOutline( );
	char	*		GetBaseTextPtr( );
	int				GetBaseTextLength( );
	BOOL			AllocBaseTextMem( int nNewLength );

	// Operations
	void	Clear( );
	int		PrepareData( int nDataType = dataInfo, int nKType = CKData::ktypeDay, BOOL bReload = FALSE );
	int		PrepareBaseInfo( BOOL bReload = FALSE );
	int		ExtractKData( int nKType, BOOL bForced );
	int		MergeBaseText( CStock &stock );
	int		MergeKData( CStock &stock, int nKType );
	int		MergeDRData( CStock &stock );
	int		StoreDRData( );

protected:
	void	SetKDataTypes( );

protected:
	CStDatabase	* m_pDatabase;
	CStockInfo	m_stockinfo;
	
	CKData		m_kdMonth;
	CKData		m_kdWeek;
	CKData		m_kdDay;
	CKData		m_kdMin60;
	CKData		m_kdMin30;
	CKData		m_kdMin15;
	CKData		m_kdMin5;
	CDRData		m_drData;
	CReport		m_report;
	CMinute		m_minute;
	COutline	m_outline;
	int			m_nBaseTextLen;
	char *		m_pBaseText;
};


#ifdef	_SP_ENABLE_INLINES
#define	_STOCK_INLINE	inline
#include "Stock.inl"
#undef	_STOCK_INLINE
#endif

#endif	//__STKLIB_STOCK_H__
