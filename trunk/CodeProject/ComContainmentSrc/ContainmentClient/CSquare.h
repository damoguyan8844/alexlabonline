#include<objbase.h>
#include "ISquare.h"

class CSquare : public ISquare {

public:
	// Implementing IUnknown Interface.
	virtual HRESULT __stdcall QueryInterface(const IID& iid,void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//Implementing ISquare Interface.
	virtual void _STDCALL Square(int Val1,int* pResult);
	
	// Constructor 
	CSquare();

	// Destructor
	~CSquare();
	
private:
	long m_cRef;		

};
