// freeeim_user_logic.h : header file
// �ɸ봫�� FreeEIM ������
// �������ڣ�2009��12��3��
// ���ߣ�������(freeeim@gmail.com)
// �ɸ봫��ٷ���վ��http://www.freeeim.com/
// ���ܣ������û���ͨѶ��������¼����Ϣ�շ���


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

	// ���ܣ��㲥�û�״̬����¼�������ߺ���
	// �������ڣ�2009��12��5��
	void SendStatus(int Status);


	// ���ܣ����͵�����Ϣ�����������û���
	// �������ڣ�2009��12��5��
	void PostMessageToGroup(HTREEITEM hItem, char *szMessage);


	// ���ܣ����͵�����Ϣ���û�
	// �������ڣ�2009��12��5��
	void PostMessageToUser(const char *szUser, char *szMessage);

	// ���ܣ����ر����û���Ϣ
	// �������ڣ�2009��12��5��
	EM_UserInfo *GetUserInfo();

protected:
private:
};

#endif // _FreeEIM_User_Logic