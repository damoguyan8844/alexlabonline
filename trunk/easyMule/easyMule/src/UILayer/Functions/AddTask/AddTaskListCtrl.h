/*
 * $Id: AddTaskListCtrl.h 4483 2008-01-02 09:19:06Z soarchin $
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
#pragma once

#include "AddTaskDoc.h"

// CAddTaskListCtrl

class CAddTaskListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CAddTaskListCtrl)

public:
	CAddTaskListCtrl();
	virtual ~CAddTaskListCtrl();

	void	SetDoc(CAddTaskDoc	*pDoc){m_pDoc = pDoc;}

protected:
	CAddTaskDoc	*m_pDoc;

	//CList<CFileHashKey*>	m_itemDatas;
protected:
	int		GetInsertIndex(LPCTSTR lpszFileName);
	void	CalStrByLinkText(LPCTSTR lpszLinkText, CString &strFileName, CString &strFileSize);
	void	CalStrByUrl(const CString &strUrl, CString &strFileName);

	void	SetItemByDocItem(int iItemIndex, const CAddTaskDoc::SItem &item, DWORD dwItemMask = CAddTaskDoc::IM_ALL);
	int		GetItemIndexByKey(const CFileHashKey &key);
	int		GetItemIndexByUrl(LPCTSTR lpszUrl);
	void	CheckAllItems(BOOL bCheck);
	void	CheckAllSelectedItems(BOOL bCheck);

protected:
	enum ItemType{IT_ED2K, IT_URL};
	struct SItemData
	{
		int	iType;
		
		CFileHashKey	fhk;
		CString			strUrl;
	};

	CList<SItemData*>	m_itemDatas;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnDocAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocModified(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocRemoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlModified(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlRemoved(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


