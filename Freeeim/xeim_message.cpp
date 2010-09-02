#include "StdAfx.h"
#include "../protocol/xeim_protocol.h"
#include "xeim_logic.h"
#include "xeim_message.h"
#include "iocpui01Dlg.h"

XEIM_MESSAGE::XEIM_MESSAGE(const SOCKET sock, const char *buf, const DWORD len, const LPVOID handle)
{
	m_pMainTop = (CIocpui01Dlg*)AfxGetMainWnd();
	m_sock = sock;
	memcpy((char*)&message, buf, sizeof(DWORD));
	memcpy(buffer, buf+sizeof(DWORD), len-sizeof(DWORD));
	length = len - sizeof(DWORD);
	m_pHandle = handle;

	ProcessMessage();
}

XEIM_MESSAGE::XEIM_MESSAGE(XEIM_OPERATION *PerIoData, XEIM_HANDLE *PerHandleData)
{
	m_pMainTop = (CIocpui01Dlg*)AfxGetMainWnd();
	m_sock = PerHandleData->Socket;

	memcpy((char*)&message, PerIoData->buf, sizeof(DWORD));
	length = PerIoData->Offset - sizeof(DWORD);
	memcpy(buffer, PerIoData->buf+sizeof(DWORD), length);
	m_pHandle = PerIoData;

	ProcessMessage();
}

XEIM_MESSAGE::~XEIM_MESSAGE()
{
}


void XEIM_MESSAGE::ProcessMessage()
{
/*	CString str;
	str.Format("%d", length);
	AfxMessageBox(str);*/

	switch (message)
	{
	case XEIM_NONE:
		OnXEIM_None(buffer, length);
		break;

	case XEIM_ECHO:
		OnXEIM_Echo(buffer, length);
		break;

	case XEIM_LOGIN:
		OnXEIM_Login(buffer, length);
		break;

	case XEIM_FOLDER:
		OnXEIM_Folder(buffer, length);
		break;

	case XEIM_FILE:
		OnXEIM_File(buffer, length);
		break;

	case XEIM_FILE_DATA:
		OnXEIM_File_Data(buffer, length);
		break;
	case XEIM_FILE_DATA_LAST:
		OnXEIM_File_Last();
		break;
	default:
		OnXEIM_Unknown();
	}
}



void XEIM_MESSAGE::OnXEIM_None(char *buf, DWORD len)
{

}

void XEIM_MESSAGE::OnXEIM_Echo(char *buf, DWORD len)
{

}

// 用户登录消息处理
void XEIM_MESSAGE::OnXEIM_Login(char *buf, DWORD len)
{
	if (! IsNewSocket())
	{
	}
	else
	{
		char szUser[128];
		char szPassword[128];
		char szVersion[16];
		char szStatus[8];

		int nLen;
		GetDataByTag(buf, len, ',', 1, szUser, nLen, 128);
		GetDataByTag(buf, len, ',', 2, szPassword, nLen, 128);
		GetDataByTag(buf, len, ',', 3, szVersion, nLen, 16);
		GetDataByTag(buf, len, ',', 4, szStatus, nLen, 8);

		XEIM_LOGIC xlogic;
		if (xlogic.UserValid(szUser, szPassword))
		{
			LVITEM lvi;
			int nIdx;

			lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
			lvi.iItem = 0xff;
			lvi.iSubItem = 0;
			lvi.pszText = szUser;
			lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
			nIdx = m_pMainTop->m_reportClient.InsertItem(& lvi);
			m_pMainTop->m_reportClient.SetItemData(nIdx, m_sock);

			sockaddr_in aa;
			int slen = sizeof(aa);
			int rval = 0;
			if (getpeername(m_sock, (sockaddr*)&aa, &slen) == SOCKET_ERROR)
			{
				m_pMainTop->m_reportClient.SetItemText(nIdx, 1, "0.0.0.0");
			}
			else
			{
				m_pMainTop->m_reportClient.SetItemText(nIdx, 1, inet_ntoa(aa.sin_addr));
			}

			//////////////////////////////////////////////////////////////////////////
			SetBuffer(XEIM_LOGIN, "OK", 3);
		}
		else
		{
			SetBuffer(XEIM_LOGIN, "NO", 3);
		}
	}
}

// 文件夹消息
void XEIM_MESSAGE::OnXEIM_Folder(char *buf, DWORD len)
{
}

// 文件消息

/*
CString stra;
stra.Format("创建时间：%s\r\n"
	"修改时间：%s\r\n"
	"访问时间：%s\r\n"
	"大小：%f\r\n"
	"文件名称：%s\r\n",
	status.m_ctime.Format("%Y-%m-%d %H:%M:%S"),
	status.m_mtime.Format("%Y-%m-%d %H:%M-%S"),
	status.m_atime.Format("%Y-%m-%d %H:%M-%S"),
	(float)status.m_size,
	status.m_szFullName);
AfxMessageBox(stra);
*/

void XEIM_MESSAGE::OnXEIM_File(char *buf, DWORD len)
{
	CFileStatus status = * (CFileStatus*)buf;

	char fileName[MAX_PATH];
	XEIM_LOGIC xLogic;
	xLogic.SetSaveFileName(status.m_szFullName, "aaaa", fileName);
	CString strXtFileName = fileName;
	strXtFileName += ".xeim";
	CFile *pFile = new CFile;

	((XEIM_OPERATION*)m_pHandle)->handle = pFile;

	if (IsFileResume(status, fileName))
	{
		BOOL bOK = pFile->Open(strXtFileName, CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);
		if (bOK)
		{
			DWORD fSize = pFile->GetLength();
			SetBuffer(XEIM_FILE_RESUME, (char*)&fSize, sizeof(DWORD));
			pFile->Seek(0, CFile::end);
		}
		else // Some error.
		{
		//	SetBuffer(XEIM_FILE_RESUME, 0, sizeof(DWORD));
			SetBuffer(XEIM_FILE, "NO", 3);
		}
	}
	else
	{
		// 保存文件信息
		SaveFileStatus(status, fileName);

		// 文件信息不同的话，即使存在旧文件，也要清 0 再重新写，用 CFile::modeCreate。
		BOOL bOK = pFile->Open(strXtFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);

		if (bOK)
		{
			SetBuffer(XEIM_FILE, "OK", 3);
		}
		else
		{
			SetBuffer(XEIM_FILE, "NO", 3);
		}
	}
}

// 文件数据消息
void XEIM_MESSAGE::OnXEIM_File_Data(char *buf, DWORD len)
{
//	AfxMessageBox("OnXEIM_File_Data");
	CFile *pFile = (CFile*)(((XEIM_OPERATION*)m_pHandle)->handle);
	pFile->Write(buf, len);
	SetBuffer(XEIM_FILE_DATA, "OK", 3);
}

// 文件最后一个数据包
void XEIM_MESSAGE::OnXEIM_File_Last()
{
	CString strFileName;
	CString strOldFileName;
	CFile *pFile = (CFile*)(((XEIM_OPERATION*)m_pHandle)->handle);
	pFile->Write(buffer, length);

	SetBuffer(XEIM_FILE_DATA_LAST, "OK", 3);
//	AfxMessageBox("OnXEIM_File_Last");
	strOldFileName = pFile->GetFilePath();
	strFileName = strOldFileName;
	strFileName.Replace(".xeim", "");
	pFile->Flush();
	pFile->Close();

	XEIM_Rename(strOldFileName, strFileName);

//	CFile::Rename(strOldFileName, strFileName);
//	AfxMessageBox(strFileName + ".info");
	CFile::Remove(strFileName + ".info");

	XEIM_DELETE_CFILE(((XEIM_OPERATION*)m_pHandle)->handle);
/*	delete pFile;
	((XEIM_OPERATION*)m_pHandle)->handle = NULL;*/
}

// 未知的消息
void XEIM_MESSAGE::OnXEIM_Unknown()
{
	SetBuffer(message, "UNKNOWN", strlen("UNKNOWN"));
}

BOOL XEIM_MESSAGE::IsNewSocket()
{
	int nItemCount = m_pMainTop->m_reportClient.GetItemCount();

	for (int i=0; i<nItemCount; i++)
	{
		SOCKET sock = m_pMainTop->m_reportClient.GetItemData(i);
		if (sock == m_sock)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// 设置返回的缓存区。
void XEIM_MESSAGE::SetBuffer(DWORD msg, char *buf, DWORD len)
{
	length = len+4;
	memcpy(buffer, (char*)&msg, sizeof(DWORD));
	memcpy(buffer+sizeof(DWORD), buf, len);
}



//////////////////////////////////////////////////////////////////////////

// 一些协助函数都放到最后

// XEIM 数据流处理函数，通过分隔符 Separator 来获取每个段的数据。
// 编写日期：21:44 2009-8-18
// FEIM 小组：飞鸽传书 www.freeeim.com

// 使用说明：
/* pSrcData 为输入数据流；
   nSrcLen 为数据流的长度；
   cSeparator 为分隔符；
   nIndex 为第几个分隔符内的数据；
   pDstData 分隔符数据保存到此缓冲区；
   nDstLen	该分隔符内数据长度；
   nMaxSize 输出缓存区大小，假如该分割符内的数据是4字节，你给3字节函数就失败，*pDstLen = NULL。
*/
/* 使用例子：
	char * src = "aa,bbbbbbbb,cc,dd";
	int len = strlen(src) + 1;

	char * dst = new char[1024];
	memset(dst, 0, 1024);
	int dstLen;

	GetDataByTag(src, len, ',', 4, dst, dstLen, 1024);

	printf("%s -- %d", dst, dstLen);
*/
//  输出结果：dd -- 2

void XEIM_MESSAGE::GetDataByTag(IN char *pSrcData,
				  IN int nSrcLen,
				  IN char cSeparator,
				  IN int nIndex,
				  OUT char *pDstData,
				  OUT int &nDstLen,
				  int nMaxSize)
{
	int i_1st = -1;
	int i_2nd = 0;

	int idx =0;
	for (; i_2nd<nSrcLen&&idx<nIndex;i_2nd++)
	{
		if (*(pSrcData+i_2nd) == cSeparator)
		{
			if (nIndex-2 == idx)
				i_1st = i_2nd;
			idx ++;
		}
	}

	if (i_1st == -1 && i_2nd == nSrcLen
		|| (i_2nd-i_1st-2) >= nMaxSize)
	{
		*pDstData = NULL;
		nDstLen = 0;
		return;
	}

	i_1st ++;
	i_2nd --;
	for (idx=0; i_1st+idx<i_2nd; idx++)
		*(pDstData+idx) = *(pSrcData+i_1st+idx);

	*(pDstData+idx) = NULL;
	nDstLen = idx;
}



// 写时冲冲，未留注释，获取文件名称
/*
	CFile file;
	BOOL bResult = file.Open("c:\\windows\\notepad.exe", CFile::modeRead|CFile::shareDenyNone, NULL);
	CFileStatus status;

	file.GetStatus(status);
	printf("创建时间：%s\r\n", status.m_ctime.Format("%Y-%m-%d %H:%M:%S"));
	printf("修改时间：%s\r\n", status.m_mtime.Format("%Y-%m-%d %H:%M-%S"));
	printf("访问时间：%s\r\n", status.m_atime.Format("%Y-%m-%d %H:%M-%S"));
	printf("大小：%f\r\n", (float)status.m_size);
	printf("文件名称：%s\r\n", status.m_szFullName);
	printf("CFileStatus:%d\r\n", sizeof(CFileStatus));

	char fileName[MAX_PATH];
	GetFileName(status.m_szFullName, fileName);
	printf("%s\r\n", fileName);
	GetFileExt(status.m_szFullName, fileName);
	printf("%s\r\n", fileName);
*/
void XEIM_MESSAGE::GetFileName(IN char* fullPathName, OUT char *fileName)
{
	int idx=0;
	int kk;

	while(fullPathName[idx] != NULL)
	{
		if (fullPathName[idx] == '\\')
			kk = idx;

		idx ++;
	}
	kk ++;

	for (int i=0;fullPathName[kk+i];i++)
	{
		fileName[i] = fullPathName[kk+i];
	}
	fileName[i] = NULL;

//	printf("%d - %s\r\n", kk, fileName);
}

// 写时冲冲，未留注释，获取文件扩展名
/*
	CFile file;
	BOOL bResult = file.Open("c:\\windows\\notepad.exe", CFile::modeRead|CFile::shareDenyNone, NULL);
	CFileStatus status;

	file.GetStatus(status);
	printf("创建时间：%s\r\n", status.m_ctime.Format("%Y-%m-%d %H:%M:%S"));
	printf("修改时间：%s\r\n", status.m_mtime.Format("%Y-%m-%d %H:%M-%S"));
	printf("访问时间：%s\r\n", status.m_atime.Format("%Y-%m-%d %H:%M-%S"));
	printf("大小：%f\r\n", (float)status.m_size);
	printf("文件名称：%s\r\n", status.m_szFullName);
	printf("CFileStatus:%d\r\n", sizeof(CFileStatus));

	char fileName[MAX_PATH];
	GetFileName(status.m_szFullName, fileName);
	printf("%s\r\n", fileName);
	GetFileExt(status.m_szFullName, fileName);
	printf("%s\r\n", fileName);
*/
void XEIM_MESSAGE::GetFileExt(IN char* fileName, OUT char *fileExt)
{
	int idx = 0;
	int kk;
	while(fileName[idx] != NULL)
	{
		if (fileName[idx] == '.')
			kk = idx;

		idx ++;
	}
	kk ++;
	for (int i=0;fileName[kk+i];i++)
	{
		fileExt[i] = fileName[kk+i];
	}
	fileExt[i] = NULL;

//	printf("%d - %s\r\n", kk, fileExt);
}

// 保存文件信息，用于续传
void XEIM_MESSAGE::SaveFileStatus(CFileStatus &status, char *fileName)
{
	CString strFileName = fileName;

	strFileName += ".info";

	TRY{
		CFile file;
		CFileException except;
		BOOL bResult = file.Open(strFileName, CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone, &except);
		if (bResult)
		{
			file.Write((char*)&status, sizeof(CFileStatus));
			file.Flush();
			file.Close();
		}
		int err = except.m_cause;
	}
	CATCH(CFileException ,e){
		if (e->m_cause == CFileException::fileNotFound)
		AfxMessageBox("Not found.");
	}
	END_CATCH
}

// 判断该文件信息是否已存在，有则进行续传
BOOL XEIM_MESSAGE::IsFileResume(CFileStatus &status, char *fileName)
{
	CString strFileName = fileName;
	strFileName += ".info";
	CFile file;
	BOOL bResult = file.Open(strFileName, CFile::modeRead|CFile::shareDenyNone, NULL);
	if (! bResult)
		return FALSE;

	CFileStatus l_status;
	file.Read((char*)&l_status, sizeof(CFileStatus));
	file.Close();

	// 访问时间当然每次都变啦。
	l_status.m_atime = status.m_atime;

	return (0 == memcmp((char*)&l_status, (char*)&status, sizeof(CFileStatus)));
}

// 重命名一个文件，如果新名字存在一个文件，就把它删了；
void XEIM_MESSAGE::XEIM_Rename(LPCTSTR pstrExisting, LPCTSTR pstrNew)
{
	OFSTRUCT ofStruct;
	ofStruct.cBytes = sizeof(OFSTRUCT);
	ofStruct.fFixedDisk = 1;

	if (HFILE_ERROR != ::OpenFile(pstrNew, &ofStruct, OF_EXIST))
		CFile::Remove(pstrNew);

	CFile::Rename(pstrExisting, pstrNew);
}
