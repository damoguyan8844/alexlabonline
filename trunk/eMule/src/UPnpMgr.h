#pragma once

#include <afxtempl.h>
#include <AfxMt.h>
#include <set>
#include "UPnpNat.h"
#include "UPnpNatMapping.h"
#include "UPnpNatMappingKey.h"

using namespace std;

class CUPnpMgr
{
public:
	CUPnpMgr(void);
	~CUPnpMgr(void);

	enum {RANDOM_RETRY_TIMES = 5};

	CSyncObject*	GetSyncObject(){return &m_cs;}
	BOOL			Lock(){return m_cs.Lock();}
	BOOL			Unlock(){return m_cs.Unlock();}
	BOOL			TryLock(){return TryEnterCriticalSection(&(m_cs.m_sect));}

	void			SetBindAddress(LPCTSTR lpszBindAddress);
	void			SetActionTimeout(int iActionTimeoutMs = 5000);
	int				GetActionTimeout();

	HRESULT			AddNATPortMapping(CUPnpNatMapping &mapping, BOOL bTryRandom = FALSE);
	void			RemoveNATPortMapping(const CUPnpNatMappingKey &mappingKey);
	void			CleanupAllEverMapping(void);

	void			WriteAddedMappingToFile(void);
	void			ReadAddedMappingFromFile(void);


	BOOL			CleanedFillupBug();
	
	CString			GetLocalIPStr();
	DWORD			GetLocalIP();
	
	static CString	ResultCode2String(HRESULT hr);
	static CString	Result2String(HRESULT hr, DWORD dwActionErrorCode);

	DWORD			GetLastActionErrorCode(){return m_nat.GetLastActionErrorCode();}
protected:
	void			InitLocalIP();
	void			CleanupMappingArr();


protected:
	CCriticalSection	m_cs;
	CUPnpNat			m_nat;

	set<CUPnpNatMappingKey>									m_setAddedMapping;

	CString		m_slocalIP;
	DWORD		m_uLocalIP;

};
