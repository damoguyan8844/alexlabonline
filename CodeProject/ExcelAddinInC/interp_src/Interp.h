#include "generic.h"

#define dimResult	200		// size of the permanent array for returning data to XL

// Local function prototypes
WORD	ClipSize( XLOPER *multi );

// global function prototypes
__declspec(dllexport) void xlAutoFree( LPXLOPER pxFree );
__declspec(dllexport) LPXLOPER Interp(	LPXLOPER x, LPXLOPER xArray,
											LPXLOPER yArray, short extrapFlag );
__declspec(dllexport) LPXLOPER InterpX(	double x, LPXLOPER xArray,
											LPXLOPER yArray, short extrapFlag );


