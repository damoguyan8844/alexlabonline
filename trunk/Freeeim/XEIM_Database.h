#ifndef _XEIM_DATABASE
#define _XEIM_DATABASE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

// 数据库连接需要的头文件
#include <icrsint.h>
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

class XEIM_Database
{
public:
	BOOL SetField(LPCTSTR szTable, LPCTSTR szField, LPCTSTR szData, LPCTSTR szFieldCondition, LPCTSTR szDataCondition);
	XEIM_Database();
	~XEIM_Database();

	_ConnectionPtr m_pConn;					// 数据库连接
	_RecordsetPtr m_pRs;					// 全局记录查询指针

	std::string m_strLastError;				// 错误保存

	// 数据库是否处于连接状态
	BOOL IsConnected();

	// 功能：连接 MS ACCESS 数据库；
	// 更细日期：2009年11月17日
	BOOL ConnectAccess(const char *szMDB);

	// 功能：查询某条记录是否存在；
	// 更细日期：2009年11月17日
	BOOL RecordExists(LPCTSTR szSQL);

	// 功能：执行一条 SQL 语句；
	// 更细日期：2009年11月17日
	BOOL ExecuteCommand(LPCTSTR szSQL);

	DWORD GetIdByCondition(LPCTSTR szCondition);

	// 功能：按条件读取某个字段的内容；
	// 更细日期：2009年11月17日
	void GetField(const char* szTable, const char *szField, const char *szCondition, char *szData);

	// 功能：连接 MS SQL 类型数据库；
	// 更细日期：2009年11月17日
	BOOL ConnectSQL(LPCTSTR server, LPCTSTR db, LPCTSTR user, LPCTSTR password);

	_RecordsetPtr &GetChatText(const char *szIdone, const char *szIdtwo, const char *szLastTime);
	_RecordsetPtr &GetChatTextByDay(const char *szIdone, const char *szIdtwo, const char *szLastTime);

public:
	static bool m_bStart;
	static void StartDB(const char *szMDB);
	static XEIM_Database &GetDB(void);
};

#endif // _XEIM_DATABASE