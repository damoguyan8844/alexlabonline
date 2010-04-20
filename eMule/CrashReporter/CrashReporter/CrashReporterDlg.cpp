// CrashReporterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <io.h>
#include "CrashReporter.h"
#include "CrashReporterDlg.h"
#include ".\crashreporterdlg.h"
#include "HttpUploadFileProc.h"
#include "WinMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCrashReporterDlg �Ի���



CCrashReporterDlg::CCrashReporterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashReporterDlg::IDD, pParent)
	, m_bAutoRestart(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//m_hProcThread = INVALID_HANDLE_VALUE;
	m_pUploadProc = NULL;
	m_bTinyWndSize = TRUE;
	m_uAutoCloseTimer = 0;
}

void CCrashReporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_STATUS, m_sttcProgrsStatus);
	DDX_Control(pDX, IDC_PROGRESS_SEND, m_progressSend);
	DDX_Control(pDX, ID_BNCANCEL, m_bnCancel);
	DDX_Control(pDX, IDOK, m_bnOk);
	DDX_Control(pDX, IDC_EDIT_DESC, m_editDesc);
	DDX_Control(pDX, IDC_STATIC_TINY_BOUNDARY, m_sttcTinyBoundary);
	DDX_Control(pDX, IDC_STATIC_LARGE_BOUNDARY, m_sttcLargeBoundary);
	DDX_Check(pDX, IDC_CHECK_AUTO_RESTART, m_bAutoRestart);
}

BEGIN_MESSAGE_MAP(CCrashReporterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_UPLOAD_PROGRESS, OnUploadProgress)

	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(ID_BNCANCEL, OnBnClickedBncancel)
	ON_EN_MAXTEXT(IDC_EDIT_DESC, OnEnMaxtextEditDesc)
	ON_EN_MAXTEXT(IDC_EDIT_EMAIL, OnEnMaxtextEditEmail)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MORE_INFO, OnBnClickedMoreInfo)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CCrashReporterDlg::SetProcStatus(int iStatus)
{
	switch(iStatus)
	{
	case PS_NOT_SEND:
		m_sttcProgrsStatus.SetWindowText(_T(""));
		m_sttcProgrsStatus.ShowWindow(SW_HIDE);
		//m_progressSend.ShowWindow(SW_HIDE);
		m_progressSend.SetPos(0);
		m_bnOk.EnableWindow(TRUE);
		m_bnCancel.SetWindowText(_T("������"));
		break;
	case PS_SENDING:
		m_sttcProgrsStatus.ShowWindow(SW_SHOW);
		//m_progressSend.ShowWindow(SW_SHOW);
		m_bnOk.EnableWindow(FALSE);
		m_bnCancel.SetWindowText(_T("ȡ��"));
		break;
	case PS_SUCCEEDED:
		m_sttcProgrsStatus.ShowWindow(SW_SHOW);
		//m_progressSend.ShowWindow(SW_SHOW);
		m_bnOk.EnableWindow(FALSE);
		m_bnCancel.SetWindowText(_T("�ر�"));
		break;
	default:
		break;
	}

	m_iProcStatus = iStatus;
}

void CCrashReporterDlg::LimitPrompt(UINT uLimit)
{
	CString	str;
	str.Format(_T("���ֻ������%d��Ӣ���ַ���%d�������ַ���"), uLimit, uLimit/2);
	::AfxMessageBox(str);
}

void CCrashReporterDlg::ChangeWndSize(BOOL bTiny)
{
	CRect	rtBoundary;
	if (bTiny)
		m_sttcTinyBoundary.GetWindowRect(&rtBoundary);
	else
		m_sttcLargeBoundary.GetWindowRect(&rtBoundary);

	CRect	rtWnd;
	GetWindowRect(&rtWnd);

	SetWindowPos(NULL, 0, 0, rtWnd.Width(), rtBoundary.top - rtWnd.top, SWP_NOZORDER | SWP_NOMOVE);

	//	�����tinyģʽ�����ʾ�༭���Ա���Tab��������Щ���ϡ�
	m_editDesc.EnableWindow(!bTiny);	
	m_editEmail.EnableWindow(!bTiny);
}

void CCrashReporterDlg::TriggerAutoClose()
{
	CString	strIniPathFile;
	strIniPathFile = _T("config\\preferences.ini");

	if (-1 == _taccess(strIniPathFile, 04))
		return;

	m_uRestartCount = GetPrivateProfileInt(_T("CrashReporter"), _T("RestartCount"), 0, strIniPathFile);
	time_t timeLastRestart = GetPrivateProfileInt(_T("CrashReporter"), _T("LastRestartTime"), 0, strIniPathFile);
	time_t timeCur = time(NULL);

	if (timeCur - timeLastRestart > 24*60*60)
		m_uRestartCount = 0;

	if (m_uRestartCount < 5)
	{
		m_uAutoCloseTimer = 1;
		SetTimer(m_uAutoCloseTimer, 3*60*1000, NULL);	//	5���Ӻ��Զ��رճ�������eMule����ֹ��eMuleʱcrashû�˹ܣ�
	}
}

void CCrashReporterDlg::SaveAutoCloseIni()
{
	CString	strIniPathFile;
	strIniPathFile = _T("config\\preferences.ini");

	CString strValue;
	strValue.Format(_T("%d"), m_uRestartCount);
	WritePrivateProfileString(_T("CrashReporter"), _T("RestartCount"), strValue, strIniPathFile);
	strValue.Format(_T("%d"), time(NULL));
	WritePrivateProfileString(_T("CrashReporter"), _T("LastRestartTime"), strValue, strIniPathFile);
}

void CCrashReporterDlg::StopAutoCloseTimer()
{
	if (0 != m_uAutoCloseTimer)
	{
		KillTimer(m_uAutoCloseTimer);
		m_uAutoCloseTimer = 0;
	}
}


// CCrashReporterDlg ��Ϣ�������

BOOL CCrashReporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_bAutoRestart = TRUE;
	UpdateData(FALSE);

	ChangeWndSize(m_bTinyWndSize);
	m_editDesc.SetLimitText(MAX_DESC - 10);
	m_editEmail.SetLimitText(MAX_PATH - 10);

	SetProcStatus(PS_NOT_SEND);

	TriggerAutoClose();	
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CCrashReporterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCrashReporterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CCrashReporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCrashReporterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();

	if (PS_NOT_SEND != GetProcStatus())
		return;

	SetProcStatus(PS_SENDING);

	CString strEmail;
	m_editEmail.GetWindowText(strEmail);
	CString strDesc;
	m_editDesc.GetWindowText(strDesc);


	m_pUploadProc = (CHttpUploadFileProc*) ::AfxBeginThread(RUNTIME_CLASS(CHttpUploadFileProc),
																		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	ZeroMemory(&m_pUploadProc->m_threadParam, sizeof(SHttpUploadFileThreadParam));
	_tcsncpy(m_pUploadProc->m_threadParam.szFileName, m_strFileName, MAX_PATH - 1);
	m_pUploadProc->m_threadParam.szFileName[MAX_PATH - 1] = _T('\0');
	m_pUploadProc->m_threadParam.hNotifyWnd = GetSafeHwnd();
	m_pUploadProc->m_threadParam.uNotifyMsg = WM_UPLOAD_PROGRESS;
	_tcsncpy(m_pUploadProc->m_threadParam.szEmail, strEmail, MAX_PATH - 1);
	m_pUploadProc->m_threadParam.szEmail[MAX_PATH - 1] = _T('\0');
	_tcsncpy(m_pUploadProc->m_threadParam.szDesc, strDesc, MAX_DESC - 1);
	m_pUploadProc->m_threadParam.szDesc[MAX_DESC - 1] = _T('\0');

	m_pUploadProc->ResumeThread();
}

LRESULT CCrashReporterDlg::OnUploadProgress(WPARAM wParam, LPARAM lParam)
{
	if (PS_SENDING != GetProcStatus())
		return 0;

	HRESULT hr = (HRESULT) wParam;

	if (SUCCEEDED(hr))
	{
		if (P_HUF_PROGRESS == hr)
		{
			m_progressSend.SetPos((int) lParam);
		}
		else
		{
			m_sttcProgrsStatus.SetWindowText(CHttpUploadFileProc::Result2Str(hr));
		}
	}

	if (S_HUF_FINISHED == hr)
	{
		SetProcStatus(PS_SUCCEEDED);
		//m_hProcThread = NULL;
		m_pUploadProc = NULL;
		::AfxMessageBox(_T("�ѳɹ����ʹ��󱨸档��л����֧�֣�"));
		OnCancel();
	}

	if (FAILED(hr))
	{
		//m_hProcThread = NULL;
		m_pUploadProc = NULL;
		SetProcStatus(PS_NOT_SEND);
		CString	strErrorInfo;
		strErrorInfo = CHttpUploadFileProc::Result2Str(hr);
		m_sttcProgrsStatus.SetWindowText(strErrorInfo);
		::AfxMessageBox(strErrorInfo);
	}

	return 0;
}

void CCrashReporterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCancel();
}

void CCrashReporterDlg::OnBnClickedBncancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (PS_SENDING == GetProcStatus())
	{
		if (!::IsBadReadPtr(m_pUploadProc, sizeof(CHttpUploadFileProc)))
			m_pUploadProc->SuspendThread();

		if (IDYES == ::AfxMessageBox(_T("ȡ�����ͣ�"), MB_YESNO))
		{
			if (!::IsBadReadPtr(m_pUploadProc, sizeof(CHttpUploadFileProc)))
			{
				m_pUploadProc->Terminate();
			}
			SetProcStatus(PS_NOT_SEND);
		}

		if (!::IsBadReadPtr(m_pUploadProc, sizeof(CHttpUploadFileProc)))
			m_pUploadProc->ResumeThread();

	}
	else
	{
		OnCancel();
	}
}

void CCrashReporterDlg::OnEnMaxtextEditDesc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LimitPrompt(MAX_DESC - 10);
}

void CCrashReporterDlg::OnEnMaxtextEditEmail()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LimitPrompt(MAX_PATH - 10);
}



void CCrashReporterDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	OnBnClickedBncancel();
	//CDialog::OnClose();
}

void CCrashReporterDlg::OnBnClickedMoreInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bTinyWndSize = !m_bTinyWndSize;
	ChangeWndSize(m_bTinyWndSize);
}

void CCrashReporterDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	StopAutoCloseTimer();
	UpdateData();
}

void CCrashReporterDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_uAutoCloseTimer == nIDEvent)
	{
		StopAutoCloseTimer();
		PostQuitMessage(0);

		m_uRestartCount++;
		SaveAutoCloseIni();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CCrashReporterDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	StopAutoCloseTimer();

	return CDialog::OnCommand(wParam, lParam);
}
