// This is the client which will be using the services provided
// by AggregationSample.dll and AggregableObject.dll servers.
// The client should be unaware of the fact that the outer component
// is aggregating the inner component.

// This is the AggregationClient Implementation.

// Code By Dinesh Ahuja, October 25,2003.

#include <iostream.h>
#include "CScientific.h"
#include "CBasic.h"

// Outer Component CLSID: {E247F701-0743-11d8-B532-30A64EC10000}
static const CLSID CLSID_CScientific = 
{ 0xe247f701, 0x743, 0x11d8, { 0xb5, 0x32, 0x30, 0xa6, 0x4e, 0xc1, 0x0, 0x0 } };


// {E247F702-0743-11d8-B532-30A64EC10000}
static const CLSID CLSID_CBasic = 
{ 0xe247f702, 0x743, 0x11d8, { 0xb5, 0x32, 0x30, 0xa6, 0x4e, 0xc1, 0x0, 0x0 } };


int main() {
	
	// Call this to Initialize COM Runtime Library.for the current thread.
	CoInitialize(NULL);

	cout<<"CScientific Creation Process starts"<<endl;
	// This is meant for storing the error description, corrosponding
	// to HRESULT.
	void * pMsgBuf;

	IClassFactory *pIFactory = NULL;
		
	// Request for a IClassFactory interface pointer on the class object.
	HRESULT hResult = CoGetClassObject(CLSID_CScientific,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,(void**)&pIFactory);
	
	if(SUCCEEDED(hResult)) {
		cout<<"IClassFactory pointer on class object for CScientific succeeded"<<endl;
		IUnknown *pUnk = NULL;
	
		// This will create a CMath's instance and returns an IUnknown 
		// pointer on that component.
		hResult = pIFactory->CreateInstance(NULL,IID_IUnknown,(void**)&pUnk);
		
		/* After instantiating the CMath's instance, the class object can be
		** released from memory.
		** At this point, we will release class object by calling Release 
		** on IClassFactory.
		*/
		cout<<"Calling Release on the IClassFactory for CScientific's instance"<<endl;
		pIFactory->Release();

		pIFactory = NULL;

		if(SUCCEEDED(hResult)) {
			ITrignometry *pITrignometry = NULL;

			/* Request for IMath interface on the component by using the
			** IUnknown interface pointer on component.
			*/
			hResult = pUnk->QueryInterface(IID_ITrignometry,(void**)&pITrignometry);
			pUnk->Release(); 

			// If succeeded, then we can call the methods of IMath.
			if(SUCCEEDED(hResult)) {
				double RetVal;
				double pi = 3.1415926535;
				pITrignometry->GetSin(pi/2,&RetVal);
				cout<<"CScientific::GetSin's returned value: "<<RetVal<<endl;

				IAddSub* pAddSub = NULL;
				// This QueryInterface will cause the reference count
				// of the outer component to be incremented by 1.
				cout<<"ITrignometry Interface queries for IAddSub Interface"<<endl;
				hResult = pITrignometry->QueryInterface(IID_IAddSub,(void**) &pAddSub);
				cout<<"Returned: ITrignometry Interface queries for IAddSub Interface"<<endl;

				// The increment in the above QueryInterface call has to be nullified
				// ,otherwise the outer component will not be released.
				// This Release will decrement the reference count on the
				// outer component.
				cout<<"Calling Release to Nullify the increment in Reference count"<<endl;
				static_cast<IUnknown*>(pITrignometry)->Release(); 
				cout<<"Returned: Release to Nullify the increment in Reference count"<<endl;
			
				IUnknown* pUnknown = NULL;
				// This will return the Controlling outer IUnknown pointer.
				// This will increment the reference count of the outer component by 1.
				cout<<"pAddSub pointer queries for IUnknown pointer"<<endl;
				hResult = pAddSub->QueryInterface(IID_IUnknown,(void**) &pUnknown);
				cout<<"Returned: pAddSub pointer queries for IUnknown pointer"<<endl;
					
				// Client is using the service provided by the IAddSub Interface.
				int retVal = 0;
				pAddSub->GetAdd(10,20,&retVal);
				cout<<"Output of IAddSub::GetAdd is: "<<retVal<<endl;
					
				// The increment in the above QueryInterface call has to be nullified
				// ,otherwise the outer component will not be released.
				// This Release will decrement the reference count on the
				// outer component.
				cout<<"Calling Release to Nullify the increment in Reference count"<<endl;
				pUnknown->Release(); 
				cout<<"Returned: Release to Nullify the increment in Reference count"<<endl;

				/* This call to Release() will cause the component to be 
				** released from memory.
				*/
				cout<<"Final Release on the Outer Component"<<endl;
				pITrignometry->Release();
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
	
	cout<<"CScientific Creation Process ends."<<endl;

	CoUninitialize();

	return 0;
}