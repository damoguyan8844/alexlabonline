// StockGraph.h : interface of the CStockGraph class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCKGRAPH_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_STOCKGRAPH_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// һЩ��ͼ���ô���ĺ궨��
#define		CHECK_NODATATODRAW			if( -1 == m_nIndexStart || -1 == m_nIndexEnd || m_nIndexStart > m_nIndexEnd ) return;
#define		DECLARE_COLOR_DEFINATION		\
	COLORREF	clrBK		=	AfxGetProfile().GetColor(CColorClass::clrGraphBK);			\
	COLORREF	clrBorder	=	AfxGetProfile().GetColor(CColorClass::clrBorder);			\
	COLORREF	clrRise		=	AfxGetProfile().GetColor(CColorClass::clrRise);				\
	COLORREF	clrFall		=	AfxGetProfile().GetColor(CColorClass::clrFall);				\
	COLORREF	clrFallEntity=	AfxGetProfile().GetColor(CColorClass::clrFallEntity);		\
	COLORREF	clrPlane	=	AfxGetProfile().GetColor(CColorClass::clrPlane);			\
	COLORREF	clrNewKLine	=	AfxGetProfile().GetColor(CColorClass::clrNewKLine);			\
	COLORREF	clrCW		=	AfxGetProfile().GetColor(CColorClass::clrCW);				\
	COLORREF	clrDJ		=	AfxGetProfile().GetColor(CColorClass::clrDJ);				\
	COLORREF	clrTitle	=	AfxGetProfile().GetColor(CColorClass::clrTitle);			\
	COLORREF	clrText		=	AfxGetProfile().GetColor(CColorClass::clrText);				\

typedef	CArray <CRect, CRect &>	CRectArray;

// Some Usefull Functions
void DrawLine( CDC * pDC, int nWidth, COLORREF clr, int xStart, int yStart, int xEnd, int yEnd );
void DrawDashLine( CDC * pDC, int nWidth, COLORREF clr, int xStart, int yStart, int xEnd, int yEnd );

void DrawDashAxis( CDC * pDC, int nWidth, COLORREF clr, int nCount, int xStart, int yStart, int xEnd, int yEnd );
void DrawAxisNumber(CDC * pDC, COLORREF clr, int nCount,
					int xStart, int yStart, int xEnd, int yEnd,
					double fMin, double fMax, BOOL bDrawMin, BOOL bDrawMax, int nDotCount );

BOOL DrawAxis(	CDC * pDC, CRect rect, int nMarginTop, int nAxisLines,
				double dMin, double dMax, BOOL bDrawMin, BOOL bDrawMax, int nDotCount );
BOOL DrawTechTitle(CDC * pDC, int x, int y, LPCTSTR lpszText,
				   UINT nTextAlign, int nFontSize, COLORREF clrBK, COLORREF clrText );

BOOL FixRect( CRect & rect, LPRECT lpRectMax );
BOOL DrawTextWithRect(CDC *pDC, int left, int top, int right, int bottom, LPCTSTR lpszText,
					  COLORREF clrRect, COLORREF clrText, COLORREF clrBK, LPRECT lpRectMax );
BOOL DrawTextWithRect(CDC *pDC, int left, int top, int right, int bottom, UINT nIDString,
					  COLORREF clrRect, COLORREF clrText, COLORREF clrBK, LPRECT lpRectMax );

/***
	��ͼ�࣬���𻭳�����ָ����ͼ�е����в���
*/
class CStockGraph
{
public:
	CStockGraph( );
	virtual	~CStockGraph( );
	void		SetParent( CWnd * pParent );
	void		ResetMargins(	int nMarginTop = 19, int nMarginBottom = 18,
								int nHeightSubtitle = 15, int nWidthSubtitle = 70,
								int nWidthParameter = 100, int nDefaultThickness = 6 );

	enum	klineMode	{
		klineCandle			=	0x01,	// ����K��
		klineAmerica		=	0x02,	// ������
		klineTower			=	0x03,	// ������
	};
	enum	sg_drawtech	{
		drawtechtype_line	=	0x01,	// ��������ʹ��ʵ��
		drawtechtype_dot	=	0x02,	// ��������ʹ������
	};
	enum	ReportWhat	{
		reportParam			=	0x01,	// ����
		reportCost			=	0x02,	// �ɱ�
		reportFlame			=	0x03,	// ����ɽ
		reportActivity		=	0x04,	// ��Ծ��
	};

	// ˢ��ʵʱ��������
	LRESULT		OnStkReceiverData( WPARAM wParam, LPARAM lParam );

	int			GetKType( );
	void		SetKType( int ktype = CKData::ktypeDay );
	int			GetKFormat( );
	void		SetKFormat( int kformat = CKData::formatOriginal );
	int			GetMaindataType( );
	void		SetMaindataType( int maindatatype = CKData::mdtypeClose );
	int			GetKLineMode( );
	void		SetKLineMode( int klineMode = CStockGraph::klineCandle );
	CRect &		GetKDataRect( );
	void		ResetIndexCurrent( int nIndexCurrent = -1 );
	int			GetIndexCurrent() { return m_nIndexCurrent; }
	void		ResetIndex( int nIndexStart = -1, int nIndexEnd = -1, int nIndexCurrent = -1 );
	void		ResetClient( CRect rectAll );
	void		ClearTechniqueCache( );
	void		ClearCurStock( );
	BOOL		SetCurStock( const char * szCode );
	BOOL		SetCurStock( CStockInfo & info );
	void		SetReportWhat( int nReportWhat );
	BOOL		PrepareStockData( BOOL bReload = FALSE );
	void		Redraw( CDC * pDC, CRect rectAll );
	void		DrawReportRegion( CDC * pDC );

	int			GetNextThickness( int nCurrentThickness, BOOL bAsc );
	void		ExpandUp( BOOL bShiftPressed = FALSE );
	void		ExpandDown( BOOL bShiftPressed = FALSE );
	void		MoveLeft( BOOL bShiftPressed = FALSE );
	void		MoveRight( BOOL bShiftPressed = FALSE );
	void		PageLeft( BOOL bShiftPressed = FALSE );
	void		PageRight( BOOL bShiftPressed = FALSE );
	void		MoveHome( BOOL bShiftPressed = FALSE );
	void		MoveEnd( BOOL bShiftPressed = FALSE );
	void		OnInsert( );
	void		OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnLButtonUp( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnMouseMove( UINT nFlags, CPoint point );
	void		OnRButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnReturn( BOOL bShiftPressed, BOOL bCtrlPressed );

	UINT		GetSelectedTech( );
	void		HideSelectedTech();
protected:
	// Move Operation
	void		MoveTo( int nIndex, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnIndexCurrentChanged( );
	void		Move( int nMove, BOOL bShiftPressed, BOOL bCtrlPressed );

	int			GetIndexClick( CPoint point );
	void		SelectTechID( CPoint point );
	BOOL		AddIndexCurrent( int nIndex );
	void		InvalidateCenter( );
	BOOL		GetOneKLineRect( int nIndex, LPRECT lpRect, long *pxEntityLeft, long *pxEntityRight, long *pxMedium );

	void		DrawStock( CDC * pDC );
	BOOL		PrepareDrawKLine( );
	void		DrawNoDataInfo( CDC * pDC );
	void		DrawDateAxis( CDC * pDC );
	void		DrawKLine( CDC * pDC );
	BOOL		GetKLineMinMaxInfo( int nStart, int nEnd, double * pdMin, double * pdMax );
	void		DrawOneKLine(CDC * pDC, int nIndexPos, int nIndexKS, CKData * pKData, double dMin, double dMax, BOOL bGreyed );
	void		DrawBorderMovingLine( CDC * pDC, CPoint point, BOOL bAddOrRemove );
	void		DrawSelectionLine( CDC * pDC, BOOL bAddOrRemove );
	void		DrawSelectionRect( CDC * pDC, CPoint ptBegin, CPoint ptEnd, BOOL bAddOrRemove );
	void		DrawKData( );
public:
	void		DrawKData( CDC * pDC, CRect rect );
protected:
	void		DrawTechLine( CDC * pDC );
	void		DrawKLineAttribute(CDC * pDC, UINT nTech, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechLine( CDC * pDC, UINT nTech, CRect rect, BOOL bDrawTitle );
	void		DrawReportParam( CDC * pDC );
	void		DrawReportCost( CDC * pDC );
	void		DrawReportFlame( CDC * pDC );
	void		DrawReportActivity( CDC * pDC );
	void		DrawReportCW( CDC * pDC, BOOL bRecent, int nEnd, int nDays, double dChangeHand, COLORREF clr, CRect rectDescript );

	// KLine Technical Attributes
	void		DrawTechMA( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechBBI( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechBOLL( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechPV( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechSAR( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechDJ( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	void		DrawTechCW( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax );
	
	// Technical Lines
	void		DrawTechMACD( CDC * pDC, CRect rect, BOOL bDrawTitle, int nTech = STT_MACD, CMACD * pmacd = NULL );
	void		DrawTechMIKE( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechPSY( CDC * pDC, CRect rect, BOOL bDrawTitle );
	
	void		DrawTechVOLUME( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechNVI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechPVI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechVR( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechVROC( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechOBV( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechMOBV( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechMFI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechVMACD( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechWVAD( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechEMV( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechVRSI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechNVRSI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechAD( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechCI( CDC * pDC, CRect rect, BOOL bDrawTitle );

	void		DrawTechKDJ( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechR( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechRSI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechBIAS( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechMTM( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechDMI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechROC( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechCCI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechCV( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechARBR( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechCR( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechOSC( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechUOS( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechMAOSC( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTech3_6BIAS( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechDPO( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechKST( CDC * pDC, CRect rect, BOOL bDrawTitle );
	
	void		DrawTechREI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechDMKI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechPCNT( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechHLC( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechCDP( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechASI( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechATR( CDC * pDC, CRect rect, BOOL bDrawTitle );

	void		DrawTechCYO( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechDCYO( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechHSL( CDC * pDC, CRect rect, BOOL bDrawTitle );
	void		DrawTechDPER( CDC * pDC, CRect rect, BOOL bDrawTitle );
	
	void		DrawTechUser( UINT nTech, CDC * pDC, CRect rect, BOOL bDrawTitle );

	// Some Usefull Functions
	void		DrawTechUtil( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData, CSPDWordArray & adwDays );
	void		DrawTechUtil1( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData, COLORREF clrLine, int nDrawTechType );
	void		DrawTechUtil2( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2,
							   CString strTitle1, CString strTitle2 );
	void		DrawTechUtil3( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3,
							   CString strTitle1, CString strTitle2, CString strTitle3 );
	void		DrawTechUtil4( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3, COLORREF clrLine4,
							   CString strTitle1, CString strTitle2, CString strTitle3, CString strTitle4 );
	void		DrawTechUtil5( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3, COLORREF clrLine4, COLORREF clrLine5,
							   CString strTitle1, CString strTitle2, CString strTitle3, CString strTitle4, CString strTitle5 );

protected:
	// ͼ��Ե�������С
	int		m_nMarginTop;			// �ϱ߱�Ե��С
	int		m_nMarginBottom;		// �ױ߱�Ե��С
	int		m_nHeightSubtitle;		// �ӱ���߶�
	int		m_nWidthSubtitle;		// �ӱ�����
	int		m_nWidthParameter;		// �ұ���Ϣ�����
	int		m_nDefaultThickness;	// K������ȱʡ���

protected:
	CWnd		*	m_pParent;	// �����ڣ�����ָ����ͼ
	CWnd		m_wndKData;		// ѡ����K��������ʾ�Ĵ���

	CRect		m_rectAll;			// ����Rect
	CStock		m_CurStock;			// ��ǰ��Ʊ
	CStock		m_CurStockDown;		// ��ǰ��Ʊ���¸�Ȩ�������
	DWORD		m_dwLatestDate;		// �������
	int			m_nCurKLineMode;	// K����״��see CStockGraph::klineMode
	int			m_nCurKType;		// ��ǰK�����ͣ����ߡ����ߡ����ߡ�5�����ߵ�
	int			m_nCurKFormat;		// ��ǰK��ģʽ������Ȩ���ϸ�Ȩ���¸�Ȩ
	int			m_nCurMaindataType;	// ��ǰ���������ͣ����̼ۡ����̼ۻ�ƽ����
	int			m_nIndexCurrent;	// ��ǰѡ���յ����
	CUIntArray	m_anIndexCurrent;	// ��ǰ���ѡ���յ����
	int			m_nIndexStart;		// ��ǰ��ʾ������һ�����ڵ����
	int			m_nIndexEnd;		// ��ǰ��ʾ������һ�����ڵ����
	int			m_nThickness;		// K�߿��
	int			m_nReportWhat;		// �Ҳ���ʾʲô��see CStockGraph::ReportWhat

	CRect		m_rectKData/*Left Top*/, m_rectKLine/* KLine */, m_rectKLineCenter,/*Real KLine, y is from m_dMinPrice to m_dMaxPrice*/
				m_rectReport/*Right*/;
	CRectArray	m_arcTech;	// ÿ��ָ���Rect
	CRect		m_rectCenter/*m_rectKLine+m_arcTech*/;

	// Mouse Move Selection Rect use
	CPoint		m_ptSelectBegin;
	CPoint		m_ptSelectEnd;

	CTechParameters	m_techparam;	// ָ�����
	CPtrArray	m_aptrTechUser;

	double		m_dMaxPrice;		// ��ǰK�ߵļ۸����ֵ
	double		m_dMinPrice;		// ��ǰK�ߵļ۸����ֵ

	// �϶��߿�ʹ�ñ���
	BOOL		m_bCursorBorder;	// ����Ƿ���Rect�߽磬Ϊ�ƶ���Сָ��AFX_IDC_VSPLITBAR
	CPoint		m_ptLastMovingLine;	// ��¼����ƶ���λ��
	int			m_nPosSelectBorder;	// ��¼��ǰ�ƶ�������һ��Border

	// ���ظ�ָ�ꡢ�޸ĸ�ָ�����ʹ�ñ���
	UINT		m_nSelectedTechID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKGRAPH_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
