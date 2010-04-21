// DlgMainTabSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "emuleDlg.h"
#include "DlgMainTabSearch.h"
#include "PageTabBkDraw.h"
#include "TabItem_Normal.h"
#include "TabItem_NormalCloseable.h"
#include "SearchListCtrl.h"
#include "SearchDlg.h"
#include "SearchResultsWnd.h"
#include "HelpIDs.h"
#include "UserMsgs.h"
#include "CmdFuncs.h"
#include "TabItem_Wnd.h"
#include "DlgMainTabResource.h"
#include "kademlia/kademlia/kademlia.h"


#define UM_SEARCHTAB_EM_DESTROY	UM_END  + 10
#define IDC_SEARCH_TAB	IDC_INTERVAL_LAB + 0x100

// CDlgMainTabSearch 对话框

IMPLEMENT_DYNAMIC(CDlgMainTabSearch, CDialog)
CDlgMainTabSearch::CDlgMainTabSearch(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgMainTabSearch::IDD, pParent)
{
	m_dwCounter = 0;
	m_SearchMap.InitHashTable(1031);
	m_dwTotalCount = 0;
}

CDlgMainTabSearch::~CDlgMainTabSearch()
{
	POSITION pos = m_SearchMap.GetStartPosition();
	while (pos != NULL) 
	{
		int key;
		SSearchParams * params;
		m_SearchMap.GetNextAssoc(pos, key, params);
		if(params != NULL)
			delete params;
	}
	m_SearchMap.RemoveAll();
}

void CDlgMainTabSearch::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMainTabSearch, CResizableDialog)
	ON_NOTIFY(NMC_TW_ACTIVE_TAB_CHANDED, IDC_SEARCH_TAB, OnNMActiveTabChanged)
	ON_NOTIFY(NMC_TW_TAB_DESTROY, IDC_SEARCH_TAB, OnNMTabDestroy)
	ON_NOTIFY(NMC_TW_TAB_CREATE, IDC_SEARCH_TAB, OnNMTabCreate)
	ON_MESSAGE(UM_SEARCHTAB_EM_DESTROY,OnEMTabDestroy)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgMainTabSearch 消息处理程序

void CDlgMainTabSearch::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	OnOK();
}

void CDlgMainTabSearch::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
//	OnCancel();
}

int CDlgMainTabSearch::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPCHILDREN);
	return 0;
}

BOOL CDlgMainTabSearch::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect	rect;
	//GetDlgItem(IDC_TAB1)->GetWindowRect(&rect);
	//ScreenToClient(&rect);
	GetClientRect(&rect);

	m_tabWnd.Create(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, this, IDC_SEARCH_TAB );
	CPageTabBkDraw	*pBarBkDraw = new CPageTabBkDraw;
	m_tabWnd.SetBarBkDraw(pBarBkDraw);

	ShowEmuleSearch();
	AddAnchor(m_tabWnd,TOP_LEFT,BOTTOM_RIGHT);
	return TRUE;  
}

void CDlgMainTabSearch::ShowEmuleSearch()
{
	DWORD CustomData = 0xF0F00000;

	CString	strDisplayText;
	strDisplayText = GetResString(IDS_SEARCHEMULE);

	SSearchParams * pSearchParams = GetParameters(_T(""));

	CustomData += m_dwCounter;

	theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = m_dwCounter;
	int iCounter = m_dwCounter;
	m_dwCounter++;

	m_SearchMap.SetAt(iCounter, pSearchParams);

	CTabItem_NormalCloseable	*pTi = NULL;
	pTi = new CTabItem_NormalCloseable;
	pTi->SetCaption(strDisplayText);
	pTi->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
	pTi->SetCustomData(CustomData);
	pTi->EnableClose(FALSE);
	pTi->SetDesireLength(150);

	m_tabWnd.AddTab(pTi,TRUE);
	m_dwTotalCount++;
}

void CDlgMainTabSearch::CreateNewSearch(LPCTSTR lpszCaption,ESearchType * pSearchType/* = NULL*/)
	{
	DWORD	iCounter;
		DWORD CustomData = 0xF0F00000;

		SSearchParams * pSearchParams = GetParameters(lpszCaption);
	if(pSearchType)
		pSearchParams->eType = *pSearchType;
	else
		pSearchParams->eType = SearchTypeEasyMuleFile;

		CustomData          += m_dwCounter;
		theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = m_dwCounter;
		iCounter = m_dwCounter;
		m_dwCounter++;
		m_SearchMap.SetAt(iCounter, pSearchParams);

		CString	strDisplayText;
		strDisplayText = GetResString(IDS_TABTITLE_SEARCH_RESULT);
		strDisplayText += lpszCaption;

		CTabItem_NormalCloseable	*pTi = NULL;
		pTi = new CTabItem_NormalCloseable;
		pTi->SetCaption(strDisplayText);
		pTi->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
		pTi->SetCustomData(CustomData);
		pTi->SetDesireLength(150);
		POSITION Pos = m_tabWnd.GetActiveTab();
		m_tabWnd.AddTab(pTi,TRUE,Pos);

		if (!theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pSearchParams))
			m_SearchMap.RemoveKey(iCounter);

		m_dwTotalCount++;		
	}

void CDlgMainTabSearch::OpenNewSearchResult(LPCTSTR lpszCaption,ESearchType * pSearchType/* = NULL */)
{
	if (m_dwTotalCount > MAX_OPENED_TABS)
	{
		MessageBox(GetResString(IDS_ERR_MAXTABS),GetResString(IDS_CAPTION),MB_ICONWARNING);
		return;
	}

	CmdFuncs::SetMainActiveTab(CMainTabWnd::TI_SEARCH);

	if( !pSearchType || *pSearchType==SearchTypeEasyMuleFile )
	{
		CreateNewSearch(lpszCaption,pSearchType);
	}
	else if ( *pSearchType == SearchTypeEd2kGlobal )
	{
		if( CGlobalVariable::serverconnect->IsConnected() )
			CreateNewSearch(lpszCaption,pSearchType);
	else
			MessageBox(GetResString(IDS_ERR_NOTCONNECTED),GetResString(IDS_CAPTION),MB_ICONWARNING);
	}
	else if( *pSearchType == SearchTypeKademlia ) 
	{
		if( Kademlia::CKademlia::IsConnected() )
		{
			DWORD	iCounter;
			DWORD CustomData = 0xF0F00000;

			if (m_dwCounter > 0x20)
			{
				return;
			}

			SSearchParams * pSearchParams = GetParameters(lpszCaption);
			pSearchParams->eType = SearchTypeKademlia;

			CustomData          += m_dwCounter;

			theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = m_dwCounter;
			iCounter = m_dwCounter;
			m_dwCounter++;
			m_SearchMap.SetAt(iCounter, pSearchParams);

			CString	strDisplayText;
			strDisplayText = GetResString(IDS_TABTITLE_SEARCH_RESULT);
			strDisplayText += lpszCaption;


			CTabItem_NormalCloseable	*pTi = NULL;
			pTi = new CTabItem_NormalCloseable;
			pTi->SetCaption(strDisplayText);
			pTi->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
			pTi->SetCustomData(CustomData);
			pTi->SetDesireLength(150);
			POSITION Pos = m_tabWnd.GetActiveTab();
			m_tabWnd.AddTab(pTi,TRUE,Pos);

			/* VC-dgkang 2008年7月17日
			//Kademlia 返回错误，但不Delete  pParams也不RemoveKey从m_SearchMap中，而是在删除标签页一起删除.
			//这是为多标签页操作修改的。
			if (!theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pSearchParams))
				m_SearchMap.RemoveKey(iCounter);
			*/
			theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pSearchParams);
			m_dwTotalCount++;
		}
		else
		{
			MessageBox(GetResString(IDS_ERR_NOTCONNECTEDKAD),GetResString(IDS_CAPTION),MB_ICONWARNING);	
		}
	}
}

void CDlgMainTabSearch::OnNMTabCreate(NMHDR* pNMHDR, LRESULT *pResult)
{
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);
	DWORD dwCustomData = m_tabWnd.GetTabData(pTabOp->posTab);
	
	if(dwCustomData >= 0xF0F00000)
		UpdateEMsClosableStatus();
	*pResult = 0;
}

void CDlgMainTabSearch::OnNMTabDestroy(NMHDR* pNMHDR, LRESULT *pResult)
{
	theApp.emuledlg->searchwnd->m_pwndResults->SetPos(0);
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);

	DWORD CustomData = m_tabWnd.GetTabData(pTabOp->posTab);
	DWORD LowData  = CustomData & 0x0000FFFF;
	DWORD HighData = CustomData & 0xFFFF0000;

	if (HighData ^ 0x0F0FFFFF)
	{
		SSearchParams *pSearchParams=NULL;
		BOOL bSearch = m_SearchMap.Lookup(LowData, pSearchParams);

		if ( bSearch && pSearchParams)
		{
			theApp.emuledlg->searchwnd->m_pwndResults->searchlistctrl.RemoveResults(pSearchParams->dwSearchID);
			m_SearchMap.RemoveKey(LowData);
		}
		delete pSearchParams;
	}
	if (CustomData >= 0xF0F00000)
		PostMessage(UM_SEARCHTAB_EM_DESTROY);

	if (m_dwTotalCount > 0)
		m_dwTotalCount--;
	*pResult = 0;
}

void CDlgMainTabSearch::OnNMActiveTabChanged(NMHDR* pNMHDR, LRESULT *pResult)
{
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);
	DWORD CustomData = m_tabWnd.GetTabData(pTabOp->posTab);

	DWORD LowData  = CustomData & 0x0000FFFF;
	DWORD HighData = CustomData & 0xFFFF0000;

	if (HighData ^ 0x0F0FFFFF)
	{
		SSearchParams *pSearchParams=NULL;
		BOOL bSearch = m_SearchMap.Lookup(LowData, pSearchParams);

		if ( bSearch && pSearchParams )
		{
			theApp.emuledlg->searchwnd->m_pwndResults->searchlistctrl.ShowResults(pSearchParams->dwSearchID);
			theApp.emuledlg->searchwnd->m_pwndResults->UpdateParamDisplay(pSearchParams);
			theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = LowData;
		}
	}
	*pResult = 0;
}

void CDlgMainTabSearch::OnMainTabChanged()
{
	POSITION Pos = m_tabWnd.GetActiveTab();
	if (Pos == NULL)
		return;

	m_tabWnd.ShowActiveTab();

	DWORD CustomData = m_tabWnd.GetTabData(Pos);
	DWORD LowData  = CustomData & 0x0000FFFF;
	DWORD HighData = CustomData & 0xFFFF0000;

	if( CustomData >= 0xF0F00000 )
	{
		CTabItem* pTabItem = m_tabWnd.GetTabItem(Pos);
		
		if (pTabItem)
			pTabItem->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
	}

	if (HighData ^ 0x0F0FFFFF)
	{
		SSearchParams *pSearchParams=NULL;
		BOOL bSearch = m_SearchMap.Lookup(LowData, pSearchParams);

		if ( bSearch && pSearchParams )
		{
			theApp.emuledlg->searchwnd->m_pwndResults->searchlistctrl.ShowResults(pSearchParams->dwSearchID);
			theApp.emuledlg->searchwnd->m_pwndResults->UpdateParamDisplay(pSearchParams);
			theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = LowData;
		}
	}
}

void CDlgMainTabSearch::UpdateEMsClosableStatus()
{
	int     iEMCount = 0;

	CTabItem_NormalCloseable * pEMItem  = NULL;
	POSITION	pos =m_tabWnd.GetFirstTab();
	CTabItem	*pItem = NULL;
	while (NULL != pos)
	{
		pItem = m_tabWnd.GetNextTab(pos);
		if (NULL != pItem)
		{
			if (pItem->m_dwCustomData >= 0xF0F00000)
			{
				pEMItem = (CTabItem_NormalCloseable *)pItem;
				pEMItem->EnableClose(TRUE);
				iEMCount ++;
			}
		}
	}
	if (1 == iEMCount && NULL != pEMItem && 
		pEMItem->GetCaption() == GetResString(IDS_SEARCHEMULE))
	{
		pEMItem->EnableClose(FALSE);
	}
	if ( 0 == iEMCount) ShowEmuleSearch();
}


BOOL CDlgMainTabSearch::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		// Don't handle Ctrl+Tab in this window. It will be handled by main window.
		if (pMsg->wParam == VK_TAB && GetAsyncKeyState(VK_CONTROL) < 0)
			return FALSE;

		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
			return FALSE;
	}
	return CResizableDialog::PreTranslateMessage(pMsg);
}

void CDlgMainTabSearch::Localize()
{
	const CString tcsEN = _T("easyMule Search"),tcsTW = _T("搜索電驢網路"), tcsCN = _T("搜索电驴网络");
	const CString szEN = _T("Search:"),szTW = _T("搜索:"), szCN = _T("搜索:");

	CString tcs;
	CTabItem_NormalCloseable * pEMItem  = NULL;
	int nLen = -1;

	POSITION	pos = m_tabWnd.GetFirstTab();
	CTabItem	*pItem = NULL;
	while (NULL != pos)
	{
		pItem = m_tabWnd.GetNextTab(pos);
		if (NULL != pItem)
		{
			if (pItem->m_dwCustomData >= 0xF0F00000)
			{
				pEMItem = (CTabItem_NormalCloseable *)pItem;
				CString tcs = pEMItem->GetCaption();
				if (tcs == tcsEN || tcs == tcsTW || tcs == tcsCN)
					pEMItem->SetCaption(GetResString(IDS_SEARCHEMULE));
				else
				{
					if (tcs.Find(szEN) == 0) nLen = szEN.GetLength();
					else if(tcs.Find(szTW) == 0) nLen = szTW.GetLength();
					else if(tcs.Find(szCN) == 0) nLen = szCN.GetLength();
					else nLen = -1;
					if (nLen != -1)
					{
						tcs.Replace(tcs.Left(nLen),GetResString(IDS_TABTITLE_SEARCH_RESULT));
						pEMItem->SetCaption(tcs);
					}
				}
			}
		}
	}
}

void CDlgMainTabSearch::UpdateSearchParam(int iIndex, SSearchParams *pSearchParams)
{
	SSearchParams	*psp;
	if (m_SearchMap.Lookup(iIndex, psp))
	{
		delete psp;
		m_SearchMap.SetAt(iIndex, pSearchParams);
	}
}

LRESULT CDlgMainTabSearch::OnEMTabDestroy(WPARAM /*wParam */, LPARAM /*lParam */)
{
	UpdateEMsClosableStatus();
	return 0;
}