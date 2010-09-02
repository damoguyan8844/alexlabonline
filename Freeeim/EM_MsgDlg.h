#if !defined(AFX_EM_MSGDLG_H__84E17C3A_C4CC_43EE_AA88_572DB196CFE5__INCLUDED_)
#define AFX_EM_MSGDLG_H__84E17C3A_C4CC_43EE_AA88_572DB196CFE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EM_MsgDlg.h : header file
//
#include "resource.h"
#include "em/EMRichEdit.h"
#include "em/EM_FileSendingDlg.h"
#include "EM_RecvChat.h"
#include "EM_SendChat.h"
#include "em/IM_HistoryWriter.h"
#include "freeeim_history.h"
/////////////////////////////////////////////////////////////////////////////
// EM_MsgDlg dialog
class CEIM02Dlg;

class EM_MsgDlg : public CDialog
{
// Construction
public:



//////////////////////////////////////////////////////////////////////////
// 函数区
//////////////////////////////////////////////////////////////////////////

	void _InitHistoryToday();
	void _InitFaceToolbar();
	void _PlaySoundSend();
	void _InsertHeader(LPCTSTR lpszInfo);
	void _InsertText(LPCTSTR lpszText);
	void _InsertFace(DWORD dwFace);
	void _PlaySoundReceive();
	// 显示消息：文件发送完毕
	void File_SendComplete(int index);
	// 显示消息：文件发送失败
	void File_SendFailed(int index);
	// 显示消息：文件接收完毕
	void File_RecvComplete(int index);
	// 显示消息：文件接收失败
	void File_RecvFailed(int index);

	// 请求语音对话
	void Voice_Request();
	// 被请求语音对话
	void Voice_Income();
	// 对方取消请求语音对话
	void Voice_CancelIncome(int nValue);
	// 对方拒绝接受对话请求
	void Voice_DoNotAcceptChat(int nValue);
	// 对方接受对话请求
	void Voice_DestAcceptChat();

	// 窗口抖动
	void Shake_Window();

	// cData:收到的数据,come:TRUE表示发出，FALSE表示接收
	void File_RecvFile(SOCKET sock);
	void AddRecvText(LPCTSTR msg);
	void AddSendText(LPCTSTR msg);
	void Voice_AddText(LPCTSTR msg);
	void File_ResAcceptFile(ENLINK *pLink);
	void File_ResDenyFile(ENLINK *pLink);
	void NewFile(LPEM_FILEINFO fInfo);
	BOOL IsCurrentUser(LPSTR hostName);
	void File_DestDenyFile(LPEM_FILEINFO fInfo);
	void File_DestAcceptFile(LPEM_FILEINFO fInfo);
	void File_RecvCancel(LPEM_FILEINFO pFinfo);




//////////////////////////////////////////////////////////////////////////
// 变量区
//////////////////////////////////////////////////////////////////////////

	CString					m_strDisplayName;
	CString					m_strUserPCName;
	CString					m_strUserIP;
	EM_FileSendingDlg		*m_pDlg[6];
	DWORD					m_dwTotleDlg;
	LPEM_FILEINFO			m_RecvfileInfo[6];
	DWORD					m_dwRecvTotal;
	LPEM_FILEINFO			m_SendfileInfo[6];

	CEIM02Dlg*				m_pMainTop; // 主窗口句柄

	// 工具栏
	CToolBar m_FirstToolBar;
	CImageList m_imgListFaces;
	CImageList& GetImageList();
	HBITMAP GetImage(CImageList& ilist, int num);

	DWORD					m_dwSendTotal;
	DWORD					m_dwFileDlgLength;
	DWORD					k; // 最大文件发送上限
	CRect					m_rtDlg;
	BOOL					m_bSendingFile;
	EM_MsgDlg(CWnd* pParent = NULL);   // standard constructor

	// 接收语音对话框
	EM_RecvChat *m_pRecvChat;
	// 向对方发送请求语音聊天
	EM_SendChat *m_pSendChat;
	// 历史记录

	~EM_MsgDlg();

// Dialog Data
	//{{AFX_DATA(EM_MsgDlg)
	enum { IDD = IDD_DIALOG1 };
	EM_MsgRichEdit	m_richInput; // 消息发送框
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EM_MsgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Rich edit 用来显示消息
	EM_MsgRichEdit	m_RichEditShow;

	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(EM_MsgDlg)
	afx_msg void OnFaceSelect(UINT nID);
	afx_msg void OnBtnSend();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnShake();
	afx_msg void OnUpdateDlg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnSendFile();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam);
	afx_msg void Voice_CancelRequest(WPARAM wParam, LPARAM lParam);
	afx_msg void Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EM_MSGDLG_H__84E17C3A_C4CC_43EE_AA88_572DB196CFE5__INCLUDED_)
