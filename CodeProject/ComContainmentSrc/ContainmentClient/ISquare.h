
#define interface struct
#define _STDCALL	 __stdcall

interface ISquare : IUnknown {

	virtual void _STDCALL Square(int Val1,int* pResult) = 0;
	
};


extern "C" {

	extern const IID IID_ISquare;
	
}
