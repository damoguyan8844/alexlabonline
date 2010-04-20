#pragma once
#include "MapKey.h"

//typedef int (*PlayerDataCallback)(const CSKey & key, void * pBuf, int nBufLen);

#define _TCP_BUF_RAW	1
#define _TCP_BUF_FILE	2

#define WM_PLAYER_DATA_REQ	(WM_USER + 7789)

const int MAX_CACHE_SIZE = 1024 * 2;

typedef struct {
	uchar filehash[16];
	DWORD pos;
	DWORD len;
} PLAYER_DATA_REQ;

typedef struct
{
	int nType;  //  1: raw data, 2: from file
	int nLen, nOffset;
	char * pBuf;
} TCP_BUFFER;

class CListenPlayerSocket : public CAsyncSocket
{
public:
	virtual void OnAccept(int nErrorCode);
};

class CPlayerTaskSocket : public CAsyncSocket
{
	friend class CPlayerTask;
public:
	CPlayerTaskSocket()
	{
		m_pTask = NULL;
		m_dwCurrentPos = 0;

		m_tmReqData = 0;
		m_nTotalBufferSize = 0;
		m_tmSendData = 0;
		m_bSentData =false;
		m_bRangeReq = false;
		m_dwTailPos = 0;
	}

	void ClearBuffer();
	bool RequestData();
	void SendBuffer(bool bKeepTail);
	void SendTinyData();
	void SaveBuffer(void * pData, int nLen);
	virtual void OnReceive(int nErrorCode);
	virtual ~CPlayerTaskSocket();
private:
	bool ParseHttpReq(CStringA strHtml, CStringA & rHashId, int & rnRange);

	CPlayerTask * m_pTask;
	DWORD m_dwCurrentPos;
	DWORD m_tmReqData, m_tmSendData;
	CList<TCP_BUFFER*> m_BufferLst;
	int m_nTotalBufferSize;
	bool m_bSentData;
	bool m_bRangeReq;
	DWORD m_dwTailPos;
public:
	virtual void OnClose(int nErrorCode);
};


class CPlayerTask
{
public:
	CPlayerTask(const uchar * fh)
	{
		memcpy(m_filehash, fh, 16);

		m_dwTotalFileSize = 0;
		m_bStarted = false;
		m_tmEmptyTask = 0;
	}
	~CPlayerTask()
	{
		POSITION pos = m_SockList.GetHeadPosition();
		while(pos)
		{
			CPlayerTaskSocket * sk= m_SockList.GetNext(pos);
			delete sk;
		}
	}
	CStringA GetHeader(DWORD dwPos);
	void AddSocket(CPlayerTaskSocket * sk);
	bool RequestData();
	bool SendData(DWORD dwPos, void * pData, int nLen);

	CList<CPlayerTaskSocket *, CPlayerTaskSocket*> m_SockList;

	DWORD m_dwTotalFileSize;
	bool m_bStarted;
	uchar m_filehash[16];

	time_t m_tmEmptyTask;
};
// CPlayerMgr

class CPlayerMgr : public CWinThread
{
	DECLARE_DYNCREATE(CPlayerMgr)
friend bool IsTaskExist(const uchar * filehash);
protected:
	CPlayerMgr();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPlayerMgr();

	CMap<CSKey, const CSKey&, CPlayerTask *, CPlayerTask *> m_TaskLst;
	CListenPlayerSocket	m_skListen;
	WORD	m_nListenPort;
	CMap<CPlayerTaskSocket *,CPlayerTaskSocket *,CPlayerTaskSocket *,CPlayerTaskSocket *> m_ForDelete;

	void Process();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	static CMutex m_Mutex;
//	static PlayerDataCallback m_PlayerCallback;

	void AddSocketToDelete(CPlayerTaskSocket * sk)
	{
		CSingleLock lock(&m_Mutex, true);
		m_ForDelete[sk]  = sk;
	}

	CPlayerTask * GetPlayerTask(const CSKey & key)
	{
		CPlayerTask * pTask=NULL;
		m_TaskLst.Lookup(key, pTask);

		return pTask;
	}
	CPlayerTask * AddPlayerTask(const CSKey & key)
	{
		CPlayerTask * pTask;
		if(m_TaskLst.Lookup(key, pTask) && pTask)
		{
			pTask->m_tmEmptyTask = 0;
			return pTask;
		}

		pTask = new CPlayerTask(key.m_key);
		m_TaskLst.SetAt(key, pTask);

		return pTask;
	}
	void RemovePlayerTask(const CSKey & key)
	{
		if(m_TaskLst.IsEmpty()) return;

		CPlayerTask * pTask;

		if(m_TaskLst.Lookup(key, pTask))
		{
			delete pTask;
			m_TaskLst.RemoveKey(key);
		}
	}
	void RemovePlayerTaskBySocket(CPlayerTaskSocket * sock)
	{
		CSingleLock slock(&m_Mutex, true);
		POSITION pos = m_TaskLst.GetStartPosition();
		while(pos)
		{
			CSKey key;
			CPlayerTask * pTask;
			m_TaskLst.GetNextAssoc(pos, key, pTask);

			if(pTask && pTask->m_SockList.Find(sock, NULL)!=NULL)
			{
				pTask->m_SockList.RemoveAt(pTask->m_SockList.Find(sock, NULL));
				if(pTask->m_SockList.IsEmpty())
				{
					m_TaskLst.RemoveKey(key);
					delete pTask;
				}

				return ;
			}
		}
	}
	WORD GetListenPort()
	{
		return m_nListenPort;
	}

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

// ����ֵ
//  0: �ɹ�
//  1: ���ļ��Ѿ����ڲ���
//  2: ע����ѯʧ��
//  3: �޷����ļ�
extern int StartPlayer(const uchar * filehash, CString strFilename, DWORD dwFileSize, CString strExt);

// ����ֵ
//  0: �ɹ�
//  1: ���ļ��Ѿ�ȡ��
//  2: ��������
//  3: ��û��ʼ
//  4: �޷���ò�����
extern int PlayerDataArrived(PLAYER_DATA_REQ * req, void * data, int len);

//extern void SetPlayerDataCallback(PlayerDataCallback pcb);
//extern void GetPlayerDataFromFile(const uchar * filehash, DWORD dwPos, DWORD dwLen);
extern bool IsTaskExist(const uchar * filehash);

extern void StopPlayer(const uchar * filehash);
extern void StopAllPlayer();

