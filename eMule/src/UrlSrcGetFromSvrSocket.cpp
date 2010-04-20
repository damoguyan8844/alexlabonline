// UrlSrcGetFromSvrSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "emule.h"
#include "UrlSrcGetFromSvrSocket.h"
#include "UrlSrcFromSvrMgr.h"
//#include "packets.h"
#include "PartFile.h"
#include "StringConversion.h"
#include "Preferences.h"
#include "log.h"

// CUrlSrcGetFromSvrSocket

CUrlSrcGetFromSvrSocket::CUrlSrcGetFromSvrSocket(CUrlSrcFromSvrMgr *pMgr, bool bStart)
{
	m_pMgr = pMgr;
	m_bStart = bStart;
}

CUrlSrcGetFromSvrSocket::~CUrlSrcGetFromSvrSocket()
{
}


// CUrlSrcGetFromSvrSocket ��Ա����

CStringA CUrlSrcGetFromSvrSocket::GetServer()
{
	return "client.stat.verycd.com";
}

CStringA CUrlSrcGetFromSvrSocket::GetUrlPath()
{
	if (::IsBadReadPtr(m_pMgr, sizeof(CUrlSrcFromSvrMgr)))
		return "";

	CStringA	strUrlPath;
	CStringA	strHash;
	CStringA	strSize;
	CStringA	strFileName;
	CStringA	strEncodedFileName;

	if( NULL==m_pMgr->m_pAssocPartFile )
		return "";

	try
	{
		//	Hash
		strHash = md4str(m_pMgr->m_pAssocPartFile->GetFileHash());
		strHash.MakeLower();		//���붼ΪСд��

		//	Size
		char szSize[1024];
		_i64toa(m_pMgr->m_pAssocPartFile->GetFileSize(), szSize,10);
		strSize = szSize;

		//strHash = "48F8220C4FF02D4E5AD11DD5E1E305CB";
		//strHash.MakeLower();
		//strSize = "125753277";
		//lptszFileName = _T("PopKart_Setup_P092.exe");

		//	FileName
		strEncodedFileName = EncodeUrlUtf8(m_pMgr->m_pAssocPartFile->GetFileName());

		//	construct UrlPath
		//strUrlPath.Format("http://client.stat.verycd.com/dl/%s%s/%s/start", strHash, strSize, strEncodedFileName);

		if (m_bStart)
			strUrlPath.Format("/dl/%s%s/%s/start", strHash, strSize, strEncodedFileName);
		else
			strUrlPath.Format("/dl/%s%s/%s/finished", strHash, strSize, strEncodedFileName);

		return strUrlPath;
	}
	catch ( ... ) 
	{
		return "";
	}
}

bool CUrlSrcGetFromSvrSocket::ProcessHttpResponse()
{
	if (m_bStart)
		return ProcessHttpResponse_Start();
	else
		return ProcessHttpResponse_Finished();
}

bool CUrlSrcGetFromSvrSocket::ProcessHttpResponseBody(const BYTE* pucData, UINT size)
{
	if (m_bStart)
		return ProcessHttpResponseBody_Start(pucData, size);
	else
		return ProcessHttpResponseBody_Finished(pucData, size);
}

bool CUrlSrcGetFromSvrSocket::ProcessHttpResponse_Start()
{
	int iMajorVer, iMinorVer;
	int iResponseCode;
	char szResponsePhrase[1024];
	sscanf(m_astrHttpHeaders[0], "HTTP/%d.%d %d %s", &iMajorVer, &iMinorVer, &iResponseCode, szResponsePhrase);

	if (thePrefs.GetVerbose())
		AddDebugLogLine(false, _T("Receive UrlSources from server (http response code = %d)"), iResponseCode);

	if (200 != iResponseCode)
		return false;

	return true;
}
bool CUrlSrcGetFromSvrSocket::ProcessHttpResponseBody_Start(const BYTE* pucData, UINT size)
{
	if (thePrefs.GetVerbose())
	{
		AddDebugLogLine(false, _T("Receive UrlSources from server (http response body)"));
		AddDebugLogLine(false, CString((const char*) pucData, size));
	}


	if (::IsBadReadPtr(m_pMgr, sizeof(CUrlSrcFromSvrMgr)))
		return true;

	CStringA	straLine;
	CString		strUrl;
	CString		strRefer;
	LPCSTR		p;
	LPCSTR		pcNl;
	int			iRemain;
	int			iLineLen;
	int			iIndexT;
	int			iSepLen;	//�ָ�������

	p = (LPCSTR) pucData;
	pcNl = NULL;
	iRemain = size;
	while (iRemain > 0)
	{
		pcNl = (LPCSTR) memchr(p, '\n', iRemain);
		if (NULL == pcNl)
		{
			iSepLen = 0;
			iLineLen = iRemain;
		}
		else
		{
			iSepLen = 1;

			if (pcNl > p							// ȷ�� (pcNl - 1) �ɶ�
				&& *(pcNl - 1) == '\r')				//����ָ����ǡ�\r\n������ָ�������Ϊ 2��
				iSepLen = 2;

			iLineLen = pcNl + 1 - p;
		}

		if (iLineLen <= 0)
			break;

		//	ȡ��һ������
		int iCopyStrLen = iLineLen - iSepLen;
		char *pCopy = new char[iCopyStrLen + 1];
		strncpy(pCopy, p, iCopyStrLen);
		pCopy[iCopyStrLen] = 0;

		straLine = pCopy;

		delete[] pCopy;
		pCopy = NULL;

		//	��ָ���ƶ�����һ��
		p += iLineLen;
		iRemain -= iLineLen;

		//	��һ�����ݽ�������������ӵ��б��С�
		iIndexT = straLine.Find('\t');
		if (-1 == iIndexT)
		{
			strUrl = straLine;
			strRefer.Empty();
		}
		else
		{
			strUrl = straLine.Left(iIndexT);
			strRefer = straLine.Mid(iIndexT + 1);
		}

		CStringW	strUrlW;
		strUrlW = strUrl;
		if (S_OK == ::IsValidURL(NULL, strUrlW, 0))
			m_pMgr->m_strlstFetchedUrlSources.AddTail(CString(straLine));	//����������������б��Ա���Refer��Ϣ��
	}

	if (NULL != m_pMgr->m_pAssocPartFile)
		m_pMgr->m_pAssocPartFile->OnUrlSrcFromSvrFetched(&m_pMgr->m_strlstFetchedUrlSources);

	return true;
}
bool CUrlSrcGetFromSvrSocket::ProcessHttpResponse_Finished()
{
	return false;
}
bool CUrlSrcGetFromSvrSocket::ProcessHttpResponseBody_Finished(const BYTE* /*pucData*/, UINT /*size*/)
{
	return false;
}
