#if !defined(AFX_SELF_H__7955D07F_CC3F_4373_A474_07D66796419C__INCLUDED_)
#define AFX_SELF_H__7955D07F_CC3F_4373_A474_07D66796419C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Self.h : header file
//

#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSelf dialog

class CSelf : public CDialog
{
// Construction
public:
	CSelf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelf)
	enum { IDD = IDD_SELF };
	CMyEdit	m_tipEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelf)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelf)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DrawTip();
	void DrawMessage(CRect &inRect);
	void PaintWindow();
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);
	int m_xScreen;
	int m_yScreen;
	
	BOOL m_bShowMsg;                //显示截取矩形大小信息
	BOOL m_bDraw;                   //是否为截取状态
	BOOL m_bFirstDraw;              //是否为首次截取
	BOOL m_bQuit;                   //是否为退出
	CPoint m_startPt;				//截取矩形左上角
	CRectTracker m_rectTracker;     //像皮筋类
	CBrush m_brush;	
    HCURSOR m_hCursor;              //光标
	CBitmap * m_pBitmap;            //背景位图
	
	CRgn m_rgn;						//背景擦除区域
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELF_H__7955D07F_CC3F_4373_A474_07D66796419C__INCLUDED_)
