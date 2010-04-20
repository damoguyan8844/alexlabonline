// UpdateInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "emule.h"
#include "UpdateInfo.h"
#include "Preferences.h"
#include "emuleDlg.h"
#include "SharedFileList.h"
#include "DownloadQueue.h"
#include "SharedFilesWnd.h"


// CUpdateInfo

CUpdateInfo::CUpdateInfo()
{

}

CUpdateInfo::~CUpdateInfo()
{
}


// CUpdateInfo ��Ա����

void CUpdateInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<m_Hash;
	}
	else
	{
		ar>>m_Hash;
	}
}

BOOL CUpdateInfo::OpenUpdateInfo(void)
{
	CFile file;

	if (file.Open(GetUpdateInfoFile(), CFile::modeRead))
	{
		try
		{
			CArchive ar(&file, CArchive::load);
			Serialize(ar);
		}
		catch (CException* e)
		{
			e->Delete();
			ASSERT(0);
		}

		file.Close();
		return TRUE;
	}
	
	return FALSE;
}

void CUpdateInfo::SaveUpdateInfo(void)
{
	CFile file;
	
	if (file.Open(GetUpdateInfoFile(), CFile::modeCreate | CFile::modeReadWrite ))
	{
		try
		{
			CArchive ar(&file, CArchive::store);
			Serialize(ar);
		}
		catch (CException* e)
		{
			e->Delete();
			ASSERT(0);
		}
	}	

	file.Close();
}

void CUpdateInfo::SetUpdateHash(CString hash)
{
	m_Hash = hash;
	SaveUpdateInfo();
}

CString CUpdateInfo::GetUpdateHash(void)
{
	if(OpenUpdateInfo())
	{
		return m_Hash;
	}

	return NULL;
}

CString CUpdateInfo::GetUpdateInfoFile(void)
{
	return thePrefs.GetMuleDirectory(EMULE_UPDATEDIR) + _T("update.dat");
}


BOOL CUpdateInfo::isUpdateFile(CString hash)
{
	CString oldhash = GetUpdateHash();

	if(oldhash.IsEmpty())
	{
		return FALSE;
	}

	if(oldhash.Compare(hash) == 0)
	{
		return TRUE;
	}

	return FALSE;
}


int CUpdateInfo::GetUpdateState(const uchar* hash)
{
	//�����ļ��б�
	const CKnownFile* file = theApp.sharedfiles->GetFileByID(hash);

	if(file)
	{
		if (file->IsPartFile())
		{
			return UPDATE_DOWNLOADING;	//�ڹ����б��У�����û��������ɡ�
		}
		else
		{
			return UPDATE_DOWNLOADED;	//�ڹ����б����У��Ѿ������ꡣ
		}
	}
	else
	{	
		//�����б�
		if ((file = theApp.downloadqueue->GetFileByID(hash)) != NULL)
		{
			return UPDATE_DOWNLOADING;	//�������б��У��϶�û���������
		}
		else
		{
			return UPDATE_NODOWNLOAD;	//��δ����
		}
	}
}

BOOL CUpdateInfo::UpdateInfoFileExists(void)
{
	return PathFileExists(thePrefs.GetMuleDirectory(EMULE_UPDATEDIR) + _T("update.dat"));
}

int CUpdateInfo::Compare(CString hash)
{
	CString oldhash = GetUpdateHash();
	return oldhash.Compare(hash);
}

BOOL CUpdateInfo::DeleteUpdate(CString hash)
{
	uchar UpdateHash[16];
	CPartFile* pPartFile;

	if(!strmd4(hash,UpdateHash))
	{
		return FALSE;
	}

	CKnownFile* file = theApp.sharedfiles->GetFileByID(UpdateHash);

	try
	{
		//�����б����Ƿ���
		if(file)
		{
			if(file->IsPartFile())
			{
				//�����б��У���δ�����꣬�Ƴ�
				pPartFile = DYNAMIC_DOWNCAST(CPartFile,file);
				if( pPartFile )
				{
					pPartFile->DeleteFile();
				}
			}
			else
			{
				//�����б��У��Ѿ��������δ��װ���Ƴ�
				DeleteFile(file->GetFilePath());
				theApp.emuledlg->sharedfileswnd->sharedfilesctrl.RemoveFile(file,true);
				theApp.sharedfiles->RemoveFile(file);
			}
		}
		
		if ((pPartFile = theApp.downloadqueue->GetFileByID(UpdateHash)) != NULL)
		{
			//�����б�û�У���δ�����꣬�Ƴ�
			pPartFile->DeleteFile();
		}		
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}

	return FALSE;
}