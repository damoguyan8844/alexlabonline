#include "stdafx.h"
#include "WordFilter.h"
#include "Preferences.h"
#include "Log.h"
#include "GetHttp.h"

#include <fstream>
#include <string>
#include <vector>
#include "StringConversion.h"

#define  UPDATE_TIME 1000 * 60 * 60 * 24 
#define  UPDATE_URL _T("http://download.verycd.com/wf/wordfilter.txt")

using namespace std;

namespace InternalBase64
{
	char encode(unsigned char uc)
	{
		if(uc < 26)
		{
			return 'A' + uc;
		}

		if(uc < 52)
		{
			return 'a' + (uc - 26);
		}

		if(uc < 62)
		{
			return '0' + (uc - 52);
		}

		if(uc == 62)
		{
			return '+';
		}

		return '/';
	}

	unsigned char decode(char c)
	{
		if(c >= 'A' && c <= 'Z')
		{
			return c - 'A';
		}

		if(c >= 'a' && c <= 'z')
		{
			return c - 'a' + 26;
		}

		if(c >= '0' && c <= '9')
		{
			return c - '0' + 52;
		}

		if(c == '+')
		{
			return 62;
		}

		return 63;
	}

	bool isBase64(char c)
	{
		if(c >= 'A' && c <= 'Z')
		{
			return true;
		}

		if(c >= 'a' && c <= 'z')
		{
			return true;
		}

		if(c >= '0' && c <= '9')
		{
			return true;
		}

		if(c == '+')
		{
			return true;
		}

		if(c == '/')
		{
			return true;
		}

		if(c == '=')
		{
			return true;
		}

		return false;
	}

	bool isBase64( const std::string & str )
	{
		for( size_t i = 0 ; i < str.size() ; i++ ) {
			if( !isBase64( str[i] ) ) {
				return false;
			}
		}

		return true;
	}

	string encode(const vector<unsigned char>& plainSeq)
	{
		string retval;

		if(plainSeq.size() == 0) 
		{
			return retval;
		}

		// Reserve enough space for the returned base64 string
		unsigned long base64Bytes = (((plainSeq.size() * 4) / 3) + 1);
		unsigned long newlineBytes = (((base64Bytes * 2) / 76) + 1);
		unsigned long totalBytes = base64Bytes + newlineBytes;

		retval.reserve(totalBytes);

		unsigned char by1 = 0;
		unsigned char by2 = 0;
		unsigned char by3 = 0;
		unsigned char by4 = 0;
		unsigned char by5 = 0;
		unsigned char by6 = 0;
		unsigned char by7 = 0;

		for(unsigned long i = 0; i < plainSeq.size(); i += 3)
		{
			by1 = plainSeq[i];
			by2 = 0;
			by3 = 0;

			if((i + 1) < plainSeq.size())
			{
				by2 = plainSeq[i+1];
			}

			if((i + 2) < plainSeq.size())
			{
				by3 = plainSeq[i+2];
			}

			by4 = by1 >> 2;
			by5 = ((by1 & 0x3) << 4) | (by2 >> 4);
			by6 = ((by2 & 0xf) << 2) | (by3 >> 6);
			by7 = by3 & 0x3f;

			retval += InternalBase64::encode(by4);
			retval += InternalBase64::encode(by5);

			if((i + 1) < plainSeq.size())
			{
				retval += encode(by6);
			}
			else
			{
				retval += "=";
			}

			if((i + 2) < plainSeq.size())
			{
				retval += encode(by7);
			}
			else
			{
				retval += "=";
			}
		}

		string outString;
		outString.reserve(totalBytes);
		string::iterator iter = retval.begin();

		while((retval.end() - iter) > 76)
		{
			copy(iter, iter+76, back_inserter(outString));
			outString += "\r\n";
			iter += 76;
		}

		copy(iter, retval.end(), back_inserter(outString));

		return outString;
	}

	vector<unsigned char> decode(const string& str)
	{
		string newStr;

		newStr.reserve(str.length());

		for(unsigned long j = 0; j < str.length(); j++)
		{
			if(isBase64(str[j]))
			{
				newStr += str[j];
			}
		}

		vector<unsigned char> retval;

		if(newStr.length() == 0)
		{
			return retval;
		}

		// Note: This is how we were previously computing the size of the return
		//       sequence.  The method below is more efficient (and correct).
		// unsigned long lines = str.size() / 78;
		// unsigned long totalBytes = (lines * 76) + (((str.size() - (lines * 78)) * 3) / 4);

		// Figure out how long the final sequence is going to be.
		unsigned long totalBytes = (newStr.size() * 3 / 4) + 1;

		retval.reserve(totalBytes);

		unsigned char by1 = 0;
		unsigned char by2 = 0;
		unsigned char by3 = 0;
		unsigned char by4 = 0;

		char c1, c2, c3, c4;

		for(unsigned long i = 0; i < newStr.length(); i += 4)
		{
			c1 = 'A';
			c2 = 'A';
			c3 = 'A';
			c4 = 'A';

			c1 = newStr[i];

			if((i + 1) < newStr.length())
			{
				c2 = newStr[i + 1];
			}

			if((i + 2) < newStr.length())
			{
				c3 = newStr[i + 2];
			}

			if((i + 3) < newStr.length())
			{
				c4 = newStr[i + 3];
			}

			by1 = decode(c1);
			by2 = decode(c2);
			by3 = decode(c3);
			by4 = decode(c4);

			retval.push_back((by1 << 2) | (by2 >> 4));

			if(c3 != '=')
			{
				retval.push_back(((by2 & 0xf) << 4) | (by3 >> 2));
			}

			if(c4 != '=')
			{
				retval.push_back(((by3 & 0x3) << 6) | by4);
			}
		}

		return retval;
	}

	bool encode2file( const std::string & src , const std::string & file )
	{
		ofstream fo;
		fo.open( file.c_str() , ios_base::binary );
		if( !fo.is_open() ) {
			return false;
		}

		std::vector<unsigned char> vec;
		for( size_t i = 0 ; i < src.size() ; i ++ ) {
			vec.push_back( *(src.c_str() + i) );
		}

		// 写入 tag
		fo << char(0x15);

		fo << encode( vec ).c_str();
		fo.close();

		return true;
	}

	std::string decodeFromFile( const std::string & file )
	{
		FILE * fp = fopen( file.c_str() , "rb" );
		if( 0 == fp )
			return "";

		string buf;

		char * pBuf = new char[512*1024];
		if( 0 == pBuf )
			return "";

		::memset( pBuf , 0 , 512*1024 );

		int retv = fread( pBuf , 1 , 512 * 1024 , fp );
		fclose( fp );
		if( retv <= 0 || retv == 512 * 1024 ) {
			delete []pBuf;
			return "";
		}

		buf.assign( pBuf , retv );

		delete []pBuf;

		std::vector<unsigned char> vec = decode( buf );
		std::auto_ptr<unsigned char> p( 
			new unsigned char[vec.size() * sizeof(unsigned char) + 1] );

		for( size_t i = 0 ; i < vec.size() ; i++ ) {
			*((char*)p.get() + i ) = (char)vec[i];
		}

		*(((char *)p.get()) + vec.size()) = '\0';
		buf = (char *)p.get();

		return buf;
	}
};


CWordFilter WordFilter;

// KMP Next values generation function
__inline void	CWordFilter::KMP_GetNext(TCHAR * sString, int * iNext)
{
	iNext[0] = -1;
	int i = -1;
	for(int j = 1; sString[j] != 0; j++)
	{
		while(sString[j] != sString[i + 1] && (i >= 0))
			i = iNext[i];
		if(sString[j] == sString[i + 1])
			iNext[j] = i + 1;
		else
			iNext[j] = -1;
		i = iNext[j];
	}
}

// KMP Match function
__inline bool	CWordFilter::KMP_Match(const CString & sString, TCHAR * sSubString, int * iNext)
{
	int i = 0, j = 0;

	while (i < sString.GetLength() && sSubString[j] != 0)
	{
		if (sString[i] == sSubString[j])
		{
			++j; ++i;
		}
		else
		{
			if(j == 0)
				++i;
			j = (j > 0) ? (iNext[j - 1] + 1) : 0;
		}
	}

	return (sSubString[j] == 0);
}

void	CWordFilter::Init()
{
	HANDLE	hFile;
	DWORD	dwRead;
	int		nLen;
	BOOL	bResult;
	CStringList list;

	//m_count = 0;

	m_filterall = false; //应该在这里初始化.VC-dgkang

	// 如果文件目录不对，程序移动一下，到config目录下 added by kernel1983 2006.07.31
	if (PathFileExists(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + FLITER_FILE)) 
		MoveFile(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + FLITER_FILE, thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE);

	if (!PathFileExists(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE))
	{
		// 不存在，所有的都过滤 added by kernel1983 2006.08.08
		m_filterall = true;
		return;
	}

	// Open file for read
	hFile = CreateFile(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//AddLogLine(false,_T(":%s\n"),thePrefs.GetConfigDir() + FLITER_FILE);
	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		// 读取错误，所有的都过滤 added by kernel1983 2006.08.08
		m_filterall = true;
		return;
	}

	DWORD dwSize = GetFileSize(hFile, NULL);

	TCHAR * pszData = new TCHAR[(dwSize / sizeof(TCHAR)) + 1];			// 申请空间
	bResult = ReadFile(hFile, pszData, dwSize, &dwRead, NULL);		// 读入文件1
	CloseHandle(hFile);
	pszData[(dwSize / sizeof(TCHAR))] = 0;

	if(bResult)
	{
		// 加入解码算法
		{
			std::string tempstr( (char*)pszData + 1 , ((int)dwSize - 1) > 0 ? dwSize -1 : 0 );

			// 查看是否是老格式
			char * pszData_a = (char*) pszData;

			if( pszData_a[0] != 0x15 ) {
				// 老格式，进行转换
				CUnicodeToMultiByte wc2mb( thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE );
				tempstr.assign( (char*)pszData , dwSize );
				InternalBase64::encode2file( tempstr , std::string((LPCSTR)wc2mb , wc2mb.GetLength()) );

				delete [] pszData;
				// 重新载入
				nLoadCount++;

				if (nLoadCount > 3)
					return;

				return Init();
			}

			vector<unsigned char> vec = InternalBase64::decode( tempstr );
			char * pszt = (char*) pszData;
			for( size_t i = 0; i < vec.size() ; i++ ) {
				pszt[i] = vec[i];
			}
			dwSize = vec.size();
			pszData[(dwSize / sizeof(TCHAR))] = 0;
		}		

		TCHAR * pszTemp = wcstok(pszData + 1, _T("\r\n"));
		while(pszTemp != NULL)
		{
			nLen = wcslen(pszTemp);
			while(pszTemp[nLen - 1] == '\t' || pszTemp[nLen - 1] == ' ')
			{
				nLen --;
				pszTemp[nLen] = 0;
			}
			while(*pszTemp == '\t' || *pszTemp == ' ')
			{
				pszTemp ++;
				nLen --;
			}
			//AddLogLine(false,_T("pszTemp:%s"),pszTemp);
			//AddLogLine(false,_T("nLen:%d"),nLen);
			if(nLen > 0)list.AddTail(pszTemp);
			//if(nLen == 8)AddLogLine(false,_T(":%d %d %d %d "),((char*)pszTemp)[0],((char*)pszTemp)[1],((char*)pszTemp)[2],((char*)pszTemp)[3]);
			pszTemp = wcstok(NULL, _T("\r\n"));
		}
	}

	delete[] pszData;

	m_count = list.GetCount();
	//AddLogLine(false,_T("m_count:%d"),m_count);

	if(bResult && m_count > 0)
	{
		m_filterwords = new TCHAR*[m_count+1];
		m_kmpvalue = new int*[m_count+1];
		ZeroMemory(m_filterwords, sizeof(TCHAR *) * m_count);
		ZeroMemory(m_kmpvalue, sizeof(int *) * m_count);
	}

	for(int i = 0; bResult && (i < m_count); i ++)
	{
		CString s = list.GetAt(list.FindIndex(i));
		s.MakeLower();
		nLen = s.GetLength();
		//AddLogLine(false,_T("nLen:%d"),nLen);
		m_filterwords[i] = new TCHAR[nLen + 1];
		m_filterwords[i][nLen] = 0;	// 最后一个字节设为0
		m_kmpvalue[i] = new int[nLen];
		//AddLogLine(false,_T("nLen:%d"),nLen);
		_tcscpy(m_filterwords[i],s);
		//AddLogLine(false,_T("m_filterwords[i]:%s"),m_filterwords[i]);
		KMP_GetNext(m_filterwords[i], m_kmpvalue[i]);	// 得到一个与内容有关的数值m_kmpvalue[i]
	}

	if(m_count == 0 || !bResult)
	{
		Free();
		//m_filterall = true;
	}
}


bool	CWordFilter::VerifyString(const CString & sString)	// 验证字符是否合法
{
	bool bIsRm = sString.Right(3)==_T(".rm");
	CString sReduceString=sString;
	CString sInterpunctionString = _T("（），().。·；：－《》『』～　“”〓！【】★×┇");
	try // VC-Huby[2007-03-20]:满足中国国情特色,加强过滤
	{		
		int j=0;
		for( int i=0; i<sString.GetLength(); i++ )
		{
			if( sString.GetAt(i)<=_T('/') && sString.GetAt(i)>=_T(' ') ) //从空格到'/'之间的字符减掉后再过滤
			{
				continue;
			}
			else if( sString.GetAt(i)<=_T('@') && sString.GetAt(i)>=_T(':') )
			{
				continue;
			}
			else if( sString.GetAt(i)<=_T('`') && sString.GetAt(i)>=_T('[') )
			{
				continue;
			}
			else if( sString.GetAt(i)<=_T('~') && sString.GetAt(i)>=_T('{') )
			{
				continue;
			}
			else if( sInterpunctionString.Find(sString.GetAt(i))>=0 )
			{
				continue;
			}
			else
			{
				sReduceString.SetAt(j,sString.GetAt(i));
				j++;
			}
		}
		if( j<sString.GetLength() )
			sReduceString.SetAt(j,_T('\0'));
	}
	catch (...) 
	{
	}

	if(m_filterall){
		//AddLogLine(false,_T("m_filterall"));
		return true;	// 检测不到文件，或者读取错误的情况下放弃过滤
	}
	if(m_count == 0){
		//AddLogLine(false,_T("m_count == 0"));
		return true;	// 文件是空的时候，放弃过滤功能
	}
	CString strSearch = ((CString)sReduceString).MakeLower();

	//vc-huby: 过滤中文字符超过15字符
	//CString sReduceString2=strSearch;		
	int k=0;
	for( int i=0; i<strSearch.GetLength(); i++ )
	{
		if( strSearch.GetAt(i)<=_T('9') && strSearch.GetAt(i)>=_T('0') )
		{
			continue;
		}
		if( strSearch.GetAt(i)<=_T('z') && strSearch.GetAt(i)>=_T('a') )
		{
			continue;
		}
		else
		{
			k++;
		}
	}

	if( k>=20 && bIsRm )
		return false;
	//int m = sReduceString2.GetLength();
	/*
	if( k>=60 )
	return false;*/


	/*if (strSearch.GetLength() > 20)
	{
	return false;
	}*/

	for(int i = 0; i < m_count; i ++)
	{
		if(KMP_Match(strSearch, m_filterwords[i], m_kmpvalue[i]))
		{
			//AddLogLine(false,_T("KMP_Match"));
			return false;	// 关键词命中了，被fliter了
		}
	}
	//AddLogLine(false,_T("漏掉的"));
	return true;

}

void CWordFilter::StopUpdate()
{
	if (pProcessThread)
	{
		if (pProcessThread->m_hThread)
		{
			ASSERT( !pProcessThread->m_bAutoDelete );

			DWORD dwWaitRes = WaitForSingleObject(pProcessThread->m_hThread, 1300);
			if (dwWaitRes == WAIT_TIMEOUT)
			{
				VERIFY( TerminateThread(pProcessThread->m_hThread, (DWORD)-1) );
				VERIFY( CloseHandle(pProcessThread->m_hThread) );
			}
			else if (dwWaitRes == -1)
			{
				TRACE("*** Failed to wait for process thread termination - Error %u\n", GetLastError());
				ASSERT(0); 
			}
		}
		delete pProcessThread;
		pProcessThread = NULL;
	}
}
void CWordFilter::UpdateWordFilter()
{
	if (::GetTickCount() - dwLastUpdateWordFilter > UPDATE_TIME || !dwLastUpdateWordFilter)
	{
		dwLastUpdateWordFilter = ::GetTickCount();

		if (pProcessThread == NULL)
		{
			pProcessThread = new CWinThread(CWordFilter::fnDownloadWordFilte,this);
			pProcessThread->m_bAutoDelete = FALSE;
			if (!pProcessThread->CreateThread())
			{
				delete pProcessThread;
				pProcessThread = NULL;
			}
		}
	}
}

UINT AFX_CDECL CWordFilter::fnDownloadWordFilte(LPVOID pThis)
{
	try
	{
	CWordFilter * pWordFilter = (CWordFilter *)pThis;
	CGetHttp Http;

	BOOL bRet = Http.DownloadFile(UPDATE_URL,thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("wordfilter.new"));
	if (bRet && PathFileExists(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("wordfilter.new")))
	{
#if 0
		DWORD Att = GetFileAttributes(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE);
		if(Att != INVALID_FILE_ATTRIBUTES &&  Att & FILE_ATTRIBUTE_READONLY)
		{
			Att &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE,Att);
		}
#endif

		MoveFileEx(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) +  _T("wordfilter.new"), thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE,MOVEFILE_REPLACE_EXISTING);
		pWordFilter->Free();
		pWordFilter->Init();		
	}
	if (pWordFilter && pWordFilter->pProcessThread)
	{
		pWordFilter->pProcessThread->m_bAutoDelete = TRUE;
		pWordFilter->pProcessThread = NULL;
	}
	}
	catch(...)
	{
		ASSERT(0);
	}
	return 0;
}

void CWordFilter::Free()
{
	try
{
	for(int i = 0; i < m_count; i ++)
	{
		if(m_filterwords[i])
			delete[] m_filterwords[i];
		if(m_kmpvalue[i])
			delete[] m_kmpvalue[i];
	}
	delete[] m_filterwords;
	delete[] m_kmpvalue;
	nLoadCount = 0;
}
	catch(...)
	{
		ASSERT(0);
	}
}

CWordFilter::~CWordFilter()
{
	Free();

	StopUpdate();
}