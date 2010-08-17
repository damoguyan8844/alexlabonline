#include "StdAfx.h"
#include "avifile.h"

/// FPS defines the Number of Frames per second;
/// Here we set FPS to 1 meaning one Frame per Second.
/// Increase it it you need more Frames Per Second.
#define FPS			1	// Frames Per Second

/// VIDEOCODEC defines the Codec that need to be used for the Movie.
/// Change it to mmioFOURCC('D','I','V','X') to use DIVX codec;
/// Change it to 0 to avoid codecs - Frames would be saved as they are;
/// But make sure you have the codec installed on the machine before using it.
#define VIDEOCODEC	mmioFOURCC('M','P','G','4')

//#define VIDEOCODEC	0	/// Frames will be saved as they are - movie size would be HUGE !!

CAviFile::CAviFile(LPCTSTR lpszFileName)
{

	AVIFileInit();

	_tcscpy(m_szFileName,lpszFileName);

	m_hHeap=NULL;
	m_hAviDC=NULL;
	m_lpBits=NULL;
	m_lSample=NULL;
	m_pAviFile=NULL;
	m_pAviStream=NULL;
	m_pAviCompressedStream=NULL;

	pAppendFrame[0]= &CAviFile::AppendDummy;			// VC8 requires & for Function Pointer; Remove it if your compiler complains;
	pAppendFrame[1]= &CAviFile::AppendFrameFirstTime;
	pAppendFrame[2]= &CAviFile::AppendFrameUsual;

	pAppendFrameBits[0]=&CAviFile::AppendDummy;
	pAppendFrameBits[1]=&CAviFile::AppendFrameFirstTime;
	pAppendFrameBits[2]=&CAviFile::AppendFrameUsual;

	nAppendFuncSelector=1;		//0=Dummy	1=FirstTime	2=Usual
}

CAviFile::~CAviFile(void)
{
	ReleaseMemory();

	AVIFileExit();
}

void CAviFile::ReleaseMemory()
{
	nAppendFuncSelector=0;		//Point to DummyFunction

	if(m_hAviDC)
	{
		DeleteDC(m_hAviDC);
		m_hAviDC=NULL;
	}
	if(m_pAviCompressedStream)
	{
		AVIStreamRelease(m_pAviCompressedStream);
		m_pAviCompressedStream=NULL;
	}
	if(m_pAviStream)
	{
		AVIStreamRelease(m_pAviStream);
		m_pAviStream=NULL;
	}
	if(m_pAviFile)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile=NULL;
	}
	if(m_lpBits)
	{
		HeapFree(m_hHeap,HEAP_NO_SERIALIZE,m_lpBits);
		m_lpBits=NULL;
	}
	if(m_hHeap)
	{
		HeapDestroy(m_hHeap);
		m_hHeap=NULL;
	}
}

HRESULT	CAviFile::AppendFrameFirstTime(HBITMAP	hBitmap)
{
	int	nMaxWidth=GetSystemMetrics(SM_CXSCREEN),nMaxHeight=GetSystemMetrics(SM_CYSCREEN);

	BITMAPINFO	bmpInfo;

	m_hAviDC=CreateCompatibleDC(NULL);
	if(m_hAviDC==NULL)	
	{
		//MessageBox(NULL,"Unable to Create Compatible DC","Error",MB_OK|MB_ICONERROR);
		goto TerminateInit;
	}

	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	
	GetDIBits(m_hAviDC,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

	bmpInfo.bmiHeader.biCompression=BI_RGB;
	
	if(bmpInfo.bmiHeader.biHeight>nMaxHeight)	nMaxHeight=bmpInfo.bmiHeader.biHeight;
	if(bmpInfo.bmiHeader.biWidth>nMaxWidth)	nMaxWidth=bmpInfo.bmiHeader.biWidth;

	m_hHeap=HeapCreate(HEAP_NO_SERIALIZE,nMaxWidth*nMaxHeight*4,0);
	if(m_hHeap==NULL)
	{
		//MessageBox(NULL,"Unable to Allocate Memory","Error",MB_OK);
		goto TerminateInit;
	}
	
	m_lpBits=HeapAlloc(m_hHeap,HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE,nMaxWidth*nMaxHeight*4);
	if(m_lpBits==NULL)	
	{	
		//MessageBox(NULL,"Unable to Allocate Memory","Error",MB_OK);
		goto TerminateInit;
	}

	if(FAILED(AVIFileOpen(&m_pAviFile,m_szFileName,OF_CREATE|OF_WRITE,NULL)))
	{
		//MessageBox(NULL,"Unable to Create the Movie File","Error",MB_OK|MB_ICONERROR);
		goto TerminateInit;
	}

	ZeroMemory(&m_AviStreamInfo,sizeof(AVISTREAMINFO));
	m_AviStreamInfo.fccType=streamtypeVIDEO;
	m_AviStreamInfo.fccHandler=VIDEOCODEC;
	m_AviStreamInfo.dwScale=1;
	m_AviStreamInfo.dwRate=FPS;		//Frames Per Second;
	m_AviStreamInfo.dwQuality=-1;	//Default Quality
	m_AviStreamInfo.dwSuggestedBufferSize=nMaxWidth*nMaxHeight*4;
    SetRect(&m_AviStreamInfo.rcFrame,0,0,bmpInfo.bmiHeader.biWidth,bmpInfo.bmiHeader.biHeight);
	strcpy(m_AviStreamInfo.szName,"Video Stream");

	if(FAILED(AVIFileCreateStream(m_pAviFile,&m_pAviStream,&m_AviStreamInfo)))
	{
		//MessageBox(NULL,"Unable to Create Stream","Error",MB_OK|MB_ICONERROR);
		goto TerminateInit;

	}

	ZeroMemory(&m_AviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	m_AviCompressOptions.fccType=streamtypeVIDEO;
	m_AviCompressOptions.fccHandler=m_AviStreamInfo.fccHandler;
	m_AviCompressOptions.dwFlags=AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	m_AviCompressOptions.dwKeyFrameEvery=15;
	//m_AviCompressOptions.dwBytesPerSecond=1000/8;
	//m_AviCompressOptions.dwQuality=100;

	if(FAILED(AVIMakeCompressedStream(&m_pAviCompressedStream,m_pAviStream,&m_AviCompressOptions,NULL)))
	{
		//MessageBox(NULL,"Unable to Create Compressed Stream","Error",MB_OK);
		goto TerminateInit;
	}

	if(FAILED(AVIStreamSetFormat(m_pAviCompressedStream,0,(LPVOID)&bmpInfo,bmpInfo.bmiHeader.biSize)))
	{
		//MessageBox(NULL,"Unable to Set Format","Error",MB_OK);
		goto TerminateInit;
	}

	nAppendFuncSelector=2;		//Point to UsualAppend Function

	return AppendFrameUsual(hBitmap);

TerminateInit:

	ReleaseMemory();	MessageBox(NULL,"Error Occured While Rendering the Movie","Error",MB_OK|MB_ICONERROR);

	return E_FAIL;
}

HRESULT CAviFile::AppendFrameUsual(HBITMAP hBitmap)
{
	BITMAPINFO	bmpInfo;

	bmpInfo.bmiHeader.biBitCount=0;
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	
	GetDIBits(m_hAviDC,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

	bmpInfo.bmiHeader.biCompression=BI_RGB;	

	GetDIBits(m_hAviDC,hBitmap,0,bmpInfo.bmiHeader.biHeight,m_lpBits,&bmpInfo,DIB_RGB_COLORS);

	if(FAILED(AVIStreamWrite(m_pAviCompressedStream,m_lSample++,1,m_lpBits,bmpInfo.bmiHeader.biSizeImage,0,NULL,NULL)))
		return E_FAIL;

    return S_OK;
}

HRESULT CAviFile::AppendDummy(HBITMAP)
{
	return E_FAIL;
}

HRESULT CAviFile::AppendNewFrame(HBITMAP hBitmap)
{
	return (this->*pAppendFrame[nAppendFuncSelector])((HBITMAP)hBitmap);
}

HRESULT	CAviFile::AppendNewFrame(int nWidth, int nHeight, LPVOID pBits,int nBitsPerPixel)
{
	return (this->*pAppendFrameBits[nAppendFuncSelector])(nWidth,nHeight,pBits,nBitsPerPixel);
}

HRESULT	CAviFile::AppendFrameFirstTime(int nWidth, int nHeight, LPVOID pBits,int nBitsPerPixel)
{
	int	nMaxWidth=GetSystemMetrics(SM_CXSCREEN),nMaxHeight=GetSystemMetrics(SM_CYSCREEN);

	BITMAPINFO	bmpInfo;

	m_hAviDC=CreateCompatibleDC(NULL);
	if(m_hAviDC==NULL)	
	{
		//MessageBox(NULL,"Unable to Create Compatible DC","Error",MB_OK|MB_ICONERROR);
		goto TerminateInitBits;
	}

	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biPlanes=1;
	bmpInfo.bmiHeader.biWidth=nWidth;
	bmpInfo.bmiHeader.biHeight=nHeight;
	bmpInfo.bmiHeader.biCompression=BI_RGB;
	bmpInfo.bmiHeader.biBitCount=nBitsPerPixel;
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biHeight*bmpInfo.bmiHeader.biBitCount/8;
	
	if(bmpInfo.bmiHeader.biHeight>nMaxHeight)	nMaxHeight=bmpInfo.bmiHeader.biHeight;
	if(bmpInfo.bmiHeader.biWidth>nMaxWidth)	nMaxWidth=bmpInfo.bmiHeader.biWidth;

	m_hHeap=HeapCreate(HEAP_NO_SERIALIZE,nMaxWidth*nMaxHeight*4,0);
	if(m_hHeap==NULL)
	{
		//MessageBox(NULL,"Unable to Allocate Memory","Error",MB_OK);
		goto TerminateInitBits;
	}
	
	m_lpBits=HeapAlloc(m_hHeap,HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE,nMaxWidth*nMaxHeight*4);
	if(m_lpBits==NULL)	
	{	
		//MessageBox(NULL,"Unable to Allocate Memory","Error",MB_OK);
		goto TerminateInitBits;
	}

	if(FAILED(AVIFileOpen(&m_pAviFile,m_szFileName,OF_CREATE|OF_WRITE,NULL)))
	{
		//MessageBox(NULL,"Unable to Create the Movie File","Error",MB_OK|MB_ICONERROR);
		goto TerminateInitBits;
	}

	ZeroMemory(&m_AviStreamInfo,sizeof(AVISTREAMINFO));
	m_AviStreamInfo.fccType=streamtypeVIDEO;
	m_AviStreamInfo.fccHandler=VIDEOCODEC;
	m_AviStreamInfo.dwScale=1;
	m_AviStreamInfo.dwRate= FPS;	//Frames Per Second;
	m_AviStreamInfo.dwQuality=-1;	//Default Quality
	m_AviStreamInfo.dwSuggestedBufferSize=nMaxWidth*nMaxHeight*4;
    SetRect(&m_AviStreamInfo.rcFrame,0,0,bmpInfo.bmiHeader.biWidth,bmpInfo.bmiHeader.biHeight);
	strcpy(m_AviStreamInfo.szName,"Video Stream");

	if(FAILED(AVIFileCreateStream(m_pAviFile,&m_pAviStream,&m_AviStreamInfo)))
	{
		//MessageBox(NULL,"Unable to Create Stream","Error",MB_OK|MB_ICONERROR);
		goto TerminateInitBits;

	}

	ZeroMemory(&m_AviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	m_AviCompressOptions.fccType=streamtypeVIDEO;
	m_AviCompressOptions.fccHandler=m_AviStreamInfo.fccHandler;
	m_AviCompressOptions.dwFlags=AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	m_AviCompressOptions.dwKeyFrameEvery=15;
	//m_AviCompressOptions.dwBytesPerSecond=1000/8;
	//m_AviCompressOptions.dwQuality=100;

	if(FAILED(AVIMakeCompressedStream(&m_pAviCompressedStream,m_pAviStream,&m_AviCompressOptions,NULL)))
	{
		//MessageBox(NULL,"Unable to Create Compressed Stream","Error",MB_OK);
		goto TerminateInitBits;
	}

	if(FAILED(AVIStreamSetFormat(m_pAviCompressedStream,0,(LPVOID)&bmpInfo,bmpInfo.bmiHeader.biSize)))
	{
		//MessageBox(NULL,"Unable to Set Format","Error",MB_OK);
		goto TerminateInitBits;
	}

	nAppendFuncSelector=2;		//Point to UsualAppend Function

	return AppendFrameUsual(nWidth,nHeight,pBits,nBitsPerPixel);

TerminateInitBits:

	ReleaseMemory();	MessageBox(NULL,"Error Occured While Rendering the Movie","Error",MB_OK|MB_ICONERROR);

	return E_FAIL;

}

HRESULT	CAviFile::AppendFrameUsual(int nWidth, int nHeight, LPVOID pBits,int nBitsPerPixel)
{
	DWORD	dwSize=nWidth*nHeight*nBitsPerPixel/8;

	if(FAILED(AVIStreamWrite(m_pAviCompressedStream,m_lSample++,1,pBits,dwSize,0,NULL,NULL)))
		return E_FAIL;

    return S_OK;
}

HRESULT	CAviFile::AppendDummy(int nWidth, int nHeight, LPVOID pBits,int nBitsPerPixel)
{
	return E_FAIL;
}
