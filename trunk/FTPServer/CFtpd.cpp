#include "stdafx.h"
#include "CFtpd.h"
//#include "Iphlpapi.h"

#define CRLF "\r\n"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SOCKADDR_IN lpsockftpd; 

UINT thAcceptCmd( LPVOID pParam )
{
	CFtpd* pFtpd = (CFtpd*)pParam;
	CFtpdPi* ftpdbindsock = pFtpd->ftpdbindsock;
	try
	{
		pFtpd->absCrit.Lock();
		char tmp[32] = "";
		wsprintf(tmp,"%d.%d.%d.%d",lpsockftpd.sin_addr.S_un.S_un_b.s_b1,lpsockftpd.sin_addr.S_un.S_un_b.s_b2,lpsockftpd.sin_addr.S_un.S_un_b.s_b3,lpsockftpd.sin_addr.S_un.S_un_b.s_b4);
		ftpdbindsock->b_auto_choose_ip = pFtpd->b_auto_choose_ip;
		ftpdbindsock->b_switch_pasv_ip = pFtpd->b_switch_pasv_ip;
		ftpdbindsock->b_ipchk_pasv = pFtpd->b_ipchk_pasv;
		ftpdbindsock->m_specify_data_port = pFtpd->m_specify_data_port;
		ftpdbindsock->m_port_from = pFtpd->m_port_from;
		ftpdbindsock->m_port_from_preset = pFtpd->m_port_from;
		ftpdbindsock->m_port_to = pFtpd->m_port_to;
		ftpdbindsock->bUSER=FALSE;
		ftpdbindsock->bPASS=FALSE;	
		ftpdbindsock->bconnected = TRUE;
		ftpdbindsock->decided = FALSE;
		strcpy(ftpdbindsock->userfile,pFtpd->userfile);
		strcpy(ftpdbindsock->ipfile,pFtpd->ipfile);
		strcpy(ftpdbindsock->vRealRoot,pFtpd->m_root_dir);
		strcpy(ftpdbindsock->client_ip,tmp);
		ftpdbindsock->pFtpd = (LPVOID)pFtpd;
		pFtpd->AddClient(ftpdbindsock);
		pFtpd->absCrit.Unlock();
		bool allow = FALSE;
		if(!FileExists(pFtpd->ipfile) || pFtpd->b_ipchk == FALSE)
			allow = TRUE;
		else
		{
			POSITION pos;
			CStringList CSLIpList;
			FileToCsl(pFtpd->ipfile,&CSLIpList);
			CString CSIP = tmp;
			CString Citem;
			for( pos = CSLIpList.GetHeadPosition(); pos != NULL; )   
			{
				Citem = CSLIpList.GetNext(pos);
				if(CSIP.Find((LPCSTR)Citem)!=-1)	
					allow = TRUE;
				if(allow == TRUE)
					break;
			}
		}
		if(allow==TRUE)
		{
			int i = 0;
			unsigned long IP=-1;
			for (i=0; IP!=0;i++) 
			{
				IP=pFtpd->GetLocalNumericIP(i);
				if (IP !=0) 
				{
					wsprintf(tmp,"%d.%d.%d.%d",(IP&0xFF),((IP>>8 ) & 0xFF),((IP>>16) & 0xFF),((IP>>24) & 0xFF));
					ftpdbindsock->ipList.AddTail(tmp);
				}
			}
			wsprintf(pFtpd->buf,"220 %s FtpD for %s%s",(LPCSTR)pFtpd->m_servername,"free",CRLF);
			ftpdbindsock->Send(pFtpd->buf,strlen(pFtpd->buf),0);
		}else
		{
			wsprintf(pFtpd->buf,"no one is allowed to login from your ip address\n\r");
			ftpdbindsock->Send(pFtpd->buf,sizeof(pFtpd->buf),0);	
			wsprintf(pFtpd->buf,"unsuccessful login attemp from %s (ip not allowed)",tmp);
			ftpdbindsock->Close();
		}
	}
	catch(...)
	{
		pFtpd->absCrit.Unlock();
	}
	return 1;
}

CFtpd::CFtpd()
{
	AfxSocketInit(NULL);
	b_speed_limit_in = false;
	b_speed_limit_out = false;
	b_ipchk = false;
	b_ipchk_pasv = false;
	b_switch_pasv_ip = false;
	b_auto_choose_ip = true;
	//	m_kbs = 150;
	m_specify_data_port = false;
	m_port_from = 32000;
	m_port_to = 65536;
	m_servername = "Ftp Station(tm)";
	sl = sizeof(SOCKADDR_IN);
	GetCurrentDirectory(256,m_root_dir);
}

CFtpd::~CFtpd()
{
	CFtpdPi* Fitem;
	for(pos = ClientList.GetHeadPosition(); pos != NULL; )    
	{
		Fitem = ClientList.GetNext( pos );
		delete Fitem;				
	}	
	Close();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFtpd, CSocket)
//{{AFX_MSG_MAP(CFtpd)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

void CFtpd::OnAccept(int nErrorCode) 
{
	try
	{
		pos = NULL;
		allow = FALSE;
		ftpdbindsock = new CFtpdPi();
		Accept(*ftpdbindsock,(SOCKADDR*)&lpsockftpd,&sl);
		AfxBeginThread(&thAcceptCmd,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	catch (...)
	{		
	}	
	CSocket::OnAccept(nErrorCode);
}

void CFtpd::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);
}

DWORD CFtpd::GetLocalNumericIP(int Index)
{
	if (0==gethostname (HostName,100))
	{
		if (!(lpHostEnt = gethostbyname(HostName))) 
		{
			return(-1);
		} else 
		{
			lpDW=((DWORD*)(*lpHostEnt).h_addr_list);
			lpDW+=Index;
			lpDW2=(DWORD *) *lpDW;
			if (lpDW2==0) 
				return 0;
			else 
				return (*lpDW2);
		}
	} else 
		return (-1);
}

void CFtpd::AddClient(CFtpdPi *pBind)
{
	try
	{
		ClientList.AddTail(pBind);
		return;
	}
	catch(...)
	{
		return;
	}
}

void CFtpd::RemoveClient(CFtpdPi *pBind)
{
	try
	{
		POSITION pos = NULL;
		if( ( pos = ClientList.Find(pBind, NULL ) )!= NULL )    
			ClientList.RemoveAt(pos);
		if(pBind != NULL)
			delete pBind;
		return;
	}
	catch(...)
	{
		return;
	}
}

bool FileExists(char *fname)
{
	WIN32_FIND_DATA MyFoundFile;
	HANDLE Hfound=FindFirstFile(fname,&MyFoundFile);
	if(Hfound == INVALID_HANDLE_VALUE)
	{
		FindClose(Hfound);
		return false;
	}else
	{
		if(MyFoundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(Hfound);
			return false;
		}else
			FindClose(Hfound);
		return true;
	}
}

void FileToCsl(char* finame,CStringList* CSL)
{
	if(FileExists(finame))
	{
		char buf[512] = "";
		CStdioFile ifs((LPCTSTR) finame , CFile::modeRead);
		if(ifs.ReadString(buf,512) != NULL)
		{
			do
			{
				int l = strlen(buf) - 1;
				if(l >= 0)
				{
					if(*(buf+l) == '\n')
						*(buf+l) = NULL;
				}
				CString newitm = buf;
				if(newitm=="");
				else
				{
					POSITION pos1,pos2;
					if( ( pos1 = CSL->Find(newitm, NULL ) )!= NULL );    
					else if( ( pos1 = CSL->GetHeadPosition() ) != NULL )    
						pos2 = CSL->InsertAfter( pos1, newitm );  
					else
						CSL->AddHead(newitm);	
				}	
			}while(ifs.ReadString(buf,512) != NULL);
		}
		ifs.Close();
	}
}

bool GetItemFromCString(int index,CString* CSTARGET,CString* CSITEM,char sep)
{
	int length = CSTARGET->GetLength();
	int poz[256];
	int ind = 0;
	for(int i=0 ; i<length ; i++)
	{
		if(*((LPCSTR)*CSTARGET+i) == sep || ( (i==length-1)&&(ind==0) ))
		{
			if((i==length-1)&&(ind==0))
				poz[ind] = i+1;
			else
				poz[ind] = i;
			ind ++;
		}	
	}
	if(index == 0)
	{
		*CSITEM = CSTARGET->Mid(0,poz[0]);
		return TRUE;
	}else if(index > 0 && index <ind)
	{
		*CSITEM = CSTARGET->Mid(poz[index-1]+1,poz[index]-poz[index-1]-1);
		return TRUE;
	}else if(index == ind)
	{
		*CSITEM = CSTARGET->Mid(poz[index-1]+1,length-poz[index-1]-1);
		return TRUE;
	}
	return FALSE;
}
