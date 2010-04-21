#pragma once

#include "updownclient.h"
#include "SourceURL.h"
#include "UrlSite.h"

/**
* class CINetClient 
* CINetClient������ CUpDownClient ������������ C/S �ܹ���һ���ͻ���,
*
*/
class CINetClient : public CUpDownClient
{
	DECLARE_DYNAMIC(CINetClient)
public:
	CINetClient(IPSite * pIpSite);
	~CINetClient(void);

	// �õ������� IpSite
	bool setIpSite( IPSite * pIpSite )
	{
		this->m_pIpSite = pIpSite;
		return true;
	}

	IPSite * getIpSite()
	{
		return this->m_pIpSite;
	}

	// ָ���Ƿ���ED2K�Ŀͻ���
	virtual bool IsEd2kClient() const 
	{ 
		return false; 
	}

	bool IsOriginalUrlSite( );

	// INet ������Զ����Ҫ���� HELLO ��
	virtual bool SendHelloPacket()
	{
		return true;
	}

	//
	// �õ�Զ���ļ��Ĵ�С
	//
	// ����ֵ:
	// -1: ����ļ�����Ϊδ֪
	//
	virtual uint64 GetFileSize( )
	{ 
		return m_uiFileSize;
	}

	// Ϊ�� Client �����µ�����,
	// ͨ�� iMaxBlocks ��ָ��������� ���񳤶�
	virtual int CreateBlockRequests_Before(int iMaxBlocks);
	virtual void CreateBlockRequests_After(int iMaxBlocks);
	virtual void ConnectionEstablished(); //< handshake finished.. just override the CUpDownClient::ConnectionEstablished
	virtual uint32  GetTimeUntilReask() const; ///< http/ftp Peer���û������,����10s����ѯ��һ��.
	
	bool TryToGetBlockRequests(int iMaxBlocks);

	//�����Ƿ��ǻ�վ��
	void SetSiteBadOrGood();

	virtual	DWORD	GetIPFrom( ) const;

public:
	// 
	CSourceURL m_SourceURL;

protected:  
	//
	// ָ���վ��ָ��� IPSite �ṹ
	//
	IPSite * m_pIpSite;

	//
	// �ļ��ĳ���,
	// -1: �ļ���С��δ֪
	//
	uint64 m_uiFileSize;
  
	//
	//�Ƿ��������Դ
	//
	bool m_bAddOtherSources;

	/// [VC-Huby-080513]: ���һ�μ�������������Դ��ʱ��
	DWORD m_dwLastAddOtherSources;

	virtual INetBlockRange_Struct* GetBlockRange();
	virtual bool RequestBlock( Requested_Block_Struct** newblocks, uint16* pCount, bool bUseParent );
	/// �Ƿ���������Ӹ�������
	bool m_bAllowedAddMoreConn;

public:
	void UpdateTransData(const CString& strURL);

	// �ú����ж��Ƿ���Ҫ���ⷢ��ͬһ��IP�Ĳ�ͬrefer������
	bool IsNeedAvoidInitalizeConnection();
};
