// freeeim_config.h : header file
// 飞鸽传书 FreeEIM 主程序
// 更新日期：2009年12月3日
// 作者：蔡镇秋(freeeim@gmail.com)
// 飞鸽传书官方网站：http://www.freeeim.com/
// 功能：读取程序配置文件

#ifndef _FreeEIM_Config
#define _FreeEIM_Config

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 文字消息
#define FREEEIM_NONE		0x00
#define FREEEIM_TEXT		0x01

class CEIM02Dlg;
class EM_UserInfo;

class FreeEIM_Config
{

	// 功能：读取配置文件
	// 更新日期：2009年12月5日
	void ReadConfig();

public:
	FreeEIM_Config();
	~FreeEIM_Config();

	CEIM02Dlg *m_pMainTop;
	CString m_strDisplayName;
	CString m_strGroup;

	// 功能：读取本地计算机名
	// 更新日期：2009年12月5日
	CString GetComputerName();


	// 功能：保存配置文件
	// 更新日期：2009年12月5日
	void SaveConfig();


	// 功能：读取用户名
	// 更新日期：2009年12月5日
	CString GetDisplayName();


	// 功能：读取用户部门
	// 更新日期：2009年12月5日
	CString GetGroup();


	// 用户信息
	EM_UserInfo *m_pUserInfo;
	// 功能：返回EM_UserInfo
	// 更新日期：2009年12月5日
	/*
	为了与旧版本兼容，只有放弃新方案了，兼容才是最重要的。
	*/
	EM_UserInfo *GetUserInfo();

private:
protected:
};

#endif // _FreeEIM_Config