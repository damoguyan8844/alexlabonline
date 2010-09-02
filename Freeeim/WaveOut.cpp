// WaveOut.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "WaveOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveOut

char CWaveOut::m_szip[256] = {0};

IMPLEMENT_DYNCREATE(CWaveOut, CWinThread)

CWaveOut::CWaveOut()
{
	m_bChatting = FALSE;
}

CWaveOut::~CWaveOut()
{
}

BOOL CWaveOut::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	m_dwTID = ::GetCurrentThreadId();
	m_bChatting = FALSE;

//	if (! Connect("127.0.0.1"))
	if (! Connect(m_szip))
		return FALSE;
	if (! SendRequest())
		return FALSE;
	if (! GetWavePCM())
		return FALSE;

/*	if (! OpenFile("record3.wav"))
		return FALSE;
*/
	if (! Start())
		return FALSE;

	return TRUE;
}

int CWaveOut::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWaveOut, CWinThread)
	//{{AFX_MSG_MAP(CWaveOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_THREAD_MESSAGE(MM_WOM_OPEN, On_MM_Open)
	ON_THREAD_MESSAGE(MM_WOM_DONE, On_MM_Done)
	ON_THREAD_MESSAGE(MM_WOM_CLOSE, On_MM_Close)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveOut message handlers
/*
BOOL CWaveOut::OpenFile(LPSTR name)
{
	MMCKINFO	pinfo;
	MMCKINFO	cinfo;
	char *hData = NULL;

	//打开WAV文件，返回一个m_hmmio句柄
	m_hmmio = mmioOpen(name, NULL, MMIO_READ);
	if (!m_hmmio)
	{
		AfxMessageBox("Wave file opening error.");
		return FALSE;
	}

	//查找父块"wave";
	pinfo.fccType=mmioFOURCC('W','A','V','E');

	if ( mmioDescend(m_hmmio, &pinfo, NULL, MMIO_FINDRIFF) )
	{
		AfxMessageBox("mmioFOURCC failed.");
		return FALSE;
	}

	//查找子块"fmt"  parent"riff";
	cinfo.ckid=mmioFOURCC('f','m','t',' ');
	if(mmioDescend(m_hmmio,&cinfo,&pinfo,MMIO_FINDCHUNK))
	{
		AfxMessageBox("mmioDescend failed.");
		return FALSE;
	}

	mmioRead(m_hmmio,(LPSTR)&m_pcm,sizeof(PCMWAVEFORMAT));//cinfo.cksize);

	if (m_pcm.wf.wFormatTag != WAVE_FORMAT_PCM)
	{
		AfxMessageBox("mmioDescend failed.");
		return FALSE;
	}

	//跳入块"FMT"
	mmioAscend(m_hmmio,&cinfo,0);

	//查找数据块
	cinfo.ckid=mmioFOURCC('d','a','t','a');

	if (mmioDescend(m_hmmio,&cinfo,&pinfo,MMIO_FINDCHUNK))
	{
		AfxMessageBox("mmioDescend failed.");
		return FALSE;
	}

	m_dwSize =cinfo.cksize;

	return TRUE;
}
*/
BOOL CWaveOut::Start()
{
	if (waveOutOpen(&m_hwo, WAVE_MAPPER,
		(LPWAVEFORMATEX)&m_pcm,
		m_dwTID, 0, CALLBACK_THREAD))
	{
		AfxMessageBox("waveOutOpen");
		return FALSE;
	}

	return TRUE;
}


void CWaveOut::On_MM_Done(UINT wParam, LONG lParam)
{
	LPWAVEHDR hdr = (LPWAVEHDR)lParam;

	delete [] hdr->lpData;
//	delete hdr;

//	AfxMessageBox("ttt");
//	if (m_curSize < m_dwSize)
	ReadAndPlay(hdr);

	if (! m_bChatting)
		waveOutClose(m_hwo);

}

void CWaveOut::On_MM_Open(UINT wParam, LONG lParam)
{
	m_bChatting = TRUE;
	ZeroMemory(&m_hdr1, sizeof(WAVEHDR));
	ReadAndPlay(&m_hdr1);

	ZeroMemory(&m_hdr2, sizeof(WAVEHDR));
	ReadAndPlay(&m_hdr2);
}

void CWaveOut::On_MM_Close(UINT wParam, LONG lParam)
{
//	AfxMessageBox("On_MM_Close");
//	mmioClose(m_hmmio, 0);
	closesocket(m_sock);
	Sleep(500); 

}
/*
BOOL CWaveOut::ReadAndPlay()
{
	char *bbf = new char[BUF_LEN];
	DWORD slen = recv(m_sock, bbf, BUF_LEN, 0);
	if (slen == SOCKET_ERROR)
	{
		AfxMessageBox("Teriable error.");
		return FALSE;
	}
/*	else
	{
		AfxMessageBox(bbf);
	}*/
//	Sleep(10);
/*	LPWAVEHDR hdr = new WAVEHDR;
	char *buf = new char[BUF_LEN];*/

//	ZeroMemory(hdr, sizeof(WAVEHDR));

/*	DWORD total = 0;
	DWORD len = 0;*/
//	len = mmioRead(m_hmmio, buf, BUF_LEN);
/*	m_curSize += len;

	hdr->lpData = bbf;//buf;
	hdr->dwBufferLength = slen;//len;

	if (waveOutPrepareHeader(m_hwo, hdr, sizeof(WAVEHDR)))
	{
		AfxMessageBox("waveOutPrepareHeader failed.");
		return FALSE;
	}
	if (waveOutWrite(m_hwo, hdr, sizeof(WAVEHDR)))
	{
		AfxMessageBox("waveOutWrite failed.");
		return FALSE;
	}

	return TRUE;*/
//}

BOOL CWaveOut::ReadAndPlay(LPWAVEHDR hdr)
{
	char *bbf = new char[BUF_LEN];
	DWORD slen = recv(m_sock, bbf, BUF_LEN, 0);
	if (slen == SOCKET_ERROR)
	{
	//	AfxMessageBox("Teriable error.");
		m_bChatting = FALSE;
		return FALSE;
	}

	hdr->lpData = bbf;//buf;
	hdr->dwBufferLength = slen;//len;

	if (waveOutPrepareHeader(m_hwo, hdr, sizeof(WAVEHDR)))
	{
		AfxMessageBox("waveOutPrepareHeader failed.");
		return FALSE;
	}

	if (waveOutWrite(m_hwo, hdr, sizeof(WAVEHDR)))
	{
		AfxMessageBox("waveOutWrite failed.");
		return FALSE;
	}

	return TRUE;
}

BOOL CWaveOut::Connect(LPSTR ip)
{
	if(NULL == ip)
	{
		AfxMessageBox("CWaveSocket::Connect:ip == NULL.");
		return FALSE;
	}

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
	{
		AfxMessageBox("CWaveSocket::Connect [socket] failed.");
		return FALSE;
	}

	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(VOICE_RECV_PORT);

	DWORD ret = connect(m_sock, (PSOCKADDR)&sin, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		AfxMessageBox("CWaveSocket::Connect [connect] failed.");
		return FALSE;
	}

	return TRUE;
}


BOOL CWaveOut::SendRequest()
{
	int ret;
	ret = send(m_sock, "8888", 20, 0);
	if (SOCKET_ERROR == ret)
	{
		AfxMessageBox("Send failed.");
	}

	return TRUE;
}


BOOL CWaveOut::GetWavePCM()
{
	int ret;
	ret = recv(m_sock, (char*)&m_pcm, sizeof(WAVEFORMATEX), 0);
	if (sizeof(WAVEFORMATEX) != ret)
	{
		AfxMessageBox("Get PCM failed.");
		return FALSE;
	}

	return TRUE;
}

BOOL CWaveOut::SetIP(LPCTSTR ip)
{
	strcpy(m_szip, ip);

	return TRUE;
}

BOOL CWaveOut::Stop()
{
	shutdown(m_sock, SD_BOTH);
	closesocket(m_sock);

//	AfxEndThread(1);
//	AfxMessageBox("aaaaaaa");
	return TRUE;
}
