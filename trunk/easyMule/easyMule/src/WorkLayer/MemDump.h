#pragma once

class CMemDump
{
public:
	CMemDump(void);
	~CMemDump(void);

	void	SetOutputFile(LPCTSTR lpszPathFile);	//	�����������־�ļ���NULL��ʾ�����Ĭ���豸��
	void	Dump();									//	����ڴ�й©��Ϣ

	void	DumpIntoDifferentFile();				//	�Զ�����ڴ�й©��Ϣ����ͬ���ļ���
protected:
	CString	m_strPathFile;
};

extern CMemDump theMemDump;