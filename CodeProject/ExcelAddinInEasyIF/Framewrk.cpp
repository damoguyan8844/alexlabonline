///***************************************************************************
// File:	FRAMEWRK.C
//
// Purpose:	Framework library for Microsoft Excel.
//
//     This library provides some basic functions
//     that help in writing Excel DLLs. It includes
//     simple functions for managing memory with XLOPERs,
//     creating temporary XLOPERs, robustly calling
//     Excel4(), and outputting debugging strings
//     to the debugger for the current application.
//   
//     The main purpose of this library is to help
//     you to write cleaner C code for calling Excel.
//     For example, using the framework library you
//     can write
//   
//         Excel(xlcDisplay, 0, 2, TempMissing(), TempBool(0));
//   
//     instead of the more verbose
//   
//         XLOPER xMissing, xBool;
//         xMissing.xltype = xltypeMissing;
//         xBool.xltype = xltypeBool;
//         xBool.val.bool = 0;
//         Excel4(xlcDisplay, 0, 2, (LPXLOPER) &xMissing, (LPXLOPER) &xBool);
//   
//   
//     The library is non-reentrant.
//   
//     Define _DEBUG to use the debugging functions.
//   
//     Source code is provided so that you may
//     enhance this library or optimize it for your
//     own application.
// 
// Platform:    Microsoft Windows
//
// Functions:		
//              debugPrintf
//              GetTempMemory
//              FreeAllTempMemory
//              Excel
//              TempNum
//              TempStr
//              TempBool
//              TempInt
//              TempErr
//              TempActiveRef
//              TempActiveCell
//              TempActiveRow
//              TempActiveColumn
//              TempActiveColumn
//              TempMissing
//              InitFramework
//				QuitFramework
//
//***************************************************************************

#include "stdafx.h"
#include <windows.h>
#include "xlcall.h"
#include "framewrk.h"
#include "xlCoerce.h"

//
// Globals (see the comment about reentrancy 
// earlier in this file)
//

char vMemBlock[MEMORYSIZE]; // Memory for temporary XLOPERs
int vOffsetMemBlock=0;      // Offset of next memory block to allocate


#ifdef _DEBUG

///***************************************************************************
// debugPrintf()
//
// Purpose: 
//      sends a string to the debugger for the current application. 
//
// Parameters:
//
//      LPSTR lpFormat  The format definition string
//      ...             The values to print
//
// Returns: 
//
// Comments:
//
///***************************************************************************

void cdecl debugPrintf(LPSTR lpFormat, ...)
{
    char rgch[256];
    wvsprintf(rgch,lpFormat,(LPSTR)((&lpFormat)+1));
    OutputDebugString(rgch);
}

#endif /* DEBUG */

///***************************************************************************
// GetTempMemory()
//
// Purpose: 
//           Allocates temporary memory. Temporary memory
//           can only be freed in one chunk, by calling
//           FreeAllTempMemory(). This is done by Excel().
//
// Parameters:
//
//      int cBytes      How many bytes to allocate
//
// Returns: 
//
//      LPSTR           A pointer to the allocated memory,
//                      or 0 if more memory cannot be
//                      allocated. If this fails,
//                      check that you are initializing
//                      vOffsetMemBlock to 0, and check that
//                      MEMORYSIZE is big enough.
//
// Comments:
//
//        Algorithm:
//       
//             The memory allocation algorithm is extremely
//             simple: on each call, allocate the next cBytes
//             bytes of a static memory buffer. If the buffer
//             becomes too full, simply fail. To free memory,
//             simply reset the pointer (vOffsetMemBlock)
//             back to zero. This memory scheme is very fast
//             and is optimized for the assumption that the
//             only thing you are using temporary memory
//             for is to hold arguments while you call Excel().
//             We rely on the fact that you will free all the
//             temporary memory at the same time. We also
//             assume you will not need more memory than
//             the amount required to hold a few arguments
//             to Excel().
//
///***************************************************************************

LPSTR GetTempMemory(int cBytes)
{
    LPSTR lpMemory;

    if (vOffsetMemBlock + cBytes > MEMORYSIZE)
    {
        return 0;
    }
    else
    {
        lpMemory = (LPSTR) &vMemBlock + vOffsetMemBlock;
        vOffsetMemBlock += cBytes;

		/* Prevent odd pointers */
		if (vOffsetMemBlock & 1) vOffsetMemBlock++;
        return lpMemory;
    }
}

///***************************************************************************
// FreeAllTempMemory()
//
// Purpose: 
//
//          Frees all temporary memory that has been allocated
//
// Parameters:
//
// Returns: 
//
// Comments:
//
//      Unlike the usual string functions, lpstricmp
//      doesn't care about collating sequence.
//
///***************************************************************************

void FreeAllTempMemory(void)
{
    vOffsetMemBlock = 0;
}

///***************************************************************************
// Excel()
//
// Purpose: 
//         A fancy wrapper for the Excel4() function. It also
//         does the following:
//        
//          (1) Checks that none of the LPXLOPER arguments are 0,
//              which would indicate that creating a temporary XLOPER
//              has failed. In this case, it doesn't call Excel
//              but it does print a debug message.
//          (2) If an error occurs while calling Excel,
//              print a useful debug message.
//          (3) When done, free all temporary memory.
//        
//          #1 and #2 require DEBUG to be defined.
//
// Parameters:
//
//      int xlfn            Function number (xl...) to call
//      LPXLOPER pxResult   Pointer to a place to stuff the result,
//                          or 0 if you don't care about the result.
//      int count           Number of arguments
//      ...                 (all LPXLOPERs) - the arguments.
//
// Returns: 
//
//      A return code (Some of the xlret... values, as defined
//      in XLCALL.H, OR'ed together).
//
// Comments:
//
///***************************************************************************

int cdecl Excel(int xlfn, LPXLOPER pxResult, int count, ...)
{
    int xlret;

#ifdef _DEBUG
    int i;
    LPXLOPER FAR *ppx;
#endif

#ifdef _DEBUG

    ppx = (LPXLOPER FAR *) (&count + 1);
    for (i=0; i<count; i++)
    {

        if (!*ppx)
        {
            debugPrintf("Error! A zero parameter is being passed "
                "to Excel. Check memory...\r");
            FreeAllTempMemory();
            return xlretFailed;
        }

        ppx++;
    }

#endif /* DEBUG */

#ifdef _WIN32  // 32-bit file only
  if (xlCoerce == xlfn) // To work-around Excel 8 xlCoerce bug
     xlret = XLCoercev(pxResult,count,(LPXLOPER FAR *)(&count+1));
  else
     xlret = Excel4v(xlfn,pxResult,count,(LPXLOPER FAR *)(&count+1));
#else   /* End of _WIN32 */
//#ifndef _WIN16  /* WIN16 */
  xlret = Excel4v(xlfn,pxResult,count,(LPXLOPER FAR *)(&count+1));
#endif   /* End of _WIN16 */

#ifdef _DEBUG

    if (xlret != xlretSuccess)
    {
        debugPrintf("Error! Excel4(");

        if (xlfn & xlCommand)
            debugPrintf("xlCommand | ");
        if (xlfn & xlSpecial)
            debugPrintf("xlSpecial | ");
        if (xlfn & xlIntl)
            debugPrintf("xlIntl | ");
        if (xlfn & xlPrompt)
            debugPrintf("xlPrompt | ");

        debugPrintf("%u) callback failed:",xlfn & 0x0FFF);

        /* More than one error bit may be on */

        if (xlret & xlretAbort)
        {
            debugPrintf(" Macro Halted\r");
        }

        if (xlret & xlretInvXlfn)
        {
            debugPrintf(" Invalid Function Number\r");
        }

        if (xlret & xlretInvCount)
        {
            debugPrintf(" Invalid Number of Arguments\r");
        }

        if (xlret & xlretInvXloper)
        {
            debugPrintf(" Invalid XLOPER\r");
        }

        if (xlret & xlretStackOvfl)
        {
            debugPrintf(" Stack Overflow\r");
        }

        if (xlret & xlretFailed)
        {
            debugPrintf(" Command failed\r");
        }

        if (xlret & xlretUncalced)
        {
            debugPrintf(" Uncalced cell\r");
        }

    }

#endif /* DEBUG */

    FreeAllTempMemory();

    return xlret;
}


///***************************************************************************
// TempNum()
//
// Purpose: 
//          Creates a temporary numeric (IEEE floating point) XLOPER.
//
// Parameters:
//
//      double d        The value
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempNum(double d)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpx->xltype = xltypeNum;
    lpx->val.num = d;

    return lpx;
}

///***************************************************************************
// TempStr()
//
// Purpose: 
//          Creates a temporary string XLOPER
//
// Parameters:
//
//      LPSTR lpstr     The string, as a null-terminated
//                      C string, with the first byte
//                      undefined. This function will
//                      count the bytes of the string
//                      and insert that count in the
//                      first byte of lpstr. Excel cannot
//                      handle strings longer than 255
//                      characters.
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      (1) This function has the side effect of inserting
//          the byte count as the first character of
//          the created string.
//
//      (2) For highest speed, with constant strings,
//          you may want to manually count the length of
//          the string before compiling, and then avoid
//          using this function.
//
//      (3) Behavior is undefined for non-null terminated
//          input or strings longer than 255 characters.
//
///***************************************************************************

LPXLOPER TempStr(LPSTR lpstr)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpstr[0] = (BYTE) lstrlen (lpstr+1);
    lpx->xltype = xltypeStr;
    lpx->val.str = lpstr;

    return lpx;
}

///***************************************************************************
// TempBool()
//
// Purpose: 
//          Creates a temporary logical (true/false) XLOPER.
//
// Parameters:
//
//      int b           0 - for a FALSE XLOPER
//                      Anything else - for a TRUE XLOPER
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempBool(int b)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpx->xltype = xltypeBool;
    lpx->val.bool = b?1:0;

    return lpx;
}


///***************************************************************************
// TempInt()
//
// Purpose: 
//          Creates a temporary integer XLOPER.
//
// Parameters:
//
//      short int i          The integer
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempInt(short int i)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpx->xltype = xltypeInt;
    lpx->val.w = i;

    return lpx;
}

///***************************************************************************
// TempErr()
//
// Purpose: 
//          Creates a temporary error XLOPER.
//
// Parameters:
//
//      WORD err        The error code. One of the xlerr...
//                      constants, as defined in XLCALL.H.
//                      See the Excel user manual for
//                      descriptions about the interpretation
//                      of various error codes.
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempErr(WORD err)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpx->xltype = xltypeErr;
    lpx->val.err = err;

    return lpx;
}

///***************************************************************************
// TempActiveRef()
//
// Purpose: 
//           Creates a temporary rectangular reference to the active
//           sheet. Remember that the active sheet is the sheet that
//           the user sees in front, not the sheet that is currently
//           being calculated.
//
// Parameters:
//
//      WORD rwFirst    (0 based) The first row in the rectangle.
//      WORD rwLast     (0 based) The last row in the rectangle.
//      BYTE colFirst   (0 based) The first column in the rectangle.
//      BYTE colLast    (0 based) The last column in the rectangle.
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempActiveRef(WORD rwFirst, WORD rwLast, BYTE colFirst, BYTE colLast)
{
    LPXLOPER lpx;
    LPXLMREF lpmref;
    int wRet;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));
    lpmref = (LPXLMREF) GetTempMemory(sizeof(XLMREF));


    /* calling Excel() instead of Excel4() would free all temp memory! */
    wRet = Excel4(xlSheetId, lpx, 0);

    if (wRet != xlretSuccess)
    {
        return 0;
    }
    else
    {
        lpx->xltype = xltypeRef;
        lpx->val.mref.lpmref = lpmref;
        lpmref->count = 1;
        lpmref->reftbl[0].rwFirst = rwFirst;
        lpmref->reftbl[0].rwLast = rwLast;
        lpmref->reftbl[0].colFirst = colFirst;
        lpmref->reftbl[0].colLast = colLast;

        return lpx;
    }
}


///***************************************************************************
// TempActiveCell()
//
// Purpose: 
//          Creates a temporary reference to a single cell on the active
//          sheet. Remember that the active sheet is the sheet that
//          the user sees in front, not the sheet that is currently
//          being calculated.
//
// Parameters:
//
//      WORD rw         (0 based) The row of the cell.
//      BYTE col        (0 based) The column of the cell.
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Unlike the usual string functions, lpstricmp
//      doesn't care about collating sequence.
//
///***************************************************************************

LPXLOPER TempActiveCell(WORD rw, BYTE col)
{
    return TempActiveRef(rw, rw, col, col);
}

///***************************************************************************
// TempActiveRow()
//
// Purpose: 
//           Creates a temporary reference to an entire row on the active
//           sheet. Remember that the active sheet is the sheet that
//           the user sees in front, not the sheet that is currently
//           being calculated.
//
// Parameters:
//
//      WORD rw         (0 based) The row.
//
// Returns: 
//
//      int         0 if they are equal
//                  Nonzero otherwise
//
// Comments:
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
///***************************************************************************

LPXLOPER TempActiveRow(WORD rw)
{
    return TempActiveRef(rw, rw, 0, 0xFF);
}

///***************************************************************************
// TempActiveColumn()
//
// Purpose: 
//         Creates a temporary reference to an entire column on the active
//         sheet. Remember that the active sheet is the sheet that
//         the user sees in front, not the sheet that is currently
//         being calculated.
//
// Parameters:
//
//      LPSTR s     First string
//      LPSTR t     Second string
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempActiveColumn(BYTE col)
{
    return TempActiveRef(0, 0x3FFF, col, col);
}


///***************************************************************************
// TempMissing()
//
// Purpose: 
//            This is used to simulate a missing argument when
//            calling Excel(). It creates a temporary
//            "missing" XLOPER.
//
// Parameters:
//
// Returns: 
//
//      LPXLOPER        The temporary XLOPER, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER TempMissing(void)
{
    LPXLOPER lpx;

    lpx = (LPXLOPER) GetTempMemory(sizeof(XLOPER));

    if (!lpx)
    {
        return 0;
    }

    lpx->xltype = xltypeMissing;

    return lpx;
}


///***************************************************************************
// InitFramework()
//
// Purpose: 
//          Initializes all the framework functions.
//
// Parameters:
//
// Returns: 
//
// Comments:
//
// History:  Date       Author        Reason
//           2/21/97     AS            Added call to InitXLCoerce
///***************************************************************************

void InitFramework(void)
{
   vOffsetMemBlock = 0;
#ifdef _WIN32  // 32-bit file only
   InitXLCoerce();
#endif   /* End of _WIN32 */

}


///***************************************************************************
// QuitFramework()
//
// Purpose: 
//          Cleans up for all framework functions.
//
// Parameters:
//
// Returns: 
//
// Comments:
//
// History:  Date       Author        Reason
//           2/21/97     AS            Added to call TermXLCoerce
///***************************************************************************
void QuitFramework(void)
{
#ifdef _WIN32  // 32-bit file only
    TermXLCoerce();
#endif   /* End of _WIN32 */
}
