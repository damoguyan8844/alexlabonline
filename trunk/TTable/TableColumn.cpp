// TableColumn.cpp : Implementation of CTableColumn
#include "TTableInc.h"
#include "TTable.h"
#include "TableColumn.h"

/////////////////////////////////////////////////////////////////////////////
// CTableColumn

void CTableColumn::registerFields()
{
	const TCHAR * table = _T("TableColumn");
// 	REGISTER_FIELD(m_ID, DB_I4, _T("ID"), _T("TableColumn_ID"),table, FP_ID|FP_AUTOINCREMENT);
// 	REGISTER_FIELD(m_lngIndex, DB_I4, _T("Index"), _T("TableColumn_Index"),table, FP_INSERTABLE|FP_UPDATABLE);
// 	REGISTER_FIELD(m_lngType, DB_I4, _T("Type"), _T("TableColumn_Type"),table, FP_INSERTABLE|FP_UPDATABLE);
// 	REGISTER_FIELD(m_bstrName, DB_STRING, _T("Name"), _T("TableColumn_Name"),table, FP_INSERTABLE|FP_UPDATABLE);
}

HRESULT CTableColumn::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	return S_OK;
}

STDMETHODIMP CTableColumn::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTableColumn>(this, xml);
}

STDMETHODIMP CTableColumn::LoadXML(BSTR xml)
{
	return LoadXML_COMMON<CTableColumn>(this, xml);
}

STDMETHODIMP CTableColumn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITableColumn
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTableColumn::Add(BSTR tableName, long *NewId)
{
	try {

		CComPtr<ITableManager> piManager;
		piManager.CoCreateInstance(CLSID_TableManager);
		
		CComBSTR DBFile;
		piManager->get_DBFile(&DBFile);
		
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(DBFile));

		db.execDMLEx("insert into usertablecolumns(tablename,colindex,coltype,colname) values('%s',%d,%d,'%s');",OLE2A(tableName),m_lngIndex,m_lngType,OLE2A(m_bstrName));

		m_ID = db.execScalarEx("select id from usertablecolumns where tablename='%s' and colname = '%s' ;",OLE2A(tableName),OLE2A(m_bstrName));

	}
	_CATCH_XXX()
	if(NewId)
		*NewId = m_ID;
	return S_OK;
}

STDMETHODIMP CTableColumn::Update(long TransactionLevel = 0)
{
	try {

		CComPtr<ITableManager> piManager;
		piManager.CoCreateInstance(CLSID_TableManager);
		
		CComBSTR DBFile;
		piManager->get_DBFile(&DBFile);
		
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(DBFile));
		
		db.execDMLEx("update usertablecolumns set colindex=%d,coltype=%d,colname='%s' where id=%d;",m_lngIndex,m_lngType,OLE2A(m_bstrName),m_ID);
	}
	_CATCH_XXX()
	return S_OK;
}
STDMETHODIMP  CTableColumn::Delete(long TransactionLevel = 0)
{
	try {
		CComPtr<ITableManager> piManager;
		piManager.CoCreateInstance(CLSID_TableManager);
		
		CComBSTR DBFile;
		piManager->get_DBFile(&DBFile);
		
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(DBFile));
		
		db.execDMLEx("delete from usertablecolumns where id=%d;",m_ID);
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableColumn::get_Name(BSTR *pVal)
{
	// TODO: Add your implementation code here
	m_bstrName.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CTableColumn::put_Name(BSTR newVal)
{
	// TODO: Add your implementation code here
	m_bstrName=newVal;
	return S_OK;
}

STDMETHODIMP CTableColumn::get_Type(DATA_TYPE *pVal)
{
	// TODO: Add your implementation code here
	*pVal=m_lngType;
	return S_OK;
}

STDMETHODIMP CTableColumn::put_Type(DATA_TYPE newVal)
{
	// TODO: Add your implementation code here
	m_lngType=newVal;
	return S_OK;
}

STDMETHODIMP CTableColumn::get_Index(long *pVal)
{
	// TODO: Add your implementation code here
	*pVal=m_lngIndex;
	return S_OK;
}

STDMETHODIMP CTableColumn::put_Index(long newVal)
{
	// TODO: Add your implementation code here
	m_lngIndex=newVal;
	return S_OK;
}

STDMETHODIMP CTableColumn::get_ID(long *pVal)
{
	// TODO: Add your implementation code here
	*pVal=m_ID;
	return S_OK;
}

STDMETHODIMP CTableColumn::put_ID(long newVal)
{
	// TODO: Add your implementation code here
	m_ID=newVal;
	return S_OK;
}
