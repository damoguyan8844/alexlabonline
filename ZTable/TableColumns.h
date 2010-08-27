// TableColumns.h : Declaration of the CTableColumns

#ifndef __TABLECOLUMNS_H_
#define __TABLECOLUMNS_H_

#include "resource.h"       // main symbols
#include "collectionhelper.h"
#include "TableColumn.h"
#include "PersistVarXml/IPersistVarXMLImpl.h"
/////////////////////////////////////////////////////////////////////////////
// CTableColumns
class ATL_NO_VTABLE CTableColumns: 
	public CComObjectRoot,
	public ISupportErrorInfo,
	public CComCollect<ITableColumns,&IID_ITableColumns,&LIBID_ZTableLib,
	CTableColumns,&CLSID_TableColumns,ITableColumn,&IID_ITableColumn>, 
	public IPersistVarXMLImpl<CTableColumns>
{
public:
DECLARE_XML_ELEMENT_NAME(TableColumns)

CTableColumns();

DECLARE_REGISTRY_RESOURCEID(IDR_TABLECOLUMNS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTableColumns)
	COM_INTERFACE_ENTRY(ITableColumns)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()

BEGIN_PROP_MAP(CTableColumns)
END_PROP_MAP()

BEGIN_XML_CONTENT_MAP(CTableColumns)
XML_CONTENT_CREATE_OBJECT("TableColumn",__uuidof(TableColumn), OnXMLContextCreateObject)
END_XML_CONTENT_MAP()

HRESULT OnXMLContextCreateObject(IPersistVarXML * pField)
{ this->Add(CComQIPtr<ITableColumn>(pField)); return S_OK; }

public:
	STDMETHOD(LoadByTableName)(/*[in]*/ BSTR tabName);
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	
	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;

protected:	

private:
//	HRESULT buildCollections(_Recordset *rst);
};

#endif //__TABLECOLUMNS_H_
