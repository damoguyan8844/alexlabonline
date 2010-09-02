
#ifndef _XEIM_MESSAGE
#define _XEIM_MESSAGE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iocpui01Dlg.h"
#include "xeim_kernel.h"

// 清空文件指针
#define XEIM_DELETE_CFILE(handle) {if(handle)delete (CFile*)handle;handle = NULL;}

class XEIM_MESSAGE
{
public:
	// 重命名一个文件，如果新名字存在一个文件，就把它删了；
	static void XEIM_Rename(LPCTSTR pstrExisting, LPCTSTR pstrNew);
	CFile *pF;
	// 设置返回的缓存区。
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

	// 消息处理函数。
	void OnXEIM_None(char* buf, DWORD len);
	void OnXEIM_Echo(char* buf, DWORD len);

	// 登录消息
	void OnXEIM_Login(char *buf, DWORD len);
	// 文件夹消息
	void OnXEIM_Folder(char *buf, DWORD len);
	// 文件消息
	void OnXEIM_File(char *buf, DWORD len);
	// 文件数据消息
	void OnXEIM_File_Data(char *buf, DWORD len);
	// 文件最后一个数据包
	void OnXEIM_File_Last();
	// 未知的消息
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

	// 保存文件信息，用于续传
	inline void SaveFileStatus(CFileStatus &status, char *fileName);
	// 判断该文件信息是否已存在，有则进行续传
	inline BOOL IsFileResume(CFileStatus &status, char *fileName);
};

#endif // _XEIM_MESSAGE