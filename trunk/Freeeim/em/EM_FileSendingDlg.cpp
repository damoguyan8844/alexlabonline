// FileSendingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../EM_MsgDlg.h"
#include "stdemdef.h"
#include "EM_FileSendingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FileSendingDlg dialog

//DWORD FileSendingDlg::m_dwTimer[5] = {11,22,33,44,55};

EM_FileSendingDlg::EM_FileSendingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EM_FileSendingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FileSendingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwCurrent = -1;
	m_dwX = m_dwY = -1;
	m_dwFileLength = 0;
	m_dwSendLength = 0;
	m_bRunning = TRUE;
	m_fDuration = 0.00;
	m_bSendingDlg = TRUE;
}

void EM_FileSendingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FileSendingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_PROGRESS, m_percent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(EM_FileSendingDlg, CDialog)
	//{{AFX_MSG_MAP(FileSendingDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_FILERECEIVINGCOMPLETE, FileReceivingComplete)
	ON_MESSAGE(WM_FILESENDINGCOMPLETE, FileSendingComplete)
//	ON_MESSAGE(WM_FILESENDINGFAILED, FileSendingFailed)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FileSendingDlg message handlers

void EM_FileSendingDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (11 == nIDEvent)
	if ((m_dwSendLength !=0) && (m_dwFileLength != 0))
	{
		float ddd = float(m_dwSendLength / m_dwFileLength);
		float aa = float(m_dwSendLength);
		float bb = float(m_dwFileLength);
		//ddd = (70000 / 17918205);
		//ddd = 3/4*100;
		ddd = aa /bb * 100;
		//ddd = 0.222;
		m_percent.SetPos((int)ddd);
		CString str;
		float filelen = float(m_dwFileLength);
		filelen /= 1024;
		filelen /= 1024;
		float sentlen = float(m_dwSendLength);
		sentlen /= 1024;
		sentlen /= 1024;
		str.Format(_T("%.2fMB/%.2fMB"), sentlen, filelen);
	//	str.Format(_T("%.2fMB/%.2fMB-%d"), sentlen, filelen, m_dwCurrent);
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(str);
		str.Format(_T("%.1f%%"), ddd);
		GetDlgItem(IDC_STATIC_PERCENT)->SetWindowText(str);
		if (m_dwSendLength == m_dwFileLength)
		{
			KillTimer(nIDEvent);
			m_percent.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_SHOW);
		}
	}
	if (22 == nIDEvent)
	{
		GetParent()->PostMessage(WM_UPDATEDLG, (WPARAM)m_dwCurrent, (LPARAM)m_bSendingDlg);
		KillTimer(nIDEvent);
	}
	CDialog::OnTimer(nIDEvent);
}

DWORD EM_FileSendingDlg::FileSendingProc(LPVOID lParam)
{
	BOOL bRet;
	CFile file;
	EM_FileSendingDlg *d = (EM_FileSendingDlg*)lParam;
    SOCKET        sClient;
	SOCKADDR_IN server;
	int total = 0;
	byte buf[FILE_BUFFER_LENGTH];

	clock_t start, finish;
	/* 测量一个事件持续的时间*/
	int readlen;
	int ret;

	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sClient)
	{
		AfxMessageBox("socket() failed.");
		return 1;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(5150);
	server.sin_addr.s_addr = inet_addr(d->m_szUserIP);
	if (SOCKET_ERROR == connect(sClient, (PSOCKADDR)&server, sizeof(server)))
	{
		AfxMessageBox("connect() failed.");
		return 1;
	}

	bRet = file.Open(d->m_szFileName, CFile::modeRead|CFile::shareDenyNone, NULL);
	if (! bRet)
	{
		AfxMessageBox("File opening error.");
		return 1;
	}
	d->m_dwFileLength = file.GetLength();
//	::MessageBox(NULL,(char*)d->m_fileInfo.FileName(),0,0);
	ret = send(sClient, (char*)&(d->m_fileInfo), sizeof(EM_FILEINFO), 0);
	if (SOCKET_ERROR == ret)
	{
		file.Close();
		AfxMessageBox("Data sending failed.");
		return FALSE;
	}

	start = clock();

	readlen=file.Read(buf, FILE_BUFFER_LENGTH);
	while(readlen > 0)
	{
		if (! d->m_bRunning)
			break;
		ret = send(sClient, (char*)buf, readlen, 0);
		if (SOCKET_ERROR == ret)
		{
		//	AfxMessageBox("Data sending failed.");
		//	d->PostMessage(WM_FILESENDINGFAILED);
			break;
		}
		d->m_dwSendLength += readlen;
		readlen = file.Read(buf, FILE_BUFFER_LENGTH);
	}
	shutdown(sClient, SD_SEND);
	closesocket(sClient);
	file.Close();

	finish = clock();
	double f1, f2;
	f1 = (double)finish;
	f2 = (double)start;
	d->m_fDuration = (f1-f2) / CLOCKS_PER_SEC;
	Sleep(88);
	d->SendMessage(WM_FILESENDINGCOMPLETE);

	// 等待它处理一些其他事
	return 1;
}

DWORD EM_FileSendingDlg::FileReceivingProc(LPVOID lParam)
{
	EM_FileSendingDlg *pDlg = (EM_FileSendingDlg *)lParam;
	byte buf[FILE_BUFFER_LENGTH];
	static int random_value = 0;
	SOCKET sock = pDlg->Sock;
	BOOL exitLoop = FALSE;
	int ret;
	int total = 0;
	CFile file;
//	char random_file_name[64];
	clock_t start, finish;

	EM_FILEINFO finfo;
	ret = recv(sock, (char*)&finfo, sizeof(EM_FILEINFO), 0);
	memcpy((char*)&(pDlg->m_fileInfo), (char*)&finfo, sizeof(EM_FILEINFO));
	pDlg->m_dwFileLength = finfo.dwSize;
	strcpy(pDlg->m_szFileExt, finfo.FileExt());
	strcpy(pDlg->m_szFileName, finfo.FileFullName());
	strcpy(pDlg->m_szFileTitle, finfo.FileName());
	pDlg->GetDlgItem(IDC_STATIC_FILE_NAME)->SetWindowText(pDlg->m_szFileTitle);
	SHFILEINFO shfi;
	SHGetFileInfo(pDlg->m_szFileTitle,FILE_ATTRIBUTE_NORMAL,&shfi,
		sizeof(shfi),SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);
	if (NULL != shfi.hIcon)
	{
		pDlg->GetDlgItem(IDC_STATIC_ICON)->SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)shfi.hIcon);
	}
//	sprintf(random_file_name, "e:\\testfile%d.zip", random_value++);
//	MessageBox(0,"The saving file name is %s.\n", 0,0);
	BOOL bRet = file.Open(pDlg->m_szFileName, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL);
	if (! bRet)
	{
		::MessageBox(0,"EMFileClient::RecvFile [file.opne] failed.",0,0);
	//	file.Close();
		closesocket(sock);
		pDlg->PostMessage(WM_FILERECEIVINGCOMPLETE);
		exitLoop = TRUE;
		return -1;
	}

/*	CString str;
	str.Format("%d\n", finfo.dwSendFileIndex);
	::MessageBox(0,str,0,0);*/

	start = clock();
	while (! exitLoop)
	{
		if (! pDlg->m_bRunning)
			break;
		ret = recv(sock, (char*)&buf, FILE_BUFFER_LENGTH, 0);
		if (0 == ret) // Graceful close
		{
		//	::MessageBox(0,"Client disconnected.\n",0,0);
			break;
		}

		else if (SOCKET_ERROR == ret)
		{
			::MessageBox(0,"recv() failed:%d\n",0,0);
			break;
		}
		else
		{
			pDlg->m_dwSendLength += ret;
			file.Write(buf, ret);
		}
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	file.Close();

	finish = clock();

	double f1, f2;
	f1 = (double)finish;
	f2 = (double)start;

	pDlg->m_fDuration = (f1-f2) / CLOCKS_PER_SEC;
	Sleep(88);
	pDlg->SendMessage(WM_FILERECEIVINGCOMPLETE);

	// 等待它处理一些其他事
	return 1;
}

void EM_FileSendingDlg::DoInitialize(DWORD dwIndex, CRect rt, DWORD dwY)
{
	GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_HIDE);
	m_dwX = rt.right-186;
	m_dwY = dwY;
	MoveWindow(m_dwX, m_dwY, 179, FILE_SENDING_DLG_LEN);
	ShowWindow(SW_SHOW);
	m_dwCurrent = dwIndex;
}

void EM_FileSendingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bRunning = FALSE;
	KillTimer(11);
//	FileReceivingComplete();
//	GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_STATIC_COMPLETED)->SetWindowText(_T("传输被终止！"));
//	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
//	m_percent.ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_STATIC_PROMPT)->ShowWindow(SW_HIDE);
//	SetTimer(22, 1888, NULL);
}
// 文件接收完成
void EM_FileSendingDlg::FileReceivingComplete(WPARAM wParam, LPARAM lParam)
{
	int i = m_fileInfo.dwRecvFileIndex;
	EM_MsgDlg *pDlg = (EM_MsgDlg*)GetParent();

	if (m_dwFileLength != m_dwSendLength)
	{
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COMPLETED)->SetWindowText(_T("接受失败！"));
		GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_SHOW);
		m_percent.ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PROMPT)->ShowWindow(SW_HIDE);

		pDlg->File_RecvFailed(i);
	}
	else
	{
		char buf[128];
		sprintf(buf, "使用时间:%.2f 秒", m_fDuration);
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(buf);
		pDlg->File_RecvComplete(i);
	}
	SetTimer(22, 3200, NULL);
}

// 文件发送完成
void EM_FileSendingDlg::FileSendingComplete(WPARAM wParam, LPARAM lParam)
{
	EM_MsgDlg *pDlg = (EM_MsgDlg*)GetParent();
	int i = m_fileInfo.dwSendFileIndex;

	if (m_dwFileLength != m_dwSendLength)
	{
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COMPLETED)->SetWindowText(_T("发送失败！"));
		GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_SHOW);
		m_percent.ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PROMPT)->ShowWindow(SW_HIDE);
		pDlg->File_SendFailed(i);
	}
	else
	{
		char buf[128];
		sprintf(buf, "使用时间:%.2f 秒", m_fDuration);
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(buf);
		pDlg->File_SendComplete(i);
	}
	SetTimer(22, 3888, NULL);
}
/*
void EM_FileSendingDlg::FileSendingFailed(WPARAM wParam, LPARAM lParam)
{
	if (m_dwFileLength != m_dwSendLength)
	{
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COMPLETED)->SetWindowText(_T("发送失败！"));
		GetDlgItem(IDC_STATIC_COMPLETED)->ShowWindow(SW_SHOW);
		m_percent.ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}
	SetTimer(22, 1888, NULL);
}*/

void EM_FileSendingDlg::AdjustDlgRect()
{
	m_dwY -= FILE_SENDING_DLG_LEN;
	m_dwCurrent -= 1;
	MoveWindow(m_dwX, m_dwY, 179, FILE_SENDING_DLG_LEN);
}

HBRUSH EM_FileSendingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if(CTLCOLOR_DLG  ==nCtlColor)
        return m_Brush;
	else if(CTLCOLOR_STATIC == nCtlColor)
    {
        pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crStatic);
        return m_Brush;
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void EM_FileSendingDlg::BeginToReceive(SOCKET &sock)
{
	// 标志成 接收
	m_bSendingDlg = FALSE;

	GetDlgItem(IDC_BUTTON_DENY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SAVE_AS)->ShowWindow(SW_HIDE);
	m_Brush.CreateSolidBrush(RGB(236,243,246));
	m_crStatic = RGB(99,97,78);
	Sock = sock;
	HANDLE hThread;
	DWORD dwTID = 0;
	hThread = CreateThread(NULL, 0, FileReceivingProc, (LPVOID)this, 0, &dwTID);
	if (NULL == hThread)
	{
		::MessageBox(0,"CreateThread() failed.",0,0);
		return;
	}
	else
	{
		CloseHandle(hThread);
		SetTimer(11, 1, NULL);
	}
}

void EM_FileSendingDlg::BeginToSend(LPEM_FILEINFO fInfo) 
{
//	memcpy(&m_fileInfo, fInfo, sizeof(EM_FILEINFO));
	m_Brush.CreateSolidBrush(RGB(250,250,250));
	m_crStatic = RGB(0,136,228);
	ASSERT(m_dwCurrent != -1);
	strcpy(m_szFileName, fInfo->FileFullName());
	strcpy(m_szFileTitle, fInfo->FileName());
	strcpy(m_szFileExt, fInfo->FileExt());

	//	MessageBox("你所打开的文件是："+(CString)dlgFileOpen.m_ofn.lpstrFile);
	// Begin Thread
	GetDlgItem(IDC_STATIC_FILE_NAME)->SetWindowText(m_szFileTitle);

	SHFILEINFO shfi;
	SHGetFileInfo(m_szFileName,FILE_ATTRIBUTE_NORMAL,&shfi,
		sizeof(shfi),SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);

	if (NULL != shfi.hIcon)
	{
		GetDlgItem(IDC_STATIC_ICON)->SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)shfi.hIcon);
	}
	else
	{
		MessageBox("Could not retrieve the file icon.");
	}
	DWORD dwTID = 0;
	HANDLE hThread = CreateThread(NULL, 0, FileSendingProc, this, 0, &dwTID);
	if (NULL == hThread)
	{
		MessageBox("Thread creating error.\r\nFile could not be send.");
	}
	else
	{
		CloseHandle(hThread);
	}
	SetTimer(11, 1, NULL);
/*	else
	MessageBox("打开文件出错！");*/
}