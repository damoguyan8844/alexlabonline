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

// �����û���Ϣ
// static EM_UserInfo g_uiLocal;

class EM_UserLogin
{
private:
	// �㲥��Ϣ���մ�����
	static DWORD WINAPI Proc_BC_Recv(LPVOID lParam);
	// UDP��Ϣ���մ�����
	static DWORD WINAPI Proc_UDP_Recv(LPVOID lParam);

public:
	// ����UDP��½��Ϣ��ָ����IP��ַ
	void SendLogin_UDP(LPCTSTR lpszIP);
	// ���͹㲥��½��Ϣ
	void SendLogin_BC(LPEM_DATA msg);

public:
	void OffLine();
	void OnLine();
	void ReName(LPCTSTR lpszName);
	void ReGroup(LPCTSTR lpszGroup);

	// ���캯��
	EM_UserLogin();

	// ��������
	~EM_UserLogin();

	// ִ�к���
	void Run(EM_UserTree &hTree);

	// �ر������߳�
	void Close();

protected:

	// UDP�����߳̾��
	HANDLE m_hUDP;

	// BC�����߳̾��
	HANDLE m_hBC;
};

#endif // !defined(AFX_EM_USERLOGIN_H__D72DDE04_00AF_491C_90A7_190133794F41__INCLUDED_)
