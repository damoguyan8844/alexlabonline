// TableColumn.h : Declaration of the CTableColumn

#ifndef __TABLECOLUMN_H_
#define __TABLECOLUMN_H_

#include "resource.h"       // main symbols
#include "PersistVarXml/IPersistVarXMLImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CTableColumn
class ATL_NO_VTABLE CTableColumn : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTableColumn, &CLSID_TableColumn>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITableColumn, &IID_ITableColumn, &LIBID_TTABLELib>,
	public IPersistVarXMLImpl<CTableColumn>
{
public:
	CTableColumn()
	{
		m_lngIndex=-1;
		m_lngType=DATA_INT;
		m_ID=-1;
	}
	
DECLARE_XML_ELEMENT_NAME(TableColumn)
DECLARE_REGISTRY_RESOURCEID(IDR_TABLECOLUMN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTableColumn)
	COM_INTERFACE_ENTRY(ITableColumn)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()


BEGIN_PROP_MAP(CTableColumn)
PROP_DATA_ENTRY("ID",m_ID,VT_I4)
PROP_DATA_ENTRY("Index",m_lngIndex,VT_I4)
PROP_DATA_ENTRY("Name",m_bstrName,VT_BSTR)
PROP_DATA_ENTRY("Type",m_lngType,VT_I4)
END_PROP_MAP()

BEGIN_XML_CONTENT_MAP(CTableColumn)
END_XML_CONTENT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ITableColumn
public:
	STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ID)(/*[in]*/ long newVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Index)(/*[in]*/ long newVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ DATA_TYPE *pVal);
	STDMETHOD(put_Type)(/*[in]*/ DATA_TYPE newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);

	//database
	STDMETHOD(Update)(/*[in, defaultvalue(0)]*/ long TransactionLevel);
	STDMETHOD(Add)(/*[in]*/ BSTR tableName, /*[out, retval]*/ long *NewId);
	STDMETHOD(Delete)(/*[in, defaultvalue(0)]*/ long TransactionLevel);
//	STDMETHOD(BuildFromRecordSet)(Fields * fields);
	//XML
	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;

protected:
	long	 m_ID;
	long	 m_lngIndex;
	DATA_TYPE	 m_lngType;
	CComBSTR m_bstrName;

private:
//	void persistTableColumn(PERSIST_TYPE pt, CDatabase& db);
    void registerFields();
};

#endif //__TABLECOLUMN_H_
