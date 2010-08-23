//IPersistVarXMLImpl.h

/////////////////////////////////////////////////////
//											
// This is a part of the ATL PersistXML add-on
// 
// Copyright (c) VetalyKartsnov.  All rights reserved.
//
// 
//
/////////////////////////////////////////////////////

#ifndef __IPersistVarXMLImp_h_
#define __IPersistVarXMLImp_h_

#include "IPersistVarXML.h"
#include "atlsax.h"

//redefine the END_PROP_MAP, add the m_hr member variable to the ATL_MAP_ENTRY array.
#undef  END_PROP_MAP
#define END_PROP_MAP() \
			PROP_DATA_ENTRY("HRESULT", m_hr, VT_I4) \
			PROP_DATA_ENTRY("ERRDESC", m_bstrErrDesc, VT_BSTR) \
			{NULL, 0, NULL, &IID_NULL, 0, 0, 0} \
		}; \
		return pPropMap; \
	}

#define _FACPersistVarXMLERR	0x301
#define MAKE_PERSISTVARXMLERRHRESULT( code )	MAKE_HRESULT( SEVERITY_ERROR, _FACPersistVarXMLERR, code )

#define ERR_CoCreateInstance_SAXAttributes			MAKE_PERSISTVARXMLERRHRESULT( 1 )
#define ERR_CoCreateInstance_SAXXMLReader			MAKE_PERSISTVARXMLERRHRESULT( 2 )
#define ERR_CoCreateInstance_MXXMLWriter			MAKE_PERSISTVARXMLERRHRESULT( 3 )

#define DECLARE_XML_ELEMENT_NAME(strName) \
	STDMETHOD(get_ElementName)(BSTR* pElementName) \
	{ \
		if(m_bstrElementName==_T("")){\
			*pElementName = ::SysAllocString(OLESTR(#strName)); \
		}\
		else\
			*pElementName = m_bstrElementName; \
		return S_OK;\
	}

struct IPersistVarXML_CONTENT_ENTRY
{
	const wchar_t* wszDesc;
	DWORD dwOffsetData;
	DWORD dwSizeData;
	VARTYPE vt;
};

// when there is more than 2 same com interfaces in one class, please use it in IPersistVarXML_SaveContent.
#define COM_BOJECT_SAVE(m_piCom) \
	 if (m_piCom != NULL)\
	{\
	 CComQIPtr<IPersistVarXML> pTemp(m_piCom);\
	 pTemp->SaveXML(CComVariant(pISAXContentHandler));\
	}\
	 else\
	 {  \
		 InsertNullToXML(pISAXContentHandler);\
	 }
//when there is more than 2 same com interfaces in one class, please use it to replace XML_CONTENT_CREATE_OBJECT
//sn:	 serial number of current object in all objects
#define XML_CONTENT_CREATE_MULTI_SAME_OBJECT(szName,pclsid,pmember,sn)  \
			if(!pISAXContentHandler)\
			{\
					if ( !wcscmp(OLESTR("NULL"),bstrElementName) )\
						flag = true;\
					if(!wcscmp(OLESTR(szName), bstrElementName))\
					{\
						if (j != intLimit)\
						{\
							j++;\
						}\
						flag = true;\
						if (order == sn)\
						{\
							CComPtr<IPersistVarXML>		pPersistVarXML1;\
							if(SUCCEEDED(pPersistVarXML1.CoCreateInstance(pclsid)))\
							{\
								if(SUCCEEDED(hRes = pPersistVarXML1->LoadXML(CComVariant(m_pCurrentXMLReader),pAttributes)))\
									hRes = pmember(pPersistVarXML1);\
							}else\
							{\
								ATLASSERT(0);\
							}\
						}\
					}\
			}


#define BEGIN_XML_CONTENT_MAP(theClass) \
	public: \
	HRESULT SaveLoadContent(ISAXContentHandler* pISAXContentHandler,ISAXAttributes * pAttributes, BSTR bstrElementName) \
	{\
		HRESULT hRes=S_OK;\
		CComPtr<IPersistVarXML>		pPersistVarXML;\
		CComBSTR	bstrEntryName;

#define XML_CONTENT_LOAD_HANDLER(szName,func)\
			if(!pISAXContentHandler)\
				if(!wcscmp(OLESTR(szName), bstrElementName))\
					hRes = func(pAttributes);

#define XML_CONTENT_CREATE_OBJECT(szName,pclsid,pmember) \
				if(!pISAXContentHandler)\
					if(!wcscmp(OLESTR(szName), bstrElementName))\
					{\
						CComPtr<IPersistVarXML>		pPersistVarXML1;\
						if(SUCCEEDED(pPersistVarXML1.CoCreateInstance(pclsid)))\
						{\
							if(SUCCEEDED(hRes = pPersistVarXML1->LoadXML(CComVariant(m_pCurrentXMLReader),pAttributes)))\
								hRes = pmember(pPersistVarXML1);\
						}else\
						{\
							ATLASSERT(0);\
						}\
					}\

#define XML_CONTENT_CREATE_OBJECT2(szName,pclsid,pmember) \
				if(!pISAXContentHandler)\
					if(!wcscmp(OLESTR(szName), bstrElementName))\
					{\
						CComPtr<IPersistVarXML>		pPersistVarXML2;\
						if(SUCCEEDED(pPersistVarXML2.CoCreateInstance(pclsid)))\
						{\
							if(SUCCEEDED(hRes = pPersistVarXML2->LoadXML(CComVariant(m_pCurrentXMLReader),pAttributes)))\
								hRes = pmember(pPersistVarXML2);\
						}else\
						{\
							ATLASSERT(0);\
						}\
					}\

#define XML_CONTENT_LOAD_OBJECT_SELF(szName,pSubObject) \
				if(!pISAXContentHandler)\
					if(!wcscmp(OLESTR(szName), bstrElementName))\
					{\
						CComPtr<IPersistVarXML>		pPersistVarXML2;\
						if(pSubObject)\
							pSubObject->QueryInterface(&pPersistVarXML2);\
						if(pPersistVarXML2)\
						{\
							hRes = pPersistVarXML2->LoadXML(CComVariant(m_pCurrentXMLReader),pAttributes);\
						}else\
						{\
							ATLASSERT(0);\
						}\
					}\

#define XML_CONTENT_ENTRY(pmember) \
			if(!pPersistVarXML)\
				if(SUCCEEDED(hRes=(pmember)->QueryInterface(__uuidof(IPersistVarXML), (void**)&pPersistVarXML)))\
				{\
					if(pISAXContentHandler)\
					{\
						hRes=pPersistVarXML->SaveXML(CComVariant(pISAXContentHandler));\
						pPersistVarXML.Release();\
					}else\
					{\
						pPersistVarXML->get_ElementName(&bstrEntryName);\
						if(wcscmp(bstrEntryName, bstrElementName))\
							pPersistVarXML.Release();\
						bstrEntryName.Empty();\
					}\
				}
		


#define XML_CONTENT_SAVE_ENTRY(pmember) \
			if(pISAXContentHandler)\
				if(SUCCEEDED(hRes=(pmember)->QueryInterface(__uuidof(IPersistVarXML), (void**)&pPersistVarXML)))\
				{\
					if(pISAXContentHandler)\
						hRes=pPersistVarXML->SaveXML(CComVariant(pISAXContentHandler));\
					pPersistVarXML.Release();\
				}

#define END_XML_CONTENT_MAP() \
			if (flag)\
			{\
				flag = false;\
				order++;\
				intLimit = j;\
				if (order == intLimit)\
				{\
					intLimit = 1000;\
					order = 1;\
					j = 1;\
				}\
			}\
			if(pPersistVarXML && !pISAXContentHandler)\
			{\
				hRes = pPersistVarXML->LoadXML(CComVariant(m_pCurrentXMLReader),pAttributes);\
				pPersistVarXML.Release();\
			}\
		return hRes;\
	}

HRESULT AtlIPersistVarXML_Load(ISAXAttributes * pAttributes, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk);
HRESULT AtlIPersistVarXML_Save(IMXAttributes*	pIMXAttributes, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk);

// IPersistFile Help functions! :)
HRESULT ATLIPersistFile_Load(LPCOLESTR pszFileName, IUnknown* pUnk);
HRESULT ATLIPersistFile_Save( LPCOLESTR pszFileName, IUnknown* pUnk);
void InsertNullToXML(ISAXContentHandler *pISAXContentHandler);

template <class T>
class ATL_NO_VTABLE IPersistVarXMLImpl : 
	public IPersistVarXML, 
	public ISAXContentHandlerImpl<T>
{
public:
	 int order,intLimit,j;
	 bool flag;
protected:
	HRESULT m_hr; 
	CComBSTR m_bstrErrDesc;
public:
    IPersistVarXMLImpl(): m_hr(S_OK),m_bstrElementName(_T("")),
				m_bstrErrDesc(L""),order(1),intLimit(1000),j(1),flag(false) {}


public:
	HRESULT get_HResult(HRESULT *hr) 
	{
		*hr=m_hr;
		return S_OK;
	}
	HRESULT get_ErrDesc(BSTR *pbstr)
	{
		return m_bstrErrDesc.CopyTo(pbstr);
	}

	HRESULT IPersistVarXML_SaveContent(ISAXContentHandler* pISAXContentHandler) \
	{
		T* pT = static_cast<T*>(this);
		return pT->SaveLoadContent(pISAXContentHandler,NULL,NULL);
	}

	HRESULT IPersistVarXML_LoadContent(BSTR bstrIlementName, ISAXAttributes * pAttributes)
	{
		T* pT = static_cast<T*>(this);
		return pT->SaveLoadContent(NULL,pAttributes,bstrIlementName);
	}

	HRESULT IPersistVarXML_LoadXML_Finished()
	{
		//Default Implimentation
		return S_OK;
	}


	HRESULT SaveLoadContent(ISAXContentHandler* pISAXContentHandler,ISAXAttributes * pAttributes, BSTR bstrElementName)
	{
		//Default Implimentation
		return S_OK;
	}

	HRESULT IPersistVarXML_Load(ISAXAttributes * pAttributes)
	{
		T* pT = static_cast<T*>(this);
		ATL_PROPMAP_ENTRY* pMap = T::GetPropertyMap();
		ATLASSERT(pMap != NULL);

		HRESULT hr = AtlIPersistVarXML_Load(pAttributes, pMap, pT, pT->GetUnknown());
		if (SUCCEEDED(hr))
			pT->m_bRequiresSave = FALSE;
		return hr;
	}
	

	HRESULT IPersistVarXML_Save(IMXAttributes*	pIMXAttributes)
	{
		T* pT = static_cast<T*>(this);
		ATL_PROPMAP_ENTRY* pMap = T::GetPropertyMap();
		ATLASSERT(pMap != NULL);

		return AtlIPersistVarXML_Save(pIMXAttributes, pMap, pT, pT->GetUnknown());
	}


	//Default Implementations:
	HRESULT StartElement (ISAXAttributes * pAttributes, BSTR ElementName)
	{
		T* pT = static_cast<T*>(this);

		HRESULT hRes = S_OK;

		USES_CONVERSION;

		if(::SysStringLen(ElementName)==0 )
		{
			//Load THIS Object!!
			return pT->IPersistVarXML_Load(pAttributes);
		}else
			return pT->IPersistVarXML_LoadContent(ElementName, pAttributes);

		return hRes;
	}

	HRESULT SaveElement (ISAXContentHandler* pISAXContentHandler)
	{
		USES_CONVERSION;
		T* pT = static_cast<T*>(this);

		HRESULT hRes = E_FAIL;

		CComBSTR bstrThisItemName;
		hRes = pT->get_ElementName(&bstrThisItemName);

		unsigned short Lenght;
		ATLASSERT(bstrThisItemName.Length());

		if(Lenght = bstrThisItemName.Length())
		{
			CComPtr<IMXAttributes>			pIMXAttributes;
			if(SUCCEEDED(hRes = pIMXAttributes.CoCreateInstance(__uuidof(SAXAttributes ))))
			{
				CComQIPtr<ISAXAttributes>		pISAXAttributes(pIMXAttributes);
				if(pISAXAttributes)
				{
					if(SUCCEEDED(hRes = pT->IPersistVarXML_Save(pIMXAttributes)))
					{
						pISAXContentHandler->startElement((unsigned short *)L"",0,(unsigned short *)OLE2W(bstrThisItemName),Lenght,(unsigned short *)OLE2W(bstrThisItemName),Lenght,pISAXAttributes);
					}
				}
			
				if(SUCCEEDED(hRes))
				{
					if (m_bSaveContent){
						hRes = pT->IPersistVarXML_SaveContent(pISAXContentHandler);
					}

					pISAXContentHandler->endElement((unsigned short *)L"",0,(unsigned short *)OLE2W(bstrThisItemName),Lenght,(unsigned short *)OLE2W(bstrThisItemName),Lenght);
				}

			}else
				hRes = ERR_CoCreateInstance_SAXAttributes;
					
		}
		return hRes;
	}

	// for IPersistVarXML
	CComPtr<ISAXContentHandler>	m_pPrevContentHandler;
	CComQIPtr<ISAXXMLReader>	m_pCurrentXMLReader;
	BOOL	m_nStarted;
	BOOL	m_bSaveContent:true; // used to mark whether to save the contents of class members to XML
								//Default value is true. 
								//Added on Sep 30, 2003 
	CComBSTR m_bstrElementName;// used to 
								//Default value is empty string
								//Added on March 2, 2004 
	// ISAXContentHandler
	STDMETHOD(startElement) (
        unsigned short * pwchNamespaceUri,
        int cchNamespaceUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * pwchQName,
        int cchQName,
        struct ISAXAttributes * pAttributes )
	{
		T* pT = static_cast<T*>(this);

		USES_CONVERSION;

		CComBSTR bstrName(cchQName,(LPCOLESTR)pwchQName);
		if(m_nStarted==0)
		{
			CComBSTR bstrThisItemName;
			pT->get_ElementName(&bstrThisItemName);

			if(bstrThisItemName==bstrName)
			{
				m_nStarted++;
				CComBSTR bstr=_T("");
				return pT->StartElement (pAttributes, bstr);
			}
		}

		return pT->StartElement (pAttributes,bstrName);
	}

	STDMETHOD(endElement)(
        unsigned short * pwchNamespaceUri,
        int cchNamespaceUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * pwchQName,
        int cchQName )
		{
			T* pT = static_cast<T*>(this);

			if(m_pPrevContentHandler)
			{
				USES_CONVERSION;				
				CComBSTR bstrName(cchQName,(LPCOLESTR)pwchQName);	
				CComBSTR bstrThisItemName;
				pT->get_ElementName(&bstrThisItemName);
				
				if(bstrThisItemName == bstrName)
				{
					ATLASSERT(m_nStarted>0);
					m_nStarted--;
					if(m_nStarted==0)
					{
						ATLASSERT(m_pPrevContentHandler);
						m_pCurrentXMLReader->putContentHandler(m_pPrevContentHandler);
						m_pPrevContentHandler.Release();
						m_pCurrentXMLReader.Release();
						pT->IPersistVarXML_LoadXML_Finished();
					}
				}	
			}

			return S_OK;
		}

        STDMETHOD(LoadXML)( 
            /* [in] */ VARIANT varSource, IUnknown * pAttributes = 0L)
		{
			T* pT = static_cast<T*>(this);

			HRESULT hRes;
			
			if(V_VT(&varSource)==VT_UNKNOWN)
			{
				m_pCurrentXMLReader = V_UNKNOWN(&varSource);
			}

			if(m_pCurrentXMLReader)
			{
				ATLASSERT(!m_pPrevContentHandler);
				m_nStarted = 1;
				hRes = m_pCurrentXMLReader->getContentHandler(&m_pPrevContentHandler);
				hRes = m_pCurrentXMLReader->putContentHandler((ISAXContentHandler*)(this));
				hRes = pT->StartElement((ISAXAttributes*)pAttributes, CComBSTR(""));
				ATLASSERT(SUCCEEDED(hRes));

			}else
			{
				m_nStarted = 0;
				if(SUCCEEDED(hRes = m_pCurrentXMLReader.CoCreateInstance(__uuidof(SAXXMLReader))))
				{
					if(SUCCEEDED(hRes = m_pCurrentXMLReader->putContentHandler(static_cast<ISAXContentHandler*>(this))))
					{
						hRes = m_pCurrentXMLReader->parse(varSource);
						ATLASSERT(SUCCEEDED(hRes));
					}

					m_pCurrentXMLReader.Release();

				}else
				{
					hRes =  ERR_CoCreateInstance_SAXXMLReader;
				}

			}
			return hRes;
		}
        
        STDMETHOD(SaveXML)( 
            /* [in] */ VARIANT VarOutput, BOOL bSaveContent = true, BSTR bstrElementName=CComBSTR(_T("")))
		{
			m_bSaveContent=bSaveContent;
			m_bstrElementName=bstrElementName;

			T* pT = static_cast<T*>(this);

			HRESULT	hRes;

			CComQIPtr<ISAXContentHandler>	pISAXContentHandler;
			if(V_VT(&VarOutput) == VT_UNKNOWN)
			{
				pISAXContentHandler = V_UNKNOWN(&VarOutput);
				if(pISAXContentHandler)
					return pT->SaveElement(pISAXContentHandler);
			}
			
			CComPtr<IMXWriter>				pXMLWriter;
			if(SUCCEEDED(hRes = pXMLWriter.CoCreateInstance(__uuidof(MXXMLWriter))))
			{
				pISAXContentHandler = pXMLWriter;
				if(pISAXContentHandler)
				{
					//Setup
					hRes = pXMLWriter->put_omitXMLDeclaration(-1);
					hRes = pXMLWriter->put_standalone(-1);
					hRes = pXMLWriter->put_indent(-1);
					CComBSTR bstr=_T("ASCII");
					hRes = pXMLWriter->put_encoding(bstr);
					hRes = pXMLWriter->put_output(VarOutput);

					//Start
					hRes = pISAXContentHandler->startDocument();

					//Save this object
					hRes = pT->SaveElement(pISAXContentHandler);
					
					//Finishing
					hRes = pISAXContentHandler->endDocument();

					if(V_VT(&VarOutput) == (VT_VARIANT|VT_BYREF) )
						hRes = pXMLWriter->get_output(V_VARIANTREF(&VarOutput));

					hRes = pXMLWriter->put_output(CComVariant(""));

				}
				pXMLWriter.Release();

			}else
				hRes = ERR_CoCreateInstance_MXXMLWriter;

			return hRes;
		}
	

		//IPersistFile Implementation (Help functions)
		HRESULT IPersistFile_Load( /* [in] */ LPCOLESTR pszFileName,/* [in] */ DWORD dwMode)
		{
			T* pT = static_cast<T*>(this);
			return ATLIPersistFile_Load( pszFileName, pT->GetUnknown());
		}

		HRESULT IPersistFile_Save( /* [unique][in] */ LPCOLESTR pszFileName,/* [in] */ BOOL fRemember)
		{
			T* pT = static_cast<T*>(this);
			return ATLIPersistFile_Save( pszFileName, pT->GetUnknown());
		}
		public:
};

#endif //__IPersistVarXMLImp_h_