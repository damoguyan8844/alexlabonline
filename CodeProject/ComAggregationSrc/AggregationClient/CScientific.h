#include<objbase.h>

#define interface struct
#define _STDCALL	 __stdcall

interface ITrignometry : IUnknown {

	virtual void _STDCALL GetSin(double Val1,double* pResult) = 0;
	
};


extern "C" {

	extern const IID IID_ITrignometry;
	
}


class CScientific : public ITrignometry {

public:
	
	// Implementing IUnknown interface.
	virtual HRESULT _STDCALL QueryInterface(const IID& iid,void **ppv);
	virtual ULONG _STDCALL AddRef();
	virtual ULONG _STDCALL Release();

	//Implementing ITrignometry interface.
	virtual void _STDCALL GetSin(double Val1,double* pResult);
	
	// Constructor 
	CScientific();

	// Destructor
	~CScientific();

	// This has been made public, so that we can understand the
	// concept of reference count in Aggregation technique.
	long m_cRef;
	
	IUnknown* m_pInnerUnknown;

};
