//this file is part of eMule
//Copyright (C)2002-2008 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
#include "emule.h"
#include "emuledlg.h"
#include "SharedFilesCtrl.h"
#include "OtherFunctions.h"
#include "FileInfoDialog.h"
#include "MetaDataDlg.h"
#include "ED2kLinkDlg.h"
#include "ArchivePreviewDlg.h"
#include "CommentDialog.h"
#include "HighColorTab.hpp"
#include "ListViewWalkerPropertySheet.h"
#include "UserMsgs.h"
#include "ResizableLib/ResizableSheet.h"
#include "KnownFile.h"
#include "MapKey.h"
#include "SharedFileList.h"
#include "MemDC.h"
#include "PartFile.h"
#include "MenuCmds.h"
#include "IrcWnd.h"
#include "SharedFilesWnd.h"
#include "Opcodes.h"
#include "InputBox.h"
#include "WebServices.h"
#include "TransferWnd.h"
#include "ClientList.h"
#include "UpDownClient.h"
#include "Collection.h"
#include "CollectionCreateDialog.h"
#include "CollectionViewDialog.h"
#include "SharedDirsTreeCtrl.h"
#include "SearchParams.h"
#include "SearchDlg.h"
#include "SearchResultsWnd.h"
#include "Ini2.h"
#include "Preferences.h"
#include "UpdateInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Added by thilon on 2006.09.01, Comment
#ifndef IDC_HAND
	#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif

bool NeedArchiveInfoPage(const CSimpleArray<CObject*>* paItems);
void UpdateFileDetailsPages(CListViewWalkerPropertySheet *pSheet,
							CResizablePage *pArchiveInfo, CResizablePage *pMediaInfo);


//////////////////////////////////////////////////////////////////////////////
// CSharedFileDetailsSheet

class CSharedFileDetailsSheet : public CListViewWalkerPropertySheet
{
	DECLARE_DYNAMIC(CSharedFileDetailsSheet)

public:
	CSharedFileDetailsSheet(CTypedPtrList<CPtrList, CShareableFile*>& aFiles, UINT uPshInvokePage = 0, CListCtrlItemWalk* pListCtrl = NULL);
	virtual ~CSharedFileDetailsSheet();

protected:
	CFileInfoDialog		m_wndMediaInfo;
	CMetaDataDlg		m_wndMetaData;
	CED2kLinkDlg		m_wndFileLink;
	CCommentDialog		m_wndFileComments;
	CArchivePreviewDlg	m_wndArchiveInfo;

	UINT m_uPshInvokePage;
	static LPCTSTR m_pPshStartPage;

	void UpdateTitle();

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg LRESULT OnDataChanged(WPARAM, LPARAM);
};

LPCTSTR CSharedFileDetailsSheet::m_pPshStartPage;

IMPLEMENT_DYNAMIC(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)

BEGIN_MESSAGE_MAP(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)
	ON_WM_DESTROY()
	ON_MESSAGE(UM_DATA_CHANGED, OnDataChanged)
END_MESSAGE_MAP()

CSharedFileDetailsSheet::CSharedFileDetailsSheet(CTypedPtrList<CPtrList, CShareableFile*>& aFiles, UINT uPshInvokePage, CListCtrlItemWalk* pListCtrl)
	: CListViewWalkerPropertySheet(pListCtrl)
{
	m_uPshInvokePage = uPshInvokePage;
	POSITION pos = aFiles.GetHeadPosition();
	while (pos)
		m_aItems.Add(aFiles.GetNext(pos));
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_wndFileComments.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndFileComments.m_psp.dwFlags |= PSP_USEICONID;
	m_wndFileComments.m_psp.pszIcon = _T("FileComments");
	m_wndFileComments.SetFiles(&m_aItems);
	AddPage(&m_wndFileComments);

	m_wndArchiveInfo.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndArchiveInfo.m_psp.dwFlags |= PSP_USEICONID;
	m_wndArchiveInfo.m_psp.pszIcon = _T("ARCHIVE_PREVIEW");
	m_wndArchiveInfo.SetFiles(&m_aItems);
	m_wndMediaInfo.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndMediaInfo.m_psp.dwFlags |= PSP_USEICONID;
	m_wndMediaInfo.m_psp.pszIcon = _T("MEDIAINFO");
	m_wndMediaInfo.SetFiles(&m_aItems);
	if (NeedArchiveInfoPage(&m_aItems))
		AddPage(&m_wndArchiveInfo);
	else
		AddPage(&m_wndMediaInfo);

	m_wndMetaData.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndMetaData.m_psp.dwFlags |= PSP_USEICONID;
	m_wndMetaData.m_psp.pszIcon = _T("METADATA");
	if (m_aItems.GetSize() == 1 && thePrefs.IsExtControlsEnabled()) {
		m_wndMetaData.SetFiles(&m_aItems);
		AddPage(&m_wndMetaData);
	}

	m_wndFileLink.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndFileLink.m_psp.dwFlags |= PSP_USEICONID;
	m_wndFileLink.m_psp.pszIcon = _T("ED2KLINK");
	m_wndFileLink.SetFiles(&m_aItems);
	AddPage(&m_wndFileLink);

	LPCTSTR pPshStartPage = m_pPshStartPage;
	if (m_uPshInvokePage != 0)
		pPshStartPage = MAKEINTRESOURCE(m_uPshInvokePage);
	for (int i = 0; i < m_pages.GetSize(); i++)
	{
		CPropertyPage* pPage = GetPage(i);
		if (pPage->m_psp.pszTemplate == pPshStartPage)
		{
			m_psh.nStartPage = i;
			break;
		}
	}
}

CSharedFileDetailsSheet::~CSharedFileDetailsSheet()
{
}

void CSharedFileDetailsSheet::OnDestroy()
{
	if (m_uPshInvokePage == 0)
		m_pPshStartPage = GetPage(GetActiveIndex())->m_psp.pszTemplate;
	CListViewWalkerPropertySheet::OnDestroy();
}

BOOL CSharedFileDetailsSheet::OnInitDialog()
{
	EnableStackedTabs(FALSE);
	BOOL bResult = CListViewWalkerPropertySheet::OnInitDialog();
	HighColorTab::UpdateImageList(*this);
	InitWindowStyles(this);
	EnableSaveRestore(_T("SharedFileDetailsSheet")); // call this after(!) OnInitDialog
	UpdateTitle();
	return bResult;
}

LRESULT CSharedFileDetailsSheet::OnDataChanged(WPARAM, LPARAM)
{
	UpdateTitle();
	UpdateFileDetailsPages(this, &m_wndArchiveInfo, &m_wndMediaInfo);
	return 1;
}

void CSharedFileDetailsSheet::UpdateTitle()
{
	if (m_aItems.GetSize() == 1)
		SetWindowText(GetResString(IDS_DETAILS) + _T(": ") + STATIC_DOWNCAST(CKnownFile, m_aItems[0])->GetFileName());
	else
		SetWindowText(GetResString(IDS_DETAILS));
}

BOOL CSharedFileDetailsSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_APPLY_NOW)
	{
		CSharedFilesCtrl* pSharedFilesCtrl = DYNAMIC_DOWNCAST(CSharedFilesCtrl, m_pListCtrl->GetListCtrl());
		if (pSharedFilesCtrl)
		{
			for (int i = 0; i < m_aItems.GetSize(); i++) {
				// so, and why does this not(!) work while the sheet is open ??
				pSharedFilesCtrl->UpdateFile(DYNAMIC_DOWNCAST(CKnownFile, m_aItems[i]));
			}
		}
	}

	return CListViewWalkerPropertySheet::OnCommand(wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////////
// CSharedFilesCtrl

IMPLEMENT_DYNAMIC(CSharedFilesCtrl, CMuleListCtrl)

BEGIN_MESSAGE_MAP(CSharedFilesCtrl, CMuleListCtrl)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	//VeryCD Start
	//�����ļ��б��е����ۣ�added by Chocobo on 2006.09.01
	//��굥�����ƶ��¼�����
	ON_NOTIFY_REFLECT(NM_CLICK, OnCommentClick)

	ON_WM_MOUSEMOVE()
	//VeryCD End
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CSharedFilesCtrl::CSharedFilesCtrl()
	: CListCtrlItemWalk(this)
{
	memset(&sortstat, 0, sizeof(sortstat));
	nAICHHashing = 0;
	m_pDirectoryFilter = NULL;
	SetGeneralPurposeFind(true, false);
}

CSharedFilesCtrl::~CSharedFilesCtrl()
{
}

void CSharedFilesCtrl::Init()
{
	SetName(_T("SharedFilesCtrl"));
	CImageList ilDummyImageList; //dummy list for getting the proper height of listview entries
	ilDummyImageList.Create(1, theApp.GetSmallSytemIconSize().cy,theApp.m_iDfltImageListColorFlags|ILC_MASK, 1, 1); 
	SetImageList(&ilDummyImageList, LVSIL_SMALL);
	ASSERT( (GetStyle() & LVS_SHAREIMAGELISTS) == 0 );
	ilDummyImageList.Detach();

	SetExtendedStyle(LVS_EX_FULLROWSELECT);
	ModifyStyle(LVS_SINGLESEL,0);

	InsertColumn(0, GetResString(IDS_DL_FILENAME) ,LVCFMT_LEFT,250,0);
	//VeryCD Start
	//�����ļ��б��е����ۣ�added by Chocobo on 2006.09.01
	//��ӡ����ۡ���
	InsertColumn(1,GetResString(IDS_DL_COMMENT),LVCFMT_CENTER, 60);		
	//VeryCD End
	InsertColumn(2,GetResString(IDS_DL_SIZE),LVCFMT_LEFT,100,1);
	InsertColumn(3,GetResString(IDS_TYPE),LVCFMT_LEFT,50,2);
	InsertColumn(4,GetResString(IDS_PRIORITY),LVCFMT_LEFT,70,3);
	InsertColumn(5,GetResString(IDS_FILEID),LVCFMT_LEFT,220,4);
	InsertColumn(6,GetResString(IDS_SF_REQUESTS),LVCFMT_LEFT,100,5);
	InsertColumn(7,GetResString(IDS_SF_ACCEPTS),LVCFMT_LEFT,100,6);
	InsertColumn(8,GetResString(IDS_SF_TRANSFERRED),LVCFMT_LEFT,120,7);
	InsertColumn(9,GetResString(IDS_UPSTATUS),LVCFMT_LEFT,100,8);
	InsertColumn(10,GetResString(IDS_FOLDER),LVCFMT_LEFT,200,9);
	InsertColumn(11,GetResString(IDS_COMPLSOURCES),LVCFMT_LEFT,100,10);
	InsertColumn(12,GetResString(IDS_SHAREDTITLE),LVCFMT_LEFT,200,11);

	SetAllIcons();
	CreateMenues();
	LoadSettings();

	// Barry - Use preferred sort order from preferences
	SetSortArrow();
	SortItems(SortProc, GetSortItem() + (GetSortAscending() ? 0:20));
}

void CSharedFilesCtrl::OnSysColorChange()
{
	CMuleListCtrl::OnSysColorChange();
	SetAllIcons();
	CreateMenues();
}

void CSharedFilesCtrl::SetAllIcons()
{
	m_ImageList.DeleteImageList();
	m_ImageList.Create(16,16,theApp.m_iDfltImageListColorFlags|ILC_MASK,0,1);
	m_ImageList.SetBkColor(CLR_NONE);
	m_ImageList.Add(CTempIconLoader(_T("EMPTY")));
	m_ImageList.Add(CTempIconLoader(_T("FileSharedServer"), 16, 16));
	m_ImageList.Add(CTempIconLoader(_T("FileSharedKad"), 16, 16));
	m_ImageList.Add(CTempIconLoader(_T("Rating_NotRated")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Fake")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Poor")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Fair")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Good")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Excellent")));
	m_ImageList.Add(CTempIconLoader(_T("Collection_Search"))); // rating for comments are searched on kad
	m_ImageList.SetOverlayImage(m_ImageList.Add(CTempIconLoader(_T("FileCommentsOvl"))), 1);
}

void CSharedFilesCtrl::Localize()
{
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	HDITEM hdi;
	hdi.mask = HDI_TEXT;
	CString strRes;

	strRes = GetResString(IDS_DL_FILENAME);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(0, &hdi);

	//VeryCD Start
	//�����ļ��б��е����ۣ�added by Chocobo on 2006.09.01
	//����������
	strRes = GetResString(IDS_DL_COMMENT);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(1, &hdi);
	//VeryCD End

	strRes = GetResString(IDS_DL_SIZE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(2, &hdi);

	strRes = GetResString(IDS_TYPE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(3, &hdi);

	strRes = GetResString(IDS_PRIORITY);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(4, &hdi);

	strRes = GetResString(IDS_FILEID);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(5, &hdi);

	strRes = GetResString(IDS_SF_REQUESTS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(6, &hdi);

	strRes = GetResString(IDS_SF_ACCEPTS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(7, &hdi);

	strRes = GetResString(IDS_SF_TRANSFERRED);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(8, &hdi);

	strRes = GetResString(IDS_SHARED_STATUS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(9, &hdi);

	strRes = GetResString(IDS_FOLDER);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(10, &hdi);

	strRes = GetResString(IDS_COMPLSOURCES);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(11, &hdi);

	strRes = GetResString(IDS_SHAREDTITLE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(12, &hdi);

		CreateMenues();

	int iItems = GetItemCount();
	for (int i = 0; i < iItems; i++)
		Update(i);
}

void CSharedFilesCtrl::AddFile(const CShareableFile* file)
{
	if (!theApp.emuledlg->IsRunning())
		return;
	// check filter conditions if we should show this file right now
	if (m_pDirectoryFilter != NULL){
		ASSERT( file->IsKindOf(RUNTIME_CLASS(CKnownFile)) || m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY );
		switch(m_pDirectoryFilter->m_eItemType){
			case SDI_ALL:
				// No filter
				break;
			case SDI_FILESYSTEMPARENT:
				return;

			case SDI_UNSHAREDDIRECTORY:
				// Items from the whole filesystem tree
				if (file->IsPartFile())
					return;
			case SDI_NO:
				// some shared directory
			case SDI_CATINCOMING:
				// Categories with special incoming dirs
				if (CompareDirectories(file->GetSharedDirectory(), m_pDirectoryFilter->m_strFullPath) != 0)
					return;
				break;

			case SDI_TEMP:
				// only tempfiles
				if (!file->IsPartFile())
					return;
				else if (m_pDirectoryFilter->m_nCatFilter != -1 && (UINT)m_pDirectoryFilter->m_nCatFilter != ((CPartFile*)file)->GetCategory())
					return;
				break;

			case SDI_DIRECTORY:
				// any userselected shared dir but not incoming or temp
				if (file->IsPartFile())
					return;
				if (CompareDirectories(file->GetSharedDirectory(), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR)) == 0)
					return;
				break;

			case SDI_INCOMING:
				// Main incoming directory
				if (CompareDirectories(file->GetSharedDirectory(), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR)) != 0)
					return;
				// Hmm should we show all incoming files dirs or only those from the main incoming dir here?
				// hard choice, will only show the main for now
				break;

		}
	}
	//if (IsFilteredItem(file))
	//	return;
	if (FindFile(file) != -1)
	{
		// in the filesystem view the shared status might have changed so we need to update the item to redraw the checkbox
		if (m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY)
			UpdateFile(file);
		return;
	}
	
	// if we are in the filesystem view, this might be a CKnownFile which has to replace a CShareableFile
	// (in case we start sharing this file), so make sure to replace the old one instead of adding a new
/*
	if (m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY && file->IsKindOf(RUNTIME_CLASS(CKnownFile)))
	{
		for (POSITION pos = liTempShareableFilesInDir.GetHeadPosition(); pos != NULL; )
		{
			CShareableFile* pFile = liTempShareableFilesInDir.GetNext(pos);
			if (pFile->GetFilePath().CompareNoCase(file->GetFilePath()) == 0)
			{
				int iOldFile = FindFile(pFile);
				if (iOldFile != (-1))
				{
					SetItemData(iOldFile, (LPARAM)file);
					Update(iOldFile);
					ShowFilesCount();
					return;
				}
			}
		}
	}
*/

	int iItem = InsertItem(LVIF_TEXT|LVIF_PARAM, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, 0, (LPARAM)file);
	if (iItem >= 0)
		Update(iItem);
}

void CSharedFilesCtrl::RemoveFile(const CShareableFile* file, bool bDeletedFromDisk)
{
	int iItem = FindFile(file);
	if (iItem != -1)
	{
		if (!bDeletedFromDisk && m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY)
		{
			// in the file system view we usally dont need to remove a file, if it becomes unshared it will
			// still be visible as its still in the file system and the knownfile object doesn't gets deleted neither
			// so to avoid having to reload the whole list we just update it instead of removing and refinding
			UpdateFile(file);
			ShowFilesCount();
		}
		else
		{
		DeleteItem(iItem);
		ShowFilesCount();
	}
}
}

void CSharedFilesCtrl::UpdateFile(const CShareableFile* file, bool bUpdateFileSummary)
{
	if (!file || !theApp.emuledlg->IsRunning())
		return;
	int iItem = FindFile(file);
	if (iItem != -1)
	{
		Update(iItem);
		if (bUpdateFileSummary && GetItemState(iItem, LVIS_SELECTED))
			theApp.emuledlg->sharedfileswnd->ShowSelectedFilesSummary();
	}
}

int CSharedFilesCtrl::FindFile(const CShareableFile* pFile)
{
	LVFINDINFO find;
	find.flags = LVFI_PARAM;
	find.lParam = (LPARAM)pFile;
	return FindItem(&find);
}

void CSharedFilesCtrl::ReloadFileList()
{
	DeleteAllItems();
	theApp.emuledlg->sharedfileswnd->ShowSelectedFilesSummary();
	
	CCKey bufKey;
	CKnownFile* cur_file;
	for (POSITION pos = theApp.sharedfiles->m_Files_map.GetStartPosition(); pos != 0; ){
		theApp.sharedfiles->m_Files_map.GetNextAssoc(pos, bufKey, cur_file);
		AddFile(cur_file);
	}
/*
	if (m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY && !m_pDirectoryFilter->m_strFullPath.IsEmpty()){
		AddShareableFiles(m_pDirectoryFilter->m_strFullPath);
	}
	else {
		while (!liTempShareableFilesInDir.IsEmpty())	// cleanup temp filelist
			delete liTempShareableFilesInDir.RemoveHead();
	}
*/
	ShowFilesCount();
}

void CSharedFilesCtrl::ShowFilesCount()
{
	CString str;
	if (theApp.sharedfiles->GetHashingCount() + nAICHHashing)
		str.Format(_T(" (%i, %s %i)"), theApp.sharedfiles->GetCount(), GetResString(IDS_HASHING), theApp.sharedfiles->GetHashingCount() + nAICHHashing);
	else
		str.Format(_T(" (%i)"), theApp.sharedfiles->GetCount());
	theApp.emuledlg->sharedfileswnd->GetDlgItem(IDC_TRAFFIC_TEXT)->SetWindowText(GetResString(IDS_SF_FILES) + str);
}

#define DLC_DT_TEXT (DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_END_ELLIPSIS)

void CSharedFilesCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!theApp.emuledlg->IsRunning())
		return;
	if (!lpDrawItemStruct->itemData)
		return;
	CDC* odc = CDC::FromHandle(lpDrawItemStruct->hDC);
	BOOL bCtrlFocused = ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS));

	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		if (bCtrlFocused)
			odc->SetBkColor(m_crHighlight);
		else
			odc->SetBkColor(m_crNoHighlight);
	}
	else
		odc->SetBkColor(GetBkColor());
	/*const*/ CKnownFile* file = (CKnownFile*)lpDrawItemStruct->itemData;
	CMemDC dc(odc, &lpDrawItemStruct->rcItem);
	CFont font;
	LOGFONT lf = {0};
	CFont* pOldFont = dc.SelectObject(GetFont());
	CRect cur_rec(lpDrawItemStruct->rcItem);
	COLORREF crOldTextColor = dc.SetTextColor((lpDrawItemStruct->itemState & ODS_SELECTED) ? m_crHighlightText : m_crWindowText);

	int iOldBkMode;
	if (m_crWindowTextBk == CLR_NONE)
	{
		DefWindowProc(WM_ERASEBKGND, (WPARAM)(HDC)dc, 0);
		iOldBkMode = dc.SetBkMode(TRANSPARENT);
	}
	else
		iOldBkMode = OPAQUE;

	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	int iCount = pHeaderCtrl->GetItemCount();
	const int iMarginX = 4;
	cur_rec.right = cur_rec.left - iMarginX*2;
	cur_rec.left += iMarginX;
	CString buffer;
	int iIconDrawWidth = theApp.GetSmallSytemIconSize().cx + 3;
	for(int iCurrent = 0; iCurrent < iCount; iCurrent++)
	{
		int iColumn = pHeaderCtrl->OrderToIndex(iCurrent);
		if( !IsColumnHidden(iColumn) )
		{
			UINT uDTFlags = DLC_DT_TEXT;
			cur_rec.right += GetColumnWidth(iColumn);
			switch(iColumn){
				case 0:{
					int iImage = theApp.GetFileTypeSystemImageIdx(file->GetFileName());
					if (theApp.GetSystemImageList() != NULL)
						::ImageList_Draw(theApp.GetSystemImageList(), iImage, dc.GetSafeHdc(), cur_rec.left, cur_rec.top, ILD_NORMAL|ILD_TRANSPARENT);
					if (!file->GetFileComment().IsEmpty() || file->GetFileRating())
						m_ImageList.Draw(dc, 0, CPoint(cur_rec.left, cur_rec.top), ILD_NORMAL | ILD_TRANSPARENT | INDEXTOOVERLAYMASK(1));
					cur_rec.left += (iIconDrawWidth - 3);

					if (thePrefs.ShowRatingIndicator() && (file->HasComment() || file->HasRating() || file->IsKadCommentSearchRunning()))
					{
						m_ImageList.Draw(dc, file->UserRating(true)+3, CPoint(cur_rec.left, cur_rec.top), ILD_NORMAL);
						cur_rec.left += 16;
						iIconDrawWidth += 16;
					}

					cur_rec.left += 3;

					buffer = file->GetFileName();
					break;
				}
				//VeryCD Start
				//�����ļ��б��е����ۣ�added by Chocobo on 2006.09.01
			    //��ʾ��������
				case 1:
					buffer = GetResString(IDS_DL_COMMENT);

					
					lf.lfHeight = 12;

					lf.lfUnderline = 1;
					lf.lfQuality = afxData.bWin95 ? NONANTIALIASED_QUALITY : ANTIALIASED_QUALITY;
					_tcscpy(lf.lfFaceName, _T("����"));

					
					font.CreateFontIndirect(&lf);
					pOldFont = dc->SelectObject(&font);
					dc->SetTextColor(RGB(0,0,255));
					dc->DrawText(buffer,buffer.GetLength(),&cur_rec, DLC_DT_TEXT | DT_CENTER);
					dc->SetTextColor(RGB(0,0,0));
					dc->SelectObject(pOldFont);
					break;
				//VeryCD End
				case 2:
					buffer = CastItoXBytes(file->GetFileSize(), false, false);
					uDTFlags |= DT_RIGHT;
					break;
				case 3:
					buffer = file->GetFileTypeDisplayStr();
					break;
				case 4:{
					switch (file->GetUpPriority()) {
						case PR_VERYLOW :
							buffer = GetResString(IDS_PRIOVERYLOW);
							break;
						case PR_LOW :
							if( file->IsAutoUpPriority() )
								buffer = GetResString(IDS_PRIOAUTOLOW);
							else
								buffer = GetResString(IDS_PRIOLOW);
							break;
						case PR_NORMAL :
							if( file->IsAutoUpPriority() )
								buffer = GetResString(IDS_PRIOAUTONORMAL);
							else
								buffer = GetResString(IDS_PRIONORMAL);
							break;
						case PR_HIGH :
							if( file->IsAutoUpPriority() )
								buffer = GetResString(IDS_PRIOAUTOHIGH);
							else
								buffer = GetResString(IDS_PRIOHIGH);
							break;
						case PR_VERYHIGH :
							buffer = GetResString(IDS_PRIORELEASE);
							break;
						default:
							buffer.Empty();
					}
					break;
				}
				case 5:
					buffer = md4str(file->GetFileHash());
					break;
				case 6:
                    buffer.Format(_T("%u (%u)"), file->statistic.GetRequests(), file->statistic.GetAllTimeRequests());
					break;
				case 7:
					buffer.Format(_T("%u (%u)"), file->statistic.GetAccepts(), file->statistic.GetAllTimeAccepts());
					break;
				case 8:
					buffer.Format(_T("%s (%s)"), CastItoXBytes(file->statistic.GetTransferred(), false, false), CastItoXBytes(file->statistic.GetAllTimeTransferred(), false, false));
					break;
				case 9:
					if (file->GetPartCount()){
						cur_rec.bottom--;
						cur_rec.top++;
						file->DrawShareStatusBar(dc,&cur_rec,false,thePrefs.UseFlatBar());
						cur_rec.bottom++;
						cur_rec.top--;
					}
					break;
				case 10:
					buffer = file->GetPath();
					PathRemoveBackslash(buffer.GetBuffer());
					buffer.ReleaseBuffer();
					break;
				case 11:
                    if (file->m_nCompleteSourcesCountLo == file->m_nCompleteSourcesCountHi)
						buffer.Format(_T("%u"), file->m_nCompleteSourcesCountLo);
                    else if (file->m_nCompleteSourcesCountLo == 0)
						buffer.Format(_T("< %u"), file->m_nCompleteSourcesCountHi);
					else
						buffer.Format(_T("%u - %u"), file->m_nCompleteSourcesCountLo, file->m_nCompleteSourcesCountHi);
					break;
				case 12:{
					CPoint pt(cur_rec.left, cur_rec.top);
					m_ImageList.Draw(dc, file->GetPublishedED2K() ? 1 : 0, pt, ILD_NORMAL | ILD_TRANSPARENT);
					pt.x += 16;
					bool bSharedInKad;
					if ((uint32)time(NULL) < file->GetLastPublishTimeKadSrc())
					{
						if (theApp.IsFirewalled() && theApp.IsConnected())
						{
							if (theApp.clientlist->GetBuddy() && (file->GetLastPublishBuddy() == theApp.clientlist->GetBuddy()->GetIP()))
								bSharedInKad = true;
							else
								bSharedInKad = false;
						}
						else
							bSharedInKad = true;
					}
					else
						bSharedInKad = false;
					m_ImageList.Draw(dc, bSharedInKad ? 2 : 0, pt, ILD_NORMAL | ILD_TRANSPARENT);
					buffer.Empty();
					break;
				}
			}

			if (iColumn != 9 && iColumn != 1)
				dc.DrawText(buffer, buffer.GetLength(), &cur_rec, uDTFlags);
			if (iColumn == 0)
				cur_rec.left -= iIconDrawWidth;
			cur_rec.left += GetColumnWidth(iColumn);
		}
	}
	ShowFilesCount();
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		RECT outline_rec = lpDrawItemStruct->rcItem;

		outline_rec.top--;
		outline_rec.bottom++;
		dc.FrameRect(&outline_rec, &CBrush(m_crWindow));
		outline_rec.top++;
		outline_rec.bottom--;
		outline_rec.left++;
		outline_rec.right--;

		if (lpDrawItemStruct->itemID > 0 && GetItemState(lpDrawItemStruct->itemID - 1, LVIS_SELECTED))
			outline_rec.top--;

		if (lpDrawItemStruct->itemID + 1 < (UINT)GetItemCount() && GetItemState(lpDrawItemStruct->itemID + 1, LVIS_SELECTED))
			outline_rec.bottom++;

		if(bCtrlFocused)
			dc.FrameRect(&outline_rec, &CBrush(m_crFocusLine));
		else
			dc.FrameRect(&outline_rec, &CBrush(m_crNoFocusLine));
	}
	
	if (m_crWindowTextBk == CLR_NONE)
		dc.SetBkMode(iOldBkMode);
	dc.SelectObject(pOldFont);
	dc.SetTextColor(crOldTextColor);
}

void CSharedFilesCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// get merged settings
	bool bFirstItem = true;
	int iSelectedItems = GetSelectedCount();
	int iCompleteFileSelected = -1;
	UINT uPrioMenuItem = 0;
	const CShareableFile* pSingleSelFile = NULL;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		const CShareableFile* pFile = (CShareableFile*)GetItemData(GetNextSelectedItem(pos));
		if (bFirstItem)
			pSingleSelFile = pFile;
		else
			pSingleSelFile = NULL;

		int iCurCompleteFile = pFile->IsPartFile() ? 0 : 1;
		if (bFirstItem)
			iCompleteFileSelected = iCurCompleteFile;
		else if (iCompleteFileSelected != iCurCompleteFile)
			iCompleteFileSelected = -1;

		UINT uCurPrioMenuItem = 0;
			if (((CKnownFile*)pFile)->IsAutoUpPriority())
			uCurPrioMenuItem = MP_PRIOAUTO;
			else if (((CKnownFile*)pFile)->GetUpPriority() == PR_VERYLOW)
			uCurPrioMenuItem = MP_PRIOVERYLOW;
			else if (((CKnownFile*)pFile)->GetUpPriority() == PR_LOW)
			uCurPrioMenuItem = MP_PRIOLOW;
			else if (((CKnownFile*)pFile)->GetUpPriority() == PR_NORMAL)
			uCurPrioMenuItem = MP_PRIONORMAL;
			else if (((CKnownFile*)pFile)->GetUpPriority() == PR_HIGH)
			uCurPrioMenuItem = MP_PRIOHIGH;
			else if (((CKnownFile*)pFile)->GetUpPriority() == PR_VERYHIGH)
			uCurPrioMenuItem = MP_PRIOVERYHIGH;
		else
			ASSERT(0);

		if (bFirstItem)
			uPrioMenuItem = uCurPrioMenuItem;
		else if (uPrioMenuItem != uCurPrioMenuItem)
			uPrioMenuItem = 0;

		bFirstItem = false;
	}

	m_SharedFilesMenu.EnableMenuItem((UINT_PTR)m_PrioMenu.m_hMenu, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	m_PrioMenu.CheckMenuRadioItem(MP_PRIOVERYLOW, MP_PRIOAUTO, uPrioMenuItem, 0);

	bool bSingleCompleteFileSelected = (iSelectedItems == 1 && iCompleteFileSelected == 1);
	m_SharedFilesMenu.EnableMenuItem(MP_OPEN, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);
	UINT uInsertedMenuItem = 0;
	static const TCHAR _szSkinPkgSuffix1[] = _T(".") EMULSKIN_BASEEXT _T(".zip");
	static const TCHAR _szSkinPkgSuffix2[] = _T(".") EMULSKIN_BASEEXT _T(".rar");
	if (bSingleCompleteFileSelected 
		&& pSingleSelFile 
		&& (   pSingleSelFile->GetFilePath().Right(ARRSIZE(_szSkinPkgSuffix1)-1).CompareNoCase(_szSkinPkgSuffix1) == 0
		    || pSingleSelFile->GetFilePath().Right(ARRSIZE(_szSkinPkgSuffix2)-1).CompareNoCase(_szSkinPkgSuffix2) == 0))
	{
		MENUITEMINFO mii = {0};
		mii.cbSize = sizeof mii;
		mii.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID;
		mii.fType = MFT_STRING;
		mii.fState = MFS_ENABLED;
		mii.wID = MP_INSTALL_SKIN;
		CString strBuff(GetResString(IDS_INSTALL_SKIN));
		mii.dwTypeData = const_cast<LPTSTR>((LPCTSTR)strBuff);
		if (::InsertMenuItem(m_SharedFilesMenu, MP_OPENFOLDER, FALSE, &mii))
			uInsertedMenuItem = mii.wID;
	}
	m_SharedFilesMenu.EnableMenuItem(MP_OPENFOLDER, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(MP_RENAME, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(MP_REMOVE, iCompleteFileSelected > 0 ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.SetDefaultItem(bSingleCompleteFileSelected ? MP_OPEN : -1);
	m_SharedFilesMenu.EnableMenuItem(MP_CMT, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(MP_DETAIL, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(thePrefs.GetShowCopyEd2kLinkCmd() ? MP_GETED2KLINK : MP_SHOWED2KLINK, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(MP_FIND, GetItemCount() > 0 ? MF_ENABLED : MF_GRAYED);
	m_SharedFilesMenu.EnableMenuItem(MP_VIRUS, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);		//Added by thilon on 2006.08.28

	m_CollectionsMenu.EnableMenuItem(MP_MODIFYCOLLECTION, ( pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL ) ? MF_ENABLED : MF_GRAYED);
	m_CollectionsMenu.EnableMenuItem(MP_VIEWCOLLECTION, ( pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL ) ? MF_ENABLED : MF_GRAYED);
	m_CollectionsMenu.EnableMenuItem(MP_SEARCHAUTHOR, ( pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL && !((CKnownFile*)pSingleSelFile)->m_pCollection->GetAuthorKeyHashString().IsEmpty()) ? MF_ENABLED : MF_GRAYED);
#if defined(_DEBUG)
	if (thePrefs.IsExtControlsEnabled()){
	//JOHNTODO: Not for release as we need kad lowID users in the network to see how well this work work. Also, we do not support these links yet.
		if (iSelectedItems > 0 && theApp.IsConnected() && theApp.IsFirewalled() && theApp.clientlist->GetBuddy())
			m_SharedFilesMenu.EnableMenuItem(MP_GETKADSOURCELINK, MF_ENABLED);
		else
			m_SharedFilesMenu.EnableMenuItem(MP_GETKADSOURCELINK, MF_GRAYED);
	}
#endif
	m_SharedFilesMenu.EnableMenuItem(Irc_SetSendLink, iSelectedItems == 1 && theApp.emuledlg->ircwnd->IsConnected() ? MF_ENABLED : MF_GRAYED);

	CTitleMenu WebMenu;
	WebMenu.CreateMenu();
	WebMenu.AddMenuTitle(NULL, true);
	int iWebMenuEntries = theWebServices.GetFileMenuEntries(&WebMenu);
	UINT flag2 = (iWebMenuEntries == 0 || iSelectedItems != 1) ? MF_GRAYED : MF_STRING;
	m_SharedFilesMenu.AppendMenu(flag2 | MF_POPUP, (UINT_PTR)WebMenu.m_hMenu, GetResString(IDS_WEBSERVICES), _T("WEB"));

	GetPopupMenuPos(*this, point);
	m_SharedFilesMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);

	m_SharedFilesMenu.RemoveMenu(m_SharedFilesMenu.GetMenuItemCount()-1,MF_BYPOSITION);
	VERIFY( WebMenu.DestroyMenu() );
	if (uInsertedMenuItem)
		VERIFY( m_SharedFilesMenu.RemoveMenu(uInsertedMenuItem, MF_BYCOMMAND) );
}

BOOL CSharedFilesCtrl::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	wParam = LOWORD(wParam);

	CTypedPtrList<CPtrList, CShareableFile*> selectedList;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos != NULL){
		int index = GetNextSelectedItem(pos);
		if (index >= 0)
			selectedList.AddTail((CShareableFile*)GetItemData(index));
	}

	if (   wParam == MP_CREATECOLLECTION
		|| wParam == MP_FIND
		|| selectedList.GetCount() > 0)
	{
		CShareableFile* file = NULL;
		if (selectedList.GetCount() == 1)
			file = selectedList.GetHead();

		CKnownFile* pKnownFile = NULL;
		if (file != NULL && file->IsKindOf(RUNTIME_CLASS(CKnownFile)))
			pKnownFile = (CKnownFile*)file;

		switch (wParam){
			case Irc_SetSendLink:
				if (pKnownFile != NULL)
					theApp.emuledlg->ircwnd->SetSendFileString(CreateED2kLink(pKnownFile));
				break;
			case MP_GETED2KLINK:{
				CString str;
				POSITION pos = selectedList.GetHeadPosition();
				while (pos != NULL)
				{
					CShareableFile* file = selectedList.GetNext(pos);
					if (file->IsKindOf(RUNTIME_CLASS(CKnownFile))){
					if (!str.IsEmpty())
						str += _T("\r\n");
						str += CreateED2kLink((CKnownFile*)file);
					}
				}
				theApp.CopyTextToClipboard(str);
				break;
			}
#if defined(_DEBUG)
			//JOHNTODO: Not for release as we need kad lowID users in the network to see how well this work work. Also, we do not support these links yet.
			case MP_GETKADSOURCELINK:{
				CString str;
				POSITION pos = selectedList.GetHeadPosition();
				while (pos != NULL)
				{
					CShareableFile* file = selectedList.GetNext(pos);
					if (file->IsKindOf(RUNTIME_CLASS(CKnownFile))){
					if (!str.IsEmpty())
						str += _T("\r\n");
						str += theApp.CreateKadSourceLink((CKnownFile*)file);
					}
				}
				theApp.CopyTextToClipboard(str);
				break;
			}
#endif
			// file operations
			case MP_OPEN:
			case IDA_ENTER:
				if (file && !file->IsPartFile())
					OpenFile(file);
				break; 
			case MP_INSTALL_SKIN:
				if (file && !file->IsPartFile())
					InstallSkin(file->GetFilePath());
				break;
			case MP_OPENFOLDER:
				if (file && !file->IsPartFile())
					ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,\"") + file->GetFilePath() + _T("\""), NULL, SW_SHOW);
				break; 
			case MP_RENAME:
			case MPG_F2:
				if (pKnownFile && !pKnownFile->IsPartFile()){
					InputBox inputbox;
					CString title = GetResString(IDS_RENAME);
					title.Remove(_T('&'));
					inputbox.SetLabels(title, GetResString(IDS_DL_FILENAME), pKnownFile->GetFileName());
					inputbox.SetEditFilenameMode();
					inputbox.DoModal();
					CString newname = inputbox.GetInput();
					if (!inputbox.WasCancelled() && newname.GetLength()>0)
					{
						// at least prevent users from specifying something like "..\dir\file"
						static const TCHAR _szInvFileNameChars[] = _T("\\/:*?\"<>|");
						if (newname.FindOneOf(_szInvFileNameChars) != -1){
							AfxMessageBox(GetErrorMessage(ERROR_BAD_PATHNAME));
							break;
						}

						CString newpath;
						PathCombine(newpath.GetBuffer(MAX_PATH), file->GetPath(), newname);
						newpath.ReleaseBuffer();
						if (_trename(pKnownFile->GetFilePath(), newpath) != 0){
							CString strError;
							strError.Format(GetResString(IDS_ERR_RENAMESF), file->GetFilePath(), newpath, _tcserror(errno));
							AfxMessageBox(strError);
							break;
						}

						if (pKnownFile->IsKindOf(RUNTIME_CLASS(CPartFile)))
						{
							pKnownFile->SetFileName(newname);
							STATIC_DOWNCAST(CPartFile, pKnownFile)->SetFullName(newpath); 
						}
						else
						{
							theApp.sharedfiles->RemoveKeywords(pKnownFile);
							pKnownFile->SetFileName(newname);
							theApp.sharedfiles->AddKeywords(pKnownFile);
						}
						pKnownFile->SetFilePath(newpath);
						UpdateFile(pKnownFile);
					}
				}
				else
					MessageBeep(MB_OK);
				break;
			case MP_REMOVE:
			case MPG_DELETE:
			{
				if (IDNO == AfxMessageBox(GetResString(IDS_CONFIRM_FILEDELETE),MB_ICONWARNING | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_YESNO))
					return TRUE;

				SetRedraw(FALSE);
				bool bRemovedItems = false;
				while (!selectedList.IsEmpty())
				{
					CShareableFile* myfile = selectedList.RemoveHead();

					//Added by thilon on 2007.01.11
					CUpdateInfo updateinfo;
					CString hash = md4str(myfile->GetFileHash());

					if(updateinfo.isUpdateFile(hash))
					{
						if(updateinfo.UpdateInfoFileExists())
						{
							DeleteFile(updateinfo.GetUpdateInfoFile());
						}
					}

					if (!myfile || myfile->IsPartFile())
						continue;
					
					BOOL delsucc = FALSE;
					if (!PathFileExists(myfile->GetFilePath()))
						delsucc = TRUE;
					else{
						// Delete
						if (!thePrefs.GetRemoveToBin()){
							delsucc = DeleteFile(myfile->GetFilePath());
						}
						else{
							// delete to recycle bin :(
							TCHAR todel[MAX_PATH+1];
							memset(todel, 0, sizeof todel);
							_tcsncpy(todel, myfile->GetFilePath(), ARRSIZE(todel)-2);

							SHFILEOPSTRUCT fp = {0};
							fp.wFunc = FO_DELETE;
							fp.hwnd = theApp.emuledlg->m_hWnd;
							fp.pFrom = todel;
							fp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;// | FOF_NOERRORUI
							delsucc = (SHFileOperation(&fp) == 0);
						}
					}
					if (delsucc){
						if (myfile->IsKindOf(RUNTIME_CLASS(CKnownFile))) 
							theApp.sharedfiles->RemoveFile((CKnownFile*)myfile, true);
						else
							RemoveFile(myfile, true);
						bRemovedItems = true;
						if (myfile->IsKindOf(RUNTIME_CLASS(CPartFile)))
							theApp.emuledlg->transferwnd->downloadlistctrl.ClearCompleted(static_cast<CPartFile*>(myfile));
					}
					else{
						CString strError;
						strError.Format( GetResString(IDS_ERR_DELFILE) + _T("\r\n\r\n%s"), myfile->GetFilePath(), GetErrorMessage(GetLastError()));
						AfxMessageBox(strError);
					}
				}
				SetRedraw(TRUE);
				if (bRemovedItems) {
					AutoSelectItem();
					// Depending on <no-idea> this does not always cause a
					// LVN_ITEMACTIVATE message sent. So, explicitly redraw
					// the item.
					theApp.emuledlg->sharedfileswnd->ShowSelectedFilesSummary();
				}
				break; 
			}
			case MP_CMT:
				ShowFileDialog(selectedList, IDD_COMMENT);
                break; 
			case MPG_ALTENTER:
			case MP_DETAIL:
				ShowFileDialog(selectedList);
				break;
			case MP_FIND:
				OnFindStart();
				break;
			case MP_CREATECOLLECTION:
			{
				CCollection* pCollection = new CCollection();
				POSITION pos = selectedList.GetHeadPosition();
				while (pos != NULL)
				{
					CShareableFile* pFile = selectedList.GetNext(pos);
					if (pFile->IsKindOf(RUNTIME_CLASS(CKnownFile)))
						pCollection->AddFileToCollection(pFile, true);
				}
				CCollectionCreateDialog dialog;
				dialog.SetCollection(pCollection,true);
				dialog.DoModal();
				//We delete this collection object because when the newly created
				//collection file is added to the sharedfile list, it is read and verified
				//and which creates the colleciton object that is attached to that file..
				delete pCollection;
				break;
			}
			case MP_SEARCHAUTHOR:
				if (pKnownFile && pKnownFile->m_pCollection)
				{
					SSearchParams* pParams = new SSearchParams;
					pParams->strExpression = pKnownFile->m_pCollection->GetCollectionAuthorKeyString();
					pParams->eType = SearchTypeKademlia;
					pParams->strFileType = ED2KFTSTR_EMULECOLLECTION;
					pParams->strSpecialTitle = pKnownFile->m_pCollection->m_sCollectionAuthorName;
					if (pParams->strSpecialTitle.GetLength() > 50){
						pParams->strSpecialTitle = pParams->strSpecialTitle.Left(50) + _T("...");
					}
					if(theApp.emuledlg->searchwnd->m_pwndResults)
						theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pParams);
				}
				break;
			case MP_VIEWCOLLECTION:
				if (pKnownFile && pKnownFile->m_pCollection)
				{
					CCollectionViewDialog dialog;
					dialog.SetCollection(pKnownFile->m_pCollection);
					dialog.DoModal();
				}
				break;
			case MP_MODIFYCOLLECTION:
				if (pKnownFile && pKnownFile->m_pCollection)
				{
					CCollectionCreateDialog dialog;
					CCollection* pCollection = new CCollection(pKnownFile->m_pCollection);
					dialog.SetCollection(pCollection,false);
					dialog.DoModal();
					delete pCollection;				
				}
				break;
			case MP_SHOWED2KLINK:
				ShowFileDialog(selectedList, IDD_ED2KLINK);
				break;
			case MP_PRIOVERYLOW:
			case MP_PRIOLOW:
			case MP_PRIONORMAL:
			case MP_PRIOHIGH:
			case MP_PRIOVERYHIGH:
			case MP_PRIOAUTO:
				{
					POSITION pos = selectedList.GetHeadPosition();
					while (pos != NULL)
					{
						if (!selectedList.GetAt(pos)->IsKindOf(RUNTIME_CLASS(CKnownFile)))
							continue;
						CKnownFile* file = (CKnownFile*)selectedList.GetNext(pos);
						switch (wParam) {
							case MP_PRIOVERYLOW:
								file->SetAutoUpPriority(false);
								file->SetUpPriority(PR_VERYLOW);
								UpdateFile(file);
								break;
							case MP_PRIOLOW:
								file->SetAutoUpPriority(false);
								file->SetUpPriority(PR_LOW);
								UpdateFile(file);
								break;
							case MP_PRIONORMAL:
								file->SetAutoUpPriority(false);
								file->SetUpPriority(PR_NORMAL);
								UpdateFile(file);
								break;
							case MP_PRIOHIGH:
								file->SetAutoUpPriority(false);
								file->SetUpPriority(PR_HIGH);
								UpdateFile(file);
								break;
							case MP_PRIOVERYHIGH:
								file->SetAutoUpPriority(false);
								file->SetUpPriority(PR_VERYHIGH);
								UpdateFile(file);
								break;	
							case MP_PRIOAUTO:
								file->SetAutoUpPriority(true);
								file->UpdateAutoUpPriority();
								UpdateFile(file); 
								break;
						}
					}
					break;
				}

			//Added by thilon 2006.08.28
			case MP_VIRUS:
				{
					CString str;
					CString url;

					POSITION pos = selectedList.GetHeadPosition();

					if(pos != NULL)
					{
						file = selectedList.GetNext(pos);
						str = m_JavaScriptEscape.Escape(file->GetFilePath());

						url = _T("http://zs.kingsoft.com/EmuleOnlineScan/index.html?ScanPath=") + str + _T("&ScanType=0&IsAutoScan=1");
						ShellOpenFile(url);

					}
					break;
				}
			default:
				if (file && wParam>=MP_WEBURL && wParam<=MP_WEBURL+256){
					theWebServices.RunURL(file, wParam);
				}
				break;
		}
	}
	return TRUE;
}

void CSharedFilesCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Barry - Store sort order in preferences
	// Determine ascending based on whether already sorted on this column

	bool sortAscending = (GetSortItem() != pNMListView->iSubItem) ? true : !GetSortAscending();

	// Ornis 4-way-sorting
	int adder=0;
	if (pNMListView->iSubItem>=5 && pNMListView->iSubItem<=7)
	{
		ASSERT( pNMListView->iSubItem - 5 < ARRSIZE(sortstat) );
		if (!sortAscending)
			sortstat[pNMListView->iSubItem - 5] = !sortstat[pNMListView->iSubItem - 5];
		adder = sortstat[pNMListView->iSubItem-5] ? 0 : 100;
	}
	else if (pNMListView->iSubItem==11)
	{
		ASSERT( 3 < ARRSIZE(sortstat) );
		if (!sortAscending)
			sortstat[3] = !sortstat[3];
		adder = sortstat[3] ? 0 : 100;
	}

	// Sort table
	if (adder==0)	
		SetSortArrow(pNMListView->iSubItem, sortAscending); 
	else
		SetSortArrow(pNMListView->iSubItem, sortAscending ? arrowDoubleUp : arrowDoubleDown);

	UpdateSortHistory(pNMListView->iSubItem + adder + (sortAscending ? 0:20),20);
	SortItems(SortProc, pNMListView->iSubItem + adder + (sortAscending ? 0:20));

	*pResult = 0;
}

int CSharedFilesCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CShareableFile* item1 = (CShareableFile*)lParam1;
	const CShareableFile* item2 = (CShareableFile*)lParam2;
	CKnownFile* kitem1 = (CKnownFile*)item1;
	CKnownFile* kitem2 = (CKnownFile*)item2;
	
	int iResult=0;
	//VeryCD Start
	//�����ļ������е����ۣ�modified by Chocobo on 2006.09.01
	//���1�������ۣ���1��ʼ�ı�Ŷ�����1
	switch(lParamSort){
		case 0: //filename asc
			iResult=CompareLocaleStringNoCase(item1->GetFileName(),item2->GetFileName());
			break;
		case 20: //filename desc
			iResult=CompareLocaleStringNoCase(item2->GetFileName(),item1->GetFileName());
			break;

		case 2: //filesize asc
			iResult=item1->GetFileSize()==item2->GetFileSize()?0:(item1->GetFileSize()>item2->GetFileSize()?1:-1);
			break;

		case 22: //filesize desc
			iResult=item1->GetFileSize()==item2->GetFileSize()?0:(item2->GetFileSize()>item1->GetFileSize()?1:-1);
			break;

		case 3: //filetype asc
			iResult=item1->GetFileTypeDisplayStr().Compare(item2->GetFileTypeDisplayStr());
			break;
		case 23: //filetype desc
			iResult=item2->GetFileTypeDisplayStr().Compare(item1->GetFileTypeDisplayStr());
			break;

		case 4: //prio asc
		{
			uint8 p1=kitem1->GetUpPriority() +1;
			if(p1==5)
				p1=0;
			uint8 p2=kitem2->GetUpPriority() +1;
			if(p2==5)
				p2=0;
			iResult=p1-p2;
			break;
		}
		case 24: //prio desc
		{
			uint8 p1=kitem1->GetUpPriority() +1;
			if(p1==5)
				p1=0;
			uint8 p2=kitem2->GetUpPriority() +1;
			if(p2==5)
				p2=0;
			iResult=p2-p1;
			break;
		}

		case 5: //fileID asc
			iResult=memcmp(item1->GetFileHash(), item2->GetFileHash(), 16);
			break;
		case 25: //fileID desc
			iResult=memcmp(item2->GetFileHash(), item1->GetFileHash(), 16);
			break;

		case 6: //requests asc
			iResult=kitem1->statistic.GetRequests() - kitem2->statistic.GetRequests();
			break;
		case 26: //requests desc
			iResult=kitem2->statistic.GetRequests() - kitem1->statistic.GetRequests();
			break;
		
		case 7: //acc requests asc
			iResult=kitem1->statistic.GetAccepts() - kitem2->statistic.GetAccepts();
			break;
		case 27: //acc requests desc
			iResult=kitem2->statistic.GetAccepts() - kitem1->statistic.GetAccepts();
			break;
		
		case 8: //all transferred asc
			iResult=kitem1->statistic.GetTransferred()==kitem2->statistic.GetTransferred()?0:(kitem1->statistic.GetTransferred()>kitem2->statistic.GetTransferred()?1:-1);
			break;
		case 28: //all transferred desc
			iResult=kitem1->statistic.GetTransferred()==kitem2->statistic.GetTransferred()?0:(kitem2->statistic.GetTransferred()>kitem1->statistic.GetTransferred()?1:-1);
			break;

		case 10: //folder asc
			iResult=CompareLocaleStringNoCase(item1->GetPath(),item2->GetPath());
			break;
		case 30: //folder desc
			iResult=CompareLocaleStringNoCase(item2->GetPath(),item1->GetPath());
			break;

		case 11: //complete sources asc
			iResult=CompareUnsigned(kitem1->m_nCompleteSourcesCount, kitem2->m_nCompleteSourcesCount);
			break;
		case 31: //complete sources desc
			iResult=CompareUnsigned(kitem2->m_nCompleteSourcesCount, kitem1->m_nCompleteSourcesCount);
			break;

		case 12: //ed2k shared asc
			iResult=kitem1->GetPublishedED2K() - kitem2->GetPublishedED2K();
			break;
		case 32: //ed2k shared desc
			iResult=kitem2->GetPublishedED2K() - kitem1->GetPublishedED2K();
			break;

		case 106: //all requests asc
			iResult=CompareUnsigned(kitem1->statistic.GetAllTimeRequests(), kitem2->statistic.GetAllTimeRequests());
			break;
		case 126: //all requests desc
			iResult=CompareUnsigned(kitem2->statistic.GetAllTimeRequests(), kitem1->statistic.GetAllTimeRequests());
			break;

		case 107: //all acc requests asc
			iResult=CompareUnsigned(kitem1->statistic.GetAllTimeAccepts(), kitem2->statistic.GetAllTimeAccepts());
			break;
		case 127: //all acc requests desc
			iResult=CompareUnsigned(kitem2->statistic.GetAllTimeAccepts(), kitem1->statistic.GetAllTimeAccepts());
			break;

		case 108: //all transferred asc
			iResult=kitem1->statistic.GetAllTimeTransferred()==kitem2->statistic.GetAllTimeTransferred()?0:(kitem1->statistic.GetAllTimeTransferred()>kitem2->statistic.GetAllTimeTransferred()?1:-1);
			break;
		case 128: //all transferred desc
			iResult=kitem1->statistic.GetAllTimeTransferred()==kitem2->statistic.GetAllTimeTransferred()?0:(kitem2->statistic.GetAllTimeTransferred()>kitem1->statistic.GetAllTimeTransferred()?1:-1);
			break;

		case 112:{ //kad shared asc
			uint32 tNow = time(NULL);
			int i1 = (tNow < kitem1->GetLastPublishTimeKadSrc()) ? 1 : 0;
			int i2 = (tNow < kitem2->GetLastPublishTimeKadSrc()) ? 1 : 0;
			iResult=i1 - i2;
			break;
		}
		case 132:{ //kad shared desc
			uint32 tNow = time(NULL);
			int i1 = (tNow < kitem1->GetLastPublishTimeKadSrc()) ? 1 : 0;
			int i2 = (tNow < kitem2->GetLastPublishTimeKadSrc()) ? 1 : 0;
			iResult=i2 - i1;
			break;
		}
		default: 
			iResult=0;
			break;
	}
	//VeryCD End
	int dwNextSort;
	//call secondary sortorder, if this one results in equal
	//(Note: yes I know this call is evil OO wise, but better than changing a lot more code, while we have only one instance anyway - might be fixed later)
	if (iResult == 0 && (dwNextSort = theApp.emuledlg->sharedfileswnd->sharedfilesctrl.GetNextSortOrder(lParamSort)) != (-1)){
		iResult= SortProc(lParam1, lParam2, dwNextSort);
	}

	return iResult;

}

void CSharedFilesCtrl::OpenFile(const CShareableFile* file)
{
	if(file->IsKindOf(RUNTIME_CLASS(CKnownFile)) && ((CKnownFile*)file)->m_pCollection)
	{
		CCollectionViewDialog dialog;
		dialog.SetCollection(((CKnownFile*)file)->m_pCollection);
		dialog.DoModal();
	}
	else
		ShellOpenFile(file->GetFilePath(), NULL);

}

void CSharedFilesCtrl::OnNMDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int iSel = GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
	if (iSel != -1)
	{
		CShareableFile* file = (CShareableFile*)GetItemData(iSel);
		if (file)
		{
			if (GetKeyState(VK_MENU) & 0x8000)
			{
				CTypedPtrList<CPtrList, CShareableFile*> aFiles;
				aFiles.AddHead(file);
				ShowFileDialog(aFiles);
			}
			else if (!file->IsPartFile())
				OpenFile(file);
		}
	}
	*pResult = 0;
}

void CSharedFilesCtrl::CreateMenues()
{
	if (m_PrioMenu) VERIFY( m_PrioMenu.DestroyMenu() );
	if (m_CollectionsMenu) VERIFY( m_CollectionsMenu.DestroyMenu() );
	if (m_SharedFilesMenu) VERIFY( m_SharedFilesMenu.DestroyMenu() );


	m_PrioMenu.CreateMenu();
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOVERYLOW,GetResString(IDS_PRIOVERYLOW));
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOLOW,GetResString(IDS_PRIOLOW));
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIONORMAL,GetResString(IDS_PRIONORMAL));
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOHIGH, GetResString(IDS_PRIOHIGH));
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOVERYHIGH, GetResString(IDS_PRIORELEASE));
	m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOAUTO, GetResString(IDS_PRIOAUTO));//UAP

	m_CollectionsMenu.CreateMenu();
	m_CollectionsMenu.AddMenuTitle(NULL, true);
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_CREATECOLLECTION, GetResString(IDS_CREATECOLLECTION), _T("COLLECTION_ADD"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_MODIFYCOLLECTION, GetResString(IDS_MODIFYCOLLECTION), _T("COLLECTION_EDIT"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_VIEWCOLLECTION, GetResString(IDS_VIEWCOLLECTION), _T("COLLECTION_VIEW"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_SEARCHAUTHOR, GetResString(IDS_SEARCHAUTHORCOLLECTION), _T("COLLECTION_SEARCH"));

	m_SharedFilesMenu.CreatePopupMenu();
	m_SharedFilesMenu.AddMenuTitle(GetResString(IDS_SHAREDFILES), true);

	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_OPEN, GetResString(IDS_OPENFILE), _T("OPENFILE"));
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_OPENFOLDER, GetResString(IDS_OPENFOLDER), _T("OPENFOLDER"));
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_RENAME, GetResString(IDS_RENAME) + _T("..."), _T("FILERENAME"));
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_REMOVE, GetResString(IDS_DELETE), _T("DELETE"));
	if (thePrefs.IsExtControlsEnabled())
		m_SharedFilesMenu.AppendMenu(MF_STRING,Irc_SetSendLink,GetResString(IDS_IRC_ADDLINKTOIRC), _T("IRCCLIPBOARD"));

	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);
	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)m_PrioMenu.m_hMenu, GetResString(IDS_PRIORITY) + _T(" (") + GetResString(IDS_PW_CON_UPLBL) + _T(")"), _T("FILEPRIORITY"));
	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);

	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)m_CollectionsMenu.m_hMenu, GetResString(IDS_META_COLLECTION), _T("COLLECTION"));
	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR); 	

	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_DETAIL, GetResString(IDS_SHOWDETAILS), _T("FILEINFO"));
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_CMT, GetResString(IDS_CMT_ADD), _T("FILECOMMENTS"));
	if (thePrefs.GetShowCopyEd2kLinkCmd())
		m_SharedFilesMenu.AppendMenu(MF_STRING,MP_GETED2KLINK, GetResString(IDS_DL_LINK1), _T("ED2KLINK") );
	else
		m_SharedFilesMenu.AppendMenu(MF_STRING,MP_SHOWED2KLINK, GetResString(IDS_DL_SHOWED2KLINK), _T("ED2KLINK") );
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_FIND, GetResString(IDS_FIND), _T("Search"));
	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);
#ifndef _ENGLISH_VERSION
	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_VIRUS, GetResString(IDS_VIRUS), _T("ANTIVIRUS"));		//Added by thilon on 2006.08.28
#endif

#if defined(_DEBUG)
	if (thePrefs.IsExtControlsEnabled()){
		//JOHNTODO: Not for release as we need kad lowID users in the network to see how well this work work. Also, we do not support these links yet.
		m_SharedFilesMenu.AppendMenu(MF_STRING,MP_GETKADSOURCELINK, _T("Copy eD2K Links To Clipboard (Kad)"));
		m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);
	}
#endif
}

void CSharedFilesCtrl::ShowComments(CShareableFile* file)
{
	if (file)
	{
		CTypedPtrList<CPtrList, CShareableFile*> aFiles;
		aFiles.AddHead(file);
		ShowFileDialog(aFiles, IDD_COMMENT);
	}
}

void CSharedFilesCtrl::OnGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (theApp.emuledlg->IsRunning()){
		// Although we have an owner drawn listview control we store the text for the primary item in the listview, to be
		// capable of quick searching those items via the keyboard. Because our listview items may change their contents,
		// we do this via a text callback function. The listview control will send us the LVN_DISPINFO notification if
		// it needs to know the contents of the primary item.
		//
		// But, the listview control sends this notification all the time, even if we do not search for an item. At least
		// this notification is only sent for the visible items and not for all items in the list. Though, because this
		// function is invoked *very* often, no *NOT* put any time consuming code here in.

		if (pDispInfo->item.mask & LVIF_TEXT){
			const CKnownFile* pFile = reinterpret_cast<CKnownFile*>(pDispInfo->item.lParam);
			if (pFile != NULL){
				switch (pDispInfo->item.iSubItem){
					case 0:
						if (pDispInfo->item.cchTextMax > 0){
							_tcsncpy(pDispInfo->item.pszText, pFile->GetFileName(), pDispInfo->item.cchTextMax);
							pDispInfo->item.pszText[pDispInfo->item.cchTextMax-1] = _T('\0');
						}
						break;
					default:
						// shouldn't happen
						pDispInfo->item.pszText[0] = _T('\0');
						break;
				}
			}
		}
	}
	*pResult = 0;
}

void CSharedFilesCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'C' && (GetKeyState(VK_CONTROL) & 0x8000))
	{
		// Ctrl+C: Copy listview items to clipboard
		SendMessage(WM_COMMAND, MP_GETED2KLINK);
		return;
	}
	else if (nChar == VK_F5)
		ReloadFileList();

	CMuleListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSharedFilesCtrl::ShowFileDialog(CTypedPtrList<CPtrList, CShareableFile*>& aFiles, UINT uPshInvokePage)
{
	if (aFiles.GetSize() > 0)
	{
		CSharedFileDetailsSheet dialog(aFiles, uPshInvokePage, this);
		dialog.DoModal();
	}
}

void CSharedFilesCtrl::SetDirectoryFilter(CDirectoryItem* pNewFilter, bool bRefresh){
	if (m_pDirectoryFilter == pNewFilter)
		return;
	m_pDirectoryFilter = pNewFilter;
	if (bRefresh)
		ReloadFileList();
}

//VeryCD Start
//�����ļ������е����ۣ�added by Chocobo on 2006.09.01
//��굥�����ƶ��¼�����
void CSharedFilesCtrl::OnCommentClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO test;
	test.pt = pNMListView->ptAction;
	test.flags = 0;
	test.iItem = -1;
	test.iSubItem = -1;

	if(ListView_SubItemHitTest(m_hWnd, &test) == -1)
	{
		return;
	}

	CKnownFile* file = (CKnownFile*)GetItemData(test.iItem);
	if (test.iSubItem == 1)
	{
		//CKnownFile* file = (CKnownFile*)content->value;
		//CPartFile* file = (CPartFile*)content->value;

		CString strURL = _T("http://www.verycd.com/files/");
		//CString strHash = CString(file->GetFileHash());
		//CString strHash = md4str(file->GetPublishedED2K());

		strURL += CreateED2kLink(file, false);
		ShellOpenFile(strURL);
	}

    // VC-SearchDream[2006-11-01]:Fix the Bug of 0000005: �����ļ���������
	theApp.emuledlg->sharedfileswnd->ShowSelectedFilesSummary();

	*pResult = 0;
}

void CSharedFilesCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	LVHITTESTINFO test;
	test.pt = point;
	test.flags = 0;
	test.iItem = -1;
	test.iSubItem = -1;

	if(ListView_SubItemHitTest(m_hWnd, &test) == -1)
	{
		return;
	}

	/*CtrlItem_Struct* content = (CtrlItem_Struct*)GetItemData(test.iItem);*/
	if (test.iSubItem == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	}

	__super::OnMouseMove(nFlags, point);
}
//VeryCD End