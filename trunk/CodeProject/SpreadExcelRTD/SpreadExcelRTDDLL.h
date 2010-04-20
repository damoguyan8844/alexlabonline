/******************************************************************************
* $Id: SpreadExcelRTDDLL.h 17 2008-02-17 17:12:16Z jim.kleckner $
*
*	File:	SpreadExcelRTDDLL.h
*
*	Date:	February 5, 2001
*
*	Description:	This file contains the class factory and standard DLL 
*					functions for the SpreadExcelRTD COM object.  It is fairly
*					generic for use with other COM objects.
*
*	Modifications:
******************************************************************************/
#include "comdef.h"
#include "initguid.h"

class SpreadExcelRTDClassFactory : public IClassFactory
{
protected:
	ULONG m_refCount;	//reference count

public:
	SpreadExcelRTDClassFactory();
	~SpreadExcelRTDClassFactory();

	/******* IUnknown Methods *******/
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	/******* IClassFactory Methods *******/
	STDMETHODIMP CreateInstance(LPUNKNOWN, REFIID, LPVOID *);
	STDMETHODIMP LockServer(BOOL);
};

LONG g_cLock = 0;	//global count of the locks on this DLL
typedef SpreadExcelRTDClassFactory FAR *LPSpreadExcelRTDClassFactory;

STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR * ppvObj);
STDAPI DllCanUnloadNow();
