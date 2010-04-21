#include "StdAfx.h"
#include "SearchlistCtrl.h"
#include "FileSearch.h"

#include "StringConversion.h"
#include "json/json.h"
//#include "libcurl/include/curl/curl.h"
#include "GlobalVariable.h"



using namespace std;

//////////////////////////////////////////////////////////////////////////
///CFileSearch
//////////////////////////////////////////////////////////////////////////

CFileSearch::CFileSearch( )
{
	m_strKeyValue = _T("");
	m_pSearchListCtrl = NULL;
	m_pFileSearchThread = NULL;
	m_bNeedToStop = false;

	m_pSearchReqSocket = new CFileSearchReqSocket;
	
}

CFileSearch::~CFileSearch( )
{

}


bool ParseSearchResult( uint32 nSearchID,
					   unsigned int& recordsTotal, 
					   unsigned int& recordsStart,
					   std::string& strSearchResult )
{
	Json::Value root;
	Json::Reader reader;
	if( reader.parse( strSearchResult, root ) )
	{
		if ( root.type() == Json::objectValue )
		{
			recordsTotal = root["total"].asUInt();
			unsigned int recordsReturned = root["recordsReturned"].asUInt();
			if ( recordsReturned != 0 )
			{
				Json::Value records = root["records"];
				if( records.type() == Json::arrayValue )
				{
					int size = records.size();
					for ( int index =0; index < size; index++ )
					{
						Json::Value item = records[index];
						if ( item.type() == Json::objectValue )
						{
							SearchItem search_item;							
							search_item.name = item["name"].asString().c_str();;
							search_item.size = item["bytes"].asDouble();
							search_item.hash = item["hash"].asString().c_str();
							search_item.completesource = item["weeklyFinish"].asUInt();
							search_item.weeklyStart = item["weeklyStart"].asUInt();
							///name
							CString sFileName = OptUtf8ToStr( search_item.name );
							CSearchFile* pSearchFile = new CSearchFile(nSearchID,sFileName,(unsigned __int64)search_item.size,
								search_item.completesource,search_item.weeklyStart,search_item.hash);
							CGlobalVariable::searchlist->AddToList(pSearchFile,false,0,false,false,false);	
							///names
							char szName [2048 * 2] = {0};
							strncpy(szName,item["names"].asString().c_str(),2048 * 2);
							char * pszTemp = strtok(szName,("|"));
							while(pszTemp != NULL)
							{
								search_item.name = pszTemp;
								if( search_item.name.Left(32)==search_item.hash )
								{
									pszTemp = strtok(NULL,"|");
									continue;
								}
								sFileName = OptUtf8ToStr( search_item.name );
								CSearchFile* pSearchFile = new CSearchFile(nSearchID,sFileName,(unsigned __int64)search_item.size,
								search_item.completesource,search_item.weeklyStart,search_item.hash);
								CGlobalVariable::searchlist->AddToList(pSearchFile,false,0,false,true,false);
								pszTemp = strtok(NULL,"|");
							}
						}
					}
				}
			}
			recordsStart += recordsReturned;
			return true;
		}
	}
	recordsTotal = 0;
	return false;
}
/*
static size_t FileSearchFn(void *buffer, size_t size, size_t nmemb, void *stream)
{
	size_t real_size = size * nmemb;
	std::string * psJSon = reinterpret_cast<std::string*>(stream);
	if ( psJSon )
	{
		psJSon->append(reinterpret_cast<char *>(buffer), real_size);
	}
	return real_size;
}

UINT AFX_CDECL CFileSearch::FileSearchThreadEntry(LPVOID lpParam)
{
	CFileSearch* pFileSearch=(CFileSearch*)lpParam;
	 
	CString strSearchUrl;	
	//CGetHttp getHttp;


	CURL * curl = curl_easy_init();
	if ( !curl )
		return 0;

	strSearchUrl.Format( _T("http://www.verycd.com/search/files/%s?format=json&num=500&sort=weeklyFinish"),URLEncode(EncodeUrlUtf8(pFileSearch->m_strKeyValue)));		

	CStringA strSearchUrlA = CStringA(strSearchUrl);
	curl_easy_setopt( curl, CURLOPT_URL, strSearchUrlA );

	std::string sJSonResult;
	//curl_easy_setopt(curl, CURLOPT_HEADER, "Accept-Encoding: gzip, deflate");
	curl_easy_setopt( curl, CURLOPT_ENCODING ,"gzip");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileSearchFn);	
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(&sJSonResult));
	int iRet = curl_easy_perform( curl );
	if( iRet!=CURLE_OK )
	{
		curl_easy_cleanup( curl );			
		return 0;
	}
	
	int retcode;
	iRet = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
	if (iRet != CURLE_OK || !( retcode>=200 && retcode <= 206 || retcode==304) )
	{
		curl_easy_cleanup( curl );			
		return 0;
	}

	curl_easy_cleanup( curl );				

	SearchListsStruct * pSearchListsStruct = new SearchListsStruct;
	pSearchListsStruct->m_nSearchID = pFileSearch->m_dwSearchID;
	unsigned int recordsTotal;
	unsigned int recordsStart;
	if( ParseSearchResult( pSearchListsStruct, recordsTotal, recordsStart, sJSonResult) )
	{

		CGlobalVariable::searchlist->AddResults( pSearchListsStruct );
		if( pFileSearch->m_pSearchListCtrl )
			pFileSearch->m_pSearchListCtrl->ShowResults(pSearchListsStruct->m_nSearchID);

		//if( CGlobalVariable::m_hListenWnd )
		//	SendMessage(CGlobalVariable::m_hListenWnd, WM_SEARCH_SHOWFILERESULT, 0, (LPARAM)pFileSearch->m_dwSearchID);				
		return 0;

	}
	else
	{
		return 0;
	}

		
	return 0;
}
*/
bool CFileSearch::Start( const CString& strKeyValue )
{
	m_strKeyValue = strKeyValue;

	/*
		m_pFileSearchThread = AfxBeginThread(FileSearchThreadEntry, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_pFileSearchThread->m_bAutoDelete = true;
		m_pFileSearchThread->ResumeThread();
	*/
	m_pSearchReqSocket->SendRequest(this);
	return true;
}

bool CFileSearch::Stop( )
{
	m_bNeedToStop = true;
	return true;
}


CFileSearchReqSocket::CFileSearchReqSocket()
{
	m_pFileSearch = NULL;
	m_pBuffer = NULL;
	m_nMaxSize = m_nLength = 0;
}

CFileSearchReqSocket::~CFileSearchReqSocket()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

bool CFileSearchReqSocket::SendRequest(CFileSearch* pFileSearch)
{
	m_pFileSearch = pFileSearch;

	if (!Create())
		return false;

	CString strSearchUrl;	
	strSearchUrl.Format( _T("/search/files/%s?format=json&num=500&sort=weeklyFinish"),URLEncode(EncodeUrlUtf8(pFileSearch->m_strKeyValue)));		
	m_szSearchURL = CStringA(strSearchUrl);
	Connect(GetServer(), 80);
	return true;
}


void CFileSearchReqSocket::OnConnect(int nErrorCode)
{
	if (0 != nErrorCode)
	{
		if (thePrefs.GetVerbose())
		{
			AddDebugLogLine(false, _T("电驴搜索，但连接%s返回失败。"),CString(GetServer()));
		}
		return;
	}

	CStringA strHttpRequest;
	strHttpRequest.AppendFormat("GET %s HTTP/1.0\r\n",m_szSearchURL);
	strHttpRequest.AppendFormat("Host: %s\r\n", GetServer());
	strHttpRequest.AppendFormat("Accept: */*\r\n");
	strHttpRequest.AppendFormat("Accept-Encoding: gzip\r\n");
	strHttpRequest.Append("\r\n");

	if (thePrefs.GetVerbose())
	{
		AddDebugLogLine(false, _T("与服务器 %s 连接成功，准备发送:"), CString(GetServer()));	
	}

	CRawPacket* pHttpPacket = new CRawPacket(strHttpRequest);
	SendPacket(pHttpPacket);
	SetHttpState(HttpStateRecvExpected);
}

void CFileSearchReqSocket::DataReceived(const BYTE* pucData, UINT uSize)
{
	/*
	if (thePrefs.GetVerbose())
	{
		CStringA	straRecv((char *)pucData, uSize);
		AddDebugLogLine(false, CString(straRecv));
	}
	*/
	
	CHttpClientReqSocket::DataReceived(pucData, uSize);
}


bool CFileSearchReqSocket::ProcessHttpResponse()
{
	int iMajorVer, iMinorVer;
	int iResponseCode;
	char szResponsePhrase[1024];
	sscanf(m_astrHttpHeaders[0], "HTTP/%d.%d %d %s", &iMajorVer, &iMinorVer, &iResponseCode, szResponsePhrase);

	if (thePrefs.GetVerbose())
		AddDebugLogLine(false, _T("Receive search from server (http response code = %d)"), iResponseCode);

	if (200 != iResponseCode)
		return false;

	return true;
}


int CFileSearchReqSocket::GzipDecompress(Bytef *&dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
	int err;
	z_stream stream = {0};
	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;

	err = inflateInit2(&stream, -MAX_WBITS);
	if (err != Z_OK)
		return err;
	*destLen = *(uLong *)&source[sourceLen - 4];
	dest = new Bytef[*destLen + 1];

	stream.next_in = (Bytef*) source + 10 ;
	stream.avail_in = (uInt) sourceLen - 18;
	stream.next_out = (Bytef*) dest;
	stream.avail_out = *destLen;
	// doit
	err = inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END)
	{
		inflateEnd(&stream);
		return err;
	}
	err = inflateEnd(&stream);
	dest[*destLen] = 0;

	return err;
}

void CFileSearchReqSocket::OnClose(int nErrorCode)
{
	LPBYTE pucData = m_pBuffer;
	int size = m_nLength;

	if (0 == nErrorCode && pucData && size > 0)
	{
		if(pucData[0] == 0x1f && pucData[1] == 0x8b)
		{
			Bytef * dest = NULL;
			uLong dlen;
			GzipDecompress(dest, &dlen, pucData, size);
			if(dest != NULL)
			{
				ProcessSearchResult(dest,dlen);
				delete []dest;
			}
		}
		else
			ProcessSearchResult(pucData,size);
	}

	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

bool CFileSearchReqSocket::ProcessHttpResponseBody(const BYTE* pucData, UINT size)
{
	if (size > m_nMaxSize - m_nLength)
	{
		int n = max(size,1024);
		int m = n + m_nMaxSize;
		BYTE * pData = (LPBYTE)malloc(m * sizeof(BYTE));
		if (m_pBuffer)
		{
			memcpy(pData,m_pBuffer,m_nLength);
			free(m_pBuffer);
		}

		memcpy(pData + m_nLength,pucData,size);
		m_nMaxSize = m;
		m_nLength += size;
		m_pBuffer = pData;
	}
	else
	{
		memcpy(m_pBuffer + m_nLength,pucData,size);
		m_nLength += size;
	}
	return true;
}

void CFileSearchReqSocket::ProcessSearchResult(const BYTE* pucData, UINT size)
{
	unsigned int recordsTotal,recordsStart;
	std::string sJSonResult((char*)pucData,size);

	if(ParseSearchResult(m_pFileSearch->m_dwSearchID, recordsTotal, recordsStart, sJSonResult) )
	{
		if( m_pFileSearch->m_pSearchListCtrl )
		{
			m_pFileSearch->m_pSearchListCtrl->ShowResults(m_pFileSearch->m_dwSearchID);
		}

		return;
	}	
	return;
}