/*
 * $Id: UrlSrcFromSvrMgr.cpp 15538 2009-09-07 09:37:30Z huby $
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
// UrlSrcFromSvrMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "emule.h"
#include "UrlSrcFromSvrMgr.h"
#include "UrlSrcGetFromSvrSocket.h"
#include "MetaLink/MetaLinkQuerySocket.h"
#include "PartFile.h"


// CUrlSrcFromSvrMgr

CUrlSrcFromSvrMgr::CUrlSrcFromSvrMgr()
{
	m_pAssocPartFile	= NULL;
}

CUrlSrcFromSvrMgr::~CUrlSrcFromSvrMgr()
{
}


// CUrlSrcFromSvrMgr ��Ա����
void CUrlSrcFromSvrMgr::SetAssocPartFile(CPartFile *pAssocPartFile)
{
	m_pAssocPartFile = pAssocPartFile;
}

bool CUrlSrcFromSvrMgr::GetSrcFromServerAsyn(void)
{
	try
	{
		if (NULL == m_pAssocPartFile)
			return false;
		
		CMetaLinkQuerySocket * pQSocket = new CMetaLinkQuerySocket(this, true);
		if (NULL == pQSocket)
			return false;
/*
		if( !m_pAssocPartFile->HasNullHash() )
		{
			CUrlSrcGetFromSvrSocket	*pSocket = new CUrlSrcGetFromSvrSocket(this, true);		//��������� CClientReqSocket �����ӶϿ���� delete �Լ�������newһ���������������ӡ�
			if (NULL != pSocket)
				return pSocket->SendRequest() && pQSocket->SendRequest();
		}
*/		
		return pQSocket->SendRequest();
	}
	catch( ... )
	{
		return false;
	}
}

bool CUrlSrcFromSvrMgr::SendReq_FileDownloaded(void)
{
	if (NULL == m_pAssocPartFile)
		return false;

	CMetaLinkQuerySocket * pQSocket = new CMetaLinkQuerySocket(this, false);
	if (NULL == pQSocket)
		return false;

/*
	CUrlSrcGetFromSvrSocket	*pSocket = new CUrlSrcGetFromSvrSocket(this, false);		//��������� CClientReqSocket �����ӶϿ���� delete �Լ�������newһ���������������ӡ�
	if (NULL == pSocket)
		return pQSocket->SendRequest();
*/

	return pQSocket->SendRequest() /*&& pSocket->SendRequest()*/;
}

bool CUrlSrcFromSvrMgr::AddSrcToServer(LPCTSTR lpszUrl)
{
	if (!IsExistInFetchedList(lpszUrl))
	{
		//todo: add to server
		m_strlstFetchedUrlSources.AddTail(lpszUrl);
	}

	return true;
}

bool CUrlSrcFromSvrMgr::IsExistInFetchedList(LPCTSTR lpszUrl)
{
	POSITION	pos;
	CString		str;

	pos = m_strlstFetchedUrlSources.GetHeadPosition();
	while (NULL != pos)
	{
		str = m_strlstFetchedUrlSources.GetNext(pos);
		if (0 == str.CompareNoCase(lpszUrl))
			return true;
	}

	return false;
}
