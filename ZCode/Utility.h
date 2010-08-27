// Utility.h : Declaration of the CUtility

#ifndef __UTILITY_H_
#define __UTILITY_H_

#include "resource.h"       // main symbols
#include <string.h>
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CUtility
class ATL_NO_VTABLE CUtility : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUtility, &CLSID_Utility>,
	public ISupportErrorInfo,
	public IDispatchImpl<IUtility, &IID_IUtility, &LIBID_ZCODELib>
{
public:
	CUtility()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UTILITY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CUtility)
	COM_INTERFACE_ENTRY(IUtility)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IUtility
public:
	STDMETHOD(CodeReplace)(/*[in]*/ BSTR inTable,/*[in]*/ BSTR toTable,/*[in]*/ BSTR src,/*[out,retval]*/ BSTR * pVal);
	STDMETHOD(CompressWithBase64)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	STDMETHOD(CompressWithBase32)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	STDMETHOD(CompressWithBase16)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	
	STDMETHOD(DecompressWithBase64)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	STDMETHOD(DecompressWithBase32)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	STDMETHOD(DecompressWithBase16)(/*[in]*/ BSTR  src, /*[out,retval]*/ BSTR * pVal);
	
	STDMETHOD(get_Base64Table)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Base32Table)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Base16Table)(/*[out, retval]*/ BSTR *pVal);
	
	HRESULT reportError(HRESULT hr, const wchar_t * fmt, ...);

};

#endif //__UTILITY_H_
