// TableManager.h : Declaration of the CTableManager

#ifndef __TABLEMANAGER_H_
#define __TABLEMANAGER_H_

#include "resource.h"       // main symbols
#include "TableColumns.h"
#include "PersistVarXml/IPersistVarXMLImpl.h"
/////////////////////////////////////////////////////////////////////////////
// CTableManager
class ATL_NO_VTABLE CTableManager : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTableManager, &CLSID_TableManager>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITableManager, &IID_ITableManager, &LIBID_ZTableLib>,
	public IPersistVarXMLImpl<CTableManager>
{
public:

DECLARE_CLASSFACTORY_SINGLETON(CTableManager)
DECLARE_XML_ELEMENT_NAME(TableManager)

	CTableManager()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TABLEMANAGER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTableManager)
	COM_INTERFACE_ENTRY(ITableManager)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()
	
BEGIN_PROP_MAP(CTableManager)
	PROP_DATA_ENTRY("DBFile",m_bstrDBFile,VT_BSTR)
END_PROP_MAP()
	
BEGIN_XML_CONTENT_MAP(CTableManager)
//	XML_CONTENT_CREATE_OBJECT("TableColumns",__uuidof(TableColumns), OnXMLContextCreateObject)
END_XML_CONTENT_MAP()
	
	HRESULT OnXMLContextCreateObject(IPersistVarXML * pField)
{ /*this->Add(CComQIPtr<ITableColumns>(pField));*/ return S_OK; }

// ITableManager
public:
	STDMETHOD(LoadDataOnCondition)(/*[in]*/ BSTR tableName,/*[in]*/ BSTR  whereState, /*[out]*/ ITableRows ** pVal);

	STDMETHOD(RemoveTableColumn)(/*[in]*/ BSTR tableName,/*[in]*/ BSTR columnName);
	STDMETHOD(AddTableColumn)(/*[in]*/ BSTR tableName,/*[in]*/ BSTR columnName,/*[in]*/ DATA_TYPE Type);

	STDMETHOD(RemoveTable)(/*[in]*/ BSTR tableName);
	STDMETHOD(AddTable)(/*[in]*/ BSTR tableName,/*[in]*/ ITableColumns * pCols);
	STDMETHOD(get_DBFile)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DBFile)(/*[in]*/ BSTR newVal);

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	
	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;

private:
	CComBSTR m_bstrDBFile;
};

#endif //__TABLEMANAGER_H_
