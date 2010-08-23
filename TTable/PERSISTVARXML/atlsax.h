// atlsax.h
//
// Basic ATL SAX parser implementation templates
//


#ifndef __ATLSAX_H__
#define __ATLSAX_H__

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error atlcom.h requires atlbase.h to be included first
#endif

// Basic COM-based SAX parser information
#import <msxml3.dll> raw_interfaces_only 
using namespace MSXML2;

//#include <map>

namespace ATL
{

//////////////////////////////////////////////////////////////////////////////
// ISAXXMLReaderImpl
template <class T>
class ATL_NO_VTABLE ISAXXMLReaderImpl : public ISAXXMLReader
{
public:
    public:
    virtual HRESULT __stdcall getFeature (
        unsigned short * pwchName,
        VARIANT_BOOL * pvfValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putFeature (
        unsigned short * pwchName,
        VARIANT_BOOL vfValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getProperty (
        unsigned short * pwchName,
        VARIANT * pvarValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putProperty (
        unsigned short * pwchName,
        VARIANT varValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getEntityResolver (
        struct ISAXEntityResolver * * ppResolver )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putEntityResolver (
        struct ISAXEntityResolver * pResolver )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getContentHandler (
        struct ISAXContentHandler * * ppHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putContentHandler (
        struct ISAXContentHandler * pHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getDTDHandler (
        struct ISAXDTDHandler * * ppHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putDTDHandler (
        struct ISAXDTDHandler * pHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getErrorHandler (
        struct ISAXErrorHandler * * ppHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putErrorHandler (
        struct ISAXErrorHandler * pHandler )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getBaseURL (
        unsigned short * * ppwchBaseUrl )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putBaseURL (
        unsigned short * pwchBaseUrl )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getSecureBaseURL (
        unsigned short * * ppwchSecureBaseUrl )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putSecureBaseURL (
        unsigned short * pwchSecureBaseUrl )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall parse (
        VARIANT varInput )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall parseURL (
        unsigned short * pwchUrl )
		{
			return S_OK;
		}
        
};

//////////////////////////////////////////////////////////////////////////////
// ISAXXMLFilterImpl
template <class T>
class ATL_NO_VTABLE ISAXXMLFilterImpl : public ISAXXMLFilter
{
public:
    virtual HRESULT __stdcall getParent (
        struct ISAXXMLReader * * ppReader )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall putParent (
        struct ISAXXMLReader * pReader )
		{
			return S_OK;
		}
};

//////////////////////////////////////////////////////////////////////////////
// ISAXLocatorImpl
template <class T>
class ATL_NO_VTABLE ISAXLocatorImpl : public ISAXLocator
{
public:
    virtual HRESULT __stdcall getColumnNumber (
        int * pnColumn )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getLineNumber (
        int * pnLine )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getPublicId (
        unsigned short * * ppwchPublicId )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getSystemId (
        unsigned short * * ppwchSystemId )
		{
			return S_OK;
		}
};
    
//////////////////////////////////////////////////////////////////////////////
// ISAXEntityResolverImpl
template <class T>
class ATL_NO_VTABLE ISAXEntityResolverImpl : public ISAXEntityResolver
{
public:
    virtual HRESULT __stdcall resolveEntity (
        unsigned short * pwchPublicId,
        unsigned short * pwchSystemId,
        VARIANT * pvarInput )
		{
			return S_OK;
		}
};

//////////////////////////////////////////////////////////////////////////////
// ISAXContentHandlerImpl

template <class T>
class ATL_NO_VTABLE ISAXContentHandlerImpl : public ISAXContentHandler   
{

public:
    virtual HRESULT __stdcall putDocumentLocator (
        struct ISAXLocator * pLocator )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall startDocument ( )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endDocument ( )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall startPrefixMapping (
        unsigned short * pwchPrefix,
        int cchPrefix,
        unsigned short * pwchUri,
        int cchUri )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endPrefixMapping (
        unsigned short * pwchPrefix,
        int cchPrefix )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall startElement (
        unsigned short * pwchNamespaceUri,
        int cchNamespaceUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * pwchQName,
        int cchQName,
        struct ISAXAttributes * pAttributes )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endElement (
        unsigned short * pwchNamespaceUri,
        int cchNamespaceUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * pwchQName,
        int cchQName )
		{
			return S_OK;
		}

    virtual HRESULT __stdcall characters (
        unsigned short * pwchChars,
        int cchChars )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall ignorableWhitespace (
        unsigned short * pwchChars,
        int cchChars )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall processingInstruction (
        unsigned short * pwchTarget,
        int cchTarget,
        unsigned short * pwchData,
        int cchData )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall skippedEntity (
        unsigned short * pwchName,
        int cchName )
		{
			return S_OK;
		}
}; 

//////////////////////////////////////////////////////////////////////////////
// ISAXDTDHandlerImpl
template <class T>
class ATL_NO_VTABLE ISAXDTDHandlerImpl : public ISAXDTDHandler   
{ 
public: 
   virtual HRESULT __stdcall notationDecl (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchPublicId,
        int cchPublicId,
        unsigned short * pwchSystemId,
        int cchSystemId )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall unparsedEntityDecl (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchPublicId,
        int cchPublicId,
        unsigned short * pwchSystemId,
        int cchSystemId,
        unsigned short * pwchNotationName,
        int cchNotationName )
		{
			return S_OK;
		}
}; 

//////////////////////////////////////////////////////////////////////////////
// ISAXErrorHandlerImpl
template <class T>
class ATL_NO_VTABLE ISAXErrorHandlerImpl : public ISAXErrorHandler   
{ 
public: 
    virtual HRESULT __stdcall error (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
	{
		ATLTRACE2(atlTraceCOM, 0, _T("ISAXErrorHandlerImpl::error\n"));
//		SetErrorInfo(0,pError);
		return S_FALSE;
	}
    virtual HRESULT __stdcall fatalError (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
	{
		ATLTRACE2(atlTraceCOM, 0, _T("ISAXErrorHandlerImpl::fatalError\n"));
//		SetErrorInfo(0,pError);
		return S_FALSE;
	}
    virtual HRESULT __stdcall ignorableWarning (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
	{
		ATLTRACE2(atlTraceCOM, 0, _T("ISAXErrorHandlerImpl::ignorableWarning\n"));
//		SetErrorInfo(0,pError);
		return S_OK;
	}
}; 

//////////////////////////////////////////////////////////////////////////////
// ISAXAttributesImpl
template <class T>
class ATL_NO_VTABLE ISAXAttributesImpl : public ISAXAttributes
{
public:
    virtual HRESULT __stdcall getLength (
        int * pnLength )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getURI (
        int nIndex,
        unsigned short * * ppwchUri,
        int * pcchUri )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getLocalName (
        int nIndex,
        unsigned short * * ppwchLocalName,
        int * pcchLocalName )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getQName (
        int nIndex,
        unsigned short * * ppwchQName,
        int * pcchQName )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getName (
        int nIndex,
        unsigned short * * ppwchUri,
        int * pcchUri,
        unsigned short * * ppwchLocalName,
        int * pcchLocalName,
        unsigned short * * ppwchQName,
        int * pcchQName )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getIndexFromName (
        unsigned short * pwchUri,
        int cchUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        int * pnIndex )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getIndexFromQName (
        unsigned short * pwchQName,
        int cchQName,
        int * pnIndex )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getType (
        int nIndex,
        unsigned short * * ppwchType,
        int * pcchType )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getTypeFromName (
        unsigned short * pwchUri,
        int cchUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * * ppwchType,
        int * pcchType )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getTypeFromQName (
        unsigned short * pwchQName,
        int cchQName,
        unsigned short * * ppwchType,
        int * pcchType )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getValue (
        int nIndex,
        unsigned short * * ppwchValue,
        int * pcchValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getValueFromName (
        unsigned short * pwchUri,
        int cchUri,
        unsigned short * pwchLocalName,
        int cchLocalName,
        unsigned short * * ppwchValue,
        int * pcchValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall getValueFromQName (
        unsigned short * pwchQName,
        int cchQName,
        unsigned short * * ppwchValue,
        int * pcchValue )
		{
			return S_OK;
		}
};


//////////////////////////////////////////////////////////////////////////////
// ISAXLexicalHandlerImpl
template <class T>
class ATL_NO_VTABLE ISAXLexicalHandlerImpl : public ISAXLexicalHandler
{
public:
    virtual HRESULT __stdcall startDTD (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchPublicId,
        int cchPublicId,
        unsigned short * pwchSystemId,
        int cchSystemId )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endDTD ( )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall startEntity (
        unsigned short * pwchName,
        int cchName )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endEntity (
        unsigned short * pwchName,
        int cchName )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall startCDATA ( )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall endCDATA ( )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall comment (
        unsigned short * pwchChars,
        int cchChars )
		{
			return S_OK;
		}
};


//////////////////////////////////////////////////////////////////////////////
// ISAXDeclHandlerImpl
template <class T>
class ATL_NO_VTABLE ISAXDeclHandlerImpl : public ISAXDeclHandler
{
public:
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall elementDecl (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchModel,
        int cchModel )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall attributeDecl (
        unsigned short * pwchElementName,
        int cchElementName,
        unsigned short * pwchAttributeName,
        int cchAttributeName,
        unsigned short * pwchType,
        int cchType,
        unsigned short * pwchValueDefault,
        int cchValueDefault,
        unsigned short * pwchValue,
        int cchValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall internalEntityDecl (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchValue,
        int cchValue )
		{
			return S_OK;
		}
    virtual HRESULT __stdcall externalEntityDecl (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchPublicId,
        int cchPublicId,
        unsigned short * pwchSystemId,
        int cchSystemId )
		{
			return S_OK;
		}
};


}; //namespace ATL

#endif // __ATLSAX_H__

/////////////////////////////////////////////////////////////////////////////
