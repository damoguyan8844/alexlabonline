// FIM_SerialNumberDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "FIM_SerialNumberDlg.h"
#include "thirdpartclass/GetHardSoftInfo.h"
#include "thirdpartclass/INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFIM_SerialNumberDlg dialog


CFIM_SerialNumberDlg::CFIM_SerialNumberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFIM_SerialNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFIM_SerialNumberDlg)
	m_strSN1 = _T("");
	m_strSN2 = _T("");
	m_strSN3 = _T("");
	m_strSN4 = _T("");
	m_strHDDSerial = _T("");
	m_bAccept = FALSE;
		m_strLicence = _T("TSͬ��ͨ���ʹ�����Э�飺\r\n\r\n"
"    ��Э����������ΪTSͬ��ͨ�����ʹ���ߣ��뱱��������ý�Ƽ����޹�˾�����³ơ�������ý����֮���й�ʹ��TSͬ��ͨ�����Э�顣\r\n\r\n"
"    �������Ķ�����ⱾЭ���й涨������Ȩ�������ƣ���ֻ����������������������ܻ��ʹ��TSͬ��ͨ�����Ȩ������һ����װ�����ƻ���������ʽʹ��TSͬ��ͨ���������ʾ��ͬ����ܱ�Э����������Լ����\r\n"
"\r\n����֤��\r\n"
"    1. ֻ��һ̨������ʹ�ñ�ϵͳ��\r\n"
"    2. Ϊ����һ̨������ʹ�ã����ڱ��ݻ򵵰������Ŀ�ģ��Ի����ɶ���ʽ������ϵͳ�Ŀ�����\r\n"
"\r\n����֤����\r\n"
"    1. �Ա�ϵͳ�ٴ�ת����ɣ�\r\n"
"    2. �Ա�ϵͳ�������򹤳̡�����������ж��\r\n"
"    3. ������ת����ϵͳ��ȫ���򲿷֣�����Э�������Ĺ涨�ĳ��⡣\r\n\r\n"
"\r\n��ϵͳ�İ�Ȩ������Ȩ��\r\n"
"    ��ϵͳ�������п��������ƣ��뱾ϵͳ��ע���Ĺ�˾ͬ�ڡ���ϵͳ���ĵ����а�Ȩ�����ܹ��Ұ�Ȩ��������ЭԼ����ı����������ܴӱ������ȥ�����Ȩ����������֤Ϊ��ϵͳ�Ŀ���(ȫ���򲿷�)���ư�Ȩ��������ͬ����ֹ���κ���ʽ�Ƿ�������ϵͳ���ĵ���\r\n\r\n"
"\r\n���ճе�\r\n"
"1. ʹ��TSͬ��ͨ������û��Լ��е����գ��ڷ�����������Χ�ڣ�������ý���κ�����²����û���ʹ�û���ʹ��TSͬ��ͨ���������������ġ�����ġ�ֱ�ӻ��ӵ���ʧ�е��⳥���Σ���ʹ������ý�����ȱ���֪���𺦷����Ŀ����ԡ� \r\n"
"2. ʹ��TSͬ��ͨ��������������κ����˵ģ�������в�Եġ��̰��Եġ����˷��еĻ�Ƿ������ݻ���Ϊ�������Ȩ�����ַ���������ð������Ϣ�ķ��գ�����е����Ϸ��ա� \r\n"
"3. ������ý��TSͬ��ͨ������ṩ�������κ����͵ĵ���������˵����κ����û���������Ƿ�ʹ�÷��������ֱ�ӡ���ӡ�żȻ�����⼰�������𺦣����е��κ����Ρ�\r\n\r\n"
"�����ֹ��\r\n"
"    ����Υ����Э�����һ����������������˾������ʱ����ֹ��ɡ���ֹ���֮ʱ���������������ٱ�ϵͳ���ĵ������п�������黹������˾��");
	//}}AFX_DATA_INIT

	m_bConfirm = TRUE;
}


void CFIM_SerialNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFIM_SerialNumberDlg)
	DDX_Text(pDX, IDC_EDIT2, m_strSN1);
	DDX_Text(pDX, IDC_EDIT3, m_strSN2);
	DDX_Text(pDX, IDC_EDIT4, m_strSN3);
	DDX_Text(pDX, IDC_EDIT5, m_strSN4);
	DDX_Text(pDX, IDC_EDIT1, m_strHDDSerial);
	DDX_Control(pDX, IDC_EXPLORER1, m_ctrlWeb);
	DDX_Text(pDX, IDC_EDIT6, m_strLicence);
	DDX_Check(pDX, IDC_CHECK1, m_bAccept);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFIM_SerialNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CFIM_SerialNumberDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFIM_SerialNumberDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CHtmlEditorDlg)
	ON_EVENT(CFIM_SerialNumberDlg, IDC_EXPLORER1, 252 /* NavigateComplete2 */, OnNavigateComplete2Explorer1, VTS_DISPATCH VTS_PVARIANT)
//	ON_EVENT(CSerialTesterDlg, IDC_EXPLORER1, 102 /* StatusTextChange */, OnStatusTextChangeExplorer1, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/////////////////////////////////////////////////////////////////////////////
// CFIM_SerialNumberDlg message handlers

void CFIM_SerialNumberDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (! m_bAccept)
	{
		MessageBox("���ע�����������Э�顣", "ע��δ���", MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_CHECK1)->SetFocus();
		return;
	}
	CString strCode;
	GetDlgItemText(IDC_EDIT1, strCode);

	CString strSN;
	strSN = m_strSN1+m_strSN2+m_strSN3+m_strSN4;
//	strSN.TrimRight();

	CString strURL;
	strURL.Format(_T("http://www.coopic.cn/ts/user_register.asp"
		"?sn=%s&bs=%s"), strSN, strCode);

//	AfxMessageBox(strURL);
//	SetDlgItemText(IDC_EDIT6, strURL);
//	m_ctrlWeb.Navigate( _T("http://www.coopic.cn/ts/user_login.asp?login=WEIH100045869635&bs=WEIH100045869635"), NULL, NULL, NULL,NULL);
	m_ctrlWeb.Navigate(strURL, NULL, NULL, NULL,NULL);
}

void CFIM_SerialNumberDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
//	CDialog::OnOK();
}

BOOL CFIM_SerialNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CGetMachineInfo serialCode;
	CString strCode = serialCode.str_DN_Serial;
	strCode.TrimLeft();
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText(strCode);
	pEdit->SetReadOnly();

	CEdit *pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT2);
	pEdit1->SetLimitText(4);
	CEdit *pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT3);
	pEdit2->SetLimitText(4);
	CEdit *pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT4);
	pEdit3->SetLimitText(4);
	CEdit *pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT5);
	pEdit4->SetLimitText(4);

	GetDlgItem(IDC_EDIT2)->SetFocus();

	m_bConfirm = FALSE;
	// ����Ѵ������к�
	if (_IsRegistered())
	{
	//	AfxMessageBox(_T("�����ע�ᣡ"));
		CString strURL;
		strURL.Format(_T("http://www.coopic.cn/ts/user_login.asp"
			"?login=%s&bs=%s"), m_strSN, strCode);

	//	SetDlgItemText(IDC_EDIT6, strURL);
		m_ctrlWeb.Navigate(strURL, NULL, NULL, NULL,NULL);
	}
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFIM_SerialNumberDlg::_IsRegistered()
{
	// ����û������ļ�
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat(szFileName, "licnece.dat");

	CIniReader iniReader(szFileName);
	if (iniReader.sectionExists("TS_SERIAL_NUMBER"))
	{
		m_strSN = iniReader.getKeyValue("SN", "TS_SERIAL_NUMBER");
		if (! m_strSN.IsEmpty())
		{
			m_bConfirm = TRUE;
		}
	}

	return m_bConfirm;
}


void CFIM_SerialNumberDlg::GetProgramDirectory(char *buf)
{
	ASSERT(NULL != buf);
	
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	buf[nIdx+1] = NULL;
}


void CFIM_SerialNumberDlg::OnNavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	//let's turn on HTML editing capabilities!
//	m_ctrlWeb.SetDesignMode(TRUE);
	
//	SetWindowText( "Current URL: " + m_ctrlWeb.GetLocationURL() );
	CString str;
	m_ctrlWeb.GetDocumentHTML(str);

	if (str.Find("yes") != -1) // ���� yes ��ʾ�ɹ�
	{
		if (m_bConfirm)
		{
		//	m_bConfirm = FALSE;
		//	AfxMessageBox("�����ע��");
		}
		else
		{
			AfxMessageBox("ע��ɹ�");
					// ����û������ļ�
			char szFileName[_MAX_PATH];
			GetProgramDirectory(szFileName);
			strcat(szFileName, "licnece.dat");

			CString strSN;
			strSN = m_strSN1+m_strSN2+m_strSN3+m_strSN4;

			CIniReader iniReader(szFileName);
			iniReader.setKey(strSN, "SN", "TS_SERIAL_NUMBER");
		}
		CDialog::OnOK();
	}
	else
	{
		if (m_bConfirm)
		{
			m_bConfirm = FALSE;
			ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox(_T("ע�᲻�ɹ������������룡"));
			m_strSN1 = _T("");
			m_strSN2 = _T("");
			m_strSN3 = _T("");
			m_strSN4 = _T("");
			UpdateData(FALSE);
			GetDlgItem(IDC_EDIT2)->SetFocus();
		}
	}
}

void CFIM_SerialNumberDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_bConfirm)
		ShowWindow(SW_HIDE);
	// Do not call CDialog::OnPaint() for painting messages
}
