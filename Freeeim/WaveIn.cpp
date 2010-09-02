// WaveIn.cpp : implementation file
//

#include "stdafx.h"
#include "EIM02.h"
#include "WaveIn.h"
#include "EIM02Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveIn

//IMPLEMENT_DYNCREATE(CWaveIn, CWinThread)

CWaveIn::CWaveIn()
{
}

CWaveIn::CWaveIn(SOCKET sock)
{
	CreateThread();
	m_sock = sock;
}

CWaveIn::~CWaveIn()
{
/*	if (m_buf1)
	{
		delete [] m_buf1;
		m_buf1 = NULL;
	}
	if (m_buf2)
	{
		delete [] m_buf2;
		m_buf2 = NULL;
	}*/
}

BOOL CWaveIn::InitInstance()
{
	m_dwThread = ::GetCurrentThreadId();
	Start(m_sock);
	return TRUE;
}

int CWaveIn::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWaveIn, CWinThread)
	//{{AFX_MSG_MAP(CWaveIn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_THREAD_MESSAGE(MM_WIM_OPEN, On_WIM_OPEN)
	ON_THREAD_MESSAGE(MM_WIM_DATA, On_WIM_DATA)
	ON_THREAD_MESSAGE(MM_WIM_CLOSE, On_WIM_CLOSE)
	ON_THREAD_MESSAGE(WM_WAVEINEND, OnWaveInEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveIn message handlers

void CWaveIn::On_WIM_OPEN(UINT wParam, LONG lParam)
{
	m_bRecording = TRUE;

	//******************************************************
	if(m_sock)
	{
		char buf[5];
		DWORD ret = recv(m_sock, buf, 5, 0);
		send(m_sock, (char*)&m_wfx, sizeof(WAVEFORMATEX), 0);
	}
	//******************************************************
}

static int t = 0;
void CWaveIn::On_WIM_DATA(UINT wParam, LONG lParam)
{
	LPWAVEHDR ccpwh = (LPWAVEHDR)lParam;

	//******************************************************
	if (m_sock)
	{
		DWORD len = send(m_sock, ccpwh->lpData, ccpwh->dwBytesRecorded, 0);
		if (len == SOCKET_ERROR)
		{
		//	m_bRecording = FALSE;
			PostThreadMessage(WM_WAVEINEND, 0, 0);
		//	AfxEndThread(1,TRUE);
		//	AfxMessageBox("thread end.");
		//	StopRecord();
		//	return;
		}
	}

	//******************************************************
	if (m_bRecording)
	{
		m_ret = ::waveInAddBuffer(m_hwi, ccpwh, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != m_ret)
		{
			AfxMessageBox("add buf failed.");
		}
	}
}

void CWaveIn::On_WIM_CLOSE(UINT wParam, LONG lParam)
{
	//******************************************************
	if (m_sock)
		closesocket(m_sock);
	//******************************************************

	if (m_buf1)
	{delete [] m_buf1; m_buf1 = NULL;}

	if (m_buf2)
	{delete [] m_buf2;m_buf2 = NULL;}
}

BOOL CWaveIn::StopRecord()
{
	m_bRecording = FALSE;

	m_ret = ::waveInStop(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "waveInStop ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		}
		AfxMessageBox(err);
	}

	m_ret = ::waveInReset(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::StopRecord waveInReset ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInUnprepareHeader(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInUnprepareHeader ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "The buffer pointed to by the pwh parameter is still in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInUnprepareHeader(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInUnprepareHeader ";;
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "The buffer pointed to by the pwh parameter is still in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInClose(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInClose ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "There are still buffers in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CWaveIn::Start(SOCKET s)
{
//	AfxMessageBox("T running");
	// TODO:  perform and per-thread initialization here
	// 具有 CD 音质的录音效果
	CString err = "error";
/* Initialize the WAVEFORMATEX for 16-bit, 44KHz, stereo */
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 2;
	m_wfx.nSamplesPerSec = 44100;
	m_wfx.wBitsPerSample = 16;
	m_wfx.nBlockAlign = m_wfx.nChannels * (m_wfx.wBitsPerSample/8);
	m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;
	m_wfx.cbSize = 0;

/*	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 1;
	m_wfx.nSamplesPerSec = 8000;
	m_wfx.wBitsPerSample = 8;
	m_wfx.nBlockAlign = m_wfx.nChannels * (m_wfx.wBitsPerSample/8);
	m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;
	m_wfx.cbSize = 0;*/

//	err.Format("%d", m_wfx.nAvgBytesPerSec);
//	AfxMessageBox(err);

/*	m_wfx.cbSize = 0;
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 1;
	m_wfx.wBitsPerSample = 8;
	m_wfx.nAvgBytesPerSec = 11025;
	m_wfx.nSamplesPerSec = 11025;
	m_wfx.nBlockAlign = 1;
	*/
	m_ret = ::waveInOpen(&m_hwi, WAVE_MAPPER, &m_wfx, m_dwThread, NULL, CALLBACK_THREAD);
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInOpen";
		AfxMessageBox(err);
		return FALSE;
	}

	m_buf1 = new char[BUF_LEN];
	m_buf2 = new char[BUF_LEN];

	ZeroMemory(&m_wh1, sizeof(WAVEHDR));
	ZeroMemory(&m_wh2, sizeof(WAVEHDR));

	m_wh1.lpData = m_buf1;
	m_wh1.dwBufferLength = BUF_LEN;
	m_wh1.dwLoops = 1;

	m_ret = ::waveInPrepareHeader(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInPrepareHeader";
		AfxMessageBox(err);
		return FALSE;
	}

	m_wh2.lpData = m_buf2;
	m_wh2.dwBufferLength = BUF_LEN;
	m_wh2.dwLoops = 1;
	m_ret = ::waveInPrepareHeader(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInPrepareHeader";
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInAddBuffer(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "waveInAddBuffer";
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInAddBuffer(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "waveInAddBuffer";
		AfxMessageBox(err);
		return FALSE;
	}

	waveInStart(m_hwi);

	return TRUE;
}

void CWaveIn::OnWaveInEnd(WPARAM wParam, LPARAM lParam)
{
//	StopRecord();
//	AfxEndThread(0);
	ExitThread(1);
}