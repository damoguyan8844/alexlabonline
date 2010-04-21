/*
 * $Id: TabItem_MainButton.h 4483 2008-01-02 09:19:06Z soarchin $
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
#include "TabItem_Normal.h"
#include "CxImage\ximage.h"

class CTabItem_MainButton : public CTabItem_Normal
{
	DECLARE_DYNCREATE(CTabItem_MainButton)
public:
	CTabItem_MainButton(void);
	virtual ~CTabItem_MainButton(void);

public:
	enum{MARGIN_HEAD = 18, ICON_TXT_GAP = 14, MARGIN_TAIL = 24,
		TOTAL_GAP = MARGIN_HEAD + ICON_TXT_GAP + MARGIN_TAIL};

	//void	SetActivedIcon(HICON hIcon);
	void	SetActivedIcon(LPCTSTR lpszPngResource);
	virtual	int	GetDesireLength(void);
protected:
	bool	m_bHasActiveIco;
	CxImage	m_imgActiveIco;
	//HICON	m_hIconActived;

protected:
	virtual void Paint(CDC* pDC);

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseHover(void);
	virtual void OnMouseLeave(void);
};
