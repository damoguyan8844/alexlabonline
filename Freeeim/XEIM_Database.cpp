#include "StdAfx.h"
#include "XEIM_Database.h"

static XEIM_Database dbOnlyOne;
bool XEIM_Database::m_bStart = false;

XEIM_Database::XEIM_Database()
{
	m_pConn = NULL;
	m_pRs = NULL;
	m_strLastError = "";

	OleInitialize(NULL);

/*	ConnectSQL(m_config.m_szServer.c_str(),
		m_config.m_szDatabase.c_str(),
		m_config.m_szUserName.c_str(),
		m_config.m_szPassword.c_str());*/
//	ConnectAccess("xeimDatabase.mdb");
}

XEIM_Database::~XEIM_Database()
{
	OleUninitialize();
}

// 连接 MS SQL 类型数据库；
BOOL XEIM_Database::ConnectSQL(LPCTSTR server, LPCTSTR db, LPCTSTR user, LPCTSTR password)
{
	/*	MS SQL 的连接语句 */
	CString strConn;
	strConn.Format("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s",
		user,password,db,server);

	HRESULT hr;
	try {
		// m_pConn 在构造函数里，必须初始化为 NULL
		hr = m_pConn.CreateInstance(__uuidof(Connection));
		// 打开数据库 strConn
		m_pConn->CursorLocation=adUseClient;
		hr = m_pConn->Open((LPCTSTR)strConn, "", "", adModeUnknown);
	}
	catch(_com_error e) {
		 m_strLastError = e.Description();
	}

	if (! m_pConn->State)
	{
		m_strLastError = "数据库打开失败！";
		return FALSE;
	}

	m_pRs.CreateInstance(__uuidof(Recordset));

	return TRUE;
}



// 查询某条记录是否存在；
BOOL XEIM_Database::RecordExists(LPCTSTR szSQL)
{
	HRESULT hr;
	BOOL bRet = FALSE;

	try {
		hr = m_pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//	if (SUCCEEDED(hr))
	//		AddErrorInfo("rs failed.");

		// 如果数据库是空白的
		if (! m_pRs->adoEOF)
			bRet = TRUE;
	}

	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
		m_pRs.Release();
		return FALSE;
	}

	m_pRs->Close();
//	m_pRs.Release();

	return bRet;
}

// 执行一条 SQL 语句；
BOOL XEIM_Database::ExecuteCommand(LPCTSTR szSQL)
{
	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute(szSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}

_RecordsetPtr &XEIM_Database::GetChatText(const char *szIdone, const char *szIdtwo, const char *szLastTime)
{
	CString strReturn;
	CString strSQL;
	CTime tmNow = CTime::GetCurrentTime();

	ASSERT(NULL != szLastTime);

	if (0 == strcmp(szIdone, szIdtwo))
	{
		char *sql = "select [%s], [%s], [%s], [%s], [%s] from [%s]" 
				"where ([%s]='%s' and [%s]='%s') and ([%s] between #%s# and #%s#);";
		strSQL.Format(sql, "sender", "receiver", "message", "senddate", "recvdate",
				"freeeim", "sender", szIdone, "receiver", szIdtwo,
				"senddate", szLastTime, tmNow.Format(_T("%Y-%m-%d %H:%M:%S")));
	}
	else
	{
		char *sql = "select [%s], [%s], [%s], [%s], [%s] from [%s]"
					"where (([%s]='%s' and [%s]='%s') "
						 "or ([%s]='%s' and [%s]='%s')) and ([%s] between #%s# and #%s#);";
		strSQL.Format(sql, "sender", "receiver", "message", "senddate", "recvdate",
				"freeeim", "sender", szIdone, "receiver", szIdtwo,
				"sender", szIdtwo, "receiver", szIdone,
				"senddate", szLastTime, tmNow.Format(_T("%Y-%m-%d %H:%M:%S")));
	}
	
	_variant_t var;
	
	try
	{
		HRESULT hr;
		hr = m_pRs->Open((LPCTSTR)strSQL, m_pConn.GetInterfacePtr(), adOpenDynamic, 
			adLockOptimistic, adCmdText);

			return m_pRs;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return m_pRs;
	}
	
	return m_pRs;
}

_RecordsetPtr &XEIM_Database::GetChatTextByDay(const char *szIdone, const char *szIdtwo, const char *szLastTime)
{
	CString strReturn;
	CString strSQL;
	CTime tmNow = CTime::GetCurrentTime();
	CString strNow = tmNow.Format(_T("%Y-%m-%d %H:%M:%S"));
	
	ASSERT(NULL != szLastTime);
	
	CString daytime_begin = CString(szLastTime) + " 00:01:01";
	CString daytime_end = CString(szLastTime) + " 23:59:59";

	if (0 == strcmp(szIdone, szIdtwo))
	{
		char *sql = "select [%s], [%s], [%s], [%s], [%s] from [%s]" 
			"where ([%s]='%s' and [%s]='%s') and ([%s] between #%s# and #%s#);";
		strSQL.Format(sql, "sender", "receiver", "message", "senddate", "recvdate",
			"freeeim", "sender", szIdone, "receiver", szIdtwo,
			"senddate", daytime_begin, daytime_end);
	}
	else
	{
		char *sql = "select [%s], [%s], [%s], [%s], [%s] from [%s]"
			"where (([%s]='%s' and [%s]='%s') "
			"or ([%s]='%s' and [%s]='%s')) and ([%s] between #%s# and #%s#);";
		strSQL.Format(sql, "sender", "receiver", "message", "senddate", "recvdate",
			"freeeim", "sender", szIdone, "receiver", szIdtwo,
			"sender", szIdtwo, "receiver", szIdone,
			"senddate", daytime_begin, daytime_end);
	}
	
	_variant_t var;
	
	try
	{
		HRESULT hr;
		hr = m_pRs->Open((LPCTSTR)strSQL, m_pConn.GetInterfacePtr(), adOpenDynamic, 
			adLockOptimistic, adCmdText);

		return m_pRs;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return m_pRs;
	}
	
	return m_pRs;
}

// 通过条件获取唯一 ID
DWORD XEIM_Database::GetIdByCondition(LPCTSTR szCondition)
{
/*	HRESULT hr;
	DWORD dwRet = 0;
	try {
		hr = m_pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//	if (SUCCEEDED(hr))
	//		AddErrorInfo("rs failed.");

		// 如果数据库是空白的
		if (! m_pRs->adoEOF)
		{
			var[1] = pRs->GetCollect(_variant_t());
			if (var[1].vt != VT_NULL)
			{
				pData->SetData((LPCTSTR)_bstr_t(var[1]));
			}
		}

		pRs->Close();
	}

	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
*/
	return -1;
}

// 功能：按条件读取某个字段的内容；
// 更细日期：2009年11月17日
void XEIM_Database::GetField(const char* szTable, const char *szField, const char *szCondition, char *szData)
{
	HRESULT hr;

	CString strSQL;
	strSQL.Format("SELECT %s FROM %s WHERE %s",szField, szTable, szCondition);

	_variant_t var;
	try {
		hr = m_pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//	if (SUCCEEDED(hr))
	//		AddErrorInfo("rs failed.");

		// 如果数据库不是空白的
		if (! m_pRs->adoEOF)
		{
			// 获取数据
			var = m_pRs->GetCollect(_variant_t(szField));
			if (var.vt != VT_NULL)
			{
				strcpy(szData, (LPCTSTR)_bstr_t(var));
			}
		}
		else
		{
			szData = NULL;
		}
	}

	catch (_com_error e) {
		AfxMessageBox(e.ErrorMessage());
		m_pRs.Release();
	}

	m_pRs->Close();
}

// 连接 MS ACCESS 数据库；
BOOL XEIM_Database::ConnectAccess(const char *szMDB)
{
	CString strConn;
	/*	MS ACCESS 的连接语句	*/
	strConn.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", szMDB);

	HRESULT hr;

	try {
		hr = m_pConn.CreateInstance(__uuidof(Connection));
		// 打开数据库 strConn
		m_pConn->CursorLocation=adUseClient;
		hr = m_pConn->Open((LPCTSTR)strConn, "", "", adModeUnknown);
	}
	catch(_com_error e) {
		m_strLastError = (LPCTSTR)e.Description();
	}

	if (! m_pConn->State)
	{
		return FALSE;
	}

	m_pRs.CreateInstance(__uuidof(Recordset));

	return TRUE;
}



BOOL XEIM_Database::IsConnected()
{
	return m_pConn->State == adStateOpen;
}

BOOL XEIM_Database::SetField(LPCTSTR szTable, LPCTSTR szField, LPCTSTR szData, LPCTSTR szFieldCondition, LPCTSTR szDataCondition)
{
	BOOL bRet = FALSE;

	if (m_pConn->State != adStateOpen)
	{
		m_strLastError = "XEIM_Database::UpdateUserStatus，数据无法打开！";
		return bRet;
	}

	if (szTable == NULL
		|| szField == NULL
		|| szData == NULL)
	{
		m_strLastError = "szTable或szField，szData参数为NULL。";
		return bRet;
	}

	CString strSQL;
	strSQL.Format("UPDATE [%s] SET [%s]='%s' WHERE [%s]=%s", szTable, szField, szData, szFieldCondition, szDataCondition);

//	AfxMessageBox(strSQL);
	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
		bRet = TRUE;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	return bRet;
}

void XEIM_Database::StartDB(const char *szMDB)
{
	if(false == m_bStart)
	{
		m_bStart = true;
		dbOnlyOne.ConnectAccess(szMDB);
	}
}

XEIM_Database &XEIM_Database::GetDB()
{
	return dbOnlyOne;
}