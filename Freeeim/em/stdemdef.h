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
// 飞鸽传书源码
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
#define EM_SENDFILEINFO			0x00000008  // 文件信息
#define EM_USERRENAME			0x00000009	// 用户更改显示名字
#define	EM_REFUSEFILE			0x00000010	// 拒绝接收文件
#define	EM_FILERECVCOMPLETED	0x00000011	// 文件收取完毕
#define	EM_FILESENDCOMPLETED	0x00000012	// 文件发送完毕
#define EM_ACCEPTFILE			0x00000013	// 同意接受文件
#define	EM_REQUESTVOICE			0x00000015	// 发送请求语音对话
#define	EM_CANCELREQUESTVOICE	0x00000016	// 你停止了语音对话的请求
#define EM_DONOTCHAT			0x00000017	// 你拒绝接受语音对话请求
#define EM_ACCEPTCHAT			0x00000018	// 你同意接受语音对话请求
#define	EM_CHATTINGEND			0x00000019	// 语音对话结束
#define	EM_SHAKEWINDOW			0x00000020	// 窗口抖动消息
#define EM_USERREGROUP			0x00000021	// 修改用户分组
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

// 开源即时通讯软件 飞鸽传书 FreeEIM
class EM_USERINFO
{
public:
	// 用户信息数据包初始化
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


// 用户信息包
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
	// 数据长度
	int m_nDataLen;
	// 日期长度
	int m_nDateLen;
	// 数据缓冲区
	char *m_bData;
	// 日期缓冲区
	char *m_bDate;
	// 用来返回的缓冲区
	char *m_buf;

public:
	// 保存偏移量的整数变量
	static int s_nOffset;

	// 总长度为 2 个整数长度加数据长度
	inline int GetLength()
	{
		int nLen = 0;
		nLen += sizeof(int); // 保存数据长度的变量长度
		nLen += sizeof(int); // 保存日期长度的变量长度
		nLen += m_nDataLen; // 数据长度
		nLen += m_nDateLen; // 日期长度
		// 返回数据包总长度
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

	char *GetBuffer()  // 使用此函数必须这样:char *tmp = GetBuffer; delete tmp;不然会内存泄漏
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

		// 用完了重新指向空
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

#ifndef _PORT_9088		// 消息接受发送端口
#define _PORT_9088
// Text message receiving and sending port
#define MSG_RECV_PORT			9088
#define MSG_SEND_PORT			MSG_RECV_PORT
#endif // _PORT_9088

#ifndef _PORT_7878 // 文件接受发送端口
#define _PORT_7878
// File sending and receiving port
#define FILE_RECV_PORT			7878
#define FILE_SEND_PORT			FILE_RECV_PORT
#endif // _PORT_7878

#ifndef _PORT_8016 // 用户信息接受发送端口
#define _PORT_8016
#define UDP_RECV_MSG_PORT		8016
#define UDP_SEND_PORT			UDP_RECV_MSG_PORT
#endif // _PORT_8016

// 3 Ports for socket define -------------------------------------------------------------------



// 4 Windows message define --------------------------------------------------------------------
// 如果有新UDP 数据就发送这个消息
#define		WM_UDP_MSG				WM_USER + 0x023
// 接收到广播消息后发送给窗口的消息
#define		WM_BC_MSG				WM_USER + 0x024
#define		WM_SENDFILE				WM_USER + 0x025
#define		WM_NEW_FILE				WM_USER	+ 0x026
// 双击User List TreeCtrl时发送的消息
#define		WM_EMDBLCLICKTREE		WM_USER + 0x027
// 通知主窗口发送消息到相应的IP地址
#define		WM_SEND_MSG				WM_USER + 0x028

// --------文件下载消息----------------------------------------------
// 添加新的文件到列表，列表只能最大256
#define		WM_ADDTOFILELIST		WM_USER + 0x029


// 开始下载wParam指定的文件
#define		WM_DOWNLOADFILE			WM_USER + 0x030


// 删除文件列表中wParam指定的文件
#define		WM_DELETEFILE			WM_USER + 0x031


// 回复BC登陆信息，发送UDP登陆信息
#define		WM_REPLYBCMSG			WM_USER + 0x032


// 如果有新的消息进来就发送这个消息
#define		WM_NEW_MSG				WM_USER + 0x033


// 拒绝接收文件
#define		WM_REFUSEFILE			WM_USER + 0x034


// 对方同意接收文件
#define		WM_DESTACCEPTFILE		WM_USER + 0x035


// 文件收取完毕
#define		WM_FILERECVCOMPLETED	WM_USER + 0x036


// 文件发送完毕
#define		WM_FILESENDCOMPLETED	WM_USER + 0x037


// 右键菜单 -> 刷新
#define		WM_EMREFLESH			WM_USER + 0x038


// 右键菜单 -> 属性
#define		WM_EMPROPERTY			WM_USER + 0x039


// 右键菜单 -> 发送信息
#define		WM_EMSENDMSG			WM_USER + 0x040


// 右键菜单 -> 群发
#define		WM_EMSENDALL			WM_USER + 0x041

// 对话框 文件发送完毕 重新调整对话框尺寸
#define		WM_UPDATEDLG			WM_USER + 0x042

#define	WM_RECVFILE					WM_USER + 0x043
#define WM_FILESENDINGCOMPLETE		WM_USER + 0x044
#define WM_FILERECEIVINGCOMPLETE	WM_USER + 0x045
#define WM_FILESENDINGFAILED		WM_USER + 0x046
// 用户退出后把用户对话框释放掉
#define		WM_USERLOGOUT			WM_USER + 0x047
// 对方向我请求语音对话
#define		WM_REQUESTVOICE			WM_USER + 0x048
// 拒绝接受语音对话
#define		WM_EMDONOTCHAT			WM_USER + 0x049
// 取消向对方请求语音对话
#define		WM_EMCANCELREQUEST		WM_USER + 0x050
// 对方取消向我请求语音对话
#define		WM_CANCELREQUESTVOICE	WM_USER + 0x051
// 对方拒绝语音对话请求
#define		WM_DONOTACCEPTCHAT		WM_USER + 0x052
// 接受对方对话请求
#define		WM_SRCACCEPTCHAT		WM_USER + 0x053
// 对方接受对话请求
#define		WM_ACCEPTCHAT			WM_USER + 0x055
// WaveIn 录音线程结束
#define		WM_WAVEINEND			WM_USER + 0x056
// 收到窗口抖动请求
#define		WM_SHAKEWINDOW			WM_USER + 0x057
// 查看历史菜单
#define		WM_EMHISTORY			WM_USER + 0x058


// 局域网中，其他用户修改了用户信息
#define		WM_DESKUPDATEINFO		WM_USER + 0x059
// 4 Windows message define --------------------------------------------------------------------


// 5 Function define ---------------------------------------------------------------------------
void EM_Error(CString strError, DWORD dwErrorCode);
// 通过计算机名来获取该主机的IP地址
//BOOL EM_GetIPByPCName(const char *name, char *buf);
// 网络初始化函数
void NetworkInitialize();
// 获取主机名称
BOOL EM_GetHostName_(const char *lpszIP, char *buf);
// 获取程序的目录
void GetProgramDirectory(char *buf);
// 判断是否为IP地址
BOOL IsIPAddress(const char* szip);

// 获取主机的MAC
void GetMAC(LPCTSTR lpszIP, char *buf);

// 获取文件的扩展名，返回的内存必须用 delete 删除
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
	DWORD index1;				// 文件的标识1
	DWORD index2;				// 文件的标识2
	DWORD dwSize;				// 文件大小
	DWORD dwSendFileIndex;		// '发送'文件标识
	DWORD dwRecvFileIndex;		// '接收'文件标识

	char *FileName();
	char *FileFullName();
	char *FileExt();
	BOOL AddFile(LPCTSTR lpszFileFullName, LPCTSTR lpszFileExt);

	char szExt[16];				// 文件扩展名
	char szName[MAX_PATH];	// 文件名
	char szFullName[MAX_PATH];	// 文件全名,包括路径
};
// 6 Data structure define ---------------------------------------------------------------------


// 7 Macro Definition ------------------------------------------------------------------------------

// 聊天记录保存文件
#define HISTORY_FILE_NAME			"hisory.dlx"
// 用户信息记录文件
#define USER_INFO_FILE_NAME			"uinfov15.dat"

// 文件传输进度对话框的长度
#define FILE_SENDING_DLG_LEN		62

// 文件传输时使用的缓冲区大小
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