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
		m_strLicence = _T("TS同事通软件使用许可协议：\r\n\r\n"
"    本协议是您，作为TS同事通软件的使用者，与北京酷屏传媒科技有限公司（以下称“酷屏传媒”）之间有关使用TS同事通软件的协议。\r\n\r\n"
"    请认真阅读和理解本协议中规定的所有权利和限制，您只有无条件接受所有条款，才能获得使用TS同事通软件的权力。您一旦安装、复制或以其他方式使用TS同事通软件，即表示您同意接受本协议各项条款的约束。\r\n"
"\r\n您保证：\r\n"
"    1. 只在一台机器上使用本系统；\r\n"
"    2. 为在这一台机器上使用，出于备份或档案管理的目的，以机器可读格式制作本系统的拷贝；\r\n"
"\r\n您保证不：\r\n"
"    1. 对本系统再次转让许可；\r\n"
"    2. 对本系统进行逆向工程、反汇编或解体拆卸；\r\n"
"    3. 拷贝或转交本系统的全部或部分，但本协议中明文规定的除外。\r\n\r\n"
"\r\n本系统的版权和所有权：\r\n"
"    本系统及其所有拷贝的名称，与本系统中注明的公司同在。本系统及文档享有版权，并受国家版权法及国际协约条款的保护。您不能从本软件中去掉其版权声明；并保证为本系统的拷贝(全部或部分)复制版权声明。您同意制止以任何形式非法拷贝本系统及文档。\r\n\r\n"
"\r\n风险承担\r\n"
"1. 使用TS同事通软件由用户自己承担风险，在法律允许的最大范围内，酷屏传媒在任何情况下不就用户因使用或不能使用TS同事通软件所发生的特殊的、意外的、直接或间接的损失承担赔偿责任，即使酷屏传媒已事先被告知该损害发生的可能性。 \r\n"
"2. 使用TS同事通软件存在有来自任何他人的，包括威胁性的、诽谤性的、令人反感的或非法的内容或行为或对他人权利的侵犯的匿名或冒名的信息的风险，您须承担以上风险。 \r\n"
"3. 酷屏传媒对TS同事通软件所提供服务不作任何类型的担保，对因此导致任何因用户不正当或非法使用服务产生的直接、间接、偶然、特殊及后续的损害，不承担任何责任。\r\n\r\n"
"许可终止：\r\n"
"    您若违返本协议的任一条款与条件，本公司可能随时会终止许可。终止许可之时，您必须立即销毁本系统及文档的所有拷贝，或归还给本公司。");
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
		MessageBox("软件注册必须接受许可协议。", "注册未完成", MB_OK | MB_ICONINFORMATION);
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
	// 软件已存在序列号
	if (_IsRegistered())
	{
	//	AfxMessageBox(_T("软件已注册！"));
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
	// 检查用户设置文件
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

	if (str.Find("yes") != -1) // 返回 yes 表示成功
	{
		if (m_bConfirm)
		{
		//	m_bConfirm = FALSE;
		//	AfxMessageBox("软件已注册");
		}
		else
		{
			AfxMessageBox("注册成功");
					// 检查用户设置文件
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
			AfxMessageBox(_T("注册不成功，请重新输入！"));
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
