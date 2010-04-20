#pragma once

#include "EmuleUpdater.h"

//////////// Updater.exe ����ֵ/////////////////////////////////////////////
const int RESULT_NEWVERSION		= 100;

const int ERROR_UNKNOWN			= 0;	// δ֪����
const int ERROR_NOCONNECTION	= 1;	// ������
const int ERROR_SERVER			= 2;	// �޷����ӵ�������

const int ERROR_CHECKFAIL		= 3;	// У��ʧ��
const int ERROR_MEMNOTCREATE	= 4;	// �����ڴ洴��ʧ��
const int ERROR_MEMNOTOPEN		= 5;	// �����ڴ��ʧ��
const int ERROR_MEMNOTMAP		= 6;	// �����ڴ�ӳ��ʧ��
const int ERROR_WRITEMEM		= 7;	// �޷�д�빲���ڴ�

const int ERROR_NONEWVERSION	= 8;	// û���°汾����
const int ERROR_LOADFAIL		= 9;	// Loadʧ��
////////////////////////////////////////////////////////////////////////////

// CUpdaterThread

class CUpdaterThread : public CWinThread
{
	DECLARE_DYNCREATE(CUpdaterThread)

protected:
	CUpdaterThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

public:
	virtual ~CUpdaterThread();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void SetParent(CDialog* pParentThread);

public:
	CString		m_strFileToDownloadInto;
	CEmuleUpdater		m_updater;

protected:
	int m_iResult;
	CDialog* m_pParent;
protected:
	DECLARE_MESSAGE_MAP()
	
	void PostMessageToParent(UINT message, WPARAM wParam, LPARAM lParam);
};