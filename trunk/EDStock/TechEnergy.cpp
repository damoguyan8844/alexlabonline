/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

*/

#include	"StdAfx.h"
#include	"Stock.h"
#include	"Technique.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//	CVOLUME
CVOLUME::CVOLUME( )
{
	SetDefaultParameters( );
}

CVOLUME::CVOLUME( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CVOLUME::~CVOLUME()
{
	Clear( );
}

void CVOLUME::SetDefaultParameters( )
{
	m_adwMADays.RemoveAll();
	m_adwMADays.Add( 5 );
	m_adwMADays.Add( 10 );
	m_itsDeviateOnBottom	=	ITS_BUYINTENSE;
	m_itsDeviateOnTop		=	ITS_SELLINTENSE;
	m_itsLong				=	ITS_BUY;
	m_itsShort				=	ITS_SELL;
}

void CVOLUME::AttachParameters( CVOLUME & src )
{
	m_adwMADays.Copy( src.m_adwMADays );
	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
	m_itsLong				=	src.m_itsLong;
	m_itsShort				=	src.m_itsShort;
}

BOOL CVOLUME::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwMADays );
	return ( VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop)
		&& VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CVOLUME::Clear( )
{
	CTechnique::Clear( );
}

int CVOLUME::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( !m_pKData || nIndex < 0 || nIndex >= m_pKData->GetSize() )
		return ITS_NOTHING;

	int	nIntensity	=	GetTrendIntensity( nIndex, m_adwMADays, m_itsLong, m_itsShort, pnCode );
	if( ITS_BUY == nIntensity
		&& m_pKData->IsNewValue( nIndex, FALSE, ITS_DAYS_DEVIATE ) )
	{	// �ױ��룬�ɼ۴��µͲ��ҳɽ�����������
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( ITS_SELL == nIntensity
		&& m_pKData->IsNewValue( nIndex, TRUE, ITS_DAYS_DEVIATE ) )
	{	// �����룬�ɼ۴��¸߲��ҳɽ�����������
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}

	return nIntensity;
}

BOOL CVOLUME::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );

	double	dMin	=	0;
	double	dMax	=	1;
	for( int k=nStart; k<=nEnd; k++ )
	{
		KDATA	& kd	=	m_pKData->ElementAt(k);
		if( dMax < kd.m_fVolume )
			dMax	=	(double)kd.m_fVolume;
	}

	dMax	=	dMax + 1;
	if( dMax - dMin < 3 )
		dMax	=	dMin + 3;

	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;
	return TRUE;
}

/***
	����nDays��ƽ���ɽ���
*/
BOOL CVOLUME::Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays );

	int	nCount	=	0;
	if( nDays > nIndex+1 )
		return FALSE;
	double	dResult	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		dResult	+=	m_pKData->ElementAt(k).m_fVolume;
		nCount	++;
		if( nCount == nDays )
		{
			if( pValue )
				*pValue	=	dResult / nDays;
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CNVI
CNVI::CNVI( )
{
	SetDefaultParameters( );
}

CNVI::CNVI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CNVI::~CNVI()
{
	Clear( );
}

void CNVI::SetDefaultParameters( )
{
	m_nMADays	=	25;
	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CNVI::AttachParameters( CNVI & src )
{
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CNVI::IsValidParameters( )
{
	return ( VALID_DAYS( m_nMADays ) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CNVI::Clear( )
{
	CTechnique::Clear( );
}

int CNVI::GetSignal( int nIndex, UINT * pnCode )
{
	PrepareCache( 0, -1, FALSE );
	// �������
	return GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
}

BOOL CNVI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	NVI��ֵ = 100
	�������ɽ���������С NVI = ǰһ��NVI + 100 * �ǵ��� ����NVI = ǰһ��NVI
*/
BOOL CNVI::Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	// Calculate
	if( m_nMADays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue, pMA ) )
		return TRUE;

	double	dValueNew = 0, dMANew = 0;
	if( bUseLast && pValue && pMA )
	{
		if( 0 == nIndex )
			dValueNew	=	100;
		else if( m_pKData->ElementAt(nIndex).m_fVolume < m_pKData->ElementAt(nIndex-1).m_fVolume
				&& m_pKData->MaindataAt(nIndex-1) > 1e-4 && m_pKData->MaindataAt(nIndex) > 1e-4 )
			dValueNew	=	(*pValue) * m_pKData->MaindataAt(nIndex) / m_pKData->MaindataAt(nIndex-1);
		else
			dValueNew	=	*pValue;
		dMANew	=	(*pMA) * (m_nMADays-1) / (m_nMADays+1) + dValueNew * 2 / (m_nMADays+1);
		StoreToCache( nIndex, &dValueNew, &dMANew );
	}
	else
	{
		for( int k=0; k<=nIndex; k++ )
		{
			if( 0 == k )
				dValueNew	=	100;
			else if( m_pKData->ElementAt(k).m_fVolume < m_pKData->ElementAt(k-1).m_fVolume
					&& m_pKData->MaindataAt(k-1) > 1e-4 && m_pKData->MaindataAt(k) > 1e-4 )
				dValueNew	=	dValueNew * m_pKData->MaindataAt(k) / m_pKData->MaindataAt(k-1);

			if( 0 == k )
				dMANew	=	dValueNew;
			else
				dMANew	=	dMANew * (m_nMADays-1) / (m_nMADays+1) + dValueNew * 2 / (m_nMADays+1);
			
			StoreToCache( k, &dValueNew, &dMANew );
		}
	}

	if( pValue )	*pValue	=	dValueNew;
	if( pMA )		*pMA	=	dMANew;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CPVI
CPVI::CPVI( )
{
	SetDefaultParameters( );
}

CPVI::CPVI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CPVI::~CPVI()
{
	Clear( );
}

void CPVI::SetDefaultParameters( )
{
	m_nMADays	=	25;
	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CPVI::AttachParameters( CPVI & src )
{
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CPVI::IsValidParameters( )
{
	return ( VALID_DAYS( m_nMADays ) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CPVI::Clear( )
{
	CTechnique::Clear( );
}

int CPVI::GetSignal( int nIndex, UINT * pnCode )
{
	PrepareCache( 0, -1, FALSE );
	// �������
	return GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
}

BOOL CPVI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	PVI��ֵ100
	�������ɽ��������մ� PVI = ǰһ��PVI + 100 * �ǵ��� ����PVI = ǰһ��PVI
*/
BOOL CPVI::Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	// Calculate
	if( m_nMADays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue, pMA ) )
		return TRUE;

	double	dValueNew = 0, dMANew = 0;
	if( bUseLast && pValue && pMA )
	{
		if( 0 == nIndex )
			dValueNew	=	100;
		else if( m_pKData->ElementAt(nIndex).m_fVolume > m_pKData->ElementAt(nIndex-1).m_fVolume
				&& m_pKData->MaindataAt(nIndex-1) > 1e-4 && m_pKData->MaindataAt(nIndex) > 1e-4 )
			dValueNew	=	(*pValue) * m_pKData->MaindataAt(nIndex) / m_pKData->MaindataAt(nIndex-1);
		else
			dValueNew	=	*pValue;
		dMANew	=	(*pMA) * (m_nMADays-1) / (m_nMADays+1) + dValueNew * 2 / (m_nMADays+1);
		StoreToCache( nIndex, &dValueNew, &dMANew );
	}
	else
	{
		for( int k=0; k<=nIndex; k++ )
		{
			if( 0 == k )
				dValueNew	=	100;
			else if( m_pKData->ElementAt(k).m_fVolume > m_pKData->ElementAt(k-1).m_fVolume
					&& m_pKData->MaindataAt(k-1) > 1e-4 && m_pKData->MaindataAt(k) > 1e-4 )
				dValueNew	=	dValueNew * m_pKData->MaindataAt(k) / m_pKData->MaindataAt(k-1);

			if( 0 == k )
				dMANew	=	dValueNew;
			else
				dMANew	=	dMANew * (m_nMADays-1) / (m_nMADays+1) + dValueNew * 2 / (m_nMADays+1);
			StoreToCache( k, &dValueNew, &dMANew );
		}
	}

	if( pValue )	*pValue	=	dValueNew;
	if( pMA )		*pMA	=	dMANew;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CVR
CVR::CVR( )
{
	SetDefaultParameters( );
}

CVR::CVR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CVR::~CVR()
{
	Clear( );
}

void CVR::SetDefaultParameters( )
{
	m_nDays		=	12;
	m_itsLong			=	ITS_BUY;
	m_itsShort			=	ITS_SELL;
}

void CVR::AttachParameters( CVR & src )
{
	m_nDays		=	src.m_nDays;
	m_itsLong		=	src.m_itsLong;
	m_itsShort		=	src.m_itsShort;
}

BOOL CVR::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CVR::Clear( )
{
	CTechnique::Clear( );
}

int CVR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( !m_pKData || nIndex < 0 || nIndex >= m_pKData->GetSize() )
		return ITS_NOTHING;

	int	nMaxDays	=	m_nDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.309, 0.682 ) )
		return ITS_NOTHING;

	double	dNowClose	=	m_pKData->ElementAt(nIndex).m_fClose;

	double	dVRNow;
	if( !Calculate( &dVRNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nIntensity	=	GetTrendIntensity1( nIndex, m_itsLong, m_itsShort, pnCode );
	if( dVRNow < dLiminalLow && nIntensity == m_itsLong )
	{	// ��λ��������
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsLong;
	}
	if( dVRNow > dLiminalHigh && nIntensity == m_itsShort )
	{	// ��λ��������
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsShort;
	}
	return	ITS_NOTHING;
}

BOOL CVR::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	     n���������ճɽ���+1/2���n���ܳɽ���
	VR = ������������������������---------��- ��100
	     n�����µ��ճɽ���+1/2���n���ܳɽ���
*/
BOOL CVR::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dINTV = 0, dDETV = 0, dTV = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		double	dAmount	=	m_pKData->ElementAt(k).m_fVolume;
		if( m_pKData->MaindataAt(k) > m_pKData->MaindataAt(k-1) )
			dINTV	+=	dAmount;
		if( m_pKData->MaindataAt(k) < m_pKData->MaindataAt(k-1) )
			dDETV	+=	dAmount;
		dTV	+=	dAmount;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( dDETV + dTV/2 < 1e-4 )
				return FALSE;
			if( pValue )	*pValue	=	(dINTV + dTV/2) * 100 /(dDETV + dTV/2);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CVROC
CVROC::CVROC( )
{
	SetDefaultParameters( );
}

CVROC::CVROC( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CVROC::~CVROC()
{
	Clear( );
}

void CVROC::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_nMADays	=	10;
	m_itsDeviateOnBottom	=	ITS_BUY;
	m_itsDeviateOnTop		=	ITS_SELL;
}

void CVROC::AttachParameters( CVROC & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;
	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
}

BOOL CVROC::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop) );
}

void CVROC::Clear( )
{
	CTechnique::Clear( );
}

int CVROC::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh ) )
		return ITS_NOTHING;

	if( IsDeviateOnBottom( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// �ױ���
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( IsDeviateOnTop( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// ������
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}

	return	ITS_NOTHING;
}

BOOL CVROC::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	          ���ճɽ���-n��ǰ�ɽ���
	VROC =   ---------------------- * 100
	              ���ճɽ���
*/
BOOL CVROC::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	if( m_pKData->ElementAt(nIndex-m_nDays).m_fVolume <= 0
		|| m_pKData->ElementAt(nIndex).m_fVolume <= 0 )
		return FALSE;

	double	x	=	m_pKData->ElementAt(nIndex).m_fVolume;
	double	y	=	m_pKData->ElementAt(nIndex-m_nDays).m_fVolume;
	if( pValue )
		*pValue	=	(x - y) * 100 / y;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

BOOL CVROC::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	COBV
COBV::COBV( )
{
	SetDefaultParameters( );
}

COBV::COBV( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

COBV::~COBV()
{
	Clear( );
}

void COBV::SetDefaultParameters( )
{
	m_itsDeviateOnBottom	=	ITS_BUY;
	m_itsDeviateOnTop		=	ITS_SELL;
}

void COBV::AttachParameters( COBV & src )
{
	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
}

BOOL COBV::IsValidParameters( )
{
	return ( VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop) );
}

void COBV::Clear( )
{
	CTechnique::Clear( );
}

int COBV::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	PrepareCache( 0, -1, FALSE );

	if( IsDeviateOnBottom( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// �ױ���
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( IsDeviateOnTop( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// ������
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}

	return	ITS_NOTHING;
}

BOOL COBV::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	�������̼۱�ǰһ�����̼۸ߣ���ɽ�����Ϊ����
	�������̼۽�ǰһ�����̼۵ͣ���ɽ�����Ϊ����
	�ۼ�ÿ��֮���򸺳ɽ���������OBVֵ
*/
BOOL COBV::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	// Calculate
	double	dValueNew = 0;
	if( bUseLast && pValue )
	{
		if( 0 == nIndex )
			dValueNew	=	m_pKData->ElementAt(nIndex).m_fVolume;
		else if( m_pKData->MaindataAt(nIndex) > m_pKData->MaindataAt(nIndex-1) )
			dValueNew	=	*pValue + m_pKData->ElementAt(nIndex).m_fVolume;
		else if( m_pKData->MaindataAt(nIndex) < m_pKData->MaindataAt(nIndex-1) )
			dValueNew	=	*pValue - m_pKData->ElementAt(nIndex).m_fVolume;
		else
			dValueNew	=	*pValue;

		StoreToCache( nIndex, &dValueNew );
	}
	else
	{
		for( int k=0; k<=nIndex; k++ )
		{
			if( 0 == k )
				dValueNew	=	m_pKData->ElementAt(k).m_fVolume;
			else if( m_pKData->MaindataAt(k) > m_pKData->MaindataAt(k-1) )
				dValueNew	+=	m_pKData->ElementAt(k).m_fVolume;
			else if( m_pKData->MaindataAt(k) < m_pKData->MaindataAt(k-1) )
				dValueNew	-=	m_pKData->ElementAt(k).m_fVolume;

			StoreToCache( k, &dValueNew );
		}
	}

	if( pValue )
		*pValue	=	dValueNew;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CMOBV
CMOBV::CMOBV( )
{
	SetDefaultParameters( );
}

CMOBV::CMOBV( CKData * pKData )
	: COBV( pKData )
{
	SetDefaultParameters( );
}

CMOBV::~CMOBV()
{
	Clear( );
}

void CMOBV::SetDefaultParameters( )
{
	COBV::SetDefaultParameters();
	m_nDays1	=	12;
	m_nDays2	=	26;
	m_itsGoldenFork		=	ITS_BUY;
	m_itsDeadFork		=	ITS_SELL;
}

void CMOBV::AttachParameters( CMOBV & src )
{
	COBV::AttachParameters( src );
	m_nDays1	=	src.m_nDays1;
	m_nDays2	=	src.m_nDays2;
	m_itsGoldenFork		=	src.m_itsGoldenFork;
	m_itsDeadFork		=	src.m_itsDeadFork;
}

BOOL CMOBV::IsValidParameters( )
{
	return ( COBV::IsValidParameters()
		&& VALID_DAYS(m_nDays1) && VALID_DAYS(m_nDays2)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CMOBV::Clear( )
{
	CTechnique::Clear( );
}

int CMOBV::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( !m_pKData || nIndex < 0 || nIndex >= m_pKData->GetSize() )
		return ITS_NOTHING;

	PrepareCache( 0, -1, FALSE );
	
	int	nMaxDays	=	max( m_nDays1 , m_nDays2 );
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPreparePrice( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.5, 0.5 ) )
		return ITS_NOTHING;

	double	dPriceNow	=	m_pKData->MaindataAt(nIndex);

	if( dPriceNow < dLiminalLow
		&& ( IsGoldenFork( nIndex, m_pdCache1, m_pdCache2 ) || IsGoldenFork( nIndex, m_pdCache2, m_pdCache3 ) ) )
	{	// ��λ���
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dPriceNow > dLiminalHigh
		&& ( IsDeadFork( nIndex, m_pdCache1, m_pdCache2 ) || IsDeadFork( nIndex, m_pdCache2, m_pdCache3 ) ) )
	{	// ��λ����
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CMOBV::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo3( nStart, nEnd, pdMin, pdMax, this );
}

/***
	*pValue1  = ����OBV
	*pValue2  = m_nDays1��OBVƽ��ֵ
	*pValue3  = m_nDays2��OBVƽ��ֵ
*/
BOOL CMOBV::Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	int	nMaxDays	=	max(m_nDays1,m_nDays2);
	if( nMaxDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2, pValue3 ) )
		return TRUE;

	double	dOBV = 0, dMOBV1 = 0, dMOBV2 = 0;
	int	nCount	=	0;
	BOOL	bHasLast	=	bUseLast;
	for( int k=nIndex; k>=0; k-- )
	{
		double	dTemp = 0;
		if( bUseLast && nIndex == k && pValue1 )
			dTemp	=	*pValue1;
		if( COBV::Calculate( &dTemp, k, bUseLast && nIndex == k && pValue1 ) )
		{
			if( nIndex == k )
				dOBV	=	dTemp;
			if( nCount < m_nDays1 )
				dMOBV1	+=	dTemp;
			if( nCount < m_nDays2 )
				dMOBV2	+=	dTemp;

			nCount	++;
			if( nCount >= m_nDays1 && nCount >= m_nDays2 )
			{
				if( pValue1 )	*pValue1	=	dOBV;
				if( pValue2 )	*pValue2	=	dMOBV1 / m_nDays1;
				if( pValue3 )	*pValue3	=	dMOBV2 / m_nDays2;
				StoreToCache( nIndex, pValue1, pValue2, pValue3 );
				return TRUE;
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CMFI
CMFI::CMFI( )
{
	SetDefaultParameters( );
}

CMFI::CMFI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMFI::~CMFI()
{
	Clear( );
}

void CMFI::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_itsLong	=	ITS_BUY;
	m_itsShort	=	ITS_SELL;
}

void CMFI::AttachParameters( CMFI & src )
{
	m_nDays		=	src.m_nDays;
	m_itsLong	=	src.m_itsLong;
	m_itsShort	=	src.m_itsShort;
}

BOOL CMFI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CMFI::Clear( )
{
	CTechnique::Clear( );
}

int CMFI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dValue = 0;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dValue <= 20 )
	{	// ��λ
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsLong;
	}
	if( dValue >= 80 )
	{	// ��λ
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsShort;
	}
	return	ITS_NOTHING;
}

BOOL CMFI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	PMF �� NMF ���¼��㣺

		TP = (High+Low+Close)/3   ���յ��м��

		PMF = n���ڣ�TP�����յ� (TP*�ɽ���) ֮�͡�
		NMF = n���ڣ�TP�½��յ� (TP*�ɽ���) ֮�͡�	

	MFI = 100 * PMF / (PMF + NMF)
	��ע��MR = PMF/NMF
*/
BOOL CMFI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dPMF = 0, dNMF = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		double	dTP		= (kd.m_fHigh+kd.m_fLow+kd.m_fClose)/3.;
		double	dTPLast = (kdLast.m_fHigh+kdLast.m_fLow+kdLast.m_fClose)/3.;
		if( dTP > dTPLast )
			dPMF	+=	dTP * kd.m_fVolume;
		if( dTPLast > dTP )
			dNMF	+=	dTP * kd.m_fVolume;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( fabs(dPMF+dNMF) < 1e-4 )
				return FALSE;
			if( pValue )	*pValue	=	100 * dPMF / (dPMF + dNMF);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CVMACD
CVMACD::CVMACD( )
{
	SetDefaultParameters( );
}

CVMACD::CVMACD( CKData * pKData )
	: CMACD( pKData )
{
	SetDefaultParameters( );
}

CVMACD::~CVMACD()
{
	Clear( );
}

/***
	�ɽ�����MACD
	EMA  = �����ƶ���ֵ
	EMA2 = �����ƶ���ֵ
	DIF  = �����ƶ���ֵ - �����ƶ���ֵ
	DEA  = DIF���ƶ�ƽ��ֵ
	��״��ֵ = DIF - DEA
*/
BOOL CVMACD::Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nEMA1Days > nIndex+1 || m_nEMA2Days > nIndex+1 || m_nDIFDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pdEMA1, pdEMA2, pdDIF, pdDEA ) )
		return TRUE;

	// Calculate EMA1, EMA2, DIF, DEA
	double	dEMA1New = 0, dEMA2New = 0, dDIFNew = 0, dDEANew = 0;
	if( bUseLast && pdEMA1 && pdEMA2 && pdDEA )
	{
		dEMA1New	=	(*pdEMA1)*(m_nEMA1Days-1)/(m_nEMA1Days+1) + 2 * m_pKData->ElementAt(nIndex).m_fVolume/(m_nEMA1Days+1);
		dEMA2New	=	(*pdEMA2)*(m_nEMA2Days-1)/(m_nEMA2Days+1) + 2 * m_pKData->ElementAt(nIndex).m_fVolume/(m_nEMA2Days+1);
		dDIFNew		=	dEMA1New-dEMA2New;
		dDEANew		=	(*pdDEA)*(m_nDIFDays-1)/(m_nDIFDays+1) + 2 * dDIFNew/(m_nDIFDays+1);
	}
	else
	{
		double	factor1 = 1, factor2 = 1;
		int k;
		for( k=nIndex; k > 0; k-- )
		{
			factor1		*=	((double)(m_nEMA1Days-1))/(m_nEMA1Days+1);
			factor2		*=	((double)(m_nEMA2Days-1))/(m_nEMA2Days+1);
			if( factor1 < 0.001 && factor2 < 0.001 )	// ̫����ǰ������Ӱ���С�����Բ���
				break;
		}
		dEMA1New	=	m_pKData->ElementAt(k).m_fVolume;
		dEMA2New	=	m_pKData->ElementAt(k).m_fVolume;
		dDIFNew		=	dEMA1New - dEMA2New;
		dDEANew		=	dDIFNew;
		for( ; k<=nIndex; k++ )
		{
			dEMA1New	=	dEMA1New * (m_nEMA1Days-1)/(m_nEMA1Days+1) + 2 * m_pKData->ElementAt(k).m_fVolume/(m_nEMA1Days+1);
			dEMA2New	=	dEMA2New * (m_nEMA2Days-1)/(m_nEMA2Days+1) + 2 * m_pKData->ElementAt(k).m_fVolume/(m_nEMA2Days+1);
			dDIFNew		=	dEMA1New - dEMA2New;
			dDEANew		=	dDEANew * (m_nDIFDays-1)/(m_nDIFDays+1) + 2 * dDIFNew / (m_nDIFDays+1);
		}
	}

	if( pdEMA1 )		*pdEMA1	=	dEMA1New;
	if( pdEMA2 )		*pdEMA2	=	dEMA2New;
	if( pdDIF )			*pdDIF	=	dDIFNew;
	if( pdDEA )			*pdDEA	=	dDEANew;
	StoreToCache( nIndex, pdEMA1, pdEMA2, pdDIF, pdDEA );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CWVAD
CWVAD::CWVAD( )
{
	SetDefaultParameters( );
}

CWVAD::CWVAD( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CWVAD::~CWVAD()
{
	Clear( );
}

void CWVAD::SetDefaultParameters( )
{
	m_nDays		=	24;
	m_itsLong				=	ITS_BUY;
	m_itsShort				=	ITS_SELL;
}

void CWVAD::AttachParameters( CWVAD & src )
{
	m_nDays		=	src.m_nDays;
	m_itsLong				=	src.m_itsLong;
	m_itsShort				=	src.m_itsShort;
}

BOOL CWVAD::IsValidParameters( )
{
	return ( VALID_DAYS( m_nDays )
		&& VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CWVAD::Clear( )
{
	CTechnique::Clear( );
}

int CWVAD::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh ) )
		return ITS_NOTHING;

	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dLast, dNow;
	if( !Calculate( &dLast, nIndex-1, FALSE )
		|| !Calculate( &dNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dNow < dLiminalLow )
	{	// ��λ
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsLong;
	}
	if( dNow > dLiminalHigh  )
	{	// ��λ
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsShort;
	}

	return	ITS_NOTHING;
}

BOOL CWVAD::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	A = �������̼� - ���쿪�̼�
	B = ������߼� - ������ͼ�
	C = A��B��V(�ɽ���)
	WVAD = �ۼ�n���Cֵ
*/
BOOL CWVAD::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	int	nCount	=	0;
	double	dResult = 0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		if( kd.m_fHigh > kd.m_fLow )
			dResult	+=	(((double)kd.m_fClose) - kd.m_fOpen)*kd.m_fVolume/(((double)kd.m_fHigh)-kd.m_fLow);

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	dResult;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CEMV
CEMV::CEMV( )
{
	SetDefaultParameters( );
}

CEMV::CEMV( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CEMV::~CEMV()
{
	Clear( );
}

void CEMV::SetDefaultParameters( )
{
	m_nDays		=	14;
	m_nMADays	=	9;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsGoldenFork	=	ITS_SELL;
}

void CEMV::AttachParameters( CEMV & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CEMV::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CEMV::Clear( )
{
	CTechnique::Clear( );
}

int CEMV::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.5, 0.5 ) )
		return ITS_NOTHING;

	double	dEMV;
	if( !Calculate( &dEMV, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dEMV < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// ��λ���
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dEMV > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// ��λ����
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	return	ITS_NOTHING;
/*  FROM BOOK */
/*	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double dValue;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dValue > 0 )
	{
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsLong;
	}
	if( dValue < 0 )
	{
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsShort;
	}
	
	return	ITS_NOTHING;
*/
}

BOOL CEMV::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	A = ��������� + ������ͣ��� 2
	B = ��ǰһ����� + ǰһ����ͣ���2
	C = ������� - �������
	EM = ��A-B����C�½���ɽ���
	EMV = �ۼ�n���EMֵ
*/
BOOL CEMV::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dEMV = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		if( 0 == kd.m_fVolume )
			return FALSE;
		double	dDIF = 0;
		dDIF	=	(kd.m_fHigh+kd.m_fLow)/2 - (((double)kdLast.m_fHigh)+kdLast.m_fLow)/2;
		dEMV	+=	dDIF * (kd.m_fHigh-kd.m_fLow) / kd.m_fVolume;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )
				*pValue	=	dEMV / m_nDays;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CEMV::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CVRSI
CVRSI::CVRSI( )
{
	SetDefaultParameters( );
}

CVRSI::CVRSI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CVRSI::~CVRSI()
{
	Clear( );
}

void CVRSI::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_itsDeviateOnBottom	=	ITS_BUYINTENSE;
	m_itsDeviateOnTop		=	ITS_SELLINTENSE;
	m_itsSold				=	ITS_BUY;
	m_itsBought				=	ITS_SELL;
}

void CVRSI::AttachParameters( CVRSI & src )
{
	m_nDays		=	src.m_nDays;
	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
	m_itsSold				=	src.m_itsSold;
	m_itsBought				=	src.m_itsBought;
}

BOOL CVRSI::IsValidParameters( )
{
	return ( VALID_DAYS( m_nDays )
		&& VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop)
		&& VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CVRSI::Clear( )
{
	CTechnique::Clear( );
}

int CVRSI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh ) )
		return ITS_NOTHING;

	double	dNow;
	if( !Calculate( &dNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( IsDeviateOnBottom( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// �ױ���
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( IsDeviateOnTop( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// ������
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}
	if( dNow < dLiminalLow )
	{	// ����
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dNow > dLiminalHigh )
	{	// ����
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}

	return	ITS_NOTHING;
}

BOOL CVRSI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	VP = N���ڳɽ��������յ�ƽ���ɽ���
	VQ = N���ڳɽ��������յ�ƽ���ɽ���
	VRSI = 100 * VP / (VP+VQ)
*/
BOOL CVRSI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dVP = 0, dVQ = 0, dResult = 0;
	int	nCount = 0, p = 0, q = 0;
	for( int k=nIndex; k>=1; k-- )
	{
		if( m_pKData->MaindataAt(k) >= m_pKData->MaindataAt(k-1) )
		{
			dVP	+=	m_pKData->ElementAt(k).m_fVolume;
			p	++;
		}
		else
		{
			dVQ	+=	m_pKData->ElementAt(k).m_fVolume;
			q	++;
		}

		nCount	++;
		if( nCount == m_nDays )
		{
			if( p > 0 )	dVP	=	dVP / p;
			if( q > 0 )	dVQ	=	dVQ / q;
			if( dVQ < 1e-4 )
				dResult	=	100;
			else
				dResult	=	100 - 100. / (1 + dVP / dVQ);
			if( pValue )	*pValue	=	dResult;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CNVRSI
CNVRSI::CNVRSI( )
{
	SetDefaultParameters( );
}

CNVRSI::CNVRSI( CKData * pKData )
	: CVRSI( pKData )
{
	SetDefaultParameters( );
}

CNVRSI::~CNVRSI()
{
	Clear( );
}

/***
	VP = N���ڳɽ��������յ��ܳɽ���
	VQ = N���ڳɽ��������յ��ܳɽ���
	VRSI = 100 * VP / (VP+VQ)
*/
BOOL CNVRSI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dUV = 0, dV = 0, dResult = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		if( m_pKData->ElementAt(k).m_fVolume > m_pKData->ElementAt(k-1).m_fVolume )
			dUV	+=	m_pKData->ElementAt(k).m_fVolume;
		dV	+=	m_pKData->ElementAt(k).m_fVolume;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( dV < 1e-4 )
				dResult	=	50;
			else
				dResult	=	100. * dUV / dV;
			if( pValue )	*pValue	=	dResult;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CAD
CAD::CAD( )
{
	SetDefaultParameters( );
}

CAD::CAD( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CAD::~CAD()
{
	Clear( );
}

void CAD::SetDefaultParameters( )
{
	m_nDays		=	8;
}

void CAD::AttachParameters( CAD & src )
{
	m_nDays		=	src.m_nDays;
}

BOOL CAD::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) );
}

void CAD::Clear( )
{
	CTechnique::Clear( );
}

int CAD::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	// �������ź�
	return	ITS_NOTHING;
}

BOOL CAD::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	A = ������߼� - ������ͼ�
	B = 2 * �������̼� - ������߼� - ������ͼ�
	C = ���ճɽ��� * B / A
	AD = N����C���ܺ�
*/
BOOL CAD::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dAD = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);

		if( kd.m_fHigh-kd.m_fLow > 1e-4 )
			dAD	+=	kd.m_fVolume * (((double)kd.m_fClose)-kd.m_fLow-kd.m_fHigh+kd.m_fClose)/(((double)kd.m_fHigh)-kd.m_fLow);

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	dAD;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CCI
CCI::CCI( )
{
	SetDefaultParameters( );
}

CCI::CCI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCI::~CCI()
{
	Clear( );
}

void CCI::SetDefaultParameters( )
{
}

void CCI::AttachParameters( CCI & src )
{
}

BOOL CCI::IsValidParameters( )
{
	return TRUE;
}

void CCI::Clear( )
{
	CTechnique::Clear( );
}

int CCI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	// �������ź�
	return	ITS_NOTHING;
}

BOOL CCI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	CI = (�������̼� - ���տ��̼�)/(������߼� - ������ͼ�)
*/
BOOL CCI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	KDATA	kd		=	m_pKData->ElementAt(nIndex);
	if( kd.m_fHigh-kd.m_fLow < 1e-4)
		return FALSE;
	double	dCI	=	(((double)kd.m_fClose)-kd.m_fOpen)/(((double)kd.m_fHigh)-kd.m_fLow);
/*
	if( nIndex > 0 )
	{
		KDATA	kdLast	=	m_pKData->ElementAt(nIndex-1);
		if( max(kd.m_fOpen,kd.m_fClose) > kdLast.m_fClose*1.08 && kd.m_fHigh>kdLast.m_fClose)
			dCI	+=	(2.*kd.m_fClose-kd.m_fLow-kdLast.m_fClose)/(((double)kd.m_fHigh)-kdLast.m_fClose);
		if( kd.m_fOpen < kdLast.m_fClose*0.92 )
			dCI	+=	(2.*kd.m_fLow-kd.m_fHigh-kdLast.m_fClose)/(((double)kd.m_fClose)-kd.m_fLow);
	}
*/
	dCI	=	kd.m_fVolume * dCI;
	if( pValue )
		*pValue	=	dCI;
	StoreToCache( nIndex, pValue );
	return TRUE;
}
