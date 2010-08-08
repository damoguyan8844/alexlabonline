/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CRateParam;
		class	COpParam;
		class	CStockOwnContainer;
		class	COpRecordContainer;
		class	CAssetSerialContainer;
		class	CTechStock;
		class	CTechStockContainer;
		class	CStrategy;
*/

#include "stdafx.h"
#include "Strategy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRateParam

CRateParam::CRateParam( )
{
	SetDefault( );
}

// ���׷��ò����Ƿ�Ϸ�
BOOL CRateParam::IsValid( )
{
	return (m_dShaa >= 0 && m_dShaa < 0.1
		&& m_dShab >= 0 && m_dShab < 0.1
		&& m_dShafund >= 0 && m_dShafund < 0.1
		&& m_dSzna >= 0 && m_dSzna < 0.1
		&& m_dSznb >= 0 && m_dSznb < 0.1
		&& m_dSznfund >= 0 && m_dSznfund < 0.1 );
}

// ȱʡ���׷���
void CRateParam::SetDefault( )
{
	m_dShaa		=	0.003;
	m_dShab		=	0.003;
	m_dShafund	=	0.001;
	m_dSzna		=	0.003;
	m_dSznb		=	0.003;
	m_dSznfund	=	0.001;
}

// ���潻�׷������ļ�
void CRateParam::Serialize( CSPArchive &ar )
{
	if( ar.IsStoring( ) )
	{
		ar << m_dShaa;
		ar << m_dShab;
		ar << m_dShafund;
		ar << m_dSzna;
		ar << m_dSznb;
		ar << m_dSznfund;
	}
	else
	{
		ar >> m_dShaa;
		ar >> m_dShab;
		ar >> m_dShafund;
		ar >> m_dSzna;
		ar >> m_dSznb;
		ar >> m_dSznfund;
	}
}

// ���ݹ�Ʊѡ���׷���
double CRateParam::GetRate( CStockInfo & info )
{
	LONG	stocktype	=	info.GetType();
	if( CStock::typeshA == stocktype || CStock::typeshIndex == stocktype )
		return m_dShaa;
	else if( CStock::typeshB == stocktype )
		return m_dShab;
	else if( info.IsShangHai() && info.IsFund() )
		return m_dShafund;
	else if( CStock::typeszA == stocktype || CStock::typeszIndex == stocktype )
		return m_dSzna;
	else if( CStock::typeszB == stocktype )
		return m_dSznb;
	else if( info.IsShenZhen() && info.IsFund() )
		return m_dSznfund;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// COpParam

// �߼��ַ�����ȫ��ָ�� ��һָ��
CSPString AfxGetLogicString( int nLogic )
{
	switch( nLogic )
	{
	case	COpParam::logicAnd:		return	strategy_logicand;
	case	COpParam::logicOr:		return	strategy_logicor;
	default:
		SP_ASSERT( FALSE );
	}
	return "";
}

// ��λ�ַ���
CSPString AfxGetStoreDivString( int nStoreDiv )
{
	switch( nStoreDiv )
	{
	case	COpParam::storedivOnce:			return strategy_sdonce;
	case	COpParam::storedivTwice:		return strategy_sdtwice;
	case	COpParam::storedivThird:		return strategy_sdthird;
	case	COpParam::storedivForth:		return strategy_sdforth;
	case	COpParam::storedivFifth:		return strategy_sdfifth;
	case	COpParam::storedivSixth:		return strategy_sdsixth;
	case	COpParam::storedivSeventh:		return strategy_sdseventh;
	case	COpParam::storedivEighth:		return strategy_sdeighth;
	case	COpParam::storedivNinth:		return strategy_sdninth;
	case	COpParam::storedivTenth:		return strategy_sdtenth;
	default:
		SP_ASSERT( FALSE );
	}
	return "";
}

COpParam::COpParam( )
{
	SetDefault( );
}

// ���������Ƿ�Ϸ�
BOOL COpParam::IsValid( )
{
	if( CKData::ktypeDay != m_nKType && CKData::ktypeWeek != m_nKType && CKData::ktypeMonth != m_nKType )
		return FALSE;
	if( m_nMaindataType < CKData::mdtypeMin || m_nMaindataType > CKData::mdtypeMax )
		return FALSE;
	if( m_nBuyLogic < logicMin || m_nBuyLogic > logicMax )
		return FALSE;
	if( m_nBuyLimit < ITS_MIN || m_nBuyLimit > ITS_MAX )
		return FALSE;
	if( m_dBuyMulti <= 0.89 || m_dBuyMulti > 1.11 )
		return FALSE;
	if( m_nSellLogic < logicMin || m_nSellLogic > logicMax )
		return FALSE;
	if( m_nSellLimit < ITS_MIN || m_nSellLimit > ITS_MAX )
		return FALSE;
	if( m_dSellMulti <= 0.89 || m_dSellMulti > 1.11 )
		return FALSE;
	if( m_atmBegin.GetSize() <= 0 || m_atmEnd.GetSize() <= 0
		|| m_atmBegin.GetSize() != m_atmEnd.GetSize() )
		return FALSE;
	// TIMEZONES
	if( m_nStartAmount < 1000 || m_nStartAmount > 2000000000 )
		return FALSE;
	if( m_nStoreDiv < storedivMin || m_nStoreDiv > COpParam::storedivMax )
		return FALSE;

	if( m_bStopLosing && ( m_dStopLosing <= 0 || m_dStopLosing >= 100 ) )
		return FALSE;
	if( m_bStopProfit && m_dStopProfit <= 0 )
		return FALSE;
	if( m_bLongTrend && CKData::ktypeDay != m_nKTypeLong && CKData::ktypeWeek != m_nKTypeLong && CKData::ktypeMonth != m_nKTypeLong )
		return FALSE;
	if( m_bIndexTrend && CKData::ktypeDay != m_nKTypeIndex && CKData::ktypeWeek != m_nKTypeIndex && CKData::ktypeMonth != m_nKTypeIndex )
		return FALSE;
	return TRUE;
}

// ȱʡ��������
void COpParam::SetDefault( )
{
	m_nKType		=	CKData::ktypeDay;
	m_nMaindataType	=	CKData::mdtypeClose;
	m_nBuyLogic		=	logicAnd;
	m_nBuyLimit		=	ITS_BUY;
	m_dBuyMulti		=	1.0;
	m_nSellLogic	=	logicOr;
	m_nSellLimit	=	ITS_SELL;
	m_dSellMulti	=	0.98;
	CSPTime	tmCurrent	=	CSPTime::GetCurrentTime();
	m_atmBegin.RemoveAll();
	m_atmEnd.RemoveAll();
	m_atmBegin.Add( tmCurrent - CSPTimeSpan( 365 * 3, 0, 0, 0 ) );
	m_atmEnd.Add( tmCurrent );
	m_nStartAmount	=	1000000;
	m_nStoreDiv		=	storedivThird;

	m_bStopLosing		=	FALSE;
	m_bStopProfit		=	FALSE;
	m_dStopLosing		=	0.1;
	m_dStopProfit		=	0.3;
	m_bLongTrend		=	FALSE;
	m_bIndexTrend		=	FALSE;
	m_nKTypeLong	=	CKData::ktypeWeek;
	m_nKTypeIndex	=	CKData::ktypeWeek;
}

// �趨���Ϸ��Ĳ�������Ϊȱʡֵ
void COpParam::SetDefaultOfInvalidMember( )
{
	if( CKData::ktypeDay != m_nKType && CKData::ktypeWeek != m_nKType && CKData::ktypeMonth != m_nKType )
		m_nKType		=	CKData::ktypeDay;
	if( m_nMaindataType < CKData::mdtypeMin || m_nMaindataType > CKData::mdtypeMax )
		m_nMaindataType	=	CKData::mdtypeClose;
	if( m_nBuyLogic < logicMin || m_nBuyLogic > logicMax )
		m_nBuyLogic	=	logicAnd;
	if( m_nBuyLimit < ITS_MIN || m_nBuyLimit > ITS_MAX )
		m_nBuyLimit		=	ITS_BUY;
	if( m_dBuyMulti <= 0.89 || m_dBuyMulti > 1.11 )
		m_dBuyMulti		=	1.0;
	if( m_nSellLogic < logicMin || m_nSellLogic > logicMax )
		m_nSellLogic	=	logicOr;
	if( m_nSellLimit < ITS_MIN || m_nSellLimit > ITS_MAX )
		m_nSellLimit	=	ITS_SELL;
	if( m_dSellMulti <= 0.89 || m_dSellMulti > 1.11 )
		m_dSellMulti	=	0.98;
	if( m_atmBegin.GetSize() <= 0 || m_atmEnd.GetSize() <= 0
		|| m_atmBegin.GetSize() != m_atmEnd.GetSize() )
	{
		m_atmBegin.RemoveAll();
		m_atmEnd.RemoveAll();
		CSPTime	tmCurrent	=	CSPTime::GetCurrentTime();
		m_atmBegin.Add( tmCurrent - CSPTimeSpan( 365 * 3, 0, 0, 0 ) );
		m_atmEnd.Add( tmCurrent );
	}
	// TIMEZONES
	if( m_nStartAmount < 1000 || m_nStartAmount > 2000000000 )
		m_nStartAmount	=	1000000;
	if( m_nStoreDiv < storedivMin || m_nStoreDiv > storedivMax )
		m_nStoreDiv		=	storedivThird;

	if( m_bStopLosing && ( m_dStopLosing <= 0 || m_dStopLosing >= 100 ) )
		m_dStopLosing	=	0.1;
	if( m_bStopProfit && m_dStopProfit <= 0 )
		m_dStopProfit	=	0.3;
	if( m_bLongTrend && CKData::ktypeDay != m_nKTypeLong && CKData::ktypeWeek != m_nKTypeLong && CKData::ktypeMonth != m_nKTypeLong )
		m_nKTypeLong		=	CKData::ktypeWeek;
	if( m_bIndexTrend && CKData::ktypeDay != m_nKTypeIndex && CKData::ktypeWeek != m_nKTypeIndex && CKData::ktypeMonth != m_nKTypeIndex )
		m_nKTypeIndex		=	CKData::ktypeWeek;
}

// ������߶�ȡӲ���ļ�
void COpParam::Serialize( CSPArchive &ar )
{
	if( ar.IsStoring( ) )
	{
		ar << m_nKType;
		ar << m_nMaindataType;
		ar << m_nBuyLogic;
		ar << m_nBuyLimit;
		ar << m_dBuyMulti;
		ar << m_nSellLogic;
		ar << m_nSellLimit;
		ar << m_dSellMulti;
		m_atmBegin.Serialize( ar );
		m_atmEnd.Serialize( ar );
		ar << m_nStartAmount;
		ar << m_nStoreDiv;

		ar << m_bStopLosing;
		ar << m_bStopProfit;
		ar << m_dStopLosing;
		ar << m_dStopProfit;
		ar << m_bLongTrend;
		ar << m_bIndexTrend;
		ar << m_nKTypeLong;
		ar << m_nKTypeIndex;
	}
	else
	{
		ar >> m_nKType;
		ar >> m_nMaindataType;
		ar >> m_nBuyLogic;
		ar >> m_nBuyLimit;
		ar >> m_dBuyMulti;
		ar >> m_nSellLogic;
		ar >> m_nSellLimit;
		ar >> m_dSellMulti;
		m_atmBegin.Serialize( ar );
		m_atmEnd.Serialize( ar );
		ar >> m_nStartAmount;
		ar >> m_nStoreDiv;

		ar >> m_bStopLosing;
		ar >> m_bStopProfit;
		ar >> m_dStopLosing;
		ar >> m_dStopProfit;
		ar >> m_bLongTrend;
		ar >> m_bIndexTrend;
		ar >> m_nKTypeLong;
		ar >> m_nKTypeIndex;
	}
}

// ����ʱ���ǲ�����ģ��ʱ����
BOOL COpParam::IsInTimeZones( CSPTime tm )
{
	for( int i=0; i<m_atmBegin.GetSize() && i<m_atmEnd.GetSize(); i++ )
	{
		if( tm >= m_atmBegin.ElementAt(i) && tm <= m_atmEnd.ElementAt(i) )
			return TRUE;
	}
	return FALSE;
}

// ��ȡģ�⿪ʼʱ��
CSPTime COpParam::GetBeginTime( )
{
	if( m_atmBegin.GetSize() > 0 )
		return m_atmBegin.ElementAt(0);
	return CSPTime::GetCurrentTime();
}

// ��ȡģ�����ʱ��
CSPTime COpParam::GetEndTime( )
{
	if( m_atmEnd.GetSize() > 0 )
		return m_atmEnd.ElementAt(m_atmEnd.GetSize()-1);
	return CSPTime::GetCurrentTime();
}

// ��ȡ��һ������ʱ��
BOOL COpParam::GetNextTradeTime( CSPTime tmNow, CSPTime &tmNext )
{
	CSPTime	sptime( tmNow.GetTime() );
	DWORD	dwDate		=	sptime.ToStockTime( CKData::IsDayOrMin(m_nKType) );
	DWORD	dwDateNext	=	CSPTime::GetStockTimeNext( dwDate, m_nKType );
	if( sptime.FromStockTime( dwDateNext, CKData::IsDayOrMin(m_nKType) ) )
	{
		tmNext	=	sptime.GetTime();
		return TRUE;
	}
	return FALSE;
}

// ��ȡ��ǰģ�����
DWORD COpParam::GetProgress( CSPTime tmNow, DWORD dwProgressMax )
{
	// TIMEZONES
	if( !IsInTimeZones( tmNow ) )
		return 0;
	CSPTime	tmBegin	=	GetBeginTime( );
	CSPTime	tmEnd	=	GetEndTime( );
	double	dBeginNow	=	( tmNow.GetTime() - tmBegin.GetTime() );
	double	dBeginEnd	=	( tmEnd.GetTime() - tmBegin.GetTime() );
	double	dProgress	=	0;
	if( fabs(dBeginEnd) > 1e-4 )
	{
		dProgress	=	( dBeginNow / dBeginEnd ) * dwProgressMax;
	}
	return (DWORD)dProgress;
}

/////////////////////////////////////////////////////////////////////////////
// CStockOwnContainer

// ӵ�й�Ʊ���飬�����Ʊ
BOOL CStockOwnContainer::AddStock( CStockInfo & info, DWORD dwShare, double dBuyPrice )
{
	if( dwShare <= 0 )
		return FALSE;

	// ����Ѿ����ˣ��ı�����
	for( int i=0; i<GetSize(); i++ )
	{
		STOCKOWN	&	own	=	ElementAt(i);
		if( info.IsEqualTo( own.dwMarket, own.szCode ) )
		{
			if( dBuyPrice > 0.005 && own.dwShare + dwShare > 0 )
			{
				own.dBuyPrice	=	(own.dBuyPrice * own.dwShare + dBuyPrice * dwShare) / (own.dwShare + dwShare);
			}
			own.dwShare	+=	dwShare;
			return TRUE;
		}
	}
	// �����µ�
	STOCKOWN	ownnew;
	memset( &ownnew, 0, sizeof(ownnew) );
	strncpy( ownnew.szCode, info.GetStockCode(), min(sizeof(ownnew.szCode)-1,strlen(info.GetStockCode())) );
	ownnew.dwShare		=	dwShare;
	ownnew.dBuyPrice	=	dBuyPrice;
	ownnew.dwMarket		=	info.GetMarket();
	return ( Add(ownnew) >= 0 );
}

// ӵ�й�Ʊ���飬�Ƴ���Ʊ
BOOL CStockOwnContainer::RemoveStock( CStockInfo & info, DWORD dwShare )
{
	for( int i=0; i<GetSize(); i++ )
	{
		STOCKOWN	&	own	=	ElementAt(i);
		if( info.IsEqualTo( own.dwMarket, own.szCode ) )
		{
			if( own.dwShare < dwShare )
				return FALSE;
			own.dwShare	-=	dwShare;
			if( 0 == own.dwShare )
				RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

// ӵ�й�Ʊ���飬�Ƿ��������Ʊ������У�������lpOwn
BOOL CStockOwnContainer::HasThisStock( CStockInfo & info, LPSTOCKOWN lpOwn )
{
	for( int i=0; i<GetSize(); i++ )
	{
		STOCKOWN	&	own	=	ElementAt(i);
		if( info.IsEqualTo( own.dwMarket, own.szCode ) )
		{
			if( lpOwn )
				memcpy( lpOwn, &own, sizeof(own) );
			return ( own.dwShare > 0 );
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COpRecordContainer
// ��������
CSPString	AfxGetStrategyOpTypeString( long lOpType )
{
	CSPString	strOp;
	if( STRATEGY_OPTYPE_BUY == lOpType )
		return strategy_optype_buy;
	else if( STRATEGY_OPTYPE_SELL == lOpType )
		return strategy_optype_sell;
	else if( STRATEGY_OPTYPE_ADDSTOCK == lOpType )
		return strategy_optype_addstock;
	else if( STRATEGY_OPTYPE_REMOVESTOCK == lOpType )
		return strategy_optype_removestock;
	else if( STRATEGY_OPTYPE_ADDCASH == lOpType )
		return strategy_optype_addcash;
	else if( STRATEGY_OPTYPE_REMOVECASH == lOpType )
		return strategy_optype_removecash;
	return strOp;
}

// ������¼���飬�����¼
int COpRecordContainer::AddRecord(long lOpType, CSPTime tm, const char *szCode,
								   DWORD dwShare, double dSharePrice, double dRateCost )
{
	SP_ASSERT( NULL != szCode && strlen(szCode) > 0 );
	if( NULL == szCode || strlen(szCode) <= 0 )
		return -1;

	OPRECORD	record;
	memset( &record, 0, sizeof(record) );

	record.lOpType		=	lOpType;
	record.time			=	tm.GetTime();
	strncpy( record.szCode, szCode, min(sizeof(record.szCode)-1,strlen(szCode)) );
	record.dwShare		=	dwShare;
	record.dSharePrice	=	dSharePrice;
	record.dRateCost	=	dRateCost;
	record.dwMarket		=	CStock::marketUnknown;
	return Add( record );
}

// ������¼���飬�����¼����¼������Ѿ��иù�Ʊ�ļ�¼������Ϊ���ڵ���Ϣ
int COpRecordContainer::AddRecordUniqueStock(long lOpType, CSPTime tm, const char *szCode,
								   DWORD dwShare, double dSharePrice, double dRateCost )
{
	SP_ASSERT( NULL != szCode && strlen(szCode) > 0 );
	if( NULL == szCode || strlen(szCode) <= 0 )
		return -1;

	OPRECORD	record;
	memset( &record, 0, sizeof(record) );

	for( int i=0; i<GetSize(); i++ )
	{
		if( 0 == strncmp( ElementAt(i).szCode, szCode, sizeof(record.szCode) ) )
		{
			ElementAt(i).lOpType		=	lOpType;
			ElementAt(i).time			=	tm.GetTime();
			strncpy( ElementAt(i).szCode, szCode, min(sizeof(ElementAt(i).szCode)-1,strlen(szCode)) );
			ElementAt(i).dwShare		=	dwShare;
			ElementAt(i).dSharePrice	=	dSharePrice;
			ElementAt(i).dRateCost		=	dRateCost;
			ElementAt(i).dwMarket		=	CStock::marketUnknown;
			return i;
		}
	}

	record.lOpType		=	lOpType;
	record.time			=	tm.GetTime();
	strncpy( record.szCode, szCode, min(sizeof(record.szCode)-1,strlen(szCode)) );
	record.dwShare		=	dwShare;
	record.dSharePrice	=	dSharePrice;
	record.dRateCost	=	dRateCost;
	record.dwMarket		=	CStock::marketUnknown;
	return Add( record );
}

// ������¼���飬�õ������¼��
int COpRecordContainer::GetBuyRecordCount()
{
	int	nCount	=	0;
	for( int i=0; i<GetSize(); i++ )
	{
		OPRECORD & record	=	ElementAt(i);
		if( STRATEGY_OPTYPE_BUY == record.lOpType )
			nCount	++;
	}
	return nCount;
}

// ������¼���飬�Ƿ���szCode��Ʊ�������¼
BOOL COpRecordContainer::HasBuyStock( const char * szCode )
{
	if( NULL == szCode || strlen(szCode) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		OPRECORD & record	=	ElementAt(i);
		if( STRATEGY_OPTYPE_BUY == record.lOpType )
		{
			if( 0 == strncmp( record.szCode, szCode, min(sizeof(record.szCode),strlen(szCode)) ) )
				return TRUE;
		}
	}
	return FALSE;
}

// ������¼���飬ͳ�ƽ���������������ɹ�������������棬�������
BOOL COpRecordContainer::StatResults( int *pnTimes, int *pnVictoryTimes, double *pdYieldMax, double *pdYieldMin )
{
	int	nTimes = 0, nVictoryTimes = 0;
	double	dYieldMax = 0, dYieldMin = 0;

	for( int i=0; i<GetSize(); i++ )
	{
		OPRECORD & record	=	ElementAt(i);
		if( STRATEGY_OPTYPE_BUY != record.lOpType )
			continue;

		// ���ҵ�ǰ��������Ķ�Ӧ��������
		for( int j=i+1; j<GetSize(); j++ )
		{
			OPRECORD & record2	=	ElementAt(j);
			if( STRATEGY_OPTYPE_SELL == record2.lOpType && 0 == strncmp(record.szCode,record2.szCode,sizeof(record.szCode)) )
			{	// �����������������������۸�������۸����������
				if( record.dwShare > 0 && record2.dwShare > 0 )
				{
					double	dBuy	=	record.dSharePrice + record.dRateCost/record.dwShare;
					double	dSell	=	record2.dSharePrice - record2.dRateCost/record2.dwShare;
					if( dBuy > 1e-4 )
					{
						double	dYield	=	(dSell - dBuy)/dBuy;
						nTimes	++;
						if( dYield > 0 )	nVictoryTimes	++;
						if( 1 == nTimes )	dYieldMax	=	dYieldMin	=	dYield;
						if( dYieldMax < dYield )	dYieldMax	=	dYield;
						if( dYieldMin > dYield )	dYieldMin	=	dYield;
					}
				}
				break;
			}
		}
	}
	if( pnTimes )			*pnTimes		=	nTimes;
	if( pnVictoryTimes )	*pnVictoryTimes	=	nVictoryTimes;
	if( pdYieldMax )		*pdYieldMax		=	dYieldMax;
	if( pdYieldMin )		*pdYieldMin		=	dYieldMin;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CAssetSerialContainer 
// �ʲ��������飬��ʱ���������
BOOL CAssetSerialContainer::SortInsert( ASSETSERIAL serial )
{
	BOOL	bAdded	=	FALSE;
	for( int i=GetSize()-1; i>=0; i-- )
	{
		ASSETSERIAL & se	=	ElementAt(i);
		if( se.time == serial.time )
		{
			se.dAsset	=	serial.dAsset;
			se.dCash	=	serial.dCash;
			bAdded		=	TRUE;
			break;
		}
		else if( se.time < serial.time )
		{
			InsertAt( i+1, serial );
			bAdded		=	TRUE;
			break;
		}
	}
	if( !bAdded )
		InsertAt( 0, serial );
	return TRUE;
}

// �ʲ��������飬����ʲ����б�׼��
BOOL CAssetSerialContainer::GetStdDev( double *pdStdDev, double *pdXiapu )
{
	if( GetSize() <= 0 )
		return FALSE;
	double	dInit	=	ElementAt(0).dAsset;
	if( dInit < 1e-4 )
		return FALSE;

	double	dSum = 0., dSquareSum = 0.;
	for( int k = 0; k<GetSize(); k++ )
	{
		double	dNow	=	(ElementAt(k).dAsset - dInit) / dInit;
		dSum	+=	dNow;
		dSquareSum	+=	dNow*dNow;
	}

	double	dAverage	=	dSum / GetSize();
	if( dAverage < 1e-4 )
		return FALSE;
	double	dStdDev	=	dSquareSum / GetSize() - ( dAverage*dAverage );
	if( dStdDev > 0 )
		dStdDev	=	sqrt( dStdDev );
	if( pdStdDev )	*pdStdDev	=	dStdDev;
	if( pdXiapu )	*pdXiapu	=	(dStdDev / dAverage);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTechStock

CTechStock::CTechStock( )
{
	m_bAutoDelete	=	TRUE;

	m_kdata.SetKType(CKData::ktypeDay);
	m_kdataLong.SetKType(CKData::ktypeDay);
}

CTechStock::CTechStock( const CTechStock & src )
{
	*this	=	src;
}

CTechStock & CTechStock::operator = ( const CTechStock &src )
{
	m_bAutoDelete	=	FALSE;

	m_info		=	src.m_info;
	m_kdata		=	src.m_kdata;
	m_kdataLong	=	src.m_kdataLong;
	m_techs.Copy( src.m_techs );
	m_techsLong.Copy( src.m_techsLong );
	return *this;
}

void CTechStock::SetAutoDelete( BOOL bAutoDelete )
{
	m_bAutoDelete	=	bAutoDelete;
}

CTechStock::~CTechStock( )
{
	Clear( );
}

// ����ڴ�
void CTechStock::Clear( )
{
	if( m_bAutoDelete )
	{
		int i=0;
		for(  i=0; i<m_techs.GetSize(); i++ )
		{
			CTechnique * pTech	=	(CTechnique *)m_techs.ElementAt(i);
			if( pTech )
				delete	pTech;
		}
		for(  i=0; i<m_techsLong.GetSize(); i++ )
		{
			CTechnique * pTech	=	(CTechnique *)m_techsLong.ElementAt(i);
			if( pTech )
				delete	pTech;
		}
	}
	m_techs.RemoveAll();
	m_techsLong.RemoveAll();
}

// ָ�����ڣ�ָ��������ָ��ί�м۸񣬵õ���Ʊ�ĳɽ��۸�
BOOL CTechStock::GetPriceOK( long lOpType, CSPTime tmCur, DWORD dwShare, double dSharePrice, double *pdPriceOK )
{
	double	dPriceOK = dSharePrice;

	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = m_kdata.GetIndexByDate( dwDate );
	if( -1 == nIndex )
		return FALSE;

	KDATA	&	kd	=	m_kdata.ElementAt(nIndex);
	if( STRATEGY_OPTYPE_BUY == lOpType )
	{
		if( dSharePrice < kd.m_fLow )
			return FALSE;
		if( dSharePrice > kd.m_fOpen )
			dPriceOK	=	kd.m_fOpen;
		else
			dPriceOK	=	dSharePrice;
	}
	else if( STRATEGY_OPTYPE_SELL == lOpType )
	{
		if( dSharePrice > kd.m_fHigh )
			return FALSE;
		if( dSharePrice < kd.m_fOpen )
			dPriceOK	=	kd.m_fOpen;
		else
			dPriceOK	=	dSharePrice;
	}
	else
		return FALSE;

	if( pdPriceOK )	*pdPriceOK	=	dPriceOK;
	return TRUE;
}

// ָ�������Ƿ�ͣ��
BOOL CTechStock::IsStopTrading( CSPTime tmCur )
{
	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = m_kdata.GetIndexByDate( dwDate );
	if( -1 == nIndex )
	{
		if( -1 != m_kdata.GetAboutIndexByDate( dwDate ) )
			return TRUE;
		return FALSE;
	}

	KDATA	&	kd	=	m_kdata.ElementAt(nIndex);
	float	fLastClose	=	kd.m_fOpen;
	if( nIndex > 0 )
		fLastClose	=	m_kdata.ElementAt(nIndex-1).m_fClose;

	if( fabs(kd.m_fOpen-kd.m_fClose) < 1e-4 && fabs(kd.m_fOpen-kd.m_fHigh) < 1e-4
		&& fabs(kd.m_fOpen-kd.m_fLow) < 1e-4 && fabs(kd.m_fOpen-fLastClose) < 1e-4 )
		return TRUE;
	return FALSE;
}

// �õ���Ʊ�����̼�
BOOL CTechStock::GetClosePrice( CSPTime tmCur, double *pdPrice )
{
	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = m_kdata.GetAboutIndexByDate( dwDate );
	if( -1 == nIndex )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	KDATA	&	kd	=	m_kdata.ElementAt(nIndex);
	if( pdPrice )	*pdPrice	=	kd.m_fClose;
	return TRUE;
}

// �õ���Ʊ�����̼��Ƿ�%
BOOL CTechStock::GetCloseDiffPercent( CSPTime tmCur, double *pdDiffPercent )
{
	if( pdDiffPercent ) *pdDiffPercent = 0;

	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = m_kdata.GetIndexByDate( dwDate );
	if( -1 == nIndex || nIndex <= 0 )
	{
		return FALSE;
	}

	KDATA	&	kd		=	m_kdata.ElementAt(nIndex);
	KDATA	&	kdLast	=	m_kdata.ElementAt(nIndex-1);
	if( pdDiffPercent && kdLast.m_fClose > 1e-4 )
	{
		*pdDiffPercent	=	100. * (kd.m_fClose-kdLast.m_fClose)/kdLast.m_fClose;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTechStockContainer

CTechStockContainer::~CTechStockContainer( )
{
	Clear( );
}

void CTechStockContainer::Clear( )
{
	m_infoIndex.Clear();
	m_kdataIndex.Clear();
	m_kdataIndex.SetKType(CKData::ktypeDay);

	int i=0;
	for(  i=0; i<m_techsIndex.GetSize(); i++ )
	{
		CTechnique * pTech	=	(CTechnique *)m_techsIndex.ElementAt(i);
		if( pTech )
			delete	pTech;
	}
	m_techsIndex.RemoveAll();

	for(  i=0; i<GetSize(); i++ )
	{
		CTechStock	&	stock	=	ElementAt(i);
		stock.Clear( );
	}
	RemoveAll();
}

// ����CTechStock������tmCur���ʽ�dCash�����׷���rate����������opparam��
// ����ӵ�й�Ʊ����nOwnedStockCount��������Ҫ����Ĺ�Ʊ������ί�м۸�ί��ʱ�䣬
// �������Ҫ���룬����FALSE
BOOL CTechStockContainer::GetShouldBuyShare( CTechStock & techstock, CSPTime tmCur, double dCash, CRateParam &rate, COpParam &opparam, int nOwnedStockCount,
									CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice )
{
	CSPTime	tmOp = tmCur;
	DWORD	dwShare = 0;
	double	dSharePrice = 0;

	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = techstock.m_kdata.GetIndexByDate( dwDate );	// �õ�nIndex��ָ��techstock.m_kdata����ĵ�ǰ����λ��
	if( -1 == nIndex )
		return FALSE;
	double	dPriceNow	=	techstock.m_kdata.ElementAt(nIndex).m_fClose;	// ��ǰ��

	// �����ж�
	BOOL	bBuy	=	TRUE;
	if( COpParam::logicOr == opparam.m_nBuyLogic )	// ȫ������������һ����
		bBuy	=	FALSE;
	for( int i=0; i<techstock.m_techs.GetSize(); i++ )	// ÿһ���趨ָ�꣬�ֱ��ж�
	{
		CTechnique * pTech	=	(CTechnique *)techstock.m_techs.ElementAt(i);	// ����ָ��
		SP_ASSERT( NULL != pTech );
		if( NULL == pTech )
			continue;

		int nIntensity	=	pTech->GetSignal( nIndex );		// ��ǰ�����ź�
		if( COpParam::logicOr == opparam.m_nBuyLogic )
		{
			if( nIntensity >= opparam.m_nBuyLimit )			// �����ź��Ƿ�ﵽ��������
				bBuy	=	TRUE;
		}
		else
		{
			if( nIntensity < opparam.m_nBuyLimit )
				bBuy	=	FALSE;
		}
	}

	// Long and Index Trend
	if( bBuy )
	{
		// �������ָ���ж�Ϊ���룬�ж��Ƿ����㳤�������ƺ�ָ������
		if( opparam.m_bLongTrend )	// ����������
		{
			int	nIndexLong	=	techstock.m_kdataLong.GetAboutIndexByDate( dwDate );
			if( opparam.m_nKTypeLong > opparam.m_nKType )
				nIndexLong	--;
			if( nIndexLong >= 0 )
			{
				for( int i=0; i<techstock.m_techsLong.GetSize(); i++ )
				{
					CTechnique * pTech	=	(CTechnique *)techstock.m_techsLong.ElementAt(i);
					SP_ASSERT( NULL != pTech );
					if( NULL == pTech )
						continue;
					pTech->ClearLastIntensity();
					int nIntensity	=	pTech->GetIntensity( nIndexLong );	// �õ���ǰ����
					if( !ITS_ISBUY(nIntensity) )
						bBuy	=	FALSE;
				}
			}
		}
		if( opparam.m_bIndexTrend )	// ָ������
		{
			int	nIndexIndex	=	m_kdataIndex.GetAboutIndexByDate( dwDate );
			if( opparam.m_nKTypeIndex > opparam.m_nKType )
				nIndexIndex	--;
			if( nIndexIndex >= 0 )
			{
				for( int i=0; i<m_techsIndex.GetSize(); i++ )
				{
					CTechnique * pTech	=	(CTechnique *)m_techsIndex.ElementAt(i);
					SP_ASSERT( NULL != pTech );
					if( NULL == pTech )
						continue;
					pTech->ClearLastIntensity();
					int nIntensity	=	pTech->GetIntensity( nIndexIndex );
					if( !ITS_ISBUY(nIntensity) )
						bBuy	=	FALSE;
				}
			}
		}
	}

	if( bBuy )	// ȷ��������
	{
		if( !opparam.GetNextTradeTime(tmCur, tmOp) )	// �������ڣ���һ��������
			return FALSE;

		if( opparam.m_nStoreDiv-nOwnedStockCount <= 0 )	// ��Ʊ�Ѿ������ˣ������������µ���
			return FALSE;
		double	dUseCash	=	dCash / (opparam.m_nStoreDiv-nOwnedStockCount);	// ��ʹ���ʽ�

		if( dCash < dUseCash )
			dUseCash	=	dCash;	// �ʽ𲻹������ж����ö���

		dSharePrice		=	opparam.m_dBuyMulti * dPriceNow;
		double	dTemp	=	dSharePrice * ( 1 + rate.GetRate( techstock.m_info ) );
		if( fabs(dTemp) < 1e-4 || dUseCash < 1e-4 )
			return FALSE;

		dwShare		=	(DWORD)( dUseCash / dTemp );	// �������
		dwShare		=	( dwShare / 100 ) * 100;		// ȡ��
		if( 0 == dwShare )
			return FALSE;

		if( ptmOp )			*ptmOp		=	tmOp;
		if( pdwShare )		*pdwShare	=	dwShare;
		if( pdSharePrice )	*pdSharePrice	=	dSharePrice;
		return TRUE;
	}
	
	return FALSE;
}

// ����CTechStock������tmCur���ʽ�dCash��ӵ�й�Ʊown(��������ʱ�۸�)����������opparam��
// ����ӵ�й�Ʊ����nOwnedStockCount��������Ҫ�����Ĺ�Ʊ������ί�м۸�ί��ʱ�䣬
// �������Ҫ����������FALSE
BOOL CTechStockContainer::GetShouldSellShare( CTechStock & techstock, CSPTime tmCur, STOCKOWN & own, COpParam &opparam,
									CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice )
{
	CSPTime	tmOp = tmCur;
	DWORD	dwShare = 0;
	double	dSharePrice = 0;

	CSPTime	sptimeCur(tmCur.GetTime());
	DWORD	dwDate	=	sptimeCur.ToStockTimeDay();
	int nIndex = techstock.m_kdata.GetIndexByDate( dwDate );	// �õ�nIndex��ָ��techstock.m_kdata����ĵ�ǰ����λ��
	if( -1 == nIndex )
		return FALSE;
	double	dPriceNow	=	techstock.m_kdata.ElementAt(nIndex).m_fClose;	// ��ǰ��

	// �����ж�
	BOOL	bSell	=	FALSE;
	if( COpParam::logicAnd == opparam.m_nSellLogic )	// ȫ������������һ����
		bSell	=	TRUE;
	for( int i=0; i<techstock.m_techs.GetSize(); i++ )	// ÿһ������ָ�꣬�ֱ��ж�
	{
		CTechnique * pTech	=	(CTechnique *)techstock.m_techs.ElementAt(i);
		SP_ASSERT( NULL != pTech );
		if( NULL == pTech )
			continue;

		int nIntensity	=	pTech->GetSignal( nIndex );		// ��ǰ�����ź�
		if( COpParam::logicAnd == opparam.m_nSellLogic )
		{
			if( nIntensity > opparam.m_nSellLimit )			// �����ź��Ƿ�ﵽ��������
				bSell	=	FALSE;
		}
		else
		{
			if( nIntensity <= opparam.m_nSellLimit )
				bSell	=	TRUE;
		}
	}

	// StopLosing and StopProfit ֹ���ֹӮ
	if( opparam.m_bStopLosing && dPriceNow < own.dBuyPrice * (1-opparam.m_dStopLosing) )	// ֹ��
		bSell	=	TRUE;
	if( opparam.m_bStopProfit && dPriceNow > own.dBuyPrice * (1+opparam.m_dStopProfit) )	// ֹӮ
		bSell	=	TRUE;

	if( bSell )	// ȷ��������
	{
		if( !opparam.GetNextTradeTime(tmCur, tmOp) )	// �������ڣ���һ��������
			return FALSE;
		dwShare			=	own.dwShare;
		dSharePrice		=	opparam.m_dSellMulti * dPriceNow;

		if( ptmOp )			*ptmOp		=	tmOp;
		if( pdwShare )		*pdwShare	=	dwShare;
		if( pdSharePrice )	*pdSharePrice	=	dSharePrice;
		return TRUE;
	}
	
	return FALSE;
}

// �õ�ĳֻ��ƱszCode������tmCur�����̼�
BOOL CTechStockContainer::GetClosePrice( const char * szCode, CSPTime tmCur, double * pdPrice )
{
	if( NULL == szCode || strlen(szCode) <= 0 || GetSize() <= 0 )
		return FALSE;

	for( int j=0; j<GetSize(); j++ )
	{
		CTechStock & techstock	=	ElementAt(j);
		if( techstock.m_info.IsEqualTo( CStock::marketUnknown, szCode ) )
		{
			return techstock.GetClosePrice( tmCur, pdPrice );
		}
	}

	return FALSE;
}

// �õ�ĳ�յ����ʲ�
BOOL CTechStockContainer::GetSumAsset( CSPTime tmCur, CStockOwnContainer &container, double * pdAsset )
{
	if( GetSize() <= 0 )
		return FALSE;

	double	dAsset	=	0;
	for( int i=0; i<container.GetSize(); i++ )
	{
		STOCKOWN &	own	=	container.ElementAt(i);
		double	dPrice	=	0;
		if( !GetClosePrice( own.szCode, tmCur, &dPrice ) )
			return FALSE;
		dAsset	+=	dPrice * own.dwShare;
	}
	
	if( pdAsset )	*pdAsset	=	dAsset;
	return TRUE;
}

// �õ���һ���гɽ����Ľ�����
BOOL CTechStockContainer::GetNextExistTradeTime( CSPTime tmCur, CSPTime & tmNext )
{
	CSPTime	sptime(tmCur.GetTime());
	DWORD	dateCur		=	sptime.ToStockTimeDay( );
	DWORD	dateNext	=	-1;
	for( int i=0; i<GetSize(); i++ )
	{
		CTechStock	&	techstock	=	ElementAt(i);
		int nIndex	=	techstock.m_kdata.GetAboutIndexByDate( dateCur );
		if( -1 != nIndex && nIndex+1 < techstock.m_kdata.GetSize() )
		{
			if( -1 == dateNext )
				dateNext	=	techstock.m_kdata.ElementAt(nIndex+1).m_date;
			else if( dateNext > techstock.m_kdata.ElementAt(nIndex+1).m_date )
				dateNext	=	techstock.m_kdata.ElementAt(nIndex+1).m_date;
		}
	}
	if( -1 != dateNext && sptime.FromStockTimeDay( dateNext ) )
	{
		tmNext	=	sptime.GetTime();
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CStrategy

// ���������ļ����õ���������
CSPString CStrategy::GetName( LPCTSTR lpszPath )
{
	if( NULL == lpszPath || strlen(lpszPath) <= 0 )
		return "";

	CSPString	strName;
	CStrategy		doc;
	CSPFile	file;
	if( file.Open( lpszPath, CSPFile::modeRead) )
	{
		CSPArchive ar(&file, CSPArchive::load | CSPArchive::bNoFlushOnDelete);
		if( file.GetLength() > 0 )
			doc.Serialize( ar, NULL, 0 );

		ar.Close();
		file.Close();
		strName	=	doc.m_strName;
	}

	if( strName.IsEmpty() )
		strName	=	lpszPath;
	return strName;
}

/////////////////////////////////////////////////////////////////////////////
// CStrategy construction/destruction

char szCELFileMagic[]	=	"BALANG CEL FILE.";
DWORD	dwCELFileVersion		=	0x20000000;

CStrategy::CStrategy()
{
	SimuReset( );
	RealReset( );
}

CStrategy::~CStrategy()
{
	ClearCache( );
}

// �򿪲����ļ�
BOOL CStrategy::OpenStrategyFile( LPCTSTR lpszPathName, LPSTR lpErr, UINT nMaxSize )
{
	CSPFile	file;
	if( NULL != lpszPathName && file.Open( lpszPathName, CSPFile::modeRead) )
	{
		if( 0 == file.GetLength() )
		{
			SetPathName(lpszPathName);
			return TRUE;
		}

		CSPArchive ar(&file, CSPArchive::load | CSPArchive::bNoFlushOnDelete);
		BOOL bSuccess = Serialize( ar, lpErr, nMaxSize );
		ar.Close();
		file.Close();
		if( bSuccess )
			SetPathName(lpszPathName);
		return bSuccess;
	}
	
	SetPathName( NULL );
	return FALSE;
}

BOOL CStrategy::SaveStrategyFile( LPCTSTR lpszPathName )
{
	CSPString newName;
	if( lpszPathName )
		newName	=	lpszPathName;
	if (newName.IsEmpty())
		newName = m_strPathName;

	CSPFile	file;
	if( file.Open( newName, CSPFile::modeCreate | CSPFile::modeWrite) )
	{
		CSPArchive ar(&file, CSPArchive::store);
		Serialize( ar, NULL, 0 );
		ar.Close();
		file.Close();
		SetPathName(newName);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CStrategy serialization

// �ļ�����Ͷ�ȡ
BOOL CStrategy::Serialize(CSPArchive& ar, LPSTR lpErr, UINT nMaxSize )
{
	if( lpErr && nMaxSize>0 )
		memset(lpErr,0,nMaxSize);

	if (ar.IsStoring())
	{
		ar.Write( szCELFileMagic, sizeof(szCELFileMagic) );
		
		ar << dwCELFileVersion;
		ar << m_strName;
		ar << m_strDescript;
		m_stocks.Serialize( ar );

		m_rate.Serialize( ar );

		m_paramBuy.Serialize( ar );
		m_paramSell.Serialize( ar );
		m_paramLongTrend.Serialize( ar );
		m_paramIndexTrend.Serialize( ar );

		m_anTechsBuy.Serialize( ar );
		m_anTechsSell.Serialize( ar );
		m_anTechsLongTrend.Serialize( ar );
		m_anTechsIndexTrend.Serialize( ar );

		m_opparam.Serialize( ar );

		// Simulation
		ar << m_SimuCurrentStatus;
		ar << m_SimuCurrentTime;
		ar << m_SimuCurrentCash;
		m_SimuStockOwn.Serialize( ar );
		m_SimuOpRecord.Serialize( ar );
		m_SimuNextOp.Serialize( ar );
		m_SimuAssetSerial.Serialize( ar );

		// Real Operation
		ar << m_RealBeginTime;
		ar << m_RealCurrentTime;
		ar << m_RealCurrentCash;
		m_RealStockOwn.Serialize( ar );
		m_RealOpRecord.Serialize( ar );
		m_RealNextOp.Serialize( ar );
		m_RealAssetSerial.Serialize( ar );
	}
	else
	{
		char	buffer[sizeof(szCELFileMagic)+1];
		if( sizeof(szCELFileMagic) != ar.Read( buffer, sizeof(szCELFileMagic) )
			|| 0 != strncmp( buffer, szCELFileMagic, sizeof(szCELFileMagic) ) )
		{
			if( lpErr )
				strncpy(lpErr,strategy_errfile,min(nMaxSize-1,strlen(strategy_errfile)) );
			return FALSE;
		}

		ar >> m_dwFileVersion;
		if( m_dwFileVersion > dwCELFileVersion )
		{
			if( lpErr )
				strncpy(lpErr,strategy_errfilever,min(nMaxSize-1,strlen(strategy_errfilever)) );
			return FALSE;
		}

		ar >> m_strName;
		ar >> m_strDescript;
		m_stocks.Serialize( ar );

		m_rate.Serialize( ar );

		m_paramBuy.Serialize( ar );
		m_paramSell.Serialize( ar );
		m_paramLongTrend.Serialize( ar );
		m_paramIndexTrend.Serialize( ar );

		m_anTechsBuy.Serialize( ar );
		m_anTechsSell.Serialize( ar );
		m_anTechsLongTrend.Serialize( ar );
		m_anTechsIndexTrend.Serialize( ar );

		m_opparam.Serialize( ar );

		// Simulation
		ar >> m_SimuCurrentStatus;
		ar >> m_SimuCurrentTime;
		ar >> m_SimuCurrentCash;
		m_SimuStockOwn.Serialize( ar );
		m_SimuOpRecord.Serialize( ar );
		m_SimuNextOp.Serialize( ar );
		m_SimuAssetSerial.Serialize( ar );

		// Real Operation
		ar >> m_RealBeginTime;
		ar >> m_RealCurrentTime;
		ar >> m_RealCurrentCash;
		m_RealStockOwn.Serialize( ar );
		m_RealOpRecord.Serialize( ar );
		m_RealNextOp.Serialize( ar );
		m_RealAssetSerial.Serialize( ar );
	}
	return TRUE;
}

BOOL CStrategy::DoFileSave( )
{
	return SaveStrategyFile( GetPathName() );
}

BOOL CStrategy::OnClose( )
{
	ClearCache( );
	return TRUE;
}

void CStrategy::OnRealOpViewed( )
{
	for( int i=0; i<m_RealNextOp.GetSize(); i++ )
		m_RealNextOp.ElementAt(i).bViewed	=	TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CStrategy commands
void CStrategy::SetPathName( LPCTSTR lpszPathName )
{
	m_strPathName	=	lpszPathName;
}

CSPString CStrategy::GetPathName( )
{
	return m_strPathName;
}

// �趨��������
void CStrategy::SetName( LPCTSTR lpszName )
{
	m_strName	=	lpszName;
}
// �õ���������
CSPString	CStrategy::GetName( )
{
	if( m_strName.IsEmpty() )
		return GetPathName();
	return m_strName;
}
// �趨��������
void CStrategy::SetDescript( LPCTSTR lpszDescript )
{
		m_strDescript	=	lpszDescript;
}
// �õ���������
CSPString	CStrategy::GetDescript( )
{
	return m_strDescript;
}
// �õ����Ա�ѡ��Ʊ
CSPStringArray & CStrategy::GetStocks( )
{
	return m_stocks;
}
// �趨���Ա�ѡ��Ʊ
void CStrategy::SetStocks( CSPStringArray & astr )
{
	m_stocks.Copy( astr );
}
// ������Ա�ѡ��Ʊ
void CStrategy::AddStock( LPCTSTR lpszCode )
{
	m_stocks.AddStock( lpszCode );
}
// �Ƴ����Ա�ѡ��Ʊ
void CStrategy::RemoveStock( LPCTSTR lpszCode )
{
	m_stocks.RemoveStock( lpszCode );
}
// �õ����Ե�һ������������
CSPString CStrategy::GetStockTechString( )
{
	int	nStockShowCount = 3, nTechShowCount = 10;

	CSPString	strStock;
	int i=0;
	for(  i=0; i<m_stocks.GetSize() && i<nStockShowCount; i++ )
	{
		if( strStock.GetLength() > 0 )
			strStock	+=	",";
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( m_stocks.ElementAt(i), &info ) )
//#ifdef CLKLAN_ENGLISH_US
//			strStock	+=	info.GetStockCode();
//#else
			strStock	+=	info.GetStockName();
//#endif
		else
			strStock	+=	m_stocks.ElementAt(i);
	}
	if( strStock.GetLength() == 0 )
		strStock	=	strategy_noselectedstock;
	else if( m_stocks.GetSize() > nStockShowCount )
		strStock	+=	"...";
	strStock	+=	";    ";

	CSPString	strTech;
	for(  i=0; i<m_anTechsBuy.GetSize() && i<nTechShowCount; i++ )
	{
		if( strTech.GetLength() > 0 )
			strTech	+=	",";
		strTech	+=	AfxGetSTTShortName(m_anTechsBuy[i]);
	}
	if( strTech.GetLength() == 0 )
		strTech	=	strategy_noselectedtech;
	else if( m_anTechsBuy.GetSize() > nTechShowCount )
		strTech	+=	"...";
	
	return strStock + strTech;
}
// ���׷��ʡ�����������
CRateParam &	CStrategy::GetRateParam( )			{	return m_rate;			}
COpParam &		CStrategy::GetOpParam( )				{	return m_opparam;		}
CTechParameters & CStrategy::GetTechParametersBuy( )	{	return m_paramBuy;		}
CTechParameters & CStrategy::GetTechParametersSell( )	{	return m_paramSell;		}
CTechParameters & CStrategy::GetTechParametersLongTrend( )	{	return m_paramLongTrend;		}
CTechParameters & CStrategy::GetTechParametersIndexTrend( )	{	return m_paramIndexTrend;		}
CSPDWordArray &	CStrategy::GetSelectedTechsBuy( )		{	return m_anTechsBuy;	}
CSPDWordArray &	CStrategy::GetSelectedTechsSell( )		{	return m_anTechsSell;	}
CSPDWordArray &	CStrategy::GetSelectedTechsLongTrend( )	{	return m_anTechsLongTrend;	}
CSPDWordArray &	CStrategy::GetSelectedTechsIndexTrend( )	{	return m_anTechsIndexTrend;	}

/////////////////////////////////////////////////////////////////////////
// Cache
// ׼�����ݣ���ȡK�����ݣ�������K�����ݣ����ɼ���ָ�����
BOOL CStrategy::PrepareData( SIMULATION_CALLBACK fnCallback, void * cookie )
{
	if( m_techstocks.GetSize() == m_stocks.GetSize() )
		return TRUE;

	ClearCache( );

	// m_techstocks.m_kdataIndex m_techstocks.m_infoIndex
	if( m_opparam.m_bIndexTrend )	// ָ��K������
	{
		CStockInfo	infoIndex;
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &infoIndex ) )
		{
			m_techstocks.m_infoIndex	=	infoIndex;
			
			CStock	stockIndex;
			stockIndex.SetStockInfo( &infoIndex );
			stockIndex.SetDatabase( &AfxGetDB() );
			stockIndex.PrepareData( CStock::dataK, m_opparam.m_nKTypeIndex );
			CKData	&	kdataIndex		=	stockIndex.GetKData(m_opparam.m_nKTypeIndex);
			kdataIndex.SetMaindataType( m_opparam.m_nMaindataType );
			m_techstocks.m_kdataIndex	=	kdataIndex;

			// m_techstocks.m_techsIndex
			for( int j=0; j<m_anTechsIndexTrend.GetSize(); j++ )
			{
				CTechnique * pTech	=	CTechnique::CreateTechnique( m_anTechsIndexTrend[j], &(m_techstocks.m_kdataIndex) );
				m_paramIndexTrend.FindParameters( m_anTechsIndexTrend[j], pTech );
				m_techstocks.m_techsIndex.Add( pTech );
			}
		}
	}
	
	m_techstocks.SetSize( m_stocks.GetSize() );
	int	nStockCount	=	0;
	for( int i=0; i<m_stocks.GetSize(); i++ )	// ��ȡÿһֻ��ѡ��Ʊ������
	{
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( m_stocks.ElementAt(i), &info ) )
		{
			// CTechStock::m_info;
			CTechStock	temp;
			temp.m_info	=	info;
			
			m_techstocks.SetAt( nStockCount, temp );
			CTechStock & techstock	=	m_techstocks.ElementAt(nStockCount);
			nStockCount	++;
			
			// CTechStock::m_kdata
			CStock	stock;
			stock.SetStockInfo( &info );
			AfxPrepareStockData( &AfxGetDB(), stock, m_opparam.m_nKType, CKData::formatXDRdown, m_opparam.m_nMaindataType, FALSE, FALSE );
			techstock.m_kdata	=	stock.GetKData(m_opparam.m_nKType);

			// CTechStock::m_techs
			techstock.SetAutoDelete( TRUE );
			for( int j=0; j<m_anTechsBuy.GetSize(); j++ )
			{
				CTechnique * pTech	=	CTechnique::CreateTechnique( m_anTechsBuy[j], &(techstock.m_kdata) );
				m_paramBuy.FindParameters( m_anTechsBuy[j], pTech );
				techstock.m_techs.Add( pTech );
			}

			// CTechStock::m_kdataLong
			if( m_opparam.m_bLongTrend )
			{
				AfxPrepareStockData( &AfxGetDB(), stock, m_opparam.m_nKTypeLong, CKData::formatXDRdown, m_opparam.m_nMaindataType, FALSE, FALSE );
				techstock.m_kdataLong	=	stock.GetKData( m_opparam.m_nKTypeLong );

				// CTechStock::m_techsLong
				for( int j=0; j<m_anTechsLongTrend.GetSize(); j++ )
				{
					CTechnique * pTech	=	CTechnique::CreateTechnique( m_anTechsLongTrend[j], &(techstock.m_kdataLong) );
					m_paramLongTrend.FindParameters( m_anTechsLongTrend[j], pTech );
					techstock.m_techsLong.Add( pTech );
				}
			}
		}

		DWORD	dwProgress	=	(DWORD)((i+1)*STRATEGY_MAXF_PROGRESS / m_stocks.GetSize());
		if( fnCallback && !fnCallback( SIMULATION_PROGRESS, dwProgress, NULL, cookie ) )
		{
			ClearCache( );
			nStockCount	=	0;
			break;
		}
	}

	m_techstocks.SetSize( nStockCount );

	if( fnCallback )
		fnCallback( SIMULATION_PROGRESS, STRATEGY_MAX_PROGRESS, NULL, cookie );

	return ( m_techstocks.GetSize() == m_stocks.GetSize() );
}

// ���ÿ��ָ�걣����ϴ�����ֵ
void CStrategy::ClearLastIntensity( )
{
	for( int i=0; i<m_techstocks.GetSize(); i++ )
	{
		CTechStock & techstock	=	m_techstocks.ElementAt(i);
			
		for( int nTech=0; nTech<techstock.m_techs.GetSize(); nTech++ )
		{
			CTechnique * pTech	=	(CTechnique *)techstock.m_techs.ElementAt(nTech);
			if( pTech )
				pTech->ClearLastIntensity();
		}
	}
}

void CStrategy::ClearCache( )
{
	m_techstocks.Clear();
}

CTechStockContainer & CStrategy::GetTechStockContainer( )
{
	return m_techstocks;
}

////////////////////////////////////////////////////////////////////////
// Simulation
// ����ģ�⣺�����趨
void CStrategy::SimuReset( )
{
	SimuSetStatusInit( );
	m_SimuCurrentTime	=	m_opparam.GetBeginTime( );
	m_SimuCurrentCash	=	m_opparam.m_nStartAmount;
	m_SimuOpRecord.RemoveAll();
	m_SimuNextOp.RemoveAll();
	m_SimuStockOwn.RemoveAll();
	m_SimuAssetSerial.RemoveAll();

	SP_ASSERT( m_opparam.IsValid() );
}
// ����ģ�⣺������һ��������
BOOL CStrategy::SimuGotoNextTime( )
{
	CSPTime	tmNext;
	if( m_opparam.GetNextTradeTime( m_SimuCurrentTime, tmNext )
		&& m_opparam.IsInTimeZones( tmNext ) )
	{
		m_SimuCurrentTime	=	tmNext;
		return TRUE;
	}
	return FALSE;
}
// ����ģ�⣺ģ��ĵ�ǰʱ��
CSPTime CStrategy::SimuGetCurrentTime( )
{
	return m_SimuCurrentTime;
}
// ����ģ�⣺ģ��ĵ�ǰ�ʽ�
double CStrategy::SimuGetCurrentCash( )
{
	return m_SimuCurrentCash;
}
// ����ģ�⣺����ִ�У�bTimeStrict��ʾ�Ƿ��ϸ����ؼƻ�ʱ��
BOOL CStrategy::SimuOperate( OPRECORD record, BOOL bTimeStrict )
{
	if( ! m_opparam.IsInTimeZones( record.time ) )
		return FALSE;
	if( bTimeStrict && m_SimuOpRecord.GetSize() > 0 && record.time < m_SimuOpRecord.ElementAt(m_SimuOpRecord.GetSize()-1).time )
		return FALSE;
	CStockInfo	info;
	if( strlen(record.szCode)>0
		&& ( !AfxGetStockContainer().GetStockInfo( record.szCode, &info )
		|| !info.IsValidStock() ) )
		return FALSE;

	double	dAmount		=	record.dwShare * record.dSharePrice;
	double	dRateCost	=	record.dRateCost;
	if( STRATEGY_OPTYPE_BUY == record.lOpType )			// ����
	{
		if( m_SimuCurrentCash < dAmount+dRateCost )
			return FALSE;
		if( !m_SimuStockOwn.AddStock( info, record.dwShare, record.dSharePrice ) )
			return FALSE;
		m_SimuCurrentCash	-=	(dAmount+dRateCost);
	}
	else if( STRATEGY_OPTYPE_SELL == record.lOpType )	// ����
	{
		if( !m_SimuStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
		m_SimuCurrentCash	+=	(dAmount-dRateCost);
	}
	else if( STRATEGY_OPTYPE_ADDSTOCK == record.lOpType )// ��ӹ�Ʊ
	{
		if( !m_SimuStockOwn.AddStock( info, record.dwShare, record.dSharePrice ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_REMOVESTOCK == record.lOpType )// �Ƴ���Ʊ
	{
		if( !m_SimuStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_ADDCASH == record.lOpType )	// ����ʽ�
	{
		m_SimuCurrentCash	+=	record.dSharePrice;
	}
	else if( STRATEGY_OPTYPE_REMOVECASH == record.lOpType ) // �Ƴ��ʽ�
	{
		if( m_SimuCurrentCash < record.dSharePrice )
			return FALSE;
		m_SimuCurrentCash	-=	record.dSharePrice;
	}
	else
		return FALSE;

	// Record
	m_SimuOpRecord.Add( record );
	return TRUE;
}
// ����ģ�⣺������¼
COpRecordContainer & CStrategy::SimuGetOpRecord( )
{
	return m_SimuOpRecord;
}
// ����ģ�⣺��һ������
COpRecordContainer & CStrategy::SimuGetNextOp( )
{
	return m_SimuNextOp;
}
// ����ģ�⣺��ǰӵ�й�Ʊ
CStockOwnContainer & CStrategy::SimuGetStockOwn( )
{
	return m_SimuStockOwn;
}
// ����ģ�⣺���ʲ�����
CAssetSerialContainer & CStrategy::SimuGetAssetSerial( )
{
	return m_SimuAssetSerial;
}
// ����ģ�⣺��ǰ����
DWORD CStrategy::SimuGetCurrentProgress( DWORD dwProgressMax )
{
	return m_opparam.GetProgress( m_SimuCurrentTime, dwProgressMax );
}
// ����ģ�⣺����ʱ������ʲ�
double CStrategy::SimuGetAsset( CSPTime tmCur )
{
	double	dAsset	=	0;
	if( m_techstocks.GetSumAsset( tmCur, m_SimuStockOwn, &dAsset ) )	// �������ʲ�
	{
		dAsset	+=	m_SimuCurrentCash;

		ASSETSERIAL	serial;
		memset( &serial, 0, sizeof(serial) );
		serial.time		=	tmCur.GetTime();
		serial.dAsset	=	dAsset;
		serial.dCash	=	m_SimuCurrentCash;
		m_SimuAssetSerial.SortInsert( serial );
		return dAsset;
	}
	else
	{
		for( int i=m_SimuAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL & serial	=	m_SimuAssetSerial.ElementAt(i);
			if( serial.time <= tmCur.GetTime() )
				return serial.dAsset;
		}
	}
	return m_opparam.m_nStartAmount;	// ��ʼ���ʲ�
}
// ����ģ�⣺��ǰ����
double CStrategy::SimuGetCurrentYield( )
{
	if( m_opparam.m_nStartAmount > 0 )
		return STRATEGY_BASEF_YIELD * SimuGetAsset(m_SimuCurrentTime) / m_opparam.m_nStartAmount;
	return STRATEGY_BASEF_YIELD;
}
// ����ģ�⣺��ǰָ�����Ƕ���
double CStrategy::SimuGetCurrentYieldIndexPercent( )
{
	CSPTime	sptmBegin( m_opparam.GetBeginTime().GetTime() );
	CSPTime	sptmNow( m_SimuCurrentTime.GetTime() );
	DWORD	dateBegin	=	sptmBegin.ToStockTimeDay();
	DWORD	dateNow		=	sptmNow.ToStockTimeDay();
	
	CKData & kdata	=	AfxGetStockMain().GetKData(m_opparam.m_nKType);
	int	nIndexBegin	=	kdata.GetAboutIndexByDate( dateBegin );
	int	nIndexNow	=	kdata.GetAboutIndexByDate( dateNow );
	if( -1 == nIndexBegin || -1 == nIndexNow )
		return 0;

	if( kdata.ElementAt(nIndexBegin).m_fClose < 1e-4 )
		return 0;

	double	dYield	=	((double)kdata.ElementAt(nIndexNow).m_fClose) - kdata.ElementAt(nIndexBegin).m_fClose;
	dYield	=	dYield / kdata.ElementAt(nIndexBegin).m_fClose;

	return dYield;
}
// ����ģ�⣺��ǰ����ٷ���
double CStrategy::SimuGetCurrentYieldPercent( )
{
	return ( SimuGetCurrentYield() - STRATEGY_BASEF_YIELD ) / STRATEGY_BASEF_YIELD;
}
// ����ģ�⣺ִ����һ�������ƻ�
BOOL CStrategy::SimuOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock )
{
	for( int j=0; j<nextop.GetSize(); j++ )	// ÿ���ƻ�����ִ��
	{
		OPRECORD &	record	=	nextop.ElementAt(j);
		if( STRATEGY_OPTYPE_BUY != record.lOpType && STRATEGY_OPTYPE_SELL != record.lOpType )
			continue;

		if( tmCur.GetTime() >= record.time
			&& techstock.m_info.IsEqualTo( CStock::marketUnknown, record.szCode ) )
		{
			if( techstock.IsStopTrading(tmCur) )	// ͣ����
			{
				CSPTime	tmNext;
				if( m_opparam.GetNextTradeTime(tmCur, tmNext) )
					record.time	=	tmNext.GetTime();	// �´���ִ��
				continue;
			}

			double	dPriceOK	=	record.dSharePrice;
			if( techstock.GetPriceOK( record.lOpType, tmCur, record.dwShare, record.dSharePrice, &dPriceOK ) )	// �ɽ���
			{
				record.time			=	tmCur.GetTime();
				record.dSharePrice	=	dPriceOK;
				record.dRateCost	=	record.dwShare * record.dSharePrice * m_rate.GetRate(techstock.m_info);
				SimuOperate( record );
			}
			else if( STRATEGY_OPTYPE_SELL == record.lOpType )	// �����������û�гɹ�����˳����һ�������գ�ֱ������
			{
				CSPTime	tmNext;
				if( m_opparam.GetNextTradeTime(tmCur, tmNext) )
					record.time	=	tmNext.GetTime();
				if( techstock.GetClosePrice(tmCur,&dPriceOK) )
					record.dSharePrice	=	dPriceOK * m_opparam.m_dSellMulti;
				continue;
			}
			nextop.RemoveAt(j);
			j --;
		}
	}
	return TRUE;
}
// ����ģ�⣺ģ������
BOOL CStrategy::SimuRun( SIMULATION_CALLBACK fnCallback, void * cookie )
{
	// TIMEZONES
	// ׼������
	if( !PrepareData( fnCallback, cookie ) )
		return FALSE;

	ClearLastIntensity( );

	DWORD	dwShare = 0;		// Temp Data
	double	dSharePrice = 0;	// Temp Data
	CSPTime	tmOp;				// Temp Data

	do	{
		CSPTime	tmCur	=	SimuGetCurrentTime();	// ģ�⵱ǰʱ��

		// Process
		for( int i=0; i<m_techstocks.GetSize(); i++ )	// ÿֻ��Ʊ�����ж�
		{
			CTechStock & techstock	=	m_techstocks.ElementAt(i);

			// Operate
			SimuOperateNextop( tmCur, m_SimuNextOp, techstock );	// ִ�н���Ĳ����ƻ�

			// Judge Whether to operate, if yes, save to nextop
			STOCKOWN	own;
			memset( &own, 0, sizeof(own) );
			if( m_SimuStockOwn.HasThisStock( techstock.m_info, &own ) )	// ����Ѿ�����֧��Ʊ���ж��Ƿ�����
			{
				if( m_techstocks.GetShouldSellShare( techstock, tmCur, own, m_opparam, &tmOp, &dwShare, &dSharePrice ) )
				{	// ���Ҫ������������һ�������ƻ�����һ��������ִ��
					m_SimuNextOp.AddRecordUniqueStock( STRATEGY_OPTYPE_SELL, tmOp, techstock.m_info.GetStockCode(), dwShare, dSharePrice, dwShare*dSharePrice*m_rate.GetRate(techstock.m_info) );
				}
			}
			else	// �ж��Ƿ�����
			{
				if( m_techstocks.GetShouldBuyShare( techstock, tmCur, m_SimuCurrentCash, m_rate , m_opparam, m_SimuStockOwn.GetSize(), &tmOp, &dwShare, &dSharePrice )
					&& ( m_SimuStockOwn.GetSize() + m_SimuNextOp.GetBuyRecordCount() < m_opparam.m_nStoreDiv || m_SimuNextOp.HasBuyStock(techstock.m_info.GetStockCode()) ) )
				{	// ���Ҫ���룬������һ�������ƻ�����һ��������ִ��
					m_SimuNextOp.AddRecordUniqueStock( STRATEGY_OPTYPE_BUY, tmOp, techstock.m_info.GetStockCode(), dwShare, dSharePrice, dwShare*dSharePrice*m_rate.GetRate(techstock.m_info) );
				}
			}
		}

		// ������ʾ
		DWORD	dwProgress	=	SimuGetCurrentProgress( STRATEGY_MAX_PROGRESS );
		double	dYield		=	SimuGetCurrentYield( );
		if( fnCallback && !fnCallback( SIMULATION_PROGRESS, dwProgress, NULL, cookie ) )
			return FALSE;
		if( fnCallback && !fnCallback( SIMULATION_YIELD, (DWORD)dYield, NULL, cookie ) )
			return FALSE;

	} while( SimuGotoNextTime() );	// ģ�����һ��������

	if( fnCallback )
		fnCallback( SIMULATION_PROGRESS, STRATEGY_MAX_PROGRESS, NULL, cookie );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Real
// ����ʵս�������趨
void CStrategy::RealReset( )
{
	CSPTime	tmLatest	=	CSPTime::GetCurrentTime();
	AfxGetDB().GetTimeLocalRange( &tmLatest, NULL, NULL );
	m_RealBeginTime		=	tmLatest;
	m_RealCurrentTime	=	tmLatest;
	m_RealCurrentCash	=	m_opparam.m_nStartAmount;
	m_RealOpRecord.RemoveAll();
	m_RealNextOp.RemoveAll();
	m_RealStockOwn.RemoveAll();
	m_RealAssetSerial.RemoveAll();

	SP_ASSERT( m_opparam.IsValid() );
}
// ����ʵս����һ������ʱ��
BOOL CStrategy::RealGotoNextTime( )
{
	CSPTime	tmNext;
	if( m_techstocks.GetNextExistTradeTime( RealGetCurrentTime(), tmNext ) )
	{
		m_RealCurrentTime	=	tmNext;
		return TRUE;
	}
	return FALSE;
}
// ����ʵս����ʼʱ��
CSPTime CStrategy::RealGetBeginTime( )
{
	return m_RealBeginTime;
}
// ����ʵս����ǰʱ��
CSPTime CStrategy::RealGetCurrentTime( )
{
	return m_RealCurrentTime;
}
// ����ʵս����ǰ�ʽ�
double CStrategy::RealGetCurrentCash( )
{
	return m_RealCurrentCash;
}
// ����ʵս��ִ��һ������
BOOL CStrategy::RealOperate( OPRECORD record, BOOL bTimeStrict )
{
	if( bTimeStrict && m_RealOpRecord.GetSize() > 0 && record.time < m_RealOpRecord.ElementAt(m_RealOpRecord.GetSize()-1).time )
		return FALSE;
	CStockInfo	info;
	if( strlen(record.szCode)>0
		&& ( !AfxGetStockContainer().GetStockInfo( record.szCode, &info )
		|| !info.IsValidStock() ) )
		return FALSE;

	double	dAmount		=	record.dwShare * record.dSharePrice;
	double	dRateCost	=	record.dRateCost;
	if( STRATEGY_OPTYPE_BUY == record.lOpType )	// ����
	{
		if( m_RealCurrentCash < dAmount+dRateCost )
			return FALSE;
		if( !m_RealStockOwn.AddStock( info, record.dwShare, record.dSharePrice ) )
			return FALSE;
		m_RealCurrentCash	-=	(dAmount+dRateCost);
	}
	else if( STRATEGY_OPTYPE_SELL == record.lOpType )	// ����
	{
		if( !m_RealStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
		m_RealCurrentCash	+=	(dAmount-dRateCost);
	}
	else if( STRATEGY_OPTYPE_ADDSTOCK == record.lOpType )	// ��ӹ�Ʊ
	{
		if( !m_RealStockOwn.AddStock( info, record.dwShare, record.dSharePrice ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_REMOVESTOCK == record.lOpType )	// �Ƴ���Ʊ
	{
		if( !m_RealStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_ADDCASH == record.lOpType )		// �����ʽ�
	{
		m_RealCurrentCash	+=	record.dSharePrice;
	}
	else if( STRATEGY_OPTYPE_REMOVECASH == record.lOpType )	// �Ƴ��ʽ�
	{
		if( m_RealCurrentCash < record.dSharePrice )
			return FALSE;
		m_RealCurrentCash	-=	record.dSharePrice;
	}
	else
		return FALSE;

	// Record
	m_RealOpRecord.Add( record );
	return TRUE;
}
// ����ʵս��������¼
COpRecordContainer & CStrategy::RealGetOpRecord( )
{
	return m_RealOpRecord;
}
// ����ʵս����һ�������ƻ�
COpRecordContainer & CStrategy::RealGetNextOp( )
{
	return m_RealNextOp;
}
// ����ʵս����ǰӵ�й�Ʊ
CStockOwnContainer & CStrategy::RealGetStockOwn( )
{
	return m_RealStockOwn;
}
// ����ʵս�����ʲ�����
CAssetSerialContainer & CStrategy::RealGetAssetSerial( )
{
	return m_RealAssetSerial;
}
// ����ʵս���õ�ָ�����ڵ����ʲ�
double CStrategy::RealGetAsset( CSPTime tmCur )
{
	double	dAsset	=	0;

	if( m_techstocks.GetSumAsset( tmCur, m_RealStockOwn, &dAsset ) )
	{
		dAsset	+=	m_RealCurrentCash;

		ASSETSERIAL	serial;
		memset( &serial, 0, sizeof(serial) );
		serial.time		=	tmCur.GetTime();
		serial.dAsset	=	dAsset;
		serial.dCash	=	m_RealCurrentCash;
		m_RealAssetSerial.SortInsert( serial );
		return dAsset;
	}
	else
	{
		for( int i=m_RealAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL & serial	=	m_RealAssetSerial.ElementAt(i);
			if( serial.time <= tmCur.GetTime() )
				return serial.dAsset;
		}
	}
	return m_opparam.m_nStartAmount;
}
// ����ʵս����ǰ����
double CStrategy::RealGetCurrentYield( )
{
	if( m_opparam.m_nStartAmount > 0 )
		return STRATEGY_BASEF_YIELD * RealGetAsset(RealGetCurrentTime()) / m_opparam.m_nStartAmount;
	return STRATEGY_BASEF_YIELD;
}
// ����ʵս����ǰָ�����ǰٷֱ�
double CStrategy::RealGetCurrentYieldIndexPercent( )
{
	CSPTime	sptmBegin( RealGetBeginTime().GetTime() );
	CSPTime	sptmNow( RealGetCurrentTime().GetTime() );
	DWORD	dateBegin	=	sptmBegin.ToStockTimeDay();
	DWORD	dateNow		=	sptmNow.ToStockTimeDay();
	
	CKData & kdata	=	AfxGetStockMain().GetKData(m_opparam.m_nKType);
	int	nIndexBegin	=	kdata.GetAboutIndexByDate( dateBegin );
	int	nIndexNow	=	kdata.GetAboutIndexByDate( dateNow );
	if( -1 == nIndexBegin || -1 == nIndexNow )
		return 0;

	if( kdata.ElementAt(nIndexBegin).m_fClose < 1e-4 )
		return 0;

	double	dYield	=	((double)kdata.ElementAt(nIndexNow).m_fClose) - kdata.ElementAt(nIndexBegin).m_fClose;
	dYield	=	dYield / kdata.ElementAt(nIndexBegin).m_fClose;

	return dYield;
}
// ����ʵս����ǰ����ٷֱ�
double CStrategy::RealGetCurrentYieldPercent( )
{
	return ( RealGetCurrentYield() - STRATEGY_BASEF_YIELD ) / STRATEGY_BASEF_YIELD;
}
// ����ʵս��ִ�в����ƻ�
BOOL CStrategy::RealOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock )
{
	for( int j=0; j<nextop.GetSize(); j++ )	// ����ִ��ÿһ���ƻ�
	{
		OPRECORD &	record	=	nextop.ElementAt(j);
		if( STRATEGY_OPTYPE_BUY != record.lOpType && STRATEGY_OPTYPE_SELL != record.lOpType )
			continue;

		if( tmCur.GetTime() >= record.time
			&& techstock.m_info.IsEqualTo( CStock::marketUnknown, record.szCode ) )
		{
			if( !record.bViewed )	//	Not Viewed
			{
				nextop.RemoveAt(j);
				j --;
				continue;
			}

			if( techstock.IsStopTrading(tmCur) )	// ͣ�ƣ�ת����һ�������յļƻ���
			{
				CSPTime	tmNext;
				if( m_opparam.GetNextTradeTime(tmCur, tmNext) )
					record.time	=	tmNext.GetTime();
				continue;
			}

			double	dPriceOK	=	record.dSharePrice;
			if( techstock.GetPriceOK( record.lOpType, tmCur, record.dwShare, record.dSharePrice, &dPriceOK ) )	// �ɽ��۸�
			{
				record.time			=	tmCur.GetTime();
				record.dSharePrice	=	dPriceOK;
				record.dRateCost	=	record.dwShare * record.dSharePrice * m_rate.GetRate(techstock.m_info);
				RealOperate( record );
			}
			else if( STRATEGY_OPTYPE_SELL == record.lOpType )	// ������������ɹ���ת����һ�������գ�ֱ�������ɹ�
			{
				CSPTime	tmNext;
				if( m_opparam.GetNextTradeTime(tmCur, tmNext) )
					record.time	=	tmNext.GetTime();
				if( techstock.GetClosePrice(tmCur,&dPriceOK) )
					record.dSharePrice	=	dPriceOK * m_opparam.m_dSellMulti;
				continue;
			}
			nextop.RemoveAt(j);
			j --;
		}
	}
	return TRUE;
}
// ����ʵս��ִ��
BOOL CStrategy::RealRun( SIMULATION_CALLBACK fnCallback, void * cookie )
{
	// TIMEZONES
	// ׼������
	if( !PrepareData( fnCallback, cookie ) )
		return FALSE;

	ClearLastIntensity( );

	DWORD	dwShare = 0;		// Temp Data
	double	dSharePrice = 0;	// Temp Data
	CSPTime	tmOp;				// Temp Data

	do	{
		CSPTime	tmCur	=	RealGetCurrentTime();	// ��ǰ����

		// Process
		for( int i=0; i<m_techstocks.GetSize(); i++ )	// ÿֻ��Ʊ�����ж�
		{
			CTechStock & techstock	=	m_techstocks.ElementAt(i);

			// Operate
			RealOperateNextop( tmCur, m_RealNextOp, techstock );	// ִ�н��ղ����ƻ�

			// Judge Whether to operate, if yes, save to nextop
			STOCKOWN	own;
			memset( &own, 0, sizeof(own) );
			if( m_RealStockOwn.HasThisStock( techstock.m_info, &own ) )	// �������֧��Ʊ���ж��Ƿ�����
			{
				if( m_techstocks.GetShouldSellShare( techstock, tmCur, own, m_opparam, &tmOp, &dwShare, &dSharePrice ) )
				{	// �����������������ƻ���
					m_RealNextOp.AddRecordUniqueStock( STRATEGY_OPTYPE_SELL, tmOp, techstock.m_info.GetStockCode(), dwShare, dSharePrice, dwShare*dSharePrice*m_rate.GetRate(techstock.m_info) );
				}
			}
			else	// �ж��Ƿ�����
			{
				if( m_techstocks.GetShouldBuyShare( techstock, tmCur, m_RealCurrentCash, m_rate , m_opparam, m_RealStockOwn.GetSize(), &tmOp, &dwShare, &dSharePrice )
					&& ( m_RealStockOwn.GetSize() + m_RealNextOp.GetBuyRecordCount() < m_opparam.m_nStoreDiv || m_RealNextOp.HasBuyStock(techstock.m_info.GetStockCode()) ) )
				{	// ������룬��������ƻ���
					m_RealNextOp.AddRecordUniqueStock( STRATEGY_OPTYPE_BUY, tmOp, techstock.m_info.GetStockCode(), dwShare, dSharePrice, dwShare*dSharePrice*m_rate.GetRate(techstock.m_info) );
				}
			}

			// ������ʾ
			DWORD	dwProgress	=	(DWORD)((i+1)*STRATEGY_MAX_PROGRESS/m_techstocks.GetSize());
			if( fnCallback && !fnCallback( SIMULATION_PROGRESS, dwProgress, NULL, cookie ) )
				return FALSE;
		}

		RealGetAsset( RealGetCurrentTime() );

	} while( RealGotoNextTime() );	// ��һ��������

	if( fnCallback )
		fnCallback( SIMULATION_PROGRESS, STRATEGY_MAX_PROGRESS, NULL, cookie );
	return TRUE;
}
// ����ʵս�����������¼������ɹ���ͬʱҪɾ�������պ�����в���
BOOL CStrategy::RealAddOpRecordStrict( OPRECORD record )
{
	COpRecordContainer recordbk;
	recordbk.Copy( m_RealOpRecord );

	// ����ִ�в�����¼���������²�����Ϊֹ
	BOOL	bOK	=	TRUE;
	m_RealCurrentCash	=	m_opparam.m_nStartAmount;
	m_RealOpRecord.RemoveAll();
	m_RealNextOp.RemoveAll();
	m_RealStockOwn.RemoveAll();
	for( int i=0; i<recordbk.GetSize(); i++ )
	{
		OPRECORD	rec	=	recordbk.ElementAt(i);
		if( bOK && rec.time < record.time )
			bOK	&=	RealOperate( rec );
	}
	
	if( bOK && RealOperate( record ) )
	{	// ����ɹ������¼������ʲ�����
		for( int i=m_RealAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL	serial	=	m_RealAssetSerial.ElementAt(i);
			if( serial.time >= record.time )
				m_RealAssetSerial.RemoveAt(i);
		}
		RealGetAsset( record.time );
		return TRUE;
	}
	else
	{
		// ����ʧ�ܣ�ȫ���������ָ�ԭ��
		m_RealCurrentCash	=	m_opparam.m_nStartAmount;
		m_RealOpRecord.RemoveAll();
		m_RealNextOp.RemoveAll();
		m_RealStockOwn.RemoveAll();
		for( int i=0; i<recordbk.GetSize(); i++ )
		{
			OPRECORD	rec	=	recordbk.ElementAt(i);
			RealOperate( rec );
		}
		return FALSE;
	}
}
// ����ʵս��ɾ��������¼������ɹ���ͬʱҪɾ���պ�����в���
BOOL CStrategy::RealDeleteOpRecordStrict( int nRecord )
{
	if( nRecord < 0 || nRecord >= m_RealOpRecord.GetSize() )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}
	OPRECORD	record	=	m_RealOpRecord.ElementAt(nRecord);

	COpRecordContainer recordbk;
	recordbk.Copy( m_RealOpRecord );

	// ����ִ�в�����¼����ɾ����Ϊֹ
	BOOL	bOK	=	TRUE;
	m_RealCurrentCash	=	m_opparam.m_nStartAmount;
	m_RealOpRecord.RemoveAll();
	m_RealNextOp.RemoveAll();
	m_RealStockOwn.RemoveAll();
	for( int i=0; i<recordbk.GetSize(); i++ )
	{
		OPRECORD	rec	=	recordbk.ElementAt(i);
		if( bOK && rec.time < record.time )
			bOK	&=	RealOperate( rec );
	}
	
	if( bOK )
	{	// �ɹ�ɾ�������¼������ʲ�����
		for( int i=m_RealAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL	serial	=	m_RealAssetSerial.ElementAt(i);
			if( serial.time >= record.time )
				m_RealAssetSerial.RemoveAt(i);
		}
		RealGetAsset( record.time );
		return TRUE;
	}
	else
	{
		// ɾ��ʧ�ܣ��ָ�ԭ��
		m_RealCurrentCash	=	m_opparam.m_nStartAmount;
		m_RealOpRecord.RemoveAll();
		m_RealNextOp.RemoveAll();
		m_RealStockOwn.RemoveAll();
		for( int i=0; i<recordbk.GetSize(); i++ )
		{
			OPRECORD	rec	=	recordbk.ElementAt(i);
			RealOperate( rec );
		}
		return FALSE;
	}
}

/*
BOOL CStrategy::RealUnOperate( OPRECORD record )
{
	CStockInfo	info;
	if( strlen(record.szCode)>0
		&& ( !AfxGetStockContainer().GetStockInfo( record.szCode, &info )
		|| !info.IsValidStock() ) )
		return FALSE;

	double	dAmount		=	record.dwShare * record.dSharePrice;
	double	dRateCost	=	record.dRateCost;
	if( STRATEGY_OPTYPE_BUY == record.lOpType )
	{
		if( !m_RealStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
		m_RealCurrentCash	+=	(dAmount+dRateCost);
	}
	else if( STRATEGY_OPTYPE_SELL == record.lOpType )
	{
		if( m_RealCurrentCash < dAmount-dRateCost )
			return FALSE;
		if( !m_RealStockOwn.AddStock( info, record.dwShare ) )
			return FALSE;
		m_RealCurrentCash	-=	(dAmount-dRateCost);
	}
	else if( STRATEGY_OPTYPE_ADDSTOCK == record.lOpType )
	{
		if( !m_RealStockOwn.RemoveStock( info, record.dwShare ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_REMOVESTOCK == record.lOpType )
	{
		if( !m_RealStockOwn.AddStock( info, record.dwShare ) )
			return FALSE;
	}
	else if( STRATEGY_OPTYPE_ADDCASH == record.lOpType )
	{
		if( m_RealCurrentCash < record.dSharePrice )
			return FALSE;
		m_RealCurrentCash	-=	record.dSharePrice;
	}
	else if( STRATEGY_OPTYPE_REMOVECASH == record.lOpType )
	{
		m_RealCurrentCash	+=	record.dSharePrice;
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CStrategy::RealAddOpRecordStrict( OPRECORD record )
{
	COpRecordContainer recordbk;
	recordbk.Copy( m_RealOpRecord );

	BOOL	bOK	=	TRUE;
	for( int i=m_RealOpRecord.GetSize()-1; i>=0; i-- )
	{
		OPRECORD	rec	=	m_RealOpRecord.ElementAt(i);
		if( bOK && rec.time > record.time )
		{
			bOK	&=	RealUnOperate( rec );
			if( bOK )
				m_RealOpRecord.RemoveAt(i);
		}
	}

	if( bOK && RealOperate( record ) )
	{
		for( i=m_RealAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL	serial	=	m_RealAssetSerial.ElementAt(i);
			if( serial.time >= record.time )
				m_RealAssetSerial.RemoveAt(i);
		}
		RealGetAsset( record.time );
		return TRUE;
	}
	else
	{
		// Restore
		m_RealCurrentCash	=	m_opparam.m_nStartAmount;
		m_RealOpRecord.RemoveAll();
		m_RealNextOp.RemoveAll();
		m_RealStockOwn.RemoveAll();
		for( i=0; i<recordbk.GetSize(); i++ )
		{
			OPRECORD	rec	=	recordbk.ElementAt(i);
			RealOperate( rec );
		}
		return FALSE;
	}
}

BOOL CStrategy::RealDeleteOpRecordStrict( int nRecord )
{
	if( nRecord < 0 || nRecord >= m_RealOpRecord.GetSize() )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	COpRecordContainer recordbk;
	recordbk.Copy( m_RealOpRecord );

	BOOL	bOK	=	TRUE;
	OPRECORD	record	=	m_RealOpRecord.ElementAt(nRecord);
	for( int i=m_RealOpRecord.GetSize()-1; i>=0; i-- )
	{
		OPRECORD	rec	=	m_RealOpRecord.ElementAt(i);
		if( bOK && rec.time >= record.time )
		{
			bOK	&=	RealUnOperate( rec );
			if( bOK )
				m_RealOpRecord.RemoveAt(i);
		}
	}

	if( bOK )
	{
		for( i=m_RealAssetSerial.GetSize()-1; i>=0; i-- )
		{
			ASSETSERIAL	serial	=	m_RealAssetSerial.ElementAt(i);
			if( serial.time >= record.time )
				m_RealAssetSerial.RemoveAt(i);
		}
		return TRUE;
	}
	else
	{
		// Restore
		m_RealCurrentCash	=	m_opparam.m_nStartAmount;
		m_RealOpRecord.RemoveAll();
		m_RealNextOp.RemoveAll();
		m_RealStockOwn.RemoveAll();
		for( i=0; i<recordbk.GetSize(); i++ )
		{
			OPRECORD	rec	=	recordbk.ElementAt(i);
			RealOperate( rec );
		}
		return FALSE;
	}
}
*/
