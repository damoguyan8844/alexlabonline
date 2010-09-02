#ifndef _FreeEIM_History
#define _FreeEIM_History

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
全新的 FreeEIM 将采用 ACCESS 作为数据库。可以解决创建过多历史文件的问题。
*/

class CEIM02Dlg;

class XEIM_Database;

class FreeEIM_History
{
public:
	// 功能：保存历史记录
	// 更新日期：2009年12月9日
	void SaveHistory(LPCTSTR lpszSend, LPCTSTR lpszUser, LPCTSTR lszData);
	FreeEIM_History();
	~FreeEIM_History();
private:

	// 功能：创建用户历史文件
	// 更新日期：2009年12月9日
	void CreateUserPath(LPCTSTR lpszUser);

	// 功能：获取指定用户的聊天记录保存路径
	// 更新日期：2009年12月9日
	CString GetHistoryPath();

	// 功能：判断文件是否存在
	// 更新日期：2009年12月9日
	BOOL UserPathExist(LPCTSTR lpszPath);

	CEIM02Dlg *GetMainTop();
	XEIM_Database *m_pDB;

	// 功能：创建历史文件保存路径
	// 更新日期：2009年12月9日
	void CreateHistoryPath();

	// 功能：创建一个目录
	// 更新日期：2009年12月9日
	// 来自：http://www.codeproject.com/KB/files/Folder_Utility.aspx
	void CreateDir(char* Path);

protected:
};

#endif // _FreeEIM_History