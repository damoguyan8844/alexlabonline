// This is server AggregationSample.dll which exposes the outer component.
// The outer component supports two interface and out which it implements
// ITrignometry and aggregates the interface IAddSu, which is implemented
// by CBasic component (implemented in Server called AggregableObject.dll) 

// This is the Outer Component Implementation.

// Code By Dinesh Ahuja, October 25,2003.

#include "CScientific.h"
#include "CBasic.h"
#include <iostream.h>
#include <math.h>

static HMODULE ghModule = NULL;

/* Count to maintains the number of components being serverd
** by the COM Server (ContainmentSample.dll).
*/
extern long gNoComponents = 0;

/* Count which will keep the server loaded in the memory, even when
** it is not serving any component.
*/
static long gNoLocks = 0;

// Entries meant for Registering the Outer Component.
const char gszScientifcName[] = "COM,Scientifc Class";
const char gszScientifcIndProgID[] = "COMSample.CScientifc";
const char gszScientifcProgID[] = "COMSample.CScientifc.1";


// Outer Component's CLSID : {E247F701-0743-11d8-B532-30A64EC10000}
static const CLSID CLSID_CScientific = 
{ 0xe247f701, 0x743, 0x11d8, { 0xb5, 0x32, 0x30, 0xa6, 0x4e, 0xc1, 0x0, 0x0 } };

// Inner Component's CLSID : {E247F702-0743-11d8-B532-30A64EC10000}
static const CLSID CLSID_CBasic = 
{ 0xe247f702, 0x743, 0x11d8, { 0xb5, 0x32, 0x30, 0xa6, 0x4e, 0xc1, 0x0, 0x0 } };


/*******************************************************************/
// CScientific's methods Implementation.

CScientific::CScientific() {
	this->m_cRef = 1;
	this->m_pInnerUnknown = NULL;
	InterlockedIncrement(&gNoComponents);
	cout<<"CScientific::Constructor Called"<<endl;
}

CScientific::~CScientific() {
	InterlockedDecrement(&gNoComponents);
	
	this->m_cRef = 1;
	
	// If m_pInnerUnknown is valid.
	if(this->m_pInnerUnknown !=NULL) {
		this->m_pInnerUnknown->Release();
		cout<<"Calling Release on the Inner's NonDelegate IUnknown pointer"<<endl;
	}

	cout<<"CScientific::Destructor called"<<endl;
}

void _STDCALL CScientific::GetSin(double Val1,double* pResult) {
	
	*pResult = sin(Val1);
}


ULONG _STDCALL CScientific::AddRef() {
	InterlockedIncrement(&this->m_cRef);
	cout<<"CScientific::AddRef Called: "<<this->m_cRef<<endl;
	
	return this->m_cRef; 
}

ULONG _STDCALL CScientific::Release() {

	InterlockedDecrement(&this->m_cRef);
	cout<<"CScientific::Release Called: "<<this->m_cRef<<endl;

	if (this->m_cRef == 0) {
		delete this;
		return 0;
	}
	
	return this->m_cRef; 
}

HRESULT _STDCALL CScientific::QueryInterface(const IID& iid, void **ppv) {

	// CScientific supports only two interfaces ITrignometry and IAddSub interface.
	// CScientific implements the ITrignometry and delegates the IAddSub interface, which
	// has been implemented by the Inner component.
	
	if (iid == IID_IUnknown) {
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid  == IID_ITrignometry) {
		*ppv = static_cast<ITrignometry*>(this);
	}
	else if (iid == IID_IAddSub) {
		// Delegate call to the Inner component's NonDelegating
		// IUnknown.           
		return this->m_pInnerUnknown->QueryInterface(iid,ppv);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	(static_cast<IUnknown*>(*ppv))->AddRef();

	return S_OK;
}


/*******************************************************************/
// ClassFactory Implementation.

class CFactory : public IClassFactory {

public:	
	// Implementing IUnknown Interface.
	virtual HRESULT _STDCALL QueryInterface(const IID& iid,void **ppv);
	virtual ULONG _STDCALL AddRef();
	virtual ULONG _STDCALL Release();

	// Implementing IClassFactory.
	virtual HRESULT _STDCALL CreateInstance(IUnknown * pUnkOuter,
								const IID& iid, void **ppv);
	
	virtual HRESULT _STDCALL LockServer(BOOL bLock);

	CFactory();
	~CFactory();

private:

	long m_cRef;
};

CFactory::CFactory() {
	m_cRef = 1;
	cout<<"CScientific's CFactory::Constructor Called"<<endl;
}

CFactory::~CFactory () {
	cout<<"CScientific's CFactory::Destructor Called"<<endl;
}

HRESULT _STDCALL CFactory::QueryInterface(const IID& iid, void **ppv) {
	
	// It supports only two interfaces IUnknown and IClassFactory.
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory)) {
		*ppv = static_cast<IClassFactory*>(this);	
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppv)->AddRef(); 
	return S_OK;
}

ULONG CFactory::AddRef() {
	
	return InterlockedIncrement(&this->m_cRef);
}

ULONG CFactory::Release() {

	if(InterlockedDecrement(&this->m_cRef) == 0 ) {
		delete this;
		return 0;
	}	

	return this->m_cRef; 
}

HRESULT _STDCALL CFactory::CreateInstance(IUnknown *pUnkOuter,const IID& iid,void **ppv) {
	
	HRESULT hResult;

	// This is meant for storing the error description, corrosponding
	// to HRESULT.
	void * pMsgBuf;

	// This is executed when the client calls the CreateInstance,
	// after getting the IClassFactory interface pointer on class object's
	// instance. The client gets the IClassFactory interface pointer
	// by calling CoGetClassObject.

	if ((iid == IID_ITrignometry) || (iid == IID_IUnknown)) {

		if (pUnkOuter != NULL) {
			return CLASS_E_NOAGGREGATION;
		}
		
		CScientific* pScientific = new CScientific();
		
		if(pScientific ==  NULL) {
			return E_OUTOFMEMORY;
		}
 
		// Here, the Outer Component intializes the inner component.

		IUnknown* pOuterUnknown = static_cast<IUnknown*>(pScientific);
		
		// The outer component created an inner component and queried for
		// inner component's NonDelegate IUnknown pointer.
		hResult = CoCreateInstance(CLSID_CBasic,pOuterUnknown,
								   CLSCTX_INPROC_SERVER,
								   IID_IUnknown,
								   (void**)&pScientific->m_pInnerUnknown);

		cout<<"CScientific::Reference Count After Creating Inner Component: "<<pScientific->m_cRef<<endl;

		if(FAILED(hResult)) {
			// This gives a string description for the error.
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
				hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
			cout<<(LPTSTR)pMsgBuf<<endl;
			LocalFree((HLOCAL)pMsgBuf);
			return E_FAIL;
		}
	
		hResult = pScientific->QueryInterface(iid,ppv);
		if(SUCCEEDED(hResult)) {
			pScientific->Release();
		}
	}
	
	return hResult;
}

HRESULT _STDCALL CFactory::LockServer(BOOL bLock)
{
	if(bLock) {
		InterlockedIncrement(&gNoLocks);
	}
	else {
		InterlockedDecrement(&gNoLocks);
	}

	return S_OK;
}


STDAPI DllRegisterServer() {

	char szFileName[MAX_PATH];
	GetModuleFileName(ghModule, szFileName, MAX_PATH);

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}"),
				REG_SZ, gszScientifcName,strlen(gszScientifcName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}\\InProcServer32"),
				REG_SZ, szFileName, strlen(szFileName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}\\ProgID"),
				REG_SZ, gszScientifcProgID, strlen(gszScientifcProgID));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000\\VersionIndependentProgID"),
				REG_SZ, gszScientifcIndProgID, strlen(gszScientifcIndProgID));

	return S_OK;
}

STDAPI DllUnregisterServer() {
	
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}\\VersionIndependentProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}\\ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F701-0743-11d8-B532-30A64EC10000}"));

	return S_OK;
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD dwReason,void * lpReserved) {
	
	if (dwReason == DLL_PROCESS_ATTACH) {
		cout<<"DllMain function called"<<endl;
		ghModule = (HINSTANCE)hModule;
	}
	
	return TRUE;
}

STDAPI DllCanUnloadNow() {

	cout<<"DllCanUnloadNow called for CScientific"<<endl;
	if((gNoComponents == 0) && (gNoLocks == 0)) {
		
		return S_OK;
	}
	else {

		return S_FALSE;
	}
}

STDAPI	DllGetClassObject(const CLSID & clsid,const IID& iid,void **ppv)
{	
	// This server supports two COM Components and hence validation is
	// performed.
	if(clsid == CLSID_CScientific) {
		cout<<"The requested component is supported by COM server (AggregationSample.dll)"<<endl;
	}
	else
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	
	CFactory *pFactory =  new CFactory();
	
	if (pFactory ==  NULL) {
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pFactory->QueryInterface(iid,ppv);

	static_cast<IUnknown*>(pFactory)->Release(); 
	
	return hResult;
}

