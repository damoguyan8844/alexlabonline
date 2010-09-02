// EM_MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "em/stdemdef.h"
#include "EM_MsgDlg.h"
#include "EIM02Dlg.h"
#include "ImageDataObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EM_MsgDlg dialog


EM_MsgDlg::EM_MsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EM_MsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EM_MsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_CHATDLG);

	m_bSendingFile = FALSE;
	m_dwFileDlgLength = 8;
	k = 0;

	m_dwTotleDlg = 0;

	for (int i=0; i<6; i++)
	{
		m_pDlg[i] = NULL;
		m_RecvfileInfo[i] = NULL;
		m_SendfileInfo[i] = NULL;
	}
	m_dwRecvTotal = 0;
	m_dwSendTotal = 0;

	m_pRecvChat = NULL;
	m_pSendChat = NULL;

	m_pMainTop = NULL;
}


void EM_MsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EM_MsgDlg)
	DDX_Control(pDX, IDC_RICHEDIT2, m_richInput);
	DDX_Control(pDX, IDC_RICHEDIT1, m_RichEditShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EM_MsgDlg, CDialog)
	//{{AFX_MSG_MAP(EM_MsgDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnBtnSend)
	ON_BN_CLICKED(IDC_BUTTON1, OnBtnClose)
	ON_BN_CLICKED(IDC_SHAKE, OnBtnShake)
	ON_MESSAGE(WM_UPDATEDLG, OnUpdateDlg)
	ON_MESSAGE(WM_EMDONOTCHAT, Voice_OnDoNotChat)
	ON_MESSAGE(WM_EMCANCELREQUEST, Voice_CancelRequest)
	ON_MESSAGE(WM_SRCACCEPTCHAT, Voice_SrcAcceptChat)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnBtnSendFile)
	ON_BN_CLICKED(IDC_VOICECHAT, Voice_Request)
	ON_WM_TIMER()
	ON_WM_DROPFILES()
	ON_COMMAND_RANGE(ID_BUTTON32794, ID_BUTTON32809, OnFaceSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EM_MsgDlg message handlers

BOOL EM_MsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 顺序不能打乱
	_InitFaceToolbar();
	_InitHistoryToday();

	DragAcceptFiles();
	m_richInput.SetFocus();

	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();

	CString strTitle;
	strTitle.Format(_T("与 %s 的对话"), m_strDisplayName);
	SetWindowText(strTitle);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EM_MsgDlg::OnBtnSend()
{
	// TODO: Add your control notification handler code here
	CString strSend;
	m_richInput.GetWindowText(strSend);
	// 如果用户什么也没输入按发送,就提示输入
	if (strSend.IsEmpty())
	{
		// 考虑要不要提示，先直接返回
		m_richInput.SetFocus();
		return;
	}

	//获取RichEdit中有几个bitmap，这里假设RichEdit中有位图文件
	int c = m_richInput.GetIRichEditOle()->GetObjectCount();     

	int pos;
	DWORD dwUser;
	int nPos=0;
	CString strFace;
	for(int i = 0 ; i < c ; i++) //遍历位图
	{
		REOBJECT object;  //位图信息存在这里
		memset(&object,0,sizeof(REOBJECT));
		object.cbStruct = sizeof(REOBJECT);
		m_richInput.GetIRichEditOle()->GetObject(i,&object,REO_GETOBJ_ALL_INTERFACES);

		pos = object.cp ; //位图的位置信息
		dwUser = object.dwUser; //位图的信息，之前应用程序设置的，应有程序当然知道什么意思了
		strFace.Format("_FreeEIM_Emotion_%d", dwUser);
		strSend.Delete(pos+nPos,1);
		strSend.Insert(pos+nPos, strFace);
		nPos += strFace.GetLength()-1;
	}

	EM_DATA data;
	data.msg = EM_TEXT;
	data.buf = (LPSTR)(LPCTSTR)strSend;
	data.len = strSend.GetLength() + 1;

	// 是群发还是单独发
	if (0 == strcmp(m_strUserPCName, "asdf--->>>xxxxxxasiew*3234234*sdfsdf"))
	{
		m_pMainTop->SendMSGToAll(&data);
	}
	else
	{
	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	//	SetWindowText(m_szUserIP);
		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	}
	AddSendText((LPCTSTR)strSend);

	m_richInput.SetWindowText("");
	m_richInput.SetFocus();
}

void EM_MsgDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
//	ShowWindow(SW_HIDE);
	OnCancel();
}

// 给对方发送抖动窗口
void EM_MsgDlg::OnBtnShake()
{
	EM_DATA data;
	data.msg = EM_SHAKEWINDOW;
	data.buf = 0;
	data.len = 0;

	// 是群发还是单独发
	if (0 == strcmp(m_strUserPCName, "asdf--->>>xxxxxxasiew*3234234*sdfsdf"))
	{
		m_pMainTop->SendMSGToAll(&data);
	}
	else
	{
	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	}

	Voice_AddText(_T("你向对方发送了一个窗口抖动"));

	int ty=3;
	CRect   m_rect;   
	GetWindowRect(&m_rect);  
	int recordy=m_rect.left;
	int recordx=m_rect.top;

	for(int i=0;i<5;i++)
	{
		m_rect.left=recordy;
		m_rect.top=recordx;
		m_rect.top = m_rect.top + ty;
		m_rect.left = m_rect.left - ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.top = m_rect.top -ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.top = m_rect.top -2*ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.left=m_rect.left+ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.left=m_rect.left+2*ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.top = m_rect.top + ty;  
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
		Sleep(35);
		m_rect.top=m_rect.top+2*ty;
		SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE ); 
		SetWindowPos( NULL,recordy,recordx,0,0,SWP_NOSIZE );
		Sleep(35);
	}
}// End 给对方发送抖动窗口

BOOL EM_MsgDlg::IsCurrentUser(LPSTR hostName)
{
	if (strcmp(m_strUserPCName, hostName))
	{
		return FALSE;
	}
	return TRUE;
}

// 点击 同意接收文件
void EM_MsgDlg::File_ResAcceptFile(ENLINK *pLink)
{
	// 由于对话框太小，最多只能容纳 6 个文件传输对话框
	if (k >= 6)
	{
		MessageBox(_T("Beta 版最多只能处理6个文件！\r\n等其它文件处理完再继续。"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}



	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] != NULL)
		{
			int current = m_RecvfileInfo[i]->index1;
			if (current == pLink->chrg.cpMin)
			{
				CString strFileExt;
				strFileExt.Format("*.%s|*.%s|All Files(*.*)|*.*||", m_RecvfileInfo[i]->FileExt(), m_RecvfileInfo[i]->FileExt());

				CFileDialog dlgFileSaveAs(FALSE, NULL, m_RecvfileInfo[i]->FileName(),
					OFN_HIDEREADONLY |
					OFN_OVERWRITEPROMPT,
					strFileExt,this);

				int structsize=0;
				DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion;
				//检测目前的操作系统，GetVersion具体用法详见MSDN
				dwVersion = GetVersion();
				dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
				dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
				// 如果运行的操作系统是Windows NT/2000
				if (dwVersion < 0x80000000)
				structsize =88;//显示新的文件对话框
				else
				//运行的操作系统Windows 95/98
				structsize =76;//显示老的文件对话框
				dlgFileSaveAs.m_ofn.lStructSize=structsize;

				// 按了保存
				if (IDOK == dlgFileSaveAs.DoModal())
				{
					// 把 (Yes)是 变灰
					CHARFORMAT Cfm;
					m_RichEditShow.GetSelectionCharFormat(Cfm);
					Cfm.cbSize = sizeof(CHARFORMAT);
					Cfm.dwMask = CFM_LINK | CFM_COLOR;
					Cfm.dwEffects ^= CFE_LINK;
					Cfm.crTextColor = RGB(150,150,150);
					m_RichEditShow.SetSelectionCharFormat(Cfm);

					// 地位 (No)否 然后把他变灰
					m_RichEditShow.SetSel(pLink->chrg.cpMax+11, pLink->chrg.cpMax + 11 + 6);
					m_RichEditShow.SetSelectionCharFormat(Cfm);

					// 显示同意接受文件
					int iEndPos = m_RichEditShow.GetWindowTextLength();
					m_RichEditShow.SetSel(iEndPos, iEndPos);
					CString str;
					str.Format(_T("你同意接受文件: %s，后台正在接收........\r\n"), m_RecvfileInfo[i]->FileName());
					m_RichEditShow.ReplaceSel(str);

					EM_DATA data;
					data.msg = EM_ACCEPTFILE;
				//	if(m_RecvfileInfo[i]->dwRecvFileIndex == i)
				//		MessageBox("aaaaaaaaaaaaaa");
					strcpy(m_RecvfileInfo[i]->szFullName, dlgFileSaveAs.GetPathName());
					strcpy(m_RecvfileInfo[i]->szName, dlgFileSaveAs.GetFileName());
					strcpy(m_RecvfileInfo[i]->szExt, dlgFileSaveAs.GetFileExt());
					data.buf = (char*)m_RecvfileInfo[i];
					data.len = sizeof(EM_FILEINFO);
				//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
					m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, & data);

					//MessageBox(dlgFileSaveAs.GetPathName());
				}
			}
		}
	}
}

// 你拒绝接收文件
void EM_MsgDlg::File_ResDenyFile(ENLINK *pLink)
{
	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] != NULL)
		{
			int current = m_RecvfileInfo[i]->index2;
			if (current == pLink->chrg.cpMin)
			{
				// 把 (No)否 变灰
				CHARFORMAT Cfm;
				m_RichEditShow.GetSelectionCharFormat(Cfm);
				Cfm.cbSize = sizeof(CHARFORMAT);
				Cfm.dwMask = CFM_LINK | CFM_COLOR;
				Cfm.dwEffects ^= CFE_LINK;
				Cfm.crTextColor = RGB(150,150,150);
				m_RichEditShow.SetSelectionCharFormat(Cfm);

				m_RichEditShow.SetSel(pLink->chrg.cpMin - 11 - 7, pLink->chrg.cpMin - 11);
				m_RichEditShow.SetSelectionCharFormat(Cfm);

				// 显示拒绝接受文件
				int iEndPos = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iEndPos, iEndPos);
				CString str;
				str.Format("你拒绝接受文件:%s\r\n", m_RecvfileInfo[i]->FileName());
				m_RichEditShow.ReplaceSel(str);
				EM_DATA data;
				data.msg = EM_REFUSEFILE;
				data.buf = (char*)m_RecvfileInfo[i];
				data.len = sizeof(EM_FILEINFO);
			//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
				m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, & data);

			//	str.Format("dwSendFileIndex:%d", m_RecvfileInfo[i]->dwSendFileIndex);
			//	m_RichEditShow.ReplaceSel(str);
				delete m_RecvfileInfo[i];
				m_RecvfileInfo[i] = NULL;
				// 接受文件总数减 1
				m_dwRecvTotal --;
			}
		}
	}
}

// 被拒绝接受文件
void EM_MsgDlg::File_DestDenyFile(LPEM_FILEINFO fInfo)
{
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s 拒绝接收文件:%s\r\n"), m_strDisplayName, fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	m_RichEditShow.LineScroll(1);

	// 清理内存
	int i = fInfo->dwSendFileIndex;
	delete m_SendfileInfo[i];
	m_SendfileInfo[i] = NULL;
	// 发送文件减 1
	m_dwSendTotal --;

	int iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iEndPos, iEndPos);
//	str.Format("dwSendFileIndex:%d", fInfo->dwSendFileIndex);
//	m_RichEditShow.ReplaceSel(str);
}

// 对方同意接收文件
void EM_MsgDlg::File_DestAcceptFile(LPEM_FILEINFO fInfo)
{
	// 设置文件发送消息
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s 同意接收文件:%s\r\n"), m_strDisplayName, fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	m_RichEditShow.LineScroll(1);
	if (! m_bSendingFile)
	{
		GetWindowRect(& m_rtDlg);
		m_bSendingFile = TRUE;
		m_rtDlg.right += 186;
		MoveWindow(m_rtDlg);
	}
	GetClientRect(& m_rtDlg);
	EM_FileSendingDlg *p = new EM_FileSendingDlg;
	if (! IsWindow(p->GetSafeHwnd()))
	{
		p->Create(IDD_SENDFILE_DIALOG);
		p->SetParent(this);
		p->DoInitialize(k, m_rtDlg, m_dwFileDlgLength);
		LPEM_FILEINFO pInfo = m_SendfileInfo[fInfo->dwSendFileIndex];
		memcpy((char*)(&p->m_fileInfo), (char*)fInfo, sizeof(EM_FILEINFO));
		strcpy(p->m_szUserIP, m_strUserIP);
		p->BeginToSend(pInfo);
		m_pDlg[k] = p;

		// 对话框形状处理
		m_dwFileDlgLength += FILE_SENDING_DLG_LEN;
		k++;
	/*	CString str;
		str.Format("%s, %s", fInfo->FileName(), fInfo->FileFullName());
		MessageBox(str);*/
	}
}

void EM_MsgDlg::AddRecvText(LPCTSTR msg)
{
	// 信息 发送者 和 发送时间
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	csStartTime.Format(_T("%s %s\r\n"), m_strDisplayName, m_StartTime1.Format(_T("%H:%M:%S")));

	m_pMainTop->m_history.SaveHistory(m_strDisplayName, m_pMainTop->GetLocalDisplayName(), msg);

	// RichEdi 写入发送时间
	_InsertHeader(csStartTime);

	CString strMsg = msg;
	strMsg += "\r\n";
	CString strTemp;
	CString strNumber;
	DWORD dwLen = 0;

	int nIndex = strMsg.Find("_FreeEIM_Emotion_");
	while (nIndex != -1)
	{
		strTemp = strMsg.Left(nIndex);
		_InsertText(strTemp);

		strNumber = (LPCTSTR)strMsg+nIndex+17;
		strNumber = strNumber.Left(5);
//		AfxMessageBox(strNumber);
		_InsertFace(atoi((LPCTSTR)strNumber));

		dwLen = strMsg.GetLength();
		strMsg = strMsg.Right(dwLen-nIndex-22);
	//	AfxMessageBox(strMsg);
		nIndex = strMsg.Find("_FreeEIM_Emotion_");
	}

	_InsertText(strMsg);
/*	int nPos = 0;
	CString strNumber;

	if (nIndex != -1)
	{

		strNumber = (LPCTSTR)strMsg+nIndex+17;

		if (nIndex > 0)
			strMsg = strMsg.Left(nIndex);

		_InsertText(strMsg);

		_InsertFace(atoi((LPCTSTR)strNumber));
	}*/


	m_RichEditShow.HideSelection(FALSE, FALSE);
	// 试滚动条滚动到最后一行
	//m_RichEditShow.LineScroll(1);
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// 播放消息提示声音
	_PlaySoundReceive();
}

void EM_MsgDlg::AddSendText(LPCTSTR msg)
{
	CTime m_StartTime1 = CTime::GetCurrentTime();
	EM_ChattingData cdata;
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s %s\r\n"), "你说", m_StartTime1.Format(_T("%H:%M:%S")));

	cdata.SetDate((LPSTR)(LPCTSTR)csStartTime, csStartTime.GetLength()+1);
	cdata.SetData((LPSTR)msg, strlen(msg)+1);

	m_pMainTop->m_history.SaveHistory(m_strDisplayName, m_pMainTop->GetLocalDisplayName(), msg);


	_InsertHeader(csStartTime);


	CString strMsg = msg;
	strMsg += "\r\n";
//	_InsertText(strMsg);


//	CString strMsg = msg;
	CString strTemp;
	CString strNumber;
	DWORD dwLen = 0;

	int nIndex = strMsg.Find("_FreeEIM_Emotion_");
	while (nIndex != -1)
	{
		strTemp = strMsg.Left(nIndex);
		_InsertText(strTemp);

		strNumber = (LPCTSTR)strMsg+nIndex+17;
		strNumber = strNumber.Left(5);
//		AfxMessageBox(strNumber);
		_InsertFace(atoi((LPCTSTR)strNumber));

		dwLen = strMsg.GetLength();
		strMsg = strMsg.Right(dwLen-nIndex-22);
	//	AfxMessageBox(strMsg);
		nIndex = strMsg.Find("_FreeEIM_Emotion_");
	}

	_InsertText(strMsg);


	// 试滚动条滚动到最后一行
	//m_RichEditShow.LineScroll(1);
	m_RichEditShow.HideSelection(TRUE, FALSE);
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// 播放消息提示声音
	_PlaySoundSend();
}


void EM_MsgDlg::Voice_AddText(LPCTSTR msg)
{
	CTime m_StartTime1 = CTime::GetCurrentTime();
	// 信息 发送者 和 发送时间
	CString str;
	str.Format("%s %s\r\n", m_StartTime1.Format(_T("%H:%M:%S")), msg);
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	m_RichEditShow.ReplaceSel((LPCTSTR)str);

	// 试滚动条滚动到最后一行
	m_RichEditShow.LineScroll(1);
}

EM_MsgDlg::~EM_MsgDlg()
{
	for (int i=0; i<6; i++)
	{
		if (m_pDlg[i] != NULL)
			delete m_pDlg[i];
		if (m_RecvfileInfo[i] != NULL)
			delete m_RecvfileInfo[i];
		if (m_SendfileInfo[i] != NULL)
			delete m_SendfileInfo[i];

		m_pDlg[i] = NULL;
		m_RecvfileInfo[i] = NULL;
		m_SendfileInfo[i] = NULL;
	}
}

void EM_MsgDlg::NewFile(LPEM_FILEINFO fInfo)
{
	// 此版本最多只能发 6 个文件
	if (m_dwRecvTotal >= 6)
		return;

	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s 向你发送文件:%s 是否接受?\r\n        "), m_strDisplayName, fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_LINK |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = CFM_LINK;
	cf.crTextColor = RGB(166, 0, 0); //文字颜色

	// 设置超连接 (Yes)是
	iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	fInfo->index1 = iTotalTextLength;
	str.Format(_T("(Yes)是"));
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	int iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iEndPos);
	m_RichEditShow.SetSelectionCharFormat(cf);

	// 设置超连接 (No)否
	m_RichEditShow.SetSel(iEndPos, iEndPos);
	m_RichEditShow.ReplaceSel(_T("           "));
	iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	fInfo->index2 = iTotalTextLength;
	m_RichEditShow.ReplaceSel(_T("(No)否"));
	iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iEndPos);
	m_RichEditShow.SetSelectionCharFormat(cf);

	m_RichEditShow.SetSel(iEndPos, iEndPos);
	m_RichEditShow.ReplaceSel(_T("\r\n"));

	// 使RichEdit响应ENM_LINK事件
	m_RichEditShow.SetEventMask(ENM_LINK);
	// 试滚动条滚动到最后一行
	m_RichEditShow.LineScroll(1);

	// 保存文件信息
	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] == NULL)
		{
			m_RecvfileInfo[i] = new EM_FILEINFO;
			memcpy(m_RecvfileInfo[i], fInfo, sizeof(EM_FILEINFO));
			m_RecvfileInfo[i]->dwRecvFileIndex = i;
			break;
		}
	}
	m_dwRecvTotal ++;
}


// 功能：发送文件
// 更新日期：2009年12月12日
void EM_MsgDlg::OnBtnSendFile()
{
	// 由于对话框太小，最多只能容纳 6 个文件传输对话框
	if (m_dwSendTotal >= 6)
	{
		MessageBox(_T("Beta 版最多只能处理6个文件！\r\n等其它文件处理完再继续。"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 文件打开操作
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	int structsize=0;
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion;

	//检测目前的操作系统，GetVersion具体用法详见MSDN
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// 如果运行的操作系统是Windows NT/2000
	if (dwVersion < 0x80000000)
	{
		structsize =88;//显示新的文件对话框
	}
	else//运行的操作系统Windows 95/98
	{
		structsize =76;//显示老的文件对话框
	}
	
	dlgFileOpen.m_ofn.lStructSize=structsize;
//	TCHAR lpstrFilename[MAX_PATH] = _T("");
//	dlgFileOpen.m_ofn.lpstrFile=lpstrFilename;

	if(dlgFileOpen.DoModal()==IDOK)
	{
		// Add file to fileinfo list.
		for (int i=0; i<6; i++)
		{
			if (m_SendfileInfo[i] == NULL)
			{
				m_SendfileInfo[i] = new EM_FILEINFO;
				m_SendfileInfo[i]->dwSendFileIndex = i;
				break;
			}
		}
		m_SendfileInfo[i]->AddFile(dlgFileOpen.GetPathName(), dlgFileOpen.GetFileExt());
		m_dwSendTotal ++;

		EM_DATA data;
		data.buf = (char*)m_SendfileInfo[i];

		data.msg = EM_FILE;
		data.len = sizeof(EM_FILEINFO);
	
		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, & data);

		int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
		m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
		CString str;
		str.Format(_T("你向 %s 发送文件:%s \r\n"), m_strDisplayName, ((LPEM_FILEINFO)data.buf)->FileName());
		m_RichEditShow.ReplaceSel((LPCTSTR)str);
		m_RichEditShow.LineScroll(1);
	}
}

void EM_MsgDlg::File_RecvCancel(LPEM_FILEINFO fInfo)
{
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("文件 %s 没有被接收\r\n"), fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	m_RichEditShow.LineScroll(1);
}

void EM_MsgDlg::OnUpdateDlg(WPARAM wParam, LPARAM lParam)
{
	// 对话框类型，FALSE:发送，TRUE:接受。
	BOOL bDlgType = (BOOL)lParam;

	// 对话框总数要减小
	k --;

	// 对话框 ID
	DWORD nIdx = (DWORD)wParam;
	m_pDlg[nIdx]->CloseWindow();

/*	DWORD dwFileInfoIndex;
	// 发送完成要清理文件列表
	if (! bDlgType) // 发送框
	{
		dwFileInfoIndex = m_pDlg[nIdx]->m_fileInfo.dwSendFileIndex;
		if(m_SendfileInfo[dwFileInfoIndex])
		{
			delete m_SendfileInfo[dwFileInfoIndex];
			m_SendfileInfo[dwFileInfoIndex] = NULL;
			// 总数减 1
			m_dwSendTotal --;
		}
	}
	else // 接收框
	{
		dwFileInfoIndex = m_pDlg[nIdx]->m_fileInfo.dwRecvFileIndex;
		if (m_RecvfileInfo[dwFileInfoIndex])
		{
			delete m_RecvfileInfo[dwFileInfoIndex];
			m_RecvfileInfo[dwFileInfoIndex] = NULL;
			// 总数减 1
			m_dwRecvTotal --;
		}
	}*/

/*	CString str;
	str.Format("-- m_ccc:%d -- k:%d --", m_ccc, k);
	MessageBox(str);*/
//	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	delete m_pDlg[nIdx];

	for(int i=nIdx; i<k; i++)
	{
		m_pDlg[i+1]->AdjustDlgRect();
		m_pDlg[i] = m_pDlg[i+1];
	}

	// 调整对话框形状
	m_dwFileDlgLength -= FILE_SENDING_DLG_LEN;
	m_pDlg[k] = NULL;

	if (k < 1)
	{
		m_bSendingFile = FALSE;
		GetWindowRect(& m_rtDlg);
		m_rtDlg.right -= 186;
		MoveWindow(m_rtDlg);
		m_dwFileDlgLength = 8;
	}
}

void EM_MsgDlg::File_RecvFile(SOCKET sock)
{
	// 由于对话框太小，最多只能容纳 6 个文件传输对话框
	if (k >= 6)
	{
		MessageBox(_T("Beta 版最多只能处理6个文件！\r\n等其它文件处理完再继续。"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (! m_bSendingFile)
	{
		GetWindowRect(& m_rtDlg);
		m_bSendingFile = TRUE;
		m_rtDlg.right += 186;
		MoveWindow(m_rtDlg);
	}

	GetClientRect(& m_rtDlg);
	EM_FileSendingDlg *p = new EM_FileSendingDlg;
	if (! IsWindow(p->GetSafeHwnd()))
	{
		p->Create(IDD_FILERECV_DIALOG);
		p->SetParent(this);
		p->ShowWindow(SW_SHOW);
		p->DoInitialize(k, m_rtDlg, m_dwFileDlgLength);
		strcpy(p->m_szUserIP, m_strUserIP);
		p->BeginToReceive(sock);
		m_pDlg[k] = p;
		m_dwFileDlgLength += FILE_SENDING_DLG_LEN;
		k++;
	}
}

void EM_MsgDlg::File_SendComplete(int index)
{
	for (int i=0; i<6; i++)
	{
		if (m_SendfileInfo[i] != NULL)
		{
			if (m_SendfileInfo[i]->dwSendFileIndex == index)
			{
				// 设置文件发送完成消息
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("文件:%s 发送完毕\r\n"), m_SendfileInfo[i]->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
				delete m_SendfileInfo[i];
				m_SendfileInfo[i] = NULL;
				m_dwSendTotal --;
			}
		}
	}

	m_richInput.SetFocus();
}

void EM_MsgDlg::File_RecvComplete(int index)
{
	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] != NULL)
		{
		/*	CString str;
			str.Format("i:%d, %d, idx:%d", i, m_RecvfileInfo[i]->dwRecvFileIndex, index);
			MessageBox(str);*/
			if (m_RecvfileInfo[i]->dwRecvFileIndex == index)
			{
				// 设置文件发送完成消息
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("文件:%s 接收完毕\r\n"), m_RecvfileInfo[i]->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
				delete m_RecvfileInfo[i];
				m_RecvfileInfo[i] = NULL;
				m_dwRecvTotal --;
				break;
			}
		}
	}

	m_richInput.SetFocus();
}

void EM_MsgDlg::File_SendFailed(int index)
{
	for (int i=0; i<6; i++)
	{
		if (m_SendfileInfo[i] != NULL)
		{
		/*	CString str;
			str.Format("i:%d, %d, idx:%d", i, m_RecvfileInfo[i]->dwRecvFileIndex, index);
			MessageBox(str);*/
			if (m_SendfileInfo[i]->dwSendFileIndex == index)
			{
				// 设置文件发送完成消息
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("文件:%s 发送失败\r\n"), m_SendfileInfo[i]->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
				delete m_SendfileInfo[i];
				m_SendfileInfo[i] = NULL;
				m_dwSendTotal --;
				break;
			}
		}
	}

	m_richInput.SetFocus();
}

void EM_MsgDlg::File_RecvFailed(int index)
{
	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] != NULL)
		{
		/*	CString str;
			str.Format("i:%d, %d, idx:%d", i, m_RecvfileInfo[i]->dwRecvFileIndex, index);
			MessageBox(str);*/
			if (m_RecvfileInfo[i]->dwRecvFileIndex == index)
			{
				// 设置文件发送完成消息
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("文件:%s 接收失败\r\n"), m_RecvfileInfo[i]->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
				delete m_RecvfileInfo[i];
				m_RecvfileInfo[i] = NULL;
				m_dwRecvTotal --;
				break;
			}
		}
	}

	m_richInput.SetFocus();
}

void EM_MsgDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 55)
	{
		CString str;
		str.Format("SendTotal:%d, RecvTotal:%d, k:%d",m_dwSendTotal, m_dwRecvTotal, k);
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(str);
	}
	CDialog::OnTimer(nIDEvent);
}

void EM_MsgDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();

}

// 窗口抖动
void EM_MsgDlg::Shake_Window()
{
	Voice_AddText(_T("对方向你发送一个窗口抖动"));
	int ty=2;
	int nDelay = 28;
	CRect   m_rect;   
	GetWindowRect(&m_rect);  
	int recordy=m_rect.left;
	int recordx=m_rect.top;

//	RedrawWindow(&m_rect);
	for(int i=0;i<5;i++)
	{
		m_rect.left=recordy;
		m_rect.top=recordx;
		m_rect.top = m_rect.top + ty;
		m_rect.left = m_rect.left - ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		RedrawWindow(&m_rect);
		Sleep(nDelay);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top = m_rect.top -2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.left=m_rect.left+2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top = m_rect.top + ty;  
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top=m_rect.top+2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW ); 
		::SetWindowPos(m_hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
	}
}// End 窗口抖动

void EM_MsgDlg::Voice_Request()
{
	
	// end 调整 Msg_Show 窗口大小

	// 启动语音请求对话框
	m_pSendChat = new EM_SendChat;
	if (! IsWindow(m_pSendChat->GetSafeHwnd()))
	{
		m_pSendChat->Create(IDD_VOICE2);
		m_pSendChat->SetParent(this);
		m_pSendChat->ShowWindow(SW_SHOW);

		// 调整 Msg_Show 窗口大小
		CRect rtRich;
		m_RichEditShow.GetWindowRect(& rtRich);
		ScreenToClient(& rtRich);

		CRect rtChat;
		m_pSendChat->GetWindowRect(& rtChat);
		ScreenToClient(& rtChat);

		rtRich.bottom -= rtChat.bottom;
		m_RichEditShow.MoveWindow(& rtRich);

		rtChat.top = rtRich.bottom;
		rtChat.bottom = rtChat.top + rtChat.bottom;
		m_pSendChat->MoveWindow(& rtChat);
	//	m_pSendChat->UpdateWindow();
	}
	// end 启动语音请求对话框

	// 使语音请求按钮变灰
	GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// 向对方发送语音对话的请求
	EM_DATA data;
	data.msg = EM_REQUESTVOICE;
	data.buf = NULL;
	data.len = 0;

//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end 向对方发送语音对话的请求

	// 添加信息到 Msg_Show
	Voice_AddText(_T("-- 等待对方接受请求"));
}

// 被请求语音对话
void EM_MsgDlg::Voice_Income()
{
	// 启动语音请求对话框
	m_pRecvChat = new EM_RecvChat;
	if (! IsWindow(m_pRecvChat->GetSafeHwnd()))
	{
		m_pRecvChat->m_strIP = m_strUserIP;
		m_pRecvChat->Create(IDD_VOICE1);
		m_pRecvChat->SetParent(this);
		m_pRecvChat->ShowWindow(SW_SHOW);

		// 调整 Msg_Show 窗口大小
		CRect rtRich;
		m_RichEditShow.GetWindowRect(& rtRich);
		ScreenToClient(& rtRich);

		CRect rtChat;
		m_pRecvChat->GetWindowRect(& rtChat);
		ScreenToClient(& rtChat);

		rtRich.bottom -= rtChat.bottom;
		m_RichEditShow.MoveWindow(& rtRich);

		rtChat.top = rtRich.bottom;
		rtChat.bottom = rtChat.top + rtChat.bottom;
		m_pRecvChat->MoveWindow(& rtChat);
	}
	// end 启动语音请求对话框

	// 使语音请求按钮变灰
	GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// 添加信息到 Msg_Show
	Voice_AddText("-- 对方请求语音对话");

}
// end 被请求语音对话


// 拒绝接受对方语音对话
void EM_MsgDlg::Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pRecvChat)
		return;

	// 删除语音接受对话框
	CRect rtChat;
	m_pRecvChat->GetWindowRect(& rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end 删除语音接受对话框

	// 调整 Msg_Show 大小
	CRect rect;
	m_RichEditShow.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rect);
	// end 调整 Msg_Show 大小

	m_RichEditShow.HideSelection(FALSE, FALSE);
	// 试滚动条滚动到最后一行
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// 添加信息到 Msg_Show
	int nRel = (int)wParam;
	if (0 == nRel)
	{
		// 向对方发送拒绝语音对话的消息
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送拒绝语音对话的消息
		Voice_AddText(_T("-- 你不同意语音对话，操作被取消！"));
	}
	if (1 == nRel)
	{
		// 向对方发送拒绝语音对话的消息
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送拒绝语音对话的消息
		Voice_AddText(_T("-- 语音对话结束！"));
	//	m_pSendChat->m_cw->Stop();
	//	m_pSendChat->m_pwo->Stop();
		m_pMainTop->m_cw->Stop();
	}
	m_pMainTop->m_bVoiceChat = FALSE;
}
// end 拒绝接受对方语音对话

// 对方接受对话请求
void EM_MsgDlg::Voice_DestAcceptChat()
{
	if (NULL == m_pSendChat)
		return;

	m_pSendChat->DestAcceptChat(m_strUserIP);

	// 添加信息到 Msg_Show
	Voice_AddText(_T("-- 对方同意语音对话，对话进行中！"));

	m_pMainTop->m_bVoiceChat = TRUE;
}
// end 对方接受对话请求

// 接受对方语音对话请求
void EM_MsgDlg::Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam)
{
	// 向对方发送同意语音对话的消息
	EM_DATA data;
	data.msg = EM_ACCEPTCHAT;
	data.buf = NULL;
	data.len = 0;

//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end 向对方发送同意语音对话的消息

	// 添加信息到 Msg_Show
	Voice_AddText(_T("-- 你同意语音对话，对话进行中！"));
}
// end 接受对方语音对话请求

// 取消向对方请求语音对话
void EM_MsgDlg::Voice_CancelRequest(WPARAM wParam, LPARAM lParam)
{
	int nValue = wParam;

	if (NULL == m_pSendChat)
		return;

	// 删除语音请求对话框
	CRect rtChat;
	m_pSendChat->GetWindowRect(& rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end 删除语音请求对话框

	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end 调整 Msg_Show 大小

	if (2 == nValue)
	{
		// 向对方发送语音对话的请求
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送语音对话的请求

		// 添加信息到 Msg_Show
		Voice_AddText(_T("-- 你停止请求语音对话！"));
	}
	else
	{
		// 向对方发送语音对话的请求
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送语音对话的请求

		// 添加信息到 Msg_Show
		Voice_AddText(_T("-- 语音对话结束！"));
		m_pMainTop->m_cw->Stop();
	}
	m_pMainTop->m_bVoiceChat = FALSE;
}
// end 取消向对方请求语音对话

// 对方拒绝接受对话请求
void EM_MsgDlg::Voice_DoNotAcceptChat(int nValue)
{
	if (NULL == m_pSendChat)
		return;

	// 删除语音请求对话框
	CRect rtChat;
	m_pSendChat->GetWindowRect(& rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end 删除语音请求对话框
	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end 调整 Msg_Show 大小

	// 添加信息到 Msg_Show
	if (0 == nValue)
	{
		Voice_AddText(_T("-- 对方不接受语音对话！"));
	}
	else
	{
		Voice_AddText(_T("-- 语音对话结束！"));
	}
}
// end 对方拒绝接受对话请求

// 对方取消请求语音对话
void EM_MsgDlg::Voice_CancelIncome(int nValue)
{
	if (NULL == m_pRecvChat)
		return;

	// 删除语音接受对话框
	CRect rtChat;
	m_pRecvChat->GetWindowRect(& rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end 删除语音接受对话框

	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end 调整 Msg_Show 大小

	// 添加信息到 Msg_Show
	if (2 == nValue)
	{
		Voice_AddText(_T("-- 对方停止请求语音对话！"));
	}
	else
	{
		Voice_AddText(_T("-- 语音对话结束！"));
	}
}
// end 被请求语音对话

// 文件拖放发送功能
void EM_MsgDlg::OnDropFiles(HDROP hDropInfo)
{
	// 由于对话框太小，最多只能容纳 6 个文件传输对话框
	if (m_dwSendTotal >= 6)
	{
		MessageBox(_T("Beta 版最多只能处理6个文件！\r\n等其它文件处理完再继续。"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

    HDROP m_hDropInfo = hDropInfo;
    CString Filename;

    if (m_hDropInfo)
	{
        int iFiles = DragQueryFile(m_hDropInfo, (UINT)-1, NULL, 0);

		for (int i=0; i<iFiles; i++)
		{
			// 先看看总文件个数
			if (m_dwSendTotal >= 6)
			{
				MessageBox(_T("Beta 版最多只能处理6个文件！\r\n等其它文件处理完再继续。"),
					_T("警告"),
					MB_OK | MB_ICONWARNING);
				break;
			} // End 先看看总文件个数

            char* pFilename = Filename.GetBuffer(_MAX_PATH);
            DragQueryFile(m_hDropInfo, i, pFilename, _MAX_PATH);

			DWORD dwAttr = GetFileAttributes(pFilename);

			if (FILE_ATTRIBUTE_DIRECTORY & dwAttr)
			{
				MessageBox(_T("不支持文件夹发送！"),
				_T("提示"),
				MB_OK | MB_ICONWARNING);
			}
			else
			{
				// 发送文件
				for (int i=0; i<6; i++)
				{
					if (m_SendfileInfo[i] == NULL)
					{
						m_SendfileInfo[i] = new EM_FILEINFO;
						m_SendfileInfo[i]->dwSendFileIndex = i;
						break;
					}
				}
				char *szExt = GetFileExt(pFilename);
				m_SendfileInfo[i]->AddFile(pFilename, szExt);
				// 小心内存泄漏，必须删除
				delete szExt;
				m_dwSendTotal ++;
				// --------------------------

				EM_DATA data;
				data.buf = (char*)m_SendfileInfo[i];

				data.msg = EM_FILE;
				data.len = sizeof(EM_FILEINFO);
//				EM_USERINFO ui(m_szUserPCName, m_szUserIP);
				m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, & data);

				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("你向 %s 发送文件:%s \r\n"), m_strDisplayName, ((LPEM_FILEINFO)data.buf)->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
			}
		}
    }

    DragFinish(m_hDropInfo);
    m_hDropInfo = 0;
}   // End of OnDropFiles 文件拖放发送功能

BOOL EM_MsgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN)
		{
			if(GetKeyState(VK_CONTROL)<0)
			{
				OnBtnSend();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void EM_MsgDlg::OnFaceSelect(UINT nID)
{
	int nFace = nID - ID_BUTTON32794;
/*	CString str;
	str.Format("%d", nFace);
	AfxMessageBox(str);*/
	
	// Get the bitmap from the imagelist
	//
	HBITMAP hBitmap = GetImage(GetImageList(), nFace);

	if (hBitmap)
	{
		// Insert the bitmap to the richedit control at the current location
		// 
		CImageDataObject::InsertBitmap(m_richInput.GetIRichEditOle(), hBitmap, nID);
	}
//	AfxMessageBox("asdf");
}

CImageList& EM_MsgDlg::GetImageList()
{
	return m_imgListFaces;
}


HBITMAP EM_MsgDlg::GetImage(CImageList& ilist, int num)
{
	CBitmap dist;
	CClientDC dc(NULL);

	IMAGEINFO ii;
	ilist.GetImageInfo(num, &ii);

	int nWidth = ii.rcImage.right - ii.rcImage.left;
	int nHeight = ii.rcImage.bottom - ii.rcImage.top;

	dist.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = memDC.SelectObject(&dist);

	memDC.FillSolidRect(0, 0, nWidth, nHeight, 
			m_richInput.SetBackgroundColor(TRUE, 0));
	ilist.Draw(&memDC, num, CPoint(0, 0), ILD_NORMAL);

	memDC.SelectObject(pOldBitmap);

	return (HBITMAP)dist.Detach();
}

void EM_MsgDlg::_PlaySoundReceive()
{
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat(szDir, "sounds\\");
	strcat(szDir, "sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void EM_MsgDlg::_InsertFace(DWORD dwFace)
{
	int nFace = dwFace - ID_BUTTON32794;

	// Get the bitmap from the imagelist
	//
	HBITMAP hBitmap = GetImage(GetImageList(), nFace);
	if (hBitmap)
	{
		// Insert the bitmap to the richedit control at the current location
		// 
		int nEndPos = m_RichEditShow.GetWindowTextLength();
		m_RichEditShow.SetSel(nEndPos, nEndPos);
		CImageDataObject::InsertBitmap(m_RichEditShow.GetIRichEditOle(), hBitmap, 0);
	}

}

void EM_MsgDlg::_InsertText(LPCTSTR lpszText)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(1, 144, 223);

	int nStartPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(nStartPos, nStartPos);

	m_RichEditShow.ReplaceSel(lpszText);

	int nEndPos = m_RichEditShow.GetWindowTextLength();

	m_RichEditShow.SetSel(nStartPos, nEndPos);
	m_RichEditShow.SetSelectionCharFormat(cf);

	m_RichEditShow.SetSel(nEndPos, nEndPos);
}


// RichEdi 写入发送时间
void EM_MsgDlg::_InsertHeader(LPCTSTR lpszInfo)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(102, 0, 237);

	int nStartPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(nStartPos, nStartPos);

	m_RichEditShow.ReplaceSel(lpszInfo);

	int nEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(nStartPos, nEndPos);

	m_RichEditShow.SetSelectionCharFormat(cf);

	m_RichEditShow.SetSel(nEndPos, nEndPos);
}

void EM_MsgDlg::_PlaySoundSend()
{
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat(szDir, "sounds\\");
	strcat(szDir, "sound_2.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void EM_MsgDlg::_InitFaceToolbar()
{

	if (!m_FirstToolBar.CreateEx(this, TBSTYLE_FLAT, CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY) ||
		!m_FirstToolBar.LoadToolBar(IDR_TOOLBAR_FACE))
	{
		AfxMessageBox("Failed to create toolbar\n");
		return;      // fail to create
	}

	// Create the list with 18x18 pixel for each image, 
	//   using a 24-bit DIB section, and using a mask.
	m_imgListFaces.Create(18, 18, ILC_COLOR24|ILC_MASK, 18, 1);
	
	// Load the list from resource, and select the transperency color
	CBitmap bmpFaces;
	bmpFaces.LoadBitmap(IDB_BITMAP4);
	m_imgListFaces.Add(& bmpFaces, RGB(255, 255, 255));

	// Finally, set the new image list to the toolbar
	m_FirstToolBar.GetToolBarCtrl().SetImageList(& m_imgListFaces);

//	m_FirstToolBar.Create(this);

	m_FirstToolBar.LoadToolBar(IDR_TOOLBAR_FACE);
	m_FirstToolBar.ShowWindow(SW_SHOW);
//	m_FirstToolBar.SetBarStyle(CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);

	GetDlgItem(IDC_STATIC_FACE)->ShowWindow(SW_HIDE);
	CRect rect;
	GetDlgItem(IDC_STATIC_FACE)->GetWindowRect(&rect);
	ScreenToClient(&rect);
    m_FirstToolBar.MoveWindow(&rect);
//	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
/*	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
		0, reposQuery, rcClientStart);*/
	//得出控件条大小.
/*	CRect rect;
	CRect rectNow;
	GetClientRect(rect);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,reposQuery,rectNow);

	//放置控件条位置
	CPoint ptOffset(rectNow.left-rect.left,rectNow.top-rect.top); 

	CRect  rcChild;
	CWnd* pwndChild=GetWindow(GW_CHILD);
	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild,FALSE);
		pwndChild=pwndChild->GetNextWindow();
	}

	//调整对话框尺寸
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right+=rect.Width()-rectNow.Width();
	rcWindow.bottom+=rect.Height()-rectNow.Height();
	MoveWindow(rcWindow, FALSE);

	//控件条定位
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	*/
}

void EM_MsgDlg::_InitHistoryToday()
{
	CStdioFile file;

	CString fileName;
	CString str;
	CTime today;

	today = CTime::GetCurrentTime();
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	fileName.Format("%shistory\\%s\\%s.imh", buf, today.Format("%y-%m-%d"), m_strDisplayName);
//	AfxMessageBox(fileName);
	if (file.Open(fileName, CFile::modeRead))
	{
		while(file.ReadString(str))
		{
		//	m_RichEditShow.ReplaceSel(str+"\n");
			CString strMsg = str;
			strMsg += "\r\n";
			CString strTemp;
			CString strNumber;
			DWORD dwLen = 0;

			int nIndex = strMsg.Find("_FreeEIM_Emotion_");
			while (nIndex != -1)
			{
				strTemp = strMsg.Left(nIndex);
				_InsertText(strTemp);

				strNumber = (LPCTSTR)strMsg+nIndex+17;
				strNumber = strNumber.Left(5);
				_InsertFace(atoi((LPCTSTR)strNumber));

				dwLen = strMsg.GetLength();
				strMsg = strMsg.Right(dwLen-nIndex-22);
				nIndex = strMsg.Find("_FreeEIM_Emotion_");
			}
			_InsertText(strMsg);
		}
		file.Close();
	}

	int nEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(nEndPos, nEndPos);

	// 试滚动条滚动到最后一行
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
}
