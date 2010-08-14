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
//一个模仿QQ自动收缩的程序
//作者昵称: 落叶
//QQ:63071024
//EM:windboyzsj@qq.com
//欢迎打扰!
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
	//修正移动时窗口的大小
	void FixMoving(UINT fwSide, LPRECT pRect);
	//修正改改变窗口大小时窗口的大小
	void FixSizing(UINT fwSide, LPRECT pRect);
	//从收缩状态显示窗口
	void DoShow();
	//从显示状态收缩窗口
	void DoHide();
	//重载函数,只是为了方便调用
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
	BOOL m_isSizeChanged;   //窗口大小是否改变了 
	BOOL m_isSetTimer;      //是否设置了检测鼠标的Timer
	
	INT  m_oldWndHeight;    //旧的窗口宽度
	INT  m_taskBarHeight;   //任务栏高度
	INT  m_edgeHeight;      //边缘高度
	INT  m_edgeWidth;       //边缘宽度

	INT  m_hideMode;        //隐藏模式
	BOOL m_hsFinished;      //隐藏或显示过程是否完成
    BOOL m_hiding;          //该参数只有在!m_hsFinished才有效
	                        //真:正在隐藏,假:正在显示
/*	
public:
	BOOL m_topMost;         //是否使用TopMost风格
	BOOL m_useSteps;        //是否使用抽屉效果
	BOOL m_toolWnd;         //是否使用Tool Window 风格
*/
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQHIDEWNDDLG_H__05267C1A_444D_40D3_988D_0E07B366230E__INCLUDED_)
