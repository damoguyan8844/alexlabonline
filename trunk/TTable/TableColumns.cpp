// TableColumns.cpp : Implementation of CTableColumns
#include "TTableInc.h"
#include "TTable.h"
#include "TableColumns.h"

/////////////////////////////////////////////////////////////////////////////
// CTableColumns

CTableColumns::CTableColumns()
{
}
STDMETHODIMP CTableColumns::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITableColumns
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CTableColumns::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	// TODO: Add your implementation code here
	return SaveXMLContent_T<ITableColumn>(pISAXContentHandler,this);
}

STDMETHODIMP CTableColumns::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTableColumns>(this, xml);
}

STDMETHODIMP CTableColumns::LoadXML(BSTR xml)
{
	this->RemoveAll();
	return LoadXML_COMMON<CTableColumns>(this, xml);
}

STDMETHODIMP CTableColumns::LoadByTableName(BSTR tabName)
{
	// TODO: Add your implementation code here
	try
	{
		CComPtr<ITableManager> piManager;
		piManager.CoCreateInstance(CLSID_TableManager);
		
		CComBSTR DBFile;
		piManager->get_DBFile(&DBFile);
		
		USES_CONVERSION;
		
		CppSQLite3DB db;
        db.open(OLE2A(DBFile));
		
		if(!db.tableExists("UserTableColumns"))
		{
			db.execDML("create table UserTableColumns\
				(id  integer primary key autoincrement, tablename varchar(64) not null,colindex int not null,coltype int not null,colname varchar(64) not null );");

			return S_OK;
		}
		
		this->RemoveAll();

		CppSQLite3Query q = db.execQueryEx("select * from UserTableColumns where tablename='%s';",OLE2A(tabName));
        while (!q.eof())
        {
			CComPtr<ITableColumn> piColumn;
			piColumn.CoCreateInstance(CLSID_TableColumn);
			piColumn->put_ID(q.getIntField(0));
            piColumn->put_Index(q.getIntField(2));
            piColumn->put_Type(DATA_TYPE(q.getIntField(3)));
			piColumn->put_Name(CComBSTR(q.getStringField(4)));

			this->Add(piColumn);

            q.nextRow();
        }
	}
	_CATCH_XXX()

	return S_OK;
}
