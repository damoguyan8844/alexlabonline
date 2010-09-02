#include "StdAfx.h"
#include "EIM02Dlg.h"
#include "freeeim_message_logic.h"


FreeEIM_Message_Logic::FreeEIM_Message_Logic()
{
	m_pMainTop = (CEIM02Dlg*)AfxGetMainWnd();
	m_nType = FREEEIM_NONE;
	m_szData = NULL;
}

FreeEIM_Message_Logic::~FreeEIM_Message_Logic()
{
}

void FreeEIM_Message_Logic::PostTo(const char *szUser)
{
	m_pMainTop->m_MSGrecv.SendMsg((char*)szUser, NULL);
}
