// DaemonManagerDlg.h : header file
//

#if !defined(AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_)
#define AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg dialog

class CDaemonManagerDlg : public CDialog
{
// Construction
public:
	CDaemonManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDaemonManagerDlg)
	enum { IDD = IDD_DAEMONMANAGER_DIALOG };
	CButton	m_btFix;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaemonManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//�����ƶ�ʱ���ڵĴ�С
	void FixMoving(UINT fwSide, LPRECT pRect);
	//�����ĸı䴰�ڴ�Сʱ���ڵĴ�С
	void FixSizing(UINT fwSide, LPRECT pRect);
	//������״̬��ʾ����
	void DoShow();
	//����ʾ״̬��������
	void DoHide();
	//���غ���,ֻ��Ϊ�˷������
	BOOL SetWindowPos(const CWnd* pWndInsertAfter,
		LPCRECT pCRect, UINT nFlags = SWP_SHOWWINDOW);

	HICON m_hIcon;
	HICON m_hIconFix;
	HICON m_hIconHide;

	// Generated message map functions
	//{{AFX_MSG(CDaemonManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnButtonFix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_isSizeChanged;   //���ڴ�С�Ƿ�ı��� 
	BOOL m_isSetTimer;      //�Ƿ������˼������Timer
	
	INT  m_oldWndHeight;    //�ɵĴ��ڿ��
	INT  m_taskBarHeight;   //�������߶�
	INT  m_edgeHeight;      //��Ե�߶�
	INT  m_edgeWidth;       //��Ե���
	
	INT  m_hideMode;        //����ģʽ
	BOOL m_hsFinished;      //���ػ���ʾ�����Ƿ����
    BOOL m_hiding;          //�ò���ֻ����!m_hsFinished����Ч
	BOOL m_initialed;
	
	BOOL m_isFix;

	CBitmapButton m_btClose;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_)
