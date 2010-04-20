#include "generic.h"
/*
/*
** LibMain
**
** This function is called by the LibEntry function, which is called
** by Windows when the DLL is first loaded. LibEntry initializes the
** DLL's heap if a HEAPSIZE is specified in the DLL's .DEF file, and
** then calls LibMain. The following LibMain function satisfies that
** call. The LibMain function should perform additional initialization
** tasks required by the DLL. LibMain will be called only once, when a DLL is
** first loaded, even if that DLL is used by multiple applications.
**
** LibMain
**
** Arguments:
**
**      HANDLE hInstance    Instance handle
**      WORD wDataSeg       Library's data segment
**      WORD wHeapSize      Heap size
**      LPSTR lpszCmdLine   Command line
**
** Returns:
**
**      int                 1 if initialization is successful.
*/

int APIENTRY DllMain( HANDLE hModule, DWORD fdwReason, LPVOID lpReserved )
{
   switch ( fdwReason )
   {
      case DLL_PROCESS_ATTACH:
        return 1;
      case DLL_THREAD_ATTACH:
         break;
      case DLL_THREAD_DETACH:
         break;
      case DLL_PROCESS_DETACH:
         break;
   }

   return 1;
}

/*
** xlAutoOpen
**
** Microsoft Excel uses xlAutoOpen to load XLL files.
** When you open an XLL file, the only action
** Microsoft Excel takes is to call the xlAutoOpen function.
**
** More specifically, xlAutoOpen is called:
**
**  - when you open this XLL file from the File menu,
**  - when this XLL is in the XLSTART directory, and is
**    automatically opened when Microsoft Excel starts,
**  - when Microsoft Excel opens this XLL for any other reason, or
**  - when a macro calls REGISTER(), with only one argument, which is the
**    name of this XLL.
**
** xlAutoOpen is also called by the Add-in Manager when you add this XLL 
** as an add-in. The Add-in Manager first calls xlAutoAdd, then calls
** REGISTER("EXAMPLE.XLL"), which in turn calls xlAutoOpen.
**
** xlAutoOpen should:
**
**  - register all the functions you want to make available while this
**    XLL is open,
**
**  - add any menus or menu items that this XLL supports,
**
**  - perform any other initialization you need, and
**
**  - return 1 if successful, or return 0 if your XLL cannot be opened.
*/


__declspec(dllexport) int xlAutoOpen( void )
{
	static XLOPER		xDLL,
						xStr[kMaxSize];
	static LPXLOPER		xPtr[kMaxSize+1];
	static XLOPER		xStrT, xIntT;
		   XLOPER		xConfirm;            /* for confirmation of license agreement */
						
	int					i, j, rv;			// Loop indices
	BYTE				len;
        
// configured to handle no more than kMaxSize function parameters
// For a short allocation on the stack, it's not worth messing with
// dynamic allocation here.

	if ( gMaxFuncParms > kMaxSize )
		return 0;
		
	xIntT.xltype = xltypeInt;
	xIntT.val.w = 2;
	xStrT.xltype = xltypeStr;

	// get the name of this XLL, which is then passed back to Excel in the xlfRegister process
	Excel4( xlGetName, &xDLL, 0 );

	xPtr[0] = (LPXLOPER) &xDLL;
   
	// Repeat the registration process for each function to be added
	for ( i=0; i<gFunctionCount; i++ )
	{
		// Copy each of the xlfRegister arguments into the XLOPER struct to be passed to Excel
		for ( j=0; j<gMaxFuncParms; j++ )
 		{
 			len = (BYTE) lstrlen ( gFuncParms[ i*gMaxFuncParms + j ] );
    		if ( len == 0 )
				break;

			xStr[j].xltype = xltypeStr;
    		// allocate memory (the length is correct, since it has a space for the byte count already)
    		xStr[j].val.str = malloc(len);
    		// copy the string to the new location
    		memmove( xStr[j].val.str, gFuncParms[i*gMaxFuncParms + j], len );
    		// insert the byte count
			xStr[j].val.str[0] = (BYTE) (len - 1);
    		
    		xPtr[j+1] = (LPXLOPER FAR) &xStr[j];
     	}
     	
     	// Call the Excel function registration routine
		rv = Excel4v( xlfRegister, &xConfirm, j+1, (LPXLOPER FAR *) &xPtr[0] );

		// free the allocated memory
		for ( j=0; j<gMaxFuncParms; j++ )
		{
			if ( xStr[j].val.str != NULL )
				free( xStr[j].val.str );
			xStr[j].val.str = NULL;
		}

		if ( rv != xlretSuccess )
		{
			xStrT.val.str = (LPSTR) "\052Error: Failed to register the Interp XLL!";
			Excel4( xlcAlert, 0, 2, (LPXLOPER) &xStrT, (LPXLOPER) &xIntT );

			return 0;
		}
	}
	
	// Free the XLL filename
	Excel4( xlFree, 0, 1, (LPXLOPER) &xDLL );
	
	return 1;
}

/*
** xlAutoClose
**
** xlAutoClose is called by Microsoft Excel:
**
**  - when you quit Microsoft Excel, or
**  - when a macro sheet calls UNREGISTER(), giving a string argument
**    which is the name of this XLL.
**
** xlAutoClose is called by the Add-in Manager when you remove this XLL from
** the list of loaded add-ins. The Add-in Manager first calls xlAutoRemove,
** then calls UNREGISTER("EXAMPLE.XLL"), which in turn calls xlAutoClose.
**
** xlAutoClose is called by EXAMPLE.XLL by the function fExit. This function
** is called when you exit Example.
**
** xlAutoClose should:
**
**  - Remove any menus or menu items that were added in xlAutoOpen,
**
**  - do any necessary global cleanup, and
**
**  - delete any names that were added (names of exported functions, and 
**    so on). Remember that registering functions may cause names to be created.
**
** xlAutoClose does NOT have to unregister the functions that were registered
** in xlAutoOpen. This is done automatically by Microsoft Excel after
** xlAutoClose returns.
**
** xlAutoClose should return 1.
*/

__declspec(dllexport) int xlAutoClose( void )
{
	return 1;
}



/*
** xlAutoRegister
**
** This function is called by Microsoft Excel if a macro sheet tries to
** register a function without specifying the type_text argument. If that
** happens, Microsoft Excel calls xlAutoRegister, passing the name of the
** function that the user tried to register. xlAutoRegister should use the
** normal REGISTER function to register the function, only this time it must
** specify the type_text argument. If xlAutoRegister does not recognize the
** function name, it should return a #VALUE! error. Otherwise, it should
** return whatever REGISTER returned.
**
** Arguments:
**
**      LPXLOPER pxName     xltypeStr containing the
**                          name of the function
**                          to be registered. This is not
**                          case sensitive.
**
** Returns:
**
**      LPXLOPER            xltypeNum containing the result
**                          of registering the function,
**                          or xltypeErr containing #VALUE!
**                          if the function could not be
**                          registered.
*/

__declspec(dllexport) LPXLOPER xlAutoRegister( LPXLOPER pxName )
{
	static XLOPER	xRegId;

	// This block initializes xRegId to a #VALUE! error first. This is done in
	// case a function is not found to register. Next, the code loops through 
	// the functions in rgFuncs[] and uses lpstricmp to determine if the 
	// current row in rgFuncs[] represents the function that needs to be 
	// registered. When it finds the proper row, the function is registered 
	// and the register ID is returned to Microsoft Excel. If no matching 
	// function is found, an xRegId is returned containing a #VALUE! error.


	// Do not allow the registration of individual functions, always return
	// an error here

	xRegId.xltype = xltypeErr;
	xRegId.val.err = xlerrValue;

	return (LPXLOPER) &xRegId;
}

/*
** xlAutoAdd
**
** This function is called by the Add-in Manager only. When you add a
** DLL to the list of active add-ins, the Add-in Manager calls xlAutoAdd()
** and then opens the XLL, which in turn calls xlAutoOpen.
**
*/

__declspec(dllexport) int xlAutoAdd( void )
{                                 
	static XLOPER	xStrT, xIntT;
	char				szBuf[100];

	// Show the Startup Alert Box
	
	strcpy( szBuf, gModuleName );
	strcat( szBuf, " Loaded!\n built on " __DATE__ " at " __TIME__ );
	szBuf[0] = strlen( &szBuf[1] );
	
	xStrT.xltype = xltypeStr;
	xStrT.val.str = szBuf;
	xIntT.xltype = xltypeInt;
	xIntT.val.w = 2;

	// Display a dialog box indicating that the XLL was successfully added
	Excel4( xlcAlert, 0, 2, (LPXLOPER) &xStrT, (LPXLOPER) &xIntT );

	return 1;
}


/*
** xlAutoRemove
**
** This function is called by the Add-in Manager only. When you remove
** an XLL from the list of active add-ins, the Add-in Manager calls
** xlAutoRemove() and then UNREGISTER("EXAMPLE.XLL").
**
** You can use this function to perform any special tasks that need to be
** performed when you remove the XLL from the Add-in Manager's list
** of active add-ins. For example, you may want to delete an
** initialization file when the XLL is removed from the list.
*/

__declspec(dllexport) int xlAutoRemove( void )
{
	char			szBuf[255];
	static XLOPER	xStr, xInt;
	
	PStrCopy( gModuleName, szBuf );
	PStrCat( szBuf, "\011 Removed!" );
	
	xStr.xltype = xltypeStr;
	xStr.val.str = (LPSTR) szBuf;
	xInt.xltype = xltypeInt;
	xInt.val.w = 2;
    
	// Show a dialog box indicating that the XLL was successfully removed
	Excel4( (short) xlcAlert, 0, 2, (LPXLOPER) &xStr, (LPXLOPER) &xInt );
    
	return 1;
}


/*
/* xlAddInManagerInfo
**
**
** This function is called by the Add-in Manager to find the long name
** of the add-in. If xAction = 1, this function should return a string
** containing the long name of this XLL, which the Add-in Manager will use
** to describe this XLL. If xAction = 2 or 3, this function should return
** #VALUE!.
**
** Arguments
**
**      LPXLOPER xAction    What information you want. One of:
**                          1 = the long name of the
**                              add-in
**                          2 = reserved
**                          3 = reserved
** Return value
**
**      LPXLOPER            The long name or #VALUE!.
**
*/

__declspec(dllexport) LPXLOPER xlAddInManagerInfo( LPXLOPER xAction )
{
	static XLOPER xInfo, xIntAction, xInt;
	static char	tempBuf[255];

	// This code coerces the passed-in value to an integer. This is how the
	// code determines what is being requested. If it receives a 1, 
	// it returns a string representing the long name. If it receives 
	// anything else, it returns a #VALUE! error.

	xInt.xltype = xltypeInt;
	xInt.val.w = xltypeInt;

	Excel4( xlCoerce, &xIntAction, 2, xAction, (LPXLOPER) &xInt );

	if ( xIntAction.val.w == 1 )
	{
		xInfo.xltype = xltypeStr;
		(const LPSTR) xInfo.val.str = (LPSTR) gModuleName;
	}
	else
	{
		xInfo.xltype = xltypeErr;
		xInfo.val.err = xlerrValue;
	}

	return (LPXLOPER) &xInfo;
}


//==================================================================
// PStrCmp - compare two pascal strings for equivalence
//           ignoring case sensitivity
//
// Arguments
//	str1 and str2 are the strings to be compared
// Return Values
//  returns FALSE if the strings do not match
//  returns TRUE if the strings are equal
//==================================================================

short PStrCmp( const char *str1, const char *str2 )
{
	short	i,
			len = (short) *str1;

	// see if they have the same length
	if ( *str1++ != *str2++ )
		return FALSE;
    
	for ( i=0; i<len; i++ )
	{
		if ( tolower(*str1) != tolower(*str2) )
			return FALSE;
		str1++;
		str2++;
	}
	
	return TRUE;
}

//==================================================================
// NumPStr - convert an integer value to a pascal string
//
// Arguments
//	value	is any short integer with the value to be converted
//			can be positive or negative
//	string	is a destination string to receive the resulting
//          pascal string, should be at least 7 bytes long to
//          handle the maximum possible string length
// Return Values
//		returns the address of "string"
//==================================================================

char *NumPStr( int value, char *string )
{
	int		pos = 1, i;
	int		mag = 10000000;
	Boolean	first = TRUE;
	
	if ( value < 0 )
	{
		string[pos++] = '-';
		value = -value;
	}
	
	do
	{
		i = value / mag;
		if ( !first || mag == 1 || i != 0 )
		{
			first = FALSE;
			string[pos++] = i + '0';
		}
		value %= mag;
		mag /= 10;
		
	} while ( mag > 0 );
	
	*string = pos - 1;
	
	return string;
}

//==================================================================
// PStrCopy - copy one pascal string to another location
//
// Arguments
//	str1	the source string to be copied
//	str2	the destination string to be copied to
// Return Values
//		void
//==================================================================

void PStrCopy( const char *str1, char *str2 )
{
	short	i,
			len = (short) *str1;
	
	for ( i=0; i<=len; i++ )
		*str2++ = *str1++;
}

//==================================================================
// PStrCat - concatenate one pascal string to another
//
// Arguments
//	str1	the primary pascal string
//	str2	the string which is to be concatenated to the end of str1
// Return Values
//		void
//==================================================================

void PStrCat( char *str1, const char *str2 )
{
	short	i,
			len1, len2;
	
	len1 = ((short) *str1) + 1;
	if ( len1 > 255 )
		return;
	
	len2 = len1 + ((short) *str2) - 1;
	if ( len2 > 255 )
		len2 = 255; 
	
	*str1 = (char) len2;
	
	str2++;
	for ( i=len1; i<=len2; i++ )
		str1[i] = *str2++;
}

//==================================================================
// ToPascalString - convert a null-terminated C string to a pascal
//                  string
//
// Arguments
//	string	the C string to be converted in place
// Return Values
//		void
//==================================================================

void ToPascalString( char *string )
{
	short	len;
	
	len = strlen( string );
	len = (len > 255) ? 255 : len;
	memmove( &string[1], &string[0], len );
	string[0] = (char) len;
	
	return;
}

//==================================================================
// StripBlanks - Remove leading and trailing blank and tab characters
//               from a pascal string
//
// Arguments
//	string	the pascal string to have blanks stripped (in place)
// Return Values
//		void
//==================================================================

void StripBlanks( char *string )
{
	short	len, first, last, i;
	
	len = string[0];
	for ( i=1; i<=len; i++ )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	
	first = i;
	
	for ( i=len; i>first; i-- )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	last = i;
	
	len = last - first + 1;
	if ( first != 1 )
		memmove( &string[1], &string[first], len );
	string[0] = (char) len;
	
	return;	
}


