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

	// ˳���ܴ���
	_InitFaceToolbar();
	_InitHistoryToday();

	DragAcceptFiles();
	m_richInput.SetFocus();

	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();

	CString strTitle;
	strTitle.Format(_T("�� %s �ĶԻ�"), m_strDisplayName);
	SetWindowText(strTitle);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EM_MsgDlg::OnBtnSend()
{
	// TODO: Add your control notification handler code here
	CString strSend;
	m_richInput.GetWindowText(strSend);
	// ����û�ʲôҲû���밴����,����ʾ����
	if (strSend.IsEmpty())
	{
		// ����Ҫ��Ҫ��ʾ����ֱ�ӷ���
		m_richInput.SetFocus();
		return;
	}

	//��ȡRichEdit���м���bitmap���������RichEdit����λͼ�ļ�
	int c = m_richInput.GetIRichEditOle()->GetObjectCount();     

	int pos;
	DWORD dwUser;
	int nPos=0;
	CString strFace;
	for(int i = 0 ; i < c ; i++) //����λͼ
	{
		REOBJECT object;  //λͼ��Ϣ��������
		memset(&object,0,sizeof(REOBJECT));
		object.cbStruct = sizeof(REOBJECT);
		m_richInput.GetIRichEditOle()->GetObject(i,&object,REO_GETOBJ_ALL_INTERFACES);

		pos = object.cp ; //λͼ��λ����Ϣ
		dwUser = object.dwUser; //λͼ����Ϣ��֮ǰӦ�ó������õģ�Ӧ�г���Ȼ֪��ʲô��˼��
		strFace.Format("_FreeEIM_Emotion_%d", dwUser);
		strSend.Delete(pos+nPos,1);
		strSend.Insert(pos+nPos, strFace);
		nPos += strFace.GetLength()-1;
	}

	EM_DATA data;
	data.msg = EM_TEXT;
	data.buf = (LPSTR)(LPCTSTR)strSend;
	data.len = strSend.GetLength() + 1;

	// ��Ⱥ�����ǵ�����
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

// ���Է����Ͷ�������
void EM_MsgDlg::OnBtnShake()
{
	EM_DATA data;
	data.msg = EM_SHAKEWINDOW;
	data.buf = 0;
	data.len = 0;

	// ��Ⱥ�����ǵ�����
	if (0 == strcmp(m_strUserPCName, "asdf--->>>xxxxxxasiew*3234234*sdfsdf"))
	{
		m_pMainTop->SendMSGToAll(&data);
	}
	else
	{
	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	}

	Voice_AddText(_T("����Է�������һ�����ڶ���"));

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
}// End ���Է����Ͷ�������

BOOL EM_MsgDlg::IsCurrentUser(LPSTR hostName)
{
	if (strcmp(m_strUserPCName, hostName))
	{
		return FALSE;
	}
	return TRUE;
}

// ��� ͬ������ļ�
void EM_MsgDlg::File_ResAcceptFile(ENLINK *pLink)
{
	// ���ڶԻ���̫С�����ֻ������ 6 ���ļ�����Ի���
	if (k >= 6)
	{
		MessageBox(_T("Beta �����ֻ�ܴ���6���ļ���\r\n�������ļ��������ټ�����"), _T("����"), MB_OK | MB_ICONWARNING);
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
				//���Ŀǰ�Ĳ���ϵͳ��GetVersion�����÷����MSDN
				dwVersion = GetVersion();
				dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
				dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
				// ������еĲ���ϵͳ��Windows NT/2000
				if (dwVersion < 0x80000000)
				structsize =88;//��ʾ�µ��ļ��Ի���
				else
				//���еĲ���ϵͳWindows 95/98
				structsize =76;//��ʾ�ϵ��ļ��Ի���
				dlgFileSaveAs.m_ofn.lStructSize=structsize;

				// ���˱���
				if (IDOK == dlgFileSaveAs.DoModal())
				{
					// �� (Yes)�� ���
					CHARFORMAT Cfm;
					m_RichEditShow.GetSelectionCharFormat(Cfm);
					Cfm.cbSize = sizeof(CHARFORMAT);
					Cfm.dwMask = CFM_LINK | CFM_COLOR;
					Cfm.dwEffects ^= CFE_LINK;
					Cfm.crTextColor = RGB(150,150,150);
					m_RichEditShow.SetSelectionCharFormat(Cfm);

					// ��λ (No)�� Ȼ��������
					m_RichEditShow.SetSel(pLink->chrg.cpMax+11, pLink->chrg.cpMax + 11 + 6);
					m_RichEditShow.SetSelectionCharFormat(Cfm);

					// ��ʾͬ������ļ�
					int iEndPos = m_RichEditShow.GetWindowTextLength();
					m_RichEditShow.SetSel(iEndPos, iEndPos);
					CString str;
					str.Format(_T("��ͬ������ļ�: %s����̨���ڽ���........\r\n"), m_RecvfileInfo[i]->FileName());
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

// ��ܾ������ļ�
void EM_MsgDlg::File_ResDenyFile(ENLINK *pLink)
{
	for (int i=0; i<6; i++)
	{
		if (m_RecvfileInfo[i] != NULL)
		{
			int current = m_RecvfileInfo[i]->index2;
			if (current == pLink->chrg.cpMin)
			{
				// �� (No)�� ���
				CHARFORMAT Cfm;
				m_RichEditShow.GetSelectionCharFormat(Cfm);
				Cfm.cbSize = sizeof(CHARFORMAT);
				Cfm.dwMask = CFM_LINK | CFM_COLOR;
				Cfm.dwEffects ^= CFE_LINK;
				Cfm.crTextColor = RGB(150,150,150);
				m_RichEditShow.SetSelectionCharFormat(Cfm);

				m_RichEditShow.SetSel(pLink->chrg.cpMin - 11 - 7, pLink->chrg.cpMin - 11);
				m_RichEditShow.SetSelectionCharFormat(Cfm);

				// ��ʾ�ܾ������ļ�
				int iEndPos = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iEndPos, iEndPos);
				CString str;
				str.Format("��ܾ������ļ�:%s\r\n", m_RecvfileInfo[i]->FileName());
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
				// �����ļ������� 1
				m_dwRecvTotal --;
			}
		}
	}
}

// ���ܾ������ļ�
void EM_MsgDlg::File_DestDenyFile(LPEM_FILEINFO fInfo)
{
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s �ܾ������ļ�:%s\r\n"), m_strDisplayName, fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	m_RichEditShow.LineScroll(1);

	// �����ڴ�
	int i = fInfo->dwSendFileIndex;
	delete m_SendfileInfo[i];
	m_SendfileInfo[i] = NULL;
	// �����ļ��� 1
	m_dwSendTotal --;

	int iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iEndPos, iEndPos);
//	str.Format("dwSendFileIndex:%d", fInfo->dwSendFileIndex);
//	m_RichEditShow.ReplaceSel(str);
}

// �Է�ͬ������ļ�
void EM_MsgDlg::File_DestAcceptFile(LPEM_FILEINFO fInfo)
{
	// �����ļ�������Ϣ
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s ͬ������ļ�:%s\r\n"), m_strDisplayName, fInfo->FileName());
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

		// �Ի�����״����
		m_dwFileDlgLength += FILE_SENDING_DLG_LEN;
		k++;
	/*	CString str;
		str.Format("%s, %s", fInfo->FileName(), fInfo->FileFullName());
		MessageBox(str);*/
	}
}

void EM_MsgDlg::AddRecvText(LPCTSTR msg)
{
	// ��Ϣ ������ �� ����ʱ��
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	csStartTime.Format(_T("%s %s\r\n"), m_strDisplayName, m_StartTime1.Format(_T("%H:%M:%S")));

	m_pMainTop->m_history.SaveHistory(m_strDisplayName, m_pMainTop->GetLocalDisplayName(), msg);

	// RichEdi д�뷢��ʱ��
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
	// �Թ��������������һ��
	//m_RichEditShow.LineScroll(1);
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// ������Ϣ��ʾ����
	_PlaySoundReceive();
}

void EM_MsgDlg::AddSendText(LPCTSTR msg)
{
	CTime m_StartTime1 = CTime::GetCurrentTime();
	EM_ChattingData cdata;
	CString csStartTime;
	// ��Ϣ ������ �� ����ʱ��
	csStartTime.Format(_T("%s %s\r\n"), "��˵", m_StartTime1.Format(_T("%H:%M:%S")));

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


	// �Թ��������������һ��
	//m_RichEditShow.LineScroll(1);
	m_RichEditShow.HideSelection(TRUE, FALSE);
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// ������Ϣ��ʾ����
	_PlaySoundSend();
}


void EM_MsgDlg::Voice_AddText(LPCTSTR msg)
{
	CTime m_StartTime1 = CTime::GetCurrentTime();
	// ��Ϣ ������ �� ����ʱ��
	CString str;
	str.Format("%s %s\r\n", m_StartTime1.Format(_T("%H:%M:%S")), msg);
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	m_RichEditShow.ReplaceSel((LPCTSTR)str);

	// �Թ��������������һ��
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
	// �˰汾���ֻ�ܷ� 6 ���ļ�
	if (m_dwRecvTotal >= 6)
		return;

	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("%s ���㷢���ļ�:%s �Ƿ����?\r\n        "), m_strDisplayName, fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_LINK |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = CFM_LINK;
	cf.crTextColor = RGB(166, 0, 0); //������ɫ

	// ���ó����� (Yes)��
	iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	fInfo->index1 = iTotalTextLength;
	str.Format(_T("(Yes)��"));
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	int iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iEndPos);
	m_RichEditShow.SetSelectionCharFormat(cf);

	// ���ó����� (No)��
	m_RichEditShow.SetSel(iEndPos, iEndPos);
	m_RichEditShow.ReplaceSel(_T("           "));
	iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	fInfo->index2 = iTotalTextLength;
	m_RichEditShow.ReplaceSel(_T("(No)��"));
	iEndPos = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iEndPos);
	m_RichEditShow.SetSelectionCharFormat(cf);

	m_RichEditShow.SetSel(iEndPos, iEndPos);
	m_RichEditShow.ReplaceSel(_T("\r\n"));

	// ʹRichEdit��ӦENM_LINK�¼�
	m_RichEditShow.SetEventMask(ENM_LINK);
	// �Թ��������������һ��
	m_RichEditShow.LineScroll(1);

	// �����ļ���Ϣ
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


// ���ܣ������ļ�
// �������ڣ�2009��12��12��
void EM_MsgDlg::OnBtnSendFile()
{
	// ���ڶԻ���̫С�����ֻ������ 6 ���ļ�����Ի���
	if (m_dwSendTotal >= 6)
	{
		MessageBox(_T("Beta �����ֻ�ܴ���6���ļ���\r\n�������ļ��������ټ�����"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	// �ļ��򿪲���
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	int structsize=0;
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion;

	//���Ŀǰ�Ĳ���ϵͳ��GetVersion�����÷����MSDN
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// ������еĲ���ϵͳ��Windows NT/2000
	if (dwVersion < 0x80000000)
	{
		structsize =88;//��ʾ�µ��ļ��Ի���
	}
	else//���еĲ���ϵͳWindows 95/98
	{
		structsize =76;//��ʾ�ϵ��ļ��Ի���
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
		str.Format(_T("���� %s �����ļ�:%s \r\n"), m_strDisplayName, ((LPEM_FILEINFO)data.buf)->FileName());
		m_RichEditShow.ReplaceSel((LPCTSTR)str);
		m_RichEditShow.LineScroll(1);
	}
}

void EM_MsgDlg::File_RecvCancel(LPEM_FILEINFO fInfo)
{
	int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
	m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
	CString str;
	str.Format(_T("�ļ� %s û�б�����\r\n"), fInfo->FileName());
	m_RichEditShow.ReplaceSel((LPCTSTR)str);
	m_RichEditShow.LineScroll(1);
}

void EM_MsgDlg::OnUpdateDlg(WPARAM wParam, LPARAM lParam)
{
	// �Ի������ͣ�FALSE:���ͣ�TRUE:���ܡ�
	BOOL bDlgType = (BOOL)lParam;

	// �Ի�������Ҫ��С
	k --;

	// �Ի��� ID
	DWORD nIdx = (DWORD)wParam;
	m_pDlg[nIdx]->CloseWindow();

/*	DWORD dwFileInfoIndex;
	// �������Ҫ�����ļ��б�
	if (! bDlgType) // ���Ϳ�
	{
		dwFileInfoIndex = m_pDlg[nIdx]->m_fileInfo.dwSendFileIndex;
		if(m_SendfileInfo[dwFileInfoIndex])
		{
			delete m_SendfileInfo[dwFileInfoIndex];
			m_SendfileInfo[dwFileInfoIndex] = NULL;
			// ������ 1
			m_dwSendTotal --;
		}
	}
	else // ���տ�
	{
		dwFileInfoIndex = m_pDlg[nIdx]->m_fileInfo.dwRecvFileIndex;
		if (m_RecvfileInfo[dwFileInfoIndex])
		{
			delete m_RecvfileInfo[dwFileInfoIndex];
			m_RecvfileInfo[dwFileInfoIndex] = NULL;
			// ������ 1
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

	// �����Ի�����״
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
	// ���ڶԻ���̫С�����ֻ������ 6 ���ļ�����Ի���
	if (k >= 6)
	{
		MessageBox(_T("Beta �����ֻ�ܴ���6���ļ���\r\n�������ļ��������ټ�����"), _T("����"), MB_OK | MB_ICONWARNING);
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
				// �����ļ����������Ϣ
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("�ļ�:%s �������\r\n"), m_SendfileInfo[i]->FileName());
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
				// �����ļ����������Ϣ
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("�ļ�:%s �������\r\n"), m_RecvfileInfo[i]->FileName());
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
				// �����ļ����������Ϣ
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("�ļ�:%s ����ʧ��\r\n"), m_SendfileInfo[i]->FileName());
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
				// �����ļ����������Ϣ
				int iTotalTextLength = m_RichEditShow.GetWindowTextLength();
				m_RichEditShow.SetSel(iTotalTextLength, iTotalTextLength);
				CString str;
				str.Format(_T("�ļ�:%s ����ʧ��\r\n"), m_RecvfileInfo[i]->FileName());
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

// ���ڶ���
void EM_MsgDlg::Shake_Window()
{
	Voice_AddText(_T("�Է����㷢��һ�����ڶ���"));
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
}// End ���ڶ���

void EM_MsgDlg::Voice_Request()
{
	
	// end ���� Msg_Show ���ڴ�С

	// ������������Ի���
	m_pSendChat = new EM_SendChat;
	if (! IsWindow(m_pSendChat->GetSafeHwnd()))
	{
		m_pSendChat->Create(IDD_VOICE2);
		m_pSendChat->SetParent(this);
		m_pSendChat->ShowWindow(SW_SHOW);

		// ���� Msg_Show ���ڴ�С
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
	// end ������������Ի���

	// ʹ��������ť���
	GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// ��Է����������Ի�������
	EM_DATA data;
	data.msg = EM_REQUESTVOICE;
	data.buf = NULL;
	data.len = 0;

//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end ��Է����������Ի�������

	// �����Ϣ�� Msg_Show
	Voice_AddText(_T("-- �ȴ��Է���������"));
}

// �����������Ի�
void EM_MsgDlg::Voice_Income()
{
	// ������������Ի���
	m_pRecvChat = new EM_RecvChat;
	if (! IsWindow(m_pRecvChat->GetSafeHwnd()))
	{
		m_pRecvChat->m_strIP = m_strUserIP;
		m_pRecvChat->Create(IDD_VOICE1);
		m_pRecvChat->SetParent(this);
		m_pRecvChat->ShowWindow(SW_SHOW);

		// ���� Msg_Show ���ڴ�С
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
	// end ������������Ի���

	// ʹ��������ť���
	GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// �����Ϣ�� Msg_Show
	Voice_AddText("-- �Է����������Ի�");

}
// end �����������Ի�


// �ܾ����ܶԷ������Ի�
void EM_MsgDlg::Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pRecvChat)
		return;

	// ɾ���������ܶԻ���
	CRect rtChat;
	m_pRecvChat->GetWindowRect(& rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end ɾ���������ܶԻ���

	// ���� Msg_Show ��С
	CRect rect;
	m_RichEditShow.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rect);
	// end ���� Msg_Show ��С

	m_RichEditShow.HideSelection(FALSE, FALSE);
	// �Թ��������������һ��
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	// �����Ϣ�� Msg_Show
	int nRel = (int)wParam;
	if (0 == nRel)
	{
		// ��Է����;ܾ������Ի�����Ϣ
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����;ܾ������Ի�����Ϣ
		Voice_AddText(_T("-- �㲻ͬ�������Ի���������ȡ����"));
	}
	if (1 == nRel)
	{
		// ��Է����;ܾ������Ի�����Ϣ
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����;ܾ������Ի�����Ϣ
		Voice_AddText(_T("-- �����Ի�������"));
	//	m_pSendChat->m_cw->Stop();
	//	m_pSendChat->m_pwo->Stop();
		m_pMainTop->m_cw->Stop();
	}
	m_pMainTop->m_bVoiceChat = FALSE;
}
// end �ܾ����ܶԷ������Ի�

// �Է����ܶԻ�����
void EM_MsgDlg::Voice_DestAcceptChat()
{
	if (NULL == m_pSendChat)
		return;

	m_pSendChat->DestAcceptChat(m_strUserIP);

	// �����Ϣ�� Msg_Show
	Voice_AddText(_T("-- �Է�ͬ�������Ի����Ի������У�"));

	m_pMainTop->m_bVoiceChat = TRUE;
}
// end �Է����ܶԻ�����

// ���ܶԷ������Ի�����
void EM_MsgDlg::Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam)
{
	// ��Է�����ͬ�������Ի�����Ϣ
	EM_DATA data;
	data.msg = EM_ACCEPTCHAT;
	data.buf = NULL;
	data.len = 0;

//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end ��Է�����ͬ�������Ի�����Ϣ

	// �����Ϣ�� Msg_Show
	Voice_AddText(_T("-- ��ͬ�������Ի����Ի������У�"));
}
// end ���ܶԷ������Ի�����

// ȡ����Է����������Ի�
void EM_MsgDlg::Voice_CancelRequest(WPARAM wParam, LPARAM lParam)
{
	int nValue = wParam;

	if (NULL == m_pSendChat)
		return;

	// ɾ����������Ի���
	CRect rtChat;
	m_pSendChat->GetWindowRect(& rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end ɾ����������Ի���

	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end ���� Msg_Show ��С

	if (2 == nValue)
	{
		// ��Է����������Ի�������
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����������Ի�������

		// �����Ϣ�� Msg_Show
		Voice_AddText(_T("-- ��ֹͣ���������Ի���"));
	}
	else
	{
		// ��Է����������Ի�������
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_pMainTop->m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����������Ի�������

		// �����Ϣ�� Msg_Show
		Voice_AddText(_T("-- �����Ի�������"));
		m_pMainTop->m_cw->Stop();
	}
	m_pMainTop->m_bVoiceChat = FALSE;
}
// end ȡ����Է����������Ի�

// �Է��ܾ����ܶԻ�����
void EM_MsgDlg::Voice_DoNotAcceptChat(int nValue)
{
	if (NULL == m_pSendChat)
		return;

	// ɾ����������Ի���
	CRect rtChat;
	m_pSendChat->GetWindowRect(& rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end ɾ����������Ի���
	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end ���� Msg_Show ��С

	// �����Ϣ�� Msg_Show
	if (0 == nValue)
	{
		Voice_AddText(_T("-- �Է������������Ի���"));
	}
	else
	{
		Voice_AddText(_T("-- �����Ի�������"));
	}
}
// end �Է��ܾ����ܶԻ�����

// �Է�ȡ�����������Ի�
void EM_MsgDlg::Voice_CancelIncome(int nValue)
{
	if (NULL == m_pRecvChat)
		return;

	// ɾ���������ܶԻ���
	CRect rtChat;
	m_pRecvChat->GetWindowRect(& rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_VOICECHAT)->EnableWindow();
	// end ɾ���������ܶԻ���

	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(& rtRich);
	ScreenToClient(& rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(& rtRich);
	// end ���� Msg_Show ��С

	// �����Ϣ�� Msg_Show
	if (2 == nValue)
	{
		Voice_AddText(_T("-- �Է�ֹͣ���������Ի���"));
	}
	else
	{
		Voice_AddText(_T("-- �����Ի�������"));
	}
}
// end �����������Ի�

// �ļ��Ϸŷ��͹���
void EM_MsgDlg::OnDropFiles(HDROP hDropInfo)
{
	// ���ڶԻ���̫С�����ֻ������ 6 ���ļ�����Ի���
	if (m_dwSendTotal >= 6)
	{
		MessageBox(_T("Beta �����ֻ�ܴ���6���ļ���\r\n�������ļ��������ټ�����"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

    HDROP m_hDropInfo = hDropInfo;
    CString Filename;

    if (m_hDropInfo)
	{
        int iFiles = DragQueryFile(m_hDropInfo, (UINT)-1, NULL, 0);

		for (int i=0; i<iFiles; i++)
		{
			// �ȿ������ļ�����
			if (m_dwSendTotal >= 6)
			{
				MessageBox(_T("Beta �����ֻ�ܴ���6���ļ���\r\n�������ļ��������ټ�����"),
					_T("����"),
					MB_OK | MB_ICONWARNING);
				break;
			} // End �ȿ������ļ�����

            char* pFilename = Filename.GetBuffer(_MAX_PATH);
            DragQueryFile(m_hDropInfo, i, pFilename, _MAX_PATH);

			DWORD dwAttr = GetFileAttributes(pFilename);

			if (FILE_ATTRIBUTE_DIRECTORY & dwAttr)
			{
				MessageBox(_T("��֧���ļ��з��ͣ�"),
				_T("��ʾ"),
				MB_OK | MB_ICONWARNING);
			}
			else
			{
				// �����ļ�
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
				// С���ڴ�й©������ɾ��
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
				str.Format(_T("���� %s �����ļ�:%s \r\n"), m_strDisplayName, ((LPEM_FILEINFO)data.buf)->FileName());
				m_RichEditShow.ReplaceSel((LPCTSTR)str);
				m_RichEditShow.LineScroll(1);
			}
		}
    }

    DragFinish(m_hDropInfo);
    m_hDropInfo = 0;
}   // End of OnDropFiles �ļ��Ϸŷ��͹���

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


// RichEdi д�뷢��ʱ��
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
	//�ó��ؼ�����С.
/*	CRect rect;
	CRect rectNow;
	GetClientRect(rect);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,reposQuery,rectNow);

	//���ÿؼ���λ��
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

	//�����Ի���ߴ�
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right+=rect.Width()-rectNow.Width();
	rcWindow.bottom+=rect.Height()-rectNow.Height();
	MoveWindow(rcWindow, FALSE);

	//�ؼ�����λ
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

	// �Թ��������������һ��
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
}
