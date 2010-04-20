#pragma once
#include "traversestrategy.h"
#include "ClientList.h"
#include "updownclient.h"
#include "ListenSocket.h"
#include "sockets.h"

class CTraverseBySourceExchange : public CTraverseStrategy
{
public:
	CTraverseBySourceExchange(const uchar * uh, CTraverseStrategy * pNext);
	~CTraverseBySourceExchange(void);

	virtual bool Initialize();
	virtual void SendPacket();
	virtual bool ProcessPacket(const uchar * data, int len, DWORD ip, WORD port);
private:
	int SendConnectReq();
	int SendPingPacket();
	void OnRecvSync(const uchar * data, int len, DWORD ip, WORD port);
	void OnRecvPing(const uchar * data, int len, DWORD ip, WORD port);

	DWORD m_dwTraverseIp;
	WORD m_wTraversePort;

	int			m_nSendReqCount ,m_nSendPingCount , m_nSendConnAck;
	int			m_nPassivePing;
	DWORD		m_SendReqTime, m_SendPingTime, m_SendConnAckTime;
	DWORD		m_dwClientIp;
	WORD		m_wClientPort;
	CAsyncSocketEx * m_Sock;

	bool		m_bAcceptor;
	bool		m_bByBuddy;
	DWORD		m_dwState;
	DWORD		m_dwConnAskNumber;

	enum {SYNC_INIT_ATTEMPT_INTERVAL = 2, SYNC_ATTEMPT_TIMES = 8, PING_ATTEMPT_TIMES = 15, PING_ATTEMPT_INTERVAL = 4};
	DWORD	m_dwNextSyncInterval;	//VC-fengwen on 2007/12/27 :重试时间使用指数退避。
};
