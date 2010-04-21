/*
 * $Id: UrlSite.h 5702 2008-05-30 09:00:18Z huby $
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

#include <vector>

enum ESiteFrom
{
	// δ֪��
	sfUnknwon = 100,

	// ��metalink�ļ��л�ȡ����
	sfMetalinkFile ,
	
	// �ӷ�������
	sfVerycdServer ,

	sfMetaServer,

	// �ֶ������
	sfManualAdded ,

	// ��ʼ���ص�
	sfStartDown
};

class CUrlSite;
struct IPSite
{
	DWORD m_dwRetryCount;
	DWORD m_dwLastError;
	DWORD m_dwSameErrorCount;
	DWORD m_dwConnectionCount;
	DWORD m_dwMaxAllowConnCount;
	DWORD m_dwIpAddress;
	BOOL m_bBadSite;
	CUrlSite* m_pUrlSite;

	IPSite() : m_dwRetryCount(0) , m_dwLastError(0) 
		, m_dwSameErrorCount(0) , m_dwConnectionCount(0),m_dwMaxAllowConnCount(2)
		, m_dwIpAddress(0),m_bBadSite(false)
	{
	}
};

/**
* CUrlSite ��������һ����Դ��
*/
class CUrlSite
{
public:
	CUrlSite(void);
	~CUrlSite(void);

	// url ��ַ
	CString m_strUrl;

	// ָ����Դ����������
	DWORD m_dwFromWhere;
	
	// metaServer���Ƿ��Ѿ�����
	BOOL  m_bExistInMetaServer;

	// ��վ�����Ѿ�������������ĸ���
	// ����ڵ㲻��Ҫ����
	DWORD m_dwConnectionCount;

	// ���վ�������ԵĴ���
	// ����ڵ㲻��Ҫ����
	DWORD m_dwRetryCount;

	// ���վ�����Ѿ�������ɵ����ݵĴ�С,������Э�����ְ���С
	__int64 m_dwDataTransferedWithoutPayload;
	__int64 m_dwOldDataTransferedWithoutPayload;

	// ���վ�����Ѿ�������ɵ����ݵĴ�С,����Э�����ְ���С
	__int64 m_dwDataTransferedWithPayload;
	__int64 m_dwOldDataTransferedWithPayload;

	// ���վ���Ƿ��ǻ���վ��,���ļ��Ĵ�С��ʵ�ʵ��ļ���С��һ��,���Ѿ����ĵ�,
	// ���������޷����ӵ�վ��
	BOOL m_bBadSite;

	// վ����뵽������ʱ�ĳ�ʼ�÷�,����ڵ���Ҫ����
	DWORD m_dwInitPreference;

	//����ڵ���Ҫ����
	BOOL m_bNeedCommitted;

	CUrlSite* m_pRedirectFrom;

	std::vector<IPSite*> m_IPSiteList;
public:
	bool IsBadUrlSite();
	DWORD GetUrlSiteConnectNum();
	bool IsMyIP( DWORD dwIP,DWORD dwFrom);
};

class InvalidFormatException : public CException
{
};

 template<typename T>
	 T & operator << ( T & output_stream , const CUrlSite & site )
 {
	 output_stream << site.m_strUrl;
	 output_stream << site.m_dwFromWhere;
	 output_stream << site.m_dwConnectionCount;
	 output_stream << site.m_dwRetryCount;
	 output_stream << site.m_dwDataTransferedWithoutPayload;
	 output_stream << site.m_dwDataTransferedWithPayload;

	 return output_stream;
 };

 // �����ȡʧ��,�ú����׳� InvalidFormatException �쳣����
 template<typename T>
	 T & operator >> (T & input_stream , CUrlSite & site )
 {
	 input_stream >> site.m_strUrl;
	 input_stream >> site.m_dwFromWhere;
	 input_stream >> site.m_dwConnectionCount;
	 input_stream >> site.m_dwRetryCount;
	 input_stream >> site.m_dwDataTransferedWithoutPayload;
	 input_stream >> site.m_dwDataTransferedWithPayload;

	 return input_stream;
 };

