/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CExpress;
		class	CIndex;
		class	CIndexContainer;
*/

#if	!defined( __STKLIB_EXPRESS_H__ )
#define	__STKLIB_EXPRESS_H__

#include "MathParser.h"
#include "Container.h"

/***
	�����Ʊ�б����Ψһ��ʶ
*/
	//	��Ʊ
#define	SLH_NONE					0
#define	SLH_MIN						1
#define	SLH_CODE					1	//	��Ʊ����
#define	SLH_NAME					2	//	��Ʊ����
	//	��������
#define	SLH_DATE					3	//	����
#define	SLH_LASTCLOSE				4	//	����
#define	SLH_OPEN					5	//	��
#define	SLH_CLOSE					6	//	���̼�
#define	SLH_HIGH					7	//	��߼�
#define	SLH_LOW						8	//	��ͼ�
#define	SLH_AVERAGE					9	//	����
#define	SLH_DIFF					10	//	�ǵ�
#define	SLH_DIFFPERCENT				11	//	�Ƿ�%
#define	SLH_SCOPE					12	//	���%
#define	SLH_VOLUME					13	//	�ɽ���
#define	SLH_AMOUNT					14	//	�ɽ���
#define	SLH_VOLUP					15	//	����
#define	SLH_VOLDOWN					16	//	����
#define	SLH_DIFFPERCENT_MIN5		17	//	������Ƿ�%
#define	SLH_SELLBUYRATIO			18	//	ί��%
#define	SLH_SELLBUYDIFF				19	//	ί����
#define	SLH_BUYPRICE3				20	//	�����
#define	SLH_BUYPRICE2				21	//	��۶�
#define	SLH_BUYPRICE1				22	//	���һ
#define	SLH_SELLPRICE1				23	//	����һ
#define	SLH_SELLPRICE2				24	//	���۶�
#define	SLH_SELLPRICE3				25	//	������
#define	SLH_BUYVOLUME3				26	//	������
#define	SLH_BUYVOLUME2				27	//	������
#define	SLH_BUYVOLUME1				28	//	����һ
#define	SLH_SELLVOLUME1				29	//	����һ
#define	SLH_SELLVOLUME2				30	//	������
#define	SLH_SELLVOLUME3				31	//	������

	//	��Ҫ����ָ��
#define	SLH_REPORTTYPE				32	//	��������
#define	SLH_PE						33	//	��ӯ��
#define	SLH_PNETASSET				34	//	�о���
#define	SLH_PMAININCOME				35	//	������
#define	SLH_RATIO_PCASH				36	//	�۸��ֽ��
#define	SLH_RATIO_CURRENCY			37	//	��ͨ��
#define	SLH_RATIO_CHANGEHAND		38	//	������
#define	SLH_RATIO_VOLUME			39	//	����
#define	SLH_RS						40	//	���ǿ��
#define	SLH_MARKETVALUE				41	//	����ֵ
#define	SLH_MARKETVALUEA			42	//	A����ֵ
#define	SLH_MARKETVALUEB			43	//	B����ֵ
	// �ﳥծ����
#define	SLH_RATIO_LIQUIDITY			44	//	��������
#define	SLH_RATIO_QUICK				45	//	�ٶ�����
#define	SLH_VELOCITY_RECEIVABLES	46	//	Ӧ���ʿ�����
	// �ﾭӪ����
#define	SLH_VELOCITY_MERCHANDISE	47	// �����ת��
#define	SLH_MAIN_INCOME				48	// ��Ӫҵ������
#define	SLH_CASH_PS					49	// ÿ�ɾ��ֽ�����
	// ��ӯ������
#define	SLH_PROFIT_MARGIN			50	// ��Ӫҵ��������
#define	SLH_NETASSET_YIELD			51	// ���ʲ�������
	// ���ʱ��ṹ
#define	SLH_DATE_BEGIN				52	//	��������
#define	SLH_SHARE_COUNT_TOTAL		53	//	�ܹɱ�
#define	SLH_SHARE_COUNT_A			54	//	��ͨA��
#define	SLH_SHARE_COUNT_B			55	//	��ͨB��
#define	SLH_SHARE_COUNT_H			56	//	��ͨH��
#define	SLH_SHARE_COUNT_NATIONAL	57	//	���й�
#define	SLH_SHARE_COUNT_CORP		58	//	���˹�
#define	SLH_PROFIT_PSUD				59	// ÿ��δ��������
#define	SLH_ASSET					60	// ���ʲ�
#define	SLH_RATIO_HOLDERRIGHT		61	// �ɶ�Ȩ�����
#define	SLH_RATIO_LONGDEBT			62	// ���ڸ�ծ��
#define	SLH_RATIO_DEBT				63	// �ʲ���ծ��
	// ��Ͷ����������
#define	SLH_NETASSET_PS				64	// ÿ�ɾ��ʲ�
#define	SLH_NETASSET_PS_REGULATE	65	// ����ÿ�ɾ��ʲ�
#define	SLH_EPS						66	// ÿ������
#define	SLH_EPS_DEDUCT				67	// �۳���ÿ������
#define	SLH_NET_PROFIT				68	// ������
#define	SLH_MAIN_PROFIT				69	// ��Ӫҵ������
#define	SLH_TOTAL_PROFIT			70	// �����ܶ�
#define	SLH_PROFIT_INC				71	// ��Ӫ����������
#define	SLH_INCOME_INC				72	// ��Ӫ����������
#define	SLH_ASSET_INC				73	// ���ʲ�������
#define	SLH_ANNGAINS_AVERAGE		74	// ��ƽ��������
#define	SLH_ANNGAINS_STDDEV			75	// �����׼��
#define	SLH_BETA					76	// ��ֵ
#define	SLH_SHARP					77	// ���շ���ָ��
#define	SLH_TRADE					78	// ��ҵ
#define	SLH_PROVINCE				79	// ʡ��
#define	SLH_MAX						79

#define	SLH_INVALID					(UINT(-1))
#define	SLH_USERDEFINE_BEGIN		10000

#define	SLH_WIDTH_MIN				60


#define	SLMAX_SLHTEXTLEN			256

typedef	struct	slh_data_t	{
	UINT	slh_id;
	char	string[SLMAX_SLHTEXTLEN+1];
	UINT	weight;
	char	varname[MP_MaxVarNameLen];
}SLH_DATA;

extern	SLH_DATA	slh_data_array[SLH_MAX+1];

	CSPString	AfxGetVariantName( UINT nVariantID, BOOL bWithParameter );	// ���ݹ�Ʊ�б���ID�õ����ƣ�bWithParameter��ʾ�������Ƿ��в���
	BOOL		AfxGetVariantNameArray( CSPStringArray & astr, BOOL bWithParameter );	// �õ����й�Ʊ�б��������
	UINT		AfxGetVariantID( CSPString strName, BOOL bWithParameter );	// ���ݹ�Ʊ�б������Ƶõ�ID
	CSPString	AfxGetVariantVarName( int nVariantID );		// ���ݹ�Ʊ�б���ID�õ�ʹ���ڱ��ʽ�еı�������
	CSPString	AfxGetVariantVarName( CSPString strName, BOOL bWithParameter );		// ���ݹ�Ʊ�б������Ƶõ�ʹ���ڱ��ʽ�еı�������
	BOOL		AfxGetVariantValue(UINT nVariantID, CStockInfo &info, double * pValue,
							   CStockContainer *pContainer );	// �õ�ĳֻ��Ʊ����Ӧֵ����λ���Ʊ�б���ͼ����ͬ
	int			AfxCompareVariantValue(int nVariantID, CStockInfo & info1, CStockInfo &info2,
								   CStockContainer *pContainer );	// �Ƚ���ֻ��Ʊ����Ӧֵ
	CSPString	AfxGetVariantDispString(UINT nVariantID, CStockInfo &info,
									CStockContainer *pContainer );		// �õ�ĳֻ��Ʊ����Ӧֵ����ʾ�ִ�
	COLORREF	AfxGetVariantColor( UINT nVariantID, CStockInfo &info );	// �õ�ĳֻ��Ʊ����Ӧֵ����ʾ��ɫ

/***
	���ʽ�����԰�������
*/
class  CExpress  
{
public:
	CExpress();
	CExpress( const CExpress &src );
	virtual ~CExpress();

	CExpress & operator = ( const CExpress &src);
	virtual	void Serialize( CSPArchive &ar );

	BOOL		SetExpressString( CSPString strExpressOrg );
	CSPString	GetExpressString( );

	virtual	CSPString	GetLastErrorMsg( );
	BOOL	GetExpressResult( double * pResult, BOOL * pError );

protected:
	virtual	BOOL	PreCompile( );
	virtual	BOOL	InitParser( );

protected:
	CSPString	m_strExpressOrg;		//	Save to disk
	int			m_nReserved;			//	Save to disk, Not Used

	BOOL		m_bIsModified;
	CSPString	m_strExpress;
	CMathParser	m_parser;
};

/***
	�½�ָ�꣬�������б����ı��ʽ���ڹ�Ʊ�б�����ʾΪһ��
*/
class  CIndex : public CExpress
{
public:
	CIndex( );
	CIndex( const CIndex &src );
	~CIndex( );

	UINT			m_nID;
	CSPString		m_strName;
	CSPString		m_strDescript;
	CSPString		m_strReserved;

	CIndex	& operator = ( const CIndex &src );
	virtual	void	Serialize( CSPArchive & ar );
	BOOL	IsInvalidID( );
	BOOL	SetNextID( );

	virtual	BOOL	CheckExpressValid( );
	BOOL	GetExpressResult( double * pResult, CStockInfo &info, BOOL *pVariantNoValue, BOOL * pError );

protected:
	virtual	BOOL	PreCompile( );
	virtual	BOOL	InitParser( CStockInfo &info, BOOL *pVariantNoValue );

protected:
	CSPDWordArray	m_adwVariant;	// temp data
};

//template class __declspec(dllexport)	CSPArray<CIndex, CIndex &>;
typedef	CSPArray<CIndex, CIndex &>	CIndexArray;
/***
	�½�ָ������
*/
class  CIndexContainer : public CIndexArray
{
public:
	CIndexContainer( );
	virtual ~CIndexContainer( );

	// for access CIndexArray
	virtual	int	Add(CIndex &newElement)	{	return CIndexArray::Add(newElement);	}

	void		RemoveIndex( UINT nID );
	CIndex	&	GetIndex( UINT nID );
	CIndex	&	GetIndex( CSPString strName );
	BOOL		EditIndex( CIndex &index );

	UINT		GetNextID( );

	void		FileSerialize( CSPArchive & ar );
};


#endif // __STKLIB_EXPRESS_H__
