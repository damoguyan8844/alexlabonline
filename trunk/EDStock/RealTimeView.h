
#pragma once

#include "RealTime.h"

#define	RTV_MAX_MULTICOUNT 9

class CRealTimeView : public CView
{
	DECLARE_DYNCREATE(CRealTimeView)
protected:
	CRealTimeView();
	virtual ~CRealTimeView();

protected:
	int m_nMultiStockCount;
	CRealTime m_realtime[RTV_MAX_MULTICOUNT];

public:
	void	ShowMultiStock( int nMultiStockCount );
	int		GetShowMultiStock( )	{	return m_nMultiStockCount;	}

	void	ChangeDrawMode( int nDrawMode );
	void	ToggleDrawTechLine( int nTechLine );
	int		GetDrawMode( )	{	return m_realtime[0].GetDrawMode();	}
	int		GetDrawTechLine()	{	return m_realtime[0].GetDrawTechLine();	}

	void	SendRequestQuote( BOOL bForced );

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	void	RedrawAll( CDC * pDC, CRect rectClient );

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRealtimePrev();
	afx_msg void OnRealtimeNext();
	afx_msg void OnRealtimeLeft();
	afx_msg void OnRealtimeRight();
	afx_msg void OnRealtimeUp();
	afx_msg void OnRealtimeDown();
	afx_msg void OnRealtimeHome();
	afx_msg void OnRealtimeEnd();
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};
