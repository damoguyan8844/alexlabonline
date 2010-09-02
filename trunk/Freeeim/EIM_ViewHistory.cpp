// EIM_ViewHistory.cpp : implementation file
//

#include "stdafx.h"
#include "em/stdemdef.h"
#include "EIM02.h"
#include "EIM_ViewHistory.h"
#include "ImageDataObject.h"

#include "XEIM_Database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EIM_ViewHistory dialog


EIM_ViewHistory::EIM_ViewHistory(CWnd* pParent /*=NULL*/)
	: CDialog(EIM_ViewHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(EIM_ViewHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_HISTORY);

	m_strDate = new char[128];
	m_strFullDate = new char[128];
	m_strSelUser = new char[64];
	m_szProgramDir = new char[MAX_PATH];
}


void EIM_ViewHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EIM_ViewHistory)
	DDX_Control(pDX, IDC_RICHEDIT1, m_richHistory);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_COMBO1, m_cbUserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EIM_ViewHistory, CDialog)
	//{{AFX_MSG_MAP(EIM_ViewHistory)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER1, OnCloseupDatetimepicker1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EIM_ViewHistory message handlers

BOOL EIM_ViewHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CTime tToday = CTime::GetCurrentTime();

	GetProgramDirectory(m_szProgramDir);
	strcpy(m_strDate, (LPCTSTR)tToday.Format("%Y-%m-%d"));
	strcpy(m_strFullDate, (LPCTSTR)tToday.Format("%y年%m月%d日"));

	AddUserList(m_strDate);
//	ReadUserHistory(m_strSelUser);

	m_imgListFaces.Create(18, 18, ILC_COLOR24|ILC_MASK, 18, 1);
	
	// Load the list from resource, and select the transperency color
	CBitmap bmpFaces;
	bmpFaces.LoadBitmap(IDB_BITMAP4);
	m_imgListFaces.Add(& bmpFaces, RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 把文件夹里的用户名添加到列表里
void EIM_ViewHistory::AddUserList(LPCTSTR szDate)
{
	CFileFind fFind;
	CString strDir = szDate;

	strDir.Format("%shistory\\%s\\*.imh", m_szProgramDir, szDate);
//	MessageBox(strDir);

	while(-1 != m_cbUserList.DeleteString(0));

	BOOL ret = fFind.FindFile(strDir);
	while(ret)
	{
		ret = fFind.FindNextFile();

		if (! fFind.IsDots())
			m_cbUserList.AddString(fFind.GetFileTitle());
	}

	fFind.Close();
}

void EIM_ViewHistory::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	DWORD dwSelected = m_cbUserList.GetCurSel();
	m_cbUserList.GetLBText(dwSelected, m_strSelUser);
	ReadUserHistory(m_strSelUser);
}

void EIM_ViewHistory::OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTime time;
	CString strDir;

	m_date.GetTime(time);

	strcpy(m_strDate, (LPCTSTR)time.Format("%Y-%m-%d"));
	strcpy(m_strFullDate, (LPCTSTR)time.Format("%y年%m月%d日"));
	ReadUserHistory(m_strSelUser);
}

void EIM_ViewHistory::AddFailedMsg()
{
	char szBuf[64];
	sprintf(szBuf, "与用户 %s 在 %s 无聊天记录。", (LPCTSTR)m_strSelUser, (LPCTSTR)m_strFullDate);
	m_richHistory.SetWindowText(szBuf);
}

void EIM_ViewHistory::ReadUserHistory(LPCTSTR szUserName)
{
	m_cbUserList.SetWindowText(szUserName);
	m_richHistory.SetWindowText("");

	if (NULL != szUserName)
	{
		XEIM_Database &rDB = XEIM_Database::GetDB();
		strcpy(m_strSelUser, szUserName);

		_RecordsetPtr &pRs = rDB.GetChatTextByDay(szUserName, m_strSelUser, m_strDate);
		//CString str;
		//str = m_strDate;
		//str += " 00:00:00";
		//_RecordsetPtr &pRs = rDB.GetChatText(szUserName, m_strSelUser, (LPCSTR)str);

		if (pRs->adoEOF)
		{
			AddFailedMsg();
		}

		while (! pRs->adoEOF)
		{
			_variant_t var;
			CString str = "";

			var = pRs->GetCollect(_variant_t("sender"));
			str = str + (LPCTSTR)_bstr_t(var) + " ";
			var = pRs->GetCollect(_variant_t("senddate"));
			str = str + (LPCTSTR)_bstr_t(var);
			AddRecvText(str);
			str = "";

			var = pRs->GetCollect(_variant_t("message"));
			str = (LPCTSTR)_bstr_t(var);
			AddRecvText(str);
			
			pRs->MoveNext();
		}

		pRs->Close();
	}
}

void EIM_ViewHistory::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (NULL != m_strDate)
	{
		delete m_strDate;
		m_strDate = NULL;
	}

	if (NULL != m_strFullDate)
	{
		delete m_strFullDate;
		m_strFullDate = NULL;
	}

	if (NULL != m_strSelUser)
	{
		delete m_strSelUser;
		m_strSelUser = NULL;
	}

	if (NULL != m_szProgramDir)
	{
		delete m_szProgramDir;
		m_szProgramDir = NULL;
	}

	CDialog::OnClose();
}


void EIM_ViewHistory::AddRecvText(LPCTSTR msg)
{
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
		_InsertFace(atoi((LPCTSTR)strNumber));

		dwLen = strMsg.GetLength();
		strMsg = strMsg.Right(dwLen-nIndex-22);
		nIndex = strMsg.Find("_FreeEIM_Emotion_");
	}

	_InsertText(strMsg);

	m_richHistory.HideSelection(FALSE, FALSE);
	// 试滚动条滚动到最后一行
	m_richHistory.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
}


void EIM_ViewHistory::_InsertFace(DWORD dwFace)
{
	int nFace = dwFace - ID_BUTTON32794;

	// Get the bitmap from the imagelist
	//
	HBITMAP hBitmap = GetImage(GetImageList(), nFace);
	if (hBitmap)
	{
		// Insert the bitmap to the richedit control at the current location
		// 
		int nEndPos = m_richHistory.GetWindowTextLength();
		m_richHistory.SetSel(nEndPos, nEndPos);
		CImageDataObject::InsertBitmap(m_richHistory.GetIRichEditOle(), hBitmap, 0);
	}

}

CImageList& EIM_ViewHistory::GetImageList()
{
	return m_imgListFaces;
}

HBITMAP EIM_ViewHistory::GetImage(CImageList& ilist, int num)
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
			m_richHistory.SetBackgroundColor(TRUE, 0));
	ilist.Draw(&memDC, num, CPoint(0, 0), ILD_NORMAL);

	memDC.SelectObject(pOldBitmap);

	return (HBITMAP)dist.Detach();
}

void EIM_ViewHistory::_InsertText(LPCTSTR lpszText)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(1, 144, 223);

	int nStartPos = m_richHistory.GetWindowTextLength();
	m_richHistory.SetSel(nStartPos, nStartPos);

	m_richHistory.ReplaceSel(lpszText);

	int nEndPos = m_richHistory.GetWindowTextLength();

	m_richHistory.SetSel(nStartPos, nEndPos);
	m_richHistory.SetSelectionCharFormat(cf);

	m_richHistory.SetSel(nEndPos, nEndPos);
}
