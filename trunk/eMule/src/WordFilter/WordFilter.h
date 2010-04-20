#include <tchar.h>

#ifndef WORDFILTER_H
#define WORDFILTER_H

/////////////////////////////////////////////////////////////////////////////
//
// WordFilter.h : interface of the CWordFilter, used to filter limited words
//                Added by Soar Chin
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FLITER_FILE _T("wordfilter.txt")

class CWordFilter
{
private:
	int		m_count;			// Count of limited words
	TCHAR **	m_filterwords;	// Limited words for filter
	int **	m_kmpvalue;			// KMP Next Value
	bool	m_filterall;
	void Free();

	__inline bool	KMP_Match(const CString & sString, TCHAR * sSubString, int * iNext);
	__inline void	KMP_GetNext(TCHAR * sString, int * iNext);

public:
	CWordFilter():
	  m_count(0), m_filterwords(NULL), m_kmpvalue(NULL), m_filterall(false) {pProcessThread = NULL;dwLastUpdateWordFilter = 0;nLoadCount = 0;} // Init values
	  ~CWordFilter();
	  void	Init();				// Init class and read from data file
	  bool	VerifyString(const CString & sString);	// Verify if the string has limited words

public:
	void UpdateWordFilter();
	void StopUpdate();
	static UINT AFX_CDECL fnDownloadWordFilte(LPVOID pThis);

	CWinThread* pProcessThread;	
	DWORD dwLastUpdateWordFilter;
	int nLoadCount;
};

extern CWordFilter WordFilter;

#endif // WORDFILTER_H
