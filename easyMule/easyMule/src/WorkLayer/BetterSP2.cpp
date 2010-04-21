/*
 * $Id: BetterSP2.cpp 4483 2008-01-02 09:19:06Z soarchin $
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
#include "StdAfx.h"
#include "BetterSP2.h"
//#include "eMule.h"
#include "Preferences.h"

#include <Imagehlp.h> //added by thilon on 2006.08.07
#include "GlobalVariable.h"
#include <Windows.h>

typedef PIMAGE_NT_HEADERS (WINAPI* PCheckSumMappedFile) (
	PVOID BaseAddress,
	DWORD FileLength,
	PDWORD HeaderSum,
	PDWORD CheckSum
);

typedef PIMAGE_NT_HEADERS (WINAPI* PImageNtHeader) (
	IN PVOID Base
);

HINSTANCE hinstImageHelp = NULL;

PCheckSumMappedFile MyCheckSumMappedFile;
PImageNtHeader MyImageNtHeader;


CBetterSP2::CBetterSP2(void)
{
	SetFileName();
	m_dwPosition = 0;
}

void CBetterSP2::SetFileName(void)
{
	memset( m_szFileName, 0, sizeof(m_szFileName));
	//��ȡtcpip.sys·��
	TCHAR	szFile[MAX_PATH + 128];
	memset(szFile, 0, sizeof(szFile));
	::GetSystemDirectory(szFile, MAX_PATH);

	if(szFile[0]&&szFile[wcslen(szFile)-1]!='\\')
	{
		lstrcat(szFile, _T("\\drivers\\tcpip.sys"));
	}
	else
	{
		lstrcat(szFile, _T("drivers\\tcpip.sys"));
	}

	_tcsncpy( m_szFileName, szFile, MAX_PATH );
}

CBetterSP2::~CBetterSP2(void)
{
	if( NULL != hinstImageHelp )
	{
		FreeLibrary( hinstImageHelp );
		hinstImageHelp = NULL;
	}
}

/****************************************************************************
                          
������:
     GetFileVersion(LPCTSTR pFile)

��������:
      	���tcpip.sys�ļ��汾��Ϣ

�����������õĺ����嵥:
      		
���ñ������ĺ����嵥:
		int DetectSystemInformation(void)
      
����:
	LPCTSTR pFile[in]����һ���ļ�·��
����ֵ:
	  CString
	  �����ļ��İ汾��Ϣ
����:
��ʷ:
    Created by thilon on 2006.08.07[�½�]
****************************************************************************/
int CBetterSP2::GetFileVersion(LPCTSTR pFile)
{
	memset( m_dwVersion, 0, sizeof(m_dwVersion) );

	if(pFile==NULL)
	{
		return	0;
	}

	if(GetFileAttributes(pFile) == (DWORD)(~0))
	{
		return 0;
	}

	DWORD dwTemp0 = 0, dwTemp1 = 0;
	DWORD dwSize = ::GetFileVersionInfoSize( pFile, &dwTemp0 );

	if(dwSize == 0)
	{
		return 0;
	}
	
	BYTE* pVersion = new BYTE[dwSize + 10];
	memset( pVersion, 0, dwSize + 10 );

	if(GetFileVersionInfo( pFile, 0, dwSize + 2, pVersion ) == FALSE)
	{
		return 0;
	}

	VS_FIXEDFILEINFO* pFixed = NULL;
	if( VerQueryValue( pVersion, _T( "\\" ), (LPVOID*)&pFixed, (PUINT)&dwTemp1 ) && NULL != pFixed && dwTemp1 >= sizeof(VS_FIXEDFILEINFO) ==  FALSE)
	{
		return 0;
	}

	m_dwVersion[0] = ( pFixed->dwFileVersionMS >> 16 ) & 0xFFFF;
	m_dwVersion[1] = ( pFixed->dwFileVersionMS & 0xFFFF );
	m_dwVersion[2] = ( pFixed->dwFileVersionLS >> 16 ) & 0xFFFF;
	m_dwVersion[3] = ( pFixed->dwFileVersionLS & 0xFFFF );

	delete[] pVersion;
	return 1;
}

DWORD CBetterSP2::GetCurrentLimit_Vista(LPCTSTR pFile)
{
	m_dwPosition = 0;

	HANDLE hFile = CreateFile( pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if(hFile == NULL||hFile == INVALID_HANDLE_VALUE)
	{
		return 0;//-2;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	DWORD dwFileRead = 0;

	if( dwFileSize < 32 * 1024 || dwFileSize > 2048 * 784 )
	{
		CloseHandle( hFile );			//������ļ���С
		return 0;//-3;
	}

	BYTE* biFileData = new BYTE[dwFileSize + 1024];
	memset( biFileData, 0, dwFileSize + 2 );

	if( SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) != 0 || ReadFile( hFile, (LPVOID)biFileData, dwFileSize, &dwFileRead, NULL ) == FALSE || dwFileRead != dwFileSize )
	{
		CloseHandle( hFile );
		delete[] biFileData;
		return 0;//-4;
	}

	CloseHandle( hFile );

	/*���濪ʼ����λ�õ�
	�㷨��
	1������5f 5e 5b c9 c2 08 00 8b 15 68 e0 0c 00 83 fa 0a��λ�õ������Ȼ��ʼ����顣
	2������Ƿ�0f 87 8b 00 00
	*/

	DWORD dwPosition = 0;
	BYTE pbiMagic[] = {0x5f, 0x5e, 0x5b, 0xc9, 0xc2, 0x08, 0x00, 0x8b, 0x15, 0x68, 0xe0, 0x0c, 0x00, 0x83, 0xfa, 0x0a };
	dwFileSize -= 1024; //����϶������������1024���ֽ��Ϊ������Ƚϵķ���
	for( DWORD i = 0; i < dwFileSize; i++ )
	{
		if( memcmp( &biFileData[i], pbiMagic, sizeof(pbiMagic) ) == 0 )
		{ //got it
			dwPosition = i;
			break;
		}
	}

	if( dwPosition == 0 || dwPosition >= dwFileSize )
	{
		delete[] biFileData;
		return 0;//-5;
	}

	dwPosition += sizeof(pbiMagic);

	BYTE pbOrig[]={0x0f, 0x87, 0x8b, 0x00, 0x00, 0x00};
	BYTE pbPack[]={0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	if(memcmp(biFileData+dwPosition, pbOrig, sizeof(pbOrig))
		&& memcmp(biFileData+dwPosition, pbPack, sizeof(pbPack)))
	{
		delete[] biFileData;
		return 0;//-6;
	}

	m_dwPosition = dwPosition;	//�õ��޸�λ��

	DWORD dwLimit = 0;
	//memcpy( &dwLimit, biFileData + dwPosition, sizeof(DWORD) );
	if(memcmp(biFileData+dwPosition, pbPack, sizeof(pbPack))==0)
		dwLimit=70000;

	delete[] biFileData;
	return dwLimit;

}

/****************************************************************************

������:
	GetCurrentLimit(LPCTSTR pFile)

��������:
	��õ�ǰ������������

�����������õĺ����嵥:

���ñ������ĺ����嵥:

����:
	LPCTSTR pFile[in]����һ���ļ�·��
����ֵ:
	DWORD ��������������
����:
��ʷ:
Created by thilon on 2006.2006.11.02[�½�]
****************************************************************************/
DWORD CBetterSP2::GetCurrentLimit(LPCTSTR pFile)
{
	if(thePrefs.GetWindowsVersion() == _WINVER_VISTA_)
	{
		return GetCurrentLimit_Vista(pFile);
	}

	m_dwPosition = 0;

	HANDLE hFile = CreateFile( pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if(hFile == NULL||hFile == INVALID_HANDLE_VALUE)
	{
		return 0;//-2;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	DWORD dwFileRead = 0;

	if( dwFileSize < 32 * 1024 || dwFileSize > 2048 * 1024 )
	{
		CloseHandle( hFile );			//������ļ���С
		return 0;//-3;
	}

	BYTE* biFileData = new BYTE[dwFileSize + 1024];
	memset( biFileData, 0, dwFileSize + 2 );

	if( SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) != 0 || ReadFile( hFile, (LPVOID)biFileData, dwFileSize, &dwFileRead, NULL ) == FALSE || dwFileRead != dwFileSize )
	{
		CloseHandle( hFile );
		delete[] biFileData;
		return 0;//-4;
	}

	CloseHandle( hFile );

	/*���濪ʼ����λ�õ�
	�㷨��
	1������Unicode��\Device\RasAcd���ӵ�һ��\��ʼ����ǰƫ��0x80���ֽڣ�λ�õ������Ȼ��ʼ����顣
	2�����\Device\RasAcd���������Ƿ�CC CC CC CC CC CC 90 90 90 90��
	3���ж�λ�õ�-1��λ�ã��Ƿ�Ϊ0x00��
	4���ж�λ�õ�+4���Ƿ�Ϊ0xB8��
	5���޸�λ�õ㡣	*/

	DWORD dwPosition = 0;
	BYTE pbiMagic[] = { '\\', 0x00, 'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00,
		'\\', 0x00, 'R', 0x00, 'a', 0x00, 's', 0x00, 'A', 0x00, 'c', 0x00, 'd', 0x00,
		0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x90, 0x90, 0x90, 0x90 };
	dwFileSize -= 1024; //����϶������������1024���ֽ��Ϊ������Ƚϵķ���
	for( DWORD i = 0; i < dwFileSize; i++ )
	{
		if( biFileData[i] != '\\' || biFileData[i + 1] != '\0' )
			continue;

		if( memcmp( &biFileData[i], pbiMagic, sizeof(pbiMagic) ) == 0 )
		{ //got it
			dwPosition = i;
			break;
		}
	}

	if( dwPosition == 0 || dwPosition >= dwFileSize )
	{
		delete[] biFileData;
		return 0;//-5;
	}

	dwPosition += 0x80;
	if( biFileData[dwPosition - 1] != '\0' || biFileData[dwPosition + 4] != 0xB8 )
	{
		delete[] biFileData;
		return 0;//-6;
	}

	m_dwPosition = dwPosition;	//�õ��޸�λ��

	DWORD dwLimit = 0;
	memcpy( &dwLimit, biFileData + dwPosition, sizeof(DWORD) );

	delete[] biFileData;
	return dwLimit;
}

int CBetterSP2::IsLimitedVersion(void)
{
	if( m_dwVersion[0] < 5 )
		return FALSE;

	if( m_dwVersion[0] == 5 )
	{
		switch( m_dwVersion[1] )
		{
		case 0: //2000
			return 0;
		case 1: //XP
			return ( m_dwVersion[2] >= 2600 && m_dwVersion[3] >= 2180 ) ? 1 : 0;
		case 2: //2003
			return ( m_dwVersion[2] >= 3790 && m_dwVersion[3] > 0 ) ? 1 : 0;
		default:
			break;
		}

		//unknow version
		return 1;
	}
	return 1; //>5, new System
}
/****************************************************************************
                          
������:
       DetectSystemInformation(void)

��������:
      	������ϵͳ��tcpip.sys��Ϣ

�����������õĺ����嵥:
      		
���ñ������ĺ����嵥:
		void CPPgConnection::LoadSettings(void)
      
����:

����ֵ:
	  int
	  �ɹ�����1��ʧ�ܷ���0
����:
��ʷ:
    Created by thilon on 2006.08.07[�½�]
****************************************************************************/
int CBetterSP2::DetectSystemInformation(void)
{
	//������ϵͳ�汾
	if(!(thePrefs.GetWindowsVersion() == _WINVER_XP_ || thePrefs.GetWindowsVersion() == _WINVER_VISTA_))
	{
		return 0;
	}
	
	//����ļ��Ƿ����
    if(!m_szFileName)
	{
		return 0;
	}

	//��ȡtcpip.sys�汾��
	if(!GetFileVersion(m_szFileName))
	{
		return 0;
	}

	//���tcpip.sys�Ƿ���ȷ
	if(!IsLimitedVersion())
	{
		return 0;
	}

	if(!GetCurrentLimit(m_szFileName))
	{
		return 0;
	}
  
   	//��⵱ǰ��tcp/ip����
	//  Comment UI
	//theApp.SetTCPIPValue(GetCurrentLimit(m_szFileName));
	CGlobalVariable::SetTCPIPValue(GetCurrentLimit(m_szFileName));
	return 1;
 
}

/****************************************************************************
                          
������:
       ChangeTCPIPValue(DWORD dwValue)

��������:
      	�޸�tcpip.sys������

�����������õĺ����嵥:
      		
���ñ������ĺ����嵥:
		void CPPgConnection::OnApply()
      
����:
	DWORD dwValue[in]Ҫ�޸ĵ�������

����ֵ:
	 
����:
��ʷ:
    Created by thilon on 2006.08.07[�½�]
****************************************************************************/
int CBetterSP2::ChangeTCPIPValue(DWORD dwValue)
{
	if(thePrefs.GetWindowsVersion() == _WINVER_VISTA_)
	{
		return ChangeTCPIPValue_Vista(dwValue);
	}

	if( dwValue <= 0 || dwValue > 0x7FFF )
	{
		return 0;
	}

	DWORD nCurLimit = GetCurrentLimit(m_szFileName);

	if( nCurLimit < 0 || m_dwPosition == 0)
	{
		return 0;
	}

	if( nCurLimit == dwValue )
	{
		return 1;
	}

	if( NULL == hinstImageHelp )
	{
		hinstImageHelp = LoadLibrary( _T( "imagehlp.dll" ) );
		if( NULL != hinstImageHelp )
		{ //GetProcAddressֻ��ANSI�汾
			MyCheckSumMappedFile = (PCheckSumMappedFile)GetProcAddress( hinstImageHelp, ( "CheckSumMappedFile" ) );
			MyImageNtHeader = (PImageNtHeader)GetProcAddress( hinstImageHelp, ( "ImageNtHeader" ) );
		}
	}

	if( NULL == hinstImageHelp || NULL == MyCheckSumMappedFile || NULL == MyImageNtHeader )
	{
		return 0;
	}

	//1, ��һ���ļ����ڶ�д
	HANDLE hFile = ::CreateFile(m_szFileName,//thilon
        						GENERIC_READ|GENERIC_WRITE,
                                FILE_SHARE_READ,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

	if(hFile==NULL||hFile==INVALID_HANDLE_VALUE) 
	{
		return 0;
	}

	DWORD  dwSize = ::GetFileSize(hFile, NULL);

	if(dwSize == (DWORD)~0||dwSize <= (DWORD)(m_dwPosition + sizeof(DWORD)))
	{
		::CloseHandle(hFile);
		return 0;
	}

	//2, ӳ���ļ�
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	::CloseHandle(hFile);

	if(hMap==NULL||hMap==INVALID_HANDLE_VALUE) 
	{
		return 0;
	}

	PVOID pFile = ::MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);

	if(pFile==NULL)
	{
		::CloseHandle(hMap);
		return 0;
	}

	//3, �ı�tcp/ip����
	DWORD dwCurLimit = (DWORD)nCurLimit;
	if( memcmp( (BYTE*)pFile + m_dwPosition, &dwCurLimit, sizeof(DWORD) ) != 0 )
	{
		UnmapViewOfFile( pFile );
		CloseHandle( hMap );
		return 0;
	}

	DWORD dwLimit = (DWORD)dwValue;

	__try
	{
		memmove( (BYTE*)pFile + m_dwPosition, &dwLimit, sizeof(DWORD) );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		UnmapViewOfFile( pFile );
		CloseHandle( hMap );
		return 0;
	}

	BYTE biPE[] = {'P', 'E', '\0', '\0'};
	int bSuccess = 0;
	DWORD Original = 0, New = 0;
	MyCheckSumMappedFile( pFile, dwSize, &Original, &New );
	PIMAGE_NT_HEADERS pHeader = MyImageNtHeader( pFile );

	if( NULL != pHeader && pHeader->OptionalHeader.CheckSum == Original && memcmp( &pHeader->Signature, biPE, sizeof(biPE) / sizeof(BYTE) ) == 0 )
	{
		__try
		{
			pHeader->OptionalHeader.CheckSum = New;
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			UnmapViewOfFile( pFile );
			CloseHandle( hMap ); 
			return 0;
		}
		bSuccess = 1;	
	}

	UnmapViewOfFile( pFile );
	CloseHandle( hMap );

	return 1;
	
}

int CBetterSP2::ChangeTCPIPValue_Vista(DWORD dwValue)
{
	if( dwValue == 0)
	{
		return 0;
	}

	dwValue = 70000;
	DWORD nCurLimit = GetCurrentLimit_Vista(m_szFileName);

	if(m_dwPosition == 0)
	{
		return 0;
	}

	if( nCurLimit == dwValue )
	{
		return 1;
	}

	if( NULL == hinstImageHelp )
	{
		hinstImageHelp = LoadLibrary( _T( "imagehlp.dll" ) );
		if( NULL != hinstImageHelp )
		{ //GetProcAddressֻ��ANSI�汾
			MyCheckSumMappedFile = (PCheckSumMappedFile)GetProcAddress( hinstImageHelp, ( "CheckSumMappedFile" ) );
			MyImageNtHeader = (PImageNtHeader)GetProcAddress( hinstImageHelp, ( "ImageNtHeader" ) );
		}
	}

	if( NULL == hinstImageHelp || NULL == MyCheckSumMappedFile || NULL == MyImageNtHeader )
	{
		return 0;
	}

	//1, ��һ���ļ����ڶ�д
	HANDLE hFile = ::CreateFile(m_szFileName,//thilon
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(hFile==NULL||hFile==INVALID_HANDLE_VALUE) 
	{
		return 0;
	}

	DWORD  dwSize = ::GetFileSize(hFile, NULL);

	if(dwSize == (DWORD)~0||dwSize <= (DWORD)(m_dwPosition + sizeof(DWORD)))
	{
		::CloseHandle(hFile);
		return 0;
	}

	//2, ӳ���ļ�
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	::CloseHandle(hFile);

	if(hMap==NULL||hMap==INVALID_HANDLE_VALUE) 
	{
		return 0;
	}

	PVOID pFile = ::MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);

	if(pFile==NULL)
	{
		::CloseHandle(hMap);
		return 0;
	}

	//3, �ı�tcp/ip����
	BYTE pbPack[]={0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	//DWORD dwCurLimit = (DWORD)nCurLimit;
	//if( memcmp( (BYTE*)pFile + m_dwPosition, &dwCurLimit, sizeof(DWORD) ) != 0 )
	//{
	//	UnmapViewOfFile( pFile );
	//	CloseHandle( hMap );
	//	return 0;
	//}

	//DWORD dwLimit = (DWORD)dwValue;

	__try
	{
		memmove( (BYTE*)pFile + m_dwPosition, pbPack, sizeof(pbPack) );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		UnmapViewOfFile( pFile );
		CloseHandle( hMap );
		return 0;
	}

	BYTE biPE[] = {'P', 'E', '\0', '\0'};
	int bSuccess = 0;
	DWORD Original = 0, New = 0;
	MyCheckSumMappedFile( pFile, dwSize, &Original, &New );
	PIMAGE_NT_HEADERS pHeader = MyImageNtHeader( pFile );

	if( NULL != pHeader && pHeader->OptionalHeader.CheckSum == Original && memcmp( &pHeader->Signature, biPE, sizeof(biPE) / sizeof(BYTE) ) == 0 )
	{
		__try
		{
			pHeader->OptionalHeader.CheckSum = New;
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			UnmapViewOfFile( pFile );
			CloseHandle( hMap ); 
			return 0;
		}
		bSuccess = 1;	
	}

	UnmapViewOfFile( pFile );
	CloseHandle( hMap );

	return 1;

}
