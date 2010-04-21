/*
 * $Id: WordFilter.cpp 14469 2009-07-20 10:24:59Z huby $
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
#include "stdafx.h"
#include "WordFilter.h"
#include "Preferences.h"
#include "Log.h"

CWordFilter WordFilter;

// KMP Next values generation function
__inline void	KMP_GetNext(TCHAR * sString, int * iNext)
{
	iNext[0] = -1;
	int i = -1;
	for(int j = 1; sString[j] != 0; j++)
	{
		while(sString[j] != sString[i + 1] && (i >= 0))
			i = iNext[i];
		if(sString[j] == sString[i + 1])
			iNext[j] = i + 1;
		else
			iNext[j] = -1;
		i = iNext[j];
	}
}

// KMP Match function
__inline bool	KMP_Match(const CString & sString, TCHAR * sSubString, int * iNext)
{
	int i = 0, j = 0;

	while (i < sString.GetLength() && sSubString[j] != 0)
	{
		if (sString[i] == sSubString[j])
		{
			++j; ++i;
		}
		else
		{
			if(j == 0)
				++i;
			j = (j > 0) ? (iNext[j - 1] + 1) : 0;
		}
	}

	return (sSubString[j] == 0);
}

//����wordfilter�ļ�������Ҫ����,����ɾ�����ܺ���
//deleted by kernel1983 2006.07.31
/*
__inline void	DecryptData(TCHAR * sString, int dwLength, DWORD * dwKey)
{
	(* dwKey) = (((* dwKey) << 5) + (DWORD)dwLength) ^ 5381;
	for(int i = 0; i < dwLength; i ++)
	{
		sString[i] = sString[i] ^ (TCHAR)(* dwKey);
		(* dwKey) = (((* dwKey) << 5) ^ (DWORD)sString[i]) ^ 5381;
	}
}

__inline void	DecryptInt(int * nNumber, DWORD * dwKey)
{
	(* dwKey) = ((* dwKey) << 5) ^ 5381;
	(* nNumber) = (* nNumber ^ 0xe7d5) ^ (*dwKey);
}
*/

void	CWordFilter::Init()
{
	HANDLE	hFile;
	DWORD	dwRead;
	int		nLen;
	BOOL	bResult;
	CStringList list;

	//m_count = 0;

	// ����ļ�Ŀ¼���ԣ������ƶ�һ�£���configĿ¼�� added by kernel1983 2006.07.31
	if (PathFileExists(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + FLITER_FILE)) 
		MoveFile(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + FLITER_FILE, thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE);

	if (!PathFileExists(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE))
	{
		// �����ڣ����еĶ����� added by kernel1983 2006.08.08
		m_filterall = true;
		return;
	}

	// Open file for read
	hFile = CreateFile(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//AddLogLine(false,_T(":%s\n"),thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + FLITER_FILE);
	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		// ��ȡ�������еĶ����� added by kernel1983 2006.08.08
		m_filterall = true;
		return;
	}

	DWORD dwSize = GetFileSize(hFile, NULL);

	TCHAR * pszData = new TCHAR[(dwSize / sizeof(TCHAR)) + 1];			// ����ռ�
	bResult = ReadFile(hFile, pszData, dwSize, &dwRead, NULL);		// �����ļ�1
	CloseHandle(hFile);
	pszData[(dwSize / sizeof(TCHAR))] = 0;

	if(bResult)
	{
		TCHAR * pszTemp = wcstok(pszData + 1, _T("\r\n"));
		while(pszTemp != NULL)
		{
			nLen = wcslen(pszTemp);
			while(pszTemp[nLen - 1] == '\t' || pszTemp[nLen - 1] == ' ')
			{
				nLen --;
				pszTemp[nLen] = 0;
			}
			while(*pszTemp == '\t' || *pszTemp == ' ')
			{
				pszTemp ++;
				nLen --;
			}
			//AddLogLine(false,_T("pszTemp:%s"),pszTemp);
			//AddLogLine(false,_T("nLen:%d"),nLen);
			if(nLen > 0)list.AddTail(pszTemp);
			//if(nLen == 8)AddLogLine(false,_T(":%d %d %d %d "),((char*)pszTemp)[0],((char*)pszTemp)[1],((char*)pszTemp)[2],((char*)pszTemp)[3]);
			pszTemp = wcstok(NULL, _T("\r\n"));
		}
	}

	delete[] pszData;

	m_count = list.GetCount();
	//AddLogLine(false,_T("m_count:%d"),m_count);

	if(bResult && m_count > 0)
	{
		m_filterwords = new TCHAR*[m_count+1];
		m_kmpvalue = new int*[m_count+1];
		ZeroMemory(m_filterwords, sizeof(TCHAR *) * m_count);
		ZeroMemory(m_kmpvalue, sizeof(int *) * m_count);
	}

	for(int i = 0; bResult && (i < m_count); i ++)
	{
		CString s = list.GetAt(list.FindIndex(i));
		s.MakeLower();
		nLen = s.GetLength();
		//AddLogLine(false,_T("nLen:%d"),nLen);
		m_filterwords[i] = new TCHAR[nLen + 1];
		m_filterwords[i][nLen] = 0;	// ���һ���ֽ���Ϊ0
		m_kmpvalue[i] = new int[nLen];
		//AddLogLine(false,_T("nLen:%d"),nLen);
		_tcscpy(m_filterwords[i],s);
		//AddLogLine(false,_T("m_filterwords[i]:%s"),m_filterwords[i]);
		KMP_GetNext(m_filterwords[i], m_kmpvalue[i]);	// �õ�һ���������йص���ֵm_kmpvalue[i]
	}

	if(m_count == 0 || !bResult)
	{
		Free();
		//m_filterall = true;
	}
}


bool	CWordFilter::VerifyString(const CString & sString)	// ��֤�ַ��Ƿ�Ϸ�
{
	#ifdef _FOREIGN_VERSION
		return true;
	#endif

	bool bIsRm = sString.Right(3)==_T(".rm");
	
	CString sString2 = sString;
	TCHAR szExt[_MAX_EXT];
	_tsplitpath(sString2, NULL, NULL, NULL, szExt);
	_tcslwr(szExt);

	if( _tcscmp(szExt, _T(".avi")) == 0
		|| _tcscmp(szExt, _T(".rmvb")) == 0
		|| _tcscmp(szExt, _T(".wmv")) == 0 )
	{
		sString2 = sString2.Left(sString2.GetLength()-_tcslen(szExt));
	}
	
	CString sReduceString=sString2;
	CString sInterpunctionString = _T("������().���������������������������������������");
	try // VC-Huby[2007-03-20]:�����й�������ɫ,��ǿ����
	{		
		int j=0;
		for( int i=0; i<sString2.GetLength(); i++ )
		{
			if( sString2.GetAt(i)<=_T('/') && sString2.GetAt(i)>=_T(' ') ) //�ӿո�'/'֮����ַ��������ٹ���
			{
				continue;
			}
			else if( sString2.GetAt(i)<=_T('@') && sString2.GetAt(i)>=_T(':') )
			{
				continue;
			}
			else if( sString2.GetAt(i)<=_T('`') && sString2.GetAt(i)>=_T('[') )
			{
				continue;
			}
			else if( sString2.GetAt(i)<=_T('~') && sString2.GetAt(i)>=_T('{') )
			{
				continue;
			}
			else if( sInterpunctionString.Find(sString2.GetAt(i))>=0 )
			{
				continue;
			}
			else
			{
				sReduceString.SetAt(j,sString2.GetAt(i));
				j++;
			}
		}
		if( j<sString2.GetLength() )
			sReduceString.SetAt(j,_T('\0'));
	}
	catch (...) 
	{
	}

	if(m_filterall){
		//AddLogLine(false,_T("m_filterall"));
		return true;	// ��ⲻ���ļ������߶�ȡ���������·�������
	}
	if(m_count == 0){
		//AddLogLine(false,_T("m_count == 0"));
		return true;	// �ļ��ǿյ�ʱ�򣬷������˹���
	}
	CString strSearch = ((CString)sReduceString).MakeLower();

	//vc-huby: ���������ַ�����15�ַ�
	//CString sReduceString2=strSearch;		
	int k=0;
	for( int i=0; i<strSearch.GetLength(); i++ )
	{
		if( strSearch.GetAt(i)<=_T('9') && strSearch.GetAt(i)>=_T('0') )
		{
			continue;
		}
		if( strSearch.GetAt(i)<=_T('z') && strSearch.GetAt(i)>=_T('a') )
		{
			continue;
		}
		else
		{
			k++;
		}
	}

	if( k>=20 && bIsRm )
		return false;
	//int m = sReduceString2.GetLength();
/*
	if( k>=60 )
		return false;*/


	/*if (strSearch.GetLength() > 20)
	{
		return false;
	}*/

	for(int i = 0; i < m_count; i ++)
	{
		if(KMP_Match(strSearch, m_filterwords[i], m_kmpvalue[i]))
		{
			//AddLogLine(false,_T("KMP_Match"));
			return false;	// �ؼ��������ˣ���fliter��
		}
	}
	//AddLogLine(false,_T("©����"));
	return true;

}

void CWordFilter::Free()	//
{
	for(int i = 0; i < m_count; i ++)
	{
		if(m_filterwords[i])
			delete[] m_filterwords[i];
		if(m_kmpvalue[i])
			delete[] m_kmpvalue[i];
	}
	delete[] m_filterwords;
	delete[] m_kmpvalue;
}

CWordFilter::~CWordFilter()
{
	Free();
}
