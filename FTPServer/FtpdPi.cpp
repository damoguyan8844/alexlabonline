#include "stdafx.h"
#include "CFtpd.h"
#include "FtpdPi.h"

#define CRLF "\r\n"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char sadr2[32];

bool canclePasvListenSocket(CFtpPasvSrv* sck)
{
	try
	{
		sck->Close();
		delete sck;
		return true;
	}
	catch(...)
	{
	}
	return false;
}

bool deleFtpPI(CFtpdPi* sck)
{
	try
	{
		CFtpd* K = (CFtpd*) sck->pFtpd;
		K->RemoveClient(sck);
	}
	catch(...)
	{
	}
	return true;
}

UINT thFtpCmd( LPVOID pParam )
{
	((CFtpdPi*)pParam)->ProcessMsg(((CFtpdPi*)pParam)->bufcmd);
	return 1;
}

UINT thSendData( LPVOID pParam)
{
	CFtpdPi* pFtpBind = (CFtpdPi*) pParam;
	CFile* inFile = NULL;
	try
	{
		char path[256] = "";
		strcpy(path,pFtpBind->RemoveLeadingSpace(pFtpBind->curCmdth+5));
		char srcPath[300] = "";
		if(strcmp(pFtpBind->vCurrentDir,"/")==0)
			wsprintf(srcPath,"%s%s%s",pFtpBind->vRealRoot,pFtpBind->vCurrentDir,path);
		else
			wsprintf(srcPath,"%s%s\\%s",pFtpBind->vRealRoot,pFtpBind->vCurrentDir,path);
		pFtpBind->slashFix(srcPath);
		if(pFtpBind->isFilePresent(path))
		{
			bool b_opened = true;
			try
			{
				if(pFtpBind->type == 1)
					inFile = new CFile(srcPath,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary);//CStdioFile(srcPath,CFile::modeRead|CFile::shareDenyNone|CFile::typeText);
				else if(pFtpBind->type == 3)
					inFile = new CFile(srcPath,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary);
			}
			catch(...)
			{
				b_opened = false;
			}
			if(b_opened)
			{
				long flen = inFile->GetLength(); 
				if(pFtpBind->restart_marker > 0)
				{
					inFile->Seek(pFtpBind->restart_marker,CFile::begin);
					pFtpBind->restart_marker = 0;
				}
				long nCount = 0;
				if(pFtpBind->type == 1)
					wsprintf(pFtpBind->str,"150 Sending %s (%u bytes). Mode STREAM Type ASCII%s",path,flen,CRLF);
				else if(pFtpBind->type == 3)
					wsprintf(pFtpBind->str,"150 Sending %s (%u bytes). Mode STREAM Type BINARY%s",path,flen,CRLF);
				pFtpBind->Send(pFtpBind->str,strlen(pFtpBind->str),0);
				if(flen)
				{
					long bytesent = 0;
					long offset = 0;
					DWORD start,end;
					do
					{
						if(flen > pFtpBind->m_buf_length_out)
							nCount = inFile->Read(pFtpBind->lpDataBuf_out,pFtpBind->m_buf_length_out);
						else
							nCount = inFile->Read(pFtpBind->lpDataBuf_out,flen);
						flen = flen - nCount;
						if(pFtpBind->b_speed_limit_out)
						{
							bytesent = 0; 
							offset = 0;
							HANDLE h = CreateEvent(NULL,false,false,"noname");
							do
							{
								start = GetTickCount();
								if((nCount-offset) > pFtpBind->m_buf_length_out)
									bytesent = pFtpBind->dbsocket->Send((char*)pFtpBind->lpDataBuf_out+offset,pFtpBind->m_buf_length_out,60);
								else
									bytesent = pFtpBind->dbsocket->Send((char*)pFtpBind->lpDataBuf_out+offset,nCount-offset,60);
								offset = offset + bytesent;
								end = GetTickCount();
								if((end-start) < 1000)
								{
									int time = end-start;
									WaitForSingleObject(h,1000-time);
								}
							}while(offset < nCount);
						}
						else
						{
							bytesent = 0; 
							offset = 0;
							do
							{
								if((nCount-offset) > pFtpBind->m_buf_length_out)
									bytesent = pFtpBind->dbsocket->Send((char*)pFtpBind->lpDataBuf_out+offset,pFtpBind->m_buf_length_out,60);
								else
									bytesent = pFtpBind->dbsocket->Send((char*)pFtpBind->lpDataBuf_out+offset,nCount-offset,60);
								offset = offset + bytesent;
							}while(offset < nCount);
							if(pFtpBind->type == 1)
							{
								pFtpBind->dbsocket->Send(CRLF,2,60);
							}
						}
					}while(flen != 0);
				}
				pFtpBind->dbsocket->Close();
				if(pFtpBind->b_passive)
					pFtpBind->svSocketSrv->Close();
				delete inFile;
				wsprintf(pFtpBind->str,"226 Transfer finished successfully. Data connection closed.%s",CRLF);
				pFtpBind->Send(pFtpBind->str,strlen(pFtpBind->str),0);
				SetEvent(pFtpBind->h_inprogress);
				return true;
			}else
			{
				pFtpBind->dbsocket->Close();
				if(pFtpBind->b_passive)
					pFtpBind->svSocketSrv->Close();
				if(inFile != NULL)
					delete inFile;
				wsprintf(pFtpBind->str,"426 Connection closed , file opening failed%s",CRLF);
				pFtpBind->Send(pFtpBind->str,strlen(pFtpBind->str),0);
				SetEvent(pFtpBind->h_inprogress);
			}
		}
	}catch(...)
	{
		inFile->Close();
		pFtpBind->dbsocket->Close();
		canclePasvListenSocket(pFtpBind->svSocketSrv);
		delete inFile;
		wsprintf(pFtpBind->str,"426 Connection closed; transfer aborted (transfer failed)%s",CRLF);
		pFtpBind->Send(pFtpBind->str,strlen(pFtpBind->str),0);
		SetEvent(pFtpBind->h_inprogress);
		return false;
	}
	return true;
}



CFtpdPi::CFtpdPi()
{
	clientIpCount = 0;
	type = 3; //1 = ascii 2 = ebcdic 3 = binary
	ipRoller = 0;
	b_use_bsock = true;
	b_verbose = true;
	b_passive = false;
	b_bell = false;
	b_prompt = true;
	b_globbing = true;
	b_debugging = false;
	b_hashmark = false;
	bLOGINOK = false;
	bUSER = false;
	bPASS = false;
	b_speed_limit_out = false;
	b_speed_limit_in = false;
	restart_marker = 0;
	strcpy(vCurrentDir,"/");
	strcpy(old_filename,"");
	strcpy(new_filename,"");
	strcpy(m_group,"nogroup");
	m_buf_length_out = 32768;
	m_buf_length_in = 32768;
	lpDataBuf_out = (unsigned char*) malloc(m_buf_length_out);
	memset(lpDataBuf_out,0,m_buf_length_out);
	lpDataBuf_in = (unsigned char*) malloc(m_buf_length_in);
	memset(lpDataBuf_in,0,m_buf_length_in);
	strcpy(monthStr[1],"Jan");
	strcpy(monthStr[2],"Feb");
	strcpy(monthStr[3],"Mar");
	strcpy(monthStr[4],"Apr");
	strcpy(monthStr[5],"May");
	strcpy(monthStr[6],"Jun");
	strcpy(monthStr[7],"Jul");
	strcpy(monthStr[8],"Aug");
	strcpy(monthStr[9],"Sep");
	strcpy(monthStr[10],"Oct");
	strcpy(monthStr[11],"Dec");
	strcpy(monthStr[12],"Nov");
	sl = sizeof(SOCKADDR_IN);
	GetLocalTime(&MyTime);
	if(FileExists(ipfile))
		LoadIpRange();
	h_inprogress = CreateEvent(NULL,false,true,"in_progress");
	h_dbsocket_ready_to_send = CreateEvent(NULL,false,false,"in_progress");
	dbsocket = NULL;
	svSocketSrv = NULL;
}

CFtpdPi::~CFtpdPi()
{
	free(lpDataBuf_in);
	free(lpDataBuf_out);
	if(dbsocket != NULL)
	{
		dbsocket->Close();
		delete dbsocket;
	}
	if(svSocketSrv != NULL)
	{
		svSocketSrv->Close();
		delete svSocketSrv;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFtpdPi, CSocket)
//{{AFX_MSG_MAP(CFtpdPi)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

void CFtpdPi::OnReceive(int nErrorCode) 
{
	unsigned char* bufnow = (unsigned char*)malloc(256);
	try
	{
		memset(bufnow,NULL,256);
		result = Receive(bufnow,256,0);
		bool cont = true;
		int currentPos = 0;
		if(*(bufnow+1)==0xf2)
			currentPos = 1;
		for(int i=0;i<result;i++)
		{
			if(*(bufnow+i)==0x0d&&*(bufnow+i+1)==0x0a)
			{
				strncpy(bufcmd,(char*)bufnow+currentPos,i-currentPos);
				*(bufcmd+i-currentPos) = NULL;
				strcpy(bufcmd_history,bufcmd);
				AfxBeginThread(&thFtpCmd,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
				currentPos = i+2;
				i = i + 1;
			}
		}
		free(bufnow);
	}
	catch (...)
	{
		delete bufnow;
		char lpMsgBuf[128]="";
		DWORD eRRR = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
		char ecp[256]="";
		wsprintf(ecp,"%s\\core.ftpd",AppDir);
		CStdioFile ofs((LPCTSTR) ecp ,CFile::modeWrite|CFile::modeCreate);
		ofs.WriteString("*---------------------------------------------------------------------------\n");
		ofs.WriteString("Exception occurs while processing socket msg\n");
		GetLocalTime(&MyTime);
		wsprintf(ecp,"Thread crashed at %.4d-%.2d-%.2d-%.2d-%.2d\n",MyTime.wYear,MyTime.wMonth,MyTime.wDay,MyTime.wHour,MyTime.wMinute);
		ofs.WriteString(ecp);
		ofs.WriteString(lpMsgBuf);
		ofs.WriteString("*---------------------------------------------------------------------------\n");
		ofs.Close();
		wsprintf(ecp,"Socket Exception occurs , aboarting current thread");
	}	
	CSocket::OnReceive(nErrorCode);
}

void CFtpdPi::ProcessMsg(char *cmd)
{
	char curCmd[256] = "";
	strcpy(curCmd,cmd);
	for(int k=0;k<=3;k++)
	{
		if(curCmd[k] != ' ')
		{
			curCmd[k] = toupper(curCmd[k]);
		}else
			k = 3;
	}
	if(!bLOGINOK)
	{
		if(strncmp("USER ",curCmd,5) == 0)
		{
			char username[256] = "";
			strcpy(username,RemoveLeadingSpace(curCmd+5));
			if(validuser(username) && strlen(username) <= 32)
			{
				strcpy(m_username,username);
				bUSER = true;
				wsprintf(str,"331 Password required for %s .%s",(LPCSTR)m_username,CRLF);
				SafeSend(str);
			}else
			{
				wsprintf(str,"530 Login incorrect for %s .%s",username,CRLF);
				SafeSend(str);
			}
		}else if(strncmp("PASS ",curCmd,5) == 0 && bUSER == true)
		{
			char password[256] = "";
			strcpy(password,RemoveLeadingSpace(curCmd+5));
			if(matchpass(password,(char*)(LPCSTR)m_username) && strlen(password) <= 32)
			{
				strcpy(m_password,password);
				bPASS = true;
			}else
			{
				wsprintf(str,"530 Password incorrect.%s",CRLF);
				SafeSend(str);
			}
		}
		if(bUSER && bPASS)
		{
			bLOGINOK = true;
			wsprintf(str,"230 User %s logged in , proceed%s",(LPCSTR)m_username,CRLF);
			SafeSend(str);
		}
	}else
	{
		if(stricmp("XPWD",curCmd) == 0 || stricmp("PWD",curCmd) == 0)
		{
			if(strlen(vCurrentDir) > 500)
				wsprintf(str,"503 current directory is too long.%s",CRLF);
			else
				wsprintf(str,"257 \"%s\" is current directory%s",vCurrentDir,CRLF);
			SafeSend(str);
			return;
		}
		else if(strncmp("CWD ",curCmd,4) == 0)
		{
			char path[512] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+4));
			if(strcmp(path,"..")==0)
			{
				if(goParent(vCurrentDir))
					wsprintf(str,"250 \"%s\" is current directory.%s",vCurrentDir,CRLF);
				else
					wsprintf(str,"550 Permission denied.%s",CRLF);
			}else if(strcmp(path,".")==0)
				wsprintf(str,"250 \"%s\" is current directory.%s",vCurrentDir,CRLF);
			else
			{
				fixVpath(path);
				if(isVpathPresent(path))
				{
					strcpy(vCurrentDir,path);
					wsprintf(str,"250 \"%s\" is current directory.%s",vCurrentDir,CRLF);
				}else
					wsprintf(str,"550 Permission denied.%s",CRLF);
			}
			SafeSend(str);
			return;
		}
		else if(strncmp("CDUP",curCmd,4) == 0)
		{
			if(goParent(vCurrentDir))
				wsprintf(str,"250 \"%s\" is current directory.%s",vCurrentDir,CRLF);
			else
				wsprintf(str,"550 Permission denied.%s",CRLF);
			SafeSend(str);
			return;
		}
		else if(strcmp("REIN",curCmd) == 0)
		{
			bUSER = false;
			bPASS = false;
			bLOGINOK = false;
			wsprintf(str,"550 Relogin required , connection reinitialized.%s",CRLF);
			SafeSend(str);
			return;
		}
		else if(strncmp("DELE ",curCmd,5) == 0)
		{
			char path[512] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+4));
			if(vRemove(path))
				wsprintf(str,"250 Requested file action ok , completed%s",CRLF);
			else
			{
				char lpMsgBuf[128]="";
				DWORD eRRR = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
				wsprintf(str,"553 Requested action not taken , %s",lpMsgBuf);
			}
			SafeSend(str);
			return;
		}
		else if(strncmp("XMKD ",curCmd,5) == 0)
		{
			char path[512] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+5));
			if(vMkdir(path))
				wsprintf(str,"257 \"%s\" created%s",path,CRLF);
			else
			{
				char lpMsgBuf[128]="";
				DWORD eRRR = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
				wsprintf(str,"553 Requested action not taken , %s",lpMsgBuf);
			}
			SafeSend(str);
			return;
		}
		else if(strncmp("RETR ",curCmd,5) == 0)
		{
			strcpy(curCmdth,curCmd);
			AfxBeginThread(&thSendData,this,THREAD_PRIORITY_TIME_CRITICAL,0,0,NULL);
			return;
		}
		else if(strcmp("ABOR",curCmd) == 0)
		{
			try
			{
				dbsocket->Close();
			}
			catch(...)
			{
			}
			wsprintf(str,"426 Transfer aborted. Data connection closed.%s",CRLF);
			SafeSend(str);
			SetEvent(h_inprogress);
			wsprintf(str,"226 Abort successful.%s",CRLF);
			SafeSend(str);
			return;
		}
		else if(strncmp("STOR ",curCmd,5) == 0 || strncmp("STOU ",curCmd,5) == 0)
		{
			CFile* outFile;
			try
			{
				char path[256] = "";
				strcpy(path,RemoveLeadingSpace(curCmd+5));
				char srcPath[300] = "";
				if(strcmp(vCurrentDir,"/")==0)
					wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,path);
				else
					wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
				slashFix(srcPath);
				if(isFilePresent(path) && strncmp("STOU ",curCmd,5) == 0)
				{
					dbsocket->Close();
					wsprintf(str,"426 Connection closed; transfer aborted (target file already existed)%s",CRLF);
					SafeSend(str);			
				}
				else
				{
					bool b_opened = true;
					try
					{
						if(type == 1)
							outFile = new CStdioFile(srcPath,CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText|CFile::modeCreate);
						else if(type == 3)
							outFile = new CFile(srcPath,CFile::modeWrite|CFile::modeNoTruncate|CFile::typeBinary|CFile::modeCreate);
					}
					catch(...)
					{
						b_opened = false;
					}
					if(b_opened)
					{
						long flen = 0;
						long nCount = 0;
						if(type == 1)
							wsprintf(str,"150 Opening ASCII data connection for %s%s",path,CRLF);
						else if(type == 3)
							wsprintf(str,"150 Opening BINARY data connection for %s%s",path,CRLF);
						SafeSend(str);
						DWORD start,end;
						HANDLE h = CreateEvent(NULL,false,false,"noname");
						do
						{
							start = GetTickCount();
							nCount = dbsocket->Receive((char*)lpDataBuf_in,m_buf_length_in,60);
							outFile->Write(lpDataBuf_in,nCount);
							end = GetTickCount();
							if(b_speed_limit_in)
							{
								if((end-start) < 1000)
								{
									int time = end-start;
									WaitForSingleObject(h,((1000-time)*nCount)/m_buf_length_in);
								}
							}
						}while(nCount > 0);
						dbsocket->Close();
						if(b_passive)
							svSocketSrv->Close();
						delete outFile;
						wsprintf(str,"226 Transfer finished successfully. Data connection closed.%s",CRLF);
						SafeSend(str);
					}else
					{
						outFile->Close();
						dbsocket->Close();
						if(b_passive)
							svSocketSrv->Close();
						delete outFile;
						wsprintf(str,"426 Connection closed; file opening failed%s",CRLF);
						SafeSend(str);
					}
				}
			}
			catch(...)
			{
				outFile->Close();
				dbsocket->Close();
				if(b_passive)
					svSocketSrv->Close();
				delete outFile;
				wsprintf(str,"426 Connection closed; transfer aborted (transfer failed)%s",CRLF);
				SafeSend(str);
			}
			SetEvent(h_inprogress);
			return;
		}
		else if(strncmp("APPE ",curCmd,5) == 0)
		{
			CFile* outFile;
			try
			{
				char path[256] = "";
				strcpy(path,RemoveLeadingSpace(curCmd+5));
				char srcPath[300] = "";
				if(strcmp(vCurrentDir,"/")==0)
					wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,path);
				else
					wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
				slashFix(srcPath);
				bool b_opened = true;
				try
				{
					if(type == 1)
						outFile = new CStdioFile(srcPath,CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText|CFile::modeCreate);
					else if(type == 3)
						outFile = new CFile(srcPath,CFile::modeWrite|CFile::modeNoTruncate|CFile::typeBinary|CFile::modeCreate);
				}
				catch(...)
				{
					b_opened = false;
				}
				if(b_opened)
				{
					
					outFile->Seek(0,CFile::end);
					long nCount = 0;
					if(type == 1)
						wsprintf(str,"150 Opening ASCII data connection for %s%s",path,CRLF);
					else if(type == 3)
						wsprintf(str,"150 Opening BINARY data connection for %s%s",path,CRLF);
					SafeSend(str);
					DWORD start,end;
					HANDLE h = CreateEvent(NULL,false,false,"noname");
					do
					{
						start = GetTickCount();
						nCount = dbsocket->Receive((char*)lpDataBuf_in,m_buf_length_in,60);
						end = GetTickCount();
						outFile->Write(lpDataBuf_in,nCount);
						if(b_speed_limit_in)
						{
							if((end-start) < 1000)
							{
								int time = end-start;
								WaitForSingleObject(h,1000-time);
							}
						}
					}while(nCount > 0);
					dbsocket->Close();
					if(b_passive)
						svSocketSrv->Close();
					delete outFile;
					wsprintf(str,"226 Transfer finished successfully. Data connection closed.%s",CRLF);
					SafeSend(str);
				}else
				{
					outFile->Close();
					dbsocket->Close();
					if(b_passive)
						svSocketSrv->Close();
					delete outFile;
					wsprintf(str,"426 Connection closed file opening failed %s",CRLF);
					SafeSend(str);
				}
			}catch(...)
			{
				outFile->Close();
				dbsocket->Close();
				if(b_passive)
					svSocketSrv->Close();
				delete outFile;
				wsprintf(str,"426 Connection closed; transfer aborted (transfer failed)%s",CRLF);
				SafeSend(str);
			}
			SetEvent(h_inprogress);
			return;
		}
		else if(strncmp("SIZE ",curCmd,5) == 0)
		{
			try
			{
				char path[256] = "";
				strcpy(path,RemoveLeadingSpace(curCmd+5));
				char srcPath[300] = "";
				if(strcmp(vCurrentDir,"/")==0)
					wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,path);
				else
					wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
				slashFix(srcPath);
				HANDLE Hfound;
				WIN32_FIND_DATA FoundFile;
				Hfound=FindFirstFile(srcPath,&FoundFile);
				DWORD size_t = FoundFile.nFileSizeHigh*MAXDWORD+FoundFile.nFileSizeLow;
				if(Hfound != INVALID_HANDLE_VALUE)
					wsprintf(str,"213 %lu%s",size_t,CRLF);
				else
					wsprintf(str,"213 0%s",CRLF);
				SafeSend(str);
				FindClose(Hfound);
			}catch(...)
			{
				wsprintf(str,"213 0%s",CRLF);
				SafeSend(str);
			}
			return;
		}
		else if(strncmp("MDTM ",curCmd,5) == 0)
		{
			try
			{
				char path[256] = "";
				strcpy(path,RemoveLeadingSpace(curCmd+5));
				char srcPath[300] = "";
				if(strcmp(vCurrentDir,"/")==0)
					wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,path);
				else
					wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
				slashFix(srcPath);
				HANDLE Hfound;
				WIN32_FIND_DATA FoundFile;
				FILETIME lft;
				SYSTEMTIME st;
				Hfound=FindFirstFile(srcPath,&FoundFile);
				FileTimeToLocalFileTime(&FoundFile.ftLastWriteTime,&lft);
				FileTimeToSystemTime(&lft,&st);
				wsprintf(str,"213 %.4d%.2d%.2d%.2d%.2d%.2d%s",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,CRLF);
				SafeSend(str);
				FindClose(Hfound);
			}catch(...)
			{
				wsprintf(str,"213 0%s",CRLF);
				SafeSend(str);
			}
			return;
		}
		else if(strncmp("MKD ",curCmd,4) == 0)
		{
			char path[512] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+4));
			if(vMkdir(path))
				wsprintf(str,"250 Requested file action ok , completed%s",CRLF);
			else
			{
				char lpMsgBuf[128]="";
				DWORD eRRR = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
				wsprintf(str,"553 Requested action not taken , %s",lpMsgBuf);
			}
			SafeSend(str);
			return;
		}
		else if(strncmp("RMD ",curCmd,4) == 0)
		{
			char path[512] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+4));
			if(vRmdir(path))
				wsprintf(str,"250 Requested file action ok , completed%s",CRLF);
			else
			{
				char lpMsgBuf[128]="";
				DWORD eRRR = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
				wsprintf(str,"553 Requested action not taken , %s",lpMsgBuf);
			}
			SafeSend(str);
			return;
		}
		else if(strncmp("TYPE ",curCmd,5) == 0)
		{
			char path[8] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+5));
			if(strcmp("I",path)==0)
			{
				type = 3;
				wsprintf(str,"200 Type set to IMAGE.%s",CRLF);
			}else if(strcmp("A",path)==0)
			{
				type = 1;
				wsprintf(str,"200 Type set to ASCII.%s",CRLF);
			}
			SafeSend(str);
			return;
		}
		else if(stricmp("SYST",curCmd) == 0)
		{
			//	wsprintf(str,"215 %s%s",sinfo.OSVER,CRLF);
			wsprintf(str,"215 %s%s","UNIX",CRLF);
			SafeSend(str);
			return;
		}
		else if(stricmp("QUIT",curCmd) == 0)
		{
			wsprintf(str,"221 Goodbye.%s",CRLF);
			SafeSend(str);
			deleFtpPI(this);
			return;
		}
		else if(stricmp("REST ",curCmd) == 0)
		{
			char marker[32] = "";
			strcpy(marker,RemoveLeadingSpace(curCmd+5));
			restart_marker = atol(marker);
			wsprintf(str,"110 Command Ok , restarting position set to %u%s",restart_marker,CRLF);
			SafeSend(str);
			return;
		}
		else if(strncmp("RNFR ",curCmd,5) == 0)
		{
			strcpy(old_filename,RemoveLeadingSpace(curCmd+5));
			wsprintf(str,"350 Requested file action pending further information.%s",CRLF);
			SafeSend(str);
			return;
		}
		else if(strncmp("PORT ",curCmd,5) == 0)
		{
			char path[32] = "";
			int pHi=0;
			int pLo=0;
			int strOffset=0;
			int portOffset=0;
			int commaCnt=0;
			char ipStr[32]="";
			strcpy(path,RemoveLeadingSpace(curCmd+5));
			for(unsigned int i=0;i<strlen(path);i++)
			{
				if(path[i] == ',')
				{
					commaCnt++;
					path[i] = '.';
				}
				if(commaCnt == 4)
				{
					strOffset = i;
					i = strlen(path);
				}
			}
			strncpy(ipStr,path,strOffset);
			for(i=0;i<strlen(path+strOffset);i++)
			{
				if(path[strOffset+i] == ',')
				{
					portOffset = i;
					path[strOffset+i] = NULL;
				}
			}
			pHi = atoi(path+strOffset+1);
            pLo = atoi(path+strOffset+portOffset+1);
			data_port = pHi*256 + pLo;
			strcpy(data_ip,ipStr);
			WaitForSingleObject(h_inprogress,10000);
			try
			{
				ResetEvent(h_inprogress);
				if(dbsocket != NULL)
				{
					dbsocket->Close();
					delete dbsocket;
				}
				dbsocket = new CBlockingSocket();
				if(!dbsocket->Create(SOCK_STREAM))
					wsprintf(str,"425 Cannot open data connection (%s %d)%s",data_ip,data_port,CRLF);
				else
				{
					CSockAddr tSockAdr(data_ip,data_port);
					if(dbsocket->Connect(tSockAdr))
					{
						SetEvent(h_dbsocket_ready_to_send);
						wsprintf(str,"200 Port command okay.%s",CRLF);
					}
					else
						wsprintf(str,"425 Cannot open data connection (%s %d)%s",data_ip,data_port,CRLF);
				}
			}catch(...)
			{
				wsprintf(str,"425 Cannot open data connection (%s %d)%s",data_ip,data_port,CRLF);
			}
			SafeSend(str);
			return;
		}
		else if(strcmp("NLST",curCmd) == 0)
		{
			ListEngine("*.*",false);
			return;
		}
		else if(strcmp("LIST",curCmd) == 0)
		{
			ListEngine("*.*",true);
			return;
		}
		else if(strncmp("NLST ",curCmd,5) == 0)
		{
			char path[256] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+5));
			ListEngine(path,false);
			return;
		}
		else if(strncmp("LIST ",curCmd,5) == 0)
		{
			char path[256] = "";
			strcpy(path,RemoveLeadingSpace(curCmd+5));
			ListEngine(path,true);
			return;
		}
		else if(strncmp("RNTO ",curCmd,5) == 0)
		{
			strcpy(new_filename,RemoveLeadingSpace(curCmd+5));
			if(vMoveFile(old_filename,new_filename))
				wsprintf(str,"250 Requested file action ok , completed%s",CRLF);
			else
			{
				char lpMsgBuf[128]="";
				DWORD eRRR = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,eRRR,0,lpMsgBuf,128,NULL);
				wsprintf(str,"553 Requested action not taken , %s%s",lpMsgBuf,CRLF);
			}
			SafeSend(str);
			return;
		}
		else if(stricmp("NOOP",curCmd) == 0)
		{
			wsprintf(str,"200 Command Ok , NOOP%s",CRLF);
			SafeSend(str);
			return;
		}
		else if(strcmp("PASV",curCmd) == 0)
		{
			try
			{
				int pHi=0;
				int pLo=0;
				char ipStr[32]="";
				WaitForSingleObject(h_inprogress,10000);
				ResetEvent(h_inprogress);
				if(dbsocket != NULL)
				{
					dbsocket->Close();
					delete dbsocket;
				}
				dbsocket = new CBlockingSocket();
				if(svSocketSrv != NULL)
				{
					svSocketSrv->Close();
					delete svSocketSrv;
				}
				svSocketSrv = new CFtpPasvSrv();
				if(1)//dbsocket->Create(SOCK_STREAM))
				{
					svSocketSrv->Create(SOCK_STREAM);
					m_port_from++;
					if(m_port_from >= m_port_to)
						m_port_from = m_port_from_preset;
					for(UINT i=m_port_from;i<=m_port_to;i++)
					{
						CSockAddr tSockAdr(INADDR_ANY,i);
						if(svSocketSrv->Bind(tSockAdr))
						{
							svSocketSrv->m_current_port = i;
							i = 65536;
						} 
					}	
					if(svSocketSrv->m_current_port && svSocketSrv->Listen())
					{
						pHi = (svSocketSrv->m_current_port-svSocketSrv->m_current_port%256)/256 ;
						pLo = svSocketSrv->m_current_port%256;
						b_passive = true;
						CString ip;
						if(!b_switch_pasv_ip)
							ip = ipList.GetTail();
						else
						{
							pos = ipList.GetHeadPosition();
							for(int j=0;j<=ipRoller;j++)
							{
								ip = ipList.GetNext(pos);
							}
							if(ipRoller+1 < ipList.GetCount())
								ipRoller++;
							else
								ipRoller=0;
						}
						for(int i=0;i<ip.GetLength();i++)
						{
							if(ip.GetAt(i) == '.')
								ip.SetAt(i,',');
						}
						wsprintf(str,"227 Entering Passive Mode (%s,%d,%d)%s",(LPCSTR)ip,pHi,pLo,CRLF);
						SafeSend(str);	
						svSocketSrv->Accept(*dbsocket,(SOCKADDR*)&lpsockftpd);
						wsprintf(str,"%d.%d.%d.%d",lpsockftpd.sin_addr.S_un.S_un_b.s_b1,lpsockftpd.sin_addr.S_un.S_un_b.s_b2,lpsockftpd.sin_addr.S_un.S_un_b.s_b3,lpsockftpd.sin_addr.S_un.S_un_b.s_b4);
						if(!isAllowedIp(str))
						{
#ifdef _TEST
							MessageBox(NULL,"ERROR","ERROR_PASV_NOT_ALLOWED_IP",MB_OK);
#endif
							canclePasvListenSocket(svSocketSrv);
							wsprintf(str,"425 Cannot open data connection (client ip denied)%s",CRLF);
							SafeSend(str);
						}
						else
							SetEvent(h_dbsocket_ready_to_send);
						return;
					}
					else
					{
						wsprintf(str,"425 Cannot open data connection (%s %d create failed)%s",(LPCSTR)ipList.GetTail(),svSocketSrv->m_current_port,CRLF);
						SafeSend(str);
					}
				}else
				{
					wsprintf(str,"425 Cannot open data connection (%s %d create failed)%s",(LPCSTR)ipList.GetTail(),svSocketSrv->m_current_port,CRLF);
					SafeSend(str);
				}
			}
			catch(...)
			{
#ifdef _TEST
				MessageBox(NULL,"ERROR","ERROR_PASV",MB_OK);
#endif
			}
			return;
		}
		else
		{
			if(strcmp(curCmd,"") !=0)
			{
				wsprintf(str,"502 Command %s not implemented%s",curCmd,CRLF);
				SafeSend(str);
			}
			return;
		}
	}
}

char* CFtpdPi::RemoveLeadingSpace(char *str)
{
	try
	{
		int breakat = 0;
		for(unsigned int i=0;i<strlen(str);i++)
		{
			if(*(str+i) != ' ')
			{
				breakat = i;
				i = strlen(str);
			}
		}
		return str+breakat;
	}
	catch(...)
	{
		return "";
	}
}

bool CFtpdPi::vMoveFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	char srcPath[512] = "";
	char tgtPath[512] = "";
	if(strcmp(vCurrentDir,"/")==0)
	{
		wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,lpExistingFileName);
		wsprintf(tgtPath,"%s%s%s",vRealRoot,vCurrentDir,lpNewFileName);
	}else
	{
		wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,lpExistingFileName);
		wsprintf(tgtPath,"%s%s\\%s",vRealRoot,vCurrentDir,lpNewFileName);
	}
	if(MoveFile(srcPath,tgtPath))
		return true;
	else
	{
		return false;
	}
}

bool CFtpdPi::isFilePresent(char *path)
{
	WIN32_FIND_DATA MyFoundFile;
	char srcPath[512] = "";
	if(strcmp(vCurrentDir,"/")==0)
		wsprintf(srcPath,"%s\\%s",vRealRoot,path);
	else
		wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
	slashFix(srcPath);
	HANDLE Hfound=FindFirstFile(srcPath,&MyFoundFile);
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

bool CFtpdPi::isVpathPresent(char *vpath)
{
	WIN32_FIND_DATA MyFoundFile;
	char srcPath[512] = "";
	if(strcmp(vpath,"/")==0)
		wsprintf(srcPath,"%s",vRealRoot);
	else
		wsprintf(srcPath,"%s%s",vRealRoot,vpath);
	slashFix(srcPath);
	HANDLE Hfound=FindFirstFile(srcPath,&MyFoundFile);
	if(Hfound == INVALID_HANDLE_VALUE)
	{
		FindClose(Hfound);
		return false;
	}else
	{
		if(MyFoundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(Hfound);
			return true;
		}else
			FindClose(Hfound);
		return false;
	}
}

char* CFtpdPi::fixVpath(char *vPath)
{
	char retPath[512]="";
	strcpy(retPath,replaceDotInVpath(vPath));
	if(strncmp("/",retPath,1)==0)
	{
		return vPath;
	}
	else
	{
		if(strcmp("/",vCurrentDir) == 0)
		{
			wsprintf(vPath,"/%s",retPath);
			return vPath;
		}
		else
		{
			wsprintf(vPath,"%s/%s",vCurrentDir,retPath);
			return vPath;
		}
	}
}

char* CFtpdPi::replaceDotInVpath(char *vPath)
{
	char vCurDirTmp[512] = "";
	strcpy(vCurDirTmp,vCurrentDir);
	char* outPut = (char*) malloc(512);
	int outPos = 0;
	bool dotAgain =  false;
	memset(outPut,NULL,512);
	for(unsigned int i =0;i<=strlen(vPath)-1;i++)
	{
		if(*(vPath+i) == '.')
		{
			if(*(vPath+i+1) == '/' || *(vPath+i+1) == NULL)
			{
				i=i+2;
			}
			else if(*(vPath+i+1) == '.' && (*(vPath+i+2) == '/' || *(vPath+i+2) == NULL))
			{
				if(dotAgain)
					strcpy(vCurDirTmp,outPut);
				if(!goParent(vCurDirTmp))
				{
					
				}else
				{
					strcpy(outPut,vCurDirTmp);
					if(strcmp(outPut,"/") == 0)
					{
						outPos = 1;
						i=i+2;
					}else
					{
						i=i+2;
						if(i <= strlen(vPath)-1)
						{
							int a = strlen(outPut);
							*(outPut+a) = '/';
							*(outPut+a+1) = NULL;
						}
						outPos = strlen(outPut);
					}
					dotAgain = true;
				}
			}
			else
			{
				*(outPut+outPos) = *(vPath+i);
				outPos++;
			}
		}else
		{
			*(outPut+outPos) = *(vPath+i);
			outPos++;
		}
	}
	*(outPut+outPos) = NULL;
	strcpy(vPath,outPut);
	free(outPut);
	return vPath;
}

bool CFtpdPi::goParent(char *path)
{
	for(unsigned int i=strlen(path)-1;i>=0;i--)
	{
		if( *(path+i) == '/' && i != strlen(path)-1)
		{
			if(i>0)
			{
				*(path+i) = NULL;
				i=0;
			}else
				*(path+1) = NULL;
			return true;
		}
	}
	return false;
}

bool CFtpdPi::vRemove(char *file)
{
	char srcPath[512] = "";
	if(strcmp(vCurrentDir,"/")==0)
		wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,file);
	else
		wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,file);
	slashFix(srcPath);
	if(remove(srcPath)==0)
		return true;
	else
	{
		return false;
	}
}

bool CFtpdPi::vMkdir(char *dir)
{
	char srcPath[512] = "";
	if(strcmp(vCurrentDir,"/")==0)
		wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,dir);
	else
		wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,dir);
	slashFix(srcPath);
	if(CreateDirectory(srcPath,NULL))
		return true;
	else
	{
		return false;
	}
}

void CFtpdPi::slashFix(char *path)
{
	for(unsigned int i=0;i<strlen(path);i++)
	{
		if(*(path+i) == '/')
			*(path+i) = '\\';
	}
	return;
}

bool CFtpdPi::vRmdir(char *path)
{
	char srcPath[512] = "";
	if(strcmp(vCurrentDir,"/")==0)
		wsprintf(srcPath,"%s%s%s",vRealRoot,vCurrentDir,path);
	else
		wsprintf(srcPath,"%s%s\\%s",vRealRoot,vCurrentDir,path);
	slashFix(srcPath);
	if(RemoveDirectory(srcPath))
		return true;
	else
	{
		return false;
	}
}


void CFtpdPi::slashFixUnix(char *path)
{
	for(unsigned int i=0;i<strlen(path);i++)
	{
		if(*(path+i) == '\\')
			*(path+i) = '/';
	}
	return;
}

bool CFtpdPi::validuser(char *buf)
{
	return true;
}

bool CFtpdPi::matchpass(char *buf, char *uname)
{
	return true;
}


void CFtpdPi::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFtpd* K = (CFtpd*) pFtpd;
	K->RemoveClient(this);	
	CSocket::OnClose(nErrorCode);
}

void CFtpdPi::SetOutStreamSpeed(int kbs)
{
	free(lpDataBuf_out);
	lpDataBuf_out = (unsigned char*) malloc(kbs*1024);
	m_buf_length_out = kbs*1024;
	b_speed_limit_out = true;
	return;
}

void CFtpdPi::SetInStreamSpeed(int kbs)
{
	free(lpDataBuf_in);
	lpDataBuf_in = (unsigned char*) malloc(kbs*1024);
	m_buf_length_in = kbs*1024;
	b_speed_limit_in = true;
	return;
}

void CFtpdPi::SafeSend(char *str)
{
	int len = strlen(str);
	try
	{
		int result = Send(str,len,0);
		if(result < len)
		{
#ifdef _TEST
			MessageBox(NULL,"ERROR","ERROR_SEND_TWICE",MB_OK);
#endif
			result = Send(str+result,len-result,0);
		}
	}catch(...)
	{
#ifdef _TEST
		MessageBox(NULL,"ERROR","ERROR_SEND_EXCEPTION",MB_OK);
#endif
	}
}

bool CFtpdPi::isAllowedIp(char *ip)
{
	for(int i = 0;i < clientIpCount;i++)
	{
		if(strncmp(ip,clientIp[i],strlen(clientIp[i]))==0)
			return true;
	}
	return false;
}

void CFtpdPi::addClientIp(char *ip)
{
	strcpy(clientIp[clientIpCount],ip);
	if(clientIpCount < 32)
		clientIpCount++;
	return;
}

void CFtpdPi::LoadIpRange()
{
	char buf[32] = "";
	CStdioFile ifs(ipfile , CFile::modeRead);
	if(ifs.ReadString(buf,32) != NULL)
	{
		do
		{
			int l = strlen(buf) - 1;
			if(l >= 0)
			{
				if(*(buf+l) == '\n')
					*(buf+l) = NULL;
			}
			addClientIp(buf);
		}while(ifs.ReadString(buf,32) != NULL);
	}
	return;
}

void CFtpdPi::ListEngine(char *vpath, bool longList)
{
	char path[256] = "";
	HANDLE Hfound;
	try
	{
		strcpy(path,vpath);
		WaitForSingleObject(h_dbsocket_ready_to_send,5000);
		ResetEvent(h_dbsocket_ready_to_send);
		char pattern[256] = "";
		if(strncmp(path,"-la",3)==0 || strncmp(path,"-l",2)==0 || strncmp(path,"-a",2)==0 || strcmp(path,"*")==0 || strcmp(path,".")==0)
			strcpy(pattern,"*.*");
		else if(strstr(path,"*.")!=NULL || strstr(path,"/*")!=NULL)
			strcpy(pattern,path);
		else
		{
			strcpy(pattern,path);
			if(strcmp(pattern,"/")==0)
				strcat(pattern,"*.*");
			else
				strcat(pattern,"/*.*");
		}
		fixVpath(pattern);
		WIN32_FIND_DATA FoundFile;
		char dbyte = '-';
		char perm[4] = "rwx";
		char temp1[300]="";
		FILETIME lft;
		SYSTEMTIME st;
		strcpy(perm,"rwx");
		if(strcmp(pattern,"/")==0)
			wsprintf(temp1,"%s\\*.*",vRealRoot);
		else
			wsprintf(temp1,"%s%s",vRealRoot,pattern);
		slashFix(temp1);
		wsprintf(str,"150 Opening ASCII NO-PRINT mode data connection for ls -l.%s",CRLF);
		SafeSend(str);
		Hfound=FindFirstFile(temp1,&FoundFile);
		if(FoundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			dbyte = 'd';
		else
			dbyte = '-';
		FileTimeToLocalFileTime(&FoundFile.ftLastWriteTime,&lft);
		FileTimeToSystemTime(&lft,&st);
		char timeStr2[16] = "";
		if(st.wYear != MyTime.wYear)
			wsprintf(timeStr2,"%5d",st.wYear);
		else
			wsprintf(timeStr2,"%.2d:%.2d",st.wHour,st.wMinute);
		if(!longList)
		{
			if(strcmp(FoundFile.cFileName,".") !=0 && strcmp(FoundFile.cFileName,"..") !=0)
			{	
				wsprintf(str,"%s%s",FoundFile.cFileName,CRLF);
				dbsocket->Send(str,strlen(str),20);
			}
		}
		else
		{
			wsprintf(str,"%c%s%s%s  1 %-9s %-9s %10u %s %d %s %s%s",dbyte,perm,perm,perm,(LPCSTR)m_username,(LPCSTR)m_group,(FoundFile.nFileSizeHigh*MAXDWORD)+FoundFile.nFileSizeLow,monthStr[st.wMonth],st.wDay,timeStr2,FoundFile.cFileName,CRLF);
			dbsocket->Send(str,strlen(str),20);
		}
		while(FindNextFile(Hfound,&FoundFile))
		{
			if(FoundFile.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				dbyte = 'd';
			else
				dbyte = '-';
			FileTimeToLocalFileTime(&FoundFile.ftLastWriteTime,&lft);
			FileTimeToSystemTime(&lft,&st);
			if(st.wYear != MyTime.wYear)
				wsprintf(timeStr2,"%5d",st.wYear);
			else
				wsprintf(timeStr2,"%.2d:%.2d",st.wHour,st.wMinute);
			if(!longList)
			{
				if(strcmp(FoundFile.cFileName,".") !=0 && strcmp(FoundFile.cFileName,"..") !=0)
				{	
					wsprintf(str,"%s%s",FoundFile.cFileName,CRLF);
					dbsocket->Send(str,strlen(str),20);
				}
			}
			else
			{
				wsprintf(str,"%c%s%s%s  1 %-9s %-9s %10u %s %d %s %s%s",dbyte,perm,perm,perm,(LPCSTR)m_username,(LPCSTR)m_group,(FoundFile.nFileSizeHigh*MAXDWORD)+FoundFile.nFileSizeLow,monthStr[st.wMonth],st.wDay,timeStr2,FoundFile.cFileName,CRLF);
				dbsocket->Send(str,strlen(str),20);
			}
		}
		FindClose(Hfound);
		dbsocket->Close();
		if(b_passive)
			svSocketSrv->Close();
		wsprintf(str,"226 Transfer finished successfully. Data connection closed.%s",CRLF);
		SafeSend(str);
		SetEvent(h_inprogress);
	}
	catch(...)
	{
		FindClose(Hfound);
		dbsocket->Close();
		if(b_passive)
			svSocketSrv->Close();
		wsprintf(str,"426 Connection closed; transfer aborted (transfer failed)%s",CRLF);
		SafeSend(str);
		SetEvent(h_inprogress);
	}
}
