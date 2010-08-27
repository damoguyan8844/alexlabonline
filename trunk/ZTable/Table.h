// Table.h : Declaration of the CTable

#ifndef __TABLE_H_
#define __TABLE_H_

#include "resource.h"       // main symbols
#include "PersistVarXml/IPersistVarXMLImpl.h"
/////////////////////////////////////////////////////////////////////////////
// CTable
class ATL_NO_VTABLE CTable : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTable, &CLSID_Table>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITable, &IID_ITable, &LIBID_ZTableLib>,
	public IPersistVarXMLImpl<CTable>
{
public:
	CTable()
	{
	}
DECLARE_XML_ELEMENT_NAME(Table)
DECLARE_REGISTRY_RESOURCEID(IDR_TABLE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTable)
	COM_INTERFACE_ENTRY(ITable)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()
	
	
BEGIN_PROP_MAP(CTable)
PROP_DATA_ENTRY("Name",m_bstrName,VT_BSTR)
END_PROP_MAP()
	
BEGIN_XML_CONTENT_MAP(CTable)
XML_CONTENT_CREATE_OBJECT("TableColumns",__uuidof(TableColumns), OnXMLContextCreateObject_Columns)
END_XML_CONTENT_MAP()

HRESULT OnXMLContextCreateObject_Columns(IPersistVarXML * pField)
{ return putref_Columns(CComQIPtr<ITableColumns>(pField));  }

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ITable
public:
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Columns)(/*[out, retval]*/ ITableColumns * *pVal);
	STDMETHOD(putref_Columns)(/*[in]*/ ITableColumns * newVal);
	
	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;

private:
	CComPtr<ITableColumns> m_piColumns;
	CComBSTR m_bstrName;
};

#endif //__TABLE_H_
