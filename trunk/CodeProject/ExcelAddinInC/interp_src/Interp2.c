#include "interp.h"
#include "intrface.h"

/*
** Interp( x, xArray, yArray, interpFlag )
**
** The Interp function does linear interpolation of a value in a table
**
** Arguments:
**
**      LPXLOPER ...    x			Can be a literal value, a reference, or an array
**		LPXLOPER ...    xArray		A list of x values for the lookup table
**										This can be a reference or an array, but
**										all elements must be numeric.  Any blank or
**										elements at the end of the array will
**										be ignored.  The values must be sorted in
**										increasing order.  The
**										preferred dimension for the table is by
**										column.  That is, if a two-dimensional table
**										is passed, then only the data in the first
**										row (multiple columns) will be used.  If a
**										one-dimensional table is passed, then all
**										the data will be used for the table, regardless
**										of whether it's column-wise or row-wise.
**		LPXLOPER ...    yArray		A list of y values that correspond to the x
**										values above.  This can be a reference or an
**										array, but all elements must be numeric.  Any
**										blank elements at the end of the
**										array will be ignored.  Interp will use the
**										smaller of the available number of elements
**										in xArray or yArray as the table size.  The
**										preferred dimension for the table is by
**										column.  That is, if a two-dimensional table
**										is passed, then only the data in the first
**										row (multiple columns) will be used.  If a
**										one-dimensional table is passed, then all
**										the data will be used for the table, regardless
**										of whether it's column-wise or row-wise.
**		short			extrapFlag	A true or false value indicating whether or not
**										extrapolation will be allowed.  If true, then
**										the function result can be extrapolated beyond
**										the table bounds.  If false, then if the x
**										value is outside the x table range, the
**										function result will be the bounding value
**										from the y table.
**
** Returns:
**
**      LPXLOPER		Interp		The interpolated result, or #VALUE! if the input
**										data is invalid.  If x is a single value, then
**										a single result will be returned.  If x is
**										a range or an array, then an array of values
**										will be returned with the same row,col
**										dimensions as the x data.
**
*/

HANDLE	hArray;				// temporary handle for returning large arrays to XL

__declspec(dllexport) LPXLOPER InterpX(	double x, LPXLOPER xArray,
										LPXLOPER yArray, short extrapFlag )
{
	static XLOPER	tempXLOper;				// Return Data

	tempXLOper.xltype = xltypeNum;
	tempXLOper.val.num = x;
	
	return Interp( (LPXLOPER) &tempXLOper, xArray, yArray, extrapFlag );	
}

__declspec(dllexport) LPXLOPER Interp(	LPXLOPER x, LPXLOPER xArray,
										LPXLOPER yArray, short extrapFlag )
{
	static XLOPER	resultBuffer[dimResult];	// Return Data Array
	
    double			xlo, xhi,					// temporary values for interpolation
    				ylo, yhi,
    				currX;						// the current x value
    			
    LPXLOPER		xPtr, yPtr,					// pointers to arrays of x & y table data
    				targPtr;					// pointer to array of input x data

    static XLOPER	xMulti,						// xArray coerced to xltypeMulti
					yMulti,						// yArray coerced to xltypeMulti
					targMulti,					// x coerced to xltypeMulti
					tempTypeMulti;				// xltypeMulti in an XLOPER for passing
					
	short			hasXMulti = 0,
					hasYMulti = 0,
					hasTargMulti = 0,	// flags to indicate memory has been allocated
					error = -1,			// -1 if no error; error code otherwise
     				sortFlag = -1;		// 0 for increasing x values, 1 for decreasing

   	WORD			xSize,				// size of the preferred dimension of xArray
    				ySize;				// size of the preferred dimension of yArray

	ULONG			lo, hi, mid,
    				i, j,				// temporary index values
    				xCount = 0;			// the number of x target values to handle

	LPXLOPER		retArrayPtr,		// pointer to the return results array
					tempPtr;			// temporary pointer
	static XLOPER	retMulti;			// return data structure

	// Initialize some variables
	
	tempTypeMulti.xltype = xltypeInt;
    tempTypeMulti.val.w = xltypeMulti;
    
	// ======= Get the xArray Data ==============
	// and verify the type

	if (	xArray->xltype != xltypeRef		&&
			xArray->xltype != xltypeSRef	&&
			xArray->xltype != xltypeMulti )
	{
		error = xlerrValue;
		goto done;
	}
	
	
	// Coerce the data into the "Multi" type since that's what we expect.
	// If coerce fails due to an uncalced cell, return immediately and Excel will
	// call us again in a moment after that cell has been calced.

	if ( xlretUncalced ==
			Excel4( xlCoerce, (LPXLOPER) &xMulti, 2, (LPXLOPER) xArray,
						(LPXLOPER) &tempTypeMulti ) )
	{
		return 0;
    }
    
	hasXMulti = 1;		// indicate that Excel has allocated memory for the xMulti

	// ======= Get the yArray Data ==============

	if (	yArray->xltype != xltypeRef &&
			yArray->xltype != xltypeSRef &&
			yArray->xltype != xltypeMulti )
	{
		error = xlerrValue;
		goto done;
	}
	
	
	if ( xlretUncalced ==
			Excel4( xlCoerce, (LPXLOPER) &yMulti, 2, (LPXLOPER) yArray,
						(LPXLOPER) &tempTypeMulti ) )
	{

		// if coerce failed due to an uncalced cell, return immediately.
		// first need to free memory in xMulti
		if ( hasXMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );

		return 0;
	}
	
	hasYMulti = 1;	// indicate Excel has allocated memory in the yMulti structure

	// determine the size of the x and y tables, ignoring empty cells
	// at the end of the data
	
    xSize = ClipSize( &xMulti );
    ySize = ClipSize( &yMulti );
    
    // save some temporary pointers to the actual x and y table data
    
 	xPtr = xMulti.val.array.lparray;
 	yPtr = yMulti.val.array.lparray;
    
	// use the smaller array dimension from x or y
	// from here on out, xSize is the dimension to use
	
	if ( ySize < xSize )
		xSize = ySize;
	
	// make sure we have at least two values, otherwise there's no table
	// for interpolation
	
	if ( xSize < 2 )
	{
		error = xlerrValue;
		goto done;
	}

	// verify that the entire xArray and yArray are nums
	// also verify that xArray is monotonically increasing or decreasing
 	
	for ( i=0; i<xSize; i++ )
	{
 		if (	xPtr[i].xltype != xltypeNum	||
				yPtr[i].xltype != xltypeNum )
 		{
 			// if the x or y table value is an error, then pass that error type
 			// through to the result, otherwise, use a #VALUE! error
 			
 			if ( xPtr[i].xltype == xltypeErr )
 				error = xPtr[i].val.err;
 			else if ( yPtr[i].xltype == xltypeErr )
 				error = yPtr[i].val.err;
 			else
				error = xlerrValue;
			goto done;
 		}
 		
 		// make sure that the x table data is monotonically increasing or decreasing
 		// sortflag is set to zero for increasing, and one for decreasing data
 		
 		if ( i > 0 )
 		{
 			// is the current value less than the previous one?
 			if ( xPtr[i].val.num < xPtr[i-1].val.num )
 			{
 				if ( sortFlag == 0 )	// if previous data was increasing, it's an error
 				{
 					error = xlerrValue;
					goto done;
				}
				sortFlag = 1;			// indicate decreasing data
 			}
  			// is the current value greater than the previous one?
			else if ( xPtr[i].val.num > xPtr[i-1].val.num )
 			{
 				if ( sortFlag == 1 )	// if previous data was decreasing, it's an error
 				{
 					error = xlerrValue;
					goto done;
				}
				sortFlag = 0;			// indicate increasing data
 			}
 		}
	}

	// ======= Get the x (Target) Data ==============
	
	// make sure it's an expected data type

	if (	x->xltype != xltypeRef		&&
			x->xltype != xltypeSRef		&&
			x->xltype != xltypeMulti	&&
			x->xltype != xltypeNum )
	{
		error = xlerrValue;
		goto done;
	}
        
	// Convert the x data to a "multi" type	
	if ( xlretUncalced == Excel4( xlCoerce, (LPXLOPER) &targMulti, 2,
				(LPXLOPER) x, (LPXLOPER) &tempTypeMulti ) )
	{
		// if coerce fails due to an uncalced cell, return immediately and Excel
		// will call us again after that cell has been calced.
		// But first, free up the memory allocated so far.

		if ( hasXMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );
		if ( hasYMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &yMulti );
			
		return 0;
	}
		
	hasTargMulti = 1;	// indicate Excel has allocated memory for the targMulti

	// save a temporary pointer to the x data values
	targPtr = targMulti.val.array.lparray;
		
	// xCount is the number of x data values that need to be interpolated
	xCount = targMulti.val.array.columns * targMulti.val.array.rows;
		
	// set up the return array data structure
	// type is "multi"
	retMulti.xltype = xltypeMulti;
	
	// row and column size match those of the array of x values								
	retMulti.val.array.columns = targMulti.val.array.columns;
	retMulti.val.array.rows = targMulti.val.array.rows;
		    
	// For efficiency, we have a static buffer that holds up to "dimResult" values
	// If it's large enough, use it.  Otherwise, allocate memory and tell excel to
	// call us back to free it ( via xlAutoFree ).
			
    if ( xCount > dimResult )
    {
		retArrayPtr = (LPXLOPER) GlobalLock( hArray =
				GlobalAlloc( GMEM_ZEROINIT, xCount * sizeof(XLOPER)) );

		retMulti.xltype |= xlbitDLLFree;
	}
	else
	{
		hArray = 0;
		retArrayPtr = resultBuffer;
	}
			                                                           
	retMulti.val.array.lparray = retArrayPtr;
	
	// at this point, we have valid data for xArray and yArray,
	// so begin the interpolations (finally)
	
	// Loop over each input x value
	for ( i=0; i<xCount; i++ )
	{
		// tempPtr points to the current element of the return data "multi" structure
		tempPtr = &retArrayPtr[i];

		// the data type will be ordinary numeric (floating point) data
		tempPtr->xltype = xltypeNum;

		if ( targPtr[i].xltype != xltypeNum )
		{
			// if the current x data value is not numeric, then it's an error
			// set the return data type to error
			tempPtr->xltype = xltypeErr;
			
			// if the current x data type is an error, pass it through
			// otherwise, set it to a #VALUE! error	
			if ( targPtr[i].xltype == xltypeErr )
				tempPtr->val.err = targPtr[i].val.err;
			else
				tempPtr->val.err = xlerrValue;
			
			// jump to the next x data value	
			continue;
		}
		
		// get the current x value
		currX = targPtr[i].val.num;
		
		// see if x is less than the x table minimum
		if ( ( sortFlag == 0 && currX <= xPtr[0].val.num ) ||
				( sortFlag == 1 && currX >= xPtr[0].val.num ) )
		{
			if ( ! extrapFlag )
			{
				// if extrapolation is not allowed, just return the first y table entry
				tempPtr->val.num = yPtr[0].val.num;
				continue;
			}
			else
			{
				// otherwise, use the first two entries to extrapolate
				lo = 0;
				hi = 1;
			}
		}
		// see if x is greater than the x table maximum
		else if ( ( sortFlag == 0 && currX >= xPtr[xSize-1].val.num ) ||
					( sortFlag == 1 && currX <= xPtr[xSize-1].val.num ) )
		{
			if ( ! extrapFlag )
			{
				// if extrapolation is not allowed, just return the last y table entry
				tempPtr->val.num = yPtr[xSize-1].val.num;
				continue;
			}
			else
			{
				// otherwise, use the last two entries to extrapolate
				lo = xSize - 2;
				hi = xSize - 1;
			}
		}
		else
		{
			// if x is within the bounds of the x table, then do a binary search
			// in the x table to find table entries that bound the x value
			lo = 0;
			hi = xSize - 1;
		    
		    // limit to 1000 loops to avoid an infinite loop problem
			for ( j=0; j<1000 && hi > lo + 1; j++ )
			{
				mid = ( hi + lo ) / 2;
				if ( ( sortFlag == 0 && currX > xPtr[mid].val.num ) ||
						( sortFlag == 1 && currX < xPtr[mid].val.num ) )
					lo = mid;
				else
					hi = mid;
			}

			// if we exceeded the max # of loops, just return a #value! error
			// for the current cell
			if ( j >= 1000 )
			{
				tempPtr->xltype = xltypeErr;
				tempPtr->val.err = xlerrValue;
				continue;
			}
		}

		// get the bounding x table values and y table values for interpolation
		xlo = xPtr[lo].val.num;
		xhi = xPtr[hi].val.num;
		ylo = yPtr[lo].val.num;
		yhi = yPtr[hi].val.num;
			
		// do the interpolation
    	tempPtr->val.num = ( currX - xlo ) / ( xhi - xlo ) * ( yhi - ylo ) + ylo;
 	}
		

done:
    
	// free the memory allocated by Excel on our behalf
	
	if ( hasXMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );
	if ( hasYMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &yMulti );
	if ( hasTargMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &targMulti );

	// if the "error" variable was set above, something significant failed
	// and we should return an error for all x targets
    if ( error != -1 )
    {
		resultBuffer->xltype = xltypeErr;
		resultBuffer->val.err = error;
		
		return (LPXLOPER) resultBuffer;
    }

	// if there was more than one x (target) value, return a multi
	// structure, otherwise just return a single XLOPER
	if ( xCount > 1 )
		return (LPXLOPER) &retMulti;
	else
		return (LPXLOPER) resultBuffer;
}


// This function is called by Excel if the xlbitDLLFree bit has been set in the
// return array of the interp function.  It allows us to free up allocated memory.

__declspec(dllexport) void xlAutoFree( LPXLOPER pxFree )
{
	if ( hArray )
	{
		GlobalUnlock( hArray );
		GlobalFree( hArray );
		hArray = 0;
	}
	return;
}

// ClipSize is a utility function that will determine the size of a "multi" array
// structure.  It checks to see if the data is organized in columns or rows (giving
// preference to columns), and ignores empty cells at the end of the array.
// It returns the size of the 1D table of valid data.

WORD ClipSize( XLOPER *multi )
{
	WORD		size, i;
	LPXLOPER	ptr;
	
	// get the number of columns in the data
	size = multi->val.array.columns;
	
	// if there's only one column, then it must be organized in multiple rows.
	if ( size == 1 )
		size = multi->val.array.rows;

	// ignore empty or error values at the end of the array.
	for ( i = size - 1; i >= 0; i-- )
	{
		ptr = multi->val.array.lparray + i;

		if ( ptr->xltype != xltypeNil )
			break;
	}
	
	return i + 1;
}
