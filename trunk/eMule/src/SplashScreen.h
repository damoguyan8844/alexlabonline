#pragma once

#include "enbitmap.h"
#include "CxImage/xImage.h" // Added by thilon on 2006.08.01,���ڼ���PNGͼƬ

class CSplashScreen : public CDialog
{
	DECLARE_DYNAMIC(CSplashScreen)

public:
	CSplashScreen(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashScreen();

// Dialog Data
	enum { IDD = IDD_SPLASH };

public:
	HBITMAP CopyScreenToBitmap(CRect& rect);// Added by thilon on 2006.08.01
protected:
	CBitmap m_imgSplash;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	void OnPaint(); 

protected:
	HBITMAP		m_hbm; 
	 BITMAP		m_bitmap;
	 CRect		rect;

	 CxImage*   m_image; // Added by thilon on 2006.08.01
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
