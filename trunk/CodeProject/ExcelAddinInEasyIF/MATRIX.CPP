#include "stdafx.h"
#include "resource.h"
#include "matrix.h"

IMPLEMENT_SERIAL(MatrixColDef, CObject, 1)
IMPLEMENT_SERIAL(MatrixColumnValue, CObject, 1)
IMPLEMENT_SERIAL(MatrixRow, CObArray, 1)
IMPLEMENT_SERIAL(Matrix, CObject, 1)


static int cDaysInMonth[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };


long JulianFromDate(int iYear, int iMonth, int iDay)
{
    long JDate = 0;

    // Add in days for months already elapsed.
    for( int i = 0; i < iMonth - 1; ++i )
        JDate += cDaysInMonth[i];

    // Add in days for this month.
    JDate += iDay;

    // Check for leap year.
    if( iYear % 100 != 0 && iYear % 4 == 0 )
        JDate++;

    // Add in year.
    JDate += ((iYear-1) * 365);

    return JDate;
}



long JulianToMatrixDate(long lDate)
{
  int iMonth = 0;
  int iDay;

  int iYear = lDate / 365;

  lDate -= (iYear * 365);
  iYear++;

  while (lDate > cDaysInMonth[iMonth])
    {
      lDate -= cDaysInMonth[iMonth];

      if(iMonth == 1 && iYear % 100 != 0 && iYear % 4 == 0 )
        lDate--;

      iMonth++;
    }

  iMonth++;

  iDay = lDate-25;

  return ((iYear * 10000) + (iMonth * 100) + iDay);
}



char *StringBinaryEncrypt(char *cpString)
{
  char far *cpKey = "Achilles";
  char far *q = cpKey;
  char far *p;
  char caBuf[256];

  int iLen = strlen(cpString);
  if (iLen > 255)
    {
      *(cpString+255) = 0;
      iLen = 255;
    }

  memcpy(caBuf,cpString,iLen);
  caBuf[iLen] = 0;

  for (p = (char *)&caBuf[0]; *p; p++)
    {
      if (*p != *q)
        *p = *p^*q;
      q++;
      if (*q == 0)
        q = cpKey;
    }

  memcpy(cpString,caBuf,iLen);

  return cpString;
}



void StringHexEncrypt(CString *pString)
{
  char caBuf[256];
  char caResult[512];
  char caByteBuf[3];

  int iLen = pString->GetLength();
  if (iLen >= sizeof(caBuf))
    iLen = sizeof(caBuf)-1;

  memcpy(caBuf,(LPCSTR)(*pString),iLen);
  caBuf[iLen] = 0;

  StringBinaryEncrypt(caBuf);

  caResult[0] = 0;

  for (int iLup = 0; iLup < iLen; iLup++)
    {
      sprintf(caByteBuf,"%02X",(char)caBuf[iLup]);
      strcat(caResult,caByteBuf);
    }

  *pString = caResult;
}



void StringHexDecrypt(CString *pString)
{
  char caHexBuf[512], caBuf[256];
  char ch;
  char *p;

  int iLen = pString->GetLength();
  if (iLen >= sizeof(caHexBuf))
    iLen = sizeof(caHexBuf)-1;

  memcpy(caHexBuf,(LPCSTR)(*pString),iLen);
  caHexBuf[iLen] = 0;

  caBuf[0] = 0;
  iLen /= 2;

  for (int iLup = 0; iLup < iLen; iLup++)
    {
      p = &caHexBuf[iLup*2];

      if (*p < 58)
        ch = *p - '0';
      else
        ch = *p - '7';

      ch <<= 4;

      p++;

      if (*p < 58)
        ch += *p - '0';
      else
        ch += *p - '7';

      caBuf[iLup] = ch;
    }

  /* reverse the encryption */
  StringBinaryEncrypt(caBuf);

  caBuf[iLen] = 0;

  *pString = caBuf;
}





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

MatrixColDef::MatrixColDef()
{
  m_iColType = COLTYPE_STRING;

  if (!m_cColName.LoadString(IDS_NEWCOLUMN))
    m_cColName = "New Column";

  m_bCompareCol = TRUE;
  m_iCompareOp = COMPAREOP_EQ;

  m_bUsePickList = FALSE;
  m_bRestrictToList = FALSE;
}



void MatrixColDef::Serialize(CArchive & archive)
{
  WORD wMagicNum;

  CObject::Serialize(archive);

  if (archive.IsStoring())
    {
      archive << (WORD)0x4344; // CD
      archive << (WORD)m_iColType << m_cColName << m_cNotes << (WORD)m_bCompareCol;
      archive << (WORD)m_iCompareOp << (WORD)m_bUsePickList << (WORD)m_bRestrictToList;
    }
  else
    {
      archive >> (WORD&)wMagicNum;
      if (wMagicNum != 0x4344)
        {
          TRACE("Bad id number in front of a MatrixColDef item\n");
          AfxThrowArchiveException(CArchiveException::generic);
        }

      archive >> (WORD&)m_iColType >> m_cColName >> m_cNotes >> (WORD&)m_bCompareCol;
      archive >> (WORD&)m_iCompareOp >> (WORD&)m_bUsePickList >> (WORD&)m_bRestrictToList;
    }

  m_oPickListChoices.Serialize(archive);
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

MatrixColumnValue::MatrixColumnValue()
{
  m_bIsWildCard = TRUE;

  m_iColType = COLTYPE_STRING;

  m_dValue = 0.0;
  m_lValue = 0;
}



MatrixColumnValue::MatrixColumnValue(MatrixColDef *pColDef, LPCSTR cpInitialValue /*=NULL*/)
{
  if (pColDef)
    m_iColType = pColDef->m_iColType;
  else
    m_iColType = COLTYPE_STRING;

  m_bIsWildCard = TRUE;

  m_dValue = 0.0;
  m_lValue = 0;

  if (cpInitialValue && *cpInitialValue)
    ValueFromString(cpInitialValue);
}



void MatrixColumnValue::SetColumnType(WORD iColType)
{
  if (iColType == m_iColType)
    return;

  WORD iOldColType = m_iColType;
  m_iColType = iColType;

  switch (iOldColType)
    {
      case COLTYPE_STRING:
        ValueFromString(m_cValue);
        break;
      case COLTYPE_INTEGER:
        ValueFromInteger(m_lValue);
        break;
      case COLTYPE_DATE:
        ValueFromDate(m_lValue);
        break;
      case COLTYPE_DOUBLE:
        ValueFromDouble(m_dValue);
        break;
      case COLTYPE_FORMULA:
        ValueFromFormula(m_cValue);
        break;
    }
}



void MatrixColumnValue::ValueFromFormula(LPCSTR cpValue)
{
  if (m_iColType == COLTYPE_FORMULA)
    m_cValue = cpValue;
  else
    ValueFromString(cpValue);
}



void MatrixColumnValue::ValueFromString(LPCSTR cpValue)
{
  if (*cpValue == 0)
    m_bIsWildCard = TRUE;

  switch (m_iColType)
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        m_cValue = cpValue;
        break;

      case COLTYPE_INTEGER:
        m_lValue = atol(cpValue);
        break;

      case COLTYPE_DOUBLE:
        m_dValue = atof(cpValue);
        break;

      case COLTYPE_DATE:
        // assuming MM/DD/YYYY format
        {
        int iYear, iMonth, iDay;
        char *cpSepPos, *cpSepPos2;
        char caBuf[11];

        iYear = iMonth = iDay = 0;

        cpSepPos = strchr(cpValue,'/');
        if (cpSepPos == NULL)
          cpSepPos = strchr(cpValue,'-');

/// INTERNATIONALIZE
        if (cpSepPos)
          {
            memcpy(caBuf,cpValue,cpSepPos-cpValue);
            caBuf[cpSepPos-cpValue] = 0;
            iMonth = atoi(caBuf);

            cpSepPos2 = strchr(cpSepPos+1,'/');
            if (cpSepPos2 == NULL)
              cpSepPos2 = strchr(cpSepPos+1,'-');

            if (cpSepPos2)
              {
                memcpy(caBuf,cpSepPos+1,cpSepPos2-cpSepPos-1);
                caBuf[cpSepPos2-cpSepPos-1] = 0;
                iDay = atoi(caBuf);

                strcpy(caBuf,cpSepPos2+1);
                iYear = atoi(caBuf);
              }
            else
              iMonth =  0;  // bad date format
          }
        else
          {
            if (strlen(cpValue) == 8)
              {
                // assume YYYYMMDD format
                memcpy(caBuf,cpValue,4);
                caBuf[4] = 0;
                iYear = atoi(caBuf);
                memcpy(caBuf,cpValue+4,2);
                caBuf[2] = 0;
                iMonth = atoi(caBuf);
                memcpy(caBuf,cpValue+6,2);
                iDay = atoi(caBuf);
              }
            else
              {
                // assume YYMMDD format
                memcpy(caBuf,cpValue,2);
                caBuf[2] = 0;
                iYear = atoi(caBuf);
                memcpy(caBuf,cpValue+2,2);
                caBuf[2] = 0;
                iMonth = atoi(caBuf);
                memcpy(caBuf,cpValue+4,2);
                iDay = atoi(caBuf);
              }
          }

        if (iYear < 1900 && iMonth && iDay)
          {
            if (iYear > 49)
              iYear += 1900;
            else
              iYear += 2000;
          }

        m_lValue = (iYear * 10000) + (iMonth * 100) + iDay;

        m_bIsWildCard = m_lValue > 0 ? FALSE : TRUE;
        }
        break;
    }
}



void MatrixColumnValue::ValueFromInteger(long lValue)
{
  char caBuf[41];

  switch (m_iColType)
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        ltoa(lValue,caBuf,10);
        m_cValue = caBuf;
        break;

      case COLTYPE_DATE:
        m_lValue = lValue;
        m_bIsWildCard = m_lValue > 0 ? FALSE : TRUE;
        break;

      case COLTYPE_INTEGER:
        m_lValue = lValue;
        break;

      case COLTYPE_DOUBLE:
        m_dValue = lValue;
        break;
    }
}



void MatrixColumnValue::ValueFromDouble(double dValue)
{
  char caBuf[41];
  int iOffset;

  switch (m_iColType)
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        sprintf(caBuf,"%10.10f",dValue);
        iOffset = strlen(caBuf)-1;
        while (iOffset > -1)
          {
            if (caBuf[iOffset] != '0')
              {
                if (caBuf[iOffset] == '.')
                  caBuf[iOffset] = 0;

                break;
              }

            caBuf[iOffset--] = 0;
          }

        m_cValue = caBuf;
        break;

      case COLTYPE_DATE:
        if (dValue < 18000101)  // excel date format
          {
            m_lValue = JulianToMatrixDate(JulianFromDate(1899,12,30) + (long)dValue);
          }
        else
          m_lValue = (long)dValue;
        break;

      case COLTYPE_INTEGER:
        m_lValue = (long)dValue;
        break;

      case COLTYPE_DOUBLE:
        m_dValue = dValue;
        break;
    }
}



void MatrixColumnValue::ValueFromDate(long lValue)
{
  char caBuf[41];
  char caBuf2[41];
  int iYear, iMonth, iDay;

  switch (m_iColType)
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        ltoa(lValue,caBuf2,10);
        memcpy(caBuf,caBuf2,4);
        caBuf[4] = 0;
        iYear = atoi(caBuf);
        memcpy(caBuf,caBuf2+4,2);
        caBuf[2] = 0;
        iMonth = atoi(caBuf);
        iDay = atoi(caBuf2+6);

/// INTERNATIONALIZE
        sprintf(caBuf,"%02d/%02d/%04d",iMonth,iDay,iYear);
        m_cValue = caBuf;

        break;

      case COLTYPE_DATE:
        m_lValue = lValue;
        m_bIsWildCard = m_lValue > 0 ? FALSE : TRUE;
        break;

      case COLTYPE_INTEGER:
        m_lValue = lValue;
        break;

      case COLTYPE_DOUBLE:
        m_dValue = lValue;
        break;
    }
}



void MatrixColumnValue::Serialize(CArchive & archive)
{
  WORD wMagicNum;

  CObject::Serialize(archive);

  if (archive.IsStoring())
    {
      archive << (WORD)0x4356; // CV
      archive << (WORD)m_iColType << (WORD)m_bIsWildCard << m_cValue;
      archive << (double)m_dValue << (DWORD)m_lValue;
    }
  else
    {
      archive >> (WORD&)wMagicNum;
      if (wMagicNum != 0x4356)
        {
          TRACE("Bad id number in front of a MatrixColumnValue item\n");
          AfxThrowArchiveException(CArchiveException::generic);
        }

      archive >> (WORD&)m_iColType >> (WORD&)m_bIsWildCard >> m_cValue;
      archive >> (double&)m_dValue >> (DWORD&)m_lValue;
    }
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


MatrixRow::MatrixRow(Matrix *pMatrix)
{
  if (pMatrix)
    {
      MatrixColDef *pColDef;

      int iColCount = pMatrix->GetColumnCount();

      for (int iLup = 0; iLup < iColCount; iLup++)
        {
          pColDef = pMatrix->GetColumnDef(iLup);
          AddColumn(pColDef);
        }

      m_oReturnValue.SetColumnType(pMatrix->GetReturnValueColumnType());
    }
}



MatrixRow::~MatrixRow()
{
  MatrixColumnValue *pColVal;

  int iCount = GetSize();

  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pColVal = (MatrixColumnValue *)GetAt(iLup);
      if (pColVal)
        delete pColVal;
    }
}



MatrixColumnValue *MatrixRow::AddColumn(MatrixColDef *pColDef, LPCSTR cpInitialValue /*=NULL*/)
{
  MatrixColumnValue *pRetval = new MatrixColumnValue(pColDef,cpInitialValue);

  if (pRetval)
    Add(pRetval);

  return pRetval;
}


void MatrixRow::InsertColumn(int iInsertBefore, MatrixColDef *pColDef, LPCSTR cpInitialValue /*=NULL*/)
{
  if (iInsertBefore >= GetSize())
    AddColumn(pColDef,cpInitialValue);
  else
    {
      MatrixColumnValue *pRetval = new MatrixColumnValue(pColDef,cpInitialValue);

      if (pRetval)
        InsertAt(iInsertBefore,pRetval);
    }
}



void MatrixRow::DeleteColumn(int iColIndex)
{
  if (iColIndex > -1 && iColIndex < GetSize())
    {
      MatrixColumnValue *pColVal = (MatrixColumnValue *)GetAt(iColIndex);
      if (pColVal)
        delete pColVal;

      RemoveAt(iColIndex);
    }
}



void MatrixRow::SetColumnType(int iColIndex, WORD iColType)
{
  if (iColIndex > -1 && iColIndex < GetSize())
    {
      MatrixColumnValue *pColVal = (MatrixColumnValue *)GetAt(iColIndex);
      if (pColVal)
        pColVal->SetColumnType(iColType);
    }
}



void MatrixRow::MoveColumn(int iColIndex, int iDirection)
{
  int iColIndex2 = iColIndex + iDirection;

  MatrixColumnValue *pColVal = (MatrixColumnValue *)GetAt(iColIndex);
  MatrixColumnValue *pColVal2 = (MatrixColumnValue *)GetAt(iColIndex2);

  SetAt(iColIndex,pColVal2);
  SetAt(iColIndex2,pColVal);
}



MatrixColumnValue *MatrixRow::GetCell(int iColIndex)
{
  MatrixColumnValue *pColVal = NULL;

  if (iColIndex > -1 && iColIndex < GetSize())
    pColVal = (MatrixColumnValue *)GetAt(iColIndex);

  return pColVal;
}



void MatrixRow::Serialize(CArchive & archive)
{
  CObArray::Serialize(archive);

  m_oReturnValue.Serialize(archive);
}




long MatrixRow::ComputeMatchWeight(MatrixRow *pRowToMatch, CObArray *pColDefs)
{
  double dDiff;
  int iCompare;
  long lWeight = 0;

  int iCount = GetSize();
  MatrixColumnValue *pMyCol, *pMatchCol;
  MatrixColDef *pColDef;

  if (iCount > pColDefs->GetSize())
    iCount = pColDefs->GetSize();

  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pColDef = (MatrixColDef *)pColDefs->GetAt(iLup);

      if (!pColDef->m_bCompareCol)
        continue;

      pMyCol = (MatrixColumnValue *)GetAt(iLup);
      pMatchCol = (MatrixColumnValue *)pRowToMatch->GetAt(iLup);

      if (pMyCol->IsWildCard() || pMatchCol->IsWildCard() || (pMyCol->GetColumnType() == COLTYPE_FORMULA))
        {
          lWeight++;
        }
      else
        {
          if (pMyCol->GetColumnType() != pMatchCol->GetColumnType())
            pMatchCol->SetColumnType(pMyCol->GetColumnType());

          switch (pMyCol->GetColumnType())
            {
              case COLTYPE_STRING:
                iCompare = strcmp(pMatchCol->m_cValue,pMyCol->m_cValue);
                break;

              case COLTYPE_INTEGER:
              case COLTYPE_DATE:
                iCompare = pMatchCol->m_lValue - pMyCol->m_lValue;
                break;

              case COLTYPE_DOUBLE:
                dDiff = pMatchCol->m_dValue - pMyCol->m_dValue;
                iCompare = 0;

                if (dDiff > 0.0001)
                  iCompare = 1;

                if (dDiff < -0.0001)
                  iCompare = -1;
                break;

              default:
                iCompare = 0;
                break;
            }

          switch (pColDef->m_iCompareOp)
            { // 0 - =, 1 - <, 2 - <=, 3 - >, 4 - >=, 5 - <>
              case COMPAREOP_EQ:
                if (iCompare)
                  return 0;
                break;
              case COMPAREOP_LT:
                if (iCompare >= 0)
                  return 0;
                break;
              case COMPAREOP_LE:
                if (iCompare > 0)
                  return 0;
                break;
              case COMPAREOP_GT:
                if (iCompare <= 0)
                  return 0;
                break;
              case COMPAREOP_GE:
                if (iCompare < 0)
                  return 0;
                break;
              case COMPAREOP_NE:
                if (iCompare == 0)
                  return 0;
                break;
              default:
                return 0;
                break;
            }

          lWeight += CalcColumnValue(iCount-iLup);
        }
    }

  return lWeight;
}



long MatrixRow::CalcColumnValue(int iColumn)
{
  long lRetval = 100;

  for (int iLup = 1; iLup < iColumn; iLup++)
    lRetval *= 10;

  return lRetval;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(LPCSTR cpMatrixName /*=NULL*/)
{
  if (cpMatrixName && *cpMatrixName)
    m_cName = cpMatrixName;
  else
    {
      if (!m_cName.LoadString(IDS_NEWMATRIXNAME))
        m_cName = "New - Untitled";
    }

  m_iSecurityFlags = 0;

  m_iReturnValueColType = COLTYPE_DOUBLE;

  m_bModifiedFlag = FALSE;
  m_iSecurityPassedFlags = 0;

  m_bNewMatrix = TRUE;
}



Matrix::Matrix(LPCSTR cpMatrixName, Matrix *pMatrix)
{
  if (cpMatrixName && *cpMatrixName)
    m_cName = cpMatrixName;
  else
    {
      if (!m_cName.LoadString(IDS_NEWMATRIXNAME))
        m_cName = "New - Untitled";
    }


  m_bModifiedFlag = TRUE;
  m_iSecurityPassedFlags = 0;

  m_bNewMatrix = TRUE;

  m_cNotes = pMatrix->m_cNotes;

  if (!m_cCreator.LoadString(IDS_MATRIXCOPIED))
    m_cCreator = "copied";

  m_cCategory = pMatrix->m_cCategory;

  m_iSecurityFlags = pMatrix->m_iSecurityFlags;
  m_cAccessPassword = pMatrix->m_cAccessPassword;
  m_cModifyPassword = pMatrix->m_cModifyPassword;

  m_iReturnValueColType = pMatrix->m_iReturnValueColType;

  // copy columns
  MatrixColDef *pColDef;
  MatrixColDef *pNewColDef;
  int iColCount = pMatrix->GetColumnCount();
  for (int iLup = 0; iLup < iColCount; iLup++)
    {
      pColDef = pMatrix->GetColumnDef(iLup);

      pNewColDef = new MatrixColDef;
      pNewColDef->m_iColType = pColDef->m_iColType;
      pNewColDef->m_cColName = pColDef->m_cColName;
      pNewColDef->m_cNotes = pColDef->m_cNotes;
      pNewColDef->m_bCompareCol = pColDef->m_bCompareCol;
      pNewColDef->m_iCompareOp = pColDef->m_iCompareOp;
      pNewColDef->m_bUsePickList = pColDef->m_bUsePickList;
      pNewColDef->m_bRestrictToList = pColDef->m_bRestrictToList;

      if (pColDef->m_oPickListChoices.GetSize() > 0)
        {
          for (int iElem = 0; iElem < pColDef->m_oPickListChoices.GetSize(); iElem++)
            pNewColDef->m_oPickListChoices.Add(pColDef->m_oPickListChoices.GetAt(iElem));
        }

      m_oColumnDefs.Add(pNewColDef);
    }

  // copy rows
  MatrixRow *pRow;
  MatrixRow *pNewRow;
  MatrixColumnValue *pColVal;
  MatrixColumnValue *pNewColVal;

  int iRowCount = pMatrix->GetRowCount();
  for (iLup = 0; iLup < iRowCount; iLup++)
    {
      pRow = pMatrix->GetRow(iLup);

      pNewRow = new MatrixRow(this);

      for (int iVal = 0; iVal < iColCount; iVal++)
        {
          pColVal = pRow->GetCell(iVal);
          pNewColVal = pNewRow->GetCell(iVal);

          pNewColVal->m_bIsWildCard = pColVal->m_bIsWildCard;
          pNewColVal->m_iColType = pColVal->m_iColType;
          pNewColVal->m_cValue = pColVal->m_cValue;
          pNewColVal->m_dValue = pColVal->m_dValue;
          pNewColVal->m_lValue = pColVal->m_lValue;
        }

      pColVal = pRow->GetReturnValue();
      pNewColVal = pNewRow->GetReturnValue();

      pNewColVal->m_bIsWildCard = pColVal->m_bIsWildCard;
      pNewColVal->m_iColType = pColVal->m_iColType;
      pNewColVal->m_cValue = pColVal->m_cValue;
      pNewColVal->m_dValue = pColVal->m_dValue;
      pNewColVal->m_lValue = pColVal->m_lValue;

      m_oRows.Add(pNewRow);
    }
}



Matrix::~Matrix()
{
  MatrixColDef *pColDef;

  int iCount = m_oColumnDefs.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pColDef = (MatrixColDef *)m_oColumnDefs.GetAt(iLup);
      if (pColDef)
        delete pColDef;
    }

  MatrixRow *pRow;
  iCount = m_oRows.GetSize();
  for (iLup = 0; iLup < iCount; iLup++)
    {
      pRow = (MatrixRow *)m_oRows.GetAt(iLup);
      if (pRow)
        delete pRow;
    }
}



void Matrix::SetMatrixName(LPCSTR cpMatrixName)
{
  m_cName = cpMatrixName;

  if (m_cFileName.IsEmpty())
    m_cFileName = m_cName;

  SetModified();
}



MatrixColDef *Matrix::GetColumnDef(int iColIndex)
{
  MatrixColDef *pColDef = NULL;

  if (iColIndex > -1 && iColIndex < GetColumnCount())
    pColDef = (MatrixColDef *)m_oColumnDefs.GetAt(iColIndex);

  return pColDef;
}



MatrixColDef *Matrix::AddColumn(LPCSTR cpInitialValue /*=NULL*/)
{
  MatrixColDef *pColDef = new MatrixColDef;

  if (pColDef)
    {
      m_oColumnDefs.Add(pColDef);

      MatrixRow *pRow;
      int iCount = m_oRows.GetSize();
      for (int iLup = 0; iLup < iCount; iLup++)
        {
          pRow = (MatrixRow *)m_oRows.GetAt(iLup);
          if (pRow)
            pRow->AddColumn(pColDef,cpInitialValue);
        }
    }

  SetModified();

  return pColDef;
}



MatrixColDef *Matrix::InsertColumn(int iInsertBefore, LPCSTR cpInitialValue /*=NULL*/)
{
  if (iInsertBefore >= m_oColumnDefs.GetSize())
    return AddColumn(cpInitialValue);

  MatrixColDef *pColDef = new MatrixColDef;

  if (pColDef)
    {
      m_oColumnDefs.InsertAt(iInsertBefore,pColDef);

      MatrixRow *pRow;
      int iCount = m_oRows.GetSize();
      for (int iLup = 0; iLup < iCount; iLup++)
        {
          pRow = (MatrixRow *)m_oRows.GetAt(iLup);
          if (pRow)
            pRow->InsertColumn(iInsertBefore,pColDef,cpInitialValue);
        }
    }

  SetModified();

  return pColDef;
}



void Matrix::DeleteColumn(int iColIndex)
{
  if (iColIndex > -1 && iColIndex < m_oColumnDefs.GetSize())
    {
      MatrixColDef *pColDef = (MatrixColDef *)m_oColumnDefs.GetAt(iColIndex);
      if (pColDef)
        delete pColDef;

      m_oColumnDefs.RemoveAt(iColIndex);

      MatrixRow *pRow;
      int iCount = m_oRows.GetSize();
      for (int iLup = 0; iLup < iCount; iLup++)
        {
          pRow = (MatrixRow *)m_oRows.GetAt(iLup);
          if (pRow)
            pRow->DeleteColumn(iColIndex);
        }
    }

  SetModified();
}



void Matrix::SetColumnType(int iColIndex, WORD iColType)
{
  MatrixColDef *pColDef = GetColumnDef(iColIndex);
  if (!pColDef || pColDef->m_iColType == iColType)
    return;

  pColDef->m_iColType = iColType;

  MatrixRow *pRow;
  int iCount = m_oRows.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pRow = (MatrixRow *)m_oRows.GetAt(iLup);
      if (pRow)
        pRow->SetColumnType(iColIndex,iColType);
    }

  SetModified();
}



void Matrix::SetColumnType(MatrixColDef *pColDef, WORD iColType)
{
  int iColIndex = -1;
  MatrixColDef *pListDef;

  int iCount = m_oColumnDefs.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pListDef = (MatrixColDef *)m_oColumnDefs.GetAt(iLup);
      if (pListDef == pColDef)
        {
          iColIndex = iLup;
          break;
        }
    }

  if (iColIndex > -1)
    SetColumnType(iColIndex,iColType);
}



MatrixRow *Matrix::AddRow()
{
  MatrixRow *pRow = new MatrixRow(this);
  if (pRow)
    m_oRows.Add(pRow);

  SetModified();

  return pRow;
}



MatrixRow *Matrix::InsertRow(int iInsertBefore)
{
  int iCount = m_oRows.GetSize();
  if (iInsertBefore >= iCount)
    return AddRow();

  MatrixRow *pRow = new MatrixRow(this);
  if (pRow)
    {
      m_oRows.Add(NULL);
      for (int iLup = iCount; iLup > iInsertBefore; iLup--)
        m_oRows.SetAt(iLup,m_oRows.GetAt(iLup-1));

      m_oRows.SetAt(iInsertBefore,pRow);
    }

  SetModified();

  return pRow;
}



MatrixRow *Matrix::GetRow(int iRowIndex)
{
  MatrixRow *pRow = NULL;

  if (iRowIndex > -1 && iRowIndex < m_oRows.GetSize())
    pRow = (MatrixRow *)m_oRows.GetAt(iRowIndex);

  return pRow;
}



void Matrix::DeleteRow(int iRowIndex)
{
  MatrixRow *pRow = GetRow(iRowIndex);
  if (pRow)
    {
      delete pRow;
      m_oRows.RemoveAt(iRowIndex);
    }

  SetModified();
}



MatrixColumnValue *Matrix::GetCell(int iRowIndex, int iColIndex)
{
  MatrixColumnValue *pColVal = NULL;

  MatrixRow *pRow = GetRow(iRowIndex);
  if (pRow)
    pColVal = pRow->GetCell(iColIndex);

  return pColVal;
}



void Matrix::SetReturnValueColumnType(WORD iColType)
{
  if (m_iReturnValueColType == iColType)
    return;

  m_iReturnValueColType = iColType;

  MatrixRow *pRow;
  int iCount = m_oRows.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pRow = (MatrixRow *)m_oRows.GetAt(iLup);
      if (pRow)
        pRow->GetReturnValue()->SetColumnType(iColType);
    }

  SetModified();
}



void Matrix::Serialize(CArchive & archive)
{
  WORD wMagicNum;
  CString cAP, cMP;

  CObject::Serialize(archive);

  if (archive.IsStoring())
    {
      archive << (WORD)0x4D58; // MX
      archive << m_cName << m_cNotes << m_cCreator << m_cCategory << (WORD)m_iSecurityFlags;

      cAP = m_cAccessPassword;
      cMP = m_cModifyPassword;

      if (!cAP.IsEmpty())
        StringHexEncrypt(&cAP);
      if (!cMP.IsEmpty())
        StringHexEncrypt(&cMP);

      archive << cAP << cMP << (WORD)m_iReturnValueColType;
    }
  else
    {
      archive >> (WORD&)wMagicNum;
      if (wMagicNum != 0x4D58)
        {
          TRACE("Bad id number in front of a Matrix item\n");
          AfxThrowArchiveException(CArchiveException::generic);
        }

      archive >> m_cName >> m_cNotes >> m_cCreator >> m_cCategory >> (WORD&)m_iSecurityFlags;
      archive >> m_cAccessPassword >> m_cModifyPassword >> (WORD&)m_iReturnValueColType;

      if (!m_cAccessPassword.IsEmpty())
        StringHexDecrypt(&m_cAccessPassword);
      if (!m_cModifyPassword.IsEmpty())
        StringHexDecrypt(&m_cModifyPassword);
    }

  m_oColumnDefs.Serialize(archive);
  m_oRows.Serialize(archive);

  SetModified(FALSE);
}



MatrixColumnValue *Matrix::GetMatch(MatrixRow *pRowToMatch, MatrixRow **pBestMatch /*=NULL*/)
{
  MatrixRow *pBestRow = NULL;
  long lBestWeight = 0;
  long lWeight;

  MatrixRow *pRow;
  int iCount = m_oRows.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pRow = (MatrixRow *)m_oRows.GetAt(iLup);
      if (pRow)
        {
          lWeight = pRow->ComputeMatchWeight(pRowToMatch,&m_oColumnDefs);
          if (lWeight > lBestWeight)
            {
              lBestWeight = lWeight;
              pBestRow = pRow;
            }
        }
    }

  if (pBestMatch)
    *pBestMatch = pBestRow;

  if (pBestRow)
    return pBestRow->GetReturnValue();
  else
    return NULL;
}



void Matrix::GetDateMask(char *cpMaskBuf)
{
  char caBuf[128];
  char *p;

  strcpy(cpMaskBuf,"99/99/9999"); // default mask

  ::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SSHORTDATE,caBuf,sizeof(caBuf));
  p = strstr(caBuf,"dd");
  if (p)
    memcpy(p,"00",2);
  else
    {
      p = strchr(caBuf,'d');
      if (p)
        {
          *p = '0';
          memmove(p+1,p,strlen(p)+1);
          *p = '9';
        }
      else
        return;
   }

  p = strstr(caBuf,"MM");
  if (p)
    memcpy(p,"00",2);
  else
    {
      p = strchr(caBuf,'M');
      if (p)
        {
          *p = '0';
          memmove(p+1,p,strlen(p)+1);
          *p = '9';
        }
      else
        return;
    }

  p = strstr(caBuf,"yyyy");
  if (p)
    memcpy(p,"0000",4);
  else
    {
      p = strstr(caBuf,"yy");
      if (p)
        {
          memcpy(p,"00",2);
        }
      else
        return;
    }

  strcpy(cpMaskBuf,caBuf);
}



void Matrix::MoveColumn(int iColIndex, int iDirection)
{
  if (iColIndex >= m_oColumnDefs.GetSize())
    return;

  if (iDirection != -1 && iDirection != 1)
    return;

  int iColIndex2 = iColIndex + iDirection;

  MatrixColDef *pColDef = (MatrixColDef *)m_oColumnDefs.GetAt(iColIndex);
  MatrixColDef *pColDef2 = (MatrixColDef *)m_oColumnDefs.GetAt(iColIndex2);

  m_oColumnDefs.SetAt(iColIndex,pColDef2);
  m_oColumnDefs.SetAt(iColIndex2,pColDef);

  MatrixRow *pRow;
  int iCount = m_oRows.GetSize();
  for (int iLup = 0; iLup < iCount; iLup++)
    {
      pRow = (MatrixRow *)m_oRows.GetAt(iLup);
      if (pRow)
        pRow->MoveColumn(iColIndex,iDirection);
    }

  SetModified();
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static char cpRegKey[] = "Software\\Webogy\\EasyIF";
static char cpGridDirectoryKey[] = "GridDirectory";

MatrixManager::MatrixManager()
{
  // load directory from registry key

  HKEY hRegKey;

  if (!::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                      cpRegKey,
                      0,
                      KEY_READ,
                      &hRegKey))
    {
      char caBuf[256];

      DWORD dwSize = sizeof(caBuf)-1;
      caBuf[0] = 0;
      ::RegQueryValueEx(hRegKey,cpGridDirectoryKey,NULL,NULL,(unsigned char *)caBuf,&dwSize);
      m_cMatrixDirectory = caBuf;

      ::RegCloseKey(hRegKey);
    }
  else
    {
      // assume it is not set up, so try to create the default
      SetMatrixDirectory("C:");
    }
}



MatrixManager::~MatrixManager()
{
  DeleteAllMatrices();
}



void MatrixManager::DeleteAllMatrices()
{
  CString cDummy;
  Matrix *pMatrix;

  POSITION pPos = m_oMatrices.GetStartPosition();

  while (pPos)
    {
      pMatrix = NULL;
      m_oMatrices.GetNextAssoc(pPos,cDummy,(CObject *&)pMatrix);
      if (pMatrix)
        delete pMatrix;
    }

  m_oMatrices.RemoveAll();
}



Matrix *MatrixManager::GetMatrix(LPCSTR cpMatrixName)
{
  Matrix *pMatrix = NULL;

  if (m_oMatrices.Lookup(cpMatrixName,(CObject *&)pMatrix))
    return pMatrix;
  else
    return NULL;
}



BOOL MatrixManager::LoadMatrices()
{
  BOOL bRetval = TRUE;

  DeleteAllMatrices();

  CString cFileSpec = m_cMatrixDirectory + CString("\\*.XLG");

  CString cFileName;
  WIN32_FIND_DATA oFD;

  HANDLE hFind = FindFirstFile((LPCSTR)cFileSpec,&oFD);
  if (hFind != INVALID_HANDLE_VALUE)
    {
      if (!(oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        bRetval = LoadMatrix(oFD.cFileName);

      while (FindNextFile(hFind,&oFD))
        {
          if (!(oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            bRetval = LoadMatrix(oFD.cFileName);
        }

      FindClose(hFind);
    }

  return bRetval;
}



BOOL MatrixManager::LoadMatrix(LPCSTR cpFileName)
{
  BOOL bRetval = TRUE;

  CString cFileSpec = m_cMatrixDirectory + CString("\\") + CString(cpFileName);

  CFile oFile;

	CFileException fe;
  if (oFile.Open(cFileSpec,CFile::modeRead|CFile::shareDenyWrite,&fe))
    {
      CArchive ar(&oFile,CArchive::load|CArchive::bNoFlushOnDelete);

      Matrix *pMatrix = new Matrix;
      if (pMatrix)
        {
          pMatrix->SetFileName(cpFileName);

          TRY
          {
            pMatrix->Serialize(ar);
            ar.Close();
            pMatrix->m_bNewMatrix = FALSE;
            m_oMatrices.SetAt(pMatrix->GetMatrixName(),(CObject *)pMatrix);
          }
          CATCH_ALL(e)
          {
            e->ReportError(MB_OK|MB_ICONSTOP);
            bRetval = FALSE;
            delete pMatrix;
          }
          END_CATCH_ALL
        }
    }
  else
    {
      fe.ReportError(MB_OK|MB_ICONSTOP);
      bRetval = FALSE;
    }

  return bRetval;
}



BOOL MatrixManager::SaveMatrices()
{
  BOOL bRetval = TRUE;
  CString cDummy;
  Matrix *pMatrix;

  POSITION pPos = m_oMatrices.GetStartPosition();

  while (pPos && bRetval)
    {
      pMatrix = NULL;
      m_oMatrices.GetNextAssoc(pPos,cDummy,(CObject *&)pMatrix);
      if (pMatrix && pMatrix->IsModified())
        bRetval = SaveMatrix(pMatrix);
    }

  return bRetval;
}



BOOL MatrixManager::SaveMatrix(Matrix *pMatrix)
{
  BOOL bRetval = TRUE;

  CString cFileSpec = m_cMatrixDirectory + CString("\\") + pMatrix->GetFileName();

  CFile oFile;

	CFileException fe;
  if (oFile.Open(cFileSpec,CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive, &fe))
    {
      CArchive ar(&oFile,CArchive::store|CArchive::bNoFlushOnDelete);

      TRY
      {
        pMatrix->Serialize(ar);
        ar.Close();
        pMatrix->m_bNewMatrix = FALSE;
      }
      CATCH_ALL(e)
      {
        e->ReportError(MB_OK|MB_ICONSTOP);
        bRetval = FALSE;
      }
      END_CATCH_ALL
    }
  else
    {
      fe.ReportError(MB_OK|MB_ICONSTOP);
      bRetval = FALSE;
    }

  return bRetval;
}



void MatrixManager::SetMatrixDirectory(LPCSTR cpDirName)
{
  m_cMatrixDirectory = cpDirName;

  if (m_cMatrixDirectory.Right(1) == "\\")
    m_cMatrixDirectory = m_cMatrixDirectory.Left(m_cMatrixDirectory.GetLength()-1);

  // write to registry key

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
//      DWORD dwType;
      char caBuf[256];

      strcpy(caBuf,m_cMatrixDirectory);
      DWORD dwSize = strlen(caBuf);
      ::RegSetValueEx(hRegKey,cpGridDirectoryKey,NULL,REG_SZ,(unsigned char *)caBuf,dwSize);

      ::RegCloseKey(hRegKey);
    }
}



BOOL MatrixManager::RemoveMatrix(Matrix *pMatrix, BOOL bDeleteFile /*=TRUE*/)
{
  BOOL bRetval = FALSE;

  if (pMatrix)
    {
      CString cFileName = m_cMatrixDirectory + CString("\\") + pMatrix->GetFileName();
      if (m_oMatrices.RemoveKey(pMatrix->GetMatrixName()))
        {
          delete pMatrix;

          if (bDeleteFile)
            bRetval = ::DeleteFile(cFileName);
          else
            bRetval = TRUE;
        }
    }

  return bRetval;
}

