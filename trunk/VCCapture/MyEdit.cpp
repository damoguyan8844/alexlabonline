// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreen.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	//�������ֱ���͸��
	pDC->SetBkMode(TRANSPARENT);
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

BOOL CMyEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//ȡ�����ָ�ѡ
	this->SetSel(0,0);
	
	//����λͼ����
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);
	
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	
	dcCompatible.SelectObject(&bitmap);
	
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	return TRUE;

	//return CEdit::OnEraseBkgnd(pDC);
}

void CMyEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetWindowRect(&rect);
	
	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	//int ySCreen = GetSystemMetrics(SM_CYSCREEN);
	
	if(m_bMove)
	{
		//�ƶ������Ͻ�
		MoveWindow(0,0,rect.Width(),rect.Height());
		m_bMove=FALSE;
	}
	else
	{
		//�ƶ������Ͻ�
		MoveWindow(xScreen-240,0,rect.Width(),rect.Height());
		m_bMove=TRUE;
	}

	CEdit::OnMouseMove(nFlags, point);
}

void CMyEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//���ع����ʾ��
	this->HideCaret();
}
