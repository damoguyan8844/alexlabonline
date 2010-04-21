/*
 * $Id: MainTabWnd.cpp 14238 2009-07-08 10:04:38Z dgkang $
 * 
 * this file is part of easyMule
 * Copyright (C)2002-2008 VeryCD Dev Team ( strEmail.Format("%s@%s", "emuledev", "verycd.com") / http: * www.easymule.org )
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
// MainTabWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainTabWnd.h"
#include "TabItem_MainTabBn.h"
#include ".\maintabwnd.h"
#include "TabItem_MainButton.h"
#include "emule.h"
#include "emuleDlg.h"
#include "SharedFilesWnd.h"
#include "ServerWnd.h"
#include "TransferWnd.h"
#include "TabItem_Wnd.h"
#include "CmdFuncs.h"
#include "MainTabBkDraw.h"

// CMainTabWnd

IMPLEMENT_DYNAMIC(CMainTabWnd, CTabWnd)
CMainTabWnd::CMainTabWnd()
{
	int	i;
	for (i = 0; i < TI_MAX; i++)
	{
		m_aposTabs[i] = NULL;
	}
}

CMainTabWnd::~CMainTabWnd()
{
	if (IsWindow(m_dlgResource.m_hWnd))
		m_dlgResource.DestroyWindow();

	if (IsWindow(m_dlgDownload.m_hWnd))
		m_dlgDownload.DestroyWindow();

	if (IsWindow(m_dlgShare.m_hWnd))
		m_dlgShare.DestroyWindow();

	if (IsWindow(m_dlgSearch.m_hWnd))
		m_dlgSearch.DestroyWindow();
}

BEGIN_MESSAGE_MAP(CMainTabWnd, CTabWnd)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
//	ON_WM_CREATE()
ON_WM_CREATE()
//ON_WM_PAINT()
END_MESSAGE_MAP()

//BEGIN_ANCHOR_MAP(CMainTabWnd)
//END_ANCHOR_MAP()

BOOL CMainTabWnd::CreateEx(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CTabWnd::Create(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParentWnd, nID);
	if (!bRet)
		return bRet;

	CMainTabBkDraw	*pBkDraw = new CMainTabBkDraw;
	SetBarBkDraw(pBkDraw);


	m_dlgResource.Create(m_dlgResource.IDD, this);
	m_dlgDownload.Create(m_dlgDownload.IDD, this);
	m_dlgShare.Create(m_dlgShare.IDD, this);
	m_dlgSearch.Create(m_dlgSearch.IDD,this);
	m_dlgAdvance.Create(m_dlgAdvance.IDD, this);

	SetBarBreadth(41);


	m_aposTabs[TI_RESOURCE] = CmdFuncs::TabWnd_AddMainButton(this, _T("Resource"),
														m_dlgResource.GetSafeHwnd(),
														_T("PNG_MAINTAB_BROWSER_N"),
														_T("PNG_MAINTAB_BROWSER_A"));
		
	
	m_aposTabs[TI_DOWNLOAD] = CmdFuncs::TabWnd_AddMainButton(this, _T("Download"),
														m_dlgDownload.GetSafeHwnd(),
														_T("PNG_MAINTAB_DOWNLOAD_N"),
														_T("PNG_MAINTAB_DOWNLOAD_A"));
	
	//m_aposTabs[TI_SHARE] = CmdFuncs::TabWnd_AddMainButton(this, _T("Share"),
	//													theApp.emuledlg->sharedfileswnd->GetSafeHwnd(),
	//													theApp.LoadIcon(_T("MAINTAB_SHARE_N"), 32, 32),
	//													theApp.LoadIcon(_T("MAINTAB_SHARE_A"), 32, 32));
	m_aposTabs[TI_SHARE] = CmdFuncs::TabWnd_AddMainButton(this, _T("Share"),
														m_dlgShare.GetSafeHwnd(),
														_T("PNG_MAINTAB_SHARE_N"),
														_T("PNG_MAINTAB_SHARE_A"));

	m_aposTabs[TI_SEARCH] = CmdFuncs::TabWnd_AddMainButton(this, _T("Search"),
														m_dlgSearch.GetSafeHwnd(),
														_T("PNG_MAINTAB_SEARCH_N"),
														_T("PNG_MAINTAB_SEARCH_A"));



	if (CPreferences::m_bAdvancePageShowed)
	{
		m_aposTabs[TI_ADVANCE] = CmdFuncs::TabWnd_AddMainButton(this, GetResString(IDS_ADVANCE),
																m_dlgAdvance.GetSafeHwnd(),
																_T("PNG_MAINTAB_ADVANCED_N"),
																_T("PNG_MAINTAB_ADVANCED_A"));
	}

	CTabItem_MainTabBn	*pTiMainTabBn = NULL;

	pTiMainTabBn = new CTabItem_MainTabBn;
	m_aposTabs[TI_BN] = AddTab(pTiMainTabBn);

	CClientRect	rtClient(this);

	m_dlgSidePanel.Create(m_dlgSidePanel.IDD, this);
	CRect	rtSidePanel;

	rtSidePanel = rtClient;
	rtSidePanel.left = rtSidePanel.right - m_dlgSidePanel.GetDesireWidth();
	rtSidePanel.bottom = rtSidePanel.top + GetBarBreadth();
	m_dlgSidePanel.MoveWindow(&rtSidePanel);

	CTabItem_Wnd	*pTabItemWnd = new CTabItem_Wnd;
	pTabItemWnd->SetItemWnd(&m_dlgSidePanel, FALSE);
	pTabItemWnd->SetWindowLength(m_dlgSidePanel.GetDesireWidth());
	pTabItemWnd->SetAttribute(pTabItemWnd->ATTR_FIXLEN | pTabItemWnd->ATTR_TAIL);
	AddTab(pTabItemWnd);

	//�����ٶ�ָʾ���
	//CTabItem_Wnd	*pTabItemWnd2 = new CTabItem_Wnd;
	//pTabItemWnd2->SetItemWnd(&m_SpeedMeterDlg, FALSE);
	//pTabItemWnd2->SetWindowLength(m_SpeedMeterDlg.GetDesireWidth());
	//pTabItemWnd2->SetAttribute(pTabItemWnd2->ATTR_TAIL | pTabItemWnd2->ATTR_FILL);
	//AddTab(pTabItemWnd2);

	//InitAnchors();

	Localize();

	//{begin} VC-dgkang 2008��8��13��
	if (!thePrefs.m_bStartShowHomePage)
		CmdFuncs::SetMainActiveTab(CMainTabWnd::TI_DOWNLOAD);
	//{end}
	return bRet;
}

BOOL CMainTabWnd::IsTabShowed(ETabId eTabId)
{
	if (NULL == m_aposTabs[eTabId])
		return FALSE;
	else
		return TRUE;
}

void CMainTabWnd::AddTabById(ETabId eTabId)
{
	switch(eTabId)
	{
	case TI_ADVANCE:
		m_aposTabs[TI_ADVANCE] = CmdFuncs::TabWnd_AddMainButton(this, GetResString(IDS_ADVANCE),
																m_dlgAdvance.GetSafeHwnd(),
																_T("PNG_MAINTAB_ADVANCED_N"),
																_T("PNG_MAINTAB_ADVANCED_A"),
																TRUE, m_aposTabs[TI_BN], FALSE);
		CPreferences::m_bAdvancePageShowed = true;
		break;		
	default:
		break;
	}

}

void CMainTabWnd::RemoveTabById(ETabId eTabId)
{
	if (m_aposTabs[TI_ADVANCE] == m_aposTabs[eTabId])
		CPreferences::m_bAdvancePageShowed = false;
	
	RemoveTab(m_aposTabs[eTabId]);
	m_aposTabs[eTabId] = NULL;
}

void CMainTabWnd::Localize()
{
	SetTabText(m_aposTabs[TI_RESOURCE], GetResString(IDS_RESOURCE));
	SetTabText(m_aposTabs[TI_DOWNLOAD], GetResString(IDS_DOWNLOAD));
	SetTabText(m_aposTabs[TI_SHARE], GetResString(IDS_SHARE));
	SetTabText(m_aposTabs[TI_ADVANCE], GetResString(IDS_ADVANCE));

	SetTabText(m_aposTabs[TI_SEARCH], GetResString(IDS_SW_SEARCHBOX));
}
// CMainTabWnd ��Ϣ�������


void CMainTabWnd::OnSize(UINT nType, int cx, int cy)
{
	CTabWnd::OnSize(nType, cx, cy);

	/*CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd); 
	Invalidate(FALSE);*/
}

BOOL CMainTabWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	//return CTabWnd::OnEraseBkgnd(pDC);
}

//int CMainTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CTabWnd::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  �ڴ������ר�õĴ�������
//
//	return 0;
//}

int CMainTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPCHILDREN, 0);

	return 0;
}

void CMainTabWnd::OnActiveTabChanged(POSITION posOldActiveTab, POSITION posNewActiveTab)
{
	if( GetActiveTab() == m_aposTabs[CMainTabWnd::TI_RESOURCE] )
		m_dlgResource.OnMainTabChanged();

	else if( GetActiveTab() == m_aposTabs[CMainTabWnd::TI_SEARCH])
		m_dlgSearch.OnMainTabChanged();
}
//void CMainTabWnd::OnPaint()
//{
//	CPaintDC dc(this);
//	
//	CRect rtClient;
//	GetWindowRect(rtClient);
//	ScreenToClient(rtClient);
//	dc.FillRect(rtClient, &CBrush(RGB(255, 255, 255)));
//}
