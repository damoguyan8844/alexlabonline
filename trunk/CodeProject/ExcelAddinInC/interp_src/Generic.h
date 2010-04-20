#include <ctype.h>
#include <windows.h>
#include "xlcall.h"
#include <math.h>      
#include <string.h>

// This is the maximum allowable value for gMaxFuncParms
// it's hard-wired to avoid dynamic memory allocation for
// temporary variables
#define	kMaxSize	15

// user defined types
typedef short	Boolean;

// function registration data
extern	LPSTR	*gFuncParms;
extern	char	const *gModuleName;
extern	int const	gFunctionCount, gMaxFuncParms;

// public function prototypes
Boolean PStrCmp( const char *str1, const char *str2 );
void PStrCopy( const char *str1, char *str2 );
void PStrCat( char *str1, const char *str2 );
char *NumPStr( int value, char *string );
