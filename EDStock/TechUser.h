/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechUser
*/

#if	!defined( __STKLIB_TECHUSER_H__ )
#define	__STKLIB_TECHUSER_H__

#ifndef TECHUSER_API
#define	TECHUSER_API	__declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#pragma pack(1)

typedef struct techuser_info_t {
	int		m_nStructSize;		// ���ֽ�Ϊ��λ��ʾ�ı��ṹ��С
	char	m_szName[64];		// ָ�����ƣ���'\0'Ϊ��β�����磺���ָ��
	char	m_szShortName[16];	// ָ���ƣ���'\0'Ϊ��β�����磺KDJ
	
	int		m_nLineCount;		// ָ���߸����������߸��ݸñ���ȷ�����÷��صļ�������ֵ�Ƿ���Ч
								// ���磺m_nLineCount = 3���� m_dValue1, m_dValue2, m_dValue3��Ч
								// m_nLineCount Ӧ����1��5֮��
	char	m_aszLineName[5][8];// ÿ���ߵ����ƣ�ǰm_nLineCount����Ч��ע��ÿ������Ӧ��'\0'Ϊ��β��
								// �������Ʋ��ܳ���7���ֽ�
	
	int		m_bAutoMA;			// ��m_nLineCount = 1 ʱ����ֵ��Ч��m_bAutoMA = true ʱ���Զ�����
								// ƽ����
	int		m_nMADays;			// �Զ�����ƽ����ʱ�ľ�������

	int		m_itsGoldenFork;	// ���ָ�� m_nLineCount = 2 ���ߡ�m_nLineCount = 1���Զ�����ƽ��
								// ��ʱ����λ��淢���������ź�
	int		m_itsDeadFork;		// ����ͬ�ϣ���λ����ʱ�������ź�
	
	/* ��ע������ m_szName, m_szShortName, m_nLineCount���븳ֵ */
} TECHUSER_INFO, *PTECHUSER_INFO;

typedef struct calculate_info_t {
	int		m_nStructSize;		// ���ֽ�Ϊ��λ��ʾ�ı��ṹ��С
	CStock *	m_pStock;		// CStockָ�룬����ȫ����Ʊ����
	CStockInfo *m_pStockInfo;	// CStockInfoָ�룬������Ʊ������Ϣ���ݣ���ָ��ʵ��ָ��
								// m_pStock->m_stockinfo
	CKData *	m_pKData;		// CKDataָ�룬����Ҫ�����K ���������飬��ָ��ʵ��ָ��m_pStock��
								// ��ӦK �����ڳ�Ա������K �������Լ��Ƿ�Ȩ���Ѿ�������ϣ�ֱ��
								// ʹ�ü���
	int			m_nIndex;		// ָ��ǰҪ����ָ���K ��λ�õ���ţ�����pKData�е����
	int		m_bUseLast;			// ��ǰֵ�Ƿ�Ϊ�ϴμ�������ֵ�����m_bUseLast=TRUE����m_dValue1,
								// m_dValue2,...Ϊ�ϴε��ø�ָ��ļ�������ֵ

	// ����Ϊ���㷵��ֵ
	double	m_dValue1;			// ��������ָ����1ֵ
	double	m_dValue2;			// ��������ָ����2ֵ
	double	m_dValue3;			// ��������ָ����3ֵ
	double	m_dValue4;			// ��������ָ����4ֵ
	double	m_dValue5;			// ��������ָ����5ֵ
	int		m_nSignal;			// �����źţ�����ITS_MIN��ITS_MAX֮��
} CALCULATE_INFO, *PCALCULATE_INFO;

#pragma pack()

#ifndef	ITS_MIN
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
#endif

TECHUSER_API UINT WINAPI GetTechUserCount();
TECHUSER_API BOOL WINAPI GetTechUserInfo( UINT nID, PTECHUSER_INFO pInfo );
TECHUSER_API BOOL WINAPI Calculate( UINT nID, PCALCULATE_INFO pInfo );
TECHUSER_API BOOL WINAPI GetSignal( UINT nID, PCALCULATE_INFO pInfo );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif	//__STKLIB_TECHUSER_H__
