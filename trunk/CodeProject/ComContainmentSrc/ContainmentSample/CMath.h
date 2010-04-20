#include<objbase.h>
#include "CSquare.h"

#define interface struct
#define _STDCALL	 __stdcall

interface IMath : IUnknown {

	virtual void _STDCALL SumSquare(int Val1,int Val2,int* pResult) = 0;
	
};

extern "C" {

	extern const IID IID_IMath;
	
}


class CMath : public IMath {

public:
	// Implementing IUnknown Interface.
	virtual HRESULT __stdcall QueryInterface(const IID& iid,void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//Implementing IMath Interface.
	virtual void _STDCALL SumSquare(int Val1,int Val2,int* pResult);
	
	// Constructor 
	CMath();

	// Destructor
	~CMath();
	ISquare* m_pISquare;

private:
	long m_cRef;		

};
