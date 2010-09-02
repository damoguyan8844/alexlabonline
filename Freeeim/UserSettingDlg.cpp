// UserSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "UserSettingDlg.h"
#include "em/EM_UserInfo.h"
#include "em/EM_UserLogin.h"
#include "em/EM_GroupInfo.h"
#include "EIM02Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserSettingDlg dialog


FreeEIM_UserSettingDlg::FreeEIM_UserSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FreeEIM_UserSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USER_SETTING);

	m_pMainTop = NULL;
}

void FreeEIM_UserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingDlg)
	DDX_Control(pDX, IDC_COMBO1, m_cbGroup);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(FreeEIM_UserSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CUserSettingDlg)
	ON_BN_CLICKED(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserSettingDlg message handlers

BOOL FreeEIM_UserSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here

	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();

	SetDlgItemText(IDC_EDIT1, m_pMainTop->m_config.m_strDisplayName);
	m_cbGroup.SetWindowText(m_pMainTop->m_config.m_strGroup);

	// 把现有的分组填写到下拉菜单中
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_pMainTop->m_treeCtrlList.GetChildItem(TVI_ROOT);
	CString strItemText;

	for(;hChildItem != NULL; hChildItem = hNextItem)
	{
		strItemText = m_pMainTop->m_treeCtrlList.GetItemText(hChildItem);
		m_cbGroup.AddString(strItemText);
		hNextItem = m_pMainTop->m_treeCtrlList.GetNextSiblingItem(hChildItem);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FreeEIM_UserSettingDlg::OnOK()
{
	CString strName;
	CString strGroup;

	GetDlgItemText(IDC_EDIT1, strName);
	m_cbGroup.GetWindowText(strGroup);

	if (strName.IsEmpty())
	{
		// 姓名不能为空
		MessageBox(_T("姓名不能为空"), _T("提示"), MB_OK);
		SetDlgItemText(IDC_EDIT1, m_pMainTop->m_config.m_strDisplayName);
		return;
	}

	if (strGroup.IsEmpty())
	{
		// 分组不能为空
		m_cbGroup.SetWindowText(m_pMainTop->m_config.m_strGroup);
		MessageBox(_T("分组不能为空"), _T("提示"), MB_OK);
		return;
	}

	m_pMainTop->EM_UpdateUserInfo(strName, strGroup);

	CDialog::OnOK();
}
