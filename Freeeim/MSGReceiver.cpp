#include "StdAfx.h"
#include "EIM02Dlg.h"
#include "MSGReceiver.h"

BOOL EM_MSGReceiver::m_bMessageReceiving = TRUE;

EM_MSGReceiver::EM_MSGReceiver()
{
	m_hWnd = NULL;

	m_hThread = NULL;
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock)
	{
		AfxMessageBox("EMMSGSender socket creating error.");
		return;
	}
}

EM_MSGReceiver::~EM_MSGReceiver()
{
	if (m_hThread != NULL)
		CloseHandle(m_hThread);
	if (INVALID_SOCKET != m_sock)
		closesocket(m_sock);
}

DWORD WINAPI EM_MSGReceiver::UDPMsgProc(LPVOID lParam)
{
	CEIM02Dlg *pDlg = (CEIM02Dlg*)AfxGetMainWnd();
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s == INVALID_SOCKET)
	{
		::EM_Error(_T("Socket creating error."), WSAGetLastError());
		return -1;
	}

	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(MSG_RECV_PORT);

	if (bind(s, (PSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		::EM_Error(_T("Socket binding error."), WSAGetLastError());
	}

	SOCKADDR_IN client;
	int sinLen;
	int ret;
	char buf[8192];

	while( m_bMessageReceiving )
	{
		sinLen = sizeof(SOCKADDR_IN);
		if ((ret = recvfrom(s, buf, 8192, 0, (PSOCKADDR)&client, &sinLen)) == SOCKET_ERROR)
		{
			::EM_Error(_T("recvfrom error occured."), WSAGetLastError());
			break;
		}
		else
		{
			// 看不顺眼啊
			EM_DATA data(buf, sinLen);

			char ip[128];
			strcpy(ip, inet_ntoa(client.sin_addr));

			// GetHostName
		//	char hostName[256];
		//	EM_GetHostName(ip, hostName);

		//	EM_USERINFO *emUI = new EM_USERINFO(hostName, ip, NULL);
			// 不同消息执行不同处理
			switch ( data.msg )
			{
				case EM_TEXT:				// 用户发送文本消息过来
				{
					char *pBuf = new char[data.len];
					memcpy(pBuf, data.buf, data.len);
					pDlg->SendMessage(WM_NEW_MSG,
						(WPARAM)pBuf, (LPARAM)ip);
					break;
				}
				case EM_FILE:				// 用户发送文件过来
				{
					LPEM_FILEINFO pInfo = new EM_FILEINFO;
					memcpy(pInfo, data.buf, sizeof(EM_FILEINFO));

					pDlg->PostMessage(WM_NEW_FILE, 
						(WPARAM)pInfo, (LPARAM)ip);
					break;
				}
				case EM_REFUSEFILE:			// 拒绝接收
				{
					LPEM_FILEINFO pInfo = new EM_FILEINFO;
					memcpy(pInfo, data.buf, sizeof(EM_FILEINFO));

					pDlg->PostMessage(WM_REFUSEFILE, 
						(WPARAM)pInfo, (LPARAM)ip);
					break;
				}
				case EM_ACCEPTFILE:			// 同意接收文件
				{
					LPEM_FILEINFO pInfo = new EM_FILEINFO;
					memcpy(pInfo, data.buf, sizeof(EM_FILEINFO));
					
					pDlg->PostMessage(WM_DESTACCEPTFILE,
						(WPARAM)pInfo, (LPARAM)ip);
					break;
				}
			/*	case EM_FILESENDCOMPLETED:	// 发送完毕
				{
					LPEM_FILEINFO pInfo = new EM_FILEINFO;
					memcpy(pInfo, data.buf, sizeof(EM_FILEINFO));

					pDlg->PostMessage(WM_FILESENDCOMPLETED,
						(WPARAM)pInfo, (LPARAM)emUI);
					break;
				}*/
				case EM_REQUESTVOICE:
				{
					pDlg->PostMessage(WM_REQUESTVOICE, NULL, (LPARAM)ip);
					break;
				}
				case EM_CANCELREQUESTVOICE:
				{
					pDlg->PostMessage(WM_CANCELREQUESTVOICE, data.len, (LPARAM)ip);
					break;
				}
				case EM_DONOTCHAT:
				{
					pDlg->PostMessage(WM_DONOTACCEPTCHAT, data.len, (LPARAM)ip);
					break;
				}
				case EM_ACCEPTCHAT:
				{
					pDlg->PostMessage(WM_ACCEPTCHAT, NULL, (LPARAM)ip);
					break;
				}
				case EM_SHAKEWINDOW:
				{
					pDlg->PostMessage(WM_SHAKEWINDOW, NULL, (LPARAM)ip);
					break;
				}
			}
		}
	}
	return 0;//没素质
}

void EM_MSGReceiver::Run(const HWND hWnd)
{
	m_hWnd = hWnd;
	if (hWnd == NULL)
	{
		::EM_Error(_T("hWnd could not be NULL"), 0);
		return;
	}

	m_hThread = CreateThread(NULL, 0, UDPMsgProc, hWnd, 0, &m_tid);

	if (m_hThread == NULL)
	{
		::EM_Error(_T("Thread creating failed."), 0);
	}
}

void EM_MSGReceiver::Close()
{
	m_bMessageReceiving = FALSE;
	// Tell the thread to terminate.
/*	if (!::TerminateThread(m_hThread, 0))
	{
		::EM_Error(_T("TerminateThread failed."), GetLastError());
		return FALSE;
	}
*/
//	return ::CloseHandle(m_hThread);
}

void EM_MSGReceiver::SendMsg(char *szIP, LPEM_DATA lpMsg)
{
//	char szip[128];
//	strcpy(szip, emUI->m_szIP);
	SOCKADDR_IN sin;
	if(lpMsg == NULL)
	{
		AfxMessageBox(_T("EMMSGSender::SendMsgTo Message is empty"));
		return;
	}
	sin.sin_addr.s_addr = inet_addr(szIP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(MSG_SEND_PORT);

	char *tmpBuf = lpMsg->GetBuffer();
	if (sendto(m_sock, tmpBuf, lpMsg->GetLength(), 0, (PSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("EMMSGSender::SendMsgTo sendto failed"));
		return;
	}
}