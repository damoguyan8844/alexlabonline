// QQHideWndDlg.h : header file
//

#if !defined(AFX_QQHIDEWNDDLG_H__05267C1A_444D_40D3_988D_0E07B366230E__INCLUDED_)
#define AFX_QQHIDEWNDDLG_H__05267C1A_444D_40D3_988D_0E07B366230E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQQHideWndDlg dialog
//***************************************
//һ��ģ��QQ�Զ������ĳ���
//�����ǳ�: ��Ҷ
//QQ:63071024
//EM:windboyzsj@qq.com
//��ӭ����!
//2007.8.21 v2.0
//***************************************

class CQQHideWndDlg : public CDialog
{
// Construction
public:
	CQQHideWndDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQQHideWndDlg)
	enum { IDD = IDD_QQHIDEWND_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQHideWndDlg)
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

	// Generated message map functions
	//{{AFX_MSG(CQQHideWndDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
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
	                        //��:��������,��:������ʾ
/*	
public:
	BOOL m_topMost;         //�Ƿ�ʹ��TopMost���
	BOOL m_useSteps;        //�Ƿ�ʹ�ó���Ч��
	BOOL m_toolWnd;         //�Ƿ�ʹ��Tool Window ���
*/
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQHIDEWNDDLG_H__05267C1A_444D_40D3_988D_0E07B366230E__INCLUDED_)
