// TableRows.cpp : Implementation of CTableRows
#include "ZTableInc.h"
#include "ZTable.h"
#include "TableRows.h"

/////////////////////////////////////////////////////////////////////////////
// CTableRows


STDMETHODIMP CTableRows::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITableRows
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CTableRows::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	// TODO: Add your implementation code here
	return SaveXMLContent_T<ITableRow>(pISAXContentHandler,this);
}

STDMETHODIMP CTableRows::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTableRows>(this, xml);
}

STDMETHODIMP CTableRows::LoadXML(BSTR xml)
{
	this->RemoveAll();
	return LoadXML_COMMON<CTableRows>(this, xml);
}
