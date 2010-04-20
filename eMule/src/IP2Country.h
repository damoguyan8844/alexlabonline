//EastShare Start - added by AndCycle, IP to Country

// by Superlexx, based on IPFilter by Bouc7
#pragma once
//#include <atlcoll.h>
#include "Log.h"

struct Country_Struct {
	CString			ShortCountryName;
	CString			MidCountryName;
	CString			LongCountryName;
	WORD			FlagIndex;
};

struct IPRange_Struct2{
	uint32          IPstart;
	uint32          IPend;
	Country_Struct*	country;
	~IPRange_Struct2() {  }
};

//	���������п����ڴ���ʱ�������߳����У���Ҫ���ʹ�����Դ��Ӧ��ͨ���ж�CIP2CountryOld::IsInMainThread()�ж��Ƿ������߳��С�
class CIP2CountryOld
{
	public:
		CIP2CountryOld(void);
		~CIP2CountryOld(void);
		
		void	Load();
		void	Unload();

		//reset ip2country referense
		void	Reset();

		//refresh passive windows
		void	Refresh();

		bool	IsIP2Country()			{return EnableIP2Country;}
		bool	ShowCountryFlag();

		Country_Struct*	GetDefaultIP2Country() {return &defaultCountry;}

		bool	LoadFromFile();
		bool	LoadCountryFlagLib();
		void	RemoveAllCountries();
		void	RemoveAllIPs();
		void	RemoveAllFlags();

		bool	AddIPRange(uint32 IPfrom,uint32 IPto, CString shortCountryName, CString midCountryName, CString longCountryName);
		bool	AddCountry(CString shortCountryName, CString midCountryName, CString longCountryName);

		Country_Struct*	GetCountryFromIP(uint32 IP);
		WORD	GetFlagResIDfromCountryCode(CString shortCountryName);

		CImageList* GetFlagImageList() {return &CountryFlagImageList;}
	private:

		//check is program current running, if it's under init or shutdown, set to false
		bool	m_bRunning;

		HINSTANCE _hCountryFlagDll;
		CImageList	CountryFlagImageList;

		bool	EnableIP2Country;
		bool	EnableCountryFlag;
		Country_Struct defaultCountry;

		CRBMap<uint32, IPRange_Struct2*> iplist;
		CRBMap<CString, Country_Struct*> countryList;
		CRBMap<CString, uint16>	CountryIDtoFlagIndex;

		bool	m_bInMainThread;	//ADDED by fengwen on 2007/01/16	: �Ƿ������߳��У� ��ֻ�������̣߳���Щ��ȫ�ֱ����ķ��ʲ��ܽ��С���
		//ADDED by fengwen on 2007/01/16	<begin> : �����಻�����߳��У����ܷ���ȫ�ֱ���ʱ��һЩ���ʣ���ʼ��Ա�������롣
		bool	m_tp_bIsIP2CountryShowFlag;
		int		m_tp_iIP2CountryNameMode;
		int		m_tp_iDfltImageListColorFlags;
		CString	m_tp_strConfigDir;
		//ADDED by fengwen on 2007/01/16	<end> : �����಻�����߳��У����ܷ���ȫ�ֱ���ʱ��һЩ���ʣ���ʼ��Ա�������롣
		void AddDebugLogLine(bool bAddToStatusBar, LPCTSTR pszLine)
		{
			if (m_bInMainThread)
				::AddDebugLogLine(bAddToStatusBar, pszLine);
		}
		friend class CIP2Country;	//ADDED by fengwen on 2007/01/16	: 
};

//EastShare End - added by AndCycle, IP to Country

//ADDED by fengwen on 2007/01/16	<begin> :	Ϊ���IP2Country�ķ�Ӧ�ٶȣ���ԭ����IP2Country�����߳���ִ�С�
class CIP2Country : public CWnd
{
	DECLARE_DYNAMIC(CIP2Country)
public:
	CIP2Country(void);
	~CIP2Country(void);

public:
	bool				ShowCountryFlag();
	CImageList*			GetFlagImageList();
	Country_Struct*		GetCountryFromIP(uint32 IP);
	bool				IsIP2Country();
	void				Load();
	void				Refresh();
	Country_Struct*		GetDefaultIP2Country();


protected:
	void				CreateOldI2CAsync(void);

	CIP2CountryOld	*m_pOldIP2Country;
	CWinThread		*m_pInitOldThread;

	
	typedef struct _OldInitThreadParam
	{
		HWND	hNotifyWnd;
		bool	bIsIP2CountryShowFlag;
		int		iIP2CountryNameMode;
		int		iDfltImageListColorFlags;
		CString	strConfigDir;
	} OLD_INIT_THREAD_PARAM;
	
	friend static UINT	CIP2Country::OldProcInitThread(LPVOID lpParam);
	static UINT	OldProcInitThread(LPVOID lpParam);
protected:
	DECLARE_MESSAGE_MAP()
	
	afx_msg LRESULT OnLoadCompleted(WPARAM wParam, LPARAM lParam);
};
//ADDED by fengwen on 2007/01/16	<end> :	Ϊ���IP2Country�ķ�Ӧ�ٶȣ���ԭ����IP2Country�����߳���ִ�С�
