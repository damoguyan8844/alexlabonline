/*
 * $Id: UrlSrcFromSvrMgr.h 4483 2008-01-02 09:19:06Z soarchin $
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


//#include "AsyncSocketEx.h"
// CUrlSrcFromSvrMgr ����Ŀ��
class CPartFile;

class CUrlSrcFromSvrMgr : public CObject
	{
	friend class CUrlSrcGetFromSvrSocket;		//���� CClientReqSocket �����ӶϿ���� delete �Լ���������һ����CUrlSrcFromSvrSocket���������ӡ�
	friend class CMetaLinkQuerySocket;
public:
	CUrlSrcFromSvrMgr();
	virtual ~CUrlSrcFromSvrMgr();

	void	SetAssocPartFile(CPartFile *pAssocPartFile);
	bool	GetSrcFromServerAsyn(void);							//�첽��������ȡurlԴ��
	bool	SendReq_FileDownloaded(void);						//���߷��������ļ�������ϡ�

	bool	AddSrcToServer(LPCTSTR lpszUrl);								//��urlԴ��ӵ��������ϱ���������

protected:
	bool	IsExistInFetchedList(LPCTSTR lpszUrl);


protected:
	CPartFile	*m_pAssocPartFile;								//��֮�������PartFile��ָ�롣

	CStringList			m_strlstFetchedUrlSources;				//�ڷ������ϵ�urlԴ�����ӷ�������ȡ��urlԴ�󣬴���ڴˡ���
};


