/*
 * $Id: PeerLog.h 5051 2008-03-20 02:43:47Z huby $
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
#include "ResizableLib\ResizableDialog.h"
#include "LogListCtrl.h"
#include "resource.h"

#include "HttpClient.h"
// CPeerLog �Ի���

class CPeerLog : public CResizableDialog
{
	DECLARE_DYNAMIC(CPeerLog)

public:
	CPeerLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPeerLog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PEERLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CLogListCtrl m_LogListCtrl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCopySelected();
	afx_msg void OnCutSelected();
	afx_msg void OnSelectAll();
	afx_msg void OnSaveAs();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

protected:
	CMenuXP*		m_pMenuXP;
	//CUpDownClient*  m_pClient;
	//CPartFile*		m_pPartFile;
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};
