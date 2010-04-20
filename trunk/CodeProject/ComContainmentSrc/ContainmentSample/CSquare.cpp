// This is server which exposes the outer component and a inner component.
// The outer component acts as a client and uses the services (exposed by interfaces)
// provided by an inner components.
// This is Inner Component Implmentation.

// Code By Dinesh Ahuja, October 11,2003.

#include "CSquare.h"
#include <iostream.h>

extern long gNoComponents ;

/*******************************************************************/
// CMath's methods Implementation.

CSquare::CSquare() {
	this->m_cRef = 1;
	InterlockedIncrement(&gNoComponents);
	cout<<"CSquare::Constructor called"<<endl;
}

CSquare::~CSquare() {
	InterlockedDecrement(&gNoComponents);
	cout<<"CSquare::Destructor called"<<endl;
}

void _STDCALL CSquare::Square(int Val1,int* pResult) {
	
	*pResult = Val1 * Val1;
}


ULONG _STDCALL CSquare::AddRef() {

	return InterlockedIncrement(&this->m_cRef);
}

ULONG _STDCALL CSquare::Release() {

	if (InterlockedDecrement(&this->m_cRef) == 0) {
		delete this;
		return 0;
	}

	return this->m_cRef; 
}

HRESULT _STDCALL CSquare::QueryInterface(const IID& iid, void **ppv) {

	// CMath supports only two interfaces IUnknown and IMath interface.
	if (iid == IID_IUnknown) {
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid  == IID_ISquare) {
		*ppv = static_cast<ISquare*>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	(static_cast<IUnknown*>(*ppv))->AddRef();

	return S_OK;
}