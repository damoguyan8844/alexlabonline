#include "stdafx.h"
#include "cstrutil.h"

SECFileSystem csrutilfs;
bool quickInited = false;
int length = 0;
int length1 = 0;
int poz[32];
int ind = 0;
BOOL NormalizeSentence(char *str)
{
	int linelength = strlen(str);
	return TRUE;
}

BOOL WORDINFILE2CSL(char finame[255],CStringList* CSL)
{
	char *buf;
	int stoffset = 0;
	int edoffset = 0;
	BOOL newword = FALSE;
	BOOL st = FALSE;
	BOOL ed = FALSE;
	WIN32_FIND_DATA MyFoundFile1;
	HANDLE Hfound1 = FindFirstFile(finame,&MyFoundFile1);
	long iSize = MyFoundFile1.nFileSizeLow;
	buf = (char*)malloc(iSize);
	CFile ifs((LPCTSTR) finame , CFile::modeRead);
	ifs.Read(buf,iSize);
	FindClose(Hfound1);
	removenontext(buf,iSize);
	for(long i=0;i<iSize;i++)
	{
		if(*(buf+i) != ' ' && st == FALSE && ed == FALSE)
		{
			stoffset = i;
			st = TRUE;
		}else if(*(buf+i) == ' ' && st == TRUE && ed == FALSE)
		{
			edoffset = i;
			ed = TRUE;
		}		
		if(st == TRUE && ed == TRUE)
		{
			volatile char wrd[64];
			strncpy((char*)wrd,buf+stoffset,(edoffset-stoffset));
			*(wrd+edoffset-stoffset) = NULL;
			CString newitm = (char*)wrd;
			if(newitm=="");
			else
			{
				POSITION pos1,pos2;
				if( ( pos1 = CSL->GetHeadPosition() ) != NULL )
					pos2 = CSL->InsertAfter( pos1, newitm );  
				else
					CSL->AddHead(newitm);	
			}
			st = FALSE;
			ed = FALSE;
		}
	}
	return TRUE;
}


BOOL removenontext(char* str,long size)
{
	for(int i=0;i<size;i++)
	{
		if((str[i]=='\'')||(str[i]<=32)||(str[i]==34)||(str[i]==42)||(str[i]==47)||(str[i]>=58&&str[i]<=64)||(str[i]>=91&&str[i]<=96)||(str[i]>=58&&str[i]<=64)||str[i]>=123||str[i]=='\\'||str[i]=='-'||str[i]=='$'||str[i]=='%')
			str[i]=' ';
	}
	return TRUE;
}
void FILE2CSLNLIST(char* finame,CStringList* CSL,LPVOID plist)
{
	if(csrutilfs.FileExists(finame))
	{
		char buf[512] = "";
		CStdioFile ifs((LPCTSTR) finame , CFile::modeRead  | CFile::shareDenyNone);
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
					{
						pos2 = CSL->InsertAfter( pos1, newitm );  
						((CListBox*)plist)->AddString((LPCSTR)newitm);
					}else
					{
						CSL->AddHead(newitm);	
						((CListBox*)plist)->AddString((LPCSTR)newitm);
					}
				}	
			}while(ifs.ReadString(buf,512) != NULL);
		}
		ifs.Close();
	}
}

void FILE2CSL(char* finame,CStringList* CSL)
{
	if(csrutilfs.FileExists(finame))
	{
		char buf[512] = "";
		CStdioFile ifs((LPCTSTR) finame , CFile::modeRead | CFile::shareDenyNone);
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

void CSL2LIST(CStringList* CSL,LPVOID plist)
{
	CString Citem;
	POSITION pos;
	for(pos = CSL->GetHeadPosition(); pos != NULL; )    
	{
		Citem = CSL->GetNext( pos );
		((CListBox*)plist)->AddString((LPCSTR)Citem);			
	}
}

void CSL2FILE(CStringList* CSL,char* finame)
{
	CStdioFile ofs((LPCTSTR) finame ,CFile::modeWrite|CFile::modeCreate);
	POSITION pos;
	CString save;
	for( pos = CSL->GetHeadPosition(); pos != NULL; )   
	{
		save = CSL->GetNext( pos );
		ofs.WriteString((LPCTSTR)save);
		ofs.WriteString("\n");
	}
	ofs.Close();
}

void LIST2CSL(LPVOID plist,CStringList* CSL)
{
//	POSITION pos;
	CString save;
	int count = ((CListBox*)plist)->GetCount();
	for(int i=0;i<count;i++)   
	{
		CString CiTEM;
		((CListBox*)plist)->GetText(i,(char*)(LPCSTR)CiTEM);
		POSITION pos1,pos2;
		if((pos1 = CSL->Find(CiTEM, NULL)) != NULL );    
		else if((pos1 = CSL->GetHeadPosition()) == NULL )    
			CSL->AddHead(CiTEM);
		else
			pos2 = CSL->InsertAfter( pos1, CiTEM );   		
	}
}

BOOL GETITEMFROMCS(int index,CString* CSTARGET,CString* CSITEM,char sep)
{
	int length = CSTARGET->GetLength();
	int length1 = length - 1;
	int poz[32];
	int ind = 0;
	for(int i=0 ; i<length ; i++)
	{
		if(CSTARGET->GetAt(i) == sep || ( (i==length1)&&(ind==0) ))
		{
			if((i==length-1)&&(ind==0))
				poz[ind] = i+1;
			else
				poz[ind] = i;
			ind ++;
		}
		if(ind == index + 1)
			i=length;
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
BOOL GETITEMSFROMCS(CString* CSTARGET,CStringList* CSITEM,char sep)
{
	int length = CSTARGET->GetLength();
	int poz[256];
	int ind = 0;
	POSITION pos;
	for(int i=0 ; i<length ; i++)
	{
		if(*((LPCSTR)*CSTARGET+i) == sep)
		{
			poz[ind] = i;
			ind ++;
		}	
	}
	for(int index = 0;index <= ind ; index++)
	{
		if(index == 0)
		{
			if(ind == 0)
				pos = CSITEM->AddHead(CSTARGET->Mid(0,length));
			else
				pos = CSITEM->AddHead(CSTARGET->Mid(0,poz[0]));
		}
		else if(index > 0 && index <ind)
			pos = CSITEM->InsertAfter(pos,CSTARGET->Mid(poz[index-1]+1,poz[index]-poz[index-1]-1));
		else if(index == ind)
			pos = CSITEM->InsertAfter(pos,CSTARGET->Mid(poz[index-1]+1,length-poz[index-1]-1));
	}
	return TRUE;
}

BOOL quickGETITEMFROMCS(int index,CString* CSTARGET,CString* CSITEM,char sep)
{
	if(index == 0)
	{
		quickInited = true;
		length = CSTARGET->GetLength();
		length1 = length - 1;
		ind = 0;
		for(int i=0 ; i<length ; i++)
		{
			if(CSTARGET->GetAt(i) == sep || ( (i==length1)&&(ind==0) ))
			{
				if((i==length-1)&&(ind==0))
					poz[ind] = i+1;
				else
					poz[ind] = i;
				ind ++;
			}
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
		quickInited = false;
		*CSITEM = CSTARGET->Mid(poz[index-1]+1,length-poz[index-1]-1);
		return TRUE;
	}
	return FALSE;
}

BOOL wideGETITEMFROMCS(int index,char* source,char* dest,char* sep)
{
	int len = strlen(source);
	int laststart = 0;
	int start = 0;
	int end = 0;
	int sepcnt = 0;
	bool found = false;
	bool eol = false;
	for(int i=0;i<len;i++)
	{
		if((*(source+i) == *(sep) && *(source+i+1) == *(sep+1)))
		{
			if(index == sepcnt)
			{
				start = laststart;
				end = i-1;
				i = len;
				found = true;
			}
			else
			{
				laststart = i+2;
				sepcnt++;
			}
		}
		else if(i == len-1)
		{
			start = laststart;
			end = i;
			found = true;
			eol= true;
		}
	}
	if(found)
	{
		strncpy(dest,source+start,end-start+1);
		if(eol)
			return false;
		return true;
	}else
		return false;
}