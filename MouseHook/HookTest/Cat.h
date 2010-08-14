#if !defined(AFX_CAT_H__346C392C_14BC_11D5_A025_006067718D04__INCLUDED_)
#define AFX_CAT_H__346C392C_14BC_11D5_A025_006067718D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cat.h : header file
//

class CWhisker {
    public:
       CWhisker(int stdx, int len, int dy, int bx, int by)
	     { startdx = stdx; length = len; deltay = dy; base.x = bx; base.y = by; }
       int startdx;
       int length;
       int deltay;
       CPoint base;
};
/////////////////////////////////////////////////////////////////////////////
// CCat window

#define UWM_GIVE_HINT_MSG _T("UWM_GIVE_HINT-{44E531B2-14D3-11d5-A025-006067718D04}")

class CCat : public CStatic
{
// Construction
public:
	CCat();
	void SetActive(BOOL mode) { active = mode; InvalidateEyes(); }
	void Redraw() { InvalidateEyes(); }
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCat();

	// Generated message map functions
protected:
        BOOL active;
	BOOL smiling;
	BOOL pawleft;
	BOOL pawright;
        void whisker(CDC &dc, CWhisker w, int offset);
	void eye(CDC & dc, CPoint pt, int size, CPoint see);
	void InvalidateEyes();
	CPoint GetFaceCenter();
	void GetEyeParam(CPoint & eyept, int & eyewidth, BOOL left);
	void mouth(CDC & dc);
	void GetMouthParam(CRect & mr);
	CRect GetFaceRect();
	void GetEarParam(CPoint pt[3], BOOL left);
	void ear(CDC & dc, CPoint pt[3]);
	void GetPawParam(CRect & pawr, BOOL left);
	void paw(CDC & dc, CRect pawr, BOOL pawdown);
	//{{AFX_MSG(CCat)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAT_H__346C392C_14BC_11D5_A025_006067718D04__INCLUDED_)
