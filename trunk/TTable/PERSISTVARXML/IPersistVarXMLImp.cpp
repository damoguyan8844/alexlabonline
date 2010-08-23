/////////////////////////////////////////////////////
//											
// This is a part of the ATL PersistXML add-on
// 
// Copyright (c) VetalyKartanosv.  All rights reserved.
//
/////////////////////////////////////////////////////

#include "../StdAfx.h"
#include "../comdate.h"
#include "IPersistVarXML.h"

#include "atlbase.h"
extern CComModule _Module;
#include "atlcom.h"
#include "atlsax.h"
#include "base64.h"


HRESULT AtlIPersistVarXML_Load(ISAXAttributes * pAttributes, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	USES_CONVERSION;
	HRESULT hr;
	wchar_t*	wchValue;
	int			chValue;
	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		wchar_t* wchPropName = OLE2W((LPOLESTR)(pMap[i].szDesc));
		CComVariant var;
		//var.vt = pMap[i].vt;
		// If raw entry -  skip it, - we don't handle it
		if (pMap[i].dwSizeData != 0)
		{
			//first we get the bstr (corresponding with the Prop Entry) from the xml
			if(SUCCEEDED(hr = pAttributes->getValueFromQName(
				(unsigned short *)wchPropName,
				wcslen(wchPropName),
				(unsigned short**)&wchValue,
				&chValue)))
			{
				var = wchValue;
			}
			else
				continue;
			
            if (VT_ARRAY == (pMap[i].vt & VT_ARRAY))
            {
                SAFEARRAY** ppsa = (SAFEARRAY**) (pMap[i].dwOffsetData + (DWORD)pThis);

				CComPtr<IStream> pStm;
				hr = CreateStreamOnHGlobal(NULL/*hGlob*/, TRUE, &pStm);
				try{
					if (pStm)
					{					
						BYTE* pRaw;
						
						var.ChangeType(VT_BSTR);
						unsigned int lBytes;
						lBytes=::SysStringLen(var.bstrVal);
						
						//pEncoded= (BYTE*)calloc(lBytes, sizeof(BYTE));
						_bstr_t bstr(var.bstrVal);
						BYTE *pEncoded = (BYTE*)T2A((TCHAR*)bstr);
						CBase64 base64;
						pRaw=(BYTE*) base64.Decode((char*)pEncoded, (int *)&lBytes);

						pStm->Write(pRaw,lBytes,NULL);
						free(pRaw);

						STATSTG statstg;
						pStm->Stat(&statstg,STATFLAG_DEFAULT);
						lBytes=statstg.cbSize.QuadPart;
						LARGE_INTEGER l;
						l.QuadPart = 0;
						pStm->Seek(l,STREAM_SEEK_SET,NULL);	


						// read VARIANT type
						VARTYPE vartype;
						hr = pStm->Read(&vartype, sizeof(VARTYPE), NULL);

						// read dimensions
						long nDim = 0;
						hr = pStm->Read(&nDim, sizeof(long), NULL);

						// read bounds
						long lTotalElements = 1;
						SAFEARRAYBOUND* psabound = new SAFEARRAYBOUND[nDim];
						hr = pStm->Read(psabound, nDim * sizeof(SAFEARRAYBOUND), NULL);
    					VARTYPE basictype = vartype & ~(VT_ARRAY);
						if (SUCCEEDED(hr))
						{
							// strip out any VT_ARRAY or VT_BYREF from the VARTYPE
							// as this will cause SafeArrayCreate to fail
							if(*ppsa)
								::SafeArrayDestroy(*ppsa);
							if(nDim>0)
								*ppsa = ::SafeArrayCreate(basictype, nDim, psabound);
                    
							for (long j=0; j<nDim; j++)
								lTotalElements *= psabound[j].cElements;
						}

						delete [] psabound;
						if (*ppsa)
						{
							long lElemSize = ::SafeArrayGetElemsize(*ppsa);
							LPVOID pData = NULL;
							::SafeArrayAccessData(*ppsa, &pData);
							if (VT_VARIANT == basictype)
							{
								VARIANT* pbstr = static_cast<VARIANT*>(pData);
								for (int j=0; j<lTotalElements; j++)
								{
									CComVariant var;
									var.ReadFromStream(pStm);
									//CopyMemory(pbstr, (void*)&var, sizeof(VARIANT*));	
									var.Detach(pbstr);
									pbstr++;
								}
							}
							else if (VT_BSTR == basictype)
							{
								BSTR* pbstr = static_cast<BSTR*>(pData);
								for (int j=0; j<lTotalElements; j++)
								{
									int i_len = 0;
									pStm->Read(&i_len, sizeof(int), NULL);
									OLECHAR* p_olechar = new OLECHAR[i_len/sizeof(OLECHAR)];
									pStm->Read(p_olechar, i_len, NULL);
									BSTR bstr_data = ::SysAllocStringLen(p_olechar, i_len/sizeof(OLECHAR));
									CopyMemory(pbstr, (void*)&bstr_data, sizeof(BSTR*));
									delete[] p_olechar;
									pbstr++;
								}
							}
							else
							{
								hr = pStm->Read(pData, lTotalElements * lElemSize, NULL);
							}
							::SafeArrayUnaccessData(*ppsa);
						} // if (*ppsa)
						pStm.Release();
					}// if (pStm)..
				}// try
				catch( ... )
				{
					pStm.Release();
					//log it
				}				
                continue;
            }
			else// for all other basic types, long, double, short, byte, etc. 
			//Also BSTR is included
			{
				void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
				hr = var.ChangeType(pMap[i].vt);
				if (SUCCEEDED(hr))
				{
					// check the type - we only have deal with the limited set
					switch (pMap[i].vt)
					{
					case VT_BSTR:
						{
							//free the bstr string if it has been allocated					
							BSTR* pp_data = (BSTR*) pData ;
							//if (::SysStringByteLen(*pp_data)>0)
							if (*pp_data)
							{
								::SysFreeString(*pp_data);
							}
							
							CComBSTR bstr=var.bstrVal;
							BSTR bstr_data = bstr.Detach();
							CopyMemory(pData, (void*)&(bstr_data), sizeof(DWORD));
						}
						break;
					case VT_UI1:
					case VT_I1:
						*((BYTE*)pData) = var.bVal;
						break;
					case VT_BOOL:
						*((VARIANT_BOOL*)pData) = var.boolVal;
						break;
					case VT_UI2:
					case VT_I2:
						*((short*)pData) = var.iVal;
						break;
					case VT_UI4:
					case VT_I4:
					case VT_INT:
					case VT_UINT:
						*((long*)pData) = var.lVal;
						break;
					case VT_R4:
						*((float*)pData) = var.fltVal;
						break;
					case VT_R8:
						*((double*)pData) = var.dblVal;
						break;
					case VT_DATE:
						*((DATE*)pData) = var.date;
						break;
					}
				}
			}
			continue;
		}

		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				return E_FAIL;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if(SUCCEEDED(hr = pAttributes->getValueFromQName(
							(unsigned short *)wchPropName,
							wcslen(wchPropName),
							(unsigned short**)&wchValue,
							&chValue)))
							{
								var = wchValue;
							}

		if (FAILED(hr))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Property %s not in XML Attributes\n"), OLE2CT(pMap[i].szDesc));
			continue;
		}

		if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT AtlIPersistVarXML_Save(IMXAttributes*	pIMXAttributes, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	HRESULT hr;
	USES_CONVERSION;
	if (pIMXAttributes == NULL)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("pIMXAttributes pointer passed in was invalid\n"));
		return E_POINTER;
	}

	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		CComBSTR bstrPropName(pMap[i].szDesc);
		CComVariant var;

		// If raw entry skip it - we don't handle it
		if (pMap[i].dwSizeData != 0)
		{
			if (VT_BSTR == pMap[i].vt)
			{
				CComPtr<IStream> spStream;
				hr = CreateStreamOnHGlobal(NULL/*hGlob*/, TRUE, &spStream);
				if (spStream)
				{
					BSTR* pp_data = (BSTR*) (pMap[i].dwOffsetData + (DWORD)pThis);
					if (::SysStringByteLen(*pp_data)>0)
					{
						int i_len = ::SysStringByteLen(*pp_data)+sizeof(OLECHAR);
						spStream->Write(&i_len, sizeof(int), NULL);
						spStream->Write(*pp_data, i_len, NULL);
					}
					else //treat the null BSTR value as empty string ""
					{
						CComBSTR bstr(_T(""));
						bstr.WriteToStream(spStream);
					}
					LARGE_INTEGER l;
					l.QuadPart = 0;
					spStream->Seek(l,STREAM_SEEK_SET,NULL);
					CComBSTR str; 
					str.ReadFromStream(spStream);
					spStream.Release();
					hr = pIMXAttributes->addAttribute(CComBSTR(""),bstrPropName,bstrPropName,CComBSTR("CDATA"),str);
				}
			}
            else if (VT_ARRAY == (pMap[i].vt & VT_ARRAY))
			{
                SAFEARRAY** ppsa = (SAFEARRAY**) (pMap[i].dwOffsetData + (DWORD)pThis);

				CComPtr<IStream> pStm;
				hr = CreateStreamOnHGlobal(NULL/*hGlob*/, TRUE, &pStm);
				try
				{
					if (pStm)
					{
						// Dump the VARIANT type
						hr = pStm->Write(&pMap[i].vt, sizeof(VARTYPE), NULL);
						if (*ppsa)
						{
							// Dump the dimensions
							long nDim = ::SafeArrayGetDim(*ppsa);
							hr = pStm->Write(&nDim, sizeof(long), NULL);

							// Dump the bounds
							SAFEARRAYBOUND* psabound = new SAFEARRAYBOUND[nDim];
							long lUBound;
							long lTotalElements = 1;
							for (long j=0; j<nDim; j++)
							{
								hr = ::SafeArrayGetLBound(*ppsa, j+1, &psabound[j].lLbound);
								if (FAILED(hr))
									break;
								hr = ::SafeArrayGetUBound(*ppsa, j+1, &lUBound);
								if (FAILED(hr))
									break;
								psabound[j].cElements = lUBound - psabound[j].lLbound + 1;

								lTotalElements *= psabound[j].cElements;
							}
							if (SUCCEEDED(hr))
								hr = pStm->Write(psabound, nDim * sizeof(SAFEARRAYBOUND), NULL);

							delete[] psabound;

							// Dump the element array
							long lElemSize = ::SafeArrayGetElemsize(*ppsa);
							LPVOID pData = NULL;
							::SafeArrayAccessData(*ppsa, &pData);
							// I handle both BSTRs and basic value types; no interface ptrs for this
							// version. So don't try it!
							if (VT_VARIANT == (pMap[i].vt & VT_VARIANT))
							{
								VARIANT* var_data = (VARIANT*)(pData);
								if (lElemSize > 0) // lTotalElements can be 1, so we need another check for an empty SAFEARRAY
								{
									for (int k=0; k<lTotalElements; k++)
									{
										CComVariant var=*var_data;
										if( var.vt==VT_NULL||var.vt==VT_EMPTY )
										{
											CComBSTR bstr(_T(""));
											var=bstr;
										}
										var.WriteToStream(pStm);
										var_data++;
									}
								}
							}
							else if (VT_BSTR == (pMap[i].vt & VT_BSTR))
							{
								BSTR* bstr_data = (BSTR*)(pData);
								// handle elements as BSTRs
								if (lElemSize > 0) // lTotalElements can be 1, so we need another check for an empty SAFEARRAY
								{
									for (int k=0; k<lTotalElements; k++)
									{
										if (::SysStringByteLen(*bstr_data)>0)
										{
											int i_len = ::SysStringByteLen(*bstr_data)+sizeof(OLECHAR);
											pStm->Write(&i_len, sizeof(int), NULL);
											pStm->Write(*bstr_data, i_len, NULL);
										}
										else //treat the null BSTR value as empty string ""
										{
											CComBSTR bstr(_T(""));
											bstr.WriteToStream(pStm);
										}
										bstr_data++;
									}
								}
							}
							else
							{
								// handle other basic types
								hr = pStm->Write(pData, lTotalElements * lElemSize, NULL);
							}
							::SafeArrayUnaccessData(*ppsa);
						} //if (*ppsa)

						///***************************************************************
						//convert the stream into a Byte*
						STATSTG statstg;
						pStm->Stat(&statstg,STATFLAG_DEFAULT);
						unsigned int lBytes=statstg.cbSize.QuadPart;
						LARGE_INTEGER l;
						l.QuadPart = 0;
						pStm->Seek(l,STREAM_SEEK_SET,NULL);	
						BYTE* pRaw;
						pRaw = (BYTE*)calloc(lBytes, sizeof(BYTE));
						pStm->Read(pRaw,lBytes,NULL);
						//convert the Byte* into BSTR
						CBase64 base64;
						BYTE *pEncoded=(BYTE*) base64.Encode((char*)pRaw, lBytes);
						CComBSTR  bstr(A2T((LPSTR)pEncoded));
						free(pRaw);
						free(pEncoded);
						
						hr = pIMXAttributes->addAttribute(CComBSTR(""),bstrPropName,bstrPropName,CComBSTR("CDATA"), bstr);						
						
						pStm.Release();
					} //if (spStream)
				}//try
				catch( ... )
				{
					pStm.Release();
					//log it
				}
			} //else if (VT_ARRAY == (pMap[i].vt & VT_ARRAY))
            else // for all other basic types, long, double, short, bool, etc
			{
				void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
				// check the type - we only have deal with the limited set
				bool bTypeOK = false;
				switch (pMap[i].vt)
				{
				case VT_UI1:
				case VT_I1:
					var.bVal = *((BYTE*)pData);
					bTypeOK = true;
					break;
				case VT_BOOL:
					var.boolVal = *((VARIANT_BOOL*)pData);
					bTypeOK = true;
					break;
				case VT_I2:
				case VT_UI2:
					var.iVal = *((short*)pData);
					bTypeOK = true;
					break;
				case VT_UI4:
				case VT_I4:
				case VT_INT:
				case VT_UINT:
					var.lVal = *((long*)pData);
					bTypeOK = true;
					break;
				case VT_R4:
					var.fltVal = *((float*)pData);
					bTypeOK = true;
					break;
				case VT_R8:
					var.dblVal = *((double*)pData);
					bTypeOK = true;
					break;
				case VT_DATE:
					var.date = *((DATE*)pData);
					bTypeOK = true;
					break;
				}
				if (bTypeOK)
				{
					var.vt = pMap[i].vt;
					if (var.vt==VT_DATE)
					{
						TCHAR szDate[255];
						TCHAR szTime[255];
						CComDATE d(var.date);
						d.FormatDate(szDate,_T("yyyy-MM-dd"));
						d.FormatTime(szTime,_T("HH:mm:ss"));
						_tcscat(szDate, _T(" "));
						_tcscat(szDate, szTime);
						CComBSTR bstr(szDate);
						hr = pIMXAttributes->addAttribute(CComBSTR(""),bstrPropName,bstrPropName,CComBSTR("CDATA"),bstr);
					}
					else
						if(SUCCEEDED(hr = var.ChangeType(VT_BSTR)))
							hr = pIMXAttributes->addAttribute(CComBSTR(""),bstrPropName,bstrPropName,CComBSTR("CDATA"),V_BSTR(&var));
				}
			}
			if (FAILED(hr))
				return hr;
			continue;
		}

		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				return E_FAIL;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			return E_FAIL;
		}

		if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
		{
			if (var.punkVal == NULL)
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Warning skipping empty IUnknown in Save\n"));
				continue;
			}
		}

		//Convert to String and Save
		if (var.vt==VT_ARRAY)
		{
			
		}
		else if(SUCCEEDED(hr = var.ChangeType(VT_BSTR)))
		{	
			hr = pIMXAttributes->addAttribute(CComBSTR(""),bstrPropName,bstrPropName,CComBSTR("CDATA"),V_BSTR(&var));
		}

		if (FAILED(hr))
			return hr;
	}
	return S_OK;
}

// IPersistFile Help functions! :)
HRESULT ATLIPersistFile_Load(LPCOLESTR pszFileName, IUnknown* pUnk)
{
	USES_CONVERSION;
	HRESULT hRes = E_FAIL;

	LPCTSTR strFileName= OLE2CT(pszFileName);
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL,
							  OPEN_EXISTING,
							  FILE_ATTRIBUTE_ARCHIVE,
							  NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	bool bXMLFormat = false;
	DWORD dwHiFileSize;
	DWORD dwSize = GetFileSize(hFile,&dwHiFileSize);
	if(dwSize)
	{
		HGLOBAL hGlob= GlobalAlloc(GHND, dwSize);
		LPVOID pString  = GlobalLock(hGlob);
		DWORD dwReaded;
		BOOL bRet= ReadFile(hFile,pString,dwSize,&dwReaded,NULL);

		//auto recognition XML format (can be improved)
		if( ((char*)pString)[0] == '<' || ( ((byte*)pString)[0] == 0xFF && ((byte*)pString)[1] == 0xFE && ((char*)pString)[2] =='<' ))
		{
			bXMLFormat = true;
		}
		GlobalUnlock(hGlob);
		CloseHandle(hFile);

		DWORD dwError;
		if(!bRet)
			hRes = HRESULT_FROM_WIN32(dwError = GetLastError());

		CComPtr<IStream> spStream;
		hRes = CreateStreamOnHGlobal(hGlob, TRUE, &spStream);
		if (spStream)
		{
			if(bXMLFormat)
			{
				//load XML
				CComQIPtr<IPersistVarXML> pIPersistVarXML(pUnk);
				if(pIPersistVarXML)
					hRes = pIPersistVarXML->LoadXML(CComVariant(spStream));
				else
				{
					ATLASSERT(0);
					hRes = E_FAIL;
				}
			}
			else
			{
				//load STREAM
				CComQIPtr<IPersistStreamInit> pIPersistStream(pUnk);
				if(pIPersistStream)
					hRes = pIPersistStream->Load(spStream);
				else
				{
					ATLASSERT(0);
					hRes = E_FAIL;
				}
			}
			ATLASSERT(SUCCEEDED(hRes));
			spStream.Release();
		}
	}else
		CloseHandle(hFile);
	return hRes;
}

HRESULT ATLIPersistFile_Save( LPCOLESTR pszFileName, IUnknown* pUnk)
{
	USES_CONVERSION;
	HRESULT hRes = S_OK;

	LPCTSTR strFileName= OLE2CT(pszFileName);
	HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL,
							  CREATE_ALWAYS,
							  FILE_ATTRIBUTE_ARCHIVE,
							  NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	//Determinate Format to Save from File Name!
	int	bTryToSaveAsXML=true;
	long Size = _tcsclen(strFileName);
	if(!_tcsncicmp(&strFileName[Size-4],_T(".bin"),4))
			bTryToSaveAsXML = false;

	DWORD dwWrited;

	HGLOBAL hGlob;// = GlobalAlloc(GHND, l.LowPart);
	CComPtr<IStream> spStream;
	hRes = CreateStreamOnHGlobal(NULL/*hGlob*/, TRUE, &spStream);
	if (spStream)
	{
		if(bTryToSaveAsXML)
		{
			//try to save XML
			CComQIPtr<IPersistVarXML> pIPersistVarXML(pUnk);
			if(pIPersistVarXML)
				hRes = pIPersistVarXML->SaveXML(CComVariant(spStream));
			else
			{
				ATLASSERT(0);
				hRes = E_FAIL;
			}
		}
		
		if(FAILED(hRes) || !bTryToSaveAsXML)
		{
			//try to save STREAM
			CComQIPtr<IPersistStreamInit> pIPersistStream(pUnk);
			if(pIPersistStream)
				hRes = pIPersistStream->Save(spStream,TRUE);
			else
			{
				ATLASSERT(0);
				hRes = E_FAIL;
			}
		}
		ATLASSERT(SUCCEEDED(hRes));

		if(SUCCEEDED(hRes))
		{
			hRes = GetHGlobalFromStream(spStream,&hGlob);
			DWORD dwSize = GlobalSize(hGlob);
			LPVOID pString  = GlobalLock(hGlob);

			BOOL bRet= WriteFile(hFile,pString,dwSize,&dwWrited,NULL);

			ATLASSERT(bRet || dwWrited!=dwSize);

			GlobalUnlock(hGlob);

			DWORD dwError;
			if(!bRet)
				hRes = HRESULT_FROM_WIN32(dwError = GetLastError());
		}
		spStream.Release();
	}
	CloseHandle(hFile);
	return hRes;
}

void InsertNullToXML(ISAXContentHandler *pISAXContentHandler)
{
		CComBSTR bstrThisItemName;
		bstrThisItemName.Append((LPCTSTR)"NULL");
	    unsigned short Lenght;
		Lenght = bstrThisItemName.Length();
		CComPtr<IMXAttributes>			pIMXAttributes;
		pIMXAttributes.CoCreateInstance(__uuidof(SAXAttributes ));
		CComQIPtr<ISAXAttributes>		pISAXAttributes(pIMXAttributes);
		pISAXContentHandler->startElement((unsigned short *)L"",0,(unsigned short *)OLE2W(bstrThisItemName),Lenght,(unsigned short *)OLE2W(bstrThisItemName),Lenght,pISAXAttributes);
		pISAXContentHandler->endElement((unsigned short *)L"",0,(unsigned short *)OLE2W(bstrThisItemName),Lenght,(unsigned short *)OLE2W(bstrThisItemName),Lenght);

}
