// freeeim_user_logic.h : header file
// 飞鸽传书 FreeEIM 主程序
// 更新日期：2009年12月3日
// 作者：蔡镇秋(freeeim@gmail.com)
// 飞鸽传书官方网站：http://www.freeeim.com/
// 功能：处理用户的通讯，包括登录，消息收发；


#ifndef _FreeEIM_User_Logic
#define _FreeEIM_User_Logic

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEIM02Dlg;
class EM_UserInfo;

class  FreeEIM_User_Logic
{
public:

	FreeEIM_User_Logic();

	~FreeEIM_User_Logic();

	CEIM02Dlg *m_pMainTop;

	// 功能：广播用户状态，登录或者下线后用
	// 更新日期：2009年12月5日
	void SendStatus(int Status);


	// 功能：发送单条消息给组里所有用户；
	// 更新日期：2009年12月5日
	void PostMessageToGroup(HTREEITEM hItem, char *szMessage);


	// 功能：发送单条消息给用户
	// 更新日期：2009年12月5日
	void PostMessageToUser(const char *szUser, char *szMessage);

	// 功能：返回本机用户信息
	// 更新日期：2009年12月5日
	EM_UserInfo *GetUserInfo();

protected:
private:
};

#endif // _FreeEIM_User_Logic