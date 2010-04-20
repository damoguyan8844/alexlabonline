// IE2EMUrlTaker.cpp : CIE2EMUrlTaker µÄÊµÏÖ

#include "stdafx.h"
#include "IE2EMUrlTaker.h"
#include ".\ie2emurltaker.h"


// CIE2EMUrlTaker


STDMETHODIMP CIE2EMUrlTaker::SendUrl(BSTR strUrl, BSTR strInfo, BSTR strRef,VARIANT_BOOL* prVB)
{
	if(strUrl[0] != 'e' && strUrl[0] != 'E')
	{
		*prVB = VARIANT_FALSE;
		return S_OK;
	}

	CRegKey hkey;

	if(hkey.Open(HKEY_CURRENT_USER, _T("Software\\eMule"), KEY_READ) != ERROR_SUCCESS)
	{
		*prVB = VARIANT_FALSE;
		return S_OK;
	}

	TCHAR rBuffer[MAX_PATH];
	ULONG maxsize = MAX_PATH;
	LONG lRes = hkey.QueryStringValue(NULL, rBuffer, &maxsize);

	hkey.Close();

	if(lRes != ERROR_SUCCESS)
	{
		*prVB = VARIANT_FALSE;	
		hkey.Close();
		return S_OK;
	}

	CComBSTR path(rBuffer);
	path.Append("\\eMule.exe");
	if(!PathFileExists(path))
	{
		* prVB = FALSE;
		return S_OK;
	}
	CComBSTR str("\"");
	str.Append(path);
	str.Append("\" \"");
	str.Append(strUrl);
	str.Append("\"");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	if(!CreateProcess( NULL, str.Copy(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ))
	{
		*prVB = VARIANT_FALSE;
		return S_OK;
	}

	*prVB = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CIE2EMUrlTaker::GetEmuleVersion(BSTR* pstrVer)
{
	try
	{
		CRegKey hkey;

		if(hkey.Open(HKEY_CURRENT_USER, _T("Software\\eMule"), KEY_READ) != ERROR_SUCCESS)
		{
			*pstrVer = CComBSTR(_T("")).Copy();
			return S_OK;
		}

		TCHAR rBuffer[MAX_PATH];
		ULONG maxsize = MAX_PATH;
		LONG lRes = hkey.QueryStringValue(_T("Version"), rBuffer, &maxsize);

		hkey.Close();

		if(lRes != ERROR_SUCCESS)
		{
			*pstrVer = CComBSTR(_T("")).Copy();
			hkey.Close();
			return S_OK;
		}

		CComBSTR str;
		str.Append(rBuffer);
		*pstrVer = str.Copy();
		return S_OK;
	}
	catch (...)
	{
		*pstrVer = BSTR(_T(""));
		return S_OK;		
	}

}

STDMETHODIMP CIE2EMUrlTaker::get_Version(BSTR* pVal)
{	
	try
	{
		*pVal = CComBSTR(_T("")).Copy();
		TCHAR   szAppPath[MAX_PATH] =   {0};  
		HMODULE hMySelf = ::GetModuleHandle(_T("IE2EM.dll"));

		::GetModuleFileName(hMySelf,szAppPath,MAX_PATH   );  
		INT   nVersionLen =   ::GetFileVersionInfoSize(szAppPath,NULL);  
		if(   nVersionLen<= 0   )  
		{  			
			return   S_OK;  
		}  

		TCHAR*   pBuffer=new TCHAR[nVersionLen];  
		if(!pBuffer)    
		{  
			return   S_OK;  
		}  

		if(!::GetFileVersionInfo(szAppPath,NULL,nVersionLen,pBuffer))  
		{  
			return   S_OK;  
		}   
		UINT   dwBytes;  
		VS_FIXEDFILEINFO* pFileInf = NULL;
		if(::VerQueryValue(pBuffer, _T("\\"),(LPVOID*)&pFileInf,&dwBytes))  
		{  
			TCHAR cVersion[MAX_PATH]={0};
			_sntprintf(cVersion,MAX_PATH,_T("%d.%d.%d.%d"),HIWORD(pFileInf->dwFileVersionMS),LOWORD(pFileInf->dwFileVersionMS),
				HIWORD(pFileInf->dwFileVersionLS),LOWORD(pFileInf->dwFileVersionLS));						
			CComBSTR str;
			str.Append(cVersion,MAX_PATH);
			*pVal = str.Copy();
		}   

		delete[] pBuffer;
	}
	catch (...)
	{
		*pVal = CComBSTR(_T("exception")).Copy();
	}

	return S_OK;
}
