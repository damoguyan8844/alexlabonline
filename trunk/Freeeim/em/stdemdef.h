// Author:Alex Thai
// Coding Date: 2007-4-18
// This header file is deisgned for Enterprise Instance Messenger.

// 1 Message Definition ----------------------------------------------------------------------------
// 2 EM_Data Class Definition ----------------------------------------------------------------------
// 3 Ports for socket Definition -------------------------------------------------------------------
// 4 Custom Windows Message Definition -------------------------------------------------------------
// 5 Function Definition ---------------------------------------------------------------------------
// 6 Data Structure definition ---------------------------------------------------------------------
// 7 Macro Definition ------------------------------------------------------------------------------
// �ɸ봫��Դ��
#ifndef STDEMDEF_H
#define STDEMDEF_H

#include <windows.h>
// 1 Message define ----------------------------------------------------------------------------
#ifndef EM_MSG_DEFINE
#define EM_MSG_DEFINE

#define EM_TEXT					0x00000001	// Sending text
#define EM_FILE					0x00000002	// Sending file
#define EM_USERLOGIN			0x00000003	// User login
#define EM_USERLOGOUT			0x00000004	// User logout
#define EM_GETFILENAME			0x00000005	// Geting the file name
#define EM_SENDFILENAME			0x00000006	// Sending the file name
#define EM_GETFILEDATA			0x00000007	// Geting the file data
#define EM_SENDFILEINFO			0x00000008  // �ļ���Ϣ
#define EM_USERRENAME			0x00000009	// �û�������ʾ����
#define	EM_REFUSEFILE			0x00000010	// �ܾ������ļ�
#define	EM_FILERECVCOMPLETED	0x00000011	// �ļ���ȡ���
#define	EM_FILESENDCOMPLETED	0x00000012	// �ļ��������
#define EM_ACCEPTFILE			0x00000013	// ͬ������ļ�
#define	EM_REQUESTVOICE			0x00000015	// �������������Ի�
#define	EM_CANCELREQUESTVOICE	0x00000016	// ��ֹͣ�������Ի�������
#define EM_DONOTCHAT			0x00000017	// ��ܾ����������Ի�����
#define EM_ACCEPTCHAT			0x00000018	// ��ͬ����������Ի�����
#define	EM_CHATTINGEND			0x00000019	// �����Ի�����
#define	EM_SHAKEWINDOW			0x00000020	// ���ڶ�����Ϣ
#define EM_USERREGROUP			0x00000021	// �޸��û�����
// Another message will be defined later.
#endif // EM_MSG_DEFINE
// 1 Message define ----------------------------------------------------------------------------


// 2 EM_Data class define ----------------------------------------------------------------------
class EM_FILEINFO;
typedef EM_FILEINFO * LPEM_FILEINFO;

#ifndef EM_MSG_DATA
#define EM_MSG_DATA

class EM_DATA
{
private:
	char *tmpBuf;
public:

	EM_DATA(char *data, DWORD dwlen);
	EM_DATA();
	~EM_DATA();
	DWORD msg;		// Data that defined as EM_XXX
	DWORD len;		// The length of buf
	char * buf;		// A pointer of buffer

	// Returning buffer.
	char *GetBuffer();
	// Return the length.
	DWORD GetLength();
	BOOL AddFileMsg(LPCTSTR lpszFileName, LPCTSTR lpszFileExt);
};
typedef EM_DATA * LPEM_DATA;

#endif // EM_MSG_DATA

// ��Դ��ʱͨѶ��� �ɸ봫�� FreeEIM
class EM_USERINFO
{
public:
	// �û���Ϣ���ݰ���ʼ��
	EM_USERINFO::EM_USERINFO(char *szhost, char *szip, char *szGroup)
	{
		ASSERT(NULL != szhost);
		ASSERT(NULL != szip);
		ASSERT(NULL != szGroup);

		strcpy(m_szHost, szhost);
		strcpy(m_szIP, szip);
		strcpy(m_szGroup, szGroup);
	}

	char m_szHost[256];
	char m_szIP[128];
	char m_szGroup[128];
};


// �û���Ϣ��
typedef struct
{
public:
	char m_szName[256];
	char m_szHost[256];
	char m_szIP[64];
	char m_szGroup[128];
} FreeEIM_USER;




#ifndef EM_CHATTINGHISTORY
#define EM_CHATTINGHISTORY
class EM_ChattingData
{
	// ���ݳ���
	int m_nDataLen;
	// ���ڳ���
	int m_nDateLen;
	// ���ݻ�����
	char *m_bData;
	// ���ڻ�����
	char *m_bDate;
	// �������صĻ�����
	char *m_buf;

public:
	// ����ƫ��������������
	static int s_nOffset;

	// �ܳ���Ϊ 2 ���������ȼ����ݳ���
	inline int GetLength()
	{
		int nLen = 0;
		nLen += sizeof(int); // �������ݳ��ȵı�������
		nLen += sizeof(int); // �������ڳ��ȵı�������
		nLen += m_nDataLen; // ���ݳ���
		nLen += m_nDateLen; // ���ڳ���
		// �������ݰ��ܳ���
		return nLen;
	}


	void GetDate(char **date, int *len)
	{
		ASSERT(m_buf != NULL);

		*len = m_nDateLen;
		*date = new char[*len];

		// [int:date] [int:data] [date] 
		s_nOffset = sizeof(int)*2;

		memcpy(*date, m_buf+s_nOffset, *len);
	}

	void GetData(char **data, int *len)
	{
		ASSERT(m_buf != NULL);

		*len = m_nDataLen;
		*data = new char[*len];

		// [int:date] [int:data] [date] [data]
		CString str;
		str.Format("%d", m_nDateLen);
		MessageBox(0,str,str,0);
		s_nOffset = sizeof(int)*2+m_nDateLen;
		memcpy(*data, m_buf+s_nOffset, *len);
	}

	void SetData(char *data, int len)
	{
		ASSERT(NULL != data);
		ASSERT(0 < len);

		m_bData = new char[len];
		memcpy(m_bData,data,len);
		m_nDataLen = len;
	}

	void SetDate(char *date, int len)
	{
		ASSERT(NULL != date);
		ASSERT(0 < len);
		
		m_bDate = new char[len];
		memcpy(m_bDate,date,len);
		m_nDateLen = len;
	}

	char *GetBuffer()  // ʹ�ô˺�����������:char *tmp = GetBuffer; delete tmp;��Ȼ���ڴ�й©
	{
		ASSERT(m_buf == NULL);
		int nOffset = 0;
		m_buf = new char[GetLength()];
		memcpy(m_buf, (char*)&m_nDateLen, sizeof(int));

		nOffset = sizeof(int);
		memcpy(m_buf+nOffset, (char*)&m_nDataLen, sizeof(int));

		nOffset += sizeof(int);
		memcpy(m_buf+nOffset, m_bDate, m_nDateLen);

		nOffset += m_nDateLen;
		memcpy(m_buf+nOffset, m_bData, m_nDataLen);

		// ����������ָ���
		if (m_bData != NULL)
			delete m_bData;
		if (m_bDate != NULL)
			delete m_bDate;

		m_bData = NULL;
		m_bDate = NULL;

		return m_buf;
	}
public:
	EM_ChattingData(char *buf, int len)
	{
		ASSERT(NULL != buf);

		m_nDataLen = 0;
		m_nDateLen = 0;
		m_bData = NULL;
		m_bDate = NULL;

		m_buf = new char[len];
		memcpy(m_buf, buf, len);
		memcpy(&m_nDateLen, m_buf, sizeof(int));
		memcpy(&m_nDataLen, m_buf+sizeof(int), sizeof(int));
	}
	EM_ChattingData()
	{
		m_nDataLen = 0;
		m_nDateLen = 0;
		m_buf = NULL;
		m_bData = NULL;
		m_bDate = NULL;
	}

	~EM_ChattingData()
	{
		if (m_buf)
			delete m_buf;
		if (m_bDate)
			delete m_bDate;
		if (m_bData)
			delete m_bData;
	}
};

#endif // EM_CHATTINGHISTORY
// 2 EM_Data class define ----------------------------------------------------------------------



// 3 Ports for socket define -------------------------------------------------------------------
#ifndef _PORT_5150
#define _PORT_5150
// Broadcast message receiving and sending port
#define		BROADCAST_RECV_PORT		5150
#define		BROADCAST_SEND_PORT		BROADCAST_RECV_PORT
#endif // _PORT_5150

#ifndef _PORT_9088		// ��Ϣ���ܷ��Ͷ˿�
#define _PORT_9088
// Text message receiving and sending port
#define MSG_RECV_PORT			9088
#define MSG_SEND_PORT			MSG_RECV_PORT
#endif // _PORT_9088

#ifndef _PORT_7878 // �ļ����ܷ��Ͷ˿�
#define _PORT_7878
// File sending and receiving port
#define FILE_RECV_PORT			7878
#define FILE_SEND_PORT			FILE_RECV_PORT
#endif // _PORT_7878

#ifndef _PORT_8016 // �û���Ϣ���ܷ��Ͷ˿�
#define _PORT_8016
#define UDP_RECV_MSG_PORT		8016
#define UDP_SEND_PORT			UDP_RECV_MSG_PORT
#endif // _PORT_8016

// 3 Ports for socket define -------------------------------------------------------------------



// 4 Windows message define --------------------------------------------------------------------
// �������UDP ���ݾͷ��������Ϣ
#define		WM_UDP_MSG				WM_USER + 0x023
// ���յ��㲥��Ϣ���͸����ڵ���Ϣ
#define		WM_BC_MSG				WM_USER + 0x024
#define		WM_SENDFILE				WM_USER + 0x025
#define		WM_NEW_FILE				WM_USER	+ 0x026
// ˫��User List TreeCtrlʱ���͵���Ϣ
#define		WM_EMDBLCLICKTREE		WM_USER + 0x027
// ֪ͨ�����ڷ�����Ϣ����Ӧ��IP��ַ
#define		WM_SEND_MSG				WM_USER + 0x028

// --------�ļ�������Ϣ----------------------------------------------
// ����µ��ļ����б��б�ֻ�����256
#define		WM_ADDTOFILELIST		WM_USER + 0x029


// ��ʼ����wParamָ�����ļ�
#define		WM_DOWNLOADFILE			WM_USER + 0x030


// ɾ���ļ��б���wParamָ�����ļ�
#define		WM_DELETEFILE			WM_USER + 0x031


// �ظ�BC��½��Ϣ������UDP��½��Ϣ
#define		WM_REPLYBCMSG			WM_USER + 0x032


// ������µ���Ϣ�����ͷ��������Ϣ
#define		WM_NEW_MSG				WM_USER + 0x033


// �ܾ������ļ�
#define		WM_REFUSEFILE			WM_USER + 0x034


// �Է�ͬ������ļ�
#define		WM_DESTACCEPTFILE		WM_USER + 0x035


// �ļ���ȡ���
#define		WM_FILERECVCOMPLETED	WM_USER + 0x036


// �ļ��������
#define		WM_FILESENDCOMPLETED	WM_USER + 0x037


// �Ҽ��˵� -> ˢ��
#define		WM_EMREFLESH			WM_USER + 0x038


// �Ҽ��˵� -> ����
#define		WM_EMPROPERTY			WM_USER + 0x039


// �Ҽ��˵� -> ������Ϣ
#define		WM_EMSENDMSG			WM_USER + 0x040


// �Ҽ��˵� -> Ⱥ��
#define		WM_EMSENDALL			WM_USER + 0x041

// �Ի��� �ļ�������� ���µ����Ի���ߴ�
#define		WM_UPDATEDLG			WM_USER + 0x042

#define	WM_RECVFILE					WM_USER + 0x043
#define WM_FILESENDINGCOMPLETE		WM_USER + 0x044
#define WM_FILERECEIVINGCOMPLETE	WM_USER + 0x045
#define WM_FILESENDINGFAILED		WM_USER + 0x046
// �û��˳�����û��Ի����ͷŵ�
#define		WM_USERLOGOUT			WM_USER + 0x047
// �Է��������������Ի�
#define		WM_REQUESTVOICE			WM_USER + 0x048
// �ܾ����������Ի�
#define		WM_EMDONOTCHAT			WM_USER + 0x049
// ȡ����Է����������Ի�
#define		WM_EMCANCELREQUEST		WM_USER + 0x050
// �Է�ȡ���������������Ի�
#define		WM_CANCELREQUESTVOICE	WM_USER + 0x051
// �Է��ܾ������Ի�����
#define		WM_DONOTACCEPTCHAT		WM_USER + 0x052
// ���ܶԷ��Ի�����
#define		WM_SRCACCEPTCHAT		WM_USER + 0x053
// �Է����ܶԻ�����
#define		WM_ACCEPTCHAT			WM_USER + 0x055
// WaveIn ¼���߳̽���
#define		WM_WAVEINEND			WM_USER + 0x056
// �յ����ڶ�������
#define		WM_SHAKEWINDOW			WM_USER + 0x057
// �鿴��ʷ�˵�
#define		WM_EMHISTORY			WM_USER + 0x058


// �������У������û��޸����û���Ϣ
#define		WM_DESKUPDATEINFO		WM_USER + 0x059
// 4 Windows message define --------------------------------------------------------------------


// 5 Function define ---------------------------------------------------------------------------
void EM_Error(CString strError, DWORD dwErrorCode);
// ͨ�������������ȡ��������IP��ַ
//BOOL EM_GetIPByPCName(const char *name, char *buf);
// �����ʼ������
void NetworkInitialize();
// ��ȡ��������
BOOL EM_GetHostName_(const char *lpszIP, char *buf);
// ��ȡ�����Ŀ¼
void GetProgramDirectory(char *buf);
// �ж��Ƿ�ΪIP��ַ
BOOL IsIPAddress(const char* szip);

// ��ȡ������MAC
void GetMAC(LPCTSTR lpszIP, char *buf);

// ��ȡ�ļ�����չ�������ص��ڴ������ delete ɾ��
LPSTR GetFileExt(CString filename);
// 5 Function define ---------------------------------------------------------------------------

// 6 Data structure define ---------------------------------------------------------------------
class EM_FILEINFO
{
public:
	EM_FILEINFO()
	{
		index1 = 0;
		index2 = 0;
		dwSize = 0;
		dwSendFileIndex = 0;
		dwRecvFileIndex = 0;

		szExt[0] = NULL;
		szName[0] = NULL;
		szFullName[0] = NULL;
	};
	DWORD index1;				// �ļ��ı�ʶ1
	DWORD index2;				// �ļ��ı�ʶ2
	DWORD dwSize;				// �ļ���С
	DWORD dwSendFileIndex;		// '����'�ļ���ʶ
	DWORD dwRecvFileIndex;		// '����'�ļ���ʶ

	char *FileName();
	char *FileFullName();
	char *FileExt();
	BOOL AddFile(LPCTSTR lpszFileFullName, LPCTSTR lpszFileExt);

	char szExt[16];				// �ļ���չ��
	char szName[MAX_PATH];	// �ļ���
	char szFullName[MAX_PATH];	// �ļ�ȫ��,����·��
};
// 6 Data structure define ---------------------------------------------------------------------


// 7 Macro Definition ------------------------------------------------------------------------------

// �����¼�����ļ�
#define HISTORY_FILE_NAME			"hisory.dlx"
// �û���Ϣ��¼�ļ�
#define USER_INFO_FILE_NAME			"uinfov15.dat"

// �ļ�������ȶԻ���ĳ���
#define FILE_SENDING_DLG_LEN		62

// �ļ�����ʱʹ�õĻ�������С
#define FILE_SENDING_BUFFER_SIZE	100*188

// 7 Macro Definition ------------------------------------------------------------------------------
/*
typedef struct {
    UINT  cbSize;
    HWND  hwnd;
    DWORD dwFlags;
    UINT  uCount;
    DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

#define FLASHW_STOP         0
#define FLASHW_CAPTION      0x00000001
#define FLASHW_TRAY         0x00000002
#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
#define FLASHW_TIMER        0x00000004
#define FLASHW_TIMERNOFG    0x0000000C
*/
#endif // STDEMDEF_H