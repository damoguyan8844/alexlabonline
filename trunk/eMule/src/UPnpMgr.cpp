#include "StdAfx.h"
#include ".\upnpmgr.h"

#include <WinSock2.h>
#include "Preferences.h"
#include "IpHlpApi.h"
#include "otherfunctions.h"




CUPnpMgr::CUPnpMgr(void)
{
	m_slocalIP.Empty();
	m_uLocalIP = 0;
}

CUPnpMgr::~CUPnpMgr(void)
{
	CleanupMappingArr();
}

void CUPnpMgr::SetBindAddress(LPCTSTR lpszBindAddress)
{
	CSingleLock		localLock(&m_cs, TRUE);
	m_nat.SetBindAddress(lpszBindAddress);
}

void CUPnpMgr::SetActionTimeout(int iActionTimeoutMs)
{
	CSingleLock		localLock(&m_cs, TRUE);
	m_nat.SetActionTimeout(iActionTimeoutMs);
}

int CUPnpMgr::GetActionTimeout()
{
	CSingleLock		localLock(&m_cs, TRUE);
	return m_nat.GetActionTimeout();
}

HRESULT CUPnpMgr::AddNATPortMapping(CUPnpNatMapping &mapping, BOOL bTryRandom)
{
	CSingleLock		localLock(&m_cs, TRUE);
	
	if(!IsLANIP(GetLocalIP()))
		return E_UNAT_NOT_IN_LAN;

	if (FAILED(m_nat.SearchDevice()))
		return E_UNAT_CANNOT_FIND_ROUTER;

	
	HRESULT		hr;
	CString		strDesc;
	int			i;

	if(0 == mapping.m_wExternalPort)
		mapping.m_wExternalPort = mapping.m_wInternalPort;

	mapping.m_strInternalClient = GetLocalIPStr();

	for (i = 0; i < RANDOM_RETRY_TIMES; i++)
	{
		strDesc.Format(_T("%s [%s: %u]"), mapping.m_strDescription, mapping.m_strProtocol, mapping.m_wExternalPort);

		hr = m_nat.AddPortMapping(CString(_T("")),
									mapping.m_wExternalPort, mapping.m_strProtocol,
									mapping.m_wInternalPort, mapping.m_strInternalClient,
									strDesc);


		switch(hr)
		{
		case S_OK:
			{
				CString		strInternalClient;
				hr = m_nat.GetSpecificPortMappingEntry(CString(_T("")),
														mapping.m_wExternalPort, mapping.m_strProtocol,
														NULL, &strInternalClient, NULL, NULL, NULL);
				if (SUCCEEDED(hr)
					&& strInternalClient == mapping.m_strInternalClient)			//	��PortMapping�Ƿ���ļӽ�ȥ�ˡ�
				{
					CUPnpNatMappingKey key;
					key.m_strRemoteHost.Empty();
					key.m_usExternalPort = mapping.m_wExternalPort;
					key.m_strProtocol = mapping.m_strProtocol;
					m_setAddedMapping.insert(key);

					WriteAddedMappingToFile();

					return S_OK;
				}
				else											//	����������ɹ����ض��ֲ����б��У����п������б�������
					return E_UNAT_ENTRY_MAYBE_FULL;

				break;
			}
		case E_UNAT_ACTION_HTTP_ERRORCODE:
			{
				if (718 == m_nat.GetLastActionErrorCode() && bTryRandom)
					mapping.m_wExternalPort = (WORD)(2049 + (65535 - 2049) * 1 *rand() / (RAND_MAX + 1));
				else
					return E_UNAT_ACTION_HTTP_ERRORCODE;
				break;
			}
		case E_UNAT_TIMEOUT:
			{
				//	Ϊ��ֹ���ܳ��֡�ʵ�ʼ����ˡ����ȴ��ظ���ʱ�����������ִ���˲����Ķ����������Ա�֮�������һ�¡�
				
				CUPnpNatMappingKey key;
				key.m_strRemoteHost.Empty();
				key.m_usExternalPort = mapping.m_wExternalPort;
				key.m_strProtocol = mapping.m_strProtocol;
				m_setAddedMapping.insert(key);

				WriteAddedMappingToFile();

				return E_UNAT_TIMEOUT;		//	��ʱδ������ֱ�ӷ��أ�����ȥ���ԡ�����Ϊ�˿̿���ʵ������ӳɹ�������ͣȥ��ӣ��������·���������������
				break;
			}
		default:
			return hr;
		}
	}

	return E_UNAT_REACH_RAND_PORT_RETRY_TIMES;
}

void CUPnpMgr::RemoveNATPortMapping(const CUPnpNatMappingKey &mappingKey)
{
	CSingleLock		localLock(&m_cs, TRUE);
	
	if (FAILED(m_nat.SearchDevice()))
		return;

	HRESULT		hr;
	hr = m_nat.DeletePortMapping(mappingKey.m_strRemoteHost, mappingKey.m_usExternalPort, mappingKey.m_strProtocol);

	if (SUCCEEDED(hr)
		|| 714 == GetLastActionErrorCode())	// No such entry in array
	{
		m_setAddedMapping.erase(mappingKey);
		WriteAddedMappingToFile();
	}
}

void CUPnpMgr::CleanupAllEverMapping(void)
{
	CSingleLock		localLock(&m_cs, TRUE);

	//ADDED by VC-fengwen 2007/08/23 <begin> : û��ӳ����Ļ�����ִ����ȥ��
	if (0 == m_setAddedMapping.size())
		return;
	//ADDED by VC-fengwen 2007/08/23 <end> : û��ӳ����Ļ�����ִ����ȥ��

	if (FAILED(m_nat.SearchDevice()))
		return;

	HRESULT				hr;
	set<CUPnpNatMappingKey>::iterator	it;
	set<CUPnpNatMappingKey>::iterator	itNext;
	int					iOpCount;

	iOpCount = 0;
	it = m_setAddedMapping.begin();
	while (it != m_setAddedMapping.end())
	{
		itNext = it;
		itNext++;

		hr = m_nat.DeletePortMapping(it->m_strRemoteHost, it->m_usExternalPort, it->m_strProtocol);
		if (SUCCEEDED(hr)
			|| 714 == GetLastActionErrorCode()	// No such entry in array
			|| 402 == GetLastActionErrorCode()) // Some router will return 402 for some reason. Delete it as well anyway.
		{
			m_setAddedMapping.erase(it);
		}

		iOpCount++;
		if (iOpCount >= 10)		//	��ֹһЩ·��������һ�δ���̫������ɾ��������಻����10�Σ�����һ������eMuleֻ�Ὺ2���˿ڣ������ֵӦ���ǹ��ġ�
			break;

		it = itNext;
	}

	WriteAddedMappingToFile();
}


BOOL CUPnpMgr::CleanedFillupBug()
{
	CSingleLock		localLock(&m_cs, TRUE);
	
	if (FAILED(m_nat.SearchDevice()))
		return FALSE;


	HRESULT		hr;
	USHORT		usIndex;
	USHORT		usExternalPort;
	CString		strProtocol;
	CString		strDesc;
	CString		strInternalClient;
	CString		strLocalIP;
	
	CArray<CUPnpNatMapping, const CUPnpNatMapping&>		arrMapping;
	CUPnpNatMapping										mapping;

	strLocalIP = GetLocalIPStr();

	for (usIndex = 0; usIndex < 128; usIndex++)		//	����ѯ128��Է�ֹ��Щ·�������ܻ�һֱ���سɹ���
	{
		hr = m_nat.GetGenericPortMappingEntry(usIndex, NULL, &usExternalPort, &strProtocol,
												NULL, &strInternalClient, NULL, &strDesc);

		if (-1 != strDesc.Find(_T("eMule (")))			//	���������ָ������
		{
			if (strInternalClient == strLocalIP)	//	�Ǳ�����ӵ�ӳ��
				m_nat.DeletePortMapping(CString(_T("")),usExternalPort, strProtocol);
			else
			{
				mapping.m_strInternalClient = strInternalClient;
				mapping.m_wExternalPort = usExternalPort;
				mapping.m_strProtocol = strProtocol;	
				arrMapping.Add(mapping);
			}
		}

		if (FAILED(hr))		//	����Failed�������ʾ�Ѿ�ȫ��ȡ���ˡ�
			break;
	}

	
	//	���ͬһip����eMuleӳ�䳬��3�����п�������ǰ��bug���¡��������<begin>
	int			i;
	int			iMappingCount;
	CString		strTestIp;

	while (! arrMapping.IsEmpty())
	{
		strTestIp = arrMapping[0].m_strInternalClient;
		iMappingCount = 0;
		for (i = 0; i < arrMapping.GetCount(); i++)
		{
			if (strTestIp == arrMapping[i].m_strInternalClient)
				iMappingCount++;
		}

		for (i = arrMapping.GetCount() - 1; i >= 0; i--)
		{
			if (strTestIp == arrMapping[i].m_strInternalClient)
			{
				if (iMappingCount > 3)			
				{
					m_nat.DeletePortMapping(CString(_T("")),
											arrMapping[i].m_wExternalPort,
											arrMapping[i].m_strProtocol);
				}

				arrMapping.RemoveAt(i);
			}

		}
	}
	//	���ͬһip����eMuleӳ�䳬��3�����п�������ǰ��bug���¡��������<end>


	return TRUE;


}


/////////////////////////////////////////////////////////////////////////////////
// Returns a CString with the local IP in format xxx.xxx.xxx.xxx
/////////////////////////////////////////////////////////////////////////////////
CString CUPnpMgr::GetLocalIPStr()
{
	CSingleLock		localLock(&m_cs, TRUE);

	if(m_slocalIP.IsEmpty())
		InitLocalIP();
	
	return m_slocalIP;
}

/////////////////////////////////////////////////////////////////////////////////
// Returns the Local IP
/////////////////////////////////////////////////////////////////////////////////
DWORD CUPnpMgr::GetLocalIP()
{
	CSingleLock		localLock(&m_cs, TRUE);

	if(m_uLocalIP == 0)
		InitLocalIP();

	return m_uLocalIP;
}

CString CUPnpMgr::ResultCode2String(HRESULT hr)
{
	CString		str;

	if (FAILED(hr))
		str = _T("ʧ�ܣ�");
	else
		str.Empty();

	switch(hr)
	{
	case S_OK:
		str = str + _T("�ɹ�");
		break;
	case E_UNAT_BAD_ARGUMENTS:
		str = str + _T("��������");
		break;
	case E_UNAT_NOT_IN_LAN:
		str = str + _T("���ǹ���IP");
		break;
	case E_UNAT_CANNOT_FIND_ROUTER:
		str = str + _T("�Ҳ���·����");
		break;
	case E_UNAT_TIMEOUT:
		str = str + _T("��ʱ�޻�Ӧ");
		break;
	case E_UNAT_ENTRY_MAYBE_FULL:
		str = str + _T("�˿�ӳ��ı����������");
		break;
	case E_UNAT_REACH_RAND_PORT_RETRY_TIMES:
		str = str + _T("�ﵽ����˿ڵ����Դ���");
		break;
	case E_UNAT_CREATE_SOCKET_FAILED:
		str = str + _T("����Socketʧ��");
		break;
	case E_UNAT_SOCKET_BIND_FAILED:
		str = str + _T("Socket��ʧ��");
		break;
	case E_UNAT_ACTION_HTTP_ERRORCODE:
		str = str + _T("��������ʧ��");
		break;
	case E_UNAT_UNKNOWN_ERROR:
	default:
		str = str + _T("δ֪����");
		break;
	}

	return str;
}

CString	CUPnpMgr::Result2String(HRESULT hr, DWORD dwActionErrorCode)
{
	CString	strOut;
	strOut = ResultCode2String(hr);

	CString strErrorCode;
	if (E_UNAT_ACTION_HTTP_ERRORCODE == hr)
	{
		switch(dwActionErrorCode)
		{
		case 718:
			strErrorCode = _T("����������ʹ�ø��ⲿ�˿�ӳ�䡣");
			break;
		default:
			strErrorCode.Empty();
			break;
		}

		CString	str;
		str.Format(_T("(#%d : %s)"), dwActionErrorCode, strErrorCode);
		strOut += str;
	}

	return strOut;
}


/////////////////////////////////////////////////////////////////////////////////
// Initializes m_localIP variable, for future access to GetLocalIP()
/////////////////////////////////////////////////////////////////////////////////
void CUPnpMgr::InitLocalIP()
{
	CSingleLock		localLock(&m_cs, TRUE);

#ifndef _DEBUG
	try
#endif
	{
		PIP_ADAPTER_INFO pAdapterInfo = NULL, pCurAdapterInfo;
		ULONG ulOutBufLen = 0;
		DWORD dwRetVal = 0;

		pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
		ulOutBufLen = sizeof(IP_ADAPTER_INFO);

		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);

		if (ERROR_BUFFER_OVERFLOW == dwRetVal)
		{
			free(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
			dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
		}

		if (ERROR_SUCCESS == dwRetVal)
		{
			pCurAdapterInfo = pAdapterInfo;
			UINT32 ip;
			do {
				ip = inet_addr(pCurAdapterInfo->GatewayList.IpAddress.String);
				m_slocalIP = pCurAdapterInfo->IpAddressList.IpAddress.String;
				m_uLocalIP = inet_addr(pCurAdapterInfo->IpAddressList.IpAddress.String);
			} while(ip == 0 && (pCurAdapterInfo = pCurAdapterInfo->Next) != NULL);
		}

		free(pAdapterInfo);
		pAdapterInfo = NULL;
	}
#ifndef _DEBUG
	catch(...){
		m_slocalIP = _T("");
		m_uLocalIP = 0;
	}
#endif
}

void CUPnpMgr::CleanupMappingArr()
{
	CSingleLock		localLock(&m_cs, TRUE);

	m_setAddedMapping.clear();
}

void CUPnpMgr::WriteAddedMappingToFile(void)
{
	CSingleLock		localLock(&m_cs, TRUE);

	CString		strDatFile;
	strDatFile.Format(_T("%sUPnp.dat"), thePrefs.GetMuleDirectory(EMULE_CONFIGDIR));

	CFile		file;
	if (!file.Open(strDatFile, CFile::modeCreate | CFile::modeReadWrite))
		return;

	try
	{
		CArchive	ar(&file, CArchive::store);

		int						iCount;

		iCount = m_setAddedMapping.size();
		ar << iCount;

		set<CUPnpNatMappingKey>::iterator	it;
		for (it = m_setAddedMapping.begin(); it != m_setAddedMapping.end(); it++)
		{
			it->Serialize(ar);
		}
		ar.Close();
	}
	catch (CArchiveException *e)
	{
		e->Delete();
		e = NULL;
	}

	file.Close();
}

void CUPnpMgr::ReadAddedMappingFromFile(void)
{
	CSingleLock		localLock(&m_cs, TRUE);

	CleanupMappingArr();
	
	CString		strDatFile;
	strDatFile.Format(_T("%sUPnp.dat"), thePrefs.GetMuleDirectory(EMULE_CONFIGDIR));

	CFile		file;
	try
	{
		if (!file.Open(strDatFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
			return;

		if (0 == file.GetLength())
		{
			file.Close();
			return;
		}
	}
	catch (CFileException *e)
	{
		e->Delete();
		e = NULL;
		
		file.Close();
		return;
	}


	try
	{
		CArchive	ar(&file, CArchive::load);

		int		iCount;
		ar >> iCount;

		int						i;
		CUPnpNatMappingKey		key;
		for (i = 0; i < iCount; i++)
		{
			key.Serialize(ar);
			m_setAddedMapping.insert(key);
		}
		ar.Close();
	}
	catch (CArchiveException *e)
	{
		e->Delete();
		e = NULL;
	}
	
	file.Close();

}
