// RealTime.h : interface of the CRealTime class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***
	��ͼ�࣬���𻭳���ʱ����ͼ�е����в���
*/
class CRealTime
{
public:
	CRealTime( );
	virtual	~CRealTime( );

	enum	DrawMode {
		modePriceLine		=	0x01,	// ��ʱ�۸���
		modeReportDetail	=	0x02,	// ÿ�ʳɽ���ϸ
		modeMinuteDetail	=	0x03,	// ���ӳɽ���ϸ
		modeBigTradeDetail	=	0x04,	// �󵥳ɽ���ϸ
	};
	enum	TechLine {
		techNone	=	0x00,
		techLBDK	=	0x01,
		techMMLD	=	0x02
	};
	enum	ReportWhat {
		reportNone		=	0x00,
		reportQuote		=	0x01,
		reportPrice		=	0x02,
		reportMinute	=	0x03,
		reportBuySellEx	=	0x04,
		reportValue		=	0x05,
		reportDistribute=	0x06,
		reportBigTrade	=	0x07,
	};
	
	// �趨�����ڣ��Ա��ڸ����������ڻ�ͼ��
	void		SetParent( CWnd * pParent );

	// �趨�߿��С
	void		ResetMargins(	int nMarginTop = 19, int nMarginLeft = 60,
								int nMarginCenter = 45, int nMarginBottom = 19,
								int nGapYAxis = 18, int nWidthReport = 200 );
	
	// �����ǰ��Ʊ������
	void		ClearCurStock( );
	
	// �趨��ǰҪ���Ĺ�Ʊ
	BOOL		SetCurStock( const char * szCode );
	BOOL		SetCurStock( CStockInfo & info );
	CStock &	GetCurStock( )	{	return m_CurStock;	}

	BOOL		PrepareStockData( );			// ׼����Ʊ����

	// ��ͼ����
	void		SetDrawMode( int nDrawMode = modePriceLine, int nTechLine = techNone, int nReportWhat = CRealTime::reportQuote );
	int			GetDrawMode( );
	int			GetDrawTechLine( );
	int			GetReportWhat( );

	// ˢ��ʵʱ��������
	LRESULT		OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	
	// �ػ�
	void		Redraw( CDC * pDC, CRect rectAll );
	void		DrawReportRegion( CDC * pDC );

public:
	//////////////////////////////////////////////////////////
	// modePriceLine �����ƶ��������Ҽ��ƶ�
	void		MoveLeft( BOOL bShiftPressed = FALSE );
	void		MoveRight( BOOL bShiftPressed = FALSE );
	void		MoveUp( BOOL bShiftPressed = FALSE );
	void		MoveDown( BOOL bShiftPressed = FALSE );
	void		MoveHome( BOOL bShiftPressed = FALSE );
	void		MoveEnd( BOOL bShiftPressed = FALSE );
	void		MoveTo( int nIndex, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		Move( int nMove, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnLButtonDblClk( UINT nFlags, CPoint point );
	void		ResetIndexCurrent( int nIndexCurrent = -1 );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	// modeReportDetail, modeMinuteDetail, modeBigTradeDetail ��ϸʹ��
	void		PageUp( );
	void		PageDown( );
	//////////////////////////////////////////////////////////

protected:
	//////////////////////////////////////////////////////////
	// modePriceLine ��ʱ�۸���ʹ��
	void		ResetClient( CRect rectAll );		// �����趨��ͼ����
	BOOL		AddIndexCurrent( int nIndex );
	void		OnIndexCurrentChanged( );
	void		DrawSelectionLine( CDC * pDC, BOOL bAddOrRemove );

	void		DrawDateAxis( CDC * pDC );			// ��ʱ����
	BOOL		GetMinMaxInfo( double* pdLastClose, double *pdMin, double *pdMax, double *pdMaxVolume, BOOL bUptoAxis );	// �õ������ͳɽ��۸���߳ɽ���
	void		DrawPriceVolume( CDC * pDC );		// ���۸��ߺͳɽ�����
	void		DrawMMLD( CDC * pDC );				// ����������ָ��
	void		DrawLBDK( CDC * pDC );				// �����ȶ��ָ��

	int			DrawBuySell( CDC * pDC, int xStart, int yStart, int nWidth );	// ��������Ϣ
	int			DrawBuySellEx( CDC * pDC, int xStart, int yStart, int nWidth );	// �������̱�����Ϣ
	void		DrawReportQuote( CDC * pDC );		// ��������Ϣ���ɽ���ϸ
	void		DrawReportPrice( CDC * pDC );		// ��������Ϣ���ּ۱�
	void		DrawReportMinute( CDC * pDC );		// ��������Ϣ�����ӳɽ���ϸ
	void		DrawReportBuySellEx( CDC * pDC );	// ��������Ϣ������������
	void		DrawReportValue( CDC * pDC );		// ��������Ϣ���۸���Ϣ
	void		DrawReportDistribute( CDC * pDC );	// ���ɽ��ֲ�
	void		DrawReportBigTrade( CDC * pDC );	// ���󵥳ɽ���Ϣ

	BOOL		LoadReportBigTrade( );				// ��ȡ����Ĵ󵥳ɽ���Ϣ
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	// modeReportDetail, modeMinuteDetail, modeBigTradeDetail ��ϸʹ��
	int			GetColumnPerPage( CRect rectAll );
	int			GetCountPerColumn( CRect rectAll );
	int			CheckValidStartPos( int nStartPosOld, int nTotalCount, int nPageCount, BOOL bAlignBegin );

	void		DrawReportDetail( CDC * pDC );
	void		DrawReportColumn( CDC * pDC, CStockInfo & info, CReport & aReport, int nStartPos, int nMaxCount, CRect rect );
	
	void		DrawMinuteDetail( CDC * pDC );
	void		DrawMinuteColumn( CDC * pDC, CStockInfo & info, CMinute & aMinute, int nStartPos, int nMaxCount, CRect rect );
	
	void		DrawBigTradeDetail( CDC * pDC );
	void		DrawBigTradeColumn( CDC * pDC, CReport & aMinute, int nStartPos, int nMaxCount, CRect rect );
	//////////////////////////////////////////////////////////

protected:
	// Attributes
	int			m_nMarginTop;		// �ϱ߿հ�
	int			m_nMarginLeft;		// ��߿հ�
	int			m_nMarginCenter;	// �м�հ�
	int			m_nMarginBottom;	// �±߿հ�
	int			m_nGapYAxis;		// ��������߸߶�
	int			m_nWidthReport;		// �ұ���Ϣ�����

	CWnd	*	m_pParent;			// ������
	CStock		m_CurStock;			// ��ǰ��Ʊ
	CRect		m_rectAll;			// ����Rect
	int			m_nDrawMode;		// See CRealTime::DrawMode����ǵ�ǰ����ʱ�۸��ߡ��ɽ���ϸ��

	// modePriceLine use
	CRect		m_rectPrice;		// �۸�Rect
	CRect		m_rectVolume;		// �ɽ���Rect
	CRect		m_rectLBDK;			// ����(���)ָ��
	CRect		m_rectMMLD;			// ���������������̼�������
	CRect		m_rectReport;		// ������Ϣ���ɽ���ϸRect
	int			m_nIndexCurrent;	// ��ǰѡ�е����
	CUIntArray	m_anIndexCurrent;	// ��ǰ���ѡ�е����
	int			m_nTechLine;		// see enum CRealTime::TechLine
	int			m_nReportWhat;		// �Ƿ�������Ϣ���ɽ���ϸ�����ͬ��ʱ����

	// others
	CReport		m_aReportBigTrade;
	int			m_nCurrentStartPos;	// ��ǰ��һ����ϸ�������������е�λ��ָ��
									// modeReportDetail => m_CurStock.GetReport()
									// modeMinuteDetail => m_CurStock.GetMinute()
									// modeBigTradeDetail => m_aReportBigTrade
									// modePriceLine's reportQuote => m_CurStock.GetReport()
									// modePriceLine's reportPrice => m_CurStock.GetMinute().StateDealInfo(...)
									// modePriceLine's reportMinute => m_CurStock.GetMinute()
									// modePriceLine's reportBuySellEx => m_CurStock.GetReport()
									// modePriceLine's reportBigTrade => m_aReportBigTrade
	int			m_nCurrentPageCount;// ��ǰ����ÿҳ��ʾ������Ӧ�ó���
									// modePriceLine's reportQuote
									// modePriceLine's reportPrice
									// modePriceLine's reportMinute
									// modePriceLine's reportBuySellEx
									// modePriceLine's reportBigTrade
	int			m_nCurrentSelectPos;// ��ǰѡ�е�ĳһ������ϸ������m_aReportBigTrade�е�λ��ָ��
									// �����󵥳ɽ���ϸ(modeBigTradeDetail��reportBigTrade)ʹ��

	double		m_dMaxPrice;		// modePriceLine ��ǰ�۸��ߵ����ֵ
	double		m_dMinPrice;		// modePriceLine ��ǰ�۸��ߵ����ֵ
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
