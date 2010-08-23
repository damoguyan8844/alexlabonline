// Table.cpp : Implementation of CTable
#include "TTableInc.h"
#include "TTable.h"
#include "Table.h"

/////////////////////////////////////////////////////////////////////////////
// CTable

STDMETHODIMP CTable::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITable
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CTable::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	if(m_piColumns!=NULL){
		CComQIPtr<IPersistVarXML> pTemp(m_piColumns);
		pTemp->SaveXML(CComVariant(pISAXContentHandler));
	}
	return S_OK;
}

STDMETHODIMP CTable::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTable>(this, xml);
}

STDMETHODIMP CTable::LoadXML(BSTR xml)
{
	return LoadXML_COMMON<CTable>(this, xml);
}

STDMETHODIMP CTable::get_Columns(ITableColumns **pVal)
{
	// TODO: Add your implementation code here
	m_piColumns.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CTable::putref_Columns(ITableColumns *newVal)
{
	// TODO: Add your implementation code here
	m_piColumns=newVal;
	return S_OK;
}

STDMETHODIMP CTable::get_Name(BSTR *pVal)
{
	// TODO: Add your implementation code here
	m_bstrName.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CTable::put_Name(BSTR newVal)
{
	// TODO: Add your implementation code here
	m_bstrName=newVal;
	return S_OK;
}

