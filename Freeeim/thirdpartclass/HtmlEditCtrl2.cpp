// WebBrowserEdit.cpp: implementation of the CWebBrowserEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HtmlEditCtrl2.h"
#include "AtlBase.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrl2

CHtmlEditCtrl2::CHtmlEditCtrl2()
{
	m_hResult = E_FAIL;
	m_pDispatch = NULL;
	m_pHtmlDoc = NULL;
	m_pCmdTarg = NULL;
}

CHtmlEditCtrl2::~CHtmlEditCtrl2()
{
	if(m_pDispatch)
	{
		m_pDispatch->Release();
	}
	
	if(m_pHtmlDoc)
	{
		m_pHtmlDoc->Release();
	}
	
	if(m_pCmdTarg)
	{
		m_pCmdTarg->Release();
	}
}

/*
IZ: Function to retive all tags that contains images (like <img src="http://www.server.com/image.jpg>") from HTML document
	Was not present in orginal CHtmlEditCtrlBase
*/
HRESULT CHtmlEditCtrl2::GetURLsOfAllImages(CStringArray& a_arrImages)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElementCollection* pImages = NULL;
	m_hResult = m_pHtmlDoc->get_images(&pImages);
	
	if( SUCCEEDED(m_hResult) && pImages )
	{
		long lNumberOfItems = 0;
		m_hResult = pImages->get_length(&lNumberOfItems);
		if( SUCCEEDED(m_hResult) )
		{	
			IDispatch *ppvDisp = NULL;
			IHTMLImgElement *ppvElement = NULL;
	
			for (long ix=0; ix<lNumberOfItems; ix++)
			{
				CComVariant index = ix;
				m_hResult = pImages->item( index, index, &ppvDisp );

				if( SUCCEEDED(m_hResult) && ppvDisp ) 
				{
					m_hResult = ppvDisp->QueryInterface( IID_IHTMLImgElement ,  (void **) &ppvElement);
					ppvDisp->Release();
					
					if ( SUCCEEDED(m_hResult) && ppvElement )
					{
						BSTR bstrText = NULL;
						CString sText;

						m_hResult = ppvElement->get_src(&bstrText);
						ppvElement->Release();

						BSTR2CString(bstrText, sText);
						::SysFreeString(bstrText);

						a_arrImages.Add( sText );
					}
				}
			}
		}

		pImages->Release();
	}

	return m_hResult;
}

/*
IZ: Was not present in orginal CHtmlEditCtrlBase
*/
HRESULT CHtmlEditCtrl2::ReplaceImageURL(const CString& a_sUrlToReplace, const CString& a_sUrlToReplaceWith)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IHTMLElementCollection* pImages = NULL;
	m_hResult = m_pHtmlDoc->get_images(&pImages);
	
	if( SUCCEEDED(m_hResult) && pImages)
	{
		long lNumberOfItems = 0;
		m_hResult = pImages->get_length(&lNumberOfItems);
		
		if( SUCCEEDED(m_hResult) )
		{	
			IDispatch *ppvDisp = NULL;
			IHTMLImgElement *ppvElement = NULL;
	
			for (long ix=0; ix<lNumberOfItems; ix++)
			{
				CComVariant index = ix;
				m_hResult = pImages->item( index, index, &ppvDisp );

				if( SUCCEEDED(m_hResult) && ppvDisp ) 
				{
					m_hResult = ppvDisp->QueryInterface( IID_IHTMLImgElement ,  (void **) &ppvElement);
					ppvDisp->Release();
					
					if ( SUCCEEDED(m_hResult) && ppvElement )
					{
						BSTR bstrText = NULL;
						CString sText;
						m_hResult = ppvElement->get_src(&bstrText);
						BSTR2CString(bstrText, sText);
						SysFreeString(bstrText);
						
						if(a_sUrlToReplace == sText)
						{
							bstrText = a_sUrlToReplaceWith.AllocSysString();
							m_hResult = ppvElement->put_src(bstrText);
							SysFreeString(bstrText);
						}

						ppvElement->Release();
					}
				}
			}
		}

		pImages->Release();
	}

	return m_hResult;
}


HRESULT CHtmlEditCtrl2::GetBodyColorParameters(CString& a_sColor, BOOL a_bTextColor)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		IHTMLBodyElement* pBodyElem = NULL;
		m_hResult = pElem->QueryInterface( IID_IHTMLBodyElement, (void**)&pBodyElem );
		pElem->Release();
		
		if( SUCCEEDED(m_hResult) && pBodyElem )
		{
			CComVariant vaRet;

			if(a_bTextColor)
				m_hResult = pBodyElem->get_text( &vaRet );
			else
				m_hResult = pBodyElem->get_bgColor( &vaRet );

			pBodyElem->Release();

			if(vaRet.vt & VT_BSTR)
			{
				BSTR2CString(vaRet.bstrVal, a_sColor);
			}
			else if(vaRet.vt & VT_I4)
			{
				ASSERT(0);//IZ TO CHECK
				a_sColor.Format(_T("%d"), vaRet.lVal);
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetBodyBackgroundColor(CString& a_sColor)
{
	return GetBodyColorParameters(a_sColor, FALSE);
}

HRESULT CHtmlEditCtrl2::GetBodyTextColor(CString& a_sColor)
{
	return GetBodyColorParameters(a_sColor, TRUE);
}

HRESULT CHtmlEditCtrl2::GetBodyBackgroundImage(CString& a_sImage)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		IHTMLBodyElement* pBodyElem = NULL;
		m_hResult = pElem->QueryInterface( IID_IHTMLBodyElement, (void**)&pBodyElem );
		pElem->Release();
		
		if( SUCCEEDED(m_hResult) && pBodyElem )
		{
			BSTR bstrBody = NULL;
			m_hResult = pBodyElem->get_background( &bstrBody );
			pBodyElem->Release();

			BSTR2CString(bstrBody, a_sImage);
			SysFreeString(bstrBody);
		}
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetBodyProperties(CString& a_sTag)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		IHTMLBodyElement* pBodyElem = NULL;
		m_hResult = pElem->QueryInterface( IID_IHTMLBodyElement, (void**)&pBodyElem );
		pElem->Release();
		
		if( SUCCEEDED(m_hResult) && pBodyElem )
		{
			BSTR bstrBody = NULL;
			m_hResult = pBodyElem->get_bgProperties( &bstrBody );
			pBodyElem->Release();
			
			BSTR2CString(bstrBody, a_sTag);
			SysFreeString(bstrBody);
		}
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::SetBodyBackgroundImage(const CString& a_sImage)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		IHTMLBodyElement* pBodyElem = NULL;
		m_hResult = pElem->QueryInterface( IID_IHTMLBodyElement, (void**)&pBodyElem );
		pElem->Release();
		
		if( SUCCEEDED(m_hResult) && pBodyElem )
		{
			BSTR bstrBody = a_sImage.AllocSysString();
			m_hResult = pBodyElem->put_background( bstrBody );
			pBodyElem->Release();
			::SysFreeString(bstrBody);
		}
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetDocumentCharset(CString& a_sCharsetEncoding)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	BSTR bstrCharset = NULL;
	m_hResult = m_pHtmlDoc->get_charset(&bstrCharset);
	BSTR2CString(bstrCharset, a_sCharsetEncoding);
	SysFreeString(bstrCharset);

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::SetDocumentCharset(const CString& a_sCharsetEncoding)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	BSTR bstrCharset = a_sCharsetEncoding.AllocSysString();
	m_hResult = m_pHtmlDoc->put_charset(bstrCharset);
	::SysFreeString(bstrCharset);

	return m_hResult;
}

BOOL CHtmlEditCtrl2::PrepareInterfaces()
{
	m_hResult  = E_FAIL;
	
	if(m_pDispatch == NULL)
	{
		m_pDispatch = CWebBrowser2::GetDocument();
		
		if(m_pDispatch)
		{
			m_hResult = m_pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&m_pHtmlDoc);
			m_hResult = m_pDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&m_pCmdTarg);
		}
	}
	
	return m_pDispatch && m_pHtmlDoc && m_pCmdTarg;
}


BOOL CHtmlEditCtrl2::SetDesignMode(BOOL bMode)
{
	if( PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}
	
	m_hResult = m_pHtmlDoc->put_designMode(bMode ? L"On" : L"Off");
	return SUCCEEDED( m_hResult );
}

BOOL CHtmlEditCtrl2::IsDesignMode()
{
	if(PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}
	
	BSTR bstrDesignMode = NULL;
	m_pHtmlDoc->get_designMode( &bstrDesignMode );
	
	CString sMode;
	BSTR2CString(bstrDesignMode, sMode);
	SysFreeString(bstrDesignMode);
	
	return ( sMode == "On" );
}


HRESULT CHtmlEditCtrl2::ExecCommand(const GUID *pGuid, long cmdID, long cmdExecOpt, VARIANT* pInVar, VARIANT* pOutVar)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	return m_pCmdTarg->Exec(pGuid, cmdID, cmdExecOpt, pInVar , pOutVar);
}

HRESULT CHtmlEditCtrl2::ExecCommand(long cmdID, long cmdExecOpt, VARIANT* pInVar, VARIANT* pOutVar)
{
	return ExecCommand(&CGID_MSHTML, cmdID, cmdExecOpt, pInVar , pOutVar );
}

long CHtmlEditCtrl2::QueryStatus(long cmdID)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	OLECMD ocmd = {cmdID, 0};
	if (S_OK == m_pCmdTarg->QueryStatus(&CGID_MSHTML, 1, &ocmd, NULL))
		return ocmd.cmdf;
	
	return 0;
}

HRESULT CHtmlEditCtrl2::GetEvent(IHTMLEventObj **ppEventObj)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IHTMLWindow2* sphtmlWnd = NULL;
	
	if (!ppEventObj)
		return E_POINTER;
	
	*ppEventObj = NULL;
	HRESULT hr = E_FAIL;
	
	m_hResult = m_pHtmlDoc->get_parentWindow(&sphtmlWnd);
	if(m_hResult == S_OK)
	{
		m_hResult = sphtmlWnd->get_event(ppEventObj);
		sphtmlWnd->Release();
	}
	
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetEventSrcElement(IHTMLElement **ppSrcElement)
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IHTMLEventObj* spEventObj = NULL;
	
	if (!ppSrcElement)
		return E_POINTER;
	
	*ppSrcElement = NULL;
	m_hResult = GetEvent(&spEventObj);
	if (m_hResult == S_OK)
	{
		m_hResult = spEventObj->get_srcElement(ppSrcElement);
		spEventObj->Release();
	}
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetDocument(IHTMLDocument2** ppDoc)
{	
	if( PrepareInterfaces() == FALSE)
	{
		return E_NOINTERFACE;
	}
	
	*ppDoc = m_pHtmlDoc;
	m_pHtmlDoc->AddRef();
	return S_OK;
}

HRESULT CHtmlEditCtrl2::NewDocument()
{
	if( PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IPersistStreamInit* spPSI = NULL;
	m_hResult = m_pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&spPSI);
	
	if( SUCCEEDED(m_hResult) && spPSI )
	{
		m_hResult = spPSI->InitNew();
		spPSI->Release();
	}
	
	return m_hResult;
}

/*
	//IZ WARNING - NOT UNICODE AWARE FUNCTION!
*/
HRESULT CHtmlEditCtrl2::GetDocumentBody(CString& a_sBody, BOOL a_bTextInsteadHTML)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		BSTR bstrText = NULL;

		if(a_bTextInsteadHTML)
			m_hResult = pElem->get_innerText(&bstrText);
		else
			m_hResult = pElem->get_outerHTML(&bstrText);

		BSTR2CString(bstrText, a_sBody);
		SysFreeString(bstrText);

		pElem->Release();
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetDocumentHTML(CString& szHTML, BOOL a_bClearDirtyFlag)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IPersistStreamInit* spPSI = NULL;
	CStreamOnCString stream;
	
	m_hResult = m_pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&spPSI);
	
	if( SUCCEEDED(m_hResult) && spPSI )
	{
		m_hResult = spPSI->Save(static_cast<IStream*>(&stream), a_bClearDirtyFlag);
		if (m_hResult == S_OK)
		{
			if (!stream.CopyData(szHTML))
				m_hResult = E_OUTOFMEMORY;
		}
		
		spPSI->Release();
	}
	
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::SetDocumentHTML(LPCTSTR szHTML)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IPersistStreamInit* spPSI = NULL;
	CStreamOnCString stream(szHTML);
	
	if (m_pHtmlDoc)
	{
		m_hResult = m_pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&spPSI);
		
		if( SUCCEEDED(m_hResult) && spPSI )
		{
			m_hResult = spPSI->Load(static_cast<IStream*>(&stream));
			spPSI->Release();
		}
	}
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetIsDirty() //returns S_OK when is dirty and S_FALSE when not dirty
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	IPersistStreamInit* spPSI = NULL;
	
	if (m_pHtmlDoc)
	{
		m_hResult = m_pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&spPSI);
		
		if( SUCCEEDED(m_hResult) && spPSI )
		{
			m_hResult = spPSI->IsDirty();
			spPSI->Release();
		}
	}
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetDocumentTitle(CString& a_sTitle)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}
	
	BSTR bstrTitle = NULL;
	m_hResult = m_pHtmlDoc->get_title(&bstrTitle);
	if( SUCCEEDED(m_hResult) )
	{
		BSTR2CString(bstrTitle, a_sTitle);
		SysFreeString(bstrTitle);
	}
	
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::GetBlockFormatNames(CStringArray &sa)
{
	CComVariant vaRet;
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(IDM_GETBLOCKFMTS);
	if (lStatus & OLECMDF_ENABLED || lStatus & OLECMDF_LATCHED)
	{
		if (S_OK == ExecCommand(IDM_GETBLOCKFMTS, OLECMDEXECOPT_DODEFAULT, NULL, &vaRet))
		{						 
			if(vaRet.vt & VT_ARRAY)
			{
				USES_CONVERSION;
				SAFEARRAY *psa = vaRet.parray;
				BSTR bstrElem = NULL;
				long lBound = 0,uBound = 0;
				if(S_OK == SafeArrayGetLBound(psa,1,&lBound) &&
					S_OK == SafeArrayGetUBound(psa,1,&uBound) )
				{
					for(long i=lBound; i<=uBound; i++)
					{	
						if( (S_OK == SafeArrayGetElement(psa, &i, &bstrElem)) )
							sa.Add(CString(OLE2T(bstrElem)));
					}		
					hr = S_OK;
				}
			}
		}
	}
	
	if (vaRet.vt == VT_ERROR)
		hr = V_ERROR(&vaRet);
	
		  return hr;
}


HRESULT CHtmlEditCtrl2::SetForeColor(LPCTSTR szColor)
{
	return ExecHelperSetVal(IDM_FORECOLOR, szColor);
}

HRESULT CHtmlEditCtrl2::SetForeColor(int nColor)
{
	return ExecHelperSetVal(IDM_FORECOLOR, nColor, OLECMDF_SUPPORTED|OLECMDF_ENABLED);
}

HRESULT CHtmlEditCtrl2::GetForeColor(int &nColor)
{
	return ExecHelperGetVal(IDM_FORECOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::GetBackColor(int& nColor)
{
	return ExecHelperGetVal(IDM_BACKCOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::SetBackColor(LPCTSTR szColor)
{
	return ExecHelperSetVal(IDM_BACKCOLOR, szColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::SetBackColor(int nColor)
{
	return ExecHelperSetVal(IDM_BACKCOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
}



HRESULT CHtmlEditCtrl2::SetDefaultComposeSettings(LPCTSTR szFontName,
												  unsigned short nFontSize,
												  COLORREF crFontColor,
												  COLORREF crFontBgColor,
												  bool bBold,
												  bool bItalic,
												  bool bUnderline )
{
	
	HRESULT hr = ExecHelperSetVal(IDM_HTMLEDITMODE, true);
	if (FAILED(hr))
		return hr;
	
	CString strBuffer;
	TCHAR szFontColor[12];
	TCHAR szBgColor[12];
	if(crFontColor == 0xFF000000)
		szFontColor[0] = 0;
	else
		if (0 > _sntprintf(szFontColor, 12, _T("%d.%d.%d"), GetRValue(crFontColor),
			GetGValue(crFontColor), GetBValue(crFontColor)))
			return E_UNEXPECTED;
		
		if (crFontBgColor == 0xFF000000)
			szBgColor[0] = 0;
		else
			if (0 > _sntprintf(szBgColor, 12, _T("%d.%d.%d"), GetRValue(crFontBgColor),
				GetGValue(crFontBgColor), GetBValue(crFontBgColor)))
				return E_UNEXPECTED;
			
			try
			{
				strBuffer.Format(_T("%d,%d,%d,%d,%s,%s,%s"),
					bBold ? 1 : 0,
					bItalic ? 1 : 0,
					bUnderline ? 1 : 0,
					nFontSize,
					szFontColor[0] ? szFontColor : _T(""),
					szBgColor[0] ? szBgColor : _T(""),
					szFontName);
			}
			catch(...)
			{
				return E_UNEXPECTED;
			}
			return ExecHelperSetVal(IDM_COMPOSESETTINGS, (LPCTSTR)strBuffer);
}

HRESULT CHtmlEditCtrl2::GetBlockFormat(CString& strFormat)
{
	return ExecHelperGetVal(IDM_BLOCKFMT, strFormat, OLECMDF_SUPPORTED|OLECMDF_ENABLED);
}

HRESULT CHtmlEditCtrl2::SetBlockFormat(LPCTSTR szFormat)
{
	return ExecHelperSetVal(IDM_BLOCKFMT, szFormat);
}

HRESULT CHtmlEditCtrl2::GetFontFace(CString& strFace)
{
	return ExecHelperGetVal(IDM_FONTNAME, strFace, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::SetFontFace(LPCTSTR szFace)
{
	return ExecHelperSetVal(IDM_FONTNAME, szFace);
}

HRESULT CHtmlEditCtrl2::IE50Paste(LPCTSTR szData)
{
	return ExecHelperSetVal(IDM_IE50_PASTE, szData);
}

HRESULT CHtmlEditCtrl2::GetBookMark(CString& strAnchor)
{
	return ExecHelperGetVal(IDM_BOOKMARK, strAnchor);
}

HRESULT CHtmlEditCtrl2::SetBookMark(LPCTSTR szAnchorName)
{
	return ExecHelperSetVal(IDM_BOOKMARK, szAnchorName,OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::SetOverwriteMode(bool bMode)
{
	return ExecHelperSetVal(IDM_OVERWRITE, bMode);
}

HRESULT CHtmlEditCtrl2::Is1DElement(bool& bValue)
{
	return ExecHelperGetVal(IDM_1D_ELEMENT, bValue);
}

HRESULT CHtmlEditCtrl2::Is2DElement(bool& bValue)
{
	return ExecHelperGetVal(IDM_2D_ELEMENT, bValue);
}

HRESULT CHtmlEditCtrl2::GetFontSize(short& nSize)
{
	return ExecHelperGetVal(IDM_FONTSIZE, nSize);
}

HRESULT CHtmlEditCtrl2::SetFontSize(unsigned short size)
{
	return ExecHelperSetVal(IDM_FONTSIZE, (short)size);
}

HRESULT CHtmlEditCtrl2::GetFrameZone(short& nZone)
{
	return ExecHelperGetVal(IDM_GETFRAMEZONE, nZone);
}

HRESULT CHtmlEditCtrl2::SetCSSEditingLevel(short nLevel)
{
	return ExecHelperSetVal(IDM_CSSEDITING_LEVEL, nLevel, 0);
}

HRESULT CHtmlEditCtrl2::HyperLink(LPCTSTR szUrl)
{
	if (szUrl)
		return ExecHelperSetVal(IDM_HYPERLINK, szUrl, OLECMDF_SUPPORTED,
		OLECMDEXECOPT_DONTPROMPTUSER);
	return ExecHelperNN(IDM_HYPERLINK, OLECMDF_SUPPORTED, OLECMDEXECOPT_DODEFAULT);
}

HRESULT CHtmlEditCtrl2::Image(LPCTSTR szUrl)
{
	if (szUrl)
		return ExecHelperSetVal(IDM_IMAGE, szUrl, OLECMDF_SUPPORTED,
		OLECMDEXECOPT_DONTPROMPTUSER);
	return ExecHelperNN(IDM_IMAGE, OLECMDF_SUPPORTED, OLECMDEXECOPT_DODEFAULT);
}

HRESULT CHtmlEditCtrl2::OrderList(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_ORDERLIST, szId);
}

HRESULT CHtmlEditCtrl2::UnorderList(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_UNORDERLIST, szId);
}


HRESULT CHtmlEditCtrl2::AddToGlyphTable(LPCTSTR szTag,
										LPCTSTR szImgUrl,
										unsigned short nTagType,
										unsigned short nAlignment,
										unsigned short nPosInfo,
										unsigned short nDirection,
										unsigned int nImgWidth,
										unsigned int nImgHeight)
{
	// Check Paramters
	if (!szTag ||
		!szImgUrl ||
		nTagType > 2 ||
		nAlignment > 3 ||
		nPosInfo > 3 ||
		nDirection > 4)
		return E_INVALIDARG;
	
	CString strGlyphEntry;
	try
	{
		strGlyphEntry.Format(_T("%%%%%s^^%%%%%s^^%%%%%d^^%%%%%d^^%%%%%d^^")
			_T("%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^**"),
			szTag, szImgUrl, nTagType, nAlignment, nPosInfo,
			nDirection, nImgWidth, nImgHeight, nImgWidth, nImgHeight);
	}
	catch(...)
	{
		return E_UNEXPECTED;
	}
	CComVariant vIn((LPCTSTR)strGlyphEntry);
	return ExecCommand(IDM_ADDTOGLYPHTABLE, OLECMDEXECOPT_DODEFAULT, &vIn);
}


HRESULT CHtmlEditCtrl2::EmptyGlyphTable()
{
	return ExecHelperNN(IDM_EMPTYGLYPHTABLE);
}

HRESULT CHtmlEditCtrl2::Button(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_BUTTON, szId);
}

HRESULT CHtmlEditCtrl2::CheckBox(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_CHECKBOX, szId);
}

HRESULT CHtmlEditCtrl2::DropDownBox(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_DROPDOWNBOX, szId);
}

HRESULT CHtmlEditCtrl2::HorizontalLine(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_HORIZONTALLINE, szId);
}

HRESULT CHtmlEditCtrl2::Iframe(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_IFRAME, szId);
}

HRESULT CHtmlEditCtrl2::InsFieldSet(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSFIELDSET, szId);
}

HRESULT CHtmlEditCtrl2::InsInputButton(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTBUTTON, szId);
}

HRESULT CHtmlEditCtrl2::InsInputHidden(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTHIDDEN, szId);
}

HRESULT CHtmlEditCtrl2::InsInputImage(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTIMAGE, szId);
}

HRESULT CHtmlEditCtrl2::InsInputPassword(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTPASSWORD, szId);
}

HRESULT CHtmlEditCtrl2::InsInputReset(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTRESET, szId);
}

HRESULT CHtmlEditCtrl2::InsInputSubmit(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTSUBMIT, szId);
}

HRESULT CHtmlEditCtrl2::InsInputUpload(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_INSINPUTUPLOAD, szId);
}

HRESULT CHtmlEditCtrl2::ListBox(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_LISTBOX, szId);
}

HRESULT CHtmlEditCtrl2::Marquee(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_MARQUEE, szId);
}

HRESULT CHtmlEditCtrl2::GetBodyBackgroundCSSText(CString& a_sStyleText)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLElement* pElem = NULL;
	m_hResult = m_pHtmlDoc->get_body(&pElem);
	
	if( SUCCEEDED(m_hResult) && pElem )
	{
		IHTMLStyle* pStyle = NULL;
		m_hResult = pElem->get_style(&pStyle);
		if(SUCCEEDED(m_hResult) && pStyle)
		{
			BSTR bstrTEMP = NULL;
			m_hResult = pStyle->get_cssText(&bstrTEMP);

			if( SUCCEEDED(m_hResult) )
			{
				BSTR2CString(bstrTEMP, a_sStyleText);
			}
			
			SysFreeString(bstrTEMP);
			pStyle->Release();
		}
		
		pElem->Release();
	}	
	return m_hResult;
}

HRESULT CHtmlEditCtrl2::Paragraph(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_PARAGRAPH, szId);
}

HRESULT CHtmlEditCtrl2::RadioButton(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_RADIOBUTTON, szId);
}

HRESULT CHtmlEditCtrl2::SaveAs(LPCTSTR szPath)
{
	return ExecHelperSetVal(IDM_SAVEAS, szPath);
}

HRESULT CHtmlEditCtrl2::TextArea(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_TEXTAREA, szId);
}

HRESULT CHtmlEditCtrl2::TextBox(LPCTSTR szId)
{
	return ExecHelperSetVal(IDM_TEXTBOX, szId);
}

HRESULT CHtmlEditCtrl2::GetAbsolutePosition(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_ABSOLUTE_POSITION) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetAbsolutePosition(bool bNewValue)
{
	return ExecHelperSetVal(IDM_ABSOLUTE_POSITION, bNewValue );
}


HRESULT CHtmlEditCtrl2::Set2DPosition(bool bNewValue)
{
	return ExecHelperSetVal(IDM_2D_POSITION, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetAtomicSelection(bool bNewValue)
{
	return ExecHelperSetVal(IDM_ATOMICSELECTION, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetAutoURLDetectMode(bool bNewValue)
{
	return ExecHelperSetVal(IDM_AUTOURLDETECT_MODE, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetDisableEditFocusUI(bool bNewValue)
{
	return ExecHelperSetVal(IDM_DISABLE_EDITFOCUS_UI, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetIE5PasteMode(bool bNewValue)
{
	return ExecHelperSetVal(IDM_IE50_PASTE_MODE, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetLiveResize(bool bNewValue)
{
	return ExecHelperSetVal(IDM_LIVERESIZE, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetMultiSelect(bool bNewValue)
{
	return ExecHelperSetVal(IDM_MULTIPLESELECTION, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetOverrideCursor(bool bNewValue)
{
	return ExecHelperSetVal(IDM_OVERRIDE_CURSOR, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::SetRespectVisInDesign(bool bNewValue)
{
	return ExecHelperSetVal(IDM_RESPECTVISIBILITY_INDESIGN, bNewValue, 0);
}

HRESULT CHtmlEditCtrl2::GetShowAlignedSiteTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWALIGNEDSITETAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowAlignedSiteTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWALIGNEDSITETAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowAllTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWALLTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowAllTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWALLTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowAreaTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWAREATAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowAreaTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWAREATAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowCommentTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWCOMMENTTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowCommentTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWCOMMENTTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowMiscTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWMISCTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowMiscTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWMISCTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::Undo()
{
	return ExecHelperNN(IDM_UNDO);
}

HRESULT CHtmlEditCtrl2::Redo()
{
	return ExecHelperNN(IDM_REDO);
}

HRESULT CHtmlEditCtrl2::Find()
{
	return ExecHelperNN(IDM_FIND);
}

HRESULT CHtmlEditCtrl2::ShowSource()
{
	return ExecHelperNN(IDM_VIEWSOURCE);
}

HRESULT CHtmlEditCtrl2::SubScriptSelectedText()
{
	return ExecHelperNN(IDM_SUBSCRIPT);
}

HRESULT CHtmlEditCtrl2::SuperScriptSelectedText()
{
	return ExecHelperNN(IDM_SUPERSCRIPT);
}

HRESULT CHtmlEditCtrl2::ShowIEOptionsDialog()
{
	return ExecHelperNN(IDM_OPTIONS);
}

HRESULT CHtmlEditCtrl2::GetShowScriptTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWSCRIPTTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowScriptTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWSCRIPTTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowStyleTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWSTYLETAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowStyleTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWSTYLETAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowUnknownTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWUNKNOWNTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowUnknownTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWUNKNOWNTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::GetShowBRTags(bool &bCurValue)
{
	bCurValue = (QueryStatus(IDM_SHOWWBRTAGS) & OLECMDF_LATCHED) != 0;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::SetShowBRTags(bool bNewValue)
{
	return ExecHelperSetVal(IDM_SHOWWBRTAGS, bNewValue);
}

HRESULT CHtmlEditCtrl2::PrintDocument()
{
	return ExecHelperNN(IDM_PRINT);
}

HRESULT CHtmlEditCtrl2::PrintDocument(LPCTSTR szPrintTemplate)
{
	return ExecHelperSetVal(IDM_PRINT, szPrintTemplate);
}

HRESULT CHtmlEditCtrl2::PrintDocument(bool bShowPrintDialog)
{
	return ExecHelperSetVal(IDM_PRINT, (short)(bShowPrintDialog ? 1 : 0));
}

HRESULT CHtmlEditCtrl2::PrintPreview()
{
	return ExecHelperNN(IDM_PRINTPREVIEW);
}

HRESULT CHtmlEditCtrl2::PrintPreview(LPCTSTR szPrintTemplate)
{
	return ExecHelperSetVal(IDM_PRINTPREVIEW, szPrintTemplate);
}

HRESULT CHtmlEditCtrl2::Bold()
{
	return ExecHelperNN(IDM_BOLD);
}

HRESULT CHtmlEditCtrl2::StrikeOut()
{
	return ExecHelperNN(IDM_STRIKETHROUGH);
}

// CopyHtml() - Copies given HTML to the clipboard.
// The HTML/BODY blanket is provided, so you only need to
// call it like CallHtml("<b>This is a test</b>");
// Source: http://support.microsoft.com/kb/q274308/
HRESULT CHtmlEditCtrl2::PutHtmlAndTextToClipboard(const CString& a_sHtmlText, const CString& a_sPlainText) 
{
    // Create temporary buffer for HTML header...
	const int iBufLen = 400 + a_sHtmlText.GetLength();
    LPTSTR buf = new TCHAR[iBufLen];
    if(!buf) return E_FAIL;

    // Get clipboard id for HTML format...
    static int cfid = 0;
    if(!cfid) cfid = RegisterClipboardFormat( _T("HTML Format") );

    // Create a template string for the HTML header...
    _tcsncpy(buf,
        _T("Version:0.9\r\n")
        _T("StartHTML:00000000\r\n")
        _T("EndHTML:00000000\r\n")
        _T("StartFragment:00000000\r\n")
        _T("EndFragment:00000000\r\n")
        _T("<html><body>\r\n")
        _T("<!--StartFragment -->\r\n"), iBufLen);

    // Append the HTML...
    _tcsncat(buf, a_sHtmlText, iBufLen);
    _tcsncat(buf, _T("\r\n"), iBufLen);
    // Finish up the HTML format...
    _tcsncat(buf,
        _T("<!--EndFragment-->\r\n")
        _T("</body>\r\n")
        _T("</html>") ,iBufLen);

    // Now go back, calculate all the lengths, and write out the
    // necessary header information. Note, wsprintf() truncates the
    // string when you overwrite it so you follow up with code to replace
    // the 0 appended at the end with a '\r'...
    TCHAR *ptr = _tcsstr(buf, _T("StartHTML"));
    _sntprintf(ptr+10, iBufLen, _T("%08u"), _tcsstr(buf, _T("<html>")) - buf);
    *(ptr+10+8) = '\r';

    ptr = _tcsstr(buf, _T("EndHTML"));
    _sntprintf(ptr+8, iBufLen, _T("%08u"), _tcslen(buf));
    *(ptr+8+8) = '\r';

    ptr = _tcsstr(buf, _T("StartFragment"));
    _sntprintf(ptr+14, iBufLen, _T("%08u"), _tcsstr(buf, _T("<!--StartFrag")) - buf);
    *(ptr+14+8) = '\r';

    ptr = _tcsstr(buf, _T("EndFragment"));
    _sntprintf(ptr+12, iBufLen, _T("%08u"), _tcsstr(buf, _T("<!--EndFrag")) - buf);
    *(ptr+12+8) = '\r';

    if(::OpenClipboard(NULL)) 
	{
        ::EmptyClipboard();

        HGLOBAL hHtmlText = GlobalAlloc(GMEM_MOVEABLE |GMEM_DDESHARE, (_tcslen(buf)+4)*sizeof(TCHAR) );
		if(hHtmlText != NULL)
		{
			TCHAR *ptr = (TCHAR *)GlobalLock(hHtmlText);
			_tcscpy(ptr, buf);
			GlobalUnlock(hHtmlText);
			::SetClipboardData(cfid, hHtmlText);
		}

		HGLOBAL hPlainText = GlobalAlloc(GMEM_MOVEABLE, a_sPlainText.GetLength()+1 * sizeof(TCHAR)); 
		if (hPlainText != NULL) 
		{ 
			LPTSTR lptstrCopy = (LPTSTR) GlobalLock(hPlainText); 
			memcpy(lptstrCopy, (LPCTSTR)a_sPlainText, a_sPlainText.GetLength() * sizeof(TCHAR)); 
			lptstrCopy[a_sPlainText.GetLength()] = (TCHAR)0;    // null character 
			GlobalUnlock(hPlainText); 
			::SetClipboardData(CF_TEXT, hPlainText); 
		} 
		
        ::CloseClipboard();
    }
	else
	{
		delete [] buf;
		return E_FAIL;
	}

    delete [] buf;
	return S_OK;
}

HRESULT CHtmlEditCtrl2::CopyEx(BOOL a_bCutText)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	CString sHTML, sPlainText;

	IHTMLSelectionObject* pSelection = NULL;
	m_hResult = m_pHtmlDoc->get_selection(&pSelection);
	if(SUCCEEDED(m_hResult) && pSelection)
	{
		IDispatch* pTextRangeDisp = NULL;
		m_hResult = pSelection->createRange(&pTextRangeDisp);
		if(SUCCEEDED(m_hResult) && pTextRangeDisp)
		{
			IHTMLTxtRange* pRange = NULL;
			m_hResult = pTextRangeDisp->QueryInterface(IID_IHTMLTxtRange, (void**)&pRange);
			if(SUCCEEDED(m_hResult) && pRange)
			{
				BSTR bstrText = NULL;
				if( SUCCEEDED( pRange->get_htmlText(&bstrText) ) && bstrText )
				{
					sHTML = bstrText;
					SysFreeString(bstrText);
				}	

				if( SUCCEEDED( pRange->get_text(&bstrText) ) && bstrText )
				{
					sPlainText = bstrText;
					SysFreeString(bstrText);
				}

				if(a_bCutText)
				{
					pRange->put_text(L"");
				}
					
				pRange->Release();
			}
			pTextRangeDisp->Release();
		}
		pSelection->Release();
	}

	if(sHTML.IsEmpty() && sPlainText.IsEmpty())
	{
		return E_FAIL;
	}

	return PutHtmlAndTextToClipboard(sHTML, sPlainText);
}

HRESULT CHtmlEditCtrl2::Copy()
{
	HRESULT hRes = ExecHelperNN(IDM_COPY);
	if( SUCCEEDED(hRes) )
	{
		return hRes;
	}

	//Standard way to copy text failed - it happens sometimes when control is in DLL file loaded in main application and main application is not a host for the control
	return CopyEx(FALSE);
}

HRESULT CHtmlEditCtrl2::Cut()
{
	HRESULT hRes = ExecHelperNN(IDM_CUT);
	if( SUCCEEDED(hRes) )
	{
		return hRes;
	}

	//Standard way to copy text failed - it happens sometimes when control is in DLL file loaded in main application and main application is not a host for the control
	return CopyEx(TRUE);
}

HRESULT CHtmlEditCtrl2::Delete()
{
	return ExecHelperNN(IDM_DELETE);
}

HRESULT CHtmlEditCtrl2::Indent()
{
	return ExecHelperNN(IDM_INDENT);
}

HRESULT CHtmlEditCtrl2::Italic()
{
	return ExecHelperNN(IDM_ITALIC);
}

HRESULT CHtmlEditCtrl2::LineBreakNormal()
{
	return ExecHelperNN(IDM_LINEBREAKNORMAL);
}

HRESULT CHtmlEditCtrl2::JustifyCenter()
{
	return ExecHelperNN(IDM_JUSTIFYCENTER);
}

HRESULT CHtmlEditCtrl2::JustifyLeft()
{
	return ExecHelperNN(IDM_JUSTIFYLEFT);
}

HRESULT CHtmlEditCtrl2::JustifyRight()
{
	return ExecHelperNN(IDM_JUSTIFYRIGHT);
}

HRESULT CHtmlEditCtrl2::Outdent()
{
	return ExecHelperNN(IDM_OUTDENT);
}

HRESULT CHtmlEditCtrl2::Paste()
{
	return ExecHelperNN(IDM_PASTE);
}

HRESULT CHtmlEditCtrl2::RemoveFormat()
{
	return ExecHelperNN(IDM_REMOVEFORMAT);
}

HRESULT CHtmlEditCtrl2::SelectAll()
{
	return ExecHelperNN(IDM_SELECTALL);
}

HRESULT CHtmlEditCtrl2::Underline()
{
	return ExecHelperNN(IDM_UNDERLINE);
}

HRESULT CHtmlEditCtrl2::Unlink()
{
	return ExecHelperNN(IDM_UNLINK);
}

HRESULT CHtmlEditCtrl2::ClearSelection()
{
	return ExecHelperNN(IDM_CLEARSELECTION);
}

HRESULT CHtmlEditCtrl2::Font()
{
	return ExecHelperNN(IDM_FONT);
}

BOOL CHtmlEditCtrl2::QueryDocumentState(BSTR a_bstrItemToQuery)
{
	if(PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}

	VARIANT_BOOL vOut = 0;
	m_pHtmlDoc->queryCommandState(a_bstrItemToQuery, &vOut);
	return (vOut == VARIANT_TRUE);
}

BOOL CHtmlEditCtrl2::IsBold()
{
	return QueryDocumentState(L"Bold");
}

BOOL CHtmlEditCtrl2::IsUnderline()
{
	return QueryDocumentState(L"Underline");
}

BOOL CHtmlEditCtrl2::IsItalic()
{
	return QueryDocumentState(L"Italic");
}

BOOL CHtmlEditCtrl2::IsStrikeOut()
{
	return QueryDocumentState(L"StrikeThrough");
}

BOOL CHtmlEditCtrl2::CanPaste() //IZ TO DO
{
	if(PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}

	VARIANT_BOOL vOut = 0;
	m_pHtmlDoc->queryCommandEnabled(L"Paste", &vOut);
	return (vOut == VARIANT_TRUE);
}

HRESULT CHtmlEditCtrl2::MoveCursorPosition(MoveType a_eType, int a_iCharacters)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLSelectionObject* pSelection = NULL;
	m_hResult = m_pHtmlDoc->get_selection(&pSelection);
	if(SUCCEEDED(m_hResult) && pSelection)
	{
		IDispatch* pTextRangeDisp = NULL;
		m_hResult = pSelection->createRange(&pTextRangeDisp);
		if(SUCCEEDED(m_hResult) && pTextRangeDisp)
		{
			IHTMLTxtRange* pRange = NULL;
			m_hResult = pTextRangeDisp->QueryInterface(IID_IHTMLTxtRange, (void**)&pRange);
			if(SUCCEEDED(m_hResult) && pRange)
			{
				int iToMove = a_iCharacters;
				if(a_eType == moveToDocStart)
				{
					iToMove = -INT_MAX;
				}
				else if(a_eType == moveToDocEnd)
				{
					iToMove = INT_MAX;
				}

				long lActual = 0;
				m_hResult = pRange->move(L"character", iToMove, &lActual); //this will move selection back
					
				if(SUCCEEDED(m_hResult))
				{
					m_hResult = pRange->select(); //this will show the new cursor point
				}

				pRange->Release();
			}
			pTextRangeDisp->Release();
		}
		pSelection->Release();
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::PasteHTMLAtCurrentSelection(const CString& a_sHTMLText, BOOL a_bSetCursorAtBeginingOfInsertedText)
{
	if(PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLSelectionObject* pSelection = NULL;
	m_hResult = m_pHtmlDoc->get_selection(&pSelection);
	if(SUCCEEDED(m_hResult) && pSelection)
	{
		IDispatch* pTextRangeDisp = NULL;
		m_hResult = pSelection->createRange(&pTextRangeDisp);
		if(SUCCEEDED(m_hResult) && pTextRangeDisp)
		{
			IHTMLTxtRange* pRange = NULL;
			m_hResult = pTextRangeDisp->QueryInterface(IID_IHTMLTxtRange, (void**)&pRange);
			if(SUCCEEDED(m_hResult) && pRange)
			{
				BSTR bstrText = a_sHTMLText.AllocSysString();
				m_hResult = pRange->pasteHTML(bstrText);
				SysFreeString(bstrText);

				if( SUCCEEDED(m_hResult) && a_bSetCursorAtBeginingOfInsertedText)
				{
					long lActual =0;
					m_hResult = pRange->move(L"character", -a_sHTMLText.GetLength(), &lActual); //this will move selection back
					
					if(SUCCEEDED(m_hResult))
					{
						m_hResult = pRange->select(); //this will show the new cursor point
					}
				}

				pRange->Release();
			}
			pTextRangeDisp->Release();
		}
		pSelection->Release();
	}

	return m_hResult;
}

HRESULT CHtmlEditCtrl2::RefreshDocument()
{
	return ExecHelperNN(IDM_REFRESH, OLECMDF_SUPPORTED);
}

HRESULT CHtmlEditCtrl2::UnBookmark()
{
	return ExecHelperNN(IDM_UNBOOKMARK);
}

HRESULT CHtmlEditCtrl2::ExecHelperNN(UINT nID, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		hr = ExecCommand(nID, nExecOpt);
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperSetVal(UINT nCmdID, LPCTSTR szID, long nMinSupportLevel, long nExecOpt)
{
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		if (szID)
		{
			CComVariant vName(szID);
			return ExecCommand(nCmdID, nExecOpt, &vName);
		}
		else
			return ExecCommand(nCmdID, nExecOpt);
	}
	return E_FAIL;
}

HRESULT CHtmlEditCtrl2::ExecHelperSetVal(UINT nCmdID, bool bValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn(bValue);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperSetVal(UINT nCmdID, short nNewVal, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn((short)nNewVal);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperSetVal(UINT nCmdID, int nNewVal, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn((int)nNewVal);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperGetVal(UINT nCmdID, bool &bValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	bValue = false;
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vOut;
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
		if (hr == S_OK)
		{
			switch(vOut.vt)
			{
			case VT_BOOL:
				bValue = vOut.boolVal == VARIANT_TRUE ? true : false;
				break;
			case VT_NULL:
			case VT_EMPTY:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vOut);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperGetVal(UINT nCmdID, short &nValue, long nMinSupportLevel, long nExecOpt)
{
	int nTempVal;
	HRESULT hr = ExecHelperGetVal(nCmdID, nTempVal, nMinSupportLevel, nExecOpt);
	if (hr == S_OK)
		nValue = (short)nTempVal;
	return hr;
}


HRESULT CHtmlEditCtrl2::ExecHelperGetVal(UINT nCmdID, int &nValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
	nValue = 0;
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vOut;
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
		if (hr == S_OK)
		{
			switch (vOut.vt)
			{
			case VT_I4:
				nValue = vOut.lVal;
				break;
			case VT_I2:
				nValue = vOut.iVal;
				break;
			case VT_EMPTY:
			case VT_NULL:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vOut);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}

HRESULT CHtmlEditCtrl2::ExecHelperGetVal(UINT nCmdID, CString& strValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	CComVariant vaRet;
	strValue.Empty();
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vaRet);
		if (hr == S_OK)
		{
			switch(vaRet.vt)
			{
			case VT_BSTR:
				{
					strValue = vaRet.bstrVal;
				}
				break;
			case VT_NULL:
			case VT_EMPTY:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vaRet);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}


/////////////////////////////////////////


//IZ: added to simulate Append method of CString from MFC70
void CHtmlEditCtrl2::CStreamOnCString::AppendToCString(CString& a_sString, LPCTSTR a_pstrToAdd, UINT a_uiLength)
{
	TCHAR* pTmp = new TCHAR[a_uiLength+1];
	if(pTmp)
	{
		RtlZeroMemory(pTmp, (a_uiLength*sizeof(TCHAR))+sizeof(TCHAR));
		_tcsncpy(pTmp, a_pstrToAdd, a_uiLength);
		a_sString += pTmp;
		delete [] pTmp;
	}
}

// Construction
CHtmlEditCtrl2::CStreamOnCString::CStreamOnCString()
{
	m_current_index = 0;
}

CHtmlEditCtrl2::CStreamOnCString::CStreamOnCString(LPCTSTR szData): m_strStream(szData)
{
	m_current_index = 0;
}

// Copies the data currently held in this
// object into a CString
BOOL CHtmlEditCtrl2::CStreamOnCString::CopyData(CString& target)
{
	target = m_strStream;
	return TRUE;
}

// Sets the value of 
BOOL CHtmlEditCtrl2::CStreamOnCString::SetData(LPCTSTR szData)
{
	try //IZ replaced: _ATLTRY
	{
		m_strStream = szData;
	}
	catch(...) //IZ replaced: _ATLCATCHALL()
	{
		return FALSE;
	}
	return TRUE;
}

// Implementation
HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::QueryInterface(REFIID iid, void **ppUnk)
{
	if (::InlineIsEqualGUID(iid, IID_IUnknown) ||
		::InlineIsEqualGUID(iid, IID_IStream) ||
		::InlineIsEqualGUID(iid, IID_ISequentialStream))
	{
		*ppUnk = (void*)this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CHtmlEditCtrl2::CStreamOnCString::AddRef( void) 
{
	return (ULONG)1;
}

ULONG STDMETHODCALLTYPE CHtmlEditCtrl2::CStreamOnCString::Release( void) 
{
	return (ULONG)1;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	if (pcbRead)
		*pcbRead = 0;
	if (m_strAnsi.GetLength() == 0)
		m_strAnsi = m_strStream;
	
	if (!pv)
		return E_POINTER;
	
	unsigned int length = m_strAnsi.GetLength();
	TCHAR *pStream = m_strAnsi.GetBuffer(0); //IZ added 0 parameter to GetBuffer call
	if (!pStream)
		return E_UNEXPECTED;
	
	TCHAR *pStart = pStream + m_current_index;
	TCHAR *pEnd = pStream + length;
	if (pStart >= pEnd)
		return S_FALSE; // no more data to read
	
	int bytes_left = (int)(pEnd-pStart);
	int bytes_to_copy = (int)min(bytes_left, (int)cb);
	if (bytes_to_copy <= 0)
	{
		// reset members so this stream can be used again
		m_current_index = 0;
		m_strAnsi.Empty();
		return S_FALSE;
	}
	
	memcpy(pv, pStream + m_current_index, bytes_to_copy);
	if (pcbRead)
		*pcbRead = (ULONG)bytes_to_copy;
	m_current_index += bytes_to_copy;
	m_strAnsi.ReleaseBuffer(0);
	return S_OK;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	if (pcbWritten)
		*pcbWritten = 0;
#ifdef _UNICODE
				int flags = IS_TEXT_UNICODE_UNICODE_MASK;
				if (0 != IsTextUnicode((LPVOID)pv, cb, &flags))
				{
					if (flags & IS_TEXT_UNICODE_ASCII16 ||
						flags & IS_TEXT_UNICODE_STATISTICS)
					{
						// compiling UNICODE and got a UNICODE buffer
						UINT nCharsToSkip = flags & IS_TEXT_UNICODE_SIGNATURE ? 1 : 0;
						//IZTOCHECK m_strStream.Append((wchar_t*)( ((wchar_t*)pv)+nCharsToSkip), (cb/sizeof(wchar_t))-nCharsToSkip);
						AppendToCString(m_strStream, (wchar_t*)( ((wchar_t*)pv)+nCharsToSkip), (cb/sizeof(wchar_t))-nCharsToSkip);
						if (pcbWritten)
							*pcbWritten = cb;
					}
				}
				else
				{
					// compiling UNICODE and got an ansi buffer
					// convert ansi buffer to UNICODE buffer
					unsigned int buffer_size = cb;
					wchar_t *pBuffer = new wchar_t[buffer_size];
					if (pBuffer)
					{
						if (buffer_size >= (unsigned int)MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pv, cb, pBuffer, buffer_size))
						{
							//IZTOCHECK m_strStream.Append(pBuffer, buffer_size);
							AppendToCString(m_strStream, pBuffer, buffer_size);
						}
					}
					delete [] pBuffer;
				}
#else
				int flags = IS_TEXT_UNICODE_UNICODE_MASK;
				if (0 != IsTextUnicode((LPVOID)pv, cb, &flags))
				{
					if (flags & IS_TEXT_UNICODE_ASCII16 ||
						flags & IS_TEXT_UNICODE_STATISTICS)
					{
						// compiling ANSI and got a UNICODE buffer
						UINT nCharsToSkip = flags & IS_TEXT_UNICODE_SIGNATURE ? 1 : 0;
						unsigned int nChars = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pv, cb,
							NULL, 0, NULL, NULL);
						
						char *pBuff = new char[nChars];
						if (pBuff)
						{
							WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pv, cb, pBuff, nChars, NULL, NULL);
							//IZ replaced: m_strStream.Append(pBuff+nCharsToSkip, nChars-nCharsToSkip);
							AppendToCString(m_strStream, pBuff+nCharsToSkip, nChars-nCharsToSkip);
							
							if (pcbWritten)
								*pcbWritten = cb;
						}
						delete [] pBuff;
					}
				}
				else
				{
					// compiling ANSI and got an ANSI buffer
					//IZ replaced: m_strStream.Append((char*)pv, cb);
					AppendToCString(m_strStream, (TCHAR*)pv, cb);
					if (pcbWritten)
						*pcbWritten = cb;
				}
#endif
				return S_OK;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Seek(LARGE_INTEGER , DWORD , ULARGE_INTEGER *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::SetSize(ULARGE_INTEGER )
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::CopyTo(IStream *, ULARGE_INTEGER , ULARGE_INTEGER *,
														   ULARGE_INTEGER *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Commit(DWORD )
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Revert( void)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::LockRegion(ULARGE_INTEGER , ULARGE_INTEGER , DWORD )
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::UnlockRegion(ULARGE_INTEGER , ULARGE_INTEGER ,DWORD )
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Stat(STATSTG *, DWORD )
{
	return E_NOTIMPL;
}

HRESULT __stdcall CHtmlEditCtrl2::CStreamOnCString::Clone(IStream **)
{
	return E_NOTIMPL;
}