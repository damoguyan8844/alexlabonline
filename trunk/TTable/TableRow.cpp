// TableRow.cpp : Implementation of CTableRow
#include "TTableInc.h"
#include "TTable.h"
#include "TableRow.h"

/////////////////////////////////////////////////////////////////////////////
// CTableRow

HRESULT CTableRow::IPersistVarXML_SaveContent (ISAXContentHandler* pISAXContentHandler)
{
	return S_OK;
}

STDMETHODIMP CTableRow::SaveXML(BSTR *xml)
{
	return SaveXML_COMMON<CTableRow>(this, xml);
}

STDMETHODIMP CTableRow::LoadXML(BSTR xml)
{
	return LoadXML_COMMON<CTableRow>(this, xml);
}

STDMETHODIMP CTableRow::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITableRow
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTableRow::get_IntField(BSTR columnName, long *pVal)
{
	// TODO: Add your implementation code here
	*pVal=0;
	USES_CONVERSION;
	std::map<std::string,_variant_t>::iterator iter=m_mapValues.find(OLE2A(columnName));
	if(iter!=m_mapValues.end())
		*pVal=iter->second;
	return S_OK;
}

STDMETHODIMP CTableRow::put_IntField(BSTR columnName, long newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	m_mapValues[OLE2A(columnName)]=newVal;

	return S_OK;
}

STDMETHODIMP CTableRow::get_StrField(BSTR columnName, BSTR *pVal)
{
	// TODO: Add your implementation code here
	*pVal=0;
	USES_CONVERSION;
	std::string strTemp;
	std::map<std::string,_variant_t>::iterator iter=m_mapValues.find(OLE2A(columnName));
	if(iter!=m_mapValues.end())
		strTemp=(const char *)_bstr_t(iter->second);
	*pVal=CComBSTR(strTemp.c_str()).Detach();
	return S_OK;
}

STDMETHODIMP CTableRow::put_StrField(BSTR columnName, BSTR newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	m_mapValues[OLE2A(columnName)]=OLE2A(newVal);
	return S_OK;
}

STDMETHODIMP CTableRow::get_DblField(BSTR columnName, double *pVal)
{
	// TODO: Add your implementation code here
	*pVal=0.0;
	USES_CONVERSION;
	std::map<std::string,_variant_t>::iterator iter=m_mapValues.find(OLE2A(columnName));
	if(iter!=m_mapValues.end())
		*pVal=iter->second;
	return S_OK;
}

STDMETHODIMP CTableRow::put_DblField(BSTR columnName, double newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	m_mapValues[OLE2A(columnName)]=newVal;
	return S_OK;
}

STDMETHODIMP CTableRow::get_DateField(BSTR columnName, DATE *pVal)
{
	// TODO: Add your implementation code here
	get_DblField(columnName,pVal);
	return S_OK;
}

STDMETHODIMP CTableRow::put_DateField(BSTR columnName, DATE newVal)
{
	// TODO: Add your implementation code here
	put_DblField(columnName,newVal);
	return S_OK;
}

STDMETHODIMP CTableRow::get_ID(long *pVal)
{
	// TODO: Add your implementation code here
	*pVal=m_ID;
	return S_OK;
}

STDMETHODIMP CTableRow::put_ID(long newVal)
{
	// TODO: Add your implementation code here
	m_ID=newVal;
	return S_OK;
}

STDMETHODIMP CTableRow::get_Value(ITableColumns *piColumns, BSTR *pVal)
{
	// TODO: Add your implementation code here
	if(piColumns==0) return S_OK;
	try{
		USES_CONVERSION;

		CComBSTR tmpName;
		DATA_TYPE dataType=DATA_INT;

		CComBSTR dataStr;
		double	dataDbl=0.0;
		long	dataInt=0;

		char datatmp[1024];
		char split=' ';
		std::string strRet;

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
				switch(dataType)
				{
				case DATA_INT:
					this->get_IntField(tmpName,&dataInt);
					sprintf(datatmp,"%c%d",split,dataInt);
					break;
				case DATA_DOUBLE:
					this->get_DblField(tmpName,&dataDbl);
					sprintf(datatmp,"%c%f",split,dataDbl);
					break;
				case DATA_STRING:
					this->get_StrField(tmpName,&dataStr);
					sprintf(datatmp,"%c%s",split,OLE2A(dataStr));
					break;
				}
				split=',';

				strRet+=datatmp;
				datatmp[0]='\0';
			}
		}
		
		if(!strRet.empty())
		{	
			m_bstrValue=&strRet[1];
			m_bstrValue.CopyTo(pVal);
		}
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableRow::put_Value(ITableColumns *piColumns,BSTR newVal)
{
	// TODO: Add your implementation code here
	if(piColumns==0) return S_OK;
		try{	
		USES_CONVERSION;

		m_mapValues.clear();
		m_bstrValue=newVal;
		std::string  newStr=OLE2A(newVal);

		double	dataDbl=0.0;
		long	dataInt=0;
		
		char split=',';
		long tokenEnd=0;
		CComBSTR tmpName;
		DATA_TYPE dataType=DATA_INT;

		long count=0;
		piColumns->get_Count(&count);
		for(long index=1;index<=count && tokenEnd<newStr.length();++index)
		{
			CComPtr<ITableColumn> piColumn;
			piColumns->get_Item(index,&piColumn);

			if(piColumn)
			{
				char * ptoken=&newStr[tokenEnd];
				
				while(tokenEnd<newStr.length()) 
				{
					if(newStr[tokenEnd++]==split)
					{
						newStr[tokenEnd-1]='\0';
						break;
					}
				}
				piColumn->get_Name(&tmpName);
				piColumn->get_Type(&dataType);
				switch(dataType)
				{
				case DATA_INT:
					sscanf(ptoken,"%d",dataInt);
					this->put_IntField(tmpName,dataInt);
					break;
				case DATA_DOUBLE:
					sscanf(ptoken,"%f",dataDbl);
					this->put_DblField(tmpName,dataDbl);
					break;
				case DATA_STRING:
					this->put_StrField(tmpName,CComBSTR(ptoken));
					break;
				}
			}
		}
	}
	_CATCH_XXX()
	return S_OK;
}

STDMETHODIMP CTableRow::ReBuildValue(ITableColumns *pCols)
{
	// TODO: Add your implementation code here
	CComBSTR bstrTmpValue;
	this->get_Value(pCols,&bstrTmpValue);
	this->put_Value(pCols,bstrTmpValue);
	return S_OK;
}
