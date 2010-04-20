// EMX.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "xlcall.h"
#include "framewrk.h"
#include "EMX.h"
#include "cfgopts.h"
#include "mtrxsel.h"
#include "PswDlg.h"
#include "gridwiz.h"
#include "gridwizchooser.h"
#include "gridfromcellwiz.h"
#include "gfrsetup.h"
#include "mtxedit.h"

#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND g_hExcelHWnd = NULL;
static HINTERNET s_hIC = NULL;

BOOL CheckMatrixAccessRights(Matrix *pMatrix);
void ParseStringVars(MatrixRow *pInputRow, MatrixRow *pMatchRow, CString &cStrResult);
void ReplaceAllInstances(CString &cStrResult, char *cpFind, char *cpReplace);
LPXLOPER __stdcall EasyEval(LPSTR cpEvalStr);

void DoGridWizCurrentCell();
void DoGridWizInsertGrid();
void DoGridWizGridFromSelection();
BOOL CellCannotBeBlank(int iRow, int iCol);

//
// Structure to pass the the EnumWindowsProc
//
typedef struct _EnumStruct {
	HWND		hwnd;   // Return value for Excel's main hWnd
	unsigned short	wLoword; //Contains LowWord of Excel's main hWnd
} EnumStruct;

#define CLASS_NAME_BUFFER	50

BOOL CALLBACK EnumProc(HWND hwnd, EnumStruct * pEnum);
BOOL GetHwnd(HWND * pHwnd);

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CEMXApp

BEGIN_MESSAGE_MAP(CEMXApp, CWinApp)
	//{{AFX_MSG_MAP(CEMXApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMXApp construction

CEMXApp::CEMXApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEMXApp object

CEMXApp theApp;


static char cpRegKey[] = "Software\\Webogy\\EasyIF";
static char cpForceRecalc[] = "ForceRecalculation";
static char cpRecalcInterval[] = "RecalculationInterval";

BOOL CEMXApp::InitInstance()
{
  InitFramework();

  m_bRecalcOnChange = TRUE;

  HKEY hRegKey;

  m_dwRecalcInterval = 1000;

  if (!::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                      cpRegKey,
                      0,
                      KEY_READ,
                      &hRegKey))
    {
      char caBuf[81];
      DWORD dwSize = sizeof(caBuf)-1;
      memset(caBuf,0,sizeof(caBuf));
      ::RegQueryValueEx(hRegKey,cpForceRecalc,NULL,NULL,(unsigned char *)caBuf,&dwSize);

      if (caBuf[0] == 0 || caBuf[0] == '1')  // not found or 1
        m_bRecalcOnChange = TRUE;
      else
        m_bRecalcOnChange = FALSE;

      dwSize = sizeof(caBuf)-1;
      memset(caBuf,0,sizeof(caBuf));
      ::RegQueryValueEx(hRegKey,cpRecalcInterval,NULL,NULL,(unsigned char *)caBuf,&dwSize);

      if (caBuf[0])
        m_dwRecalcInterval = (DWORD)atol(caBuf);

      ::RegCloseKey(hRegKey);
    }

  m_pFloatingGridsThread = NULL;
  m_pHiddenComWnd = NULL;

  return CWinApp::InitInstance();
}



int CEMXApp::ExitInstance()
{
  if (s_hIC)
    InternetCloseHandle(s_hIC);

  SaveSettings();

	QuitFramework();

	return CWinApp::ExitInstance();
}



void CEMXApp::SaveSettings()
{
  HKEY hRegKey;
  DWORD dw;

  if (!::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                        cpRegKey,
                        0,
                        REG_NONE,
                        REG_OPTION_NON_VOLATILE,
                        KEY_READ | KEY_WRITE,
                        NULL,
                        &hRegKey,
                        &dw))
    {
      char caBuf[81];

      caBuf[0] = (m_bRecalcOnChange ? '1' : '0');
      caBuf[1] = 0;

      DWORD dwSize = strlen(caBuf);
      ::RegSetValueEx(hRegKey,cpForceRecalc,NULL,REG_SZ,(unsigned char *)caBuf,dwSize);

      ltoa((long)m_dwRecalcInterval,caBuf,10);
      dwSize = strlen(caBuf);
      ::RegSetValueEx(hRegKey,cpRecalcInterval,NULL,REG_SZ,(unsigned char *)caBuf,dwSize);

      ::RegCloseKey(hRegKey);
    }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/// INTERNATIONALIZE
// proc, type_text, function_text, arg, macro_type, category,
// shortcut_text, help_topic, function_help
static LPSTR cpExcelFunctions[][12] = {
{" EasyIFToggleGridPad", " I", " EasyIFToggleGridPad", " ", " 0", " EasyIF", " ", " ", " Show/Hide GridPad Palette ", " Show/Hide GridPad Palette "},
{" EasyIFConfig", " I", " EasyIFConfig", " ", " 0", " EasyIF", " ", " ", " Display the configuration dialog ", " Display the configuration dialog "},
{" EasyIFGridWiz", " I", " EasyIFGridWiz", " ", " 0", " EasyIF", " ", " ", " Invoke Wizard ", " Invoke Wizard "},
{" EasyIFMaint", " I", " EasyIFMaint", " ", " 0", " EasyIF", " ", " ", " Display the grid maintenance dialog ", " Display the grid maintenance dialog "},
{" EasyIF", " RCRRRRRRRRRRRRRRRRRRRRRRRRRRRR", " EasyIF", " Grid Name,Col1,Col2,... ", " 1", " EasyIF", " ", " ", " Evalate the specified grid using the supplied arguments ",
      " Evaluate the grid using the supplied column values, returning the best match result, if found "
},
{" EasyEval", " RC", " EasyEval", " String To Evaluate ", " 1", " EasyIF", " ", " ", " Evalate the specified string as a formula, returning the result ",
      " Evalate the specified string as a formula, returning the result "
},
{" EasyQuote", " RCC", " EasyQuote", " Stock Symbol,Info: (P)rice (H)igh (L)ow (V)olume (C)lose (%)change (N)ame ", " 1", " EasyIF", " ", " ", " Get the stock information for a given symbol ",
      " Get the stock information for a given symbol "
},
{0, 0, 0, 0, 0, 0, 0, 0}
};


// Menu table
/// INTERNATIONALIZE

static LPSTR cpExcelMenuItems[][6] = {
{" Eas&yIF", " ", " ", " Excel Grid Add-In", " "},
{" Grid &Setup..."," EasyIFMaint", " ", " Maintain Grids.", " "},
{" Grid &Wizard..."," EasyIFGridWiz", " ", " Invoke Grid Wizard.", " "},
{" Show/Hide &GridPad..."," EasyIFToggleGridPad", " ", " Show or hide the GridPad Display.", " "},
{" &Options..."," EasyIFConfig", " ", " EasyIF Configuration Settings.", " "},
{0, 0, 0, 0, 0}
};


#define WSZ_XLPROGID    OLESTR("Excel.Application")

// Initialization routine
BOOL __stdcall xlAutoOpen(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  int i, j, err;

  // Get XLL file name
  static XLOPER xDll;
  Excel(xlGetName, &xDll, 0);

  // Prefix strengths with their length & count items
  // Note the framework's TempStr() function prefixes the
  // lengths anyway, but this is for other code that might
  // use the arrays
  for (int nFuncs = 0; cpExcelFunctions[nFuncs][0]; nFuncs++)
    {
      for (i = 0; i < 10; i++)
        {
          cpExcelFunctions[nFuncs][i][0] = (BYTE)strlen(cpExcelFunctions[nFuncs][i]+1);
        }
    }

  for (int nMenuItems = 0; cpExcelMenuItems[nMenuItems][0]; nMenuItems++)
    {
      for ( i = 0; i < 5; i++)
        {
          cpExcelMenuItems[nMenuItems][i][0] = (BYTE)strlen(cpExcelMenuItems[nMenuItems][i]+1);
        }
    }

    // Loop through the function list, and register the functions
  for (i = 0; i < nFuncs; i++)
    {
      // Register a function
      err = Excel(xlfRegister, 0, 10, (LPXLOPER)&xDll,
                  (LPXLOPER)TempStr(cpExcelFunctions[i][0]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][1]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][2]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][3]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][4]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][5]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][6]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][7]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][8]),
                  (LPXLOPER)TempStr(cpExcelFunctions[i][9])
                 );

      if (err != xlretSuccess)
        {
          char caBuf[256];
          sprintf(caBuf, "xlfRegister for function %d, err = %d", i, err);
          AfxMessageBox(caBuf, MB_SETFOREGROUND);
        }
    }

  // Free XLL file name from the xlGetName call made earlier
  Excel(xlFree, 0, 1, (LPXLOPER)&xDll);

  // Menu support section
  static XLOPER xMenu;
  static XLOPER xMenuList[10*5];
  ASSERT(nMenuItems < 10);

  // Build menu
  xMenu.xltype            = xltypeMulti;
  xMenu.val.array.lparray = &xMenuList[0];
  xMenu.val.array.rows    = nMenuItems;
  xMenu.val.array.columns = 5;

  for (i = 0; i < nMenuItems; i++)
    {
      for (j = 0; j < 5; j++)
        {
          xMenuList[j+i*5].xltype  = xltypeStr;
          xMenuList[j+i*5].val.str = cpExcelMenuItems[i][j];
        }
    }

    // Add menu
/// INTERNATIONALIZE
  Excel(xlfAddMenu,0,3,TempNum(1),(LPXLOPER)&xMenu,TempStr(" Help"));

  // Load Matrices
  CEMXApp *pApp = (CEMXApp *)AfxGetApp();
  pApp->m_oMatrixManager.LoadMatrices();


  if (GetHwnd(&g_hExcelHWnd))
    {
      pApp->m_pFloatingGridsThread = (FloatingThread *)AfxBeginThread(RUNTIME_CLASS(FloatingThread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

      pApp->m_pFloatingGridsThread->m_hExcelMainWnd = g_hExcelHWnd;
      pApp->m_pFloatingGridsThread->m_pMainApp = pApp;

      LPDISPATCH pXLDisp = NULL;

      if (GetExcelApplicationObject(&pXLDisp) == S_OK)
        {
          if (pXLDisp)
            {
              // create hidden COM window
              pApp->m_pHiddenComWnd = new HiddenComWin(pXLDisp);
              pApp->m_pHiddenComWnd->m_iRecalcInterval = (UINT)pApp->m_dwRecalcInterval;
              if (pApp->m_pHiddenComWnd->Create(NULL,"EasyIF COM Updater",WS_POPUP))
                {
                  pApp->m_pFloatingGridsThread->m_hComWnd = pApp->m_pHiddenComWnd->m_hWnd;
                }
              else
                {
                  delete pApp->m_pHiddenComWnd;
                  pApp->m_pHiddenComWnd = NULL;
                }
            }
        }
      else
        {
/// INTERNATIONALIZE
          ::MessageBox(g_hExcelHWnd,"Unable To Get Excel Automation Interface!","OLE Automation Error",MB_ICONEXCLAMATION | MB_OK);
        }

      pApp->m_pFloatingGridsThread->ResumeThread();
    }

  // Finished
  return 1;
}



BOOL CALLBACK EnumProc(HWND hwnd, EnumStruct * pEnum)
{
	// first check the class of the window.  Must be "XLMAIN"
	char rgsz[CLASS_NAME_BUFFER];

  GetClassName(hwnd, rgsz, CLASS_NAME_BUFFER);

  if (!lstrcmpi(rgsz, "XLMAIN")) {
		// if that hits, check the loword of the window handle
		if (LOWORD((DWORD) hwnd) == pEnum->wLoword) {
			// We have a match, return Excel's main hWnd
      pEnum->hwnd = hwnd;
			return FALSE;
		}
	}

	// no luck - continue the enumeration
	return TRUE;
}

BOOL GetHwnd(HWND * pHwnd)
{
	XLOPER x;

  //
  // xlGetHwnd only returns the LoWord of Excel's hWnd
  // so all the windows have to be enumerated to see
  // which match the LoWord retuned by xlGetHwnd
  //
  if (Excel4(xlGetHwnd, &x, 0) == xlretSuccess) {
		EnumStruct enm;

		enm.hwnd = NULL;
		enm.wLoword = x.val.w;

		EnumWindows((WNDENUMPROC) EnumProc, (LPARAM) &enm);

		if (enm.hwnd != NULL) {
			*pHwnd = enm.hwnd;
			return TRUE;
		}
	}
	return FALSE;
}


// Cleanup routine
BOOL __stdcall xlAutoClose(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CEMXApp *pApp = (CEMXApp *)AfxGetApp();

  if (pApp->m_pHiddenComWnd)
    pApp->m_pHiddenComWnd->DestroyWindow();

  pApp->m_pFloatingGridsThread->PostThreadMessage(WM_QUIT,0,0);

  // Delete menu
  Excel(xlfDeleteMenu, 0, 2, TempNum(1), TempStr(" EasyIF"));

  return 1;
}


// Support for descriptive information about the add-in(s)
// You can add a new customized title for the user, but
// unfortunately, only an add-in written in Microsoft Visual Basic
// can add a description string.
LPXLOPER _stdcall xlAddInManagerInfo(LPXLOPER xAction)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  static XLOPER xInfo, xIntAction;

  // Find out what action must be taken
  Excel(xlCoerce, &xIntAction, 2, xAction, TempInt(xltypeInt));

  // Set title if asked
  if (xIntAction.val.w == 1)
    {
      xInfo.xltype = xltypeStr;
/// INTERNATIONALIZE
      xInfo.val.str = " EasyIF Grid Processing Add-In";
      xInfo.val.str[0] = (char)strlen(&xInfo.val.str[1]);
    }
  else
    {
      xInfo.xltype = xltypeErr;
      xInfo.val.err = xlerrValue;
    }

  return (LPXLOPER)&xInfo;
}




short __stdcall EasyIFConfig(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CEMXApp *pApp = (CEMXApp *)AfxGetApp();
  ConfigOptions oConfigDlg(&pApp->m_oMatrixManager);

  if (oConfigDlg.DoModal() == IDOK)
    {
      pApp->SaveSettings();

      if (pApp->m_pHiddenComWnd)
        pApp->m_pHiddenComWnd->ResetTimerInterval((UINT)pApp->m_dwRecalcInterval);

      if (oConfigDlg.m_bPathChanged)
        pApp->RefreshFloatingGrids();
    }

  return 1;
}



LPXLOPER __stdcall EasyIF(LPSTR cpMatrixName,
          LPXLOPER px1,
          LPXLOPER px2,
          LPXLOPER px3,
          LPXLOPER px4,
          LPXLOPER px5,
          LPXLOPER px6,
          LPXLOPER px7,
          LPXLOPER px8,
          LPXLOPER px9,
          LPXLOPER px10,
          LPXLOPER px11,
          LPXLOPER px12,
          LPXLOPER px13,
          LPXLOPER px14,
          LPXLOPER px15,
          LPXLOPER px16,
          LPXLOPER px17,
          LPXLOPER px18,
          LPXLOPER px19,
          LPXLOPER px20,
          LPXLOPER px21,
          LPXLOPER px22,
          LPXLOPER px23,
          LPXLOPER px24,
          LPXLOPER px25,
          LPXLOPER px26,
          LPXLOPER px27,
          LPXLOPER px28)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  static XLOPER xResult;  // Return value
  static char s_caStringBuf[256];

  Matrix *pMatrix = NULL;
  if (cpMatrixName && *cpMatrixName)
    pMatrix = ((CEMXApp *)AfxGetApp())->m_oMatrixManager.GetMatrix(cpMatrixName);

  if (pMatrix && CheckMatrixAccessRights(pMatrix))
    {
      // build params into a row for comparison...
      LPXLOPER pxParams[28];
      pxParams[0] = px1; pxParams[1] = px2; pxParams[2] = px3;
      pxParams[3] = px4; pxParams[4] = px5; pxParams[5] = px6;
      pxParams[6] = px7; pxParams[7] = px8; pxParams[8] = px9;
      pxParams[9] = px10; pxParams[10] = px11; pxParams[11] = px12;
      pxParams[12] = px13; pxParams[13] = px14; pxParams[14] = px15;
      pxParams[15] = px16; pxParams[16] = px17; pxParams[17] = px18;
      pxParams[18] = px19; pxParams[19] = px20; pxParams[20] = px21;
      pxParams[21] = px22; pxParams[22] = px23; pxParams[23] = px24;
      pxParams[24] = px25; pxParams[25] = px26; pxParams[26] = px27;
      pxParams[27] = px28;

      MatrixRow oMatchRow(NULL);

      XLOPER xMulti;          // Argument coerced to xltypeMulti
      LPXLOPER pxParam;
      LPXLOPER pxEmbeddedParam;
      MatrixColumnValue *pColVal;
      int iLup = 0;
      int iParamOffset = 0;
      BOOL bError = FALSE;

      // int iCount = pMatrix->GetColumnCount();
      // while (iLup < iCount && !bError)
      // changed these lines to allow for the evaluation to
      // process all input parameters to allow for {PARAMx} values above and
      // beyond the basic matrix column compare values...
      while (iParamOffset < 28 && !bError)
        {
          pxParam = pxParams[iParamOffset];
          switch (pxParam->xltype)
            {
              case xltypeNil:
              case xltypeMissing:
                // wildcard value
                if (oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup)))
                  iLup++;
                break;

              case xltypeNum:
                pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                if (pColVal)
                  {
                    iLup++;
                    pColVal->ValueFromDouble(pxParam->val.num);
                    pColVal->SetWildCard(FALSE);
                  }
                break;

              case xltypeRef:
              case xltypeSRef:
              case xltypeMulti:
                {
		        // Changed from xlretUncalced == to xlretSuccess !=
		        // so it returns anytime xlCoerce fails
                if (xlretSuccess != Excel(xlCoerce, &xMulti, 2,
    	            pxParam, TempInt(xltypeMulti)))
                  {
                    // That coerce might have failed due to an
                    // uncalced cell, in which case, we need to
                    // return immediately. Microsoft Excel will
                    // call us again in a moment after that cell
                    // has been calced.
                    //
                    return 0;
                  }


                for (int iLup2 = 0; iLup2 < (xMulti.val.array.rows * xMulti.val.array.columns); iLup2++)
                  {
                    // obtain a pointer to the current item //
                    pxEmbeddedParam = xMulti.val.array.lparray + iLup2;

                    // switch on XLOPER type //
                    switch (pxEmbeddedParam->xltype)
                      {
                        case xltypeNum:
                          pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                          if (pColVal)
                            {
                              iLup++;
                              pColVal->ValueFromDouble(pxEmbeddedParam->val.num);
                              pColVal->SetWildCard(FALSE);
                            }
                          break;

                        // if an error store in error //
                        case xltypeErr:
                          pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                          if (pColVal)
                            {
                              iLup++;
                              pColVal->ValueFromInteger((long)pxEmbeddedParam->val.err);
                              pColVal->SetWildCard(FALSE);
                            }
                          break;

                        case xltypeNil:
                          // wildcard value
                          if (oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup)))
                            iLup++;
                          break;

                        case xltypeStr:
                          memcpy(s_caStringBuf,pxEmbeddedParam->val.str+1,(int)*(pxEmbeddedParam->val.str));
                          s_caStringBuf[(int)*(pxEmbeddedParam->val.str)] = 0;
                          pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                          if (pColVal)
                            {
                              iLup++;
                              pColVal->ValueFromString((LPCSTR)s_caStringBuf);
                              pColVal->SetWildCard(FALSE);
                            }
                          break;

                        case xltypeInt:
                          pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                          if (pColVal)
                            {
                              iLup++;
                              pColVal->ValueFromInteger((long)pxEmbeddedParam->val.w);
                              pColVal->SetWildCard(FALSE);
                            }
                          break;

                        case xltypeBool:
                          pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                          if (pColVal)
                            {
                              iLup++;
                              pColVal->ValueFromInteger((long)pxEmbeddedParam->val.bool);
                              pColVal->SetWildCard(FALSE);
                            }
                          break;

                        // if anything else set error //
                        default:
                          bError = TRUE;
                          break;
                      }

                    if (bError)
                      break;
                  }

                // free the returned array //
                Excel(xlFree, 0, 1, (LPXLOPER) &xMulti);
                }
                break;

              case xltypeStr:
                memcpy(s_caStringBuf,pxParam->val.str+1,(int)*(pxParam->val.str));
                s_caStringBuf[(int)*(pxParam->val.str)] = 0;
                pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                if (pColVal)
                  {
                    iLup++;
                    pColVal->ValueFromString((LPCSTR)s_caStringBuf);
                    pColVal->SetWildCard(FALSE);
                  }
                break;

              case xltypeInt:
                pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                if (pColVal)
                  {
                    iLup++;
                    pColVal->ValueFromInteger((long)pxParam->val.w);
                    pColVal->SetWildCard(FALSE);
                  }
                break;

              case xltypeBool:
                pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                if (pColVal)
                  {
                    iLup++;
                    pColVal->ValueFromInteger((long)pxParam->val.bool);
                    pColVal->SetWildCard(FALSE);
                  }
                break;

              case xltypeErr:
                pColVal = oMatchRow.AddColumn(pMatrix->GetColumnDef(iLup));
                if (pColVal)
                  {
                    iLup++;
                    pColVal->ValueFromInteger((long)pxParam->val.err);
                    pColVal->SetWildCard(FALSE);
                  }
                break;

              default:
                bError = TRUE;
                break;
            }

          if (!bError)
            iParamOffset++;
        }

      // error encountered in parameters...
      if (bError)
        {
          xResult.xltype = xltypeErr;
          xResult.val.err = xlerrValue;
        }
      else
        {
          MatrixRow *pBestMatchRow = NULL;
          pColVal = pMatrix->GetMatch(&oMatchRow,&pBestMatchRow);
          if (pColVal)
            {
              CString cStrResult;
              switch (pColVal->m_iColType)
                {
                  case COLTYPE_STRING: // string
                  case COLTYPE_FORMULA: // Excel Formula
                    xResult.xltype = xltypeStr;
                    cStrResult = pColVal->m_cValue;
                    ParseStringVars(&oMatchRow,pBestMatchRow,cStrResult);
                    if (pColVal->m_iColType == COLTYPE_FORMULA)
                      return EasyEval((LPSTR)((LPCSTR)cStrResult));
                    else
                      {
                        sprintf(s_caStringBuf,"%c%s",cStrResult.GetLength(),(LPCSTR)cStrResult);
                        xResult.val.str = s_caStringBuf;
                      }
                    break;

                  case COLTYPE_INTEGER: // int
                    if (pColVal->m_lValue < 32766)
                      {
                        xResult.xltype = xltypeInt;
                        xResult.val.w = (short)pColVal->m_lValue;
                      }
                    else
                      {
                        xResult.xltype = xltypeNum;
                        xResult.val.num = pColVal->m_lValue;
                      }
                    break;

                  case COLTYPE_DOUBLE: // double
                    xResult.xltype = xltypeNum;
                    xResult.val.num = pColVal->m_dValue;
                    break;

                  case COLTYPE_DATE: // date
                    {
                    char caBuf[41];
                    char caBuf2[41];
                    int iYear, iMonth, iDay;

                    xResult.xltype = xltypeStr;

                    ltoa(pColVal->m_lValue,caBuf2,10);
                    memcpy(caBuf,caBuf2,4);
                    caBuf[4] = 0;
                    iYear = atoi(caBuf);
                    memcpy(caBuf,caBuf2+4,2);
                    caBuf[2] = 0;
                    iMonth = atoi(caBuf);
                    iDay = atoi(caBuf2+6);

                    SYSTEMTIME sTime;
                    memset(&sTime,0,sizeof(SYSTEMTIME));
                    sTime.wYear = iYear;
                    sTime.wMonth = iMonth;
                    sTime.wDay = iDay;

                    GetDateFormat(NULL,DATE_SHORTDATE,&sTime,NULL,s_caStringBuf+1,sizeof(s_caStringBuf)-1);
                    s_caStringBuf[0] = strlen(s_caStringBuf+1);
                    xResult.val.str = s_caStringBuf;
                    }
                    break;

                  default:
                    xResult.xltype = xltypeErr;
                    xResult.val.err = xlerrValue;
                    break;
                }
            }
          else
            {
              xResult.xltype = xltypeErr;
              xResult.val.err = xlerrValue;
            }
        }
    }
  else
    {
      xResult.xltype = xltypeErr;
      xResult.val.err = xlerrName;
    }


  return (LPXLOPER) &xResult;
}



void ReplaceAllInstances(CString &cStrResult, char *cpFind, char *cpReplace)
{
  int iPos = cStrResult.Find(cpFind);
  while (iPos > -1)
    {
      cStrResult = cStrResult.Left(iPos) + CString(cpReplace) + cStrResult.Mid(iPos+strlen(cpFind));
      iPos = cStrResult.Find(cpFind);
    }
}



void ParseStringVars(MatrixRow *pInputRow, MatrixRow *pMatchRow, CString &cStrResult)
{
  if (cStrResult.Find("{") >= 0)
    {
      char caKeyBuf[41];
      int iPos;
      char caReplace[512];
      MatrixColumnValue *pColVal;

      for (int iLup = 0; iLup < pInputRow->GetSize(); iLup++)
        {
          sprintf(caKeyBuf,"{PARAM%d}",iLup+1);
          iPos = cStrResult.Find(caKeyBuf);
          if (iPos > -1)
            {
              pColVal = pInputRow->GetCell(iLup);

              if (!pColVal || pColVal->IsWildCard())
                strcpy(caReplace,"#N/A");
              else
                {
                  switch (pColVal->GetColumnType())
                    {
                      case COLTYPE_STRING:
                        strcpy(caReplace,(LPCSTR)pColVal->m_cValue);
                        break;

                      case COLTYPE_INTEGER:
                        ltoa(pColVal->m_lValue,caReplace,10);
                        break;

                      case COLTYPE_DOUBLE:
                        {
                        sprintf(caReplace,"%.10f",pColVal->m_dValue);
                        int iOffset = strlen(caReplace) - 1;
                        while (iOffset > 0 && caReplace[iOffset] == '0')
                          caReplace[iOffset--] = 0;

                        if (iOffset > -1 && caReplace[iOffset] == '.')
                          caReplace[iOffset] = 0;
                        }
                        break;

                      case COLTYPE_DATE:
                        {
                        char caBuf2[21];
                        int iYear, iDay, iMonth;

                        ltoa(pColVal->m_lValue,caBuf2,10);
                        memcpy(caReplace,caBuf2,4);
                        caReplace[4] = 0;
                        iYear = atoi(caReplace);
                        memcpy(caReplace,caBuf2+4,2);
                        caReplace[2] = 0;
                        iMonth = atoi(caReplace);
                        iDay = atoi(caBuf2+6);

                        sprintf(caReplace,"%02d/%02d/%04d",iMonth,iDay,iYear);
                        }
                        break;

                      default:
                        strcpy(caReplace,"#N/A");
                        break;
                    }
                }

              ReplaceAllInstances(cStrResult,caKeyBuf,caReplace);
            }
        }

      if (cStrResult.Find("{") >= 0)
        {
          for (iLup = 0; iLup < pMatchRow->GetSize(); iLup++)
            {
              sprintf(caKeyBuf,"{GRIDCOL%d}",iLup+1);
              iPos = cStrResult.Find(caKeyBuf);
              if (iPos > -1)
                {
                  pColVal = pMatchRow->GetCell(iLup);

                  if (!pColVal || pColVal->IsWildCard())
                    strcpy(caReplace,"#N/A");
                  else
                    {
                      switch (pColVal->GetColumnType())
                        {
                          case COLTYPE_STRING:
                            strcpy(caReplace,(LPCSTR)pColVal->m_cValue);
                            break;

                          case COLTYPE_INTEGER:
                            ltoa(pColVal->m_lValue,caReplace,10);
                            break;

                          case COLTYPE_DOUBLE:
                            {
                            sprintf(caReplace,"%.10f",pColVal->m_dValue);
                            int iOffset = strlen(caReplace) - 1;
                            while (iOffset > 0 && caReplace[iOffset] == '0')
                              caReplace[iOffset--] = 0;

                            if (iOffset > -1 && caReplace[iOffset] == '.')
                              caReplace[iOffset] = 0;
                            }
                            break;

                          case COLTYPE_DATE:
                            {
                            char caBuf2[21];
                            int iYear, iDay, iMonth;

                            ltoa(pColVal->m_lValue,caBuf2,10);
                            memcpy(caReplace,caBuf2,4);
                            caReplace[4] = 0;
                            iYear = atoi(caReplace);
                            memcpy(caReplace,caBuf2+4,2);
                            caReplace[2] = 0;
                            iMonth = atoi(caReplace);
                            iDay = atoi(caBuf2+6);

                            sprintf(caReplace,"%02d/%02d/%04d",iMonth,iDay,iYear);
                            }
                            break;

                          default:
                            strcpy(caReplace,"#N/A");
                            break;
                        }
                    }

                  ReplaceAllInstances(cStrResult,caKeyBuf,caReplace);
                }
            }
        }
    }

  if (cStrResult.GetLength() > 255)
    cStrResult = cStrResult.Left(255);
}



short __stdcall EasyIFToggleGridPad(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  ((CEMXApp *)AfxGetApp())->ToggleFloatingGridDisplay();

  return 1;
}



short __stdcall EasyIFMaint(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  static XLOPER xSendKeys;

  MatrixSel oSelDlg(&(((CEMXApp *)AfxGetApp())->m_oMatrixManager));

  oSelDlg.DoModal();

  if (oSelDlg.m_bMatricesChanged)
    {
      char caKeyBuf[21];
      strcpy(caKeyBuf," ^%{F9}");
      caKeyBuf[0] = 6;
      xSendKeys.xltype = xltypeStr;
      xSendKeys.val.str = caKeyBuf;
      Excel(xlcSendKeys,0,1,(LPXLOPER)&xSendKeys);
    }

  return 1;
}




BOOL CheckMatrixAccessRights(Matrix *pMatrix)
{
  if (!pMatrix->RequiresAccessPassword() || (pMatrix->m_iSecurityPassedFlags & 1))
    return TRUE;

  // do password entry dialog here
  BOOL bRetval = FALSE;

  PasswordEntryDlg oPswDialog(pMatrix->GetAccessPassword());
  if (oPswDialog.DoModal() == IDOK)
    {
      bRetval = TRUE;
      pMatrix->m_iSecurityPassedFlags |= 1;
    }

  return bRetval;
}



LPXLOPER __stdcall EasyEval(LPSTR cpEvalStr)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  static XLOPER xResult;  // Return value
  static XLOPER xFormula;

  char caEvalBuf[512];

  if (cpEvalStr)
    strcpy(caEvalBuf,cpEvalStr);
  else
    caEvalBuf[0] = 0;

  // trim buffer
  int iOffset = strlen(caEvalBuf)-1;
  while (caEvalBuf[iOffset] == ' ' && iOffset >= 0)
    caEvalBuf[iOffset--] = 0;


  if (caEvalBuf[0])
    {
      int iLen = strlen(caEvalBuf);
      memmove(caEvalBuf+1,caEvalBuf,iLen+1);
      caEvalBuf[0] = (char)iLen;

      xFormula.xltype = xltypeStr;
      xFormula.val.str = caEvalBuf;

      int iRetval = Excel4(xlfEvaluate, &xResult, 1, (LPXLOPER)&xFormula);
      if (iRetval != xlretSuccess)
        {
          xResult.xltype = xltypeErr;
          xResult.val.err = xlerrValue;
        }
    }
  else
    {
      // error encountered in parameters...
      xResult.xltype = xltypeErr;
      xResult.val.err = xlerrName;
    }

  return (LPXLOPER) &xResult;
}


// quotes.nasdaq.com/quote.dll?page=multi&mode=stock&symbol=NIKU

LPXLOPER __stdcall EasyQuote(LPSTR cpSymbol, LPSTR cpElement)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  static XLOPER xResult;  // Return value
  static char s_caNameBuf[256];

  char caBuf[8196];
  char cElement = 'P';

  if (cpSymbol)
    {  // trim it
      int iOffset = strlen(cpSymbol)-1;
      while (iOffset > -1 && *(cpSymbol+iOffset) == ' ')
        {
          *(cpSymbol+iOffset) = 0;
          iOffset--;
        }
    }

  if (!cpSymbol || !(*cpSymbol))
    {
      // error encountered in parameters...
      xResult.xltype = xltypeErr;
      xResult.val.err = xlerrName;
    }
  else
    {
      // default to error condition...
      xResult.xltype = xltypeErr;
      xResult.val.err = xlerrValue;

      if (strchr("NPHLCV%",toupper(*cpElement)))
        cElement = toupper(*cpElement);

      if (!s_hIC)
        s_hIC = InternetOpen("EasyIF",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);

      if (s_hIC)
        {
          sprintf(caBuf,"http://quotes.nasdaq.com/quote.dll?page=multi&mode=stock&symbol=%s",cpSymbol);
          HINTERNET hURL = InternetOpenUrl(s_hIC,caBuf,NULL,0,INTERNET_FLAG_RELOAD,0);
          if (hURL)
            {
              DWORD dwBytesRead = 0;
              BOOL bRetval = TRUE;
              CString cHTML;

              while (bRetval)
                {
                  caBuf[0] = 0;
                  bRetval = InternetReadFile(hURL,caBuf,sizeof(caBuf)-1,&dwBytesRead);
                  if (bRetval)
                    {
                      if (dwBytesRead == 0)
                        {
                          // have entire string!
                          CString cFindStr = "Last Sale:";
                          CString cLocator = "$&nbsp;";
                          switch (cElement)
                            {
                              case 'N':
                                cFindStr = "<td nowrap><font face=\"Arial, Helvetica, Verdana\" size=\"2\"><b>";
                                cLocator = "</b>";
                                break;
                              case 'H':
                                cFindStr = "Today's High:";
                                break;
                              case 'L':
                                cFindStr = "Today's Low:";
                                break;
                              case 'C':
                                cFindStr = "Previous Close:";
                                break;
                              case 'V':
                                cFindStr = "Volume:";
                                cLocator = "width=\"85\"><b>";
                                break;
                              case '%':
                                cFindStr = "Net Change:";
                                cLocator = "</font>";
                                break;
                            }

                          int iPos = cHTML.Find(cFindStr);
                          if (iPos > -1)
                            {
                              cHTML = cHTML.Mid(iPos+cFindStr.GetLength());
                              // now, extract the value
                              iPos = cHTML.Find(cLocator);
                              if (iPos > -1)
                                {
                                  if (cElement == 'N')
                                    {
                                      cHTML = cHTML.Left(iPos);
                                    }
                                  else if (cElement == '%')
                                    {
                                      cHTML = cHTML.Left(iPos);
                                      iPos = cHTML.ReverseFind('>');
                                      if (iPos > -1)
                                        cHTML = cHTML.Mid(iPos+1);
                                    }
                                  else
                                    {
                                      cHTML = cHTML.Mid(iPos+cLocator.GetLength());
                                      iPos = cHTML.Find('<');
                                      if (iPos > -1)
                                        cHTML = cHTML.Left(iPos);
                                    }

                                  if (cElement == 'N')
                                    {
                                      sprintf(s_caNameBuf,"%c%s",(char)cHTML.GetLength(),(LPCSTR)cHTML);
                                      xResult.xltype = xltypeStr;
                                      xResult.val.str = s_caNameBuf;
                                    }
                                  else
                                    {
                                      iPos = cHTML.Find(',');
                                      while (iPos > -1)
                                        {
                                          cHTML = cHTML.Left(iPos) + cHTML.Mid(iPos+1);
                                          iPos = cHTML.Find(',');
                                        }

                                      xResult.xltype = xltypeNum;
                                      xResult.val.num = atof((LPCSTR)cHTML);
                                    }
                                }
                            }

                          break;
                        }

                      caBuf[dwBytesRead] = 0;
                      cHTML += CString(caBuf);
                    }
                }

              InternetCloseHandle(hURL);
            }
        }
    }

  return (LPXLOPER) &xResult;
}



void DoGridWizCurrentCell()
{
  static XLOPER xSendKeys;
  XLOPER xCellRef, xCellValue;
  char caStringBuf[256];

  int iCurrRow, iCurrCol;

  xCellRef.xltype = xltypeSRef;
  xCellRef.val.sref.count = 1;
  xCellRef.val.sref.ref.rwFirst = 0;
  xCellRef.val.sref.ref.rwLast = 0;
  xCellRef.val.sref.ref.colFirst = 0;
  xCellRef.val.sref.ref.colLast = 0;
  Excel4(xlfActiveCell,(LPXLOPER)&xCellRef,0);

  if (xCellRef.xltype != xltypeSRef)
    {
      CString cTitle, cMsg;
      if (!cTitle.LoadString(IDS_UNSUPPORTEDACTIVECELLHDG))
        cTitle = "Unsupported Current Cell";

      if (!cMsg.LoadString(IDS_UNSUPPORTEDACTIVECELL))
        cMsg = "Unsupported Active Region Selection or Multiple Regions selected!";

      ::MessageBox(NULL,(LPCSTR)cMsg,(LPCSTR)cTitle,MB_OK|MB_ICONEXCLAMATION);
    }
  else
    {
      // save current cell position
      iCurrRow = xCellRef.val.sref.ref.rwFirst;
      iCurrCol = xCellRef.val.sref.ref.colFirst;

      // extract the column types and headings
      int iCol = 0;

      int iRow, iType;
      ExcelColInfo *pColInfo;
      CObArray oColList;

      while (iCol < iCurrCol) // only go to prev column from active cell
        {
          iRow = iCurrRow;
          iType = xltypeStr;
          pColInfo = new ExcelColInfo;
          sprintf(pColInfo->m_caColHeading,"Column %d",iCol+1);
          pColInfo->m_iColType = xltypeNil;

          while (iRow >= 0 && iType != xltypeNil)
            {
              xCellRef.xltype = xltypeSRef;
              xCellRef.val.sref.count = 1;
              xCellRef.val.sref.ref.rwFirst = iRow;
              xCellRef.val.sref.ref.rwLast = iRow;
              xCellRef.val.sref.ref.colFirst = iCol;
              xCellRef.val.sref.ref.colLast = iCol;

              Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);

              if (iRow == iCurrRow)
                {
                  pColInfo->m_iColType = xCellValue.xltype;
                }
              else
                {
                  if (xCellValue.xltype == xltypeStr)
                    {
                      memcpy(pColInfo->m_caColHeading,xCellValue.val.str+1,*xCellValue.val.str);
                      pColInfo->m_caColHeading[*xCellValue.val.str] = 0;
                    }
                }

              iType = xCellValue.xltype;
              Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);

              iRow--;
            }

          if (pColInfo->m_iColType != xltypeNil)
            {
              strcpy(pColInfo->m_caExcelHeading,pColInfo->m_caColHeading);
              switch (pColInfo->m_iColType)
                {
                  case xltypeNum:
                    pColInfo->m_iColDataType = COLTYPE_DOUBLE;
                    break;
                  case xltypeInt:
                  case xltypeBool:
                  case xltypeErr:
                    pColInfo->m_iColDataType = COLTYPE_INTEGER;
                    break;
                  case xltypeStr:
                  default:
                    pColInfo->m_iColDataType = COLTYPE_STRING;
                    break;
                }

              pColInfo->m_iFieldType = 0;
              pColInfo->m_iCompareOp = COMPAREOP_EQ;
              pColInfo->m_iExcelCol = iCol;
              pColInfo->m_bUseColumn = TRUE;

              oColList.Add((CObject *)pColInfo);
            }
          else
            delete pColInfo;

          iCol++;
        }

      if (oColList.GetSize() > 0)
        {
          // do dialog...
          GridFromCellWiz oWizDlg(&oColList,"??");
          if (oWizDlg.DoModal() == IDOK)
            {
              Matrix *pMatrix = new Matrix(oWizDlg.m_pGridInfo->m_cMatrixName);
              pMatrix->SetFileName(pMatrix->GetMatrixName()+CString(".xlg"));
              pMatrix->SetReturnValueColumnType((WORD)oWizDlg.m_pGridInfo->m_iReturnType);

              char *p;
              MatrixColDef *pColDef, *pColDef2;
              CString cAddlParams;
              CString cFunctionStr = CString("=EasyIF(\"") + pMatrix->GetMatrixName() + CString("\"");
              for (int iLup = 0; iLup < oColList.GetSize(); iLup++)
                {
                  pColInfo = (ExcelColInfo *)oColList.GetAt(iLup);
                  if (!pColInfo->m_bUseColumn)
                    continue;

                  switch (pColInfo->m_iFieldType)
                    {
                      case 0: // standard
                        pColDef = pMatrix->AddColumn();
                        pColDef->m_iCompareOp = pColInfo->m_iCompareOp;
                        pColDef->m_iColType = pColInfo->m_iColDataType;
                        pColDef->m_cColName = pColInfo->m_caColHeading;
                        sprintf(caStringBuf,",R[0]C[-%d]",iCurrCol-pColInfo->m_iExcelCol);
                        cFunctionStr += CString(caStringBuf);
                        break;

                      case 1: // range
                        pColDef = pMatrix->AddColumn();
                        pColDef2 = pMatrix->AddColumn();

                        pColDef->m_iColType = pColInfo->m_iColDataType;
                        pColDef2->m_iColType = pColDef->m_iColType;

                        p = strchr(pColInfo->m_caColHeading, ',');
                        if (p)
                          {
                            *p = 0;
                            pColDef->m_cColName = pColInfo->m_caColHeading;
                            pColDef->m_cColName.TrimRight();
                            pColDef2->m_cColName = CString(p+1);
                            pColDef2->m_cColName.TrimLeft();
                          }
                        else
                          {
                            pColDef->m_cColName = CString(pColInfo->m_caColHeading) + CString(" From");
                            pColDef2->m_cColName = CString(pColInfo->m_caColHeading) + CString(" To");
                          }

                        switch (pColInfo->m_iCompareOp)
                          {
                            case 0:
                              pColDef->m_iCompareOp = COMPAREOP_GE;
                              pColDef2->m_iCompareOp = COMPAREOP_LT;
                              break;
                            case 1:
                              pColDef->m_iCompareOp = COMPAREOP_GE;
                              pColDef2->m_iCompareOp = COMPAREOP_LE;
                              break;
                            case 2:
                              pColDef->m_iCompareOp = COMPAREOP_GT;
                              pColDef2->m_iCompareOp = COMPAREOP_LE;
                              break;
                            case 3:
                              pColDef->m_iCompareOp = COMPAREOP_GT;
                              pColDef2->m_iCompareOp = COMPAREOP_LT;
                              break;
                          }

                        sprintf(caStringBuf,",R[0]C[-%d]",iCurrCol-pColInfo->m_iExcelCol);
                        cFunctionStr += CString(caStringBuf);
                        cFunctionStr += CString(caStringBuf);
                        break;

                      case 2: // restrict 2 list
                        pColDef = pMatrix->AddColumn();
                        pColDef->m_iCompareOp = pColInfo->m_iCompareOp;
                        pColDef->m_iColType = pColInfo->m_iColDataType;
                        pColDef->m_cColName = pColInfo->m_caColHeading;
                        pColDef->m_bUsePickList = TRUE;
                        pColDef->m_bRestrictToList = TRUE;
                        sprintf(caStringBuf,",R[0]C[-%d]",iCurrCol-pColInfo->m_iExcelCol);
                        cFunctionStr += CString(caStringBuf);
                        break;

                      case 3: // param only
                        sprintf(caStringBuf,",R[0]C[-%d]",iCurrCol-pColInfo->m_iExcelCol);
                        cAddlParams += CString(caStringBuf);
                        break;
                    }
                }

              // add a wildcard row and set the default value
              MatrixRow *pRow = pMatrix->AddRow();
              if (pRow)
                pRow->GetReturnValue()->ValueFromString(oWizDlg.m_pGridInfo->m_cDefValue);

              if (((CEMXApp *)AfxGetApp())->m_oMatrixManager.SaveMatrix(pMatrix))
                {
                  ((CEMXApp *)AfxGetApp())->m_oMatrixManager.m_oMatrices.SetAt(pMatrix->GetMatrixName(),(CObject *)pMatrix);

                  // now, let's generate the code for the active cell!
                  xCellRef.val.sref.ref.rwFirst = iCurrRow;
                  xCellRef.val.sref.ref.rwLast = iCurrRow;
                  xCellRef.val.sref.ref.colFirst = iCurrCol;
                  xCellRef.val.sref.ref.colLast = iCurrCol;

                  cFunctionStr += cAddlParams + CString(")");
                  sprintf(caStringBuf,"%c%s",(char)cFunctionStr.GetLength(),(LPCSTR)cFunctionStr);
                  xCellValue.xltype = xltypeStr;
                  xCellValue.val.str = caStringBuf;
                  Excel4(xlcFormula,0,2,(LPXLOPER)&xCellValue,(LPXLOPER)&xCellRef);

                  // pop up the editing dialog for the matrix...
                  if (oWizDlg.m_pGridInfo->m_bLaunchEditor)
                    {
                      MatrixEdit oEdit(pMatrix,IDS_EDITGRIDTITLE,NULL,2);
                      if (oEdit.DoModal() == IDOK)
                        {
                          ((CEMXApp *)AfxGetApp())->m_oMatrixManager.SaveMatrix(pMatrix);
                          strcpy(caStringBuf," ^%{F9}");
                          caStringBuf[0] = 6;
                          xSendKeys.xltype = xltypeStr;
                          xSendKeys.val.str = caStringBuf;
                          Excel(xlcSendKeys,0,1,(LPXLOPER)&xSendKeys);
                        }
                      else
                        {
                          CString cMatrixName = pMatrix->GetMatrixName();
                          CString cFileName = pMatrix->GetFileName();
                          ((CEMXApp *)AfxGetApp())->m_oMatrixManager.LoadMatrix(cFileName);
                          delete pMatrix;
                        }
                    }
                }
              else
                {
                  delete pMatrix;
                  pMatrix = NULL;
                }
            }
        }

      // clean up column list...
      for (int iLup = 0; iLup < oColList.GetSize(); iLup++)
        {
          pColInfo = (ExcelColInfo *)oColList.GetAt(iLup);
          if (pColInfo)
            delete pColInfo;
        }
    }
}



void DoGridWizInsertGrid()
{
  XLOPER xCellRef, xCellValue;
  char caStringBuf[256];

  int iStartRow = 0, iStartCol = 0;

  GridWiz oGridWiz(IDS_GRIDWIZARDCAPTION);

  xCellRef.xltype = xltypeSRef;
  xCellRef.val.sref.count = 1;
  xCellRef.val.sref.ref.rwFirst = 0;
  xCellRef.val.sref.ref.rwLast = 0;
  xCellRef.val.sref.ref.colFirst = 0;
  xCellRef.val.sref.ref.colLast = 0;
  Excel4(xlfActiveCell,(LPXLOPER)&xCellRef,0);

  if (xCellRef.xltype == xltypeSRef)
    {
      // save current cell position
      int iRow = xCellRef.val.sref.ref.rwFirst;
      int iCol = xCellRef.val.sref.ref.colFirst;

      // convert to A1 reference
      if (iCol > 25)
        {
          int iUpper = (iCol / 26) - 1;
          int iLower = iCol - ((iUpper+1) * 26);
          sprintf(caStringBuf,"%c%c%d",(char)('A'+iUpper),(char)('A' + iLower),iRow+1);
        }
      else
        sprintf(caStringBuf,"%c%d",(char)('A'+iCol),iRow+1);

      oGridWiz.m_pInsertPos->m_cInsertPos = caStringBuf;
    }

  if (oGridWiz.DoModal() == IDOK)
    {
      Matrix *pMatrix = ((CEMXApp *)AfxGetApp())->m_oMatrixManager.GetMatrix(oGridWiz.m_pSelect->m_cMatrix);

      if (pMatrix && CheckMatrixAccessRights(pMatrix))
        {
          // convert A1 style cell ref to 0-based row/col values
          strcpy(caStringBuf,(LPCSTR)oGridWiz.m_pInsertPos->m_cInsertPos);
          char *p = caStringBuf;
          while (*p && (*p < '0' || *p > '9'))
            p++;

          iStartRow = atoi(p);
          if (iStartRow > 0)
            iStartRow--;

          *p = 0;

          if ((p-caStringBuf) == 2)
            iStartCol = ((caStringBuf[0] - 'A' + 1) * 26) + (caStringBuf[1] - 'A');
          else
            iStartCol = caStringBuf[0] - 'A';


          MatrixColDef *pColDef;
          CString cParamStr = CString("=EasyIF(\"") + CString(pMatrix->GetMatrixName()) + CString("\"");

          xCellRef.xltype = xltypeSRef;
          xCellRef.val.sref.count = 1;

          int iExcelColumn = 0;
          void *vpDummy;

          int iColCount = pMatrix->GetColumnCount();
          for (int iLup = 0; iLup < iColCount; iLup++)
            {
              pColDef = pMatrix->GetColumnDef(iLup);
              strcpy(caStringBuf+1,(LPCSTR)pColDef->m_cColName);
              caStringBuf[0] = (char)pColDef->m_cColName.GetLength();

              xCellRef.val.sref.ref.rwFirst = iStartRow;
              xCellRef.val.sref.ref.rwLast = iStartRow;
              xCellRef.val.sref.ref.colFirst = iStartCol+iExcelColumn;
              xCellRef.val.sref.ref.colLast = iStartCol+iExcelColumn;

              xCellValue.xltype = xltypeStr;
              xCellValue.val.str = caStringBuf;

              Excel4(xlSet,0,2,(LPXLOPER)&xCellRef,(LPXLOPER)&xCellValue);

              xCellRef.val.sref.ref.rwFirst = iStartRow+1;
              xCellRef.val.sref.ref.rwLast = iStartRow+1;

              switch (pColDef->m_iColType)
                {
                  case COLTYPE_STRING:
                    xCellValue.xltype = xltypeStr;
/// INTERNATIONALIZE
                    xCellValue.val.str = "\010(string)";
                    break;
                  case COLTYPE_INTEGER:
                    xCellValue.xltype = xltypeInt;
                    xCellValue.val.w = 0;
                    break;
                  case COLTYPE_DOUBLE:
                    xCellValue.xltype = xltypeNum;
                    xCellValue.val.num = 1.23;
                    break;
                  case COLTYPE_DATE:
                    xCellValue.xltype = xltypeNum;
                    xCellValue.val.num = 36891;  // 12/31/2000
                    break;
                }

              Excel4(xlSet,0,2,(LPXLOPER)&xCellRef,(LPXLOPER)&xCellValue);

              // add to parameter string
    // for some reason, it requires R[x]C[y] R1C1 format rather than A1 format

              sprintf(caStringBuf,",R[0]C[-%d]",iColCount-iExcelColumn-oGridWiz.m_pRange->m_oColRange.GetCount());
              cParamStr += caStringBuf;

              // see if this is a range aggregate...
              if (oGridWiz.m_pRange->m_oColRange.Lookup((WORD)iLup,(void *&)vpDummy))
                {
                  cParamStr += caStringBuf;  // send it again
                  iLup++;  // and skip next column
                }

              iExcelColumn++;
            }

          // result heading

          xCellRef.val.sref.ref.rwFirst = iStartRow;
          xCellRef.val.sref.ref.rwLast = iStartRow;
          xCellRef.val.sref.ref.colFirst = iStartCol+iExcelColumn;
          xCellRef.val.sref.ref.colLast = iStartCol+iExcelColumn;

          xCellValue.xltype = xltypeStr;
/// INTERNATIONALIZE
          xCellValue.val.str = "\006Result";
          Excel4(xlSet,0,2,(LPXLOPER)&xCellRef,(LPXLOPER)&xCellValue);

          // now, add a cell with the formula set up
          xCellRef.val.sref.ref.rwFirst = iStartRow+1;
          xCellRef.val.sref.ref.rwLast = iStartRow+1;

          cParamStr += CString(")");
          sprintf(caStringBuf,"%c%s",(char)cParamStr.GetLength(),(LPCSTR)cParamStr);
          xCellValue.val.str = caStringBuf;
          Excel4(xlcFormula,0,2,(LPXLOPER)&xCellValue,(LPXLOPER)&xCellRef);
        }
    }
}



BOOL CellCannotBeBlank(int iRow, int iCol)
{
  BOOL bRetval = TRUE;
  char caA1Buf[41];
  char caBuf[256];

  if (iCol > 25)
    {
      int iUpper = (iCol / 26) - 1;
      int iLower = iCol - ((iUpper+1) * 26);
      sprintf(caA1Buf,"%c%c%d",(char)('A'+iUpper),(char)('A' + iLower),iRow+1);
    }
  else
    sprintf(caA1Buf,"%c%d",(char)('A'+iCol),iRow+1);

  CString cTitle, cMsg;
  if (!cTitle.LoadString(IDS_CELLCANNOTBEBLANKHDG))
    cTitle = "Missing Cell Data";

  if (!cMsg.LoadString(IDS_CELLCANNOTBEBLANK))
    cMsg = "The Cell [%s] Should Not Be Blank!\nIgnore Anyway And Treat As A Wildcard?";

  sprintf(caBuf,(LPCSTR)cMsg,caA1Buf);

  if (::MessageBox(NULL,caBuf,(LPCSTR)cTitle,MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDYES)
    bRetval = FALSE;

  return bRetval;
}



void DoGridWizGridFromSelection()
{
  XLOPER xCellRef, xCellValue;
  char caStringBuf[256];
  BOOL bBadSelectionType = FALSE;

  xCellRef.xltype = xltypeSRef;
  xCellRef.val.sref.count = 1;
  xCellRef.val.sref.ref.rwFirst = 0;
  xCellRef.val.sref.ref.rwLast = 0;
  xCellRef.val.sref.ref.colFirst = 0;
  xCellRef.val.sref.ref.colLast = 0;

  Excel4(xlfSelection,(LPXLOPER)&xCellRef,0);

  // verify that the selection is something we can deal with...
  if (xCellRef.xltype != xltypeSRef)
    bBadSelectionType = TRUE;

  if ((xCellRef.val.sref.ref.rwFirst == xCellRef.val.sref.ref.rwLast) &&
      (xCellRef.val.sref.ref.colFirst == xCellRef.val.sref.ref.colLast))
    bBadSelectionType = TRUE;

  if (bBadSelectionType)
    {
      CString cTitle, cMsg;
      if (!cTitle.LoadString(IDS_UNSUPPORTEDSELECTIONHDG))
        cTitle = "Unsupported Selection Type";

      if (!cMsg.LoadString(IDS_UNSUPPORTEDSELECTION))
        cMsg = "Unsupported Region Selection\nor Multiple Regions selected!\n\nPlease Select A Single Rectangular\nRegion Before Invoking This Wizard.";

      ::MessageBox(NULL,(LPCSTR)cMsg,(LPCSTR)cTitle,MB_OK|MB_ICONEXCLAMATION);

      return;
    }


  CRect oR(xCellRef.val.sref.ref.colFirst,xCellRef.val.sref.ref.rwFirst,xCellRef.val.sref.ref.colLast,xCellRef.val.sref.ref.rwLast);
  GFRSetup oDlg(&oR);

  // see if we can determine whether the first row of selection could be column headers...
  int iHeadCount = 0;
  for (int iLup = oR.left; iLup <= oR.right; iLup++)
    {
      xCellRef.xltype = xltypeSRef;
      xCellRef.val.sref.count = 1;
      xCellRef.val.sref.ref.rwFirst = (USHORT)oR.top;
      xCellRef.val.sref.ref.rwLast = (USHORT)oR.top;
      xCellRef.val.sref.ref.colFirst = (UCHAR)iLup;
      xCellRef.val.sref.ref.colLast = (UCHAR)iLup;

      Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);

      if (xCellValue.xltype == xltypeStr || xCellValue.xltype == xltypeNil)
        iHeadCount++;

      Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);
    }

  oDlg.m_bFirstRowColHeadings = (iHeadCount == (oR.right-oR.left+1));

  if (oDlg.DoModal() == IDOK)
    {
      Matrix *pMatrix = new Matrix(oDlg.m_cMatrixName);
      pMatrix->SetFileName(pMatrix->GetMatrixName()+CString(".xlg"));

      // figure out the return value type...
      if (oR.top != oR.bottom || !oDlg.m_bFirstRowColHeadings)
        {
          xCellRef.xltype = xltypeSRef;
          xCellRef.val.sref.count = 1;
          xCellRef.val.sref.ref.rwFirst = (USHORT)(oR.top + (oDlg.m_bFirstRowColHeadings ? 1 : 0));
          xCellRef.val.sref.ref.rwLast = (USHORT)(oR.top + (oDlg.m_bFirstRowColHeadings ? 1 : 0));
          xCellRef.val.sref.ref.colFirst = (UCHAR)oR.right;
          xCellRef.val.sref.ref.colLast = (UCHAR)oR.right;

          int iOffset = xCellRef.val.sref.ref.rwFirst;
          int iType = xltypeNil;
          while (iType == xltypeNil && iOffset <= oR.bottom)
            {
              Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);
              iType = xCellValue.xltype;
              Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);

              iOffset++;
              xCellRef.val.sref.ref.rwFirst++;
              xCellRef.val.sref.ref.rwLast++;
            }

          switch (iType)
            {
              case xltypeNum:
                pMatrix->SetReturnValueColumnType(COLTYPE_DOUBLE);
                break;
              case xltypeInt:
              case xltypeBool:
              case xltypeErr:
                pMatrix->SetReturnValueColumnType(COLTYPE_INTEGER);
                break;
              case xltypeStr:
                pMatrix->SetReturnValueColumnType(COLTYPE_STRING);
                break;
              default:
                // let it use the default from the matrix constructor
                break;
            }
        }

      // extract and build columns
      MatrixColDef *pColDef;
      int iColType;

      for (int iCell = oR.left; iCell < oR.right; iCell++)
        {
          iColType = xltypeNil;
          pColDef = pMatrix->AddColumn();

          xCellRef.xltype = xltypeSRef;
          xCellRef.val.sref.count = 1;
          xCellRef.val.sref.ref.rwFirst = (USHORT)oR.top;
          xCellRef.val.sref.ref.rwLast = (USHORT)oR.top;
          xCellRef.val.sref.ref.colFirst = iCell;
          xCellRef.val.sref.ref.colLast = iCell;

          Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);

          iColType = xCellValue.xltype;
/// INTERNATIONALIZE
          sprintf(caStringBuf,"Column %d",iCell-oR.left+1);  // default heading

          if (oDlg.m_bFirstRowColHeadings)
            {
              if (xCellValue.xltype == xltypeStr)
                {
                  memcpy(caStringBuf,xCellValue.val.str+1,*xCellValue.val.str);
                  caStringBuf[*xCellValue.val.str] = 0;
                }

              iColType = xltypeNil;  // force column type logic to scan to next row...
            }

          Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);

          pColDef->m_cColName = caStringBuf;

          // scan the next row(s) to determine data type
          int iOffset = oR.top+1;
          xCellRef.xltype = xltypeSRef;
          xCellRef.val.sref.count = 1;
          xCellRef.val.sref.ref.rwFirst = iOffset;
          xCellRef.val.sref.ref.rwLast = iOffset;
          xCellRef.val.sref.ref.colFirst = iCell;
          xCellRef.val.sref.ref.colLast = iCell;

          while (iColType == xltypeNil && iOffset <= oR.bottom)
            {
              Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);
              iColType = xCellValue.xltype;
              Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);

              iOffset++;
              xCellRef.val.sref.ref.rwFirst++;
              xCellRef.val.sref.ref.rwLast++;
            }

          switch (iColType)
            {
              case xltypeNum:
                pColDef->m_iColType = COLTYPE_DOUBLE;
                break;
              case xltypeInt:
              case xltypeBool:
              case xltypeErr:
                pColDef->m_iColType = COLTYPE_INTEGER;
                break;
              case xltypeStr:
              default:
                pColDef->m_iColType = COLTYPE_STRING;
                break;
            }
        }

      // extract grid data
      BOOL bDataError = FALSE;
      MatrixRow *pRow;
      MatrixColumnValue *pColVal;
      int iCol;

      for (int iRow = oR.top + (oDlg.m_bFirstRowColHeadings ? 1 : 0); iRow <= oR.bottom; iRow++)
        {
          pRow = pMatrix->AddRow();

          for (iCol = oR.left; iCol <= oR.right; iCol++)
            {
              if (iCol == oR.right)
                pColVal = pRow->GetReturnValue();
              else
                pColVal = pRow->GetCell(iCol-oR.left);

              xCellRef.xltype = xltypeSRef;
              xCellRef.val.sref.count = 1;
              xCellRef.val.sref.ref.rwFirst = iRow;
              xCellRef.val.sref.ref.rwLast = iRow;
              xCellRef.val.sref.ref.colFirst = iCol;
              xCellRef.val.sref.ref.colLast = iCol;

              Excel(xlCoerce, (LPXLOPER) &xCellValue, 1, (LPXLOPER) &xCellRef);
              switch (xCellValue.xltype)
                {
                  case xltypeNum:
                    pColVal->ValueFromDouble(xCellValue.val.num);
                    pColVal->SetWildCard(FALSE);
                    break;
                  case xltypeInt:
                    pColVal->ValueFromInteger((long)xCellValue.val.w);
                    pColVal->SetWildCard(FALSE);
                    break;
                  case xltypeBool:
                    pColVal->ValueFromInteger((long)xCellValue.val.bool);
                    pColVal->SetWildCard(FALSE);
                    break;
                  case xltypeErr:
                    pColVal->ValueFromInteger((long)xCellValue.val.err);
                    pColVal->SetWildCard(FALSE);
                    break;
                  case xltypeStr:
                    memcpy(caStringBuf,xCellValue.val.str+1,*xCellValue.val.str);
                    caStringBuf[*xCellValue.val.str] = 0;
                    if (caStringBuf[0])
                      {
                        if (strlen(caStringBuf) > 1 || caStringBuf[0] != '*')
                          {
                            pColVal->ValueFromString(caStringBuf);
                            pColVal->SetWildCard(FALSE);
                          } // else leave as wildcard
                      }
                    else
                      {
                        if (CellCannotBeBlank(iRow,iCol))
                          {
                            Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);
                            bDataError = TRUE;
                          }
                      }
                    break;
                  case xltypeMissing:
                  case xltypeNil:
                    if (CellCannotBeBlank(iRow,iCol))
                      {
                        Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);
                        bDataError = TRUE;
                      }
                    break;
                }

              Excel(xlFree, 0, 1, (LPXLOPER) &xCellValue);

              if (bDataError)
                break;
            }

          if (bDataError)
            break;
        }

      if (!bDataError)
        {
          if (((CEMXApp *)AfxGetApp())->m_oMatrixManager.SaveMatrix(pMatrix))
            {
              ((CEMXApp *)AfxGetApp())->m_oMatrixManager.m_oMatrices.SetAt(pMatrix->GetMatrixName(),(CObject *)pMatrix);

              // pop up the editing dialog for the matrix...
              MatrixEdit oEdit(pMatrix,IDS_EDITGRIDTITLE);
              if (oEdit.DoModal() == IDOK)
                {
                  ((CEMXApp *)AfxGetApp())->m_oMatrixManager.SaveMatrix(pMatrix);
                }
              else
                {
                  CString cMatrixName = pMatrix->GetMatrixName();
                  CString cFileName = pMatrix->GetFileName();
                  ((CEMXApp *)AfxGetApp())->m_oMatrixManager.LoadMatrix(cFileName);
                  delete pMatrix;
                }
            }
          else
            {
              delete pMatrix;
              pMatrix = NULL;
            }
        }
      else
        {
          // set focus to the data cell in error
          xCellRef.xltype = xltypeSRef;
          xCellRef.val.sref.count = 1;
          xCellRef.val.sref.ref.rwFirst = iRow;
          xCellRef.val.sref.ref.rwLast = iRow;
          xCellRef.val.sref.ref.colFirst = iCol;
          xCellRef.val.sref.ref.colLast = iCol;
          Excel(xlcSelect, 0, 1, (LPXLOPER)&xCellRef);
        }
    }
}



short __stdcall EasyIFGridWiz(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  GridWizChooser oChooser;
  if (oChooser.DoModal() == IDOK)
    {
      if (oChooser.m_iWizardSelection == 0)
        DoGridWizCurrentCell();

      if (oChooser.m_iWizardSelection == 1)
        DoGridWizInsertGrid();

      if (oChooser.m_iWizardSelection == 2)
        DoGridWizGridFromSelection();
    }

  return 1;
}



