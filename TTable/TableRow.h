// TableRow.h : Declaration of the CTableRow

#ifndef __TABLEROW_H_
#define __TABLEROW_H_

#include "resource.h"       // main symbols
#include "PersistVarXml/IPersistVarXMLImpl.h"
#include "HelpFunctions.h"
#include <string>
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CTableRow
class ATL_NO_VTABLE CTableRow : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTableRow, &CLSID_TableRow>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITableRow, &IID_ITableRow, &LIBID_ZTableLib>,
	public IPersistVarXMLImpl<CTableRow>
{
public:
	CTableRow()
	{
		m_ID=-1;
	}
DECLARE_XML_ELEMENT_NAME(TableRow)
DECLARE_REGISTRY_RESOURCEID(IDR_TABLEROW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTableRow)
	COM_INTERFACE_ENTRY(ITableRow)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()
	

BEGIN_PROP_MAP(CTableRow)
	PROP_DATA_ENTRY("ID",m_ID,VT_I4)
	PROP_DATA_ENTRY("TableName",m_bstrTableName,VT_BSTR)
	PROP_DATA_ENTRY("Value",m_bstrValue,VT_BSTR)
END_PROP_MAP()
	
BEGIN_XML_CONTENT_MAP(CTableRow)
END_XML_CONTENT_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ITableRow
public:
	STDMETHOD(get_TableName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TableName)(/*[in]*/ BSTR newVal);
	STDMETHOD(ReBuildValue)(ITableColumns *pCols);
	STDMETHOD(get_Value)(ITableColumns *pCols,/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Value)(ITableColumns *pCols,/*[in]*/ BSTR newVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ID)(/*[in]*/ long newVal);
	STDMETHOD(get_DateField)(/*[in]*/ BSTR columnName, /*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_DateField)(/*[in]*/ BSTR columnName, /*[in]*/ DATE newVal);
	STDMETHOD(get_DblField)(/*[in]*/ BSTR columnName, /*[out, retval]*/ double *pVal);
	STDMETHOD(put_DblField)(/*[in]*/ BSTR columnName, /*[in]*/ double newVal);
	STDMETHOD(get_StrField)(/*[in]*/ BSTR columnName, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_StrField)(/*[in]*/ BSTR columnName, /*[in]*/ BSTR newVal);
	STDMETHOD(get_IntField)(/*[in]*/ BSTR columnName, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_IntField)(/*[in]*/ BSTR columnName, /*[in]*/ long newVal);

	//database
	STDMETHOD(Update)(/*[in, defaultvalue(0)]*/ long TransactionLevel);
	STDMETHOD(Add)(/*[in]*/ BSTR tableName, /*[out, retval]*/ long *NewId);
	STDMETHOD(Delete)(/*[in, defaultvalue(0)]*/ long TransactionLevel);
//	STDMETHOD(BuildFromRecordSet)(Fields * fields);

	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;

private:
	long m_ID;
	CComBSTR m_bstrValue;
	std::map<std::string,_variant_t,KeyComp> m_mapValues;
	CComBSTR m_bstrTableName;
};

#endif //__TABLEROW_H_
