#pragma once



// CSplashWndThread

class CSplashWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CSplashWndThread)

protected:
	CSplashWndThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSplashWndThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	CSplashScreen		m_splashScreen;
protected:
	DECLARE_MESSAGE_MAP()
};


