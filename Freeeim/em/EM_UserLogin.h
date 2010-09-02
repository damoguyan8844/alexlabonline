// EM_UserLogin.h: interface for the EM_UserLogin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EM_USERLOGIN_H__D72DDE04_00AF_491C_90A7_190133794F41__INCLUDED_)
#define AFX_EM_USERLOGIN_H__D72DDE04_00AF_491C_90A7_190133794F41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EM_UserTree.h"
#include "stdemdef.h"
#include "em_userinfo.h"

// 本地用户信息
// static EM_UserInfo g_uiLocal;

class EM_UserLogin
{
private:
	// 广播消息接收处理函数
	static DWORD WINAPI Proc_BC_Recv(LPVOID lParam);
	// UDP消息接收处理函数
	static DWORD WINAPI Proc_UDP_Recv(LPVOID lParam);

public:
	// 发送UDP登陆消息到指定的IP地址
	void SendLogin_UDP(LPCTSTR lpszIP);
	// 发送广播登陆消息
	void SendLogin_BC(LPEM_DATA msg);

public:
	void OffLine();
	void OnLine();
	void ReName(LPCTSTR lpszName);
	void ReGroup(LPCTSTR lpszGroup);

	// 构造函数
	EM_UserLogin();

	// 析构函数
	~EM_UserLogin();

	// 执行函数
	void Run(EM_UserTree &hTree);

	// 关闭所有线程
	void Close();

protected:

	// UDP接收线程句柄
	HANDLE m_hUDP;

	// BC接收线程句柄
	HANDLE m_hBC;
};

#endif // !defined(AFX_EM_USERLOGIN_H__D72DDE04_00AF_491C_90A7_190133794F41__INCLUDED_)
