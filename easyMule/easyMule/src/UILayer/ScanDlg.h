#pragma once
#include "PPgDownloadSecurity.h"

// CScanDlg �Ի���

class CScanDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanDlg)

public:
	CScanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScanDlg();

// �Ի�������
	enum { IDD = IDD_SCAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	CPPgDownloadSecurity* m_pParent;

public:
	void Localize();
protected:
	void LoadSettings();
	virtual void OnOK();
};
