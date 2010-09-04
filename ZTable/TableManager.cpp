// TableManager.cpp : Implementation of CTableManager
#include "ZTableInc.h"
#include "ZTable.h"
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
						case DATA_BIN:
							CppSQLite3Binary blob;
							blob.setEncoded(reinterpret_cast<const unsigned char *>(q.fieldValue(mapFieldIndex[OLE2A(tmpName)])));
							piRow->put_BinField(tmpName,CComBSTR(reinterpret_cast<const char *>(blob.getBinary())));
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

		q.finalize();
		
		pRows.CopyTo(pVal);
	}
	_CATCH_XXX()
	return S_OK;
}


HRESULT CTableManager::reportError(HRESULT hr, const wchar_t * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	wchar_t temp[300];
	memset(temp, 0, sizeof(temp));
	_vsnwprintf(temp, sizeof(temp)/sizeof(wchar_t)-1, fmt, args);
	if (hr == E_OUTOFMEMORY) {
		MEMORYSTATUS stat;
		GlobalMemoryStatus (&stat);
		wchar_t buf[100];
		swprintf(buf, L"[vm: %d/%d]", stat.dwAvailVirtual, stat.dwTotalVirtual);
		wcscat(temp, buf);
	}
	AtlReportError(GetObjectCLSID(), temp, GUID_NULL, hr);
	
	va_end(args);
	
	return hr;
}

STDMETHODIMP CTableManager::ReadFile(BSTR fileName, BSTR *pVal)
{
	// TODO: Add your implementation code here

	unsigned long nInFile = ::SysStringLen(fileName);
	if (nInFile <= 8)
		return S_OK;
	char * pInFile = new char[nInFile+1];
	if (pInFile == 0)
		return reportError(E_OUTOFMEMORY, L"ZTable::ReadFile() - InFile - new %d bytes failed", nInFile+1);
	Destroyer dInFile(pInFile);
	
	::ZeroMemory(pInFile, sizeof(pInFile));
	nInFile = ::WideCharToMultiByte(CP_ACP, 0, fileName, -1, pInFile, nInFile+1, 0, 0);
	if (nInFile > 0) nInFile--;

	FILE *fp=0;
	long filesize = 0;
	char * ffile=0;
	fp = fopen(pInFile, "rb");
	if(fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		ffile = new char[filesize+1];

		if (ffile == 0)
			return reportError(E_OUTOFMEMORY, L"ZTable::ReadFile() - new %d bytes failed", filesize+1);
		Destroyer ddest(ffile);

		size_t sz = fread(ffile, sizeof(char), filesize+1, fp);		
		fclose(fp);

		CComBSTR(ffile).CopyTo(pVal);
		if (*pVal == 0)
			return reportError(E_OUTOFMEMORY, L"ZTable::ReadFile() - A2WBSTR() returns 0 for %d bytes string", filesize);

	}

	return S_OK;
}

STDMETHODIMP CTableManager::WriteFile(BSTR fileName, BSTR binVal)
{
	// TODO: Add your implementation code here

	unsigned long nInFile = ::SysStringLen(fileName);
	if (nInFile <= 8)
		return S_OK;
	char * pInFile = new char[nInFile+1];
	if (pInFile == 0)
		return reportError(E_OUTOFMEMORY, L"ZTable::WriteFile() - InFile - new %d bytes failed", nInFile+1);
	Destroyer dInFile(pInFile);
	
	::ZeroMemory(pInFile, sizeof(pInFile));
	nInFile = ::WideCharToMultiByte(CP_ACP, 0, fileName, -1, pInFile, nInFile+1, 0, 0);
	if (nInFile > 0) nInFile--;
	

	unsigned long nInContent = ::SysStringLen(binVal);
	if (nInContent <= 8)
		return S_OK;
	char * pInContent = new char[nInContent+1];
	if (pInContent == 0)
		return reportError(E_OUTOFMEMORY, L"ZTable::WriteFile() - InContent - new %d bytes failed", nInContent+1);
	Destroyer dInContent(pInContent);
	
	::ZeroMemory(pInContent, sizeof(pInContent));
	nInContent = ::WideCharToMultiByte(CP_ACP, 0, binVal, -1, pInContent, nInContent+1, 0, 0);
	if (nInContent > 0) nInContent--;
	

	FILE *fp2;
	fp2 = fopen(pInFile, "wb");
	if(fp2 != NULL)
	{
		size_t ret = fwrite(pInContent, sizeof(char), nInContent, fp2);
		fclose(fp2);
	}

	return S_OK;
}
