///***************************************************************************
// File:			xlCoerce.cpp
//
// Purpose:		Code to implement a work-around for a bug in Microsoft Excel
//					97 (version 8).  This is written to only use the work-around 
//					for version 8.  Any later or earlier versions will use the
//					origional Excel4 call.  If this bug is not fixed by 
//					version 8.0a, then the XLCoerce function will have to be 
//					re-written to use the work-around for version 8.0 and 8.0a
//
//	Bug Description:
//					Using xlCoerce to coerce a reference containing any text, 
//					will cause Excel to Crash if the DLL function is a custom 
//					worksheet function, i.e. it is called as a result of a 
//					recalculation from a worksheet. Excel4(xlCoerce,.. works
//					fine if called in a macro function DLL.
//
//	To Implement:
//					Include xlCoerce.cpp and xlAuto.cpp into the project
//					Place framewrk.h and xlcoerce.h in the project's directory
//
//					Include xlCoerce.h into any source file that uses XLCoercev
//					or XLCoerce .
//
//					Place CheckInstance in the EXPORTS section of the DEF file
//
//					If using XLCoercev or XLCoerce directly without the Framework:
//						In the xlAutoOpen call InitXLCoerce()
//						In the xlAutoClose call TermXLCoerce()
//
//						Replace the Excel4(xlCoerce,... calls with a call to XLCoerce
//						Example:
//								Excel4(xlCoerce, &xArray, 2, pxRange, &xType);
//							with
//								XLCoerce(&xArray, 2, pxRange, &xType);
//					
//					If using this with the Framework:
//						In the xlAutoOpen call InitFramework()
//						In the xlAutoClose call QuitFramework()
//
//						Then just call the Excel() function as normal.
// 
// Platform:    Microsoft Windows
//
// Functions:		
//					WorksheetFunction
//					RangeContainsText
//					XLCoerce
//					ConvertVariantToxltypeMulti
//					AutomationXLCoerce
//					ConvertREFtoVARIANTS
//					ConvertVariantToXLOPER
//					CreatexltypeStr
//					ParseSheetNm
//					XLFree
//
// Development: Andy Steenson
//					 Microsoft Technical Support
//
// Copyright (c) 1997 Microsoft Corporation. All rights reserved.
///***************************************************************************

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "xlcall.h"
#include "xlcoerce.h"

//---------------------------------------------------------------------------
// Function Prototypes:
//---------------------------------------------------------------------------
BOOL WorksheetFunction();
BOOL RangeContainsText(LPXLOPER pxRange);
int AutomationXLCoerce(LPXLOPER pxResult, LPXLOPER pxType, LPXLOPER pxRef);
int ConvertREFtoVARIANTS(  LPXLOPER pxRef,
                           LPVARIANT pvWorkbook, LPVARIANT pvWorksheet, 
                           LPVARIANT pvA1Range);
int GetValues(VARIANT vWorkbook, VARIANT vWorksheet, 
              VARIANT vA1Range, LPVARIANT pvRange);
int ParseSheetNm(LPXLOPER pxRef, 
                 LPVARIANT pvWorkbook, LPVARIANT pvWorksheet);
int ConvertVariantToXLOPER(LPXLOPER pxResult, LPXLOPER pxType, VARIANT vRange);
int CreatexltypeStr(BSTR bstrVal, LPXLOPER pxResult);
int FreexltypeStr(LPXLOPER pxResult);
HRESULT ConvertVariantToxltypeMulti(LPXLOPER pxResult, VARIANT vRange);
int XLFree(LPXLOPER pxArray);


//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------
extern BOOL g_bCorrectInstance;
extern LPSTR g_szXLVersion;

/*********
*  XLCoerce
*
*  Purpose: Uses Excel4(xlCoerce, ... if range does not contain Text
*           uses OLE Automation if range does contain text
*
*	Parameters:
*				pxRes[out]		parameter containing Coerced data
*				pxSource[in]	XLOPER to coerce.
*				pxType[in]		Type to coerce pxSource to.
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
******************************************************************************/
extern "C" int far _cdecl XLCoerce(LPXLOPER pxRes, int count,... )
{
	return XLCoercev(pxRes,count,(LPXLOPER FAR *)(&count+1));
}

/*********
*  XLCoercev
*
*  Purpose: Uses Excel4(xlCoerce, ... if range does not contain Text
*           uses OLE Automation if range does contain text
*
*	Parameters:
*				pxRes[out]		parameter containing Coerced data
*				opers[0]      pxSource[in]	XLOPER to coerce.
*				opers[1]      pxType[in]		Type to coerce pxSource to.
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
				3/19/97		AJS			  Setting pxType incorrectly
******************************************************************************/
extern "C" int WINAPI XLCoercev(LPXLOPER pxRes, int count, LPXLOPER far opers[])
{
	int newCount = count;
	int iRet = xlretSuccess;
    XLOPER xType;
	// Get optional parameters	If only 1 parameter, use xType as second parameter
	LPXLOPER pxSource, pxType;
	if (2 == newCount && xltypeInt == opers[1]->xltype)
		pxType = opers[1];
	else if (1 == newCount)
	{
		pxType = &xType;
		pxType->xltype = xltypeMissing;
		newCount = 2;
	} else 
		return xlretInvCount;
 	pxSource = opers[0];
  
	if((xltypeSRef == pxSource->xltype 	  //
		  || xltypeRef == pxSource->xltype)  // Only if a Reference
		&& (xltypeMulti == pxType->val.w 	  // Only if conveting to a multi
		 || xltypeStr == pxType->val.w		  // a string or
		 || xltypeMissing == pxType->xltype)  // a default conversion
		&& !lstrcmp(g_szXLVersion,"8.0")   // Only for Version 8.0
		&&	WorksheetFunction()			//Only if called as worksheet function
		&& RangeContainsText(pxSource))	  // Only if the range contains Text
   {
    // ?Using the correct instance of Excel for Automation
		if (g_bCorrectInstance)	
		{
			XLOPER xRef;
			XLOPER xRefType;
			xRefType.xltype = xltypeInt;
			xRefType.val.w = xltypeRef; 
			iRet = Excel4(xlCoerce, &xRef, 2, pxSource, &xRefType);
			if (xlretSuccess == iRet)
         {
				iRet = AutomationXLCoerce(pxRes, pxType, &xRef);
			   Excel4(xlFree,0,1, &xRef);
         }
		} else  // Caused by Multiple intances of Excel running
		{
			static BOOL bAlreadyDone = FALSE;
			//Only notify the user once, if ignored then pxRes will be
			//be an error
			if (!bAlreadyDone)	
			{
				MessageBox(GetActiveWindow(),
				"Unable to complete calculation with "
				"other instances of Excel Opened\n"
				"Please close all other instances of Excel, then Click OK",
				"Error",	MB_OK);
				bAlreadyDone = TRUE;
				InitXLCoerce();
				iRet = XLCoercev(pxRes, count, opers);
			} else
				iRet = xlretFailed;
		}
   }
   else	 // If we don't have to worry about the bug, call the normal xlCoerce
      iRet = Excel4(xlCoerce,pxRes, newCount, pxSource, pxType);

   return iRet;
}


/*********
*  WorksheetFunction
*
*  Purpose: Determines if a function is called as a result of calculation
*
*  Return:  TRUE  -  Worksheet function
*           FALSE -  Not a worksheet function
*
*  History: Date        Author        Reason
******************************************************************************/
BOOL WorksheetFunction()
{
	XLOPER xResult;
	int iRet = Excel4(xlcActivate, &xResult,0);
	if (xlretSuccess != iRet      // Class 1 function
		 || !xResult.val.bool)		// Class 2 function
		return TRUE;		  //Worksheet Function
	else
		return FALSE;		  // Command Function
}


/*********
*  RangeContainsText
*
*  Purpose: Determine if a XLOPER Reference points to a range that contains
*           any cells containing text.  Excel 97 will crash if xlCoerce is
*           used to convert a reference to a string.
*
*	Parameter:
*				pxRange[in]		Range to check.
*
*  Return:  TRUE  -  Range contains text, or Excel4 call fails.
*           FALSE -  Range does not contain text and it is safe to use 
*                    xlCoerce on the reference.  For bug in Excel 97.
*
*  History: Date        Author        Reason
******************************************************************************/
BOOL RangeContainsText(LPXLOPER pxRange)
{
  DWORD dwRet = 0;
  XLOPER xIsText;
  BOOL bRet = TRUE;  // Value returned if Excel4 Fails

  // Check to make sure we are working with a range
  if( !((xltypeSRef | xltypeRef) & (pxRange->xltype)))
    return bRet;

  dwRet = Excel4(xlfIstext,&xIsText,1,pxRange);
	// Get an array of xltypeBool XLOPERS 
  if( xlretSuccess == dwRet && xltypeErr != xIsText.xltype)
  {
		XLOPER xRet;
    dwRet = Excel4(xlfOr,&xRet,1,&xIsText);
    if( xlretSuccess == dwRet && xltypeBool == xRet.xltype)
      bRet = xRet.val.bool;
  }
  Excel4(xlFree,0,1,&xIsText);
  return bRet;
}



/*********
*  AutomationXLCoerce
*
*  Purpose: Uses Automation to get the values in the cells referenced
*           by pxRef.
*
*	Parameters:
*				pxResult[out]		parameter containing Coerced data
*				pxType[in]			Type to coerce pxSource to.
*				pxRef[in]			Reference to coerce.
*
*  Return:  Excel4 return values
*
*  History: Date        Author        Reason
******************************************************************************/
int AutomationXLCoerce(LPXLOPER pxResult, LPXLOPER pxType, LPXLOPER pxRef)
{
   int iRet = xlretSuccess;
   
   // does not support disjoint references
   if ( 1 != pxRef->val.mref.lpmref->count)
   {
      iRet = xlretFailed;
      return iRet;
   }

   VARIANT vWorkbook,	 //Workbook name
					 vWorksheet, //Worksheet name
					 vA1Range, 	 //Range address in A1 notation
					 vRange;		 //Will hold the values of contents of Range

   // Converts XLOPER reference to variants for use with Automation
	 iRet = ConvertREFtoVARIANTS(pxRef, &vWorkbook, &vWorksheet, &vA1Range);
   
   if (xlretSuccess != iRet)
      return iRet;

   VariantInit(&vRange);
   // Gets Values of Range into vRange
	 iRet = GetValues(vWorkbook, vWorksheet, vA1Range,  &vRange);
   if (SUCCEEDED(iRet))
   {
     // Converts Variant to XLOPER 
		 ConvertVariantToXLOPER(pxResult,pxType, vRange);
     VariantClear(&vRange);
   }
   // Clean up memory
	 VariantClear(&vWorkbook);
   VariantClear(&vWorksheet);
   VariantClear(&vA1Range);
   return iRet;
}


/*********
*  ConvertXLREFtoVARIANTS
*
*  Purpose: Converts an XLREF to variants
*
*	Parameters:
*				pxRef[in]			Reference to range
*				pvWorkbook[out]	Variant containing Workbook name
*				pvWorksheet[out]	Variant containing Worksheet name
*				pvA1Range[out]		Variant containing Reference in A1 notation
*
*  Return:  Excel4 return values
*
*  History: Date        Author        Reason
******************************************************************************/
int ConvertREFtoVARIANTS(  LPXLOPER pxRef,
                           LPVARIANT pvWorkbook, LPVARIANT pvWorksheet, 
                           LPVARIANT pvA1Range)
{
   assert(xltypeRef == pxRef->xltype);

   int iRet = xlretSuccess;
	 LPXLREF pxlref = pxRef->val.mref.lpmref->reftbl;
   
	 LPSTR szA1Ref = new char[26];
   LPWSTR wszA1Ref = new WCHAR[26];
   UINT iCnt = 0;

   // Converts Row/Column indexes to A1 Notation
	 if (pxlref->colFirst >= 26) // if column is 2-characters
   {
      szA1Ref[iCnt] = (char)('a' + pxlref->colFirst / 26 - 1);
      iCnt++;
   }
   wsprintf(szA1Ref + iCnt, "%c%u:",
                     char('a' + pxlref->colFirst % 26),	//Column
                     (UINT) (pxlref->rwFirst + 1));			//Row
   iCnt = lstrlen(szA1Ref);

   if (pxlref->colLast >= 26)	// if column is 2-characters
   {
      szA1Ref[iCnt] = (char)('a' + pxlref->colLast / 26 - 1);
      iCnt++;
   }
   
   wsprintf(szA1Ref + iCnt, "%c%u",
                     char('a' + pxlref->colLast % 26),	//Column
                     UINT(pxlref->rwLast + 1));					//Row
   
   // Change to UNICODE
	 if ( 0 == MultiByteToWideChar(CP_ACP, 0, szA1Ref, 
         -1,  wszA1Ref, 52))
   {
      iRet = xlretFailed;
      return iRet;
   }
   
   // Copy to BSTR
	 pvA1Range->bstrVal = SysAllocString(wszA1Ref);
   if ( NULL == pvA1Range-> bstrVal)
   {
      iRet = xlretFailed;
      return iRet;
   }
   pvA1Range->vt = VT_BSTR;

   // Get Workbook and Worksheet Name
	 iRet = ParseSheetNm(pxRef, pvWorkbook, pvWorksheet);

   return iRet;
}


/*********
*  ParseSheetNm
*
*  Purpose: Retrieves the Workbook and worksheet name.
*
*	Parameters:
*				pxRef[in]			Reference to range
*				pvWorkbook[out]	Variant containing Workbook name
*				pvWorksheet[out]	Variant containing Worksheet name
*
*  Return:  Excel4 return values
*
*  History: Date        Author        Reason
******************************************************************************/
int ParseSheetNm(LPXLOPER pxRef, 
                 LPVARIANT pvWorkbook, LPVARIANT pvWorksheet)
{
   int iRet = xlretSuccess;
   XLOPER xSheetNm;
   int cbSheetNm = 0;
   LPOLESTR wszWorkbook = NULL;
   LPOLESTR wszWorksheet = NULL;


   // Gets Workbook and sheet name ex: [book1.xls]sheet1
	 iRet = Excel4(xlSheetNm, &xSheetNm, 1, pxRef);
   if (xlretSuccess != iRet || !(xltypeStr & xSheetNm.xltype))
   {
      if(xlretSuccess == iRet)
         iRet = xlretFailed;
      goto Error1;
   }
   // Number of characters in name
	 cbSheetNm = *((LPBYTE)xSheetNm.val.str) ;  // not null terminated
   
	 int cbWorkbook, cbWorksheet;

   // Get number of characters in Workbook name ex book1.xls
	 for ( cbWorkbook = 0; 
         *(xSheetNm.val.str + cbWorkbook + 2) != ']' 
            && cbWorkbook < cbSheetNm;
         cbWorkbook++);
   
	 // Get number of charactes in Worksheet name ex: sheet1
	 cbWorksheet = cbSheetNm - cbWorkbook - 2;

   // Copy worksheet and workbook name into Unicode buffers
	 wszWorkbook = new OLECHAR[(cbWorkbook + 1) * 2];
   if (NULL == wszWorkbook)
   {
      iRet = xlretFailed;
      goto Error1;
   }
   
   wszWorksheet = new OLECHAR[(cbWorksheet + 1) * 2];
   if (NULL == wszWorksheet)
   {
      iRet = xlretFailed;
      goto Error2;
   }
   if ( 0 == MultiByteToWideChar(CP_ACP, 0, (xSheetNm.val.str + 2), 
         cbWorkbook, wszWorkbook, (cbWorkbook + 1) * 2))
   {
      iRet = xlretFailed;
      goto Error3;
   }
   if ( 0 == MultiByteToWideChar(CP_ACP, 0, (xSheetNm.val.str + 3 + cbWorkbook), 
         cbWorksheet, wszWorksheet, (cbWorksheet + 1) * 2))
   {
      iRet = xlretFailed;
      goto Error3;
   }
   
   // Null terminate string
	 wszWorkbook[ cbWorkbook ] = NULL;
   // Copy into BSTR
	 pvWorkbook->bstrVal = SysAllocString(wszWorkbook);
   if ( NULL == pvWorkbook-> bstrVal)
   {
      iRet = xlretFailed;
      goto Error3;
   }
   pvWorkbook->vt = VT_BSTR;

   // Null terminate string
   wszWorksheet[ cbWorksheet ] = NULL;
   // Copy into BSTR
   pvWorksheet->bstrVal = SysAllocString(wszWorksheet);
   if ( NULL == pvWorksheet-> bstrVal)
   {
      iRet = xlretFailed;
      VariantClear(pvWorkbook);
      goto Error3;
   }
   pvWorksheet->vt = VT_BSTR;

Error3:
   delete wszWorksheet;
Error2:
   delete wszWorkbook;
Error1:
   Excel4(xlFree,0,1,&xSheetNm);

   return iRet;
}


/*********
*  ConvertVariantToXLOPER
*
*  Purpose: Coverts a variant containing a BSTR to an xltypeStr, 
*           or a SafeArray to an xltypeMulti XLOPER
*
*	Parameters:
*				pxResult[out]		XLOPER containg range values
*				pxType[in]			Type of XLOPER to create
*				vRange[in]			Variant containing either String or SafeArray
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
******************************************************************************/
int ConvertVariantToXLOPER(LPXLOPER pxResult, LPXLOPER pxType, VARIANT vRange)
{
   int iRet = xlretSuccess;
   XLOPER xResult;

   //Case 1 BSTR Variant
   if (VT_BSTR == vRange.vt)
      iRet = CreatexltypeStr(vRange.bstrVal, &xResult);
   else // Case 2 Array of Variants
   {
      if ( (VT_VARIANT | VT_ARRAY) == vRange.vt)
				 // Creates xltypeMulti from SafeArray
         iRet = ConvertVariantToxltypeMulti(&xResult, vRange);
      else
         iRet = xlretFailed;
   }
   if (SUCCEEDED(iRet))
	{
		// Convert an xltypeMulti to xltypeMulti so that Excel4(xlFree,...
		// may be used to clean up the memory, and XLFree can clean
		// up XLCoerce created memory
		iRet = Excel4(xlCoerce,pxResult, 2, &xResult, pxType);
		XLFree(&xResult);
	}
	return iRet;
}


/*********
*  CreatexltypeStr
*
*  Purpose: Copies a BSTR into an XLOPER
*
*	Parameters:
*				bstrVal[in]
*				pxResult[out]  XLOPER 
*					
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
*           2/3/96      AS            Bug in Excel 97
******************************************************************************/
int CreatexltypeStr(BSTR bstrVal, LPXLOPER pxResult)
{
   int iRet = xlretSuccess;
   UINT ccString = SysStringLen(bstrVal);
   pxResult->val.str = new char[ccString + 1];
   if (NULL == pxResult->val.str)	// Did new work
      return xlretFailed;
   else
   {
      // XLOPER strings are byte counted
		  pxResult->val.str[0] = (BYTE) ccString;
      // Copy UNICODE BSTR to MultiByte xltpeStr
			if (!WideCharToMultiByte(CP_ACP, 0, bstrVal, 
         ccString, pxResult->val.str + 1, ccString + 1, NULL, NULL))
      {
         // WideCharToMultiByte Failed, cleanup memory
				 delete pxResult->val.str;
         return xlretFailed;
      }
			pxResult->xltype = xltypeStr;
   }
   return iRet;
}


/*********
*  ConvertVariantToxltypeMulti
*
*  Purpose: Points pxResult to an array of XLOPERs filled with the
*           values in vRange 
*
*	Parameters:
*				pxResult[out]		XLOPER containg range values
*				vRange[in]			Variant containing either String or SafeArray
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
*           4/25/97     AJS           Implemented VT_DATE conversion
******************************************************************************/
HRESULT ConvertVariantToxltypeMulti(LPXLOPER pxResult, VARIANT vRange)
{
	HRESULT hr = S_OK;
	LPVARIANT HUGEP pvArray = NULL;		// pointer to SafeArray
	WORD wRows = (WORD) vRange.parray->rgsabound[1].cElements;
	WORD wColumns = (WORD) vRange.parray->rgsabound[0].cElements;
 
	// Array to put values into
	LPXLOPER pxArray = new XLOPER[wRows * wColumns];
	if (NULL == pxArray)
	{
		hr = E_FAIL;
		goto Error1;
	}

	// Get pointer to SafeArray's Data
	hr = SafeArrayAccessData(vRange.parray, (void HUGEP* FAR*)&pvArray);
	if (FAILED(hr))
		goto Error2;

	// Setup XLOPER as xltypeMulti
	pxResult->xltype = xltypeMulti;
	pxResult->val.array.rows = wRows;
	pxResult->val.array.columns = wColumns;
	pxResult->val.array.lparray = pxArray;

	// Go through SafeArray and fill XLOPER Array
	WORD wRowIndex, wColumnIndex;
	WORD wsaIndex, wxlIndex;
	for (wRowIndex = 0; wRowIndex < wRows; wRowIndex++)
	for (wColumnIndex = 0; wColumnIndex < wColumns; wColumnIndex++)
	{
		//Index for XLOPER Array
		wxlIndex = wColumnIndex + wRowIndex * wColumns;
		//Index for SAFEARRAY  Transposed from XLOPER Array
		wsaIndex = wRowIndex + wColumnIndex * wRows;
		// Determine type of XLOPER
		switch (pvArray[wsaIndex].vt)
		{
		case VT_I2:
			pxArray[wxlIndex].xltype = xltypeInt;
			pxArray[wxlIndex].val.w = pvArray[wsaIndex].iVal;
			break;
		case VT_R8:
			pxArray[wxlIndex].xltype = xltypeNum;
			pxArray[wxlIndex].val.num = pvArray[wsaIndex].dblVal;
			break;
		case VT_BSTR:
			hr = CreatexltypeStr(pvArray[wsaIndex].bstrVal, pxArray + wxlIndex);
			break;
      case VT_DATE:
			pxArray[wxlIndex].xltype = xltypeNum;
			pxArray[wxlIndex].val.num = pvArray[wsaIndex].date;
         break;      
      case VT_ERROR:
			pxArray[wxlIndex].xltype = xltypeErr;
			pxArray[wxlIndex].val.err = xlerrName;
			break;
		case VT_EMPTY:
			pxArray[wxlIndex].xltype = xltypeNil;
			break;
		default:
			// should never hit here
			assert(FALSE);
		}
	}
	SafeArrayUnaccessData(vRange.parray);
	goto NoError;

Error2:
	delete pxArray;
Error1:
NoError:
	return hr;
}

/*********
*  XLFree
*
*  Purpose: Frees a XLOPER created with XLCoerce
*
*	Parameters:
*				pxArray[in]	XLOPER that points to memory to free
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
******************************************************************************/
int XLFree(LPXLOPER pxArray)
{
	 int iRet = xlretFailed;
	 if (xltypeStr == pxArray->xltype)
   {
      // String XLOPER
		  delete pxArray->val.str;
      iRet = xlretSuccess;
	 }
   else if (xltypeMulti == pxArray->xltype)
	 {
			// Array of XLOPER's, need to iterate through array
			WORD wRows = pxArray->val.array.rows;
			WORD wColumns = pxArray->val.array.columns;
			WORD wRowIndex, wColumnIndex, wIndex;
			for (wRowIndex = 0; wRowIndex < wRows; wRowIndex++)
			for (wColumnIndex = 0; wColumnIndex < wColumns; wColumnIndex++)
			{
				wIndex = wColumnIndex + wRowIndex * wColumns;
				// String is only XLOPER type that needs to be delete
				if (xltypeStr == pxArray->val.array.lparray[wIndex].xltype)
					delete pxArray->val.array.lparray[wIndex].val.str;
			}
			// Delete the Array of XLOPERs
			delete pxArray->val.array.lparray;
			pxArray->xltype = xltypeStr;
			iRet = xlretSuccess;
	}
	return iRet;
}

