// EDStockDoc.cpp : implementation of the CEDStockDoc class
//

#include "stdafx.h"
#include "EDStock.h"

#include "EDStockDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEDStockDoc

IMPLEMENT_DYNCREATE(CEDStockDoc, CDocument)

BEGIN_MESSAGE_MAP(CEDStockDoc, CDocument)
	//{{AFX_MSG_MAP(CEDStockDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEDStockDoc construction/destruction

CEDStockDoc::CEDStockDoc()
{
	// TODO: add one-time construction code here

}

CEDStockDoc::~CEDStockDoc()
{
}

BOOL CEDStockDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEDStockDoc serialization

void CEDStockDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEDStockDoc diagnostics

#ifdef _DEBUG
void CEDStockDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEDStockDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEDStockDoc commands
