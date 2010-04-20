// UrlSrcFromSvrMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "emule.h"
#include "UrlSrcFromSvrMgr.h"
#include "UrlSrcGetFromSvrSocket.h"
#include "PartFile.h"


// CUrlSrcFromSvrMgr

CUrlSrcFromSvrMgr::CUrlSrcFromSvrMgr()
{
	m_pAssocPartFile	= NULL;
}

CUrlSrcFromSvrMgr::~CUrlSrcFromSvrMgr()
{
}


// CUrlSrcFromSvrMgr ��Ա����
void CUrlSrcFromSvrMgr::SetAssocPartFile(CPartFile *pAssocPartFile)
{
	m_pAssocPartFile = pAssocPartFile;
}

bool CUrlSrcFromSvrMgr::GetSrcFromServerAsyn(void)
{
	try
	{
		if (NULL == m_pAssocPartFile)
			return false;
		
		CUrlSrcGetFromSvrSocket	*pSocket = new CUrlSrcGetFromSvrSocket(this, true);		//��������� CClientReqSocket �����ӶϿ���� delete �Լ�������newһ���������������ӡ�
		if (NULL == pSocket)
			return false;
		
		return pSocket->SendRequest();
	}
	catch( ... )
	{
		return false;
	}
}

bool CUrlSrcFromSvrMgr::SendReq_FileDownloaded(void)
{
	if (NULL == m_pAssocPartFile)
		return false;

	CUrlSrcGetFromSvrSocket	*pSocket = new CUrlSrcGetFromSvrSocket(this, false);		//��������� CClientReqSocket �����ӶϿ���� delete �Լ�������newһ���������������ӡ�
	if (NULL == pSocket)
		return false;

	return pSocket->SendRequest();
}

bool CUrlSrcFromSvrMgr::AddSrcToServer(LPCTSTR lpszUrl)
{
	if (!IsExistInFetchedList(lpszUrl))
	{
		//todo: add to server
		m_strlstFetchedUrlSources.AddTail(lpszUrl);
	}

	return true;
}

bool CUrlSrcFromSvrMgr::IsExistInFetchedList(LPCTSTR lpszUrl)
{
	POSITION	pos;
	CString		str;

	pos = m_strlstFetchedUrlSources.GetHeadPosition();
	while (NULL != pos)
	{
		str = m_strlstFetchedUrlSources.GetNext(pos);
		if (0 == str.CompareNoCase(lpszUrl))
			return true;
	}

	return false;
}
