// SplashWndThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "emule.h"
#include "SplashWndThread.h"
#include "SplashScreen.h"


// CSplashWndThread

IMPLEMENT_DYNCREATE(CSplashWndThread, CWinThread)

CSplashWndThread::CSplashWndThread()
{
}

CSplashWndThread::~CSplashWndThread()
{
}

BOOL CSplashWndThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	m_splashScreen.Create(m_splashScreen.IDD, CWnd::GetDesktopWindow());
	m_splashScreen.ShowWindow(SW_SHOW);

	return TRUE;
}

int CSplashWndThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSplashWndThread, CWinThread)
END_MESSAGE_MAP()


// CSplashWndThread ��Ϣ�������
