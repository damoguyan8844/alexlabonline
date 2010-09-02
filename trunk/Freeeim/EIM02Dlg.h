// EIM02Dlg.h : header file
// 飞鸽传书 FreeEIM 主程序
// 更新日期：2009年12月3日
// 作者：蔡镇秋(freeeim@gmail.com)
// 飞鸽传书官方网站：http://www.freeeim.com/
// 功能：控制整体实现

#if !defined(AFX_EIM02DLG_H__4D50BAB8_62C8_4203_A64B_2027BD828241__INCLUDED_)
#define AFX_EIM02DLG_H__4D50BAB8_62C8_4203_A64B_2027BD828241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "thirdpartclass/SystemTray.h"
#include "thirdpartclass/PictureEx.h"
#include "em/EM_UserTree.h"
#include "em/EM_UserLogin.h"
#include "em/EM_FileReceiving.h"
#include "freeeim_user_logic.h"
#include "freeeim_config.h"
#include "EM_MsgDlg.h"
#include "MSGReceiver.h"
#include "EM_RecvChat.h"
#include "EIMChatingWaiter.h"
#include "webbrowser2.h"

/////////////////////////////////////////////////////////////////////////////
// CEIM02Dlg dialog

//class _DLG_Advertise;
//class FreeEIM_User_Logic;
//class FreeEIM_Config;

class CEIM02Dlg : public CDialog
{
// Construction
public:
	LPCTSTR GetLocalDisplayName();
	void EM_CheckForUpdate(LPCTSTR lpszURL);

	char m_szLastError[1024];	// 所有线程内发生的错误信息；
	void _Msg_DelDialog(HTREEITEM hItem);
	void _Dest_User_Update_Group(FreeEIM_USER *pUser);
	void _Dest_User_Update_Name(FreeEIM_USER *pUser);


//	void _Init_Autorun();

	EM_MsgDlg* _Msg_GetDialog(HTREEITEM hItem);

	HTREEITEM	_User_GetUserItem(LPCTSTR lpszIP);
	HTREEITEM	_User_GetGroupItem(LPCTSTR lpszGroup);
	void		_User_UserUpdate(LPCTSTR lpszName, EM_USERINFO & ui);
	void		_User_UserUpdate(LPCTSTR lpszIP, LPCTSTR lpszName, LPCTSTR lpszGroup);
	void		_User_AddNewUser(LPCTSTR lpszName, EM_USERINFO & ui);
	BOOL		_User_IsNewUser(LPCTSTR lpszIP);


	// 功能：获取当前程序所在目录
	// 更新日期：2009年12月5日
	void GetProgramDirectory(char *buf);
	CString GetAppDir();


	// 功能：获取指定计算机的名称
	// 更新日期：2009年12月5日
	CString GetComputerName();


	// 本机用户配置信息；2009年12月5日
	FreeEIM_Config m_config;

	BOOL m_bVoiceChat; // 正在进行一个语音对话

	CRITICAL_SECTION cs;

	// User status
	enum{
		STATUS_ONLINE=0,
			STATUS_OFFLINE=1
	};

	DWORD m_dwStatus;

	// 检测自动更新
//	_DLG_Advertise *m_pAdv;

	// 在线用户 treectrl 图标
	CImageList	m_ilUser;

	// 主画面 Tab 的图标
	CImageList	m_ilTools;

	HICON m_hIcon;
	CBrush m_Brush;
	COLORREF m_crStatic;

	//系统托盘
	CSystemTray m_sysTray;

	// 启动画面的动画
	CPictureEx m_pictureLoading;

	// 录音并且发送给对方
	CEIMChatingWaiter *m_cw;

	// 在线用户列表控件
	EM_UserTree	m_treeCtrlList;

	// 本地用户名
//	char m_szLocalName[256];

	// 本地用户组
//	char m_szLocalGroup[256];

	// 消息收发
	EM_MSGReceiver m_MSGrecv;

	// 文件接收
	EM_FileReceiving m_fileRecv;

	// 用户登陆
	EM_UserLogin m_UserLogin;

	// 接收文件信息
	LPEM_FILEINFO m_fInfo[256];

	// 对话框数组
	EM_MsgDlg *m_Chatdlg[256];

	// 用来标识是否已登录
	BOOL m_bIsLogin;

	// 用户列表 [hItem], [strIP], 第一位就是自己
	map<HTREEITEM, string> m_mapUsers;

	// 更新处理
	CWebBrowser2 m_webBrowser;

	// 历史记录
	FreeEIM_History m_history;
	// 程序退出处理
	void EM_QuitEIM();
	void InitUI();
	void CreateToday_sFile();
	void EM_UpdateUserInfo(LPCTSTR lpszName, LPCTSTR lpszGroup);
	void SendMSGToAll(LPEM_DATA lpData);
	void EM_HideLoading();

	// FreeEIM 初始化工作
	void EM_OnInitDlg();

// Dialog Data
// Implementation
	CEIM02Dlg(CWnd* pParent = NULL);	// standard constructor
	//{{AFX_DATA(CEIM02Dlg)
	enum { IDD = IDD_EIM02_DIALOG };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEIM02Dlg)
	public:
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CEIM02Dlg)
	afx_msg void FileComming(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_DownloadFile(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_NewMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_NewFile(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_AddFileToList(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_FileSendCompleted(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_RefuseFile(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_DestAcceptFile(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_DeleteFileFromList(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_ReplyUDPMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_Voice_Income(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_Voice_CancelIncome(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_Voice_DoNotAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_Voice_DestAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_ShakeWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_ViewHistory(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHelptopic();
	afx_msg void OnViewHistory();
	afx_msg void EM_UserSetting();
	afx_msg void OnAbouteim();
	afx_msg void OnTrayRestore();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuit();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEM_DblClickTree(WPARAM wParam, LPARAM lParam);
	afx_msg void OnQuiteim();
//	afx_msg void EM_OnAutorun();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void EM_Reflesh(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_UserLogout(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_SendAll(WPARAM wParam, LPARAM lParam);
	afx_msg void EM_EMProperty(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRelesh();
	afx_msg void OnSendall();
	afx_msg void OnLanexplorer();
	afx_msg void OnLockscreen();
	afx_msg void OnClosescreen();
	afx_msg void OnSnow();
	afx_msg void OnWininfo();
	afx_msg void OnDiskname();
	afx_msg void OnNote();
	afx_msg void OnRecvChat(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOK();
	afx_msg void OnCancel();

	afx_msg void _Dest_User_Update(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EIM02DLG_H__4D50BAB8_62C8_4203_A64B_2027BD828241__INCLUDED_)
