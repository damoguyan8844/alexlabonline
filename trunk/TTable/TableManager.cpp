// TableManager.cpp : Implementation of CTableManager
#include "TTableInc.h"
#include "TTable.h"
#include "TableManager.h"
#include "HelpFunctions.h"

/////////////////////////////////////////////////////////////////////////////
// CTableManager

STDMETHODIMP CTableManager::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITableManager
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CTableManager::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	// TODO: Add your implementation code here
	return S_OK;
}

STDMETHODIMP CTableManager::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTableManager>(this, xml);
}

STDMETHODIMP CTableManager::LoadXML(BSTR xml)
{
	return LoadXML_COMMON<CTableManager>(this, xml);
}

STDMETHODIMP CTableManager::get_DBFile(BSTR *pVal)
{
	// TODO: Add your implementation code here
	m_bstrDBFile.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CTableManager::put_DBFile(BSTR newVal)
{
	// TODO: Add your implementation code here
	m_bstrDBFile=newVal;
	try{
		USES_CONVERSION;

		CppSQLite3DB db;
		db.open(OLE2A(m_bstrDBFile));
		
		if(!db.tableExists("usertablecolumns"))
		{
			db.execDML("create table usertablecolumns\
				(id  integer primary key autoincrement, tablename varchar(64) not null,colindex int not null,coltype int not null, colname varchar(64) not null );");
		}		
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableManager::AddTable(BSTR tableName, ITableColumns *piCols)
{
	// TODO: Add your implementation code here

	try{
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(m_bstrDBFile));
		
		db.execDMLEx("drop table if exists %s",OLE2A(tableName));
		db.execDMLEx("delete from usertablecolumns where tablename='%s';",OLE2A(tableName));

		db.execDMLEx("create table %s(id  integer primary key autoincrement);",OLE2A(tableName));	

		long newID=0;

		CComPtr<ITableColumn> piPrimaryKey;
		piPrimaryKey.CoCreateInstance(CLSID_TableColumn);
		piPrimaryKey->put_Type(DATA_INT);
		piPrimaryKey->put_Index(1);
		piPrimaryKey->put_Name(L"ID");
		piPrimaryKey->Add(tableName,&newID);
		
		long count=0;
		piCols->get_Count(&count);
		for(long index=1;index<=count;++index)
		{
			CComPtr<ITableColumn> piCol;
			piCols->get_Item(index,&piCol);
			if(piCol)
			{
				CComBSTR bstrColName;
				piCol->get_Name(&bstrColName);

				DATA_TYPE dtType=DATA_INT;
				piCol->get_Type(&dtType);

				this->AddTableColumn(tableName,bstrColName,dtType);
			}
		}

		piCols->LoadByTableName(tableName);
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableManager::RemoveTable(BSTR tableName)
{
	// TODO: Add your implementation code here
	try{
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(m_bstrDBFile));
		
		db.execDMLEx("drop table if exists %s",OLE2A(tableName));
		db.execDMLEx("delete from usertablecolumns where tablename='%s';",OLE2A(tableName));
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableManager::AddTableColumn(BSTR tableName,BSTR columnName, DATA_TYPE Type)
{
	// TODO: Add your implementation code here
	try{
		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(m_bstrDBFile));
		
		if(db.tableExists(OLE2A(tableName)))
		{
			long lastIndex=db.execScalarEx(" select max(colindex) from usertablecolumns where tableName='%s';",OLE2A(tableName));

			CComPtr<ITableColumn> piColumn;
			piColumn.CoCreateInstance(CLSID_TableColumn);
			
			long newID=-1;
			piColumn->put_Name(columnName);
			piColumn->put_Type(Type);
			piColumn->put_Index(lastIndex+1);
			piColumn->Add(tableName,&newID);

			db.execDMLEx("alter table %s add Column %s %s;",OLE2A(tableName),OLE2A(columnName),GetSQLiteTypeString(Type).c_str());	
		}
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableManager::RemoveTableColumn(BSTR tableName,BSTR columnName)
{
	// TODO: Add your implementation code here
	try{
		CComPtr<ITableColumns> piColumns;
		piColumns.CoCreateInstance(CLSID_TableColumns);
		piColumns->LoadByTableName(tableName);
		
		long count=0;
		piColumns->get_Count(&count);
		for(long index=1;index<=count;++index)
		{
			CComPtr<ITableColumn> piColumn;
			piColumns->get_Item(index,&piColumn);
			
			if(piColumn)
			{
				CComBSTR tmpName;
				piColumn->get_Name(&tmpName);
				if(tmpName==columnName)
					piColumn->Delete();
			}
		}
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableManager::LoadDataOnCondition(BSTR tableName,BSTR whereState, ITableRows **pVal)
{
	// TODO: Add your implementation code here
	try{
		CComPtr<ITableColumns> piColumns;
		piColumns.CoCreateInstance(CLSID_TableColumns);
		piColumns->LoadByTableName(tableName);

		USES_CONVERSION;
		
		CppSQLite3DB db;
		db.open(OLE2A(m_bstrDBFile));

		CComBSTR tmpName;
		DATA_TYPE dataType=DATA_INT;
		
		CComPtr<ITableRows> pRows;
		pRows.CoCreateInstance(CLSID_TableRows);
		
		std::map<std::string,long,KeyComp> mapFieldIndex;

		CppSQLite3Query q = db.execQueryEx("select * from %s where 1=1 and %s;",OLE2A(tableName),OLE2A(whereState));
		while (!q.eof())
		{
			if(mapFieldIndex.empty())
			{
				for(int i=0;i<q.numFields();i++)
				{
					mapFieldIndex[q.fieldName(i)]=i; 
				}
			}

			CComPtr<ITableRow> piRow;
			piRow.CoCreateInstance(CLSID_TableRow);

			long count=0;
			piColumns->get_Count(&count);
			for(long index=1;index<=count;++index)
			{
				CComPtr<ITableColumn> piColumn;
				piColumns->get_Item(index,&piColumn);		
				if(piColumn)
				{	
					piColumn->get_Name(&tmpName);
					piColumn->get_Type(&dataType);

					if(mapFieldIndex.find(OLE2A(tmpName)) !=mapFieldIndex.end())
					{
						switch(dataType)
						{
						case DATA_INT:
							piRow->put_IntField(tmpName,q.getIntField(mapFieldIndex[OLE2A(tmpName)],0));
							break;
						case DATA_DOUBLE:
							piRow->put_DblField(tmpName,q.getFloatField(mapFieldIndex[OLE2A(tmpName)],0.0));
							break;
						case DATA_STRING:
							piRow->put_StrField(tmpName,CComBSTR(q.getStringField(mapFieldIndex[OLE2A(tmpName)], "")));
							break;
						}
						
						tmpName.ToLower();
						if(tmpName=="id")
						{
							piRow->put_ID(q.getIntField(mapFieldIndex[OLE2A(tmpName)],-1));
						}
					}
					else
					{
						LOG_INFO("Table %s , Column %s Removed ",OLE2A(tableName),OLE2A(tmpName));
					}
				}
			}
			piRow->put_TableName(tableName);
			pRows->Add(piRow);
			q.nextRow();
		}
		pRows.CopyTo(pVal);
	}
	_CATCH_XXX()
	return S_OK;
}
