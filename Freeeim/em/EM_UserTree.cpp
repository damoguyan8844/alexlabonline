// MyTreeCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "EM_UserTree.h"
#include "stdemdef.h"
#include "../EIM02Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

EM_UserTree::EM_UserTree()
{
}

EM_UserTree::~EM_UserTree()
{
}


BEGIN_MESSAGE_MAP(EM_UserTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers

void EM_UserTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Click event handler Expand/COLLAPSE items
	UINT iFlags;
	UINT TVIF;
	HTREEITEM hItem = HitTest(point, &iFlags);
	if((hItem != NULL) && ItemHasChildren(hItem))
	{
		SetRedraw(FALSE);
		if((iFlags & TVHT_ONITEMICON))
		{
			TVIF = GetItemState(hItem, TVIF_STATE) & TVIS_EXPANDED ? TVE_COLLAPSE : TVE_EXPAND;
			Expand(hItem, TVIF);
			if (TVIF == TVE_COLLAPSE)
				SetItemImage(hItem, 1, 1);
			else
				SetItemImage(hItem, 0, 0);
			UpdateData(FALSE);
		}
		SetRedraw();
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void EM_UserTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
		if (TVE_COLLAPSE == pNMTreeView->action)
		SetItemImage(pNMTreeView->itemNew.hItem, 1,1);
	*pResult = 0;
}


void EM_UserTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (TVE_EXPAND == pNMTreeView->action)
		SetItemImage(pNMTreeView->itemNew.hItem, 0, 0);
	*pResult = 0;
}