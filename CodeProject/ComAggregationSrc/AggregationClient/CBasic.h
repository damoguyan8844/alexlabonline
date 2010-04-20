#include<objbase.h>

#define interface struct
#define _STDCALL	 __stdcall

interface IAddSub : IUnknown {

	virtual void _STDCALL GetAdd(int Val1,int Val2,int* pResult) = 0;
	virtual void _STDCALL GetSub(int Val1,int Val2,int* pResult) = 0;
	
};


interface INonDelegateUnknown {
	
	virtual HRESULT _STDCALL NonDelegateQueryInterface (const IID&, void**) = 0;
	virtual ULONG _STDCALL NonDelegateAddRef() = 0;
	virtual ULONG _STDCALL NonDelegateRelease() = 0;
	
};

extern "C" {

	extern const IID IID_IAddSub;
	extern const IID IID_INonDelegateUnknown;
	
}


class CBasic : public IAddSub, public INonDelegateUnknown {

public:
	
	// Implementing Delegating IUnknown Interface. The Delegating
	// IUnknown interface is the interface which is inherited
	// by the IAddSub interface.
	virtual HRESULT _STDCALL QueryInterface(const IID& iid,void **ppv);
	virtual ULONG _STDCALL AddRef();
	virtual ULONG _STDCALL Release();

	// Implementing NonDelegating IUnknown Interface.
	virtual HRESULT _STDCALL NonDelegateQueryInterface (const IID&, void**);
	virtual ULONG _STDCALL NonDelegateAddRef();
	virtual ULONG _STDCALL NonDelegateRelease();
	

	//Implementing IAddSub Interface.
	virtual void _STDCALL GetAdd(int Val1,int Val2,int* pResult);
	virtual void _STDCALL GetSub(int Val1,int Val2,int* pResult);
	
	// Constructor 
	CBasic(IUnknown* m_pOuterUnknown);

	// Destructor
	~CBasic();
	
	long m_cInnerRef;
	
	IUnknown* m_pOuterUnknown;

};
