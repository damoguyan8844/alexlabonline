#include "StdAfx.h"
#include "EIM02Dlg.h"
#include "freeeim_user_logic.h"
#include "FreeEIM_Message_Logic.h"

FreeEIM_User_Logic::FreeEIM_User_Logic()
{
	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();
}

FreeEIM_User_Logic::~FreeEIM_User_Logic()
{
}

// ���͵�����Ϣ���û���
void FreeEIM_User_Logic::PostMessageToUser(const char *szUser, char *szMessage)
{
	FreeEIM_Message_Logic message;
	message.m_nType = FREEEIM_TEXT;
	message.m_szData = szMessage;

	message.PostTo(szUser);
}

// ���͵�����Ϣ�����������û���
void FreeEIM_User_Logic::PostMessageToGroup(HTREEITEM hItem, char *szMessage)
{
	// �����û��б�ؼ�
	CTreeCtrl *pTree = & m_pMainTop->m_treeCtrlList;

	// ����һ���ڵ�������ӽڵ�
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = pTree->GetChildItem(hItem);
	for(; hChildItem != NULL; hChildItem = hNextItem)
	{
		CString strText = pTree->GetItemText(hChildItem);

		// һ��һ���ط���
		PostMessageToUser((LPCTSTR)strText, szMessage);

		hNextItem = pTree->GetNextSiblingItem(hChildItem);
	}
}

void FreeEIM_User_Logic::SendStatus(int Status)
{

}


EM_UserInfo *FreeEIM_User_Logic::GetUserInfo()
{
	return m_pMainTop->m_config.GetUserInfo();
}