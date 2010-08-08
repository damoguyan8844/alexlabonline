// strings.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include	"StdAfx.h"
#include	"Qianlong.h"
#include	"Shenglong.h"
#include	"SelfDB.h"
#include	"Strings.h"

#ifdef	CLKLAN_ENGLISH_US
char CQianlong::m_szDataType[]	=	"QianLong";
char CShenglong::m_szDataType[]	=	"Salon";
char CSelfDB::m_szDataType[]	=	"Ninebulls-Stock";
#else
char CQianlong::m_szDataType[]	=	"Ǯ��";
char CShenglong::m_szDataType[]	=	"ʤ��";
char CSelfDB::m_szDataType[]	=	"����Ϊ��";
#endif

//////////////////////////////////////////////////////////////////////////////////
// �ַ���
#ifdef	CLKLAN_ENGLISH_US
char	sz_stockname_fund[]		=	"����";
char	sz_stockname_bond[]		=	"ծȯ";
char	sz_stockname_index[]	=	"ָ��";
char	sz_stockname_bondindex[]		=	"��ծָ��";
char	sz_stockname_bondindexcorp[]	=	"��ծָ��";
#else
char	sz_stockname_fund[]		=	"����";
char	sz_stockname_bond[]		=	"ծȯ";
char	sz_stockname_index[]	=	"ָ��";
char	sz_stockname_bondindex[]		=	"��ծָ��";
char	sz_stockname_bondindexcorp[]	=	"��ծָ��";
#endif

//////////////////////////////////////////////////////////////////////////////////
// ָ�������ź��ַ���
#ifdef	CLKLAN_ENGLISH_US
char	its_sellintense[]		=	"Strong Sell";
char	its_sell[]				=	"Sell";
char	its_sellfeeble[]		=	"Faintly Sell";
char	its_nothing[]			=	"Nothing";
char	its_buyfeeble[]			=	"Faintly Buy";
char	its_buy[]				=	"Buy";
char	its_buyintense[]		=	"Strong Buy";

char	itsc_nothing[]			=	"nothing";
char	itsc_deviateonbottom[]	=	"deviate on bottom";
char	itsc_deviateontop[]		=	"deviate on top";
char	itsc_goldenfork[]		=	"golden fork";
char	itsc_deadfork[]			=	"dead fork";
char	itsc_long[]				=	"long";
char	itsc_short[]			=	"short";
char	itsc_overbought[]		=	"overbought";
char	itsc_oversold[]			=	"oversold";
char	itsc_support[]			=	"support";
char	itsc_resistance[]		=	"resistance";
#else
char	its_sellintense[]		=	"ǿ������";
char	its_sell[]				=	"����";
char	its_sellfeeble[]		=	"΢������";
char	its_nothing[]			=	"��";
char	its_buyfeeble[]			=	"΢�����";
char	its_buy[]				=	"���";
char	its_buyintense[]		=	"ǿ�����";

char	itsc_nothing[]			=	"��";
char	itsc_deviateonbottom[]	=	"�ױ���";
char	itsc_deviateontop[]		=	"������";
char	itsc_goldenfork[]		=	"���";
char	itsc_deadfork[]			=	"����";
char	itsc_long[]				=	"��ͷǿ��";
char	itsc_short[]			=	"��ͷǿ��";
char	itsc_overbought[]		=	"����";
char	itsc_oversold[]			=	"����";
char	itsc_support[]			=	"֧��";
char	itsc_resistance[]		=	"ѹ��";
#endif

//////////////////////////////////////////////////////////////////////////////////
// ָ���ַ���

// ��������ָ��Ψһ��ţ����ؼ���ָ����������
CSPString	AfxGetSTTName( UINT nTech )
{
	static	CSPString	stt_names[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );
	
	if( stt_names[0].GetLength() <= 0 )
	{
		// stt_names
		stt_names[0]			=	"STTNames";

#ifdef	CLKLAN_ENGLISH_US
		//	K��������
		stt_names[STT_MA]		=	("Moving Average");
		stt_names[STT_BBI]		=	("BBI");
		stt_names[STT_BOLL]		=	("Boollinger Bands");
		stt_names[STT_PV]		=	("Price/Volume");
		stt_names[STT_SAR]		=	("Stop And Reverse");
		stt_names[STT_DJ]		=	("Superposition");
		stt_names[STT_CW]		=	("Chips Distribution");
		//	������
		stt_names[STT_MACD]		=	("Moving Average Convergence/Divergence");
		stt_names[STT_MIKE]		=	("Mike Base");
		stt_names[STT_PSY]		=	("Psychological Line");
		//	������
		stt_names[STT_VOLUME]	=	("Volume");
		stt_names[STT_NVI]		=	("Negative Volume Indicator");
		stt_names[STT_PVI]		=	("Positive Volume Indicator");
		stt_names[STT_VR]		=	("Volume Ratio");
		stt_names[STT_VROC]		=	("Volume Rate Indicator");
		stt_names[STT_OBV]		=	("On Balance Volume");
		stt_names[STT_MOBV]		=	("Moving On Balance Volume");
		stt_names[STT_MFI]		=	("Money Flow Indicator");
		stt_names[STT_VMACD]	=	("Volume Moving Average Convergence/Divergence");
		stt_names[STT_WVAD]		=	("William's Variable Accumulation Distribution");
		stt_names[STT_EMV]		=	("Ease of Movement Value");
		stt_names[STT_VRSI]		=	("Volume Relative Strength Indicator");
		stt_names[STT_NVRSI]	=	("New Volume Relative Strength Indicator");
		stt_names[STT_AD]		=	("Accumulation/Distribution");
		stt_names[STT_CI]		=	("Cumulation Indicator");
		//	�ڶ���
		stt_names[STT_KDJ]		=	("Stochastics");
		stt_names[STT_R]		=	("William's Overbought/Oversold Indicator");
		stt_names[STT_RSI]		=	("Relative Strength Indicator");
		stt_names[STT_BIAS]		=	("Bias");
		stt_names[STT_MTM]		=	("Momentum");
		stt_names[STT_DMI]		=	("Directional Movement Indicator");
		stt_names[STT_ROC]		=	("Rate Of Change");
		stt_names[STT_CCI]		=	("Commodity Channel Indicator");
		stt_names[STT_CV]		=	("Chavkin");
		stt_names[STT_ARBR]		=	("AR BR");
		stt_names[STT_CR]		=	("CR");
		stt_names[STT_OSC]		=	("Oscillator");
		stt_names[STT_UOS]		=	("Ultimate Oscillator");
		stt_names[STT_MAOSC]	=	("Moving Average Oscillator");
		stt_names[STT_36BIAS]	=	("3-6 Bias");
		stt_names[STT_DPO]		=	("Detrended Price Oscillator");
		stt_names[STT_KST]		=	("Know Sure Things");
		//	������
		stt_names[STT_REI]		=	("Range Expansion Indicator");
		stt_names[STT_DMKI]		=	("Demaker Indicator");
		stt_names[STT_PCNT]		=	("Percent");
		stt_names[STT_HLC]		=	("Highest and Lowest and Close Price");
		stt_names[STT_CDP]		=	("CDP");
		stt_names[STT_ASI]		=	("Accumulation Swing Indicator");
		stt_names[STT_ATR]		=	("Average True Ranger");
		//	����Ϊ��ָ����
		stt_names[STT_CYO]		=	("Cycle Oscillator");
		stt_names[STT_DCYO]		=	("Degaussed Cycle Oscillator");
		stt_names[STT_HSL]		=	("Relative Change Hands");
		stt_names[STT_DPER]		=	("Detrended Percent Oscillator");
#else
		//	K��������
		stt_names[STT_MA]		=	("�ƶ�ƽ����");
		stt_names[STT_BBI]		=	("���ָ��");
		stt_names[STT_BOLL]		=	("���ִ�");
		stt_names[STT_PV]		=	("��֤������");
		stt_names[STT_SAR]		=	("ͣ���ת��ָ��");
		stt_names[STT_DJ]		=	("����ͼ");
		stt_names[STT_CW]		=	("����ֲ�ͼ");
		//	������
		stt_names[STT_MACD]		=	("ָ��ƽ����ͬ�ƶ�ƽ����");
		stt_names[STT_MIKE]		=	("���ָ��");
		stt_names[STT_PSY]		=	("������");
		//	������
		stt_names[STT_VOLUME]	=	("�ɽ���");
		stt_names[STT_NVI]		=	("���ɽ���ָ��");
		stt_names[STT_PVI]		=	("���ɽ���ָ��");
		stt_names[STT_VR]		=	("��������");
		stt_names[STT_VROC]		=	("�ɽ����䶯��ָ��");
		stt_names[STT_OBV]		=	("������");
		stt_names[STT_MOBV]		=	("��������");
		stt_names[STT_MFI]		=	("�ʽ�����ָ��");
		stt_names[STT_VMACD]	=	("��ָ��ƽ����ͬ�ƶ�ƽ����");
		stt_names[STT_WVAD]		=	("����������ɢ��");
		stt_names[STT_EMV]		=	("���ײ���ָ��");
		stt_names[STT_VRSI]		=	("�����ǿ��ָ��");
		stt_names[STT_NVRSI]	=	("�������ǿ��ָ��");
		stt_names[STT_AD]		=	("��/ɢָ��");
		stt_names[STT_CI]		=	("�ۻ�ָ��");
		//	�ڶ���
		stt_names[STT_KDJ]		=	("���ָ��");
		stt_names[STT_R]		=	("����ָ��");
		stt_names[STT_RSI]		=	("���ǿ��ָ��");
		stt_names[STT_BIAS]		=	("������");
		stt_names[STT_MTM]		=	("����ָ��");
		stt_names[STT_DMI]		=	("����ָ��");
		stt_names[STT_ROC]		=	("�䶯��ָ��");
		stt_names[STT_CCI]		=	("˳��ͨ��ָ��");
		stt_names[STT_CV]		=	("����ָ��");
		stt_names[STT_ARBR]		=	("������Ըָ��");
		stt_names[STT_CR]		=	("�м���Ըָ��");
		stt_names[STT_OSC]		=	("�ڶ���");
		stt_names[STT_UOS]		=	("���ڶ�ָ��");
		stt_names[STT_MAOSC]	=	("�ƶ�ƽ���ڶ���");
		stt_names[STT_36BIAS]	=	("3��6�չ���");
		stt_names[STT_DPO]		=	("ȥ�Ƽ۸�ڶ�ָ��");
		stt_names[STT_KST]		=	("ȷȻָ��");
		//	������
		stt_names[STT_REI]		=	("��Χ����ָ��");
		stt_names[STT_DMKI]		=	("����˶�ָ��");
		stt_names[STT_PCNT]		=	("���ȱ�");
		stt_names[STT_HLC]		=	("���ȷ���");
		stt_names[STT_CDP]		=	("���Ʋ���ָ��");
		stt_names[STT_ASI]		=	("�ۻ��ڶ�ָ��");
		stt_names[STT_ATR]		=	("����ָ��");
		//	����Ϊ��ָ����
		stt_names[STT_CYO]		=	("���ڰڶ���");
		stt_names[STT_DCYO]		=	("ȥ�����ڰڶ���");
		stt_names[STT_HSL]		=	("��Ի�����");
		stt_names[STT_DPER]		=	("ȥ�ưٷֱ�ָ��");
#endif
	}
	
	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_names[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserName( nTech );
	}
	return "";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ��Ӣ�ļ��
CSPString	AfxGetSTTShortName( UINT nTech )
{
	static	CSPString	stt_shortnames[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_shortnames[0].GetLength() <= 0 )
	{
		// stt_shortnames
		stt_shortnames[0]			=	"STTShortNames";

		//	K��������
		stt_shortnames[STT_MA]		=	("MA");
		stt_shortnames[STT_BBI]		=	("BBI");
		stt_shortnames[STT_BOLL]	=	("BOLL");
		stt_shortnames[STT_PV]		=	("P/V");
		stt_shortnames[STT_SAR]		=	("SAR");
		stt_shortnames[STT_DJ]		=	("DJ");
		stt_shortnames[STT_CW]		=	("CW");
		//	������
		stt_shortnames[STT_MACD]	=	("MACD");
		stt_shortnames[STT_MIKE]	=	("MIKE");
		stt_shortnames[STT_PSY]		=	("PSY");
		//	������
		stt_shortnames[STT_VOLUME]	=	("VOLUME");
		stt_shortnames[STT_NVI]		=	("NVI");
		stt_shortnames[STT_PVI]		=	("PVI");
		stt_shortnames[STT_VR]		=	("VR");
		stt_shortnames[STT_VROC]	=	("VROC");
		stt_shortnames[STT_OBV]		=	("OBV");
		stt_shortnames[STT_MOBV]	=	("MOBV");
		stt_shortnames[STT_MFI]		=	("MFI");
		stt_shortnames[STT_VMACD]	=	("VMACD");
		stt_shortnames[STT_WVAD]	=	("WVAD");
		stt_shortnames[STT_EMV]		=	("EMV");
		stt_shortnames[STT_VRSI]	=	("VRSI");
		stt_shortnames[STT_NVRSI]	=	("NVRSI");
		stt_shortnames[STT_AD]		=	("A/D");
		stt_shortnames[STT_CI]		=	("CI");
		//	�ڶ���
		stt_shortnames[STT_KDJ]		=	("KDJ");
		stt_shortnames[STT_R]		=	("R");
		stt_shortnames[STT_RSI]		=	("RSI");
		stt_shortnames[STT_BIAS]	=	("BIAS");
		stt_shortnames[STT_MTM]		=	("MTM");
		stt_shortnames[STT_DMI]		=	("DMI");
		stt_shortnames[STT_ROC]		=	("ROC");
		stt_shortnames[STT_CCI]		=	("CCI");
		stt_shortnames[STT_CV]		=	("CV");
		stt_shortnames[STT_ARBR]	=	("ARBR");
		stt_shortnames[STT_CR]		=	("CR");
		stt_shortnames[STT_OSC]		=	("OSC");
		stt_shortnames[STT_UOS]		=	("UOS");
		stt_shortnames[STT_MAOSC]	=	("MAOSC");
		stt_shortnames[STT_36BIAS]	=	("3-6BIAS");
		stt_shortnames[STT_DPO]		=	("DPO");
		stt_shortnames[STT_KST]		=	("KST");
		//	������
		stt_shortnames[STT_REI]		=	("REI");
		stt_shortnames[STT_DMKI]	=	("DMKI");
		stt_shortnames[STT_PCNT]	=	("PCNT");
		stt_shortnames[STT_HLC]		=	("HLC");
		stt_shortnames[STT_CDP]		=	("CDP");
		stt_shortnames[STT_ASI]		=	("ASI");
		stt_shortnames[STT_ATR]		=	("ATR");
		//	����Ϊ��ָ����
		stt_shortnames[STT_CYO]		=	("CYO");
		stt_shortnames[STT_DCYO]	=	("DCYO");
		stt_shortnames[STT_HSL]		=	("HSL");
		stt_shortnames[STT_DPER]	=	("DPER");
	}

	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_shortnames[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserShortName( nTech );
	}
	return "";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ����������+Ӣ�ļ��
CSPString	AfxGetSTTFullName( UINT nTech )
{
	return AfxGetSTTName( nTech ) + "(" + AfxGetSTTShortName( nTech ) + ")";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ���������
CSPString	AfxGetSTTClassName( UINT nTech )
{
	static	CSPString	stt_classnames[STT_CLASS_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_classnames[0].GetLength() <= 0 )
	{
		// stt_classnames
		stt_classnames[0]					=	"STTClassNames";

#ifdef	CLKLAN_ENGLISH_US
		stt_classnames[STT_CLASS_KLINE]		=	"K Line Attribute";
		stt_classnames[STT_CLASS_TREND]		=	"Trend";
		stt_classnames[STT_CLASS_ENERGY]	=	"Energy";
		stt_classnames[STT_CLASS_SWING]		=	"Swing";
		stt_classnames[STT_CLASS_OTHER]		=	"Others";
		stt_classnames[STT_CLASS_CLK]		=	"Ninebulls-Stock";
		stt_classnames[STT_CLASS_USER]		=	"User Define";
#else
		stt_classnames[STT_CLASS_KLINE]		=	"K��������";
		stt_classnames[STT_CLASS_TREND]		=	"������";
		stt_classnames[STT_CLASS_ENERGY]	=	"������";
		stt_classnames[STT_CLASS_SWING]		=	"�ڶ���";
		stt_classnames[STT_CLASS_OTHER]		=	"������";
		stt_classnames[STT_CLASS_CLK]		=	"����Ϊ��ָ����";
		stt_classnames[STT_CLASS_USER]		=	"���ָ����";
#endif
	}

	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
		return stt_classnames[STT_CLASS_KLINE];
	else if( nTech >= STT_TREND_MIN && nTech <= STT_TREND_MAX )
		return stt_classnames[STT_CLASS_TREND];
	else if( nTech >= STT_ENERGY_MIN && nTech <= STT_ENERGY_MAX )
		return stt_classnames[STT_CLASS_ENERGY];
	else if( nTech >= STT_SWING_MIN && nTech <= STT_SWING_MAX )
		return stt_classnames[STT_CLASS_SWING];
	else if( nTech >= STT_OTHER_MIN && nTech <= STT_OTHER_MAX )
		return stt_classnames[STT_CLASS_OTHER];
	else if( nTech >= STT_CLK_MIN && nTech <= STT_CLK_MAX )
		return stt_classnames[STT_CLASS_CLK];
	else if( nTech >= STT_USER_MIN )
		return stt_classnames[STT_CLASS_USER];
	return "";
}

// ���������ź�ֵ���õ�����˵��
CSPString	AfxGetIntensityString( int nIntensity )
{
	if( nIntensity >= ITS_MIN && nIntensity <= ITSG_SELLINTENSE )
		return its_sellintense;
	else if( nIntensity > ITSG_SELLINTENSE && nIntensity <= ITSG_SELL )
		return its_sell;
	else if( nIntensity > ITSG_SELL && nIntensity <= ITSG_SELLFEEBLE )
		return its_sellfeeble;
	else if( nIntensity > ITSG_SELLFEEBLE && nIntensity < ITSG_BUYFEEBLE  )
		return its_nothing;
	else if( nIntensity >= ITSG_BUYFEEBLE && nIntensity < ITSG_BUY )
		return its_buyfeeble;
	else if( nIntensity >= ITSG_BUY && nIntensity < ITSG_BUYINTENSE )
		return its_buy;
	else if( nIntensity >= ITSG_BUYINTENSE && nIntensity <= ITS_MAX )
		return its_buyintense;
	
	SP_ASSERT( FALSE );
	return "";
}

// ���������ź�����˵��ֵ���õ�����˵��
CSPString	AfxGetIntensityCodeString( UINT nCode )
{
	switch( nCode )
	{
	case	ITSC_NOTHING:			return	itsc_nothing;
	case	ITSC_DEVIATEONBOTTOM:	return	itsc_deviateonbottom;
	case	ITSC_DEVIATEONTOP:		return	itsc_deviateontop;
	case	ITSC_GOLDENFORK:		return	itsc_goldenfork;
	case	ITSC_DEADFORK:			return	itsc_deadfork;
	case	ITSC_LONG:				return	itsc_long;
	case	ITSC_SHORT:				return	itsc_short;
	case	ITSC_OVERBOUGHT:		return	itsc_overbought;
	case	ITSC_OVERSOLD:			return	itsc_oversold;
	case	ITSC_SUPPORT:			return	itsc_support;
	case	ITSC_RESISTANCE:		return	itsc_resistance;
	default:
		SP_ASSERT( FALSE );
	}
	return "";
}

CSPString AfxGetKTypeString( int ktype )
{
#ifdef	CLKLAN_ENGLISH_US
	if( CKData::ktypeMonth == ktype )
		return "Month Line";
	else if( CKData::ktypeWeek == ktype )
		return "Week Line";
	else if( CKData::ktypeDay == ktype )
		return "Day Line";
	else if( CKData::ktypeMin5 == ktype )
		return "5 Minutes Line";
	else if( CKData::ktypeMin15 == ktype )
		return "15 Minutes Line";
	else if( CKData::ktypeMin30 == ktype )
		return "30 Minutes Line";
	else if( CKData::ktypeMin60 == ktype )
		return "60 Minutes Line";
	else
		return "Unknown";
#else
	if( CKData::ktypeMonth == ktype )
		return "����";
	else if( CKData::ktypeWeek == ktype )
		return "����";
	else if( CKData::ktypeDay == ktype )
		return "����";
	else if( CKData::ktypeMin5 == ktype )
		return "�������";
	else if( CKData::ktypeMin15 == ktype )
		return "ʮ�������";
	else if( CKData::ktypeMin30 == ktype )
		return "��ʮ������";
	else if( CKData::ktypeMin60 == ktype )
		return "��ʮ������";
	else
		return "δ֪";
#endif
	return "";
}

CSPString AfxGetMaindataTypeString( int type )
{
#ifdef	CLKLAN_ENGLISH_US
	if( CKData::mdtypeClose == type )
		return "Close Price";
	else if( CKData::mdtypeOpen == type )
		return "Open Price";
	else if( CKData::mdtypeAverage == type )
		return "Average Price";
	else
		return "Unknown";
#else
	if( CKData::mdtypeClose == type )
		return "���̼�";
	else if( CKData::mdtypeOpen == type )
		return "���̼�";
	else if( CKData::mdtypeAverage == type )
		return "ƽ����";
	else
		return "δ֪";
#endif
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// ��Ʊ�б��������ַ���
#ifdef	CLKLAN_ENGLISH_US
char	slh_hdrday[]	=	"Day";
char	slh_avrcode[]	=	"AVR";
char	slh_avrname[]	=	"Average";
char	slh_wavrcode[]	=	"WAVR";
char	slh_wavrname[]	=	"Weight-Average";
#else
char	slh_hdrday[]	=	"��";
char	slh_avrcode[]	=	"AVR";
char	slh_avrname[]	=	"ƽ��ֵ";
char	slh_wavrcode[]	=	"WAVR";
char	slh_wavrname[]	=	"��Ȩƽ��";
#endif

CSPString	AfxGetSLHTitle( UINT nSLH )
{
	static	CSPString	slh_titles[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_titles[0].GetLength() <= 0 )
	{
		// slh_titles
		slh_titles[SLH_NONE]				=	"SLHTitles";
		
#ifdef	CLKLAN_ENGLISH_US
		slh_titles[SLH_CODE]				=	"Code";
		slh_titles[SLH_NAME]				=	"Name";
		
		//	��������
		slh_titles[SLH_DATE]				=	"Trade Time";
		slh_titles[SLH_LASTCLOSE]			=	"Prev Close";
		slh_titles[SLH_OPEN]				=	"Open";
		slh_titles[SLH_CLOSE]				=	"Last Trade";
		slh_titles[SLH_HIGH]				=	"High";
		slh_titles[SLH_LOW]					=	"Low";
		slh_titles[SLH_AVERAGE]				=	"Average";
		slh_titles[SLH_DIFF]				=	"Change";
		slh_titles[SLH_DIFFPERCENT]			=	"Change(%)";
		slh_titles[SLH_SCOPE]				=	"Range(%)";
		slh_titles[SLH_VOLUME]				=	"Volume(100)";
		slh_titles[SLH_AMOUNT]				=	"Amount(1000)";
		slh_titles[SLH_VOLUP]				=	"Up Vol.";
		slh_titles[SLH_VOLDOWN]				=	"Down Vol.";
		slh_titles[SLH_DIFFPERCENT_MIN5]	=	"5-Minute Change(%)";
		slh_titles[SLH_SELLBUYRATIO]		=	"Consign Ratio(%)";
		slh_titles[SLH_SELLBUYDIFF]			=	"Consign Volume Difference";
		slh_titles[SLH_BUYPRICE3]			=	"Bid 3";
		slh_titles[SLH_BUYPRICE2]			=	"Bid 2";
		slh_titles[SLH_BUYPRICE1]			=	"Bid 1";
		slh_titles[SLH_SELLPRICE1]			=	"Ask 1";
		slh_titles[SLH_SELLPRICE2]			=	"Ask 2";
		slh_titles[SLH_SELLPRICE3]			=	"Ask 3";
		slh_titles[SLH_BUYVOLUME3]			=	"Bid Vol 3(100)";
		slh_titles[SLH_BUYVOLUME2]			=	"Bid Vol 2(100)";
		slh_titles[SLH_BUYVOLUME1]			=	"Bid Vol 1(100)";
		slh_titles[SLH_SELLVOLUME1]			=	"Ask Vol 1(100)";
		slh_titles[SLH_SELLVOLUME2]			=	"Ask Vol 2(100)";
		slh_titles[SLH_SELLVOLUME3]			=	"Ask Vol 3(100)";

		//	��Ҫ����ָ��
		slh_titles[SLH_REPORTTYPE]			=	"Report Forms";
		slh_titles[SLH_PE]					=	"P/E";
		slh_titles[SLH_PNETASSET]			=	"P/B";
		slh_titles[SLH_PMAININCOME]			=	"P/S"; // "Market Capitalisation/Income";
		slh_titles[SLH_RATIO_PCASH]			=	"Price/Cash";
		slh_titles[SLH_RATIO_CURRENCY]		=	"Currency Percent(%)";
		slh_titles[SLH_RATIO_CHANGEHAND]	=	"Change hands Percent(%)";
		slh_titles[SLH_RATIO_VOLUME]		=	"Volume Ratio";
		slh_titles[SLH_RS]					=	"Relative Strength";
		slh_titles[SLH_MARKETVALUE]			=	"Market Capitalisation(10000)";
		slh_titles[SLH_MARKETVALUEA]		=	"A Market Capitalisation(10000)";
		slh_titles[SLH_MARKETVALUEB]		=	"B Market Capitalisation(10000)";
		
		// �ﳥծ����
		slh_titles[SLH_RATIO_LIQUIDITY]		=	"Liquidity Ratio";
		slh_titles[SLH_RATIO_QUICK]			=	"Quickassets Ratio";
		slh_titles[SLH_VELOCITY_RECEIVABLES]=	"Receivables Velocity";
		
		// �ﾭӪ����
		slh_titles[SLH_VELOCITY_MERCHANDISE]=	"Merchandise Velocity";
		slh_titles[SLH_MAIN_INCOME]			=	"Income(10000)";
		slh_titles[SLH_CASH_PS]				=	"Cash per share";
		
		// ��ӯ������
		slh_titles[SLH_PROFIT_MARGIN]		=	"Profit Margin(%)";
		slh_titles[SLH_NETASSET_YIELD]		=	"Return On Equity(%)";
		
		// ���ʱ��ṹ
		slh_titles[SLH_DATE_BEGIN]			=	"IPO Date";
		slh_titles[SLH_SHARE_COUNT_TOTAL]	=	"Total Shares(10000)";
		slh_titles[SLH_SHARE_COUNT_A]		=	"Issued Shares A(10000)";
		slh_titles[SLH_SHARE_COUNT_B]		=	"Issued Shares B(10000)";
		slh_titles[SLH_SHARE_COUNT_H]		=	"Issued Shares H(10000)";
		slh_titles[SLH_SHARE_COUNT_NATIONAL]=	"National Shares(10000)";
		slh_titles[SLH_SHARE_COUNT_CORP]	=	"Corporation Shares(10000)";
		slh_titles[SLH_PROFIT_PSUD]			=	"Not Assigned Profit per Share";
		slh_titles[SLH_ASSET]				=	"Total Asset(10000)";
		slh_titles[SLH_RATIO_HOLDERRIGHT]	=	"Shareholders Rights Ratio(%)";
		slh_titles[SLH_RATIO_LONGDEBT]		=	"Long Debt Ratio(%)";
		slh_titles[SLH_RATIO_DEBT]			=	"Debt Ratio(%)";

		// ��Ͷ����������
		slh_titles[SLH_NETASSET_PS]			=	"Net Asset per Share";
		slh_titles[SLH_NETASSET_PS_REGULATE]=	"Net Asset per Share Regulated";
		slh_titles[SLH_EPS]					=	"EPS"; // "Earnings per Share";
		slh_titles[SLH_EPS_DEDUCT]			=	"EPS Deducted"; // "Earnings per Share Deducted";
		slh_titles[SLH_NET_PROFIT]			=	"Net Profit/Loss(10000)";
		slh_titles[SLH_MAIN_PROFIT]			=	"Main Profit/Loss(10000)";
		slh_titles[SLH_TOTAL_PROFIT]		=	"Total Profit/Loss(10000)";
		slh_titles[SLH_PROFIT_INC]			=	"Profit Increase(%)";
		slh_titles[SLH_INCOME_INC]			=	"Income Increase(%)";
		slh_titles[SLH_ASSET_INC]			=	"Asset Increase(%)";
		slh_titles[SLH_ANNGAINS_AVERAGE]	=	"Average Ann-Gains(%)";
		slh_titles[SLH_ANNGAINS_STDDEV]		=	"Ann-Gains Standard Deviation(%)";
		slh_titles[SLH_BETA]				=	"Beta";
		slh_titles[SLH_SHARP]				=	"Sharp Venture";
		slh_titles[SLH_TRADE]				=	"Business Classification";
		slh_titles[SLH_PROVINCE]			=	"Province";
#else
		slh_titles[SLH_CODE]				=	"����";
		slh_titles[SLH_NAME]				=	"����";
		
		//	��������
		slh_titles[SLH_DATE]				=	"����";
		slh_titles[SLH_LASTCLOSE]			=	"����";
		slh_titles[SLH_OPEN]				=	"���̼�";
		slh_titles[SLH_CLOSE]				=	"�ּ�";
		slh_titles[SLH_HIGH]				=	"��߼�";
		slh_titles[SLH_LOW]					=	"��ͼ�";
		slh_titles[SLH_AVERAGE]				=	"����";
		slh_titles[SLH_DIFF]				=	"�ǵ�";
		slh_titles[SLH_DIFFPERCENT]			=	"�Ƿ�%";
		slh_titles[SLH_SCOPE]				=	"���%";
		slh_titles[SLH_VOLUME]				=	"�ɽ���(��)";
		slh_titles[SLH_AMOUNT]				=	"�ɽ���(ǧԪ)";
		slh_titles[SLH_VOLUP]				=	"����";
		slh_titles[SLH_VOLDOWN]				=	"����";
		slh_titles[SLH_DIFFPERCENT_MIN5]	=	"������Ƿ�%";
		slh_titles[SLH_SELLBUYRATIO]		=	"ί��%";
		slh_titles[SLH_SELLBUYDIFF]			=	"ί����";
		slh_titles[SLH_BUYPRICE3]			=	"�����";
		slh_titles[SLH_BUYPRICE2]			=	"��۶�";
		slh_titles[SLH_BUYPRICE1]			=	"���һ";
		slh_titles[SLH_SELLPRICE1]			=	"����һ";
		slh_titles[SLH_SELLPRICE2]			=	"���۶�";
		slh_titles[SLH_SELLPRICE3]			=	"������";
		slh_titles[SLH_BUYVOLUME3]			=	"������";
		slh_titles[SLH_BUYVOLUME2]			=	"������";
		slh_titles[SLH_BUYVOLUME1]			=	"����һ";
		slh_titles[SLH_SELLVOLUME1]			=	"����һ";
		slh_titles[SLH_SELLVOLUME2]			=	"������";
		slh_titles[SLH_SELLVOLUME3]			=	"������";

		//	��Ҫ����ָ��
		slh_titles[SLH_REPORTTYPE]			=	"��������";
		slh_titles[SLH_PE]					=	"��ӯ��";
		slh_titles[SLH_PNETASSET]			=	"�о���";
		slh_titles[SLH_PMAININCOME]			=	"������";
		slh_titles[SLH_RATIO_PCASH]			=	"�۸��ֽ��";
		slh_titles[SLH_RATIO_CURRENCY]		=	"��ͨ��%";
		slh_titles[SLH_RATIO_CHANGEHAND]	=	"������%";
		slh_titles[SLH_RATIO_VOLUME]		=	"����";
		slh_titles[SLH_RS]					=	"���ǿ��";
		slh_titles[SLH_MARKETVALUE]			=	"����ֵ-��";
		slh_titles[SLH_MARKETVALUEA]		=	"A����ֵ-��";
		slh_titles[SLH_MARKETVALUEB]		=	"B����ֵ-��";
		
		// �ﳥծ����
		slh_titles[SLH_RATIO_LIQUIDITY]		=	"��������";
		slh_titles[SLH_RATIO_QUICK]			=	"�ٶ�����";
		slh_titles[SLH_VELOCITY_RECEIVABLES]=	"Ӧ���˿�����";
		
		// �ﾭӪ����
		slh_titles[SLH_VELOCITY_MERCHANDISE]=	"�����ת��";
		slh_titles[SLH_MAIN_INCOME]			=	"��Ӫ����-��";
		slh_titles[SLH_CASH_PS]				=	"ÿ�ɾ��ֽ�";
		
		// ��ӯ������
		slh_titles[SLH_PROFIT_MARGIN]		=	"��Ӫ������%";
		slh_titles[SLH_NETASSET_YIELD]		=	"���ʲ�������%";
		
		// ���ʱ��ṹ
		slh_titles[SLH_DATE_BEGIN]			=	"��������";
		slh_titles[SLH_SHARE_COUNT_TOTAL]	=	"�ܹɱ�-��";
		slh_titles[SLH_SHARE_COUNT_A]		=	"A��-��";
		slh_titles[SLH_SHARE_COUNT_B]		=	"B��-��";
		slh_titles[SLH_SHARE_COUNT_H]		=	"H��-��";
		slh_titles[SLH_SHARE_COUNT_NATIONAL]=	"���й�-��";
		slh_titles[SLH_SHARE_COUNT_CORP]	=	"���˹�-��";
		slh_titles[SLH_PROFIT_PSUD]			=	"ÿ��δ������";
		slh_titles[SLH_ASSET]				=	"���ʲ�-��";
		slh_titles[SLH_RATIO_HOLDERRIGHT]	=	"�ɶ�Ȩ���%";
		slh_titles[SLH_RATIO_LONGDEBT]		=	"���ڸ�ծ��%";
		slh_titles[SLH_RATIO_DEBT]			=	"�ʲ���ծ��%";

		// ��Ͷ����������
		slh_titles[SLH_NETASSET_PS]			=	"ÿ�ɾ��ʲ�";
		slh_titles[SLH_NETASSET_PS_REGULATE]=	"����ÿ�ɾ��ʲ�";
		slh_titles[SLH_EPS]					=	"ÿ������";
		slh_titles[SLH_EPS_DEDUCT]			=	"�۳�ÿ������";
		slh_titles[SLH_NET_PROFIT]			=	"������-��";
		slh_titles[SLH_MAIN_PROFIT]			=	"��Ӫҵ������-��";
		slh_titles[SLH_TOTAL_PROFIT]		=	"�����ܶ�-��";
		slh_titles[SLH_PROFIT_INC]			=	"��Ӫ����������%";
		slh_titles[SLH_INCOME_INC]			=	"����������%";
		slh_titles[SLH_ASSET_INC]			=	"���ʲ�������%";
		slh_titles[SLH_ANNGAINS_AVERAGE]	=	"��ƽ��������%";
		slh_titles[SLH_ANNGAINS_STDDEV]		=	"�����׼��%";
		slh_titles[SLH_BETA]				=	"��ֵ";
		slh_titles[SLH_SHARP]				=	"���շ���ָ��";
		slh_titles[SLH_TRADE]				=	"��ҵ";
		slh_titles[SLH_PROVINCE]			=	"ʡ��";
#endif
	}

	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_titles[nSLH];
	}
	return "";
}

CSPString	AfxGetSLHDescript( UINT nSLH )
{
	static	CSPString	slh_descripts[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_descripts[0].GetLength() <= 0 )
	{
		// slh_descripts
#ifdef	CLKLAN_ENGLISH_US
		slh_descripts[SLH_PE]			=	"Ratio of Price and Earnings per Share";

		slh_descripts[SLH_PNETASSET]	=	"Ratio of Price and Net Asset per Share";
		slh_descripts[SLH_PMAININCOME]	=	"Ratio of Market Capitalisation and Income";
		slh_descripts[SLH_RATIO_PCASH]	=	"Ratio of Price and Cash per Share";
		slh_descripts[SLH_RS]			=	"Ratio of Rising and Falling in n Days";
		slh_descripts[SLH_ANNGAINS_AVERAGE]="Average Ann-Gains";
		slh_descripts[SLH_ANNGAINS_STDDEV]=	"Ann-Gains Standard Deviation";
		slh_descripts[SLH_BETA]			=	"Correlation with Indicator";
		slh_descripts[SLH_SHARP]		=	"Ratio of Ann-Gains and Ann-Gains' Standard Deviation";
#else
		slh_descripts[SLH_PE]			=	"ÿ�ɼ۸��ÿ������";

		slh_descripts[SLH_PNETASSET]	=	"ÿ�ɼ۸��ÿ�ɾ��ʲ�";
		slh_descripts[SLH_PMAININCOME]	=	"����ֵ����Ӫ����";
		slh_descripts[SLH_RATIO_PCASH]	=	"ÿ�ɼ۸��ÿ�ɾ��ֽ�";
		slh_descripts[SLH_RS]			=	"n�����Ƿ������֮��";
		slh_descripts[SLH_ANNGAINS_AVERAGE]="Ͷ�ʸù�Ʊ��������";
		slh_descripts[SLH_ANNGAINS_STDDEV]=	"�������ʵı�׼��";
		slh_descripts[SLH_BETA]			=	"��ʾ����������";
		slh_descripts[SLH_SHARP]		=	"�������ʱȱ�׼��";
#endif
	}
	
	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_descripts[nSLH];
	}
	return "";
}


//////////////////////////////////////////////////////////////////////////////////
// ���ʽ������ʾ�ַ���
#ifdef	CLKLAN_ENGLISH_US
char	express_errparserstack[]	=	"Expression: Expression is too long and stack overflow.";
char	express_errbadrange[]		=	"Expression: Value is out of range.";
char	express_errexpression[]		=	"Expression: Strings error.";
char	express_erroperator[]		=	"Expression: Operator Sign error.";
char	express_erropenparen[]		=	"Expression: Open parenthesis error.";
char	express_errcloseparen[]		=	"Expression: Close parenthesis error.";
char	express_errinvalidnum[]		=	"Expression: Invalid number.";
char	express_errmath[]			=	"Expression: Mathematical error.";
char	express_errunknown[]		=	"Expression: Unknown.";
#else
char	express_errparserstack[]	=	"���ʽ�����ʽ̫����ջ�����";
char	express_errbadrange[]		=	"���ʽ����ֵ������Χ��";
char	express_errexpression[]		=	"���ʽ���ַ������ִ���";
char	express_erroperator[]		=	"���ʽ�����������ִ���";
char	express_erropenparen[]		=	"���ʽ�������ų��ִ���";
char	express_errcloseparen[]		=	"���ʽ�������ų��ִ���";
char	express_errinvalidnum[]		=	"���ʽ���Ƿ����֡�";
char	express_errmath[]			=	"���ʽ����ѧ������ִ���";
char	express_errunknown[]		=	"���ʽ��δ֪����";
#endif


//////////////////////////////////////////////////////////////////////////////////
// ��Ʊ����ַ���
#ifdef	CLKLAN_ENGLISH_US
char	domain_self[]	=	"Favorites";
#else
char	domain_self[]	=	"��ѡ��";
#endif


//////////////////////////////////////////////////////////////////////////////////
// ������ɫ�����ַ���
#ifdef	CLKLAN_ENGLISH_US
char	colorclass_standard[]		=	"Standard color schemes";
char	colorclass_byzantinesque[]	=	"Byzantinesque color schemes";
char	colorclass_gothic[]			=	"Gothic color schemes";
char	colorclass_baroque[]		=	"Barococo color schemes";
#else
char	colorclass_standard[]		=	"��ͳʽ��ɫ����";
char	colorclass_byzantinesque[]	=	"��ռͥʽ��ɫ����";
char	colorclass_gothic[]			=	"����ʽ��ɫ����";
char	colorclass_baroque[]		=	"�����ʽ��ɫ����";
#endif


//////////////////////////////////////////////////////////////////////////////////
// ������˵��������֧��
#ifdef	CLKLAN_ENGLISH_US
char	sz_mainserverinfo[]			=	"Main Server";
char	sz_mailsubjectsupport[]		=	"Support";
#else
char	sz_mainserverinfo[]			=	"��������";
char	sz_mailsubjectsupport[]		=	"����֧��";
#endif


//////////////////////////////////////////////////////////////////////////////////
// ��
#ifdef	CLKLAN_ENGLISH_US
char	sz_sunday[]		=	"Sunday";
char	sz_monday[]		=	"Monday";
char	sz_tuesday[]	=	"Tuesday";
char	sz_wednesday[]	=	"Wednesday";
char	sz_thursday[]	=	"Thursday";
char	sz_friday[]		=	"Friday";
char	sz_saturday[]	=	"Saturday";
#else
char	sz_sunday[]		=	"������";
char	sz_monday[]		=	"����һ";
char	sz_tuesday[]	=	"���ڶ�";
char	sz_wednesday[]	=	"������";
char	sz_thursday[]	=	"������";
char	sz_friday[]		=	"������";
char	sz_saturday[]	=	"������";
#endif


//////////////////////////////////////////////////////////////////////////////////
// ��������
#ifdef	CLKLAN_ENGLISH_US
char	sz_reportQuarter[]	=	"1st quarter";
char	sz_reportMid[]		=	"midyear";
char	sz_reportQuarter3[]	=	"3rd quarter";
char	sz_reportAnnals[]	=	"annals";
#else
char	sz_reportQuarter[]	=	"��һ���ȼ���";
char	sz_reportMid[]		=	"�б�";
char	sz_reportQuarter3[]	=	"�������ȼ���";
char	sz_reportAnnals[]	=	"�걨";
#endif

//////////////////////////////////////////////////////////////////////////////////
// �����ַ���
#ifdef	CLKLAN_ENGLISH_US
char	db_errcreateselfdb[]	=	"Fail to create file, reinstall this software, please.";
char	db_errrootpath[]		=	"Fail to initialize data, reinstall this software, please.";
char	db_errqianlongpath[]	=	"Fail to read referred QianLong data source.";
#else
char	db_errcreateselfdb[]	=	"���������ļ�ʧ�ܣ������°�װ������������������Ӧ����ϵ";
char	db_errrootpath[]		=	"��ʼ������ʧ�ܣ������°�װ������������������Ӧ����ϵ";
char	db_errqianlongpath[]	=	"��ȡָ����Ǯ������Դʧ�ܡ�";
#endif


//////////////////////////////////////////////////////////////////////////////////
// �����ַ���

#ifdef	CLKLAN_ENGLISH_US

char	strategy_logicand[]		=	"All";
char	strategy_logicor[]		=	"Any";

char	strategy_sdonce[]		=	"Full";
char	strategy_sdtwice[]		=	"Half";
char	strategy_sdthird[]		=	"One-third";
char	strategy_sdforth[]		=	"Quarter";
char	strategy_sdfifth[]		=	"One-fifth";
char	strategy_sdsixth[]		=	"One-sixth";
char	strategy_sdseventh[]	=	"One-seventh";
char	strategy_sdeighth[]		=	"One-eighth";
char	strategy_sdninth[]		=	"One-ninth";
char	strategy_sdtenth[]		=	"One-tenth";

char	strategy_noselectedstock[]	=	"there is no selected stock.";
char	strategy_noselectedtech[]	=	"there is no selected indicator.";

char	strategy_optype_buy[]		=	"Buy";
char	strategy_optype_sell[]		=	"Sell";
char	strategy_optype_addstock[]	=	"Add Stock";
char	strategy_optype_removestock[]	=	"Remove Stock";
char	strategy_optype_addcash[]		=	"Add Cash";
char	strategy_optype_removecash[]	=	"Reduce Cash";

char	strategy_errfile[]			=	"File type is not coincident, the file is not strategy file.";
char	strategy_errfilever[]		=	"File version is not supported, please upgrade this software.";

#else

char	strategy_logicand[]		=	"ȫ��";
char	strategy_logicor[]		=	"����һ��";

char	strategy_sdonce[]		=	"ȫ��";
char	strategy_sdtwice[]		=	"���";
char	strategy_sdthird[]		=	"����֮һ��";
char	strategy_sdforth[]		=	"�ķ�֮һ��";
char	strategy_sdfifth[]		=	"���֮һ��";
char	strategy_sdsixth[]		=	"����֮һ��";
char	strategy_sdseventh[]	=	"�߷�֮һ��";
char	strategy_sdeighth[]		=	"�˷�֮һ��";
char	strategy_sdninth[]		=	"�ŷ�֮һ��";
char	strategy_sdtenth[]		=	"ʮ��֮һ��";

char	strategy_noselectedstock[]	=	"û��ѡ�й�Ʊ";
char	strategy_noselectedtech[]	=	"û��ѡ��ָ��";

char	strategy_optype_buy[]		=	"����";
char	strategy_optype_sell[]		=	"����";
char	strategy_optype_addstock[]	=	"���ӹ�Ʊ";
char	strategy_optype_removestock[]	=	"���ٹ�Ʊ";
char	strategy_optype_addcash[]		=	"�����ֽ�";
char	strategy_optype_removecash[]	=	"�����ֽ�";

char	strategy_errfile[]			=	"�ļ����Ͳ�����ָ���ļ����ǲ����ļ���";
char	strategy_errfilever[]		=	"������ļ��汾��������������Ա���Զ�ȡ�߰汾�Ĳ����ļ���";

#endif


//////////////////////////////////////////////////////////////////////////////////
// ��ݼ�
// ������ݼ�Ψһ��ţ����ؿ�ݼ�������������
CSPString	AfxGetAccelerator( UINT nAcce, int nCharLeft )
{
	static	CSPString	acce_names[ACCE_MAX+1];
	SP_ASSERT( ACCE_MAX >= 0 );
	
	if( acce_names[0].GetLength() <= 0 )
	{
		// acce_names
		acce_names[0]			=	"ACCENames";

#ifdef	CLKLAN_ENGLISH_US
		// ��ݼ�
		acce_names[ACCE_01]		=	"01      Hot Key F1";
		acce_names[ACCE_02]		=	"02      Hot Key F2";
		acce_names[ACCE_03]		=	"03      Hot Key F3";
		acce_names[ACCE_04]		=	"04      Hot Key F4";
		acce_names[ACCE_05]		=	"05      Hot Key F5";
		acce_names[ACCE_06]		=	"06      Hot Key F6";
		acce_names[ACCE_07]		=	"07      Hot Key F7";
		acce_names[ACCE_08]		=	"08      Hot Key F8";
		acce_names[ACCE_09]		=	"09      Hot Key F9";
		acce_names[ACCE_10]		=	"10      Hot Key F10";
		acce_names[ACCE_1]		=	"1       Shanghai A Shares List";
		acce_names[ACCE_2]		=	"2       Shanghai B Shares List";
		acce_names[ACCE_3]		=	"3       Shenzhen A Shares List";
		acce_names[ACCE_4]		=	"4       Shenzhen B Shares List";
		acce_names[ACCE_5]		=	"5       Shanghai Bond List";
		acce_names[ACCE_6]		=	"6       Shenzhen Bond List";
		acce_names[ACCE_61]		=	"61      Shanghai A Shares Change";
		acce_names[ACCE_62]		=	"62      Shanghai B Shares Change";
		acce_names[ACCE_63]		=	"63      Shenzhen A Shares Change";
		acce_names[ACCE_64]		=	"64      Shenzhen B Shares Change";
		acce_names[ACCE_65]		=	"65      Shanghai Bond Change";
		acce_names[ACCE_66]		=	"66      Shenzhen Bond Change";
		acce_names[ACCE_71]		=	"71      Shanghai Info.";
		acce_names[ACCE_72]		=	"72      Shenzhen Info.";
		acce_names[ACCE_73]		=	"73      Stockjobber Info.";
		acce_names[ACCE_81]		=	"81      Shanghai A Shares Sort";
		acce_names[ACCE_82]		=	"82      Shanghai B Shares Sort";
		acce_names[ACCE_83]		=	"83      Shenzhen A Shares Sort";
		acce_names[ACCE_84]		=	"84      Shenzhen B Shares Sort";
		acce_names[ACCE_85]		=	"85      Shanghai Bond Sort";
		acce_names[ACCE_86]		=	"86      Shenzhen Bond Sort";
		acce_names[ACCE_LBDK]	=	"LBDK    Volume Ratio";
		acce_names[ACCE_MMLD]	=	"MMLD    Consign Ratio";
#else
		// ��ݼ�
		acce_names[ACCE_01]		=	"01      �ȼ�F1";
		acce_names[ACCE_02]		=	"02      �ȼ�F2";
		acce_names[ACCE_03]		=	"03      �ȼ�F3";
		acce_names[ACCE_04]		=	"04      �ȼ�F4";
		acce_names[ACCE_05]		=	"05      �ȼ�F5";
		acce_names[ACCE_06]		=	"06      �ȼ�F6";
		acce_names[ACCE_07]		=	"07      �ȼ�F7";
		acce_names[ACCE_08]		=	"08      �ȼ�F8";
		acce_names[ACCE_09]		=	"09      �ȼ�F9";
		acce_names[ACCE_10]		=	"10      �ȼ�F10";
		acce_names[ACCE_1]		=	"1       ��A����";
		acce_names[ACCE_2]		=	"2       ��B����";
		acce_names[ACCE_3]		=	"3       ��A����";
		acce_names[ACCE_4]		=	"4       ��B����";
		acce_names[ACCE_5]		=	"5       ��ծ����";
		acce_names[ACCE_6]		=	"6       ��ծ����";
		acce_names[ACCE_61]		=	"61      ��A�ǵ�";
		acce_names[ACCE_62]		=	"62      ��B�ǵ�";
		acce_names[ACCE_63]		=	"63      ��A�ǵ�";
		acce_names[ACCE_64]		=	"64      ��B�ǵ�";
		acce_names[ACCE_65]		=	"65      ��ծ�ǵ�";
		acce_names[ACCE_66]		=	"66      ��ծ�ǵ�";
		acce_names[ACCE_71]		=	"71      ��֤��Ϣ";
		acce_names[ACCE_72]		=	"72      ��֤��Ϣ";
		acce_names[ACCE_73]		=	"73      ȯ����Ϣ";
		acce_names[ACCE_81]		=	"81      ��A�ۺ�";
		acce_names[ACCE_82]		=	"82      ��B�ۺ�";
		acce_names[ACCE_83]		=	"83      ��A�ۺ�";
		acce_names[ACCE_84]		=	"84      ��B�ۺ�";
		acce_names[ACCE_85]		=	"85      ��ծ�ۺ�";
		acce_names[ACCE_86]		=	"86      ��ծ�ۺ�";
		acce_names[ACCE_LBDK]	=	"LBDK    ���ȣ���գ�";
		acce_names[ACCE_MMLD]	=	"MMLD    �����������ǵ��ʣ�";
#endif
	}
	
	if( nAcce >= ACCE_MIN && nAcce <= ACCE_MAX )
	{
		if( 8 == nCharLeft )
			return	acce_names[nAcce];
		
		CSPString	strResult	=	acce_names[nAcce];
		int nIndex = strResult.Find( ' ' );
		if( -1 != nIndex )
			strResult	=	strResult.Left(nIndex) + strResult.Mid(8);
		return strResult;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// ���һ����

void AfxGetAllDistrict( CSPStringArray & astr )
{
#ifdef	CLKLAN_ENGLISH_US
	astr.SetSize( 0, 30 );
	astr.Add( "Australia & NZ" );
	astr.Add( "Argentina" );
	astr.Add( "Brazil" );
	astr.Add( "Canada" );
	astr.Add( "China" );
	astr.Add( "Denmark" );
	astr.Add( "France" );
	astr.Add( "Germany" );
	astr.Add( "Hong Kong" );
	astr.Add( "India" );
	astr.Add( "Italy" );
	astr.Add( "Japan" );
	astr.Add( "Korea" );
	astr.Add( "Mexico" );
	astr.Add( "Norway" );
	astr.Add( "Singapore" );
	astr.Add( "Spain" );
	astr.Add( "Sweden" );
	astr.Add( "Taiwan" );
	astr.Add( "UK & Ireland" );
	astr.Add( "U.S." );
	astr.Add( "Other" );
#else
	astr.SetSize( 0, 30 );
	astr.Add( "�Ĵ�����" );
	astr.Add( "����͢" );
	astr.Add( "����" );
	astr.Add( "���ô�" );
	astr.Add( "�й�" );
	astr.Add( "����" );
	astr.Add( "����" );
	astr.Add( "�¹�" );
	astr.Add( "�й����" );
	astr.Add( "ӡ��" );
	astr.Add( "�����" );
	astr.Add( "�ձ�" );
	astr.Add( "����" );
	astr.Add( "ī����" );
	astr.Add( "Ų��" );
	astr.Add( "�¼���" );
	astr.Add( "������" );
	astr.Add( "���" );
	astr.Add( "�й�̨��" );
	astr.Add( "Ӣ��" );
	astr.Add( "����" );
	astr.Add( "����" );
#endif
}


//////////////////////////////////////////////////////////////////////////////////
// �򵥷���
#define	CHS_ENU_MAX_WORDS	32
typedef	struct	chs_enu_pair_t	{
	char	chs[CHS_ENU_MAX_WORDS];
	char	enu[CHS_ENU_MAX_WORDS];
}CHS_ENU_PAIR;

CSPString	AfxFaintlyChsToEnu( const char * szCh )
{
	static	CHS_ENU_PAIR	chs_enu_pairs[]	=	{
		{ "�й�",	" China " },
		{ "ָ��",	" Index " },
		{ "��֤",	" Shanghai " },
		{ "��֤",	" Shenzhen " },
		{ "����",	" Fund " },
		{ "��ծ",	" Bond " },
		{ "ծȯ",	" Bond " },
		{ "תծ",	" Convertible Bond " },
		{ "����",	" Bank " },
		{ "����",	" Other " },
		{ "����ҵ",	" Bank " },
		{ "",	"" },
	};
	
	CSPString	sResult = szCh;
	int i = 0;
	while( * chs_enu_pairs[i].chs )
	{
		sResult.Replace( chs_enu_pairs[i].chs, chs_enu_pairs[i].enu );
		i ++;
	}
	sResult.TrimLeft();
	sResult.TrimRight();

	return sResult;
}

