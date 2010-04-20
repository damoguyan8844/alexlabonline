// This is server which exposes the outer component and a inner component.
// The outer component acts as a client and uses the services (exposed by interfaces)
// provided by an inner components.
// This is the Outer Component Implementation.

// Code By Dinesh Ahuja, October 11,2003.


#include "CMath.h"
#include <iostream.h>

static HMODULE ghModule = NULL;

/* Count to maintains the number of components being serverd
** by the COM Server (ContainmentSample.dll).
*/
extern long gNoComponents = 0;

/* Count which will keep the server loaded in the memory, even when
** it is not serving any component.
*/
static long gNoLocks = 0;

const char gszMathName[] = "COM,Math Class";
const char gszMathIndProgID[] = "COMSample.CMath";
const char gszMathProgID[] = "COMSample.CMath.1";

const char gszSquareName[] = "COM,Square Class";
const char gszSquareIndProgID[] = "COMSample.CSquare";
const char gszSquareProgID[] = "COMSample.CSquare.1";


//Outer Component ClassID : {CD082341-FC1E-11d7-B532-30B04EC10000}
static const CLSID CLSID_CMath = 
{ 0xcd082341, 0xfc1e, 0x11d7, { 0xb5, 0x32, 0x30, 0xb0, 0x4e, 0xc1, 0x0, 0x0 } };

// Inner Component ClassID : {CD082342-FC1E-11d7-B532-30B04EC10000}
static const CLSID CLSID_CSquare = 
{ 0xcd082342, 0xfc1e, 0x11d7, { 0xb5, 0x32, 0x30, 0xb0, 0x4e, 0xc1, 0x0, 0x0 } };


/*******************************************************************/
// CMath's methods Implementation.

CMath::CMath() {
	this->m_cRef = 1;
	this->m_pISquare = NULL;
	InterlockedIncrement(&gNoComponents);
	cout<<"CMath::Constructor called"<<endl;
}

CMath::~CMath() {
	InterlockedDecrement(&gNoComponents);

	// If m_pISquare is valid.
	if(m_pISquare!=NULL) {
		m_pISquare->Release();
		cout<<"Inner Component Released"<<endl;

	}

	cout<<"CMath::Destructor called"<<endl;
}

void _STDCALL CMath::SumSquare(int Val1,int Val2,int* pResult) {
	
	int sqVal1,sqVal2;
	
	if(m_pISquare!=NULL) {

		// Calling the "Square" method of ISquare interface on the inner component.
		this->m_pISquare->Square(Val1,&sqVal1);
		this->m_pISquare->Square(Val2,&sqVal2);
	}

	*pResult = sqVal1 + sqVal2;
}


ULONG _STDCALL CMath::AddRef() {

	return InterlockedIncrement(&this->m_cRef);
}

ULONG _STDCALL CMath::Release() {

	if (InterlockedDecrement(&this->m_cRef) == 0) {
		delete this;
		return 0;
	}

	return this->m_cRef; 
}

HRESULT _STDCALL CMath::QueryInterface(const IID& iid, void **ppv) {

	// CMath supports only two interfaces IUnknown and IMath interface.
	if (iid == IID_IUnknown) {
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid  == IID_IMath) {
		*ppv = static_cast<IMath*>(this);
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
	cout<<"CFactory::Constructor Called"<<endl;
}

CFactory::~CFactory () {
	cout<<"CFactory::Destructor Called"<<endl;
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

	if (pUnkOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}
	
	// This is executed when the client calls the CreateInstance,
	// after getting the IClassFactory interface pointer on CMath's 
	// instance.The client gets the IClassFactory interface pointer
	// by calling CoGetClassObject.
	if ((iid == IID_IMath) || (iid == IID_IUnknown)) {
		
		CMath* pMath = new CMath();

		if(pMath ==  NULL) {
			return E_OUTOFMEMORY;
		}
 
		// Here, the Outer Component intializes the inner component.
		cout<<"Call to Create the Inner Component"<<endl;
		hResult = CoCreateInstance(CLSID_CSquare,NULL,CLSCTX_INPROC_SERVER,IID_ISquare,(void**)&pMath->m_pISquare);
		cout<<"CoCreateInstance for CSquare has been called"<<endl;


		if(FAILED(hResult)) {
			// This gives a string description for the error.
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
				hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
			cout<<(LPTSTR)pMsgBuf<<endl;
			LocalFree((HLOCAL)pMsgBuf);
			return E_FAIL;
		}
	
		hResult = pMath->QueryInterface(iid,ppv);
		if(SUCCEEDED(hResult)) {
			pMath->Release();
		}
	}
	else if (iid == IID_ISquare) {
		// This part is executed when the outer component calls
		// CoCreateInstance with the clsid as CLSID_CSquare and query for
		// an interface pointer on CSquare.

		cout<<"Creation of Inner Component in the CFactory::CreateInstance"<<endl;
		CSquare* pSquare = new CSquare();

		if(pSquare ==  NULL) {
			return E_OUTOFMEMORY;
		}
 	
		hResult = pSquare->QueryInterface(iid,ppv);
		if(SUCCEEDED(hResult)) {
			pSquare->Release();
		}
		else if (FAILED(hResult)) {
				// This gives a string description for the error.
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
				hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
			cout<<(LPTSTR)pMsgBuf<<endl;
			LocalFree((HLOCAL)pMsgBuf);
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
				TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}"),
				REG_SZ, gszMathName,strlen(gszMathName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\InProcServer32"),
				REG_SZ, szFileName, strlen(szFileName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\ProgID"),
				REG_SZ, gszMathProgID, strlen(gszMathProgID));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\VersionIndependentProgID"),
				REG_SZ, gszMathIndProgID, strlen(gszMathIndProgID));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}"),
				REG_SZ, gszSquareName,strlen(gszSquareName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\InProcServer32"),
				REG_SZ, szFileName, strlen(szFileName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\ProgID"),
				REG_SZ, gszSquareProgID, strlen(gszSquareProgID));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\VersionIndependentProgID"),
				REG_SZ, gszSquareIndProgID, strlen(gszSquareIndProgID));

	return S_OK;
}

STDAPI DllUnregisterServer() {
	
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\VersionIndependentProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}\\ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082341-FC1E-11d7-B532-30B04EC10000}"));

	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\VersionIndependentProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}\\ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{CD082342-FC1E-11d7-B532-30B04EC10000}"));

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

	cout<<"DllCanUnloadNow called"<<endl;
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
	if((clsid == CLSID_CMath) || (clsid == CLSID_CSquare)) {
		cout<<"The requested component is supported by COM server (ContainmentSample.dll)"<<endl;
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

