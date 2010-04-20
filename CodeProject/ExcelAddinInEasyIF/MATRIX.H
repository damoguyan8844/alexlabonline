#if !defined(_MATRIX_H_)
#define _MATRIX_H_

#define COMPAREOP_EQ   0
#define COMPAREOP_LT   1
#define COMPAREOP_LE   2
#define COMPAREOP_GT   3
#define COMPAREOP_GE   4
#define COMPAREOP_NE   5

#define COLTYPE_STRING    0
#define COLTYPE_INTEGER   1
#define COLTYPE_DOUBLE    2
#define COLTYPE_DATE      3
#define COLTYPE_FORMULA   4


class MatrixColDef : public CObject
{
  DECLARE_SERIAL(MatrixColDef)

public:

  WORD m_iColType;  // 0 = string, 1 = integer, 2 = double, 3 = date

  CString m_cColName;
  CString m_cNotes;

  BOOL m_bCompareCol;
  WORD m_iCompareOp; // 0 - =, 1 - <, 2 - <=, 3 - >, 4 - >=, 5 - <>

  BOOL m_bUsePickList;
  BOOL m_bRestrictToList;
  CStringArray m_oPickListChoices;

  MatrixColDef();

    // routines for serialization and debugging support ----------------------
  virtual void Serialize(CArchive & archive);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};



class MatrixColumnValue : public CObject
{
  DECLARE_SERIAL(MatrixColumnValue)

public:
  BOOL m_bIsWildCard;

  WORD m_iColType;  // 0 = string, 1 = integer, 2 = double, 3 = date

  CString m_cValue;
  double m_dValue;
  long m_lValue;

  MatrixColumnValue();
  MatrixColumnValue(MatrixColDef *pColDef, LPCSTR cpInitialValue = NULL);

  void SetColumnType(WORD iColType);
  WORD GetColumnType()  { return m_iColType; }

  void ValueFromString(LPCSTR cpValue);
  void ValueFromInteger(long lValue);
  void ValueFromDouble(double dValue);
  void ValueFromDate(long lValue);
  void ValueFromFormula(LPCSTR cpValue);

  BOOL IsWildCard()                         { return m_bIsWildCard; }
  void SetWildCard(BOOL bWildCard = TRUE)   { m_bIsWildCard = bWildCard; }

    // routines for serialization and debugging support ----------------------
  virtual void Serialize(CArchive & archive);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};


class Matrix;

class MatrixRow : public CObArray
{
  DECLARE_SERIAL(MatrixRow)

protected:
  MatrixRow() { ; }

public:

  MatrixColumnValue m_oReturnValue;

  MatrixRow(Matrix *pMatrix);
  virtual ~MatrixRow();

  MatrixColumnValue *AddColumn(MatrixColDef *pColDef, LPCSTR cpInitialValue = NULL);
  void InsertColumn(int iInsertBefore, MatrixColDef *pColDef, LPCSTR cpInitialValue = NULL);
  void DeleteColumn(int iColIndex);
  void SetColumnType(int iColIndex, WORD iColType);
  void MoveColumn(int iColIndex, int iDirection);

  MatrixColumnValue *GetCell(int iColIndex);
  MatrixColumnValue *GetReturnValue()    { return &m_oReturnValue; }


  long ComputeMatchWeight(MatrixRow *pRowToMatch, CObArray *pColDefs);
  long CalcColumnValue(int iColumn);

    // routines for serialization and debugging support ----------------------
  virtual void Serialize(CArchive & archive);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};



class Matrix : public CObject
{
  DECLARE_SERIAL(Matrix)

public:
  CString m_cFileName;

  CString m_cName;
  CString m_cNotes;
  CString m_cCreator;
  CString m_cCategory;

  WORD    m_iSecurityFlags;   // 1 = access psw req, 2 = modify psw req
  CString m_cAccessPassword;
  CString m_cModifyPassword;


  CObArray m_oColumnDefs;
  CObArray m_oRows;

  WORD m_iReturnValueColType;  // 0 = string, 1 = integer, 2 = double, 3 = date

  BOOL m_bModifiedFlag;
  WORD m_iSecurityPassedFlags;

  BOOL m_bNewMatrix;

public:
  Matrix(LPCSTR cpMatrixName = NULL);
  Matrix(LPCSTR cpMatrixName, Matrix *pMatrix);
  virtual ~Matrix();

  LPCSTR GetFileName()                   { return m_cFileName; }
  void   SetFileName(LPCSTR cpFileName)  { m_cFileName = cpFileName; }

  // accessors...........
  LPCSTR GetMatrixName()      { return m_cName; }
  void   SetMatrixName(LPCSTR cpMatrixName);

  LPCSTR GetMatrixNotes()      { return m_cNotes; }
  void   SetMatrixNotes(LPCSTR cpMatrixNotes)  { m_cNotes = cpMatrixNotes; SetModified(); }

  LPCSTR GetCreator()                 { return m_cCreator; }
  void   SetCreator(LPCSTR cpCreator) { m_cCreator = cpCreator; SetModified(); }

  LPCSTR GetCategory()                   { return m_cCategory; }
  void   SetCategory(LPCSTR cpCategory)  { m_cCategory = cpCategory; }

  BOOL   RequiresAccessPassword()     { return ((m_iSecurityFlags & 1) == 1); }
  BOOL   RequiresModifyPassword()     { return ((m_iSecurityFlags & 2) == 2); }

  void   SetRequiresAccessPassword(BOOL bReq)
      { if (bReq)  m_iSecurityFlags |= 1; else m_iSecurityFlags &= ~1; SetModified(); }
  void   SetRequiresModifyPassword(BOOL bReq)
      { if (bReq)  m_iSecurityFlags |= 2; else m_iSecurityFlags &= ~2; SetModified(); }

  LPCSTR GetAccessPassword()                  { return m_cAccessPassword; }
  void   SetAccessPassword(LPCSTR cpPassword) { m_cAccessPassword = cpPassword; SetModified(); }

  LPCSTR GetModifyPassword()                  { return m_cModifyPassword; }
  void   SetModifyPassword(LPCSTR cpPassword) { m_cModifyPassword = cpPassword; SetModified(); }

  BOOL IsModified()  { return m_bModifiedFlag; }
  void SetModified(BOOL bModified = TRUE)  { m_bModifiedFlag = bModified; }

  WORD GetReturnValueColumnType()               { return m_iReturnValueColType; }
  void SetReturnValueColumnType(WORD iColType);

  // operations..........
  MatrixColDef *GetColumnDef(int iColIndex);
  int           GetColumnCount()  { return m_oColumnDefs.GetSize(); }
  MatrixColDef *AddColumn(LPCSTR cpInitialValue = NULL);
  MatrixColDef *InsertColumn(int iInsertBefore, LPCSTR cpInitialValue = NULL);
  void          DeleteColumn(int iColIndex);
  void          SetColumnType(int iColIndex, WORD iColType);
  void          SetColumnType(MatrixColDef *pColDef, WORD iColType);
  void          MoveColumn(int iColIndex, int iDirection);

  int        GetRowCount()  { return m_oRows.GetSize(); }
  MatrixRow *AddRow();
  MatrixRow *InsertRow(int iInsertBefore);
  MatrixRow *GetRow(int iRowIndex);
  void       DeleteRow(int iRowIndex);

  MatrixColumnValue *GetCell(int iRowIndex, int iColIndex);

  void GetDateMask(char *cpMaskBuf);

  // actual pattern matching stuff...
  MatrixColumnValue *GetMatch(MatrixRow *pRowToMatch, MatrixRow **pBestMatch = NULL);

    // routines for serialization and debugging support ----------------------
  virtual void Serialize(CArchive & archive);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};


class MatrixManager
{
public:
  CString m_cMatrixDirectory;

  CMapStringToOb m_oMatrices;

  MatrixManager();
  virtual ~MatrixManager();

  Matrix *GetMatrix(LPCSTR cpMatrixName);

  BOOL LoadMatrices();
  BOOL LoadMatrix(LPCSTR cpFileName);
  BOOL SaveMatrices();
  BOOL SaveMatrix(Matrix *pMatrix);

  void SetMatrixDirectory(LPCSTR cpDirName);

  BOOL RemoveMatrix(LPCSTR cpMatrixName, BOOL bDeleteFile = TRUE)
          { return RemoveMatrix(GetMatrix(cpMatrixName),bDeleteFile); }

  BOOL RemoveMatrix(Matrix *pMatrix, BOOL bDeleteFile = TRUE);

  void DeleteAllMatrices();
};

#endif
