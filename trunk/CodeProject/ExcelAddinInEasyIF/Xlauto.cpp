///***************************************************************************
// File:			xlAuto.cpp
//
// Purpose:		Contains Automation Code used by xlCoerce.cpp
//
// Platform:    Microsoft Windows
//
// Functions:
//					InitXLCoerce
//					TermXLCoerce
//					GetRangeObject
//					GetValue
//					GetWorkbookObject
//					GetWorkbooksObject
//					GetWorksheetObject
//					GetWorksheetsObject
//					GetXLDisp
//					IterateRot
//					GetExcelObject
//					RunExcelMacro
//					SetIsCorrectInstanceFlag
//					SetupAutomation
//					GetValues
//					GetXLVersion
//					KeepDLLLoaded
//					OleInitXL
//					CleanupOleInitXL
//					CheckInstance
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
#include <ole2.h>
#include <oleauto.h>
#include <initguid.h>
#include <assert.h>
#include "xlcall.h"
#include "xlCoerce.h"

//---------------------------------------------------------------------------
// Function Prototypes:
//---------------------------------------------------------------------------
int KeepDLLLoaded();
HRESULT GetXLDisp(LPDISPATCH * ppdispExcel);
HRESULT IterateRot(LPDISPATCH * ppdispExcel);
HRESULT GetExcelObject(LPUNKNOWN* ppunkXL);
HRESULT GetWorkbooksObject(LPDISPATCH g_pdispXL, LPDISPATCH * ppdispWorkbooks);
BOOL OleInitXL();
int CleanupOleInitXL();
HRESULT GetValue(LPDISPATCH pdispRange,  LPVARIANT pvRange);
HRESULT GetRangeObject(LPDISPATCH pdispWS, VARIANT vA1Range,
                       LPDISPATCH * ppdispRange);
HRESULT GetWorksheetObject(LPDISPATCH pdispWorksheets,
                           VARIANT vSheetNm, LPDISPATCH * ppdispWorksheet);
HRESULT GetWorksheetsObject(LPDISPATCH pdispWorkBook,
                            LPDISPATCH * ppdispWorksheets);
HRESULT GetWorkbookObject(LPDISPATCH pdispWorkbooks,
                           VARIANT vSheetNm, LPDISPATCH * ppdispWorkbook);
int GetXLVersion(LPSTR szXLVersion);
HRESULT SetIsCorrectInstanceFlag();
HRESULT RunExcelMacro(LPDISPATCH pdispExcel, VARIANT vExcel4Macro);
HRESULT SetupAutomation();


//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------
LPDISPATCH g_pdispXL = NULL;		// Excel.Application
LPDISPATCH g_pdispWBs = NULL;		// Application.Workbooks()
BOOL g_bOleInit = FALSE;				// OLE Initialized
LCID g_lcidEnglish = MAKELANGID(LANG_ENGLISH,SUBLANG_NEUTRAL);
LPSTR g_szXLVersion = "      ";  // 6 characters long enough for xx.xa
BOOL g_bOkayToSetIFlag = FALSE;	 // if True OK to set g_bCorrectInstance
BOOL g_bCorrectInstance = FALSE; // if g_pdispXL is the pointer to current
																 // Excel instance

//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define WSZ_XLPROGID		OLESTR("Excel.Application")
#define ARRAY_SIZE		1

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define IF_FAILGO(x)	{\
							if (FAILED(x)){\
								goto Error;\
							}\
						}
#define DELETE_INTERFACE(i)	{\
								if (NULL != (i)){\
									(i)->Release();\
									(i) = NULL;\
								}\
							}\

/*********
*  InitXLCoerce
*
*  Purpose: Called from xlAutoOpen to initilize Automation
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
*			3/3/97		AJS			  Return value of KeepDLLLoaded is a
*									  Excel4 error not an HRESULT
******************************************************************************/
extern "C" HRESULT InitXLCoerce()
{
   HRESULT hr = 0;
   int iRet;
 	 // In case this gets called more than once
	 TermXLCoerce();

   // This keeps the dll loaded by registering a dummy function in the DLL
   if (xlretSuccess != (iRet = KeepDLLLoaded()))
		return E_FAIL;

	// Get the version of Excel, since the problem only occurs with Excel 8.0
	hr = GetXLVersion(g_szXLVersion);

	if (!lstrcmp(g_szXLVersion, "8.0"))
		hr = SetupAutomation();	 // Setup Automation

	return hr;
}

/*********
*  SetupAutomation
*
*  Purpose: Get's the IDispatch of Excel.Application and Workbooks collection
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT SetupAutomation()
{
	HRESULT hr;
	// This gets the LPDISPATCH for the Excel.Application
	hr = GetXLDisp(&g_pdispXL);
	if (SUCCEEDED(hr))
	{
		// Gets the Workbooks Collections
		hr = GetWorkbooksObject(g_pdispXL, &g_pdispWBs);
		if (FAILED(hr))
		{
			// Cleanup since it didn't work
			DELETE_INTERFACE(g_pdispXL);
		}
	}
	return hr;
}

/*********
*  GetXLVersion
*
*  Purpose: Fill buffer with version of Excel ex "8.0"
*
*	Parameters:
				szXLVersion[out]	Version of Excel
*
*  Return:  Value of the Excel4 call
*
*  History: Date        Author        Reason
******************************************************************************/
int GetXLVersion(LPSTR szXLVersion)
{
	int iRet;
	XLOPER xTypeNum;
	XLOPER xVersion;
	xTypeNum.xltype = xltypeInt;
	xTypeNum.val.w = 2;

	// Excel4 macro Get.Workspace(2)  see macrofun.hlp or xlmacr8.hlp for docs
	iRet = Excel4(xlfGetWorkspace, &xVersion, 1, &xTypeNum);
	if ( xlretSuccess != iRet)
		return iRet;

	if (xltypeStr == xVersion.xltype)
	{
		// Copy version string to szXLVersion
		lstrcpyn(szXLVersion, xVersion.val.str + 1, *((BYTE *) xVersion.val.str) + 1);
		szXLVersion[*((BYTE *) xVersion.val.str)] = NULL;
	}
	else
		return xlretFailed;
	// Clean up memory
	Excel4(xlFree,0,1,&xVersion);
	return iRet;
}


/*********
*  SetIsCorrectInstanceFlag
*
*  Purpose: Sets g_bCorrectInstance by calling CheckInstance through
*				Automation
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT SetIsCorrectInstanceFlag()
{
	HRESULT hr = S_OK;
	assert(g_pdispXL != NULL);

//ExecuteExcel4Macro("cALL(""XLCOERCE.XLL"",""CheckInstance"",""j"")")
	CHAR szExcel4Macro[MAX_PATH + 33];
	WCHAR wszExcel4Macro[MAX_PATH + 33];

	// Get the Name of this DLL insert it into the macro string
	// Get name of DLL
	XLOPER xDLL;
	int iRet = Excel4(xlGetName, &xDLL, 0);
	if ((xlretSuccess != iRet) || (xltypeStr != xDLL.xltype ))
		return E_FAIL; //failed
	// Copy DLL name into NULL terminated string
	int ccDLL = *((LPBYTE)xDLL.val.str);
	LPSTR szDLL = new CHAR[ccDLL + 1];
	lstrcpyn(szDLL, xDLL.val.str + 1, ccDLL + 1);
  // Create Macro String
	wsprintf(szExcel4Macro, "%s%s%s",
					"cALL(\"", szDLL, "\",\"CheckInstance\",\"j\")");

	// Free memory
	delete szDLL;
	Excel4(xlFree,0,1,&xDLL);

	// Convert Macro String into UNICODE BSTR
	if ( 0 == MultiByteToWideChar(CP_ACP, 0, szExcel4Macro,
		 -1,  wszExcel4Macro, MAX_PATH + 33))
	{
		return E_FAIL;
	}

	VARIANT vExcel4Macro;
	vExcel4Macro.bstrVal = SysAllocString(wszExcel4Macro);
	if (NULL == vExcel4Macro.bstrVal)
	{
		hr = E_FAIL;
		goto Error1;
	}
	vExcel4Macro.vt = VT_BSTR;

	// Run Macro
	g_bOkayToSetIFlag = TRUE;
	hr = RunExcelMacro(g_pdispXL, vExcel4Macro);
	g_bOkayToSetIFlag = FALSE;

	VariantClear(&vExcel4Macro);
Error1:
	return hr;
}

/*********
*  CheckInstance
*
*  Purpose: Called by automation, if in same process as the caller, sets
*				g_bCorrectInstance to TRUE.
*				Also used to keep DLL loaded by having at least one function
*				registerd.
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
int WINAPI CheckInstance()
{
	if (g_bOkayToSetIFlag) // Set in SetIsCorrectInstanceFlag
		g_bCorrectInstance = TRUE;
	return 1;
}

/*********
*  KeepDLLLoaded
*
*  Purpose: Keeps DLL Loaded, until TermXLCoerce() is called, by registering
*						one function.
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
int KeepDLLLoaded()
{
   int iRet;
	 XLOPER xRegID;
   char szType[] = "\x1" "J";		// Type Text (long)
	 char szFunc[] = "\xD" "CheckInstance";	 //Function to Register
   XLOPER xDLL, xFunc, xType;
   xFunc.xltype = xType.xltype = xltypeStr;
   xFunc.val.str = szFunc;
   xType.val.str = szType;

   // Get the name of the DLL
	 iRet = Excel4(xlGetName, &xDLL, 0);
   if ((xlretSuccess != iRet) || (xltypeStr != xDLL.xltype ))
      return xlretFailed; //failed

	 // Check to see if it is already registered
	 iRet = Excel4(xlfRegisterId, &xRegID, 2, &xDLL, &xFunc);
	 if (xlretSuccess == iRet && xltypeNum == xRegID.xltype)
		 return iRet;  //function already registered


   // Register Function
   iRet = Excel4(xlfRegister, &xRegID, 3, &xDLL, &xFunc, &xType);
   if ((xlretSuccess == iRet) && (xltypeNum == xRegID.xltype ))
      return 0; //success
   else
   {
      OutputDebugString("KeepDLLLoaded Failed\n");
      return xlretFailed; //failed
   }
}

/*********
*  GetXLDisp
*
*  Purpose: Gets Excel Dispatch Pointer from the running object table
*
*	Parameters:
*				ppdispExcel[out]	Excel's Application Dispatch Pointer
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetXLDisp(LPDISPATCH * ppdispExcel)
{
   HRESULT hr = E_FAIL;
	 assert(NULL ==	*ppdispExcel);  // Should not get called
	// Have Excel initialize OLE
	BOOL bNeedCleanup = OleInitXL();
   if (NULL != *ppdispExcel)
   {
      OutputDebugString("NULL != g_pdispXL\n");
      return hr;
   } else if (!g_bOleInit)
   {
      // Initialize OLE
			hr = OleInitialize(NULL);
      if (FAILED(hr))
      {
         OutputDebugString("OleInitialize Failed\n");
         return hr;
      } else
         g_bOleInit = TRUE;
   }

   //	Get Excel's Appliation object for correct instance of Excel
	hr = IterateRot(ppdispExcel);
	if (bNeedCleanup)
		CleanupOleInitXL();
   return hr;
}


/*********
*  IterateRot
*
*  Purpose: Gets Excel Dispatch Pointer from the running object table
*
*	Parameters:
*				ppdispExcel[out]	Excel's Application Dispatch Pointer
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT IterateRot(LPDISPATCH * ppdispExcel)
{
	HRESULT hr, hret;
	LPUNKNOWN punkXL = NULL;
	LPDISPATCH pDisp = NULL;
	OLECHAR* rgszNames[ARRAY_SIZE];
	DISPID rgDispid[ARRAY_SIZE];
	DISPPARAMS dispparams;
	EXCEPINFO excep;
	IRunningObjectTable* pIrot = NULL;
	IEnumMoniker* pem = NULL;
	IMoniker* pmon = NULL;
	ULONG uFetched;
	IBindCtx* pbc = NULL;

	// Get a pointer to XL's IUnknown.
	IF_FAILGO(hr = GetExcelObject(&punkXL));

	// Get a Dispatch pointer to Excel's Application object.
	IF_FAILGO(hr = punkXL->QueryInterface(IID_IDispatch, (LPVOID*)&pDisp));

	// Initialize the argument array.  The first element is the name
	// of the method to call.
	rgszNames[0] = OLESTR("Application");

	// Get Dispids for the method we want to call and for the
	// named arguments.
	IF_FAILGO(hr = pDisp->GetIDsOfNames(IID_NULL, rgszNames, ARRAY_SIZE,
										LOCALE_USER_DEFAULT, rgDispid));

	// Initialize the dispparams.  We are passing no arguments
	dispparams.rgvarg = NULL;
	dispparams.rgdispidNamedArgs = NULL;
	dispparams.cArgs = 0;
	dispparams.cNamedArgs = 0;

	do{
		VARIANT vdispExcel;
		VariantInit(&vdispExcel);
		if (SUCCEEDED(hr = pDisp->Invoke(rgDispid[0], IID_NULL, LOCALE_USER_DEFAULT,
									  DISPATCH_METHOD | DISPATCH_PROPERTYGET,
									  &dispparams, &vdispExcel, &excep, NULL)))
		{
			if (VT_DISPATCH == vdispExcel.vt)
			{
				*ppdispExcel = vdispExcel.pdispVal;
				hr = SetIsCorrectInstanceFlag();
				if (g_bCorrectInstance)
				{	// Correct instance of Excel
					break;
				}
			}
		}




		// Release the dispatch pointer. Since we failed, this isn't
		// the IDispatch we are looking for.
		assert(NULL != pDisp);
		DELETE_INTERFACE(pDisp);
		DELETE_INTERFACE(*ppdispExcel);

		// Have we got a pointer to the ROT etc, yet?
		if (NULL == pIrot){
			assert (NULL == pem);
			assert(NULL == pbc);
			assert(NULL == pmon);

			// Get a BindCtx so we can attach to
			// whatever object the moniker we
			// enumerate represents.  We also
			// use this interface to get a pointer
			// to the ROT.
			IF_FAILGO(hr = CreateBindCtx(0, &pbc));

			// Get a pointer to the ROT.
			IF_FAILGO(hr = pbc->GetRunningObjectTable(&pIrot));

			// Get a pointer to IEnumMoniker
			IF_FAILGO(hr = pIrot->EnumRunning(&pem));

			// Make sure we are at the beginning
			// of the list before we enum.
			IF_FAILGO(hr = pem->Reset());
		}

		// Enumerate the ROT until we get the next dispatch
		// pointer or we run out of objects.

		// As long as there is another object...
		while(S_FALSE != (hr = pem->Next(1, &pmon, &uFetched))){

			// Try to get a dispatch pointer to the object that the
			// moniker is bound to.
			hret = pmon->BindToObject(pbc, NULL, IID_IDispatch,
									  (LPVOID*)&pDisp);

			// Regardless of whether we got a dispatch pointer,
			// we can delete this moniker.
			DELETE_INTERFACE(pmon);

			// If we got a dispatch pointer, break out of the inner
			// loop and try using it to invoke our macro.
			if (SUCCEEDED(hret)){
				break;
			}
			// Otherwise, this object doesn't support IDispatch,
			// so continue enumerating the ROT to find the next
			// object that does.

		} // End while.

	}while (S_FALSE != hr);
	// Jump to here when IDispatch::Invoke call succeeds.

Error:
	// Release any interface pointers that we are holding.
	DELETE_INTERFACE(pIrot);
	DELETE_INTERFACE(pem);
	DELETE_INTERFACE(pbc);
	DELETE_INTERFACE(pDisp);
	DELETE_INTERFACE(punkXL);
	if (g_bCorrectInstance)
		return S_OK;
	else
		return E_FAIL;
}

/*********
*  GetExcelObject
*
*  Purpose: Gets an Excel Object Pointer from the running object table
*
*	Parameters:
*				ppunkXL[out]	Excel's Object Pointer
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetExcelObject(LPUNKNOWN* ppunkXL)
{
	CLSID clsid;
	HRESULT hr;

	// Get the clsid for excel from the registry by passing
	// the version independent clsid.
	IF_FAILGO(hr = CLSIDFromProgID(WSZ_XLPROGID, &clsid));

	// Get a pointer to the running version of Excel's IUnknown.
	IF_FAILGO(hr = GetActiveObject(clsid, NULL, ppunkXL));

Error:
	return hr;
}

/*********
*  GetWorkbooksObject
*
*  Purpose: Get the Workbooks collection object
*
*	Parameters:
*				pdispExcel[in]			Excel's Application Dispatch Pointer
*				ppdispWorkbooks[out]	Excel's Workbooks Collection Dispatch
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetWorkbooksObject(LPDISPATCH pdispExcel, LPDISPATCH * ppdispWorkbooks)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   DISPPARAMS dispparams = {NULL, NULL, 0, 0};

   assert(NULL != pdispExcel);

   // Get a dispid for the Workbooks method
   //
   cNames = 1;
   wszName = OLESTR("Workbooks");
   hr = pdispExcel->GetIDsOfNames(IID_NULL, &wszName, cNames, g_lcidEnglish,
                             &dispid);
   if (FAILED(hr))
     return hr;

   // Call Application.Workbooks.  The dispatch pointer to
   // the Workbooks object is returned in &vRet.
   //
   hr = pdispExcel->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,NULL, &uiArgError);
   if (FAILED(hr))
     return hr;

   // Get the dispatch pointer out of vRet.
   //
   if (VT_DISPATCH == vRet.vt)
   {
     *ppdispWorkbooks = vRet.pdispVal;
   }
   else
   {
     assert(FALSE);
     VariantClear(&vRet);
     return DISP_E_PARAMNOTFOUND;
   }

   return NOERROR;
}



/*********
*  TermXLCoerce
*
*  Purpose: Cleans up the Dispatch Pointes and uninitializes OLE
*           and unregisters CheckInstance so the DLL can be unloaded
*
*  History: Date        Author        Reason
******************************************************************************/
extern "C" VOID TermXLCoerce()
{
	// Clean up Dispatch Pointes
	if ( NULL != g_pdispWBs)
	{
		g_pdispWBs->Release();
		g_pdispWBs = NULL;
	}
	if ( NULL != g_pdispXL)
	{
		g_pdispXL->Release();
		g_pdispXL = NULL;
	}
	// Call OleUnitialize for each OleInitialize
	if (g_bOleInit)
	{
		OleUninitialize();
		g_bOleInit = FALSE;
	}

	// Unregister function
	int iRet;
	XLOPER xRegID;
	char szFunc[] = "\xD" "CheckInstance";	 //Function to Register
	XLOPER xDLL, xFunc;
	xFunc.xltype = xltypeStr;
	xFunc.val.str = szFunc;

	// Get the name of the DLL
	iRet = Excel4(xlGetName, &xDLL, 0);
	if ((xlretSuccess != iRet) || (xltypeStr != xDLL.xltype ))
		return; //failed

	// Check to see if it is already registered
	iRet = Excel4(xlfRegisterId, &xRegID, 2, &xDLL, &xFunc);
	if (xlretSuccess == iRet && xltypeNum == xRegID.xltype)
	{   //Unregister it.  Will fail if called from Class1 or Class 2 Function
		iRet = Excel4(xlfUnregister, 0, 1, (LPXLOPER) &xRegID);
	}
}

/*********
*  OleInitXL
*
*  Purpose: Makes sure that Excel is regitered in the ROT
*
*	Notes:	Excel does not register itself in the ROT at startup so
*				this function uses the Exce4 call to force Excel to
*				initialize OLE
*
*  Return:  True if need to call CleanupOleInitXL
*
*  History: Date        Author        Reason
******************************************************************************/
BOOL OleInitXL()
{
   // for Excel 95 only
   //HWND hExcelMainWnd =0;
   //hExcelMainWnd = FindWindow("XLMAIN",NULL);
   //if(!hExcelMainWnd)
   //   MessageBox(GetForegroundWindow(),"No instances of Excel running?"
	//							,"Error",MB_OK);
   //LRESULT lResult = SendMessage(hExcelMainWnd,WM_USER + 18, 0, 0);

   // works for Excel 5 for NT , Excel 95, and Excel 97
   XLOPER xTypeNum;
   xTypeNum.xltype = xltypeNum;
   xTypeNum.val.num = 6;  //New workbook with one Visual Basic module

   XLOPER xFalse;
   xFalse.xltype = xltypeBool;
   xFalse.val.bool = FALSE;

   XLOPER xTrue;
   xTrue.xltype = xltypeBool;
   xTrue.val.bool = TRUE;

	int iRet;
   XLOPER xRet;

   // Turn refresh off so that user does not see temp workbook is opened
	 Excel4(xlcEcho, 0, 1, &xFalse);
   // Open New workbook with Visual basic module to initialize OLE
	 if (xlretSuccess == (iRet = Excel4(xlcNew, &xRet, 1, &xTypeNum))
            && xltypeErr != xRet.xltype)
		return TRUE;  // need to call cleanup
	 else
		 return FALSE;
}

/*********
*  CleanupOleInitXL
*
*  Purpose: Makes sure that Excel is regitered in the ROT
*
*	Notes:	Excel does not register itself in the ROT at startup so
*				this function uses the Exce4 call to force Excel to
*				initialize OLE
*
*  Return:  True if need to call CleanupOleInitXL
*
*  History: Date        Author        Reason
******************************************************************************/
int CleanupOleInitXL()
{
   XLOPER xFalse;
   xFalse.xltype = xltypeBool;
   xFalse.val.bool = FALSE;

   XLOPER xTrue;
   xTrue.xltype = xltypeBool;
   xTrue.val.bool = TRUE;

   int iRet;

   iRet = Excel4(xlcClose, 0, 1, &xFalse);	// Close without saving
   Excel4(xlcEcho, 0, 1, &xTrue);  // Turn echo back on

   return iRet;
}


/*********
*  GetValues
*
*  Purpose: Fills pvRange with SafeArray of values from vA1Range
*
*	Parameters:
*				vWorkbook[in]  Workbook name VT_BSTR
*				vWorksheet[in] Worksheet name VT_BSTR
*				vA1Range[in]	Range in A1 notation VT_BSTR
*				pvRange[out]	SAFEARRAY variant containing Values
*
*  Return:  Excel4 return values
*
*  History: Date        Author        Reason
******************************************************************************/
int GetValues(VARIANT vWorkbook, VARIANT vWorksheet,
              VARIANT vA1Range,  LPVARIANT pvRange)
{
   int iRet = xlretSuccess;
   HRESULT hr = S_OK;
   LPDISPATCH pdispWorkbook = NULL;	 // Workbook object
   LPDISPATCH pdispWorksheets = NULL;	 // Worksheet Collections
   LPDISPATCH pdispWorksheet = NULL;	 // Worksheet object
   LPDISPATCH pdispRange = NULL;			 // Range object

   if (NULL == g_pdispXL)
      return DISP_E_UNKNOWNINTERFACE;


   if (FAILED(hr = GetWorkbookObject(g_pdispWBs, vWorkbook, &pdispWorkbook)))
      return hr;

   if (FAILED(hr = GetWorksheetsObject(pdispWorkbook, &pdispWorksheets)))
      return hr;

   if (FAILED(hr = GetWorksheetObject(pdispWorksheets, vWorksheet, &pdispWorksheet)))
      goto Error3;

   if (FAILED(hr = GetRangeObject(pdispWorksheet, vA1Range, &pdispRange)))
      goto Error2;

   if (FAILED(hr = GetValue(pdispRange, pvRange)))
      goto Error1;

Error1:
   pdispRange->Release();
Error2:
   pdispWorksheet->Release();
Error3:
   pdispWorkbook->Release();

   return hr;
}

/*********
*  GetWorkbookObject
*
*  Purpose: Gets the workbook object refered to by vWorkbook
*
*	Parameters:
*				pdispWorkbooks[in]	Excel's Workbooks Collection
*				vWorkbook[in]			Workbook name
*				ppdispWorkbook[out]	Workbook object
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetWorkbookObject(LPDISPATCH pdispWorkbooks, VARIANT vWorkbook,
                          LPDISPATCH * ppdispWorkbook)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   EXCEPINFO excepinfo;
   DISPPARAMS dispparams = {&vWorkbook, NULL, 1, 0};

   assert(NULL != pdispWorkbooks);

   // Get a dispid for the Item method
   //
   cNames = 1;
   wszName = OLESTR("Item");
   hr = pdispWorkbooks->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call Application.Workbooks.Item(vWorkbook).  The dispatch pointer to
   // the Workbook object is returned in &vRet.
   //
   hr = pdispWorkbooks->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,&excepinfo, &uiArgError);
   if (FAILED(hr))
     return hr;

   // Get the dispatch pointer out of vRet.
   //
   if (VT_DISPATCH == vRet.vt)
   {
     *ppdispWorkbook = vRet.pdispVal;
   }
   else
   {
     assert(FALSE);
     VariantClear(&vRet);
     return DISP_E_PARAMNOTFOUND;
   }

   return NOERROR;
}

/*********
*  GetWorksheetsObject
*
*  Purpose: Gets the Worksheets collection object
*
*	Parameters:
*				pdispWorkbook[in]			Workbook Object
*				ppdispWorksheets[out]	Worksheets Collection
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetWorksheetsObject(LPDISPATCH pdispWorkbook,
                            LPDISPATCH * ppdispWorksheets)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   DISPPARAMS dispparams = {NULL, NULL, 0, 0};

   assert(NULL != pdispWorkbook);

   // Get a dispid for the Worksheets method
   //
   cNames = 1;
   wszName = OLESTR("Worksheets");
   hr = pdispWorkbook->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call WB.Worksheets().  The dispatch pointer to
   // the Worksheets collection is returned in &vRet.
   //
   hr = pdispWorkbook->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,NULL, &uiArgError);
   if (FAILED(hr))
     return hr;

   // Get the dispatch pointer out of vRet.
   //
   if (VT_DISPATCH == vRet.vt)
   {
     *ppdispWorksheets = vRet.pdispVal;
   }
   else
   {
     assert(FALSE);
     VariantClear(&vRet);
     return DISP_E_PARAMNOTFOUND;
   }

   return NOERROR;
}


/*********
*  GetWorksheetObject
*
*  Purpose: Gets Worksheet object pointed to by vSheetNm
*
*	Parameters:
*				pdispWorksheets[in]	Worksheets Collection
*				vWorksheet[in]			Worksheet name
*				ppdispWorksheet[out]	Worksheet object
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetWorksheetObject(LPDISPATCH pdispWorksheets,
                           VARIANT vWorksheet, LPDISPATCH * ppdispWorksheet)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   DISPPARAMS dispparams = {&vWorksheet, NULL, 1, 0};

   assert(NULL != pdispWorksheets);

   // Get a dispid for the Item method
   //
   cNames = 1;
   wszName = OLESTR("Item");
   hr = pdispWorksheets->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call Worksheets.Item(vWorksheet).  The dispatch pointer to
   // the Worksheet object is returned in &vRet.
   //
   hr = pdispWorksheets->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,NULL, &uiArgError);
   if (FAILED(hr))
     return hr;

   // Get the dispatch pointer out of vRet.
   //
   if (VT_DISPATCH == vRet.vt)
   {
     *ppdispWorksheet = vRet.pdispVal;
   }
   else
   {
     assert(FALSE);
     VariantClear(&vRet);
     return DISP_E_PARAMNOTFOUND;
   }

   return NOERROR;
}


/*********
*  GetRangeObject
*
*  Purpose: Gets Range object
*
*	Parameters:
*				pdispWorksheet[in]	Worksheet object
*				vA1Range[in]			Range in A1 notation
*				ppdispRange[out]		Range object
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetRangeObject(LPDISPATCH pdispWorksheet, VARIANT vA1Range,
                       LPDISPATCH * ppdispRange)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   DISPPARAMS dispparams = {&vA1Range, NULL, 1, 0};

   assert(NULL != pdispWorksheet);

   // Get a dispid for the Range method
   //
   cNames = 1;
   wszName = OLESTR("Range");
   hr = pdispWorksheet->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call Range(vA1Range).  The dispatch pointer to
   // the Range object is returned in &vRet.
   //
   hr = pdispWorksheet->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,NULL, &uiArgError);
   if (FAILED(hr))
     return hr;

   // Get the dispatch pointer out of vRet.
   //
   if (VT_DISPATCH == vRet.vt)
   {
     *ppdispRange = vRet.pdispVal;
   }
   else
   {
     assert(FALSE);
     VariantClear(&vRet);
     return DISP_E_PARAMNOTFOUND;
   }

   return NOERROR;
}


/*********
*  GetValue
*
*  Purpose: Fills psaRange with values in Range Object
*
*	Parameters:
*				pdispRange[in]		Range object
*				pvRange[out]		Values from worksheet
*
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
HRESULT GetValue(LPDISPATCH pdispRange, LPVARIANT pvRange)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   HRESULT hr = S_OK;
   DISPPARAMS dispparams = {NULL, NULL, 0, 0};

   assert(NULL != pdispRange);

   // Get a dispid for the Value method
   //
   cNames = 1;
   wszName = OLESTR("Value");
   hr = pdispRange->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call Range.Value.  The values from the
   // the range object are returned in &vRet.
   //
   hr = pdispRange->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                      &dispparams, pvRange,NULL, &uiArgError);
   if (FAILED(hr))
     return hr;

   return NOERROR;
}


/*********
*  RunExcelMacro
*
*  Purpose: Runs Excel Macro
*
*	Parameters:
*				pdispExcel[in]		Excels Application Object
*				vExcel4Macro[in]	VT_BSTR of Macro to run
*  Return:  HRESULT
*
*  History: Date        Author        Reason
******************************************************************************/
//ExecuteExcel4Macro("cALL(""XLCOERCE.XLL"",""CheckInstance"",""j"")")
HRESULT RunExcelMacro(LPDISPATCH pdispExcel, VARIANT vExcel4Macro)
{
   OLECHAR* wszName;
   UINT cNames, uiArgError;
   DISPID dispid;
   VARIANTARG vRet;
   HRESULT hr = S_OK;
   EXCEPINFO excepinfo;
   DISPPARAMS dispparams = {&vExcel4Macro, NULL, 1, 0};
   assert(NULL != pdispExcel);

   // Get a dispid for the ExecuteExcel4Macro method
   //
   cNames = 1;
   wszName = OLESTR("ExecuteExcel4Macro");
   hr = pdispExcel->GetIDsOfNames(IID_NULL, &wszName, cNames,
                           g_lcidEnglish, &dispid);
   if (FAILED(hr))
     return hr;

   // Call Application.ExecuteExcel4Macro(vExcel4Macro)
   //
   hr = pdispExcel->Invoke(dispid, IID_NULL, g_lcidEnglish,
                      DISPATCH_PROPERTYGET,
                      &dispparams, &vRet,&excepinfo, &uiArgError);

   if (FAILED(hr))
     return hr;

   return NOERROR;
}


