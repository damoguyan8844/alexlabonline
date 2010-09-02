// EMRichEdit.cpp : implementation file
// Author : Alex
// Date : 2007-5-9

#include "stdafx.h"
#include "../EIM02Dlg.h"
#include "stdemdef.h"
#include "EMRichEdit.h"
#include "../EM_MsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEMRichEdit

EM_MsgRichEdit::EM_MsgRichEdit()
{
}

EM_MsgRichEdit::~EM_MsgRichEdit()
{
}


BEGIN_MESSAGE_MAP(EM_MsgRichEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CEMRichEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMRichEdit message handlers


BOOL EM_MsgRichEdit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_NOTIFY == message)
	{
		ENLINK *pLink;
		pLink = (ENLINK*)lParam;
		if (pLink->nmhdr.code == EN_LINK)
		{
			if (pLink->msg == WM_LBUTTONUP)
			{
				SetSel(pLink->chrg);
				CString strLink = GetSelText();
			/*	if (strLink == _T("(Yes)是"))
				{
					// 你同意接受文件
					((EM_MsgDlg*)GetParent())->File_ResAcceptFile(pLink);
				}
				else if (strLink == _T("(No)否"))
				{
					// 拒绝接受文件
					((EM_MsgDlg*)GetParent())->File_ResDenyFile(pLink);
				}
				else
				{
					AfxMessageBox("超链接有问题。");
				}*/
				if (strLink.Find("Yes") != -1)
				{
					// 你同意接受文件
					((EM_MsgDlg*)GetParent())->File_ResAcceptFile(pLink);
				}
				else if (strLink.Find("No") != -1)
				{
					// 拒绝接受文件
					((EM_MsgDlg*)GetParent())->File_ResDenyFile(pLink);
				}
				else
				{
					AfxMessageBox("超链接有问题。");
				}
				SetSel(pLink->chrg.cpMax-1,pLink->chrg.cpMax-1);
			}
		}
	}
	return CRichEditCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}


void EM_MsgRichEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	::GetCursorPos(&point); //where is the mouse?
 	CMenu menu; //lets display out context menu :) 
 	UINT dwSelectionMade;                                       
	VERIFY(menu.LoadMenu(IDR_MENU2) );  
  	CMenu *pmenuPopup = menu.GetSubMenu(0);
  	ASSERT(pmenuPopup != NULL);
	UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED);
	pmenuPopup->EnableMenuItem(IDM_REDIT_COPY, MF_BYCOMMAND|nSel);

  	dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
  							point.x, point.y, this
        						);                                

 	pmenuPopup->DestroyMenu();
	switch(dwSelectionMade)
	{
	case IDM_REDIT_COPY:
		Copy();
		break;
	case IDM_REDIT_SELECTALL:
		SetSel(0, -1);
		break;
	}
}