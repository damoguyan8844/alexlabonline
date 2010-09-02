#ifndef _FreeEIM_History
#define _FreeEIM_History

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
ȫ�µ� FreeEIM ������ ACCESS ��Ϊ���ݿ⡣���Խ������������ʷ�ļ������⡣
*/

class CEIM02Dlg;

class XEIM_Database;

class FreeEIM_History
{
public:
	// ���ܣ�������ʷ��¼
	// �������ڣ�2009��12��9��
	void SaveHistory(LPCTSTR lpszSend, LPCTSTR lpszUser, LPCTSTR lszData);
	FreeEIM_History();
	~FreeEIM_History();
private:

	// ���ܣ������û���ʷ�ļ�
	// �������ڣ�2009��12��9��
	void CreateUserPath(LPCTSTR lpszUser);

	// ���ܣ���ȡָ���û��������¼����·��
	// �������ڣ�2009��12��9��
	CString GetHistoryPath();

	// ���ܣ��ж��ļ��Ƿ����
	// �������ڣ�2009��12��9��
	BOOL UserPathExist(LPCTSTR lpszPath);

	CEIM02Dlg *GetMainTop();
	XEIM_Database *m_pDB;

	// ���ܣ�������ʷ�ļ�����·��
	// �������ڣ�2009��12��9��
	void CreateHistoryPath();

	// ���ܣ�����һ��Ŀ¼
	// �������ڣ�2009��12��9��
	// ���ԣ�http://www.codeproject.com/KB/files/Folder_Utility.aspx
	void CreateDir(char* Path);

protected:
};

#endif // _FreeEIM_History