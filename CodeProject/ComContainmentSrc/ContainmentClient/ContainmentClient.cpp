// This is a client side coding in which the client will be using the 
// outer component. The outer component acts as a client for an inner
// component and uses the services provided by the inner component.

// Code By Dinesh Ahuja, October 11,2003.

#include <iostream.h>
#include "CMath.h"


//Outer Component ClassID : {CD082341-FC1E-11d7-B532-30B04EC10000}
static const CLSID CLSID_CMath = 
{ 0xcd082341, 0xfc1e, 0x11d7, { 0xb5, 0x32, 0x30, 0xb0, 0x4e, 0xc1, 0x0, 0x0 } };

// Inner Component ClassID : {CD082342-FC1E-11d7-B532-30B04EC10000}
static const CLSID CLSID_CSquare = 
{ 0xcd082342, 0xfc1e, 0x11d7, { 0xb5, 0x32, 0x30, 0xb0, 0x4e, 0xc1, 0x0, 0x0 } };

int main() {
	
	// Call this to Initialize COM Runtime Library.for the current thread.
	CoInitialize(NULL);

	cout<<"CMath creation Process starts"<<endl;
	// This is meant for storing the error description, corrosponding
	// to HRESULT.
	void * pMsgBuf;

	IClassFactory *pIFactory = NULL;
		
	// Request for a IClassFactory interface pointer on the class object.
	HRESULT hResult = CoGetClassObject(CLSID_CMath,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,(void**)&pIFactory);
	
	if(SUCCEEDED(hResult)) {
		cout<<"IClassFactory pointer on class object for CMath succeeded"<<endl;
		IUnknown *pUnk = NULL;
	
		// This will create a CMath's instance and returns an IUnknown 
		// pointer on that component.
		hResult = pIFactory->CreateInstance(NULL,IID_IUnknown,(void**)&pUnk);
		
		/* After instantiating the CMath's instance, the class object can be
		** released from memory.
		** At this point, we will release class object by calling Release 
		** on IClassFactory.
		*/
		cout<<"Calling Release on the IClassFactory for CMath's instance"<<endl;
		pIFactory->Release();

		pIFactory = NULL;

		if(SUCCEEDED(hResult)) {
			IMath *pIMath = NULL;

			/* Request for IMath interface on the component by using the
			** IUnknown interface pointer on component.
			*/
			hResult = pUnk->QueryInterface(IID_IMath,(void**)&pIMath);
			pUnk->Release(); 

			// If succeeded, then we can call the methods of IMath.
			if(SUCCEEDED(hResult)) {
				int RetVal;
				pIMath->SumSquare(250,25,&RetVal);
				cout<<"CMath:: SumSquare's returned value: "<<RetVal<<endl;

				/* This call to Release() will cause the component to be 
				** released from memory.
				*/
				pIMath->Release();
			}
			else {
				
				// This gives a string description for the error.
				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
					hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
				cout<<(LPTSTR)pMsgBuf<<endl;
				LocalFree((HLOCAL)pMsgBuf);
			}
		}
	}
	else {
		
		// This gives a string description for the error.
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,
			hResult,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);

		cout<<(LPTSTR)pMsgBuf<<endl;
		LocalFree((HLOCAL)pMsgBuf);
	}
	
	cout<<"CMath creation Process ends."<<endl;

	CoUninitialize();

	return 0;
}