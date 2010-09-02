
#ifndef _XEIM_MESSAGE
#define _XEIM_MESSAGE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iocpui01Dlg.h"
#include "xeim_kernel.h"

// ����ļ�ָ��
#define XEIM_DELETE_CFILE(handle) {if(handle)delete (CFile*)handle;handle = NULL;}

class XEIM_MESSAGE
{
public:
	// ������һ���ļ�����������ִ���һ���ļ����Ͱ���ɾ�ˣ�
	static void XEIM_Rename(LPCTSTR pstrExisting, LPCTSTR pstrNew);
	CFile *pF;
	// ���÷��صĻ�������
	inline void SetBuffer(DWORD msg, char *buf, DWORD len);
	char buffer[MAX_PACKAGE_SIZE];
	DWORD length;
	DWORD message;

	SOCKET m_sock;
	XEIM_MESSAGE(XEIM_OPERATION *PerIoData, XEIM_HANDLE *PerHandleData);
	XEIM_MESSAGE(const SOCKET sock, const char *buf, const DWORD len, const LPVOID handle);
	~XEIM_MESSAGE();
private:

	LPVOID m_pHandle;
	void ProcessMessage();

	// ��Ϣ��������
	void OnXEIM_None(char* buf, DWORD len);
	void OnXEIM_Echo(char* buf, DWORD len);

	// ��¼��Ϣ
	void OnXEIM_Login(char *buf, DWORD len);
	// �ļ�����Ϣ
	void OnXEIM_Folder(char *buf, DWORD len);
	// �ļ���Ϣ
	void OnXEIM_File(char *buf, DWORD len);
	// �ļ�������Ϣ
	void OnXEIM_File_Data(char *buf, DWORD len);
	// �ļ����һ�����ݰ�
	void OnXEIM_File_Last();
	// δ֪����Ϣ
	void OnXEIM_Unknown();




	//////////////////////////////////////////////////////////////////////////
	inline BOOL IsNewSocket();

	//////////////////////////////////////////////////////////////////////////
	inline void GetDataByTag(IN char *pSrcData,
				  IN int nSrcLen,
				  IN char cSeparator,
				  IN int nIndex,
				  OUT char *pDstData,
				  OUT int &nDstLen,
				  int nMaxSize);

	inline void GetFileName(IN char* fullPathName,
				OUT char *fileName);

	inline void GetFileExt(IN char* fileName,
				OUT char *fileExt);

	// �����ļ���Ϣ����������
	inline void SaveFileStatus(CFileStatus &status, char *fileName);
	// �жϸ��ļ���Ϣ�Ƿ��Ѵ��ڣ������������
	inline BOOL IsFileResume(CFileStatus &status, char *fileName);
};

#endif // _XEIM_MESSAGE