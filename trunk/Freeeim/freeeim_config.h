// freeeim_config.h : header file
// �ɸ봫�� FreeEIM ������
// �������ڣ�2009��12��3��
// ���ߣ�������(freeeim@gmail.com)
// �ɸ봫��ٷ���վ��http://www.freeeim.com/
// ���ܣ���ȡ���������ļ�

#ifndef _FreeEIM_Config
#define _FreeEIM_Config

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ������Ϣ
#define FREEEIM_NONE		0x00
#define FREEEIM_TEXT		0x01

class CEIM02Dlg;
class EM_UserInfo;

class FreeEIM_Config
{

	// ���ܣ���ȡ�����ļ�
	// �������ڣ�2009��12��5��
	void ReadConfig();

public:
	FreeEIM_Config();
	~FreeEIM_Config();

	CEIM02Dlg *m_pMainTop;
	CString m_strDisplayName;
	CString m_strGroup;

	// ���ܣ���ȡ���ؼ������
	// �������ڣ�2009��12��5��
	CString GetComputerName();


	// ���ܣ����������ļ�
	// �������ڣ�2009��12��5��
	void SaveConfig();


	// ���ܣ���ȡ�û���
	// �������ڣ�2009��12��5��
	CString GetDisplayName();


	// ���ܣ���ȡ�û�����
	// �������ڣ�2009��12��5��
	CString GetGroup();


	// �û���Ϣ
	EM_UserInfo *m_pUserInfo;
	// ���ܣ�����EM_UserInfo
	// �������ڣ�2009��12��5��
	/*
	Ϊ����ɰ汾���ݣ�ֻ�з����·����ˣ����ݲ�������Ҫ�ġ�
	*/
	EM_UserInfo *GetUserInfo();

private:
protected:
};

#endif // _FreeEIM_Config