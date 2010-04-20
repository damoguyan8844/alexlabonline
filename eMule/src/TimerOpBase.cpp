// TimerOpBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimerOpBase.h"


// CTimerOpBase

IMPLEMENT_DYNAMIC(CTimerOpBase, CWnd)
CTimerOpBase::CTimerOpBase()
{
	m_wParam		= 0;
	m_lParam		= 0;
	m_uLeftTimes	= 0;

	CreateEx(0, _T("static"), NULL, 0, CRect(0, 0, 0, 0), NULL, 0);
}

CTimerOpBase::~CTimerOpBase()
{
	Stop();
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CTimerOpBase, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CTimerOpBase ��Ϣ�������

BOOL CTimerOpBase::Start(UINT uInterval, WPARAM wParam, LPARAM lParam, UINT uTimes)
{
	m_wParam		= wParam;
	m_lParam		= lParam;
	m_uLeftTimes	= uTimes;

	SetTimer(1, uInterval, NULL);

	return TRUE;
}

void CTimerOpBase::Stop()
{
	KillTimer(1);

	m_wParam		= 0;
	m_lParam		= 0;
	m_uLeftTimes	= 0;
}

void CTimerOpBase::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		if (0 != m_uLeftTimes)	// �� 0 == m_uLeftTimes ʱ����ʾ�ö�ʱ��һֱѭ����ȥ��
		{
			m_uLeftTimes--;
			if (0 == m_uLeftTimes)
				KillTimer(1);
		}
		
		TimerOp(m_wParam, m_lParam);
	}

	CWnd::OnTimer(nIDEvent);
}
