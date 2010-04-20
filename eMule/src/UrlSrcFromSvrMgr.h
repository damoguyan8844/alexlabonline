#pragma once


//#include "AsyncSocketEx.h"
// CUrlSrcFromSvrMgr ����Ŀ��
class CPartFile;

class CUrlSrcFromSvrMgr : public CObject
	{
	friend class CUrlSrcGetFromSvrSocket;		//���� CClientReqSocket �����ӶϿ���� delete �Լ���������һ����CUrlSrcFromSvrSocket���������ӡ�
public:
	CUrlSrcFromSvrMgr();
	virtual ~CUrlSrcFromSvrMgr();

	void	SetAssocPartFile(CPartFile *pAssocPartFile);
	bool	GetSrcFromServerAsyn(void);							//�첽��������ȡurlԴ��
	bool	SendReq_FileDownloaded(void);						//���߷��������ļ�������ϡ�

	bool	AddSrcToServer(LPCTSTR lpszUrl);								//��urlԴ��ӵ��������ϱ���������

protected:
	bool	IsExistInFetchedList(LPCTSTR lpszUrl);


protected:
	CPartFile	*m_pAssocPartFile;								//��֮�������PartFile��ָ�롣

	CStringList			m_strlstFetchedUrlSources;				//�ڷ������ϵ�urlԴ�����ӷ�������ȡ��urlԴ�󣬴���ڴˡ���
};


