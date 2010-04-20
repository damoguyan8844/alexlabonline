// This is server AggregableObject.dll which exposes the inner component.
// The inner component exposes two interfaces IAddSub and IMultiDiv.
// The IAddSub interface has been aggregated by the outer component
// called CScientific (implemented in Server called AggregationSample.dll) 
// The interface IMultiDiv has not been implemented by the Inner component as 
// it has been used only to explain the theoretical concept that it is not
// neccessary that all the interfaces supported by the inner component
// should get aggregated by the outer component.

// This is the Inner Component Implementation.

// Code By Dinesh Ahuja, October 25,2003.

#include "CBasic.h"
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

// Entries meant for Registering the Inner Component.
const char gszBasicName[] = "COM,Basic Class";
const char gszBasicIndProgID[] = "COMSample.CBasic";
const char gszBasicProgID[] = "COMSample.CBasic.1";

// Inner Component CLSID : {E247F702-0743-11d8-B532-30A64EC10000}
static const CLSID CLSID_CBasic = 
{ 0xe247f702, 0x743, 0x11d8, { 0xb5, 0x32, 0x30, 0xa6, 0x4e, 0xc1, 0x0, 0x0 } };


HRESULT CBasic::NonDelegateQueryInterface( const IID& iid, void** ppv) {

	cout<<"CBasic::NonDelegateQueryInterface Called"<<endl;
	if (iid == IID_IUnknown) {
		// When the AddRef will be called on the ppv before returning
		// from this function,the reference count of the inner component
		// is incremented by 1.
		cout<<"Typecasting this pointer to INonDelegateUnknown pointer"<<endl;
		*ppv = static_cast<INonDelegateUnknown*>(this);
	}
	else if (iid == IID_IAddSub) {
		// When the AddRef will be called on the ppv before returning
		// from this function,the reference count of the outer component
		// is incremented by 1.This is because that the IUnknown implementation
		// of  IAddSub interface on inner component should delegate calls
		// to the outer component's controlling IUnknown implementation.
		cout<<"Typecasting this pointer to IAddSub pointer"<<endl;
		*ppv = static_cast<IAddSub*>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	
	cout<<"Calling AddRef in the CBasic::NonDelegateQueryInterface"<<endl;
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;

}

ULONG __stdcall CBasic::NonDelegateAddRef() {
	
	InterlockedIncrement(&this->m_cInnerRef);
	cout<<"CBasic::NonDelegateAddRef Called: "<<this->m_cInnerRef<<endl;
	return this->m_cInnerRef; 

}

ULONG __stdcall CBasic::NonDelegateRelease() {
	InterlockedDecrement(&this->m_cInnerRef);
	
	cout<<"CBasic::NonDelegateRelease Called: "<<this->m_cInnerRef<<endl;

	if (this->m_cInnerRef == 0) {
		delete this;
		return 0;
	}

	return this->m_cInnerRef; 
}

CBasic::CBasic(IUnknown* m_pUnknown) {
	
	cout<<"CBasic::Constructor Called"<<endl;

	this->m_cInnerRef = 1;

	::InterlockedIncrement(&::gNoComponents);
	
	if (m_pOuterUnknown == NULL) {
		cout<<"Delegate Unknown will forward calls to the NonDelegate Unknown"<<endl;
		m_pOuterUnknown = reinterpret_cast<IUnknown*>(static_cast<INonDelegateUnknown*>(this));
	}
	else {
		cout<<"Inner Component is Aggregated!"<<endl;
		this->m_pOuterUnknown = m_pUnknown;
	}

}


CBasic::~CBasic() {

	InterlockedDecrement(&::gNoComponents);
	cout<<"CBasic::Destructor Called"<<endl;
}


HRESULT __stdcall CBasic::QueryInterface(const IID& iid,void** ppv) {

	cout<<"Inner Component delegating call to QueryInterface to the Controlling Unknown"<<endl;
	return this->m_pOuterUnknown->QueryInterface(iid,ppv);  
}


ULONG __stdcall CBasic::AddRef() {

	cout<<"Inner Component delegating call to AddRef to the Controlling Unknown"<<endl;
	return this->m_pOuterUnknown->AddRef();  
}
  
ULONG __stdcall CBasic::Release() {

	cout<<"Inner Component delegating call to Release to the Controlling Unknown"<<endl;
	return this->m_pOuterUnknown->Release();  
}


void __stdcall CBasic::GetAdd(int Val1,int Val2,int* pResult) {

	*pResult = Val1 + Val2;
}

void __stdcall CBasic::GetSub(int Val1,int Val2,int* pResult) {

	*pResult = Val1 - Val2;
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
	cout<<"CBasic's CFactory::Constructor Called"<<endl;
}

CFactory::~CFactory () {
	cout<<"CBasic's CFactory::Destructor Called"<<endl;
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

	 if ((iid == IID_IAddSub) || (iid == IID_IUnknown)) {

		if ((pUnkOuter!= NULL) && (iid != IID_IUnknown)) {

			return CLASS_E_NOAGGREGATION;
		}
		
		CBasic* pBasic = new CBasic(pUnkOuter);
		
		if (pBasic == NULL) {
			return E_OUTOFMEMORY;
		}

		// NonDelegateQueryInterface has been called so that the NonDelegate
		// IUnknown pointer should be returned to the outer component.
		// This call to NonDelegateQueryInterface will increment the
		// reference count of the inner component by 1.
		cout<<"Calling NonDelegateQueryInterface on pBasic pointer"<<endl;
		HRESULT hResult = pBasic->NonDelegateQueryInterface(iid,ppv);
		cout<<"Returned from NonDelegateQueryInterface on pBasic pointer"<<endl;

		if(FAILED(hResult)) {
			// This gives a string description for the error.
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
				hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
			cout<<(LPTSTR)pMsgBuf<<endl;
			LocalFree((HLOCAL)pMsgBuf);
			return E_FAIL;
		}
		
		// This call to Release will decrements the reference count of the
		// inner component by 1.
		pBasic->NonDelegateRelease();
		return hResult;
	}
	else {
		
		hResult = E_FAIL;
		return hResult;
	}
	
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
				TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}"),
				REG_SZ, gszBasicName,strlen(gszBasicName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\InProcServer32"),
				REG_SZ, szFileName, strlen(szFileName));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\ProgID"),
				REG_SZ, gszBasicProgID, strlen(gszBasicProgID));

	RegSetValue(HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\VersionIndependentProgID"),
				REG_SZ, gszBasicIndProgID, strlen(gszBasicIndProgID));

	return S_OK;
}

STDAPI DllUnregisterServer() {
	
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\VersionIndependentProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}\\ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT,TEXT("CLSID\\{E247F702-0743-11d8-B532-30A64EC10000}"));

	return S_OK;
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD dwReason,void * lpReserved) {
	
	if (dwReason == DLL_PROCESS_ATTACH) {
		cout<<"DllMain Function Called for CBasic"<<endl;
		ghModule = (HINSTANCE)hModule;
	}
	
	return TRUE;
}

STDAPI DllCanUnloadNow() {

	cout<<"DllCanUnloadNow called for CBasic"<<endl;
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
	if(clsid == CLSID_CBasic) {
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

