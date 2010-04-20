// MtxGrid.cpp: implementation of the MatrixGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EMX.h"
#include "MtxGrid.h"
#include "mtxEdDat.h"
#include "fsgrid.h"
#include "flotvalp.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixGrid::MatrixGrid()
{
  m_pMatrix = NULL;
  m_hExcelArrow = NULL;
  m_hExcelWEResize = NULL;
  m_hExcelNSResize = NULL;

  m_iParentType = 0;
}



MatrixGrid::~MatrixGrid()
{
  if (m_hExcelArrow)
    DestroyCursor(m_hExcelArrow);

  if (m_hExcelWEResize)
    DestroyCursor(m_hExcelWEResize);

  if (m_hExcelNSResize)
    DestroyCursor(m_hExcelNSResize);
}



BOOL MatrixGrid::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.dwExStyle |= WS_EX_CLIENTEDGE;

  return CUGExcelCtrl::PreCreateWindow(cs);
}


void MatrixGrid::OnSetup()
{
  char caBuf[128];

  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SSHORTDATE,caBuf,sizeof(caBuf));
  char *p = caBuf;
  BOOL bYearSeen = FALSE;
  int iYearCount = 0;
  BOOL bMonthSeen = FALSE;
  BOOL bDaySeen = FALSE;
  char caFmt[4];
  caFmt[0] = 0;

  while (*p)
    {
      if (*p == 'y')
        {
          iYearCount++;
          if (!bYearSeen)
            strcat(caFmt,"Y");
          bYearSeen = TRUE;
        }

      if (*p == 'M' && !bMonthSeen)
        {
          bMonthSeen = TRUE;
          strcat(caFmt,"M");
        }

      if (*p == 'd' && !bDaySeen)
        {
          bDaySeen = TRUE;
          strcat(caFmt,"D");
        }

      p++;
    }

  m_iDateFormat = 0;

  if (!strcmp(caFmt,"MDY"))
    m_iDateFormat = 0;
  if (!strcmp(caFmt,"DMY"))
    m_iDateFormat = 1;
  if (!strcmp(caFmt,"YMD"))
    m_iDateFormat = 2;

  if (iYearCount > 2)
    m_iDateFormat += 100;


  CUGExcelCtrl::OnSetup();
  SetCancelMode(FALSE);

  int iColCount = m_pMatrix->GetColumnCount();
  int iRowCount = m_pMatrix->GetRowCount();

  m_hExcelArrow = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR3));
  m_hExcelWEResize  = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1));
  m_hExcelNSResize  = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR2));

  SetNumberRows(iRowCount + 1);  // new row
  SetNumberCols(iColCount + 1);  // include return column

  EnableMenu(0);

  SetArrowCursor(m_hExcelArrow);
  SetWESizingCursor(m_hExcelWEResize);
  SetNSSizingCursor(m_hExcelNSResize);

  RECT rect =  {0,0,0,0};

  m_oMaskedEdit.Create(WS_VISIBLE,rect,this,125);
  SetNewEditClass(&m_oMaskedEdit);
}



int MatrixGrid::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
  return CUGExcelCtrl::OnCanMove(oldcol,oldrow,newcol,newrow);
}



void MatrixGrid::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
  CUGExcelCtrl::OnCellChange(oldcol,newcol,oldrow,newrow);
}



void MatrixGrid::OnGetCell(int col, long row, CUGCell *cell)
{
  if (col < 0)
    CUGExcelCtrl::OnGetCell(col,row,cell);
  else
    {
      if (row < 0)
        {
          if (col == m_pMatrix->GetColumnCount())
            {
              CString cTitle;
              if (!cTitle.LoadString(IDS_RESULTCOLNAME))
                cTitle = "Result";

              cell->SetText(cTitle);
            }
          else
            cell->SetText(m_pMatrix->GetColumnDef(col)->m_cColName);
        }
      else
        {
          // get data value for cell
          if (col == m_pMatrix->GetColumnCount())  // result column
            {
              MatrixRow *pRow = m_pMatrix->GetRow(row);
              if (pRow)
                {
                  MatrixColumnValue *pColVal = pRow->GetReturnValue();
                  switch (pColVal->m_iColType)
                    {
                      case COLTYPE_STRING:
                      case COLTYPE_FORMULA:
                        cell->SetText(pColVal->m_cValue);
                        break;

                      case COLTYPE_INTEGER:
                        cell->SetNumber(pColVal->m_lValue);
                        break;

                      case COLTYPE_DOUBLE:
                        cell->SetNumber(pColVal->m_dValue);
                        break;

                      case COLTYPE_DATE:
                        {
                        char caBuf[41];

                        ValueToDateStr(pColVal->m_lValue,caBuf);
                        cell->SetText(caBuf);
                        }
                        break;

                      default:
                        cell->SetText(pColVal->m_cValue);
                        break;
                    }
                }
              else
                {
                  switch (m_pMatrix->GetReturnValueColumnType())
                    {
                      case COLTYPE_STRING:
                      case COLTYPE_FORMULA:
                        cell->SetText("");
                        break;

                      case COLTYPE_INTEGER:
                        cell->SetNumber(0L);
                        break;

                      case COLTYPE_DOUBLE:
                        cell->SetNumber(0.0);
                        break;

                      case COLTYPE_DATE:
                        cell->SetText("");
                        break;

                      default:
                        cell->SetText("");
                        break;
                    }
                }
            }
          else
            {
              MatrixColumnValue *pColVal = m_pMatrix->GetCell(row,col);
              if (pColVal && !pColVal->IsWildCard())
                {
                  switch (pColVal->m_iColType)
                    {
                      case COLTYPE_STRING:
                      case COLTYPE_FORMULA:
                        cell->SetText(pColVal->m_cValue);
                        break;

                      case COLTYPE_INTEGER:
                        cell->SetNumber(pColVal->m_lValue);
                        break;

                      case COLTYPE_DOUBLE:
                        cell->SetNumber(pColVal->m_dValue);
                        break;

                      case COLTYPE_DATE:
                        {
                        char caBuf[41];

                        ValueToDateStr(pColVal->m_lValue,caBuf);
                        cell->SetText(caBuf);
                        }
                        break;

                      default:
                        cell->SetText(pColVal->m_cValue);
                        break;
                    }
                }
              else
                cell->SetText("*");
            }
        }
    }
}



void MatrixGrid::OnSetCell(int col, long row, CUGCell *cell)
{
  BOOL bAppendRow = FALSE;
  if (row == m_pMatrix->GetRowCount())
    {
      m_pMatrix->AddRow();
      bAppendRow = TRUE;
    }

  CString cTextVal;
  cell->GetText(&cTextVal);
  cTextVal.TrimRight();
  long lValue;
  double dValue;

  BOOL bChanged = FALSE;

  if (col == m_pMatrix->GetColumnCount())
    {
      MatrixColumnValue *pColVal = m_pMatrix->GetRow(row)->GetReturnValue();
      switch (pColVal->m_iColType)
        {
          case COLTYPE_STRING:
          case COLTYPE_FORMULA:
            if (pColVal->m_cValue != cTextVal)
              {
                pColVal->m_cValue = cTextVal;
                bChanged = TRUE;
              }
            break;

          case COLTYPE_INTEGER:
            cell->GetNumber(&lValue);
            if (pColVal->m_lValue != lValue)
              {
                pColVal->m_lValue = lValue;
                bChanged = TRUE;
              }
            break;

          case COLTYPE_DOUBLE:
            cell->GetNumber(&dValue);
            if (fabs(pColVal->m_dValue-dValue) > 0.0001)
              {
                pColVal->m_dValue = dValue;
                bChanged = TRUE;
              }
            break;

          case COLTYPE_DATE:
            {
            CString cDate;
            cell->GetText(&cDate);
            lValue = DateStrToValue(cDate);
            if (pColVal->m_lValue != lValue)
              {
                pColVal->ValueFromDate(lValue);
                bChanged = TRUE;
              }
            }
            break;

          default:
            if (pColVal->m_cValue != cTextVal)
              {
                pColVal->m_cValue = cTextVal;
                bChanged = TRUE;
              }
            break;
        }

      pColVal->SetWildCard(FALSE);
    }
  else
    {
      MatrixColumnValue *pColVal = m_pMatrix->GetCell(row,col);
      if (pColVal)
        {
          if (cTextVal.IsEmpty() || cTextVal == "*")
            {
              if (!pColVal->IsWildCard())
                {
                  pColVal->SetWildCard();
                  bChanged = TRUE;
                }
            }
          else
            {
              switch (pColVal->m_iColType)
                {
                  case COLTYPE_STRING:
                  case COLTYPE_FORMULA:
                    if (pColVal->m_cValue != cTextVal)
                      {
                        pColVal->m_cValue = cTextVal;
                        bChanged = TRUE;
                      }
                    break;

                  case COLTYPE_INTEGER:
                    cell->GetNumber(&lValue);
                    if (pColVal->m_lValue != lValue)
                      {
                        pColVal->m_lValue = lValue;
                        bChanged = TRUE;
                      }
                    break;

                  case COLTYPE_DOUBLE:
                    cell->GetNumber(&dValue);
                    if (fabs(pColVal->m_dValue-dValue) > 0.0001)
                      {
                        pColVal->m_dValue = dValue;
                        bChanged = TRUE;
                      }
                    break;

                  case COLTYPE_DATE:
                    {
                    CString cDate;
                    cell->GetText(&cDate);
                    lValue = DateStrToValue(cDate);
                    if (pColVal->m_lValue != lValue)
                      {
                        pColVal->ValueFromDate(lValue);
                        bChanged = TRUE;
                      }
                    }
                    break;

                  default:
                    if (pColVal->m_cValue != cTextVal)
                      {
                        pColVal->m_cValue = cTextVal;
                        bChanged = TRUE;
                      }
                    break;
                }

              pColVal->SetWildCard(FALSE);
            }
        }
    }

  if (bChanged)
    {
      m_pMatrix->SetModified();
      if (m_iParentType == 2)
        ((FloatingValuesPage *)GetParent())->GridChanged();
    }

  CUGExcelCtrl::OnSetCell(col,row,cell);

  if (bAppendRow)
    {
      // only add a new row if the last row is not all wildcards...
      if (m_pMatrix->GetRowCount() < 2 || !LastRowIsWildCards())
        AppendRow();
    }
}



BOOL MatrixGrid::LastRowIsWildCards()
{
  BOOL bRetval = FALSE;

  MatrixRow *pRow = m_pMatrix->GetRow(m_pMatrix->GetRowCount()-1);
  if (pRow)
    {
      int iWildCardCount = 0;

      int iColCount = pRow->GetSize();
      for (int iLup = 0; iLup < iColCount; iLup++)
        {
          if (pRow->GetCell(iLup)->IsWildCard())
            iWildCardCount++;
        }

      if (iWildCardCount == iColCount)
        bRetval = TRUE;
    }

  return bRetval;
}



int MatrixGrid::OnEditStart(int col, long row, CWnd **edit)
{
  MatrixColDef *pColDef = m_pMatrix->GetColumnDef(col);
  if (pColDef && pColDef->m_bUsePickList && pColDef->m_bRestrictToList && GetCurrentRow() >= 0)
    {
      return FALSE;
    }
  else
    return CUGExcelCtrl::OnEditStart(col,row,edit);
}



void MatrixGrid::OnCharDown(UINT *vcKey, BOOL processed)
{
  MatrixColDef *pColDef = m_pMatrix->GetColumnDef(GetCurrentCol());

  if (pColDef && pColDef->m_bUsePickList && GetCurrentRow() >= 0)
    {
      if (!pColDef->m_bRestrictToList)
        StartEdit(*vcKey);
    }
  else
    CUGExcelCtrl::OnCharDown(vcKey,processed);
}



int MatrixGrid::OnEditFinish(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag)
{
  BOOL bRetval = TRUE;

  MatrixColDef *pColDef = m_pMatrix->GetColumnDef(col);
  if (pColDef && pColDef->m_iColType == COLTYPE_DATE)
    {
      if (*string != 0 && *string != '*')
        {
          // validate date
          long lValue = DateStrToValue(string);
          if (lValue)
            {
              char caBuf[128], caBuf2[21];
              int iYear, iMonth, iDay;
              CString cErrMsg;

              ltoa(lValue,caBuf2,10);
              memcpy(caBuf,caBuf2,4);
              caBuf[4] = 0;
              iYear = atoi(caBuf);
              memcpy(caBuf,caBuf2+4,2);
              caBuf[2] = 0;
              iMonth = atoi(caBuf);
              iDay = atoi(caBuf2+6);

              if (iMonth < 1 || iMonth > 12)
                {
                  if (!cErrMsg.LoadString(IDS_INVALIDMONTH))
                    cErrMsg = "Invalid Month!";
                }
              else
                {
                  if (iDay < 0 || iDay > 31)
                    {
                      if (!cErrMsg.LoadString(IDS_INVALIDDAY))
                        cErrMsg = "Invalid Day!";
                    }
                  else
                    {
                      if (iMonth == 2 && iDay > 29)
                        {
                          if (!cErrMsg.LoadString(IDS_INVALIDDAY))
                            cErrMsg = "Invalid Day!";
                        }
                      else
                        {
                          if ((iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11) && iDay > 30)
                            {
                              if (!cErrMsg.LoadString(IDS_INVALIDDAY))
                                cErrMsg = "Invalid Day!";
                            }
                        }
                    }
                }

              if (!cErrMsg.IsEmpty())
                {
                  CString cFmt, cTitle;

                  if (!cTitle.LoadString(IDS_DATAENTRYERRORTITLE))
                    cTitle = "Data Entry Error";

                  if (!cFmt.LoadString(IDS_INVALIDDATEMESSAGE))
                    cFmt = "%s\x0D\x0A Please Enter A Valid Date.";

                  sprintf(caBuf,(LPCSTR)cFmt,(LPCSTR)cErrMsg);
                  ::MessageBox(NULL,caBuf,(LPCSTR)cTitle,MB_ICONEXCLAMATION|MB_OK);
                  bRetval = FALSE;
                }
            }
        }
    }

  return bRetval;
}



void MatrixGrid::OnKeyDown(UINT *vcKey, int processed)
{
  if (*vcKey == VK_DELETE)
    {
      int  iStartCol, iEndCol;
      long lStartRow, lEndRow;

      iStartCol = iEndCol = GetCurrentCol();
      lStartRow = lEndRow = GetCurrentRow();

      m_GI->m_multiSelect->GetTotalRange(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
      // m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);

      if (m_iParentType == 0)
        {
          if (iStartCol == 0 && iEndCol == (GetNumberCols()-1))  // whole row(s) selected
            ((FullScreenGrid *)GetParent())->DeleteSelectedRows();
          else
            ((FullScreenGrid *)GetParent())->WildcardSelectedCells();
        }
      else if (m_iParentType == 1)
        {
          if (iStartCol == 0 && iEndCol == (GetNumberCols()-1))  // whole row(s) selected
            ((MatrixEditData *)GetParent())->DeleteSelectedRows();
          else
            ((MatrixEditData *)GetParent())->WildcardSelectedCells();
        }
      else if (m_iParentType == 2)
        {
          if (iStartCol == 0 && iEndCol == (GetNumberCols()-1))  // whole row(s) selected
            ((FloatingValuesPage *)GetParent())->DeleteSelectedRows();
          else
            ((FloatingValuesPage *)GetParent())->WildcardSelectedCells();
        }
    }
  else
    CUGExcelCtrl::OnKeyDown(vcKey,processed);
}



void MatrixGrid::ValueToDateStr(long lValue, char *cpDateBuf)
{
  char caSep[21];
  char caYearFmt[5];
  char caFmt[41];
  int iFormat = m_iDateFormat;

  int iDay, iMonth, iYear;
  char caBuf[21], caBuf2[21];

  ltoa(lValue,caBuf2,10);
  memcpy(caBuf,caBuf2,4);
  caBuf[4] = 0;
  iYear = atoi(caBuf);
  memcpy(caBuf,caBuf2+4,2);
  caBuf[2] = 0;
  iMonth = atoi(caBuf);
  iDay = atoi(caBuf2+6);

  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDATE,caSep,sizeof(caSep));

  if (iFormat > 99)
    {
      iFormat -= 100;
      strcpy(caYearFmt,"%04d");
    }
  else
    strcpy(caYearFmt,"%02d");

  switch (iFormat)
    {
      case 0:  // mdy
        sprintf(caFmt,"%%02d%s%%02d%s%s",caSep,caSep,caYearFmt);
        sprintf(caBuf,caFmt,iMonth,iDay,iYear);
        break;

      case 1:  // dmy
        sprintf(caFmt,"%%02d%s%%02d%s%s",caSep,caSep,caYearFmt);
        sprintf(caBuf,caFmt,iDay,iMonth,iYear);
        break;

      case 2:  // YMD
        sprintf(caFmt,"%s%s%%02d%s%%02d",caYearFmt,caSep,caSep);
        sprintf(caBuf,caFmt,iYear,iMonth,iDay);
        break;
    }

  strcpy(cpDateBuf,caBuf);
}



long MatrixGrid::DateStrToValue(LPCSTR cpDateBuf)
{
  char caSep[21];
  int iDay = 0, iMonth = 0, iYear = 0;
  char caBuf[81];
  int iFormat = m_iDateFormat;

  if (iFormat > 99)
    iFormat -= 100;

  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDATE,caSep,sizeof(caSep));

  char *p = strstr(cpDateBuf,caSep);
  if (p)
    {
      memcpy(caBuf,cpDateBuf,p-cpDateBuf);
      caBuf[cpDateBuf,p-cpDateBuf] = 0;

      if (iFormat == 0)
        iMonth = atoi(caBuf);
      else if (iFormat == 1)
        iDay = atoi(caBuf);
      else if (iFormat == 2)
        iYear = atoi(caBuf);

      strcpy(caBuf,p+strlen(caSep));
      p = strstr(caBuf,caSep);
      if (p)
        {
          *p = 0;

          if (iFormat == 1 || iFormat == 2)
            iMonth = atoi(caBuf);
          else if (iFormat == 0)
            iDay = atoi(caBuf);

          strcpy(caBuf,p+strlen(caSep));

          if (iFormat == 0 || iFormat == 1)
            iYear = atoi(caBuf);
          else if (iFormat == 2)
            iDay = atoi(caBuf);
        }
    }

  if (iYear < 1000 && iMonth && iDay)
    {
      if (iYear > 49)
        iYear += 1900;
      else
        iYear += 2000;
    }

  long lRetval = ((iYear * 10000) + (iMonth * 100) + iDay);

  return lRetval;
}
