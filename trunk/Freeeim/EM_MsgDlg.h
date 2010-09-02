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
// ������
//////////////////////////////////////////////////////////////////////////

	void _InitHistoryToday();
	void _InitFaceToolbar();
	void _PlaySoundSend();
	void _InsertHeader(LPCTSTR lpszInfo);
	void _InsertText(LPCTSTR lpszText);
	void _InsertFace(DWORD dwFace);
	void _PlaySoundReceive();
	// ��ʾ��Ϣ���ļ��������
	void File_SendComplete(int index);
	// ��ʾ��Ϣ���ļ�����ʧ��
	void File_SendFailed(int index);
	// ��ʾ��Ϣ���ļ��������
	void File_RecvComplete(int index);
	// ��ʾ��Ϣ���ļ�����ʧ��
	void File_RecvFailed(int index);

	// ���������Ի�
	void Voice_Request();
	// �����������Ի�
	void Voice_Income();
	// �Է�ȡ�����������Ի�
	void Voice_CancelIncome(int nValue);
	// �Է��ܾ����ܶԻ�����
	void Voice_DoNotAcceptChat(int nValue);
	// �Է����ܶԻ�����
	void Voice_DestAcceptChat();

	// ���ڶ���
	void Shake_Window();

	// cData:�յ�������,come:TRUE��ʾ������FALSE��ʾ����
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
// ������
//////////////////////////////////////////////////////////////////////////

	CString					m_strDisplayName;
	CString					m_strUserPCName;
	CString					m_strUserIP;
	EM_FileSendingDlg		*m_pDlg[6];
	DWORD					m_dwTotleDlg;
	LPEM_FILEINFO			m_RecvfileInfo[6];
	DWORD					m_dwRecvTotal;
	LPEM_FILEINFO			m_SendfileInfo[6];

	CEIM02Dlg*				m_pMainTop; // �����ھ��

	// ������
	CToolBar m_FirstToolBar;
	CImageList m_imgListFaces;
	CImageList& GetImageList();
	HBITMAP GetImage(CImageList& ilist, int num);

	DWORD					m_dwSendTotal;
	DWORD					m_dwFileDlgLength;
	DWORD					k; // ����ļ���������
	CRect					m_rtDlg;
	BOOL					m_bSendingFile;
	EM_MsgDlg(CWnd* pParent = NULL);   // standard constructor

	// ���������Ի���
	EM_RecvChat *m_pRecvChat;
	// ��Է�����������������
	EM_SendChat *m_pSendChat;
	// ��ʷ��¼

	~EM_MsgDlg();

// Dialog Data
	//{{AFX_DATA(EM_MsgDlg)
	enum { IDD = IDD_DIALOG1 };
	EM_MsgRichEdit	m_richInput; // ��Ϣ���Ϳ�
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

	// Rich edit ������ʾ��Ϣ
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
