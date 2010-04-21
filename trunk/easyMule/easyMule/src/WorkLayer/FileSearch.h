#ifndef _H_FILESEARCH_H_
#define _H_FILESEARCH_H_

#include "../zlib/zlib.h"
#include "ListenSocket.h"
#include "HttpClientReqSocket.h"
#include "Searchlist.h"

struct SearchItem
{
	CStringA name;
	double size;
	CStringA hash;
	UINT completesource;
	UINT weeklyStart;
};

class CSearchListCtrl;

class CFileSearchReqSocket: public CHttpClientReqSocket
{
public:
	CFileSearchReqSocket();
	virtual ~CFileSearchReqSocket();
	bool		SendRequest(CFileSearch* pFileSearch);
	CStringA	GetServer(){return "www.verycd.com";}
	int			GzipDecompress(Bytef *&dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
	void		ProcessSearchResult(const BYTE* pucData, UINT size);

protected:
	virtual void	DataReceived(const BYTE* pucData, UINT uSize);
	virtual bool	ProcessHttpResponse();
	virtual bool	ProcessHttpResponseBody(const BYTE* pucData, UINT size);
	virtual void	OnConnect(int nErrorCode);
	virtual void	OnClose(int nErrorCode);

public:
	CStringA m_szSearchURL;
	CFileSearch *m_pFileSearch;
	BYTE		*m_pBuffer;
	int			m_nMaxSize;
	int			m_nLength;
};

class CFileSearch 
{
public:
	CFileSearch( );
	~CFileSearch( );

public:
	bool Start( const CString& strKeyValue);
	bool Stop( );

	CWinThread*  m_pFileSearchThread; 
	CSearchListCtrl* m_pSearchListCtrl;
	DWORD m_dwSearchID;
	CString m_strKeyValue;
	CFileSearchReqSocket *m_pSearchReqSocket;

protected:
	bool m_bNeedToStop;
	CArray<SearchItem>	m_FileSearchItems;

	//static UINT AFX_CDECL FileSearchThreadEntry(LPVOID lpParam);
};



#endif//_H_FILESEARCH_H_
