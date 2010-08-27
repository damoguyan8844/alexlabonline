// Utility.cpp : Implementation of CUtility
#include "stdafx.h"
#include "ZCode.h"
#include "Utility.h"

#include "CyoEncode.h"
#include "CyoDecode.h"
#include "zlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <map>

#define TEST_BASExx(base,str,expected) \
    printf( "TEST_BASE%s('%s')='%s'", #base, str, expected ); \
    required = cyoBase##base##EncodeGetLength( strlen( str )); \
    encoded = malloc( required ); \
    if (encoded == NULL) { \
	printf( "\n*** ERROR: Unable to allocate buffer for encoding ***\n" ); \
	goto exit; \
    } \
    cyoBase##base##Encode( encoded, str, strlen( str )); \
    if (strcmp( encoded, expected ) != 0) { \
	printf( "\n*** ERROR: Encoding failure ***\n" ); \
	goto exit; \
    } \
    valid = cyoBase##base##Validate( encoded, strlen( encoded )); \
    if (valid < 0) \
    { \
	printf( "\n*** ERROR: Unable to validate encoding (error %d) ***\n", valid ); \
	goto exit; \
    } \
    printf( " [passed]\n" ); \
free( encoded ); encoded = NULL;

#define TEST_BASE64(str,expected) TEST_BASExx(64,str,expected)
#define TEST_BASE32(str,expected) TEST_BASExx(32,str,expected)
#define TEST_BASE16(str,expected) TEST_BASExx(16,str,expected)

#define CHECK_INVALID_BASExx(base,str,res) \
    printf( "CHECK_INVALID_BASE%s('%s')=%d", #base, str, res ); \
    valid = cyoBase##base##Validate( str, strlen( str )); \
    if (valid == 0) \
    { \
	printf( "\n*** ERROR: This is a valid encoding! ***\n" ); \
	goto exit; \
    } \
    if (valid != res) \
    { \
	printf( "\n*** ERROR: Expected a different return code! (%d) ***\n", valid ); \
	goto exit; \
    } \
    printf( " [passed]\n", #base, str ); \
	
#define CHECK_INVALID_BASE16(enc,res) CHECK_INVALID_BASExx(16,enc,res)
#define CHECK_INVALID_BASE32(enc,res) CHECK_INVALID_BASExx(32,enc,res)
#define CHECK_INVALID_BASE64(enc,res) CHECK_INVALID_BASExx(64,enc,res)


using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CUtility
struct Destroyer
{
	Destroyer(char * p) : _p(p)
	{}
	
	~Destroyer()
	{ if (_p) delete[] _p; }
	
	void DestroyNow() { delete[] _p; _p = 0; }
	
	char * _p;
};


STDMETHODIMP CUtility::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IUtility
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUtility::get_Base16Table(BSTR *pVal)
{
	// TODO: Add your implementation code here	
	static const char* const BASE16_TABLE = "0123456789ABCDEF";
	CComBSTR(BASE16_TABLE).CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUtility::get_Base32Table(BSTR *pVal)
{
	// TODO: Add your implementation code here
	static const char* const BASE32_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
	CComBSTR(BASE32_TABLE).CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUtility::get_Base64Table(BSTR *pVal)
{
	// TODO: Add your implementation code here
	static const char* const BASE64_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	CComBSTR(BASE64_TABLE).CopyTo(pVal);
	return S_OK;
}

HRESULT CUtility::reportError(HRESULT hr, const wchar_t * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	wchar_t temp[300];
	memset(temp, 0, sizeof(temp));
	_vsnwprintf(temp, sizeof(temp)/sizeof(wchar_t)-1, fmt, args);
	if (hr == E_OUTOFMEMORY) {
		MEMORYSTATUS stat;
		GlobalMemoryStatus (&stat);
		wchar_t buf[100];
		swprintf(buf, L"[vm: %d/%d]", stat.dwAvailVirtual, stat.dwTotalVirtual);
		wcscat(temp, buf);
	}
	AtlReportError(GetObjectCLSID(), temp, GUID_NULL, hr);
	
	va_end(args);
	
	return hr;
}


STDMETHODIMP CUtility::CompressWithBase16(BSTR Source, BSTR *pVal)
{
	unsigned long nSrc = ::SysStringByteLen(Source);
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase16() - pSrc - new %d bytes failed", nSrc+1);
	
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = compressBound(nSrc);
	char * pDest = new char[nDest];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase16() - pDest - new %d bytes failed", nDest);
	
	Destroyer ddest(pDest);
	int ret = compress((Bytef*)pDest, &nDest, 
		(Bytef*)pSrc, nSrc);
	
	if (ret != Z_OK) 
	{ 	
		return reportError(E_FAIL, L"ZCode::CompressWithBase16() - compress() Error: %d", ret);
	}
	
	/*Encode With Base16*/
	size_t required = cyoBase16EncodeGetLength( strlen( pDest ));
	char* pEncoded  = new char[required];
	if (pEncoded == NULL)
	{
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase16() - pEncoded - new %d bytes failed", required);
	}
	Destroyer dencoded(pEncoded);
	cyoBase16Encode( pEncoded, pDest, strlen( pDest ));
	
	/* Validate encoding */
	int valid = cyoBase16Validate( pEncoded, strlen( pEncoded ));
	if (valid < 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase16() *** ERROR: Encoding failure (error %d) ***\n", valid );
	
	CComBSTR(pEncoded).CopyTo(pVal);
	
	if (*pVal == 0)
		return reportError(E_OUTOFMEMORY, L"TZip::cmpr() - A2WBSTR() returns 0 for %d bytes string", required);

	return S_OK;
}

STDMETHODIMP CUtility::CompressWithBase32(BSTR Source, BSTR *pVal)
{
	// TODO: Add your implementation code here

	unsigned long nSrc = ::SysStringByteLen(Source);
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase32() - pSrc - new %d bytes failed", nSrc+1);
	
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = compressBound(nSrc);
	char * pDest = new char[nDest];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase32() - pDest - new %d bytes failed", nDest);
	
	Destroyer ddest(pDest);
	int ret = compress((Bytef*)pDest, &nDest, 
		(Bytef*)pSrc, nSrc);
	
	if (ret != Z_OK) 
	{ 	
		return reportError(E_FAIL, L"ZCode::CompressWithBase32() - compress() Error: %d", ret);
	}
	
	/*Encode With Base32*/
	size_t required = cyoBase32EncodeGetLength( strlen( pDest ));
	char* pEncoded  = new char[required];
	if (pEncoded == NULL)
	{
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase32() - pEncoded - new %d bytes failed", required);
	}
	Destroyer dencoded(pEncoded);
	cyoBase32Encode( pEncoded, pDest, strlen( pDest ));

	/* Validate encoding */
	int valid = cyoBase32Validate( pEncoded, strlen( pEncoded ));
	if (valid < 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase32() *** ERROR: Encoding failure (error %d) ***\n", valid );

	CComBSTR(pEncoded).CopyTo(pVal);

	if (*pVal == 0)
		return reportError(E_OUTOFMEMORY, L"TZip::cmpr() - A2WBSTR() returns 0 for %d bytes string", required);

	return S_OK;
}

STDMETHODIMP CUtility::CompressWithBase64(BSTR Source, BSTR *pVal)
{
	unsigned long nSrc = ::SysStringByteLen(Source);
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase64() - pSrc - new %d bytes failed", nSrc+1);
	
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = compressBound(nSrc);
	char * pDest = new char[nDest];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase64() - pDest - new %d bytes failed", nDest);
	
	Destroyer ddest(pDest);
	int ret = compress((Bytef*)pDest, &nDest, 
		(Bytef*)pSrc, nSrc);
	
	if (ret != Z_OK) 
	{ 	
		return reportError(E_FAIL, L"ZCode::CompressWithBase64() - compress() Error: %d", ret);
	}
	
	/*Encode With Base64*/
	size_t required = cyoBase64EncodeGetLength( strlen( pDest ));
	char* pEncoded  = new char[required];
	if (pEncoded == NULL)
	{
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase64() - pEncoded - new %d bytes failed", required);
	}
	Destroyer dencoded(pEncoded);
	cyoBase64Encode( pEncoded, pDest, strlen( pDest ));
	
	/* Validate encoding */
	int valid = cyoBase64Validate( pEncoded, strlen( pEncoded ));
	if (valid < 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CompressWithBase64() *** ERROR: Encoding failure (error %d) ***\n", valid );
	
	CComBSTR(pEncoded).CopyTo(pVal);
	
	if (*pVal == 0)
		return reportError(E_OUTOFMEMORY, L"TZip::cmpr() - A2WBSTR() returns 0 for %d bytes string", required);

	return S_OK;
}

STDMETHODIMP CUtility::DecompressWithBase16(BSTR Source, BSTR *pVal)
{
	unsigned long nSrc = ::SysStringLen(Source);
	if (nSrc <= 8)
		return S_OK;
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase16() - new %d bytes failed", nSrc+1);
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = cyoBase16DecodeGetLength( strlen( pSrc ));
    char * pRealSrc = new char[nDest];
    if (pRealSrc == NULL)
    {
        return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase16() *** ERROR: Unable to allocate buffer %d for decoding ***",nDest);
    }
	
    cyoBase16Decode( pRealSrc, pSrc, strlen( pSrc ));
	
	Destroyer drsrc(pRealSrc);
	char * pDest = new char[nDest+1];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase16() - new %d bytes failed", nDest+1);
	Destroyer ddest(pDest);
	
	int ret = uncompress((Bytef*)pDest, &nDest, 
		(Bytef*)pRealSrc, nSrc);
	
	if (ret == Z_OK) {
		pDest[nDest] = '\0';
		CComBSTR(pDest).CopyTo(pVal);
		if (*pVal == 0)
			return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase16() - A2WBSTR() returns 0 for %d bytes string", nDest);
	}
	else
		return reportError(E_FAIL, L"ZCode::DecompressWithBase16() - uncompress() Error: %d", ret);

	return S_OK;
}


STDMETHODIMP CUtility::DecompressWithBase32(BSTR Source, BSTR *pVal)
{
	unsigned long nSrc = ::SysStringLen(Source);
	if (nSrc <= 8)
		return S_OK;
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase32() - new %d bytes failed", nSrc+1);
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = cyoBase32DecodeGetLength( strlen( pSrc ));
    char * pRealSrc = new char[nDest];
    if (pRealSrc == NULL)
    {
        return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase32() *** ERROR: Unable to allocate buffer %d for decoding ***",nDest);
    }

    cyoBase32Decode( pRealSrc, pSrc, strlen( pSrc ));

	Destroyer drsrc(pRealSrc);
	char * pDest = new char[nDest+1];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase32() - new %d bytes failed", nDest+1);
	Destroyer ddest(pDest);
	
	int ret = uncompress((Bytef*)pDest, &nDest, 
		(Bytef*)pRealSrc, nSrc);
	
	if (ret == Z_OK) {
		pDest[nDest] = '\0';
		CComBSTR(pDest).CopyTo(pVal);
		if (*pVal == 0)
			return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase32() - A2WBSTR() returns 0 for %d bytes string", nDest);
	}
	else
		return reportError(E_FAIL, L"ZCode::DecompressWithBase32() - uncompress() Error: %d", ret);
	
	return S_OK;
}


STDMETHODIMP CUtility::DecompressWithBase64(BSTR Source, BSTR *pVal)
{
	unsigned long nSrc = ::SysStringLen(Source);
	if (nSrc <= 8)
		return S_OK;
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase64() - new %d bytes failed", nSrc+1);
	Destroyer dsrc(pSrc);
	
	::ZeroMemory(pSrc, sizeof(pSrc));
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, Source, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	unsigned long nDest = cyoBase64DecodeGetLength( strlen( pSrc ));
    char * pRealSrc = new char[nDest];
    if (pRealSrc == NULL)
    {
        return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase64() *** ERROR: Unable to allocate buffer %d for decoding ***",nDest);
    }
	
    cyoBase64Decode( pRealSrc, pSrc, strlen( pSrc ));
	
	Destroyer drsrc(pRealSrc);
	char * pDest = new char[nDest+1];
	if (pDest == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase64() - new %d bytes failed", nDest+1);
	Destroyer ddest(pDest);
	
	int ret = uncompress((Bytef*)pDest, &nDest, 
		(Bytef*)pRealSrc, nSrc);
	
	if (ret == Z_OK) {
		pDest[nDest] = '\0';
		CComBSTR(pDest).CopyTo(pVal);
		if (*pVal == 0)
			return reportError(E_OUTOFMEMORY, L"ZCode::DecompressWithBase64() - A2WBSTR() returns 0 for %d bytes string", nDest);
	}
	else
		return reportError(E_FAIL, L"ZCode::DecompressWithBase64() - uncompress() Error: %d", ret);
	
	return S_OK;
}

STDMETHODIMP CUtility::CodeReplace(BSTR inTable, BSTR toTable, BSTR src, BSTR *pVal)
{
	// TODO: Add your implementation code here

	unsigned long nInTable = ::SysStringLen(inTable);
	if (nInTable <= 8)
		return S_OK;
	char * pInTable = new char[nInTable+1];
	if (pInTable == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CodeReplace() - inTable - new %d bytes failed", nInTable+1);
	Destroyer dInTable(pInTable);
	
	::ZeroMemory(pInTable, sizeof(pInTable));
	nInTable = ::WideCharToMultiByte(CP_ACP, 0, inTable, -1, pInTable, nInTable+1, 0, 0);
	if (nInTable > 0) nInTable--;

	
	unsigned long nToTable = ::SysStringLen(toTable);
	if (nToTable <= 8)
		return S_OK;
	char * pToTable = new char[nToTable+1];
	if (pToTable == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CodeReplace() - ToTable - new %d bytes failed", nToTable+1);
	Destroyer dToTable(pToTable);
	
	::ZeroMemory(pToTable, sizeof(pToTable));
	nToTable = ::WideCharToMultiByte(CP_ACP, 0, toTable, -1, pToTable, nToTable+1, 0, 0);
	if (nToTable > 0) nToTable--;
	
	unsigned long nSrc = ::SysStringLen(src);
	if (nSrc <= 8)
		return S_OK;
	char * pSrc = new char[nSrc+1];
	if (pSrc == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CodeReplace() - Src - new %d bytes failed", nSrc+1);
	Destroyer dSrc(pSrc);
	::ZeroMemory(pSrc, sizeof(pSrc));
	
	nSrc = ::WideCharToMultiByte(CP_ACP, 0, src, -1, pSrc, nSrc+1, 0, 0);
	if (nSrc > 0) nSrc--;
	
	std::map<char,char> replacer;
	for(long index=0; index <nToTable && index< nInTable; index++)
		replacer[*(pInTable+index)]=*(pToTable+index);
	
	std::map<char,char>::iterator iter;
	for(long i=0; i<nSrc; i++)
	{
		iter=replacer.find(*(pSrc+i));
		if(iter!=replacer.end())
			*(pSrc+i)=iter->second;
	}

	*pVal=CComBSTR(pSrc).Detach();

	if (*pVal == 0)
		return reportError(E_OUTOFMEMORY, L"ZCode::CodeReplace() - A2WBSTR() returns 0 for %d bytes string", nSrc);

	return S_OK;
}
