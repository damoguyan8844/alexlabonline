
#include "QianLong.h"

#pragma	pack(1)

/* 2.0�������Ϣ���ݽṹ dat\basetbl.dat

typedef struct clk20_basedata_t {
	LONG	type;								//	stock type
	char	code[8];							// ����
	char	domain[16+1];						// ���
	char	province[8+1];						// ʡ��

	DWORD	date;				 	// ����	Format is YYYYMMDD for base data
	LONG	reporttype;				// �������ͣ��걨���б�������
	LONG	erate_dollar;			// ������Ԫ���� 0.001
	LONG	erate_hkdollar;			// ���ڸ۱һ��� 0.001

	// �ﳥծ����
	LONG	ratio_liquidity;		// �������� 0.001
	LONG	ratio_quick;			// �ٶ����� 0.001
	LONG	velocity_receivables;	// Ӧ���ʿ����� 0.001
	// �ﾭӪ����
	LONG	velocity_merchandise;	// �����ת�� 0.001
	LONG	main_income;			// ��Ӫҵ������ 10000
	LONG	cash_ps;				// ÿ�ɾ��ֽ����� 0.001
	// ��ӯ������
	LONG	profit_margin;			// ��Ӫҵ�������� % 0.001
	LONG	netasset_yield;			// ���ʲ������� % 0.001
	// ���ʱ��ṹ
	LONG	date_begin;				// �������� Format is YYYYMMDD
	LONG	share_count_total;		// �ܹɱ� 10000
	LONG	share_count_a;			// ��ͨA�� 10000
	LONG	share_count_b;			// ��ͨB�� 10000
	LONG	share_count_h;			// ��ͨH�� 10000
	LONG	share_count_national;	// ���й� 10000
	LONG	share_count_corp;		// ���˹� 10000
	LONG	profit_psud;			// ÿ��δ�������� 0.001
	LONG	asset;					// ���ʲ� 10000
	LONG	ratio_holderright;		// �ɶ�Ȩ����� 0.001
	LONG	ratio_longdebt;			// ���ڸ�ծ�� 0.001
	LONG	ratio_debt;				// �ʲ���ծ�� 0.001
	// ��Ͷ����������
	LONG	netasset_ps;			// ÿ�ɾ��ʲ� 0.001
	LONG	netasset_ps_regulate;	// ����ÿ�ɾ��ʲ� 0.001
	LONG	eps;					// ÿ������ 0.001
	LONG	eps_deduct;				// �۳���ÿ������ 0.001
	LONG	net_profit;				// ������ 10000
	LONG	main_profit;			// ��Ӫҵ������ 10000
	LONG	total_profit;			// �����ܶ� 10000
	// ��������
	LONG	profit_inc;				// ��Ӫ����������% 0.001
	LONG	income_inc;				// ��Ӫ����������% 0.001
	LONG	asset_inc;				// ���ʲ�������% 0.001
	// �＼������ͳ��ֵ
	LONG	yield_average;			// ƽ��������% 0.001
	LONG	yield_stddev;			// �����׼��% 0.001
	LONG	beite;					// ��ֵ 0.001

	DWORD	reserved[4];
}CLK20_BASEDATA, *PCLK20_BASEDATA;
*/

// ��Ȩ����
typedef struct clk_drdata_t {
	DWORD	date;
	DWORD	bonus;			//	10��XԪ		/1000
	DWORD	bonus_share;	//	10��X��		/100
	DWORD	ration_share;	//	10��X��		/100
	DWORD	ration_value;	//	��ɼ�		/1000
	DWORD	reserved;
}CLK_DRDATA, * PCLK_DRDATA;

// �����Ҹ�ʽ��Ȩ����
typedef union fxj_drdata_t {
	struct
	{
		DWORD	m_dwMagic;		// = -1
		WORD	m_wMarket;		// SH �Ϻ� SZ ����
		char	m_szCode[6];
		char	m_szReserved[8];
	};
	struct
	{
		time_t	m_time;
		float	m_fGive;			// ÿ����
		float	m_fPei;				// ÿ����
		float	m_fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
		float	m_fProfit;			// ÿ�ɺ���
	};
}FXJ_DRDATA, * PFXJ_DRDATA;

// ͨ���Ÿ�ʽ��������
#define	TDX_FHEADER_SIZE	0x521
typedef struct tdx_basedata_t {
	char	m_szMagic;
	char	m_szMarket;						// �г� '0' ���� '1' �Ϻ�
	char	m_szCode[6];
	char	m_szDate_modified[8];			// ��������
	char	m_szShare_count_total[14];		// �ܹɱ� 10000
	char	m_szShare_count_national[14];	// ���ҹ� 10000
	char	m_szShare_count_orig[14];		// ����� 10000
	char	m_szShare_count_corp[14];		// ���˹� 10000
	char	m_szShare_count_b[14];			// B�� 10000
	char	m_szShare_count_h[14];			// H�� 10000
	char	m_szShare_count_currency[14];	// ��ͨ�� 10000
	char	m_szShare_count_employ[14];		// ְ���� 10000
	char	m_szUnkown1[14];				// 
	char	m_szAsset[14];					// ���ʲ� 1000
	char	m_szAsset_currency[14];			// �����ʲ� 1000
	char	m_szAsset_capital[14];			// �̶��ʲ� 1000
	char	m_szAsset_imma[14];				// �����ʲ� 1000
	char	m_szInvest_long[14];			// ����Ͷ�� 1000
	char	m_szDebt_currency[14];			// ������ծ 1000
	char	m_szDebt_long[14];				// ���ڸ�ծ 1000
	char	m_szAcc_fund[14];				// ������ 1000
	char	m_szNet_asset[14];				// ���ʲ� 1000
	char	m_szMain_income[14];			// ��Ӫ���� 1000
	char	m_szMain_profit[14];			// ��Ӫ���� 1000
	char	m_szOther_profit[14];			// �������� 1000
	char	m_szBusiness_profit[14];		// Ӫҵ���� 1000
	char	m_szInvest_profit[14];			// Ͷ������ 1000
	char	m_szAllow_profit[14];			// �������� 1000
	char	m_szOut_profit[14];				// ҵ����֧ 1000
	char	m_szProfit_regulate[14];		// ������� 1000
	char	m_szTotal_profit[14];			// �����ܶ� 1000
	char	m_szProfit_aftertax[14];		// ˰������ 1000
	char	m_szNet_profit[14];				// ������ 1000
	char	m_szProfit_ud[14];				// δ������ 1000
	char	m_szNet_asset_ps_regulate[14];	// ÿ�ɾ���2
	char	m_szUnknown2[3];				// 
	char	m_szUnknown3[4];				// 
	char	m_szUnknown4[2];				//
	char	m_szDate_begin[8];				// ��������
	char	m_szUnknown5[3];				// 
	char	m_szUnknown6[11];				// 
}TDX_BASEDATA, *PTDX_BASEDATA;

// �����Ҹ�ʽ��������
#define	FXJ_FHEADER_SIZE	0x8
typedef struct fxj_basedata_header_t {
	DWORD	m_dwMagic1;	//	0x223FD90C;
	DWORD	m_dwMagic2;	//	0x000000A6;
}FXJ_BASEDATA_HEADER, *PFXJ_BASEDATA_HEADER;
typedef struct fxj_basedata_t {
	WORD	m_wMarket;						// �г� 'ZS'" ���� 'HS' �Ϻ�
	WORD	m_wUnknown;
	char	m_szCode[6];
	float	m_fData[39];
/*
	m_fData[0]
	m_fData[1]
	m_fData[2]*10000	// �ܹɱ�
	m_fData[3]*10000	// ���й�
	m_fData[4]*10000	// �����˷��˹�
	m_fData[5]*10000	// ���˹�
	m_fData[6]*10000	// ��ͨB��
	m_fData[7]*10000	// ��ͨH��
	m_fData[8]*10000	// ��ͨA��
	m_fData[9]
	m_fData[10]
	m_fData[11]*1000	// ���ʲ�
	m_fData[12]*1000	// ��ͨ�ʲ�
	m_fData[13]*1000	// �̶��ʲ�
	m_fData[14]
	m_fData[15]*1000	// ����Ͷ��
	m_fData[16]*1000	// ��ͨ��ծ
	m_fData[17]*1000	// ���ڸ�ծ
	m_fData[18]*1000	// �ʱ�������
	m_fData[19]			// ������/��
	m_fData[20]*1000	// �ɶ�Ȩ��
	m_fData[21]*1000	// ��Ӫҵ������
	m_fData[22]*1000	// ��Ӫҵ������
	m_fData[23]*1000	// ����ҵ������
	m_fData[24]*1000	// Ӫҵ����
	m_fData[25]*1000	// Ͷ������
	m_fData[29]*1000	// �����ܶ�
	m_fData[30]
	m_fData[31]*1000	// ������
	m_fData[32]*1000	// δ��������
	m_fData[33]			// ÿ��δ��������
	m_fData[34]			// ÿ������
	m_fData[35]			// ÿ�ɾ��ʲ�
	m_fData[36]			// ����ÿ�ɾ��ʲ�
	m_fData[37]			// �ɶ�Ȩ�����
	m_fData[38]			// ���ʲ������� %
*/
}FXJ_BASEDATA, *PFXJ_BASEDATA;

/*	���ø����������ʹ洢���ļ���ʽ����������

	1. �ļ�ͷ��CLK_FHEADER

	2. ����������СΪ CLK_FHEADER.m_dwIndexRecordSize * CLK_FHADER.m_dwIndexRecordCount
				��Ϊ CLK_FHADER.m_dwIndexRecordCount ���飬
				ÿ��Ϊ CLK_FHEADER.m_dwIndexRecordSize���� CLK_INDEXRECORD

	3. ����������СΪCLK_FHEADER.m_dwBlockSize * CLK_FHEADER.m_dwBlockCount
				��Ϊ CLK_FHADER.m_dwBlockCount �� Block��
				ÿһ Block ��һ�� CLK_BLOCKHEADER���ͽ������� CLK_BLOCKHEADER.m_dwDataRecordCount ����Ʊ���ݼ�¼
				���һ�� Block ���ܼ�¼�ù�Ʊȫ����¼���� CLK_BLOCKHEAHDRE.m_dwPosNextBlock ָ����һ�����ݿ�
*/

#define	CLK_FHEADER_MAGIC		'FKLC'
#define	CLK_FHEADER_VERMAJOR	'2003'
#define	CLK_FHEADER_VERMINOR	'1002'

// �����ļ���ʽ���ļ�ͷ
typedef	struct clk_fheader
{
	// �ļ���Ϣ
	DWORD	m_dwMagic;					// = CLK_FHEADER_MAGIC
	DWORD	m_dwVerMajor;				// = CLK_FHEADER_VERMAJOR
	DWORD	m_dwVerMinor;				// = CLK_FHEADER_VERMINOR
	DWORD	m_dwReserved1;				// ����
	DWORD	m_dwDataType;				// ���ݷ���˵����see CStock::DataType
	DWORD	m_dwMarket;					// �г���see CStock::StockMarket������ļ���������г�����=0��Ч
	time_t	m_time;						// �ļ�����޸�����

	// ��������Ϣ
	DWORD	m_dwPosFirstIndex;			// ��������ʼλ��
	DWORD	m_dwIndexRecordSize;		// ��������¼��Ԫ�ֽ���
	DWORD	m_dwIndexRecordCount;		// ��������¼��Ԫ����
	DWORD	m_dwStockCount;				// ���ļ��������е��ܹ�Ʊ��

	// ��������Ϣ������m_dwBlockCount��Block��ÿ��Block����m_dwBlockSize/m_dwDataRecordSize��DataRecord
	DWORD	m_dwDataRecordSize;			// ��������¼�ֽ���
										// ����m_dwDataType��
										// = sizeof(REPORT)
										// or = sizeof(MINUTE)
										// or = sizeof(KDATA)
	DWORD	m_dwRecordPerBlock;			// ÿ��Block�ļ�¼��
	DWORD	m_dwBlockSize;				// ������Block��Ԫ�ֽ�����һ��Block��һ��BlockHeader�����ɸ�DataRecord
	DWORD	m_dwPosFirstBlock;			// �ļ��е�һ��Blockλ��.
	DWORD	m_dwPosFirstBlankBlock;		// �ļ��е�һ����Block��λ�ã����п�Block���һ������

	DWORD	m_dwReserved2[4];			// ����

	char	m_szDescript[128];			// �ļ�˵��
	char	m_szNextFileName[MAX_PATH];	// ��ʱ��һ���ļ�����̫���ж���ļ���m_szNextFileNameָ����
										// һ��ͬ�����ļ������Ŀ¼��Ŀǰδʹ��
} CLK_FHEADER;

#define	CLK_INDEXRECORD_MAGIC	'XDNI'

// �����ļ���ʽ����Ʊ�������ֵ�Ԫ
typedef	struct clk_indexrecord
{
	DWORD	m_dwMagic;					// = CLK_INDEXRECORD_MAGIC
	DWORD	m_dwMarket;					// ��Ʊ�г� see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];	// ��Ʊ����
	DWORD	m_dwPosFirstBlock;			// ����Ʊ��һ��Block��ʼλ�ã�-1��0��ʾ��
	DWORD	m_dwDataRecordCountTotal;	// ����Ʊ���ݼ�¼����
	DWORD	m_dwReserved;				// ����
} CLK_INDEXRECORD;

#define	CLK_BLOCKHEADER_MAGIC	'KCLB'

// �����ļ���ʽ�����������֣����ݿ�ͷ
typedef	struct clk_blockheader
{
	DWORD	m_dwMagic;					// = CLK_BLOCKHEADER_MAGIC
	DWORD	m_bUsed;					// ��Block�Ƿ�ʹ��
	DWORD	m_dwPosFirstRecord;			// ��Block���ݼ�¼��ʼλ��
	DWORD	m_dwDataRecordCount;		// ��Block�����ݼ�¼����
	DWORD	m_dwPosNextBlock;			// ���б���Ʊ���ݵ���һ��Block����ʼλ�ã�-1��0��ʾ��
	DWORD	m_dwReserved;				// ����
} CLK_BLOCKHEADER;

#pragma	pack()

//=============================================================================
// CTSKFile

class CTSKFile
{
public:
	CTSKFile();
	virtual ~CTSKFile();

protected:
	CSPFile	 m_file;
	CLK_FHEADER	m_header;

	// ���µ�ǰ���ڲ����Ĺ�Ʊ
	CLK_INDEXRECORD m_CurIndexRecord;
	DWORD m_dwPosCurIndex;

public:
	BOOL Open(LPCTSTR lpszFileName);
	void Close();

public:
	static BOOL BuildEmptyFile(LPCTSTR lpszFileName, DWORD dwDataType, DWORD dwMarket, DWORD dwIndexRecordCount = 3000,
				DWORD dwRecordPerBlock = 50, BOOL bRebuildIfExists = FALSE);
	static BOOL RemoveFileIfOutoftime(LPCTSTR lpszFileName, time_t tmNow);

	// Store Function
	BOOL	EmptyAll();
	DWORD	StoreDataRecord(DWORD dwMarket, const char * szCode, void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount, BOOL bOverWrite);	// ���سɹ������¼��

	// Load Function
	DWORD	GetDataRecordCount(DWORD dwMarket, const char * szCode);
	DWORD	LoadDataRecord(DWORD dwMarket, const char * szCode,
							void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement);// ���سɹ���ȡ��¼��

protected:
	DWORD	Hash(LPCTSTR key, DWORD dwMax);

	BOOL	GetDataInfo(DWORD dwMarket, const char * szCode, CLK_INDEXRECORD & idxRet, DWORD & dwPosIndexFind, BOOL bAddIfNotExists);
	BOOL	SetDataInfo(DWORD dwPosIndex, CLK_INDEXRECORD idx, BOOL bFlush);

	DWORD	GetFirstBlankBlockPos(BOOL bAddIfNotExists, BOOL bUseIt);
	DWORD	EmptyBlockChain(DWORD dwPosFirstBlock);
	
	DWORD	ReadData(DWORD dwPostBlock, void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement);
	DWORD	WriteData(DWORD dwPosBlock, void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount, BOOL bFlush);
};

//=============================================================================
// CSelfDB

class CSelfDB : public CQianlong
{
public:
	CSelfDB(const char * rootpath, BOOL bOK = TRUE);
	virtual	~CSelfDB();

public:
	static char m_szDataType[256];

public:
	virtual	BOOL GetFileName(CSPString& sFileName, int nDataType, CStockInfo* pInfo = NULL, int nKType = CKData::ktypeDay);
	static int GetType() { return dbtypeSelfDB; }
	static const char * GetName() { return m_szDataType; }
	virtual	int	GetDBType() { return GetType(); }
	virtual	const char * GetDBTypeName() { return GetName(); }
	static CSPString GetNewsPath(DWORD dwMarket);
	static CSPString GetBasePath(DWORD dwMarket);
	static BOOL CreateSelfDB(const char * rootpath);
	static BOOL GetAccurateRoot(const char * rootpath, char *accurateroot, int maxlen);

public:
	virtual	int LoadCodetable(CStockContainer& container);
	virtual	int	StoreCodetable(CStockContainer& container);
	virtual	int	LoadKData(CStock* pstock, int nKType);
	virtual	int	LoadBasetable(CStockContainer& container);
	virtual	int StoreBasetable(CStockContainer& container);
	virtual	int	LoadDRData(CStock* pstock);
	virtual	int StoreDRData(CStock* pstock);
	virtual int	LoadOutline(CStock* pstock);
	virtual int	StoreOutline(OUTLINE* pOutline, int nCount);

	virtual int	LoadMinute(CStock* pstock);
	virtual int	StoreMinute(MINUTE* pMinute, int nCount);
	virtual int	LoadReport(CStock* pstock);
	virtual int	StoreReport(REPORT* pReport, int nCount, BOOL bBigTrade);

public:
	virtual	int	InstallCodetbl(const char * filename, const char * orgname);
	virtual	int	InstallCodetblBlock(const char * filename, const char * orgname);
	virtual	int	InstallCodetblFxjBlock(const char * filename, const char * orgname);
	virtual	int InstallDRData(CDRData& drdata);
	virtual	int	InstallDRDataClk(const char * filename, const char * orgname);
	virtual	int	InstallDRDataFxj(const char * fxjfilename);
	virtual	int	InstallBasetable(const char * filename, const char * orgname);
	virtual	int	InstallBasetableTdx(const char * filename);
	virtual	int	InstallBasetableFxj(const char * filename);
	virtual	int InstallNewsText(const char * filename, const char * orgname);
	virtual	int InstallNewsText(const char * buffer, int nLen, const char * orgname);
};
