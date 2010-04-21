/*
 * $Id: ToolTipCtrlZ.cpp 4483 2008-01-02 09:19:06Z soarchin $
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
// ToolTipCtrlZ.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ToolTipCtrlZ.h"
#include ".\tooltipctrlz.h"


// CToolTipCtrlZ

IMPLEMENT_DYNAMIC(CToolTipCtrlZ, CToolTipCtrl)
CToolTipCtrlZ::CToolTipCtrlZ()
{
}

CToolTipCtrlZ::~CToolTipCtrlZ()
{
}


BEGIN_MESSAGE_MAP(CToolTipCtrlZ, CToolTipCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CToolTipCtrlZ ��Ϣ�������


int CToolTipCtrlZ::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	GetParent()->EnableToolTips();
	Activate(TRUE);
	SetMaxTipWidth(SHRT_MAX);
	SetMargin(CRect(4, 4, 4, 4));


	return 0;
}
