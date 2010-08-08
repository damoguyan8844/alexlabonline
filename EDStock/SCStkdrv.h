#ifndef _STOCK_DRV_SC_H_
#define _STOCK_DRV_SC_H_

#include "Stockdrv.h"

// �������ڣ�20060131
// �������ݣ�
// 1. tagSCPankou�����ʱ���Ϊlong���͡�
//    SC_PankouReply����ĸ���short m_nCount��Ϊlong m_lCount
// 2. SC_KLineAsk��SC_KLineReply��������
// 3. �����г����壬���塢��㡢�ڻ���̨���г���ʹ��SCMarket���ص�m_wMarketֵ���ɡ�
// 4. �󲿷��������͵ĵ�#defineֵ�и��ģ����±��뼴��
// 5. SC_Downdload�е�m_nCount��Ϊm_lCount�����͸�Ϊlong. 
//    �ṹ���ƴ�'SC_Downdload'��Ϊ'SC_Download'
// 6. SH_MARKET_EX��SZ_MARKET_EX�ȵĶ�����ɾ����ֱ����SCMarket�����m_wMarket���ɡ�
// 7. ��ʼ����������ΪSCInit
// 8. ʾ�����������ϸ����ο����ǵ����ӣ�SCDemo

#define	STKLABEL_LEN				10		// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define	STKNAME_LEN					32		// ��������
#define	TITLE_LEN					64		// ��Ϣ���ױ��ⳤ��

#define SC_INIT						0x2001	// SCInit����

#define SC_CLEAR_PREVREQ			0x2101	// ����ǰ����������󣬽����ڷ����󣨻��������֮ǰ����
#define SC_MARKET					0x2131	// ����ָ���г�����Ϣ�����������б仯�ͻ��Զ�����

#define SC_NOWDATA					0x2201

#define SC_PANKOU					0x2301	// �����̿����ݣ���Ӧ��long m_time������ʹ�������

#define SC_IMTYPELIST				0x2401	// ������Ϣ������Ϣ�����
#define SC_IMTITLE					0x2402	// ������Ϣ���ױ����б�
#define SC_IMCONTENT				0x2403	// ������Ϣ��������(f10, ���������桢ʵʱ����ʷ�ĸ�����Ϣ��)

#define SC_KLINE					0x2501	// ����K�����ݣ����ߣ�

#define SC_CQ						0x2601	// �����Ȩ����

#define SC_DL_KLINE					0x2701	// K����������
#define SC_DL_CQ					0x2702	// ��Ȩ��������
#define SC_DL_XX					0x2703	// ��Ϣ���أ���F10�����������桢���ŵ�
#define SC_DL_TRACE					0x2704	// ��ϸ���ݣ��ɵģ��°��Ѳ���
#define SC_DL_5MIN					0x2705	// 5����K������


#pragma pack(1)

struct SCHead {								// �����ͷ
	WORD		m_nType;					// ����/Ӧ�����ͣ�SC_MARKET��SC_IMCONTENT��
	char		m_cStatus;					// ��ΪӦ���ʱ��m_cStatus=1�ɹ���m_cStatus=0ʧ�ܣ�m_cStatus=2����
	long		m_lIndex;					// ����Ψһ��������Ҫ��Ϊ0�������ʱ��ֵ������Ӧ�����
											// �ж��Ƿ���������е���ͬ�������ͬ�����ʾ��������������Ӧ��Ӧ���
											// ������ֵΪ0����ʾ����������������ݱ仯
};

typedef struct tagStock {					// ֤ȯ
	char	m_szLabel[STKLABEL_LEN];		// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];			// ��Ʊ����,��'\0'��β
	short	m_nHand;						// ÿ�ֹ���
} StockInfo;

typedef struct tagMarket {					//�г�����
	union {
		WORD			m_wMarket;			//�г�����, 'HS', 'SZ', 'BS', 'HW', 'KH', 'ZH', 'QS', 'QD', 'QZ', 'WT',
		char			m_EShortName[3];	//Ӣ�ļ�д, "SH", "SZ", "SB", "WH", "HK", "HZ", .....
	};
	char			m_Name[16];				//�г�����
	char			m_CShortName[5];		//���ļ��
	unsigned long	m_lProperty;			//�г����ԣ�δ���壩
	unsigned long	m_lDate;				//�������ڣ�20030114��
	short			m_PeriodCount;			//����ʱ�θ���
	short			m_OpenTime[5];			//����ʱ�� 1,2,3,4,5
	short			m_CloseTime[5];			//����ʱ�� 1,2,3,4,5
	short			m_nCount;				//���г���֤ȯ����
	StockInfo       m_Siif[1];   
} SCMarket;

typedef struct tagStockID {					//֤ȯ��ʶ
	WORD			m_wMarket;				//�г�����
	short			m_nIndex;				//�ڸ��г�SCMarket::m_Siff�е�ƫ��
} StockID;


typedef struct tagSCNow
{
	StockID m_sID;
	long	m_time;							// �ɽ�ʱ��

	float	m_fLastClose;					// ����
	float	m_fOpen;						// ��
	float	m_fHigh;						// ���
	float	m_fLow;							// ���
	float	m_fNewPrice;					// ����
	float	m_fVolume;						// �ɽ���
	float	m_fAmount;						// �ɽ���

	long	m_lStroke;						// ����ʵ�ʳɽ�����
	float	m_fBuyPrice[5];					// �����1,2,3,4,5
	float	m_fBuyVolume[5];				// ������1,2,3,4,5
	float	m_fSellPrice[5];				// ������1,2,3,4,5
	float	m_fSellVolume[5];				// ������1,2,3,4,5
} RCV_NOW_STRUCTEx;

typedef struct tagSCPankou {				
	long	m_time;							// UCT

	float	m_fHigh;						// ���
	float	m_fLow;							// ���
	float	m_fNewPrice;					// ����
	float	m_fVolume;						// �ɽ���
	float	m_fAmount;						// �ɽ���

	long	m_lStroke;						// ����ʵ�ʳɽ�����
	float	m_fBuyPrice[5];					// �����1,2,3
	float	m_fBuyVolume[5];				// ������1,2,3��
	float	m_fSellPrice[5];				// ������1,2,3
	float	m_fSellVolume[5];				// ������1,2,3
} RCV_PANKOU_STRUCTEx;

/*
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		long	m_time;				//UCT
		float	m_fOpen;			//����
		float	m_fHigh;			//���
		float	m_fLow;				//���
		float	m_fClose;			//����
		float	m_fVolume;			//��
		float	m_fAmount;			//��
		WORD	m_wAdvance;			//����,��������Ч
		WORD	m_wDecline;			//����,��������Ч
	};
} RCV_HISTORY_STRUCTEx;

typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		long	m_time;				// UCT
		float	m_fGive;			// ÿ����
		float	m_fPei;				// ÿ����
		float	m_fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
		float	m_fProfit;			// ÿ�ɺ���
	};
}RCV_POWER_STRUCTEx,*PRCV_POWER_STRUCTEx;
*/

///////////////////////////////////////////////////////////////////////////////////
// �����ʼ����
struct SC_InitAsk {							// ������ʼ�����󣬵���SCInitʱʹ�ã��޶�Ӧ���ذ�
	SCHead				m_Head;
	DWORD				m_dwSoftware;		// ���������ʶ������������
	HWND				m_hWnd;				// �����������ݵĴ��ھ��
	DWORD				m_nMsg;				// �����������ݵ���Ϣ
};


///////////////////////////////////////////////////////////////////////////////////
//�������Ӧ���
struct SC_MarketReply {		// ������������󼴿��յ������г���Ϣ�仯��ʱ��Ϳ��յ�
	SCHead				m_Head;
	SCMarket			m_Market;
};

struct SC_NowDataReply {	// ������������󼴿��յ��������и��£����Զ�����
	SCHead				m_Head;
	short				m_nCount;
	RCV_NOW_STRUCTEx	m_Now[1];
};

struct SC_PankouAsk {				// ����һ���̿����ݣ���֧�ֵ��죬��ʷ�̿ڽ�ͨ�����ػ��
	SCHead				m_Head;
	StockID				m_sID;
	long				m_lDate;	// FORMAT: 20010305
};

struct SC_PankouReply {
	SCHead				m_Head;
	StockID				m_sID;
	long				m_lDate;			// FORMAT: 20010305
	float				m_fLastClose;		// ����
	float				m_fOpen;			// ��
	long				m_lCount;
	RCV_PANKOU_STRUCTEx	m_Data[1];		
};

//define for SC_KLineAsk::m_wDataType
#define KLINE_DAYDATA			0		//����
#define KLINE_MIN5DATA			1		//5������

// ע�⣺
// 1�����ڷ����������ã�����ֻ���յ����ĳһʱ�ε����ݣ���������ݣ�����ͨ�����ػ�á�
// 2������K�������������ʾ����ʱ��long�ĸ�ʽΪyyyymmdd��
// 3������K�����������Ϊ�������ʱ��long�ĸ�ʽ���������TDate�ṹ��ʾ
//	struct TDate {		// �����ʱ���ʽ
//		unsigned long m_Minute : 6;
//		unsigned long m_Hour : 5;
//		unsigned long m_Day  : 5;
//		unsigned long m_Month : 4; 
//		unsigned long m_Year : 12;
//	};
// 4������ʱ���ת�У������ṩ��������ʾ�����룬�ɲο���
// 5����K��Ӧ��������ߺ��޷��ӵ�ʱ��ͳһΪlong��ʽ���Ѿ��������ںͷ�����Ϣ��

struct SC_KLineAsk {
	SCHead				m_Head;
	StockID				m_sID;
	WORD				m_wDataType;		// KLINE_DAYDATA or KLINE_MIN5DATA
	long				m_tBegin;			// ��ʼʱ���λ�ã�0��ʾ�ӵ�һ�����ݿ�ʼ
	long				m_tEnd;				// ����ʱ�������������0��ʾֱ������
};

struct SC_KLineReply {
	SCHead				m_Head;
	StockID				m_sID;
	WORD				m_wDataType;		// KLINE_DAYDATA or KLINE_MIN5DATA
	long				m_lDataCount;		// m_pData����
	RCV_HISTORY_STRUCTEx m_pData[1];		
};

// ��Ȩ���ݲ�У�飬�����Է�������Ϊ׼
struct SC_CQAsk {
	SCHead				m_Head;
	StockID				m_sID;
};

struct SC_CQReply {
	SCHead				m_Head;
	StockID				m_sID;
	short				m_nCount;
	RCV_POWER_STRUCTEx	m_pData[1];			// m_head��ʹ��
};

//////////////////////////////////////////////////////////////////////////////////
//////////////  ��Ϣ���׽ӿ�
//��Ϣ�����������ԵĶ���
#define CLASS_JYSXX			0		// ����������
#define CLASS_STKINFO       1		// ��ʷ��Ϣ����
#define CLASS_NEWS          2		// ����
#define CLASS_STOCKBASE     3		// F10
#define CLASS_MKTINFO       4		// �ۺ���Ѷ

struct SCIMType {					// ��Ϣ���࣬���ش������˾���桢���ɵ�����
	char    m_cFlag[8];
	char    m_cTitle[24];
};

struct SC_IMTypeReply {				// ���������Զ�����
	SCHead			m_Head;
	char			m_cVendor;
	char            m_cClass;
	short			m_nSize;
	SCIMType		m_Type[1];
};

struct SCIMTitle {
	unsigned long  m_lID;			// ����ID
	long		   m_lChkSum;		// �����ӣ�����Ӧ�����ݵ�Checksum
	unsigned long  m_lDate;			// yyyymmdd
	unsigned long  m_lTime;			// hhmmss
	char           m_cFlag[8];
	char		   m_cMainType[9];
	char           m_cSubtype[17];
	char		   m_cTitle[64];
	char		   m_szLabel[9];	// ��Ʊ����,��'\0'��β
};

struct SCIMTitleID {
	unsigned long  m_lID;			// ����ID
	long		   m_lChkSum;		// �����ӣ�����Ӧ�����ݵ�Checksum
};

struct SC_IMTitleAsk {				// ����ĳ��ʱ�������Ϣ���ױ���
	SCHead			m_Head;
	char			m_cClass;
	StockID			m_sID;			//���ڸ�����Ѷ����������F10ʹ��m_sID������������ֻʹ��
	                                //���е��г������š��ۺ���Ѷ��ʹ������ֶ�
	long			m_lStartDate;	//���š��ۺ���Ѷ������������ֻʹ��m_lStartDate��m_lEndDate��ʹ��
	long			m_lEndDate;		//������Ѷʹ��m_lStartDate��m_lEndDate��
};

struct SC_IMTitleReply { 
	SCHead			m_Head;
	char			m_cVendor;		// Ӧ����г���
	char			m_cClass;
	StockID			m_sID;				//���ڸ�����Ѷ����������F10��ʹ��m_sID,
	short           m_nSize;			// �������
	SCIMTitle		m_IMTitle[1];
};

struct SC_IMDataAsk {					// ��Ϣ������������һ�ο����������������
	SCHead			m_Head;
//	char			m_cVendor;			// �����û�г��̴���
	char			m_cClass;
	StockID			m_sID;				//���ڸ�����Ѷ���������ϡ�������Ϣ����ʹ��m_sID,
	short			m_nSize;			// ����ID����
	long			m_lDate;			//���ڣ����š����������桢�ۺ���Ѷ��Ҫ��һ��ֻ����һ������
	SCIMTitleID		m_ID[1];
};

struct SCIMData {						// ��Ϣ��������
	SCIMTitle		m_IMTitle;
	unsigned long   m_lDataLength;
	char		    m_cData[1];
};

struct SC_IMDataReply {					// ��Ϣ��������Ӧ��һ�οɷ��ض�����������
	SCHead			m_Head;
	char			m_cVendor;
	char			m_cClass;
	StockID			m_sID;				//���ڸ�����Ѷ����������F10ʹ��m_sID,
	long			m_lDate;			//�������š��ۺ���Ѷ���ƾ����š�ȯ����Ϣʹ��m_lDate
	short			m_nSize;
	SCIMData		m_Data[1];
};

typedef struct tagIMTitle {
	unsigned long	m_lOffset;
	unsigned long	m_lDataLength;
	long			m_time;
	char			m_cCaption[TITLE_LEN];
} IMTitleEx;

// ����������������ز���֮�󣬻��Զ��յ���������������������������ָ�����ݡ�
struct SC_Download {						// �������ؽӿ�
	SCHead					m_Head;			// m_head�н�m_nType��Ч
	StockID					m_sID;
	long					m_lCount;		// // m_KLine,m_power,m_Trace,m_Title�ĸ���
	union {
		RCV_HISTORY_STRUCTEx	m_KLine[1]; // k�ߡ��������������
		RCV_POWER_STRUCTEx		m_power[1]; // ��Ȩ��������
		RCV_PANKOU_STRUCTEx		m_Trace[1];	// ��ϸ��������
		struct {							// F10�����������桢���š��ۺ���Ѷ����
			char			m_cClass;		// ���
			IMTitleEx		m_Title[1];
			char			m_cData[1];
		};
	};
};

#pragma pack()

//////////////////////////////////////////////////////////////////////////////////
//�����ӿ�
#ifdef __cplusplus
extern "C"{
#endif

//�½ӿڵ�DLL��ʹ��HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv�µ�SCDriver��

// �ӿں���1
//****************************************************************************************\\
// ���ô�����Ϣ�Ĵ��ھ�����Զ������Ϣ
// SCAskData����������������Ϣ�з��ؽ��
// ���ǣ�������һ��Ҫ����SCAskData�������ݷ��أ���û�е���SCAskData������£���������������᷵������
// 1���г��仯�������ʼ�����¹����еȣ�������SC_MarketReply����
// 2������ʱ���ڣ�ʵʱ���ݲ��ϱ仯������SC_NowReply����
// 3��������������������ز�������������������ָ������SC_Downdload
// ���������ͨ������SCAskData������ָ�����͵����ݡ�
BOOL WINAPI SCInit(void *pInit, int nSize); // SC_InitAsk, sizeof(SC_InitAsk)
//****************************************************************************************//

// �ӿں���2
//****************************************************************************************\\
// �˳���ʱ�����������������ͷŵ���SCInitʱ�������Դ��
void WINAPI SCQuit();
//****************************************************************************************//


//  �ӿں���3
//****************************************************************************************\\
// �������Ϣƽ̨�������ݣ�
// nSizeΪ������Ĵ�С
// pAsk�ֱ�Ϊ SC_IMVendorAsk *, SC_TraceAsk *, SC_NowAsk *, SC_IMAsk * ��
// �����ں���SCInit�������Ϣ�з��أ��ɲο�CSCDemoDlg::OnReply(...)
// ��Ϣ�е����ݿ�����SCAskData֮�󷵻أ�Ҳ������SCAskData��δ����֮ǰ����
// �û�ͨ������AskData����Ϣƽ̨���ܴӱ��ط������ݣ�Ҳ���ܰ��û�������ת�������ϵķ�����
// �ٷ������ݣ����û���˵��Ч�����÷�����һ���ģ��ɲο�CSCDemoDlg::OnReply(...)��
void WINAPI SCAskData(void *pAsk, int nSize);
//****************************************************************************************//


//  �ӿں���4
//****************************************************************************************\\
// ȡϵͳ��Ϣ
enum TSCInfo { 
	INFO_VERSION = 1,
	INFO_USERNAME = 2,
};
UINT WINAPI SCGetInfo(TSCInfo Info, void *pData, int *nSize);
// Info=INFO_VERSIONʱ��pData����ΪNULL����������ֵΪ����汾��
// Info=INFO_USERNAMEʱ��pDataָ��һ���ַ�����
//                       ����ֵΪ0��ʾδ��¼��pDataָ�������δ����
//                       ����ֵΪ1��ʾ�ѳɹ���¼��pDataָ�������Ϊ�û���
//****************************************************************************************\\



#ifdef __cplusplus
}
#endif


#endif // _STOCK_DRV_SC_H_