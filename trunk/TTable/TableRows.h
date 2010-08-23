// TableRows.h : Declaration of the CTableRows

#ifndef __TABLEROWS_H_
#define __TABLEROWS_H_

#include "resource.h"       // main symbols
#include "collectionhelper.h"
#include "TableRow.h"
#include "persistvarxml/IPersistVarXMLImpl.h"
/////////////////////////////////////////////////////////////////////////////
// CTableRows
class ATL_NO_VTABLE CTableRows : 
	public CComObjectRoot,
	public ISupportErrorInfo,
	public CComCollect<ITableRows,&IID_ITableRows,&LIBID_TTABLELib,
	CTableRows,&CLSID_TableRows,ITableRow,&IID_ITableRow>, 
	public IPersistVarXMLImpl<CTableRows>
{
public:
DECLARE_XML_ELEMENT_NAME(TableRows)
		
CTableRows()
{
}
	
DECLARE_REGISTRY_RESOURCEID(IDR_TABLEROWS)
		
DECLARE_PROTECT_FINAL_CONSTRUCT()
		
BEGIN_COM_MAP(CTableRows)
	COM_INTERFACE_ENTRY(ITableRows)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISAXContentHandler)
	COM_INTERFACE_ENTRY(IPersistVarXML)
END_COM_MAP()
		
BEGIN_PROP_MAP(CTableRows)
END_PROP_MAP()
		
BEGIN_XML_CONTENT_MAP(CTableRows)
	XML_CONTENT_CREATE_OBJECT("TableRow",__uuidof(TableRow), OnXMLContextCreateObject)
END_XML_CONTENT_MAP()
		
	HRESULT OnXMLContextCreateObject(IPersistVarXML * pField)
	{ this->Add(CComQIPtr<ITableRow>(pField)); return S_OK; }
	
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	
	STDMETHOD(LoadXML)(/*[in]*/ BSTR xml);
	STDMETHOD(SaveXML)(/*[out,retval]*/ BSTR * xml);
	HRESULT IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler);
	unsigned m_bRequiresSave:1;
	
protected:	
	
};

#endif //__TABLEROWS_H_
