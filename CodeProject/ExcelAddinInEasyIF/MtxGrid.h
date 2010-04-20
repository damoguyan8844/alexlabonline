// MtxGrid.h: interface for the MatrixGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTXGRID_H__6D16485E_4B98_4FFE_A33D_37F394475B16__INCLUDED_)
#define AFX_MTXGRID_H__6D16485E_4B98_4FFE_A33D_37F394475B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __AFXOLE_H__
  #undef __AFXOLE_H__
#endif

#include <ugexcel.h>
#include "matrix.h"
#include "ugmedit.h"

class MatrixGrid : public CUGExcelCtrl
{
  HCURSOR m_hExcelArrow;
  HCURSOR m_hExcelWEResize;
  HCURSOR m_hExcelNSResize;

  CUGMaskedEdit m_oMaskedEdit;

  int m_iDateFormat;

  BOOL LastRowIsWildCards();
  void ValueToDateStr(long lValue, char *cpDateBuf);
  long DateStrToValue(LPCSTR cpDateBuf);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
  Matrix *m_pMatrix;

  int m_iParentType;

	MatrixGrid();
	virtual ~MatrixGrid();

	virtual void OnSetup();

	virtual void OnCharDown(UINT *vcKey,BOOL processed);
    virtual void OnKeyDown(UINT *vcKey, int processed);

    virtual int OnEditStart(int col, long row, CWnd **edit);
    virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);

    virtual int  OnCanSizeRow(long row) { return FALSE; }

    virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);

	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
};

#endif // !defined(AFX_MTXGRID_H__6D16485E_4B98_4FFE_A33D_37F394475B16__INCLUDED_)
