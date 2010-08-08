/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPMutex
		class	CSPMutex::Scoped
*/

#ifndef	__SP_LOCK_H__
#define	__SP_LOCK_H__

class CSPMutex
{
	HANDLE	m_hObject;
public:
	CSPMutex( )
	{
		m_hObject = ::CreateMutex( NULL, FALSE, NULL);
	}
	~CSPMutex ()
	{
		if (m_hObject != NULL)
		{
			::CloseHandle(m_hObject);
			m_hObject = NULL;
		}
	}
	BOOL Lock()
	{
		if (::WaitForSingleObject(m_hObject, INFINITE) == WAIT_OBJECT_0)
			return TRUE;
		return FALSE;
	}

	BOOL UnLock()
	{
		return ::ReleaseMutex(m_hObject);
	}

	class Scoped
	{
		CSPMutex *mx;
	public:
		Scoped(CSPMutex& m) : mx(&m) { mx->Lock(); }
		~Scoped () { mx->UnLock(); }
	};
};

#endif //__SP_LOCK_H__
