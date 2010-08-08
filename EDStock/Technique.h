/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechnique
*/

#if	!defined( __STKLIB_TECHNIQUE_H__ )
#define	__STKLIB_TECHNIQUE_H__

#include "SpColl.h"

/* The Folowing Is Stock Technique Analysis Identify Number */
/* ����ָ��Ψһ��� */
#define	STT_MIN			1
//	K��������
#define	STT_KLINE_MIN	1
#define	STT_MA			1		//	�ƶ�ƽ����MA
#define	STT_BBI			2		//	���ָ��BBI
#define	STT_BOLL		3		//	���ִ�BOLL
#define	STT_PV			4		//	��֤������P/V
#define	STT_SAR			5		//	ͣ���ת��ָ��SAR
#define	STT_DJ			6		//	����ͼDJ
#define	STT_CW			7		//	����ֲ�ͼCW
#define	STT_KLINE_MAX	7

//	������
#define	STT_TREND_MIN	8
#define	STT_MACD		8		//	ָ��ƽ����ͬ�ƶ�ƽ����MACD
#define	STT_MIKE		9		//	���ָ��MIKE
#define	STT_PSY			10		//	������PSY
#define	STT_TREND_MAX	10

//	������
#define	STT_ENERGY_MIN	11
#define	STT_VOLUME		11		//	�ɽ���
#define	STT_NVI			12		//	���ɽ���ָ��NVI
#define	STT_PVI			13		//	���ɽ���ָ��PVI
#define	STT_VR			14		//	��������VR
#define	STT_VROC		15		//	�ɽ����䶯��ָ��VROC
#define	STT_OBV			16		//	������OBV
#define	STT_MOBV		17		//	��������MOBV
#define	STT_MFI			18		//	�ʽ�����ָ��MFI
#define	STT_VMACD		19		//	��ָ��ƽ����ͬ�ƶ�ƽ����VMACD
#define	STT_WVAD		20		//	����������ɢ��WVAD
#define	STT_EMV			21		//	���ײ���ָ��EMV
#define	STT_VRSI		22		//	�����ǿ��ָ��VRSI
#define	STT_NVRSI		23		//	�������ǿ��ָ��VRSI
#define	STT_AD			24		//	��/ɢָ��A/D
#define	STT_CI			25		//	�ۻ�ָ��CI
#define	STT_ENERGY_MAX	25

//	�ڶ���
#define	STT_SWING_MIN	26
#define	STT_KDJ			26		//	���ָ��KDJ
#define	STT_R			27		//	����ָ��R
#define	STT_RSI			28		//	���ǿ��ָ��
#define	STT_BIAS		29		//	������BIAS
#define	STT_MTM			30		//	����ָ��MTM
#define	STT_DMI			31		//	����ָ��DMI
#define	STT_ROC			32		//	�䶯��ָ��ROC
#define	STT_CCI			33		//	˳��ͨ��ָ��CCI
#define	STT_CV			34		//	����ָ��CV
#define	STT_ARBR		35		//	������Ըָ��ARBR
#define	STT_CR			36		//	�м���Ըָ��CR
#define	STT_OSC			37		//	�ڶ���OSC
#define	STT_UOS			38		//	���ڶ�ָ��UOS
#define	STT_MAOSC		39		//	�ƶ�ƽ���ڶ���MAOSC
#define	STT_36BIAS		40		//	3��6�չ���3-6BIAS
#define	STT_DPO			41		//	ȥ�Ƽ۸�ڶ�ָ��DPO
#define	STT_KST			42		//	ȷȻָ��KST
#define	STT_SWING_MAX	42

//	������
#define	STT_OTHER_MIN	43
#define	STT_REI			43		//	��Χ����ָ��REI
#define	STT_DMKI		44		//	����˶�ָ��DMKI
#define	STT_PCNT		45		//	���ȱ�PCNT
#define	STT_HLC			46		//	���ȷ���HLC
#define	STT_CDP			47		//	���Ʋ���ָ��CDP
#define	STT_ASI			48		//	�ۻ��ڶ�ָ��ASI
#define	STT_ATR			49		//	����ָ��ATR
#define	STT_OTHER_MAX	49

#define	STT_CLK_MIN		50
#define	STT_CYO			50		//	���ڰڶ���CYO
#define	STT_DCYO		51		//	ȥ�����ڰڶ���DCYO
#define	STT_HSL			52		//	��Ի�����HSL
#define	STT_DPER		53		//	ȥ�ưٷֱ�ָ��DPER
#define	STT_CLK_MAX		53

#define	STT_MAX			53

#define	STT_USER_MIN	10000

#define	STT_INVALID		DWORD(-1)

// ָ�����
#define	STT_CLASS_MIN		1
#define	STT_CLASS_KLINE		1	// K�ߵ�����
#define	STT_CLASS_TREND		2	// ������
#define	STT_CLASS_ENERGY	3	// ������
#define	STT_CLASS_SWING		4	// �ڶ���
#define	STT_CLASS_OTHER		5	// ������
#define	STT_CLASS_CLK		6	// ����Ϊ����
#define	STT_CLASS_USER		7	// ���ָ����
#define	STT_CLASS_MAX		7

// �����ź�
#define	ITS_MIN				-15
#define	ITS_SELLINTENSE		-15	//	ǿ������
#define	ITSG_SELLINTENSE	-13	//	����
#define	ITS_SELL			-10	//	����
#define	ITSG_SELL			-8	//	����
#define	ITS_SELLFEEBLE		-5	//	΢������
#define	ITSG_SELLFEEBLE		-3	//	����
#define	ITS_NOTHING			0	//	��
#define	ITSG_BUYFEEBLE		3	//	����
#define	ITS_BUYFEEBLE		5	//	΢�����
#define	ITSG_BUY			8	//	����
#define	ITS_BUY				10	//	���
#define	ITSG_BUYINTENSE		13	//	����
#define	ITS_BUYINTENSE		15	//	ǿ�����
#define	ITS_MAX				15

// �����ź�����
#define	ITSC_MIN				1
#define	ITSC_NOTHING			1	//	��
#define	ITSC_DEVIATEONBOTTOM	2	//	�ױ���
#define	ITSC_DEVIATEONTOP		3	//	������
#define	ITSC_GOLDENFORK			4	//	���
#define	ITSC_DEADFORK			5	//	����
#define	ITSC_LONG				6	//	��ͷ
#define	ITSC_SHORT				7	//	��ͷ
#define	ITSC_OVERBOUGHT			8	//	����
#define	ITSC_OVERSOLD			9	//	����
#define	ITSC_SUPPORT			10	//	֧��λ
#define	ITSC_RESISTANCE			11	//	����λ
#define	ITSC_MAX				11

#define	ITS_ISSELL(x)			(x<0)
#define	ITS_ISBUY(x)			(x>0)
#define	ITS_ISSELLORNOTHING(x)	(x<=0)
#define	ITS_ISBUYORNOTHING(x)	(x>=0)

#define	ITS_GETMINMAXDAYRANGE	120
#define	ITS_DAYS_DEVIATE		115


/***
	һЩ�궨�壬��Ҫ�Ǽ���ָ������г��õ�һЩ����
*/
#define	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd )	\
	SP_ASSERT( m_pKData && nStart >= 0 && nStart < m_pKData->GetSize() && nEnd >= 0 && nEnd < m_pKData->GetSize() );	\
	if( nStart < 0 || nStart >=m_pKData->GetSize() || nEnd < 0 || nEnd >=m_pKData->GetSize() )							\
		return FALSE;

#define	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays )		\
	SP_ASSERT( m_pKData && nIndex >= 0 && nIndex < m_pKData->GetSize() && nDays > 0 );	\
	if( !m_pKData || nIndex < 0 || nIndex >=m_pKData->GetSize() || nDays <= 0 )			\
		return FALSE;																	\
	if( !IsValidParameters( ) )															\
		SetDefaultParameters( );

#define	STT_ASSERT_CALCULATE1( m_pKData, nIndex )			\
	SP_ASSERT( m_pKData && nIndex >= 0 && nIndex < m_pKData->GetSize() );	\
	if( !m_pKData || nIndex < 0 || nIndex >=m_pKData->GetSize() )			\
		return FALSE;														\
	if( !IsValidParameters( ) )												\
		SetDefaultParameters( );

#define	VALID_DAYS( x )		(x>=1&&x<=1024)
#define	STT_VALID_DAYSARRAY( a )						\
	if( a.GetSize() <= 0 )								\
		return FALSE;									\
	for( int k=0; k<a.GetSize(); k++ )					\
		if( a[k] < 1 || a[k] > 1024 )					\
			return FALSE;
#define	VALID_ITS( x )		(x>=ITS_MIN&&x<=ITS_MAX)


/***
	һЩ����ʹ�õĴ��룬�ú������棬�õ�����ָ����ָ����Χ�ڵ������Сֵ
*/
class	CTechnique;
BOOL AfxGetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech, CSPDWordArray & adwDays );	// ����ָ���в�ͬ������Ϊ����
BOOL AfxGetMinMaxInfo1( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// ����ָ����1������ֵ
BOOL AfxGetMinMaxInfo2( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// ����ָ����2������ֵ
BOOL AfxGetMinMaxInfo3( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// ����ָ����3������ֵ
BOOL AfxGetMinMaxInfo4( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// ����ָ����4������ֵ
BOOL AfxGetMinMaxInfo5( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// ����ָ����5������ֵ
DWORD	AfxGetMaxDays( CSPDWordArray & adw );	// ����adw�����ֵ

/**
	����ָ�����
*/
class  CTechnique
{
public:
	// Constructors
	CTechnique( );
	CTechnique( CKData * pKData );
	virtual ~CTechnique();

	void SetKData( CKData * pKData );	//	�趨K������
	CKData * GetKData( );
	virtual	void	Clear( );			//	���
	virtual	void	ClearLastIntensity( );	//	����ϴ������ź�
	virtual	BOOL	PrepareCache( int nStart, int nEnd, BOOL bForceRecalculate );	//	��������ݣ����뻺����

protected:
	BOOL	LoadFromCache( int nIndex, double * pValue1,
				double * pValue2 = NULL, double * pValue3 = NULL, double * pValue4 = NULL, double * pValue5 = NULL );	// �ӻ�����������
	BOOL	StoreToCache( int nIndex, double * pValue1,
				double * pValue2 = NULL, double * pValue3 = NULL, double * pValue4 = NULL, double * pValue5 = NULL );	// �����������������

	// Intensity Utility
	BOOL	IntensityPrepare( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow = 0.3, double dGateHigh = 0.7 );		// ׼�����ݣ��õ��ϴ�ֵ�ͽ�Сֵ
	BOOL	IntensityPreparePrice( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow = 0.3, double dGateHigh = 0.7 );	// ׼�����ݣ��õ��۸�ϴ�ֵ�ͽ�Сֵ
	BOOL	IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays, double * pdValue1, double * pdValue2 );	// �Ƿ��¸߻��ߴ��µ�
	BOOL	IsDeviateOnBottom( int nIndex, double * pdValue1, double * pdValue2 );	// �Ƿ�ױ���
	BOOL	IsDeviateOnTop( int nIndex, double * pdValue1, double * pdValue2 );		// �Ƿ񶥱���
	BOOL	IsGoldenFork( int nIndex, double * pdValue1, double * pdValue2 );		// �Ƿ���
	BOOL	IsDeadFork( int nIndex, double * pdValue1, double * pdValue2 );			// �Ƿ�����
	int		GetForkSignal( int nIndex, CSPDWordArray & adwDays, UINT itsGoldenFork, UINT itsDeadFork,  UINT * pnCode );	// �õ������������ź�
	int		GetForkSignal( int nIndex, UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode );		// �õ������������ź�
	int		GetForkSignal(int nIndex, double * pdValue1, double * pdValue2, UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode );	// �õ���������ź�
	int		GetTrendIntensity(int nIndex, CSPDWordArray & adwDays, UINT itsLong, UINT itsShort, UINT * pnCode );	// �õ������ź�
	int		GetTrendIntensity1( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode );	// �õ������ź�
	int		GetTrendIntensity2( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode );	// �õ������ź�
	int		GetTrendIntensity(int nIndex, double * pdValue1, double * pdValue2, UINT itsLong, UINT itsShort, UINT *pnCode );	// �õ������ź�

public:
	// Attributes
	virtual	void	SetDefaultParameters( );		// �趨ȱʡ����
	void	AttachParameters( CTechnique & src );	// ָ��������src��ͬ
	virtual	BOOL	IsValidParameters( );			// �����Ƿ�Ϸ�

	// Creations
	static	CTechnique *	CreateTechnique( UINT nTech, CKData * pKData );	// ����CTechnique����
	
	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );	// �õ������ź�
	virtual	int		GetIntensity( int nIndex, UINT * pnCode = NULL, BOOL bTraceBack = TRUE );	// �õ����������ź�
	virtual	int		GetIntensityTraceback( int nIndex, UINT * pnCode = NULL );					// �õ����������ź�
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );		// �õ������Сֵ
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );			// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, double * pValue5, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	CalculateMA( double * pValue, double * pMA, int nIndex, BOOL bUseLast, int nMADays );		// ����ָ��ֵ�;�ֵ

protected:
	CKData	*	m_pKData;

	double	*	m_pdCache1;
	double	*	m_pdCache2;
	double	*	m_pdCache3;
	double	*	m_pdCache4;
	double	*	m_pdCache5;
	BOOL	*	m_pbHasCache1;
	BOOL	*	m_pbHasCache2;
	BOOL	*	m_pbHasCache3;
	BOOL	*	m_pbHasCache4;
	BOOL	*	m_pbHasCache5;
	int			m_nLength;
	int			m_nIndexStart;

	int			m_nLastIntensity;
	UINT		m_nLastIntensityCode;
};

// K��
class  CKLine : public CTechnique
{
public:
	// Constructors
	CKLine( );
	CKLine( CKData * pKData );
	virtual ~CKLine();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKLine & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
};

/////////////////////////////////////////////////////////////////////////
//	K��������
//	�ƶ�ƽ����MA
class  CMA : public CTechnique
{
public:
	// Constructors
	CMA( );
	CMA( CKData * pKData );
	virtual ~CMA();

public:
	virtual	void Clear( );

	// Attributes
	enum MATypes {
		typeMA		=	0x01,
		typeEXPMA	=	0x02,
	};
	int				m_nType;
	CSPDWordArray	m_adwMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMA & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	���ָ��BBI
class  CBBI : public CTechnique
{
public:
	// Constructors
	CBBI( );
	CBBI( CKData * pKData );
	virtual ~CBBI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMA1Days;
	int		m_nMA2Days;
	int		m_nMA3Days;
	int		m_nMA4Days;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBBI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	���ִ�BOLL
class  CBOLL : public CTechnique
{
public:
	// Constructors
	CBOLL( );
	CBOLL( CKData * pKData );
	virtual ~CBOLL();

public:
	virtual	void Clear( );

	// Attributes
	double	m_dMultiUp;
	double	m_dMultiDown;
	int		m_nMADays;
	int		m_itsSupport;
	int		m_itsResistance;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBOLL & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pdMA, double * pdUp, double * pdDown, int nIndex, BOOL bUseLast );
};

//	��֤������P/V
class  CPV : public CTechnique
{
public:
	// Constructors
	CPV( );
	CPV( CKData * pKData );
	virtual ~CPV();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	ͣ���ת��ָ��SAR
class  CSAR : public CTechnique
{
public:
	// Constructors
	CSAR( );
	CSAR( CKData * pKData );
	virtual ~CSAR();

protected:
	BOOL	CalculateSAR( double * pValue, int nIndex, BOOL bUseLast );

public:
	virtual	void Clear( );

	// Attributes
	int		m_nInitDays;
	int		m_bFirstUp;
	double	m_dAFStep;
	double	m_dAFMax;
	int		m_itsBuy;
	int		m_itsSell;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CSAR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );

protected:
	BOOL			m_bCurUp;
	BOOL			m_bTurn;
	double			m_dCurAF;
	double			m_dCurHigh;
	double			m_dCurLow;
};

//	����ͼDJ
class  CDJ : public CTechnique
{
public:
	// Constructors
	CDJ( );
	CDJ( CKData * pKData );
	virtual ~CDJ();

	static	CStock		m_stockSha;
	static	CStock		m_stockSzn;
	static	CSPString	m_strCodeOrg;

public:
	virtual	void Clear( );

	// Attributes
	CSPString		m_strCodeSha;
	CSPString		m_strCodeSzn;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDJ & src );
	virtual	BOOL	IsValidParameters( );
	BOOL	PrepareStockData(CStDatabase * pDatabase, const char * szCodeOrg,
									int nCurKType, int nCurKFormat, int nCurMaindataType,
									DWORD dwAutoResumeDRBegin, int nAutoResumeDRLimit );

	// Operations
};

//	����ֲ�ͼCW
class  CCW : public CTechnique
{
public:
	// Constructors
	CCW( );
	CCW( CKData * pKData );
	virtual ~CCW();

public:
	virtual	void Clear( );

	// Attributes
	double	m_dChangeHand;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCW & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	BOOL			GetRange( int & nStart, int & nEnd, CStockInfo & info );
	BOOL			GetMinMaxInfo(int nStart, int nEnd, double dMinPrice, double dMaxPrice, double dStep,
								double *pdMinVolume, double *pdMaxVolume );
	BOOL			CalculateCW( double *pdVolume, int nStart, int nEnd, double dPrice, double dStep );


	BOOL	CalculateCW(int nStart, int nEnd, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );
	BOOL	CalculateRecentCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );
	BOOL	CalculatePastCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );

	// Stat
	static	BOOL	StatGainPercent( double *pdGainPercent, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dPriceSel );
	static	BOOL	StatCostAverage( double *pdCostAve, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume );
	static	BOOL	StatMass(double *pdLower, double *pdUpper, double *pdMassPrice, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dMassVol );
};

/////////////////////////////////////////////////////////////////////////
//	������
//	ָ��ƽ����ͬ�ƶ�ƽ����MACD
class  CMACD : public CTechnique
{
public:
	// Constructors
	CMACD( );
	CMACD( CKData * pKData );
	virtual ~CMACD();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nEMA1Days;
	int		m_nEMA2Days;
	int		m_nDIFDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMACD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast );
};

//	���ָ��MIKE
class  CMIKE : public CTechnique
{
public:
	// Constructors
	CMIKE( );
	CMIKE( CKData * pKData );
	virtual ~CMIKE();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMIKE & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	BOOL	CalculateMIKE(	double *pWR, double *pMR, double *pSR,
							double *pWS, double *pMS, double *pSS, int nIndex );
};

//	������PSY
class  CPSY : public CTechnique
{
public:
	// Constructors
	CPSY( );
	CPSY( CKData * pKData );
	virtual ~CPSY();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPSY & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	������
//	�ɽ���
class  CVOLUME : public CTechnique
{
public:
	// Constructors
	CVOLUME( );
	CVOLUME( CKData * pKData );
	virtual ~CVOLUME();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMADays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVOLUME & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	���ɽ���ָ��NVI
class  CNVI : public CTechnique
{
public:
	// Constructors
	CNVI( );
	CNVI( CKData * pKData );
	virtual ~CNVI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CNVI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	���ɽ���ָ��PVI
class  CPVI : public CTechnique
{
public:
	// Constructors
	CPVI( );
	CPVI( CKData * pKData );
	virtual ~CPVI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPVI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	��������VR
class  CVR : public CTechnique
{
public:
	// Constructors
	CVR( );
	CVR( CKData * pKData );
	virtual ~CVR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	�ɽ����䶯��ָ��VROC
class  CVROC : public CTechnique
{
public:
	// Constructors
	CVROC( );
	CVROC( CKData * pKData );
	virtual ~CVROC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVROC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	������OBV
class  COBV : public CTechnique
{
public:
	// Constructors
	COBV( );
	COBV( CKData * pKData );
	virtual ~COBV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( COBV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pdOBV, int nIndex, BOOL bUseLast );
};

//	��������MOBV
class  CMOBV : public COBV
{
public:
	// Constructors
	CMOBV( );
	CMOBV( CKData * pKData );
	virtual ~CMOBV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMOBV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );
};

//	�ʽ�����ָ��MFI
class  CMFI : public CTechnique
{
public:
	// Constructors
	CMFI( );
	CMFI( CKData * pKData );
	virtual ~CMFI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMFI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	��ָ��ƽ����ͬ�ƶ�ƽ����VMACD
class  CVMACD : public CMACD
{
public:
	// Constructors
	CVMACD( );
	CVMACD( CKData * pKData );
	virtual ~CVMACD();

protected:

public:
	virtual	BOOL	Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast );
};

//	����������ɢ��WVAD
class  CWVAD : public CTechnique
{
public:
	// Constructors
	CWVAD( );
	CWVAD( CKData * pKData );
	virtual ~CWVAD();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CWVAD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	���ײ���ָ��EMV
class  CEMV : public CTechnique
{
public:
	// Constructors
	CEMV( );
	CEMV( CKData * pKData );
	virtual ~CEMV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CEMV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	�����ǿ��ָ��VRSI
class  CVRSI : public CTechnique
{
public:
	// Constructors
	CVRSI( );
	CVRSI( CKData * pKData );
	virtual ~CVRSI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVRSI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	�������ǿ��ָ��VRSI
class  CNVRSI : public CVRSI
{
public:
	// Constructors
	CNVRSI( );
	CNVRSI( CKData * pKData );
	virtual ~CNVRSI();

public:
	// Operations
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	��/ɢָ��A/D
class  CAD : public CTechnique
{
public:
	// Constructors
	CAD( );
	CAD( CKData * pKData );
	virtual ~CAD();

public:
	virtual	void Clear( );

	// Attributes
	int				m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CAD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	�ۻ�ָ��CI
class  CCI : public CTechnique
{
public:
	// Constructors
	CCI( );
	CCI( CKData * pKData );
	virtual ~CCI();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	�ڶ���
//	���ָ��KDJ
class  CKDJ : public CTechnique
{
public:
	// Constructors
	CKDJ( );
	CKDJ( CKData * pKData );
	virtual ~CKDJ();

protected:
	BOOL	CalculateRSV( double * pValue, int nIndex );

public:
	virtual	void Clear( );

	// Attributes
	enum modeJ	{
		mode3K2D	=	0x01,
		mode3D2K	=	0x02,
	};
	int		m_nRSVDays;
	int		m_nKDays;
	int		m_nDDays;
	int		m_nJ;		//	1 For 3K-2D, 2 For 3D-2K
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKDJ & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double *pValue1, double *pValue2, double *pValue3, int nIndex, BOOL bUseLast );
};

//	����ָ��R
class  CR : public CTechnique
{
public:
	// Constructors
	CR( );
	CR( CKData * pKData );
	virtual ~CR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	���ǿ��ָ��
class  CRSI : public CTechnique
{
public:
	// Constructors
	CRSI( );
	CRSI( CKData * pKData );
	virtual ~CRSI();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwDays;
	int		m_itsSold;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CRSI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	������BIAS
class  CBIAS : public CTechnique
{
public:
	// Constructors
	CBIAS( );
	CBIAS( CKData * pKData );
	virtual ~CBIAS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBIAS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	����ָ��MTM
class  CMTM : public CTechnique
{
public:
	// Constructors
	CMTM( );
	CMTM( CKData * pKData );
	virtual ~CMTM();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMTM & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	����ָ��DMI
class  CDMI : public CTechnique
{
public:
	// Constructors
	CDMI( );
	CDMI( CKData * pKData );
	virtual ~CDMI();

protected:
	double	*	m_pdDMICache;
	int		*	m_pnDMICacheIndex;
	int			m_nDMICacheCurrent;
	BOOL	CalculateDM( double *pDMPlus, double *pDMMinus, double *pTR, int nIndex );
	BOOL	CalculateDIDX( double *pDIPlus, double *pDIMinus, double *pDX, int nIndex, int nDays );
public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDMI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pDIPlus, double * pDIMinus, double *pADX, double *pADXR, int nIndex, BOOL bUseLast );
};

//	�䶯��ָ��ROC
class  CROC : public CTechnique
{
public:
	// Constructors
	CROC( );
	CROC( CKData * pKData );
	virtual ~CROC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CROC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	˳��ͨ��ָ��CCI
class  CCCI : public CTechnique
{
public:
	// Constructors
	CCCI( );
	CCCI( CKData * pKData );
	virtual ~CCCI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	double	m_dQuotiety;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCCI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	����ָ��CV
class  CCV : public CTechnique
{
public:
	// Constructors
	CCV( );
	CCV( CKData * pKData );
	virtual ~CCV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMAHLDays;
	int		m_nCVDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	������Ըָ��ARBR
class  CARBR : public CTechnique
{
public:
	// Constructors
	CARBR( );
	CARBR( CKData * pKData );
	virtual ~CARBR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CARBR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pAR, double *pBR, int nIndex, BOOL bUseLast );
};

//	�м���Ըָ��CR
class  CCR : public CTechnique
{
public:
	// Constructors
	CCR( );
	CCR( CKData * pKData );
	virtual ~CCR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADaysA;
	int		m_nMADaysB;
	int		m_nMADaysC;
	int		m_nMADaysD;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pA, double * pB, double * pC, double * pD, int nIndex, BOOL bUseLast );
};

//	�ڶ���OSC
class  COSC : public CTechnique
{
public:
	// Constructors
	COSC( );
	COSC( CKData * pKData );
	virtual ~COSC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( COSC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	���ڶ�ָ��UOS
class  CUOS : public CTechnique
{
public:
	// Constructors
	CUOS( );
	CUOS( CKData * pKData );
	virtual ~CUOS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_nDays3;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CUOS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	�ƶ�ƽ���ڶ���MAOSC
class  CMAOSC : public CTechnique
{
public:
	// Constructors
	CMAOSC( );
	CMAOSC( CKData * pKData );
	virtual ~CMAOSC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMAOSC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	3��6�չ���3-6BIAS
class  C36BIAS : public CTechnique
{
public:
	// Constructors
	C36BIAS( );
	C36BIAS( CKData * pKData );
	virtual ~C36BIAS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( C36BIAS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	ȥ�Ƽ۸�ڶ�ָ��DPO
class  CDPO : public CTechnique
{
public:
	// Constructors
	CDPO( );
	CDPO( CKData * pKData );
	virtual ~CDPO();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDPO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	ȷȻָ��KST
class  CKST : public CTechnique
{
public:
	// Constructors
	CKST( );
	CKST( CKData * pKData );
	virtual ~CKST();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwROCDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKST & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	������
//	��Χ����ָ��REI
class  CREI : public CTechnique
{
public:
	// Constructors
	CREI( );
	CREI( CKData * pKData );
	virtual ~CREI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CREI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	����˶�ָ��DMKI
class  CDMKI : public CTechnique
{
public:
	// Constructors
	CDMKI( );
	CDMKI( CKData * pKData );
	virtual ~CDMKI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDMKI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	���ȱ�PCNT
class  CPCNT : public CTechnique
{
public:
	// Constructors
	CPCNT( );
	CPCNT( CKData * pKData );
	virtual ~CPCNT();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPCNT & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	���ȷ���HLC
class  CHLC : public CTechnique
{
public:
	// Constructors
	CHLC( );
	CHLC( CKData * pKData );
	virtual ~CHLC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CHLC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	���Ʋ���ָ��CDP
class  CCDP : public CTechnique
{
public:
	// Constructors
	CCDP( );
	CCDP( CKData * pKData );
	virtual ~CCDP();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCDP & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pAH, double * pNH, double * pAL, double * pNL, int nIndex, BOOL bUseLast );
};

//	�ۻ��ڶ�ָ��ASI
class  CASI : public CTechnique
{
public:
	// Constructors
	CASI( );
	CASI( CKData * pKData );
	virtual ~CASI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CASI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	����ָ��ATR
class  CATR : public CTechnique
{
public:
	// Constructors
	CATR( );
	CATR( CKData * pKData );
	virtual ~CATR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CATR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	����Ϊ����
//	���ڰڶ���CYO
class  CCYO : public CTechnique
{
public:
	// Constructors
	CCYO( );
	CCYO( CKData * pKData );
	virtual ~CCYO();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMTMDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCYO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	ȥ�����ڰڶ���DCYO
class  CDCYO : public CTechnique
{
public:
	// Constructors
	CDCYO( );
	CDCYO( CKData * pKData );
	virtual ~CDCYO();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMTMDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDCYO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	��Ի�����HSL
class  CHSL : public CTechnique
{
public:
	// Constructors
	CHSL( );
	CHSL( CKData * pKData );
	virtual ~CHSL();

	static	double GetMainChangeHand( DWORD dwMarket, CKData & kdata, int nIndexKD );

public:
	virtual	void Clear( );

	CStockInfo	m_stockinfo;

	// Attributes
	int		m_nDays;	// Not Used
	int		m_nMADays;	// Not Used
	int		m_itsGoldenFork;	// Not Used
	int		m_itsDeadFork;		// Not used
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CHSL & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	ȥ�ưٷֱ�ָ��DPER
class  CDPER : public CTechnique
{
public:
	// Constructors
	CDPER( );
	CDPER( CKData * pKData );
	virtual ~CDPER();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_nDetrendDays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDPER & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

#define	TECHUSER_API
#include "TechUser.h"

// �û����ָ��
class  CTechUser : public CTechnique
{
public:
	// Constructors
	CTechUser( );
	CTechUser( CKData * pKData );
	virtual ~CTechUser();

	// static functions
	static	UINT		GetTechUserCount();
	static	CSPString	GetTechUserName( int nTech );
	static	CSPString	GetTechUserShortName( int nTech );

	// techuser attributes
	BOOL		SetTechUserID( int nTechUserID );
	UINT		GetTechUserID( );
	void		SetStock( CStock * pStock );
	UINT		GetLineCount( );
	CSPString	GetLineName( int nLine );

public:
	virtual	void	Clear( );			//	���

public:
	// Attributes
	virtual	void	SetDefaultParameters( );		// �趨ȱʡ����
	void	AttachParameters( CTechUser & src );	// ָ��������src��ͬ
	virtual	BOOL	IsValidParameters( );			// �����Ƿ�Ϸ�

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );	// �õ������ź�
	virtual	int		GetIntensity( int nIndex, UINT * pnCode = NULL, BOOL bTraceBack = TRUE );	// �õ����������ź�
	virtual	int		GetIntensityTraceback( int nIndex, UINT * pnCode = NULL );					// �õ����������ź�
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );		// �õ������Сֵ
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );					// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, int nIndex, BOOL bUseLast );		// ����ָ��ֵ
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, double * pValue5, int nIndex, BOOL bUseLast );		// ����ָ��ֵ

protected:
	UINT			m_nTechUserID;
	TECHUSER_INFO	m_techuserinfo;
	CStock *		m_pStock;
};

#endif	//__STKLIB_TECHNIQUE_H__
