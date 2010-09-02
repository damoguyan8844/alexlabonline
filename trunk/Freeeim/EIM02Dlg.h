// EIM02Dlg.h : header file
// �ɸ봫�� FreeEIM ������
// �������ڣ�2009��12��3��
// ���ߣ�������(freeeim@gmail.com)
// �ɸ봫��ٷ���վ��http://www.freeeim.com/
// ���ܣ���������ʵ��

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

	char m_szLastError[1024];	// �����߳��ڷ����Ĵ�����Ϣ��
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


	// ���ܣ���ȡ��ǰ��������Ŀ¼
	// �������ڣ�2009��12��5��
	void GetProgramDirectory(char *buf);
	CString GetAppDir();


	// ���ܣ���ȡָ�������������
	// �������ڣ�2009��12��5��
	CString GetComputerName();


	// �����û�������Ϣ��2009��12��5��
	FreeEIM_Config m_config;

	BOOL m_bVoiceChat; // ���ڽ���һ�������Ի�

	CRITICAL_SECTION cs;

	// User status
	enum{
		STATUS_ONLINE=0,
			STATUS_OFFLINE=1
	};

	DWORD m_dwStatus;

	// ����Զ�����
//	_DLG_Advertise *m_pAdv;

	// �����û� treectrl ͼ��
	CImageList	m_ilUser;

	// ������ Tab ��ͼ��
	CImageList	m_ilTools;

	HICON m_hIcon;
	CBrush m_Brush;
	COLORREF m_crStatic;

	//ϵͳ����
	CSystemTray m_sysTray;

	// ��������Ķ���
	CPictureEx m_pictureLoading;

	// ¼�����ҷ��͸��Է�
	CEIMChatingWaiter *m_cw;

	// �����û��б�ؼ�
	EM_UserTree	m_treeCtrlList;

	// �����û���
//	char m_szLocalName[256];

	// �����û���
//	char m_szLocalGroup[256];

	// ��Ϣ�շ�
	EM_MSGReceiver m_MSGrecv;

	// �ļ�����
	EM_FileReceiving m_fileRecv;

	// �û���½
	EM_UserLogin m_UserLogin;

	// �����ļ���Ϣ
	LPEM_FILEINFO m_fInfo[256];

	// �Ի�������
	EM_MsgDlg *m_Chatdlg[256];

	// ������ʶ�Ƿ��ѵ�¼
	BOOL m_bIsLogin;

	// �û��б� [hItem], [strIP], ��һλ�����Լ�
	map<HTREEITEM, string> m_mapUsers;

	// ���´���
	CWebBrowser2 m_webBrowser;

	// ��ʷ��¼
	FreeEIM_History m_history;
	// �����˳�����
	void EM_QuitEIM();
	void InitUI();
	void CreateToday_sFile();
	void EM_UpdateUserInfo(LPCTSTR lpszName, LPCTSTR lpszGroup);
	void SendMSGToAll(LPEM_DATA lpData);
	void EM_HideLoading();

	// FreeEIM ��ʼ������
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
