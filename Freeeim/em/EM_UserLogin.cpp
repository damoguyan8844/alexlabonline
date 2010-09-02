// EM_UserLogin.cpp: implementation of the EM_UserLogin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdemdef.h"
#include "EM_UserLogin.h"
#include "em_userinfo.h"
#include "EM_UserTree.h"
#include "winsock2.h"
#include "../EIM02Dlg.h"
// 为了回复登陆信息时不同时发送，采用随机数
#include "stdlib.h"
#include "time.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EM_UserLogin::EM_UserLogin()
{
	m_hBC = NULL;
	m_hUDP = NULL;
}

EM_UserLogin::~EM_UserLogin()
{
	Close();
}

// 关闭所有线程
void EM_UserLogin::Close()
{
	if (m_hUDP != NULL)
	{
		TerminateThread(m_hUDP, 0);
		CloseHandle(m_hUDP);
		m_hUDP = NULL;
	}
	if (m_hBC != NULL)
	{
		TerminateThread(m_hBC, 0);
		CloseHandle(m_hBC);
		m_hBC = NULL;
	}
}

void EM_UserLogin::Run(EM_UserTree &hTree)
{
	DWORD dwTID1 = 0,
		  dwTID2 = 0;

	// 创建BC接受线程
	m_hBC = CreateThread(NULL, 0, Proc_BC_Recv, &hTree, 0, &dwTID1);
	// 创建UDP接受线程
	m_hUDP = CreateThread(NULL, 0, Proc_UDP_Recv, &hTree, 0, &dwTID2);
}

// 发送广播登陆消息
void EM_UserLogin::SendLogin_BC(LPEM_DATA msg)
{
	CEIM02Dlg *pDlg = (CEIM02Dlg*)AfxGetMainWnd();

	SOCKET m_socket;
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (INVALID_SOCKET == m_socket)
	{
		MessageBox(0, "socket failed.", NULL, MB_OK);
	}
	SOCKADDR_IN sin;

	sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(BROADCAST_SEND_PORT);

	BOOL bOpt = TRUE;
	if (SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt)))
	{
		// Error occurred
		::MessageBox(0,"BC setsockopt error.",0,0);
		return;
	}
	char *tmpBuf = msg->GetBuffer();
	if (SOCKET_ERROR == sendto(m_socket, tmpBuf, msg->GetLength(), 0, (PSOCKADDR)&sin, sizeof(sin)))
	{
		// Error occurred
		if (pDlg->m_dwStatus == CEIM02Dlg::STATUS_ONLINE)
		{
			CString strInfo;
			strInfo.Format(_T("网络连接失败，%s将自动退出。"), APP_TITLE);
			pDlg->m_dwStatus = CEIM02Dlg::STATUS_OFFLINE;
			pDlg->MessageBox(strInfo, _T("网络连接断开"), MB_OK | MB_ICONERROR);
			pDlg->EM_QuitEIM();
			return;
		}
		else
		{
		//	::MessageBox(0,"STATUS_ONLINE",0,0);
			return;
		}
	}

	closesocket(m_socket);
	pDlg->m_dwStatus = CEIM02Dlg::STATUS_ONLINE;
}

// 发送UDP登陆消息到指定的IP地址
void EM_UserLogin::SendLogin_UDP(LPCTSTR lpszIP)
{
	CEIM02Dlg* pDlg = ((CEIM02Dlg*)AfxGetMainWnd());

	srand(time(NULL));
	int nTimeToWait = rand() / 1000;
	Sleep(nTimeToWait);

	EM_DATA myinfo;
	myinfo.msg = EM_USERLOGIN;

	EM_UserInfo eInfo;
	eInfo.SetDisplayName(pDlg->m_config.m_strDisplayName);
	eInfo.SetGroupName(pDlg->m_config.m_strGroup);

	myinfo.len = sizeof(eInfo);
	myinfo.buf = (char*)&eInfo;

	SOCKET m_sock;
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock)
	{
		closesocket(m_sock);
		m_sock = NULL;
		AfxMessageBox("socket creating error.");
		return;
	}
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr(lpszIP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP_SEND_PORT);
	char *tmpBuf = myinfo.GetBuffer();
	if (sendto(m_sock, tmpBuf, myinfo.GetLength(), 0, (PSOCKADDR)&sin, sizeof(sin))
		== SOCKET_ERROR)
	{
		AfxMessageBox(_T("EM_UserLogin::SendLogin_UDP sendto failed"));
		return;
	}

	closesocket(m_sock);
}

// 回调函数
DWORD WINAPI EM_UserLogin::Proc_BC_Recv(LPVOID lParam)
{
	CEIM02Dlg* pDlg = ((CEIM02Dlg*)AfxGetMainWnd());
	HWND hWnd = pDlg->GetSafeHwnd();

	char buf[4096];
	char ip[128];
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == sock)
	{
		AfxMessageBox(_T("socket() error in thread BCReceiver::MsgRecvProc"));
		return -1;
	}

	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(BROADCAST_RECV_PORT);

	int fromlen = sizeof(sin);

	if (SOCKET_ERROR == bind(sock, (PSOCKADDR)&sin, fromlen))
	{
		AfxMessageBox(_T("Broadcast receiver bind failed"));
		return -1;
	}
	int recvlen = 0;

	while (TRUE)
	{
		recvlen = recvfrom(sock, buf, 4096, 0, (PSOCKADDR)&sin, &fromlen);
		// 需要优化
		EM_DATA data(buf, recvlen);
		if (data.msg == EM_USERLOGIN
			|| data.msg == EM_USERLOGOUT
			|| data.msg == EM_USERRENAME
			|| data.msg == EM_USERREGROUP)
		{
			FreeEIM_USER *pUser;
			strcpy(ip, inet_ntoa(sin.sin_addr));
			char name[512];
			char hostname[128];
			char groupname[128];
			EM_UserInfo *ui = ((EM_UserInfo*)data.buf);
			ui->GetDisplayName(name);
			ui->GetHostName(hostname);
			ui->GetGroupName(groupname);
			EM_USERINFO uinfo(strupr(hostname), ip, groupname);

			if (data.msg == EM_USERLOGIN)
			{
				char *tmp = new char[128];
				strcpy(tmp, ip);
				// Add to tree ---------------------------------------------------
				pDlg->_User_AddNewUser(name, uinfo);
				// ===============================================================
				// Reply that i was online.---------------------------------------
				pDlg->SendMessage(WM_REPLYBCMSG, (WPARAM)tmp, NULL);
				// ===============================================================
			}
			else if (data.msg == EM_USERLOGOUT)
			{
				char *tmp = new char[128];
				strcpy(tmp, ip);
				::PostMessage(hWnd, WM_USERLOGOUT, (WPARAM)tmp, NULL);
			//	pDlg->_User_DeleteUser(uinfo);
			}
			else
			{
				// 重新分配内存
				pUser = new FreeEIM_USER;

				if (data.msg == EM_USERRENAME)
					strcpy(pUser->m_szName, data.buf);
				else
					strcpy(pUser->m_szGroup, data.buf);

				strcpy(pUser->m_szIP, inet_ntoa(sin.sin_addr));

				::PostMessage(hWnd, WM_DESKUPDATEINFO, data.msg, (LPARAM)pUser);
			}
		}
	}
	return 1;
}

DWORD WINAPI EM_UserLogin::Proc_UDP_Recv(LPVOID lParam)
{
	CEIM02Dlg* pDlg = ((CEIM02Dlg*)AfxGetMainWnd());
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		::MessageBox(NULL, _T("Socket creating error."), _T("接受线程错误"), MB_OK);
		return -1;
	}

	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP_RECV_MSG_PORT);

	if (bind(s, (PSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		::MessageBox(NULL, _T("Socket binding error."), _T("接受线程错误"), MB_OK);
	}

	SOCKADDR_IN client;
	int sinLen;
	int ret;
	char buf[8192];
	char ip[128];
	while(1)
	{
		sinLen = sizeof(SOCKADDR_IN);
		if ((ret = recvfrom(s, buf, 8192, 0, (PSOCKADDR)&client, &sinLen))
			== SOCKET_ERROR)
		{
			::MessageBox(0,"UDP recvfrom failed.", "Alert", MB_OK);
			break;
		}

		EM_DATA data(buf, ret);

		if (data.msg == EM_USERLOGIN || data.msg == EM_USERLOGOUT)
		{
			strcpy(ip, inet_ntoa(client.sin_addr));
			char name[256];
			char hostName[256];
			char groupName[128];
			EM_UserInfo *ui = ((EM_UserInfo*)data.buf);
			ui->GetDisplayName(name);
			ui->GetHostName(hostName);
			ui->GetGroupName(groupName);
			// Add to tree ---------------------------------------------------
			EM_USERINFO uinfo(strupr(hostName), ip, groupName);
			pDlg->_User_AddNewUser(name, uinfo);
			// ===============================================================
		}
	}
	return 0;
}

void EM_UserLogin::OnLine()
{
	FreeEIM_User_Logic fLogic;
	EM_DATA data;

	data.msg = EM_USERLOGIN;	// 登陆
	data.buf = (char*)fLogic.GetUserInfo();
	data.len = sizeof(EM_UserInfo);
	SendLogin_BC(& data);
}

void EM_UserLogin::OffLine()
{
	FreeEIM_User_Logic fLogic;
	EM_DATA data;

	data.msg = EM_USERLOGOUT;	// 退出
	data.buf = (char*)fLogic.GetUserInfo();
	data.len = sizeof(EM_UserInfo);
	SendLogin_BC(&data);
}

void EM_UserLogin::ReName(LPCTSTR lpszName)
{
	EM_DATA data;
	data.msg = EM_USERRENAME;	// 改名
	data.buf = (LPSTR)lpszName;
	data.len = strlen(lpszName) + 1;

	SendLogin_BC(&data);
}


void EM_UserLogin::ReGroup(LPCTSTR lpszGroup)
{
	EM_DATA data;
	data.msg = EM_USERREGROUP;	// 改名
	data.buf = (LPSTR)lpszGroup;
	data.len = strlen(lpszGroup) + 1;

	SendLogin_BC(&data);
}