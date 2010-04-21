/*
 * $Id: FileMgr.h 11398 2009-03-17 11:00:27Z huby $
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

#include "SearchFile.h"
#include "KnownFile.h"
#include "UrlSite.h"
#include "SafeFile.h"
#include "FileMgrDB.h"

//ע�⣺�벻Ҫ��������Щ�궨���ֵ����Ϊ�����Ѿ������񱾵ش洢����,�޸�ֵ��������ش���! VC-Huby
#define FILESTATE_NOT_EXIST				0   //filemgr �в����ڸ��ļ����� 
#define FILESTATE_DOWNLOADING			1
#define FILESTATE_COMPLETED				2	//�������	
#define FILESTATE_HASH					3
#define FILESTATE_DOWNLOADED_SHARE		4	//��������ɲ�����  FILESTATE_SHARE_TASK_DOWNLOADED
#define FILESTATE_LOCAL_SHARE			5	//���ع���(�����ļ�ͨ������ҳ����'Add Share files'�����ȥ��)
#define FILESTATE_DELETED				6   //ɾ��(������ɾ����Ӧ�������ļ�..)
#define FILESTATE_ZEROSIZE_DOWNLOADED	7	//(http/ftp)0�ֽڴ�С���������
#define FILESTATE_SHARE_TASK_DELED      8   //�����ص�����ɾ�����ļ�����,����Ҫ���ַ���״̬.
#define FILESTATE_DOWNLOADED_UNSHARE    9   //��������ɵ�ȥ���˷���


class FILEKEY
{
public:
	FILEKEY()
	{
	}
	FILEKEY(const uchar * id)
	{
		memcpy(key, id, 16);
	}
	uchar key[16];
	bool operator<(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) < 0;
	}
	bool operator==(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) == 0;
	}
	bool operator>(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) > 0;
	}
};


class CFileTaskItem
{
public:
	CFileTaskItem()
	{
		m_nFileState = FILESTATE_NOT_EXIST;
		m_FileSize = 0;
		m_metBakId = 0;
	}
	void Serialize(CArchive & ar, int iVerion)
	{		
		if(ar.IsLoading())
		{
			if (iVerion >=1)
			{
				int nStringNum = 0;
				ar>>m_nFileState;
				ar>>nStringNum;
				ASSERT(nStringNum>=1);
				ar>>m_strFilePath;
				ar>>m_strUrl;
				ar>>m_FileSize;
				ar>>m_FileName;
			}
			if (iVerion >= 2)
				ar>>m_strEd2kLink;
			if(iVerion >= 3)
				ar>>m_tFiletime;
			if(iVerion >= 4)
				ar>>m_metBakId;
 			if(iVerion >= 5)
			{
 				int count = 0;
				ar >> count;
   				for( int i = 0; i < count; i++ ) 
   				{
                    CUrlSite urlSite;  
  					ar >> urlSite; 
   					m_lMetaLinkURLList.AddTail( urlSite );
 				}

 			}
		}
		else
		{
			if (iVerion >=1)
			{
				ar<<m_nFileState;
				ar<<int(1);
				ar<<m_strFilePath;
				ar<<m_strUrl;
				ar<<m_FileSize;
				ar<<m_FileName;
			}
			if (iVerion >= 2)
				ar<<m_strEd2kLink;
			if(iVerion >= 3)
				ar<<m_tFiletime;
			if(iVerion >= 4)
				ar<<m_metBakId;
			ar<<m_lMetaLinkURLList.GetCount();
 			if(iVerion >= 5)
 			{
   				POSITION pos = m_lMetaLinkURLList.GetHeadPosition();
   				while(pos)
   				{
                   ar<<m_lMetaLinkURLList.GetNext(pos);
  				}
 			}
		}
	}
	
	int m_nFileState;
	CString m_strFilePath; //������·���ļ��� fullPathName
	CString m_strUrl;
	uint64 m_FileSize;
	CString m_FileName;
	CString m_strEd2kLink;
	CTime m_tFiletime;
	UINT32 m_metBakId;
	CList <CUrlSite>  m_lMetaLinkURLList;
};

class CFileMgr
{
	friend class CFileMgrDB;
public:
	CFileMgr(void);
	~CFileMgr(void);

	enum{CURRENT_FILELIST_VERSION = 6};

	void LoadFiles(CString strFileListPath);
	void ReloadLocalSharedFiles();
	bool NewDownloadFile(CString strlink, CString strFilepath, int cat,bool bNewTask = true,CFileTaskItem* pFileTaskItem=NULL);
	void RemoveFileItem(CAbstractFile * pFile,bool bKeepShared=false);
	void RemoveEdLinkFileTask(CString strEd2kUrl);
	bool AddLocalSharedFile(const CString & strFile, bool bNeedHash, bool bLoadFromFileListDat=false,CFileTaskItem *pFileItem=NULL,const uchar* pFileHash=NULL);
	bool RemoveFileShare(const CString & strFilePath);
	//void NewDownloadBySearchFile(CSearchFile* toadd, uint8 paused, CString path, int cat);
	bool AddPartFile(CPartFile * partfile, int filestate);

	CString GetUrl(const uchar * hashkey);
	CFileTaskItem* GetFileTaskItem(const uchar * hashkey);
	CFileTaskItem* GetFileTaskItem(const CString& strUrl);
	int GetFileState(const uchar * hashkey);
	int GetFileState(const CString & strLink);
	bool HasFileSharedInDir(const CString& strDir );
	//bool IsFileDownloading(CString strFilename);
	bool IsFileLocalShared(CKnownFile * file);

	void DownloadCompleted(CPartFile * partfile);
	//void DownloadCompleted(CString strUrl);
	void HashCompleted(CKnownFile * file);
	void HashFailed (CString strFile);
	bool IsWaitforHash( CString& strFile );
	
	CFileTaskItem* AddURLTask(CString strUrl,const CString strLocalDir);
	// VC-yunchenn.chen[2007-07-12]: -1���޸ľ�ֵ�������ڷ���false�� strLocalPathΪ�ղ��޸ľ�ֵ
	bool ModifyURLState(CString strUrl, CString strLocalPath, int nFileState=-1);
	void RemoveURLTask(CString strUrl);

	int	GetUrlTaskState(CString strUrl);
	BOOL AddDownLoadRequest(const CString & strUrl, const CString & strFilePath, CPartFile* &pPartFile,bool bNewTask);
	void UpdateFileItem(const CKnownFile *pFile);

	void OnPartfileUrlChanged( const CString & old_url , const CString & new_url , CPartFile * partfile );
   void OnAddMetaLinkUrl(CONST BYTE * lpHash, CString strUrl,CUrlSite* pUrlSite);

protected:
	//  for loading
	bool AsyncShareList();
	bool AfterLoadFileItem(CFileTaskItem * pFileItem,const uchar* pFileHash=NULL);
	bool LoadDownloadFile(CFileTaskItem * pNewItem);
	bool LoadSharedFile(CFileTaskItem * pNewItem,const uchar* pFileHash=NULL);
	bool LoadDownloadUrlFile( CString& strUrl, CFileTaskItem * pFileTaskItem );
public:
	CString GetDatabaseFile();
protected:
	CMutex m_Mutex;
	CFileMgrDB * m_fmdb;
	CRBMap<FILEKEY,CFileTaskItem*> m_FileList;
	CRBMap<CString, int> m_WaitforHashList;
public:
	CRBMap<CString, CFileTaskItem*> m_UrlList;

    void SaveFile();
    UINT SaveFileItem(CSafeBufferedFile &file,CFileTaskItem *pItem,UINT uTagCount);
	void OnTimer(UINT nIDEvent);
	bool LoadFinished(){return m_bLoadFinished;}
	bool m_bLoadFinished;
	CString GetFileName(const CString & strLink);
	CString GetUrlFileName(CString strUrl);
};

extern CString UrlConvert( CString strUrl );

